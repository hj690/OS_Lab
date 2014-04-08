#ifndef _BUFFER
#define _BUFFER
#include <vector>
#include <iomanip>
#include "Process.h"
#include "Variable.h"
#include "RandVals.h"

class WaitingList{
private:
	vector<Process> list;
public:
	WaitingList(){}
	WaitingList(vector<Process> processes){
		this->list = processes;
	}
	int getSize(){return list.size();}
	bool isEmpty(){return list.empty();}
	vector<Process> getAvailableProcesses(){
		vector<Process> processes;
		vector<Process>::iterator it = list.begin();

		for(int i = 0; i < list.size(); i++){
			if(list[i].getAT() == timer){
				list[i].setState(Ready);
				list[i].setEventStartTime(timer);
				processes.push_back(list[i]); //*it: dereference
				list.erase(list.begin() + i);
				i--;
			}
		}
		return processes;
	}
	void show(){
		for(int i = 0; i < list.size(); i++){
			cout << list[i].getIndex() << " " << list[i].getAT() << " " << list[i].getTC() << " " << list[i].getCB() << " " << list[i].getIO() << endl;
		}
		
	}
};

class RunningBuffer{  
private:
	vector<Process> runner;
	int totalRunningTime;
public:
	RunningBuffer(){
		totalRunningTime = 0;
	}
	int getSize(){return runner.size();}
	int getTotalRunningTime(){return this->totalRunningTime;}
	bool isEmpty(){return runner.size() == 0;}
	void push(Process p){
		if(isEmpty()){//running buf is empty, ok to add
			this->runner.push_back(p);
		}
		else{
			cout << "Error: Running Buffer outflow! " << endl;
		}
	}

	//pop the process if CPU burst is over(or quantum is over)
	Process pop(){
		if(runner.size() == 1){
			Process p = runner.front();
			// if process is finished, will go to finish
			if(p.getEventStartTime() + p.getRET() == timer){
				totalRunningTime = timer - p.getEventStartTime() + totalRunningTime;
				p.setFT(timer);
				p.setTT(timer - p.getAT());
				p.setRET(0);
				p.setState(Finished);
				this->runner.clear();
				return p;
			}
			// if CPU burst runs out, will go to blocked
			if(p.getEventStartTime() + p.getRemainCB() == timer ){ 
				totalRunningTime = timer - p.getEventStartTime() + totalRunningTime;
				p.setRemainCB(0);
				p.setRET(p.getRET() - (timer - p.getEventStartTime()));
				p.setRemainIO(randNum.myRandom(p.getIO()));
				p.setEventStartTime(timer);
				p.setState(Blocked);
				this->runner.clear();
				return p;
			}
			// if it's RR process and quantum runs out, will go to ready
			if(p.getQuantum() != -1 && p.getEventStartTime() + p.getQuantum() == timer ){
				totalRunningTime = timer - p.getEventStartTime() + totalRunningTime;
				p.setRemainCB(p.getRemainCB() - (timer - p.getEventStartTime()));
				p.setRET(p.getRET() - (timer - p.getEventStartTime()));
				p.setEventStartTime(timer);
				p.setState(Ready);
				this->runner.clear();
				return p;
			}
		}
	
		return Process(); 	//this process: index = -1. means null
	}
	


};

class BlockedBuffer{
private:
	vector<Process> IObuf;
	int totalIOTime;
	int lastOnTime;

public:
	BlockedBuffer(){
		totalIOTime = 0;
		lastOnTime = 0;
	}
	int getSize(){return IObuf.size();}
	int getTotalIoTime(){return this->totalIOTime;}
	bool isEmpty(){return IObuf.size() == 0;}
	void push(Process p){
		if(IObuf.size() == 0)
			lastOnTime = timer;
		IObuf.push_back(p);
	}
	vector<Process> pop(){//pop out the processes which have finished IO burst
		vector<Process> processes;
		if(!isEmpty()){
			for(int i = 0; i < IObuf.size(); i++){
				Process tmp = IObuf[i];
				//if IO finished, will go to ready
				if(tmp.getEventStartTime() + tmp.getRemainIO() == timer){
					tmp.setIT(tmp.getIT() + timer - tmp.getEventStartTime());
					tmp.setRemainIO(0);
					tmp.setEventStartTime(timer);
					tmp.setState(Ready);
					processes.push_back(tmp);
					this->IObuf.erase(IObuf.begin() + i);
					i--;
				}
			}

			if(IObuf.size() == 0){
				totalIOTime += (timer - lastOnTime);
			}
		}
		return processes;
	}


};

class ReadyBuffer{
private:
	vector<Process> ReadyBuf;
public:
	ReadyBuffer(){}
	vector<Process> getReadyBuf(){return this->ReadyBuf;}
	bool isEmpty(){return ReadyBuf.size() == 0;}
	int getSize(){return ReadyBuf.size();}
	void push(vector<Process> processes){
		for(vector<Process>::iterator it = processes.begin(); it != processes.end(); it++){
			ReadyBuf.push_back(*it);
		}
	}
	void push(Process p){
		ReadyBuf.push_back(p);
	}
	Process pop(int index){ //index is the index of the process, not index in vector
		for(int i = 0; i < ReadyBuf.size(); i++){
			Process tmp = ReadyBuf[i];
				if(tmp.getIndex() == index){
					if(tmp.getRemainCB() == 0){
						tmp.setRemainCB(randNum.myRandom(tmp.getCB()));
					}

					tmp.setCW(tmp.getCW() + timer - tmp.getEventStartTime());
					tmp.setEventStartTime(timer);
					tmp.setState(Running);
					Process p = (tmp);
					this->ReadyBuf.erase(ReadyBuf.begin() + i);
					i--;
					return p;
				}
			}
	}
	
};

class FinishedBuffer{
private:
	vector<Process> finishBuf;
	int lastFinishTime;
public:
	FinishedBuffer(){}
	void push(Process p){
		lastFinishTime = timer;
		this->finishBuf.push_back(p);
	}
	vector<Process> getFinishBuf(){
		return this->finishBuf;
	}
	int getSize(){return this->finishBuf.size();}
	int getLastFinishTime(){return this->lastFinishTime;}
	float getAverageTT(){
		int totalTime = 0;
		for(int i = 0; i < finishBuf.size(); i++){
			totalTime += finishBuf[i].getTT();
		}
		return (float)totalTime/finishBuf.size();
	}
	float getAverageCW(){
		int totalTime = 0;
		for(int i = 0; i < finishBuf.size(); i++){
			totalTime += finishBuf[i].getCW();
		}
		return (float)totalTime/finishBuf.size();
	}
	float getSpace(){
		return (float)finishBuf.size() * 100 / lastFinishTime;
	}
	void print(){
		//sort buffer according to the index

		for(int i = 1; i < finishBuf.size(); i++){
			Process tmp = finishBuf[i];
			int j = i;
			while(j > 0 && finishBuf[j-1].getIndex() > tmp.getIndex()){
				 finishBuf[j] =  finishBuf[j - 1];
				 j--;
			}
			finishBuf[j] = tmp;
		}

		for(vector<Process>::iterator it = finishBuf.begin(); it != finishBuf.end(); ++it){
			cout << setfill('0') << setw(4) << (*it).getIndex(); 
			cout << ": " << setfill(' ') << setw(4) << (*it).getAT() << " " << setw(4) << (*it).getTC() << " "<< setw(4) << (*it).getCB() << " "<< setw(4) << (*it).getIO()
				<< " | " << setw(4)<< (*it).getFT() << " "<< setw(4) << (*it).getTT() << " "<< setw(4) << (*it).getIT() << " "<< setw(4) << (*it).getCW() << endl;
		}
	}
};

#endif