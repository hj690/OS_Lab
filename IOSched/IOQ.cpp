#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "IOQ.h"


pair<int,int> parseLine(string line){
	pair<int, int> instr;
	int i=0;
	const char *delimiter = " ";
	char *p;
	char *cstr = new char[line.size() + 1];
	strcpy(cstr, line.c_str());
	p = strtok(cstr, delimiter);
	while(p && i < 2){
		string str(p); //convert char* to string
		if(str.at(0) == '#'){ //invalid instruction, will not be processed
			instr.first = -1;
			instr.second = -1;
		}
		else{//valid instruction, will be processed
			if(i == 0)
				instr.first = atoi(str.c_str());
			else //i==1
				instr.second = atoi(str.c_str());
		}
		i++;
		p = strtok(NULL, delimiter);
	}
	return instr;

}

// IOQ ====================================================================================

void IOQ::getAllRequests(string filename){
	pair<int, int> instr;
	int index = 0;
	string line;
	ifstream myfile(filename.c_str());
	if(myfile.is_open()){
		while( getline(myfile, line)){
			if(line.length() > 0){
				instr = parseLine(line);
				if(instr.first != -1){ //valid request
					readyQ.push(request(index, instr.first, instr.second));
					index++;
				}
			}
		}
		myfile.close();
		
	}
}

void IOQ::show(){
	while(!readyQ.empty()){
		request req = readyQ.front();
		cout << req.getIndex() << " " << req.getaddTime()
			<< " " << req.getTrackNum() << " " << req.getIssueTime() 
			<< " " << req.getFinishTime() << endl;
		readyQ.pop();
	}
}

bool IOQ::ifNewQeq(){
	if(!readyQ.empty())
		return readyQ.front().getaddTime() <= timer;
	return false;
}

bool IOQ::ifRunningOneFinish(){
	if(running.getIndex() == -1)
		return true;
	else if(running.getFinishTime() <= timer){
		//cout << timer << ": " << running.getIndex() << " finish " << timer - running.getaddTime() << endl;
		return true;
	}
	else
		return false;

}

void IOQ::printSum(){
	int total_time = timer - 1;
	int tot_movement = 0;
	double tot_turnaround = 0;
	double tot_waittime = 0;
	int max_waittime = 0;
	int size = finishedQ.size();
	int lastTrack = 0;
	for(vector<request>::iterator it = finishedQ.begin(); it != finishedQ.end(); ++it){
		tot_movement += abs(it->getTrackNum() - lastTrack);
		lastTrack = it->getTrackNum();
		tot_turnaround += (it->getFinishTime() - it->getaddTime());
		tot_waittime += it->getIssueTime() - it->getaddTime();
		if(it->getIssueTime() - it->getaddTime() > max_waittime)
			max_waittime = it->getIssueTime() - it->getaddTime();
	}
	
	double avg_turnaround = tot_turnaround/size;
	double avg_waittime = tot_waittime/size;

	//cout << "finish size: " << finishedQ.size() << endl;
	printf("SUM: %d %d %.2lf %.2lf %d\n", 
	total_time, 
	tot_movement, 
	avg_turnaround, 
	avg_waittime, 
	max_waittime); 
}

void IOQ::process(){

	while(!ifAllFinish()){
		//if there's new request can be added
		while(ifNewQeq()){
			addNewReq();
		}
		//if the running request finish, issue a new one

		if(ifRunningOneFinish()){

			if(timer == 935)
				cout << "";
			//get current track #
			if(running.getIndex() != -1){
				track = running.getTrackNum(); //track has already move to this position
				finishedQ.push_back(running);
			}
			request req = getNextReq();
			req.setIssueTime(timer);
			req.setFinishTime(timer + abs(track - req.getTrackNum()));
			running = req;
			if(running.getIndex() != -1 && running.getTrackNum() == track)
				timer--;
		}

		timer++;

	}
	printSum();
}

// IOQ_FIFO ====================================================================================

bool IOQ_FIFO::ifAllFinish(){
	return readyQ.empty() && addedQ.empty() && (running.getIndex() == -1);
}

void IOQ_FIFO::addNewReq(){
	request req = readyQ.front();
	addedQ.push(req);
	readyQ.pop();
	//cout << timer << ": " << req.getIndex() << " add " << req.getTrackNum() << endl;

}

request IOQ_FIFO::getNextReq(){
	if(!addedQ.empty()){
		request req = addedQ.front();
		addedQ.pop();
		//cout << timer << ": " << req.getIndex() << " issue " << req.getTrackNum() << endl;
		return req;
	}
	return request();
}


// IOQ_SSTF ====================================================================================

void IOQ_SSTF::addNewReq(){
	request req = readyQ.front();
	addedQ.push_back(req);
	readyQ.pop();
	//cout << timer << ": " << req.getIndex() << " add " << req.getTrackNum() << endl;
}

bool IOQ_SSTF::ifAllFinish(){
	return readyQ.empty() && addedQ.empty() && (running.getIndex() == -1);
}

request IOQ_SSTF::getNextReq(){
	if(!addedQ.empty()){
		request req = addedQ[0];
		int min = 0;
		for(int i = 1; i < addedQ.size(); i++){
			if(abs(req.getTrackNum() - track) > abs(addedQ[i].getTrackNum() - track)){
				min = i;
				req = addedQ[i];
			}
		}
		//cout << timer << ": " << req.getIndex() << " issue " << req.getTrackNum() << endl;
		addedQ.erase(addedQ.begin() + min);
		return req;
	}
	return request();
}

// IOQ_SCAN ====================================================================================

bool IOQ_SCAN::ifAllFinish(){
	return readyQ.empty() && addedQ.empty() && (running.getIndex() == -1);
}

void IOQ_SCAN::addNewReq(){
	request req = readyQ.front();
	int size = addedQ.size();

	//insert the new request in order of track #
	if(addedQ.empty())
		addedQ.push_back(req);
	else{
		int i = 0;
		for(; i < size; i++){
			if(addedQ[i].getTrackNum() > req.getTrackNum())
				break;
		}

		addedQ.insert(addedQ.begin() + i, req);
	}

	readyQ.pop();
	//cout << timer << ": " << req.getIndex() << " add " << req.getTrackNum() << endl;
}


void IOQ_SCAN::addNewReq(request req){ //used for FSCAN
	int size = addedQ.size();

	//insert the new request in order of track #
	if(addedQ.empty())
		addedQ.push_back(req);
	else{
		int i = 0;
		for(; i < size; i++){
			if(addedQ[i].getTrackNum() > req.getTrackNum())
				break;
		}
		addedQ.insert(addedQ.begin() + i, req);
	}

	//cout << timer << ": " << req.getIndex() << " add " << req.getTrackNum() << endl;
}

request IOQ_SCAN::getNextReq(int track){
	this->track = track;
	return getNextReq();
}

request IOQ_SCAN::getNextReq(){
	if(!addedQ.empty()){
		request req = request();
		int index;
		if(asscend){
			for(int i = 0; i < addedQ.size(); i++){
				if(addedQ[i].getTrackNum() >= track){
					index = i;
					req = addedQ[i];
					break;
				}
			}

			if(req.getIndex() == -1){
				asscend = false;
				for(int i = addedQ.size() - 1; i >=0; i--){
					if(( addedQ[i].getTrackNum() <= track && i == 0 )
						|| (addedQ[i].getTrackNum() <= track && addedQ[i-1].getTrackNum() < track)){
							index = i;
							req = addedQ[i];
							break;
					}
				}
			}
		}

		else{
			for(int i = addedQ.size() - 1; i >=0; i--){
				if(( addedQ[i].getTrackNum() <= track && i == 0 )
					|| (addedQ[i].getTrackNum() <= track && addedQ[i-1].getTrackNum() < track)){
						index = i;
						req = addedQ[i];
						break;
				}
			}
			if(req.getIndex() == -1){
				asscend = true;
				for(int i = 0; i < addedQ.size(); i++){
					if(addedQ[i].getTrackNum() >= track){
						index = i;
						req = addedQ[i];
						break;
					}
				}
			}
		}

		//cout << timer << ": " << req.getIndex() << " issue " << req.getTrackNum() << endl;
		addedQ.erase(addedQ.begin() + index);
		return req;
	}
	return request();
}

// IOQ_CSCAN ====================================================================================

bool IOQ_CSCAN::ifAllFinish(){
	return readyQ.empty() && addedQ.empty() && (running.getIndex() == -1);
}

void IOQ_CSCAN::addNewReq(){
	request req = readyQ.front();
	int size = addedQ.size();

	//insert the new request in order of track #
	if(addedQ.empty())
		addedQ.push_back(req);
	else{
		int i = 0;
		for(; i < size; i++){
			if(addedQ[i].getTrackNum() > req.getTrackNum())
				break;
		}

		addedQ.insert(addedQ.begin() + i, req);
	}

	readyQ.pop();
	//cout << timer << ": " << req.getIndex() << " add " << req.getTrackNum() << endl;
}

request IOQ_CSCAN::getNextReq(){
	if(!addedQ.empty()){
		int index;
		request req = request();
		for(int i = 0; i < addedQ.size(); i++){
			if(addedQ[i].getTrackNum() >= track){
				index = i;
				req = addedQ[i];
				break;
			}
		}
		if(req.getIndex() == -1){
			req = addedQ[0];
			index = 0;
		}

		//cout << timer << ": " << req.getIndex() << " issue " << req.getTrackNum() << endl;
		addedQ.erase(addedQ.begin() + index);
		return req;
	}
	return request();
}

// IOQ_FSCAN ====================================================================================

bool IOQ_FSCAN::ifAllFinish(){
	return readyQ.empty() && scan1.getAddedQ().empty() 
		&& scan2.getAddedQ().empty()  && (running.getIndex() == -1);
}

void IOQ_FSCAN::addNewReq(){
	request req = readyQ.front();
	
	if(ongoing == 2){
		scan1.addNewReq(req);
		scan1.resetAsscend();
	}
	else if(ongoing == 1){
		scan2.addNewReq(req);
		scan2.resetAsscend();
	}
	readyQ.pop();
	//cout << "scan1: " << scan1.getAddedQ().size() << endl;
	//cout << "scan2: " << scan2.getAddedQ().size() << endl;
}

bool IOQ_FSCAN::ifRunningOneFinish(){
	if(running.getIndex() == -1)
		return true;
	else if(running.getFinishTime() <= timer){
		if(ongoing == 1 && scan1.getAddedQ().empty()){
			scan1.resetAsscend();
			ongoing = 2;
		}
		if(ongoing == 2 && scan2.getAddedQ().empty()){
			scan2.resetAsscend();
			ongoing = 1;
		}
		//cout << timer << ": " << running.getIndex() << " finish " << timer - running.getaddTime() << endl;
		return true;
	}
	else
		return false;

}


request IOQ_FSCAN::getNextReq(){
	if(!scan1.getAddedQ().empty() && scan2.getAddedQ().empty()){
		ongoing = 1;
	}
	else if(scan1.getAddedQ().empty() && !scan2.getAddedQ().empty()){
		ongoing = 2;
	}
	if(ongoing == 1)
		return scan1.getNextReq(track);
	else if(ongoing == 2) 
		return scan2.getNextReq(track);
	
	return request();
	
}
