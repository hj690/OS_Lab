#ifndef _SCHEDULER
#define _SCHEDULER

#include <iostream>
#include <vector>
#include "Variable.h"
#include "Process.h"
#include "Buffer.h"


using namespace std;

class Scheduler{
protected:
	WaitingList waitingList;
	ReadyBuffer readyBuf;
	RunningBuffer runningBuf;
	BlockedBuffer blockBuf;
	FinishedBuffer finishBuf;
public:
	Scheduler(){}
	Scheduler(vector<Process> processes){
		this->waitingList = WaitingList(processes);
	}
	
	virtual Process selectNextProcess() = 0;//operater on readyBuf
	void run();
	void print();
	void showSize(){
		cout << "waiting: " << waitingList.getSize() << endl;
		cout << "ready: " << readyBuf.getSize() << endl;
		cout << "running: " << runningBuf.getSize() << endl;
		cout << "blocked: " << blockBuf.getSize() << endl;
		cout << "finished: " << finishBuf.getSize() << endl;
	}
	void showWL(){
		waitingList.show();
	}
	void showSum();

};

class Scheduler_FCFS : public Scheduler{
public:
	Scheduler_FCFS(vector<Process> processes){
		this->waitingList = processes;
	}
	Process selectNextProcess(){
		if(!readyBuf.isEmpty()){
			//pop the first element in vector
			return readyBuf.pop(readyBuf.getReadyBuf().front().getIndex());
		}
		else
			return Process();
	}

};

class Scheduler_LCFS : public Scheduler{
public:
	Scheduler_LCFS(vector<Process> processes){
		this->waitingList = processes;
	}
	Process selectNextProcess(){
		if(!readyBuf.isEmpty()){
			//pop the last element in vector
			return readyBuf.pop(readyBuf.getReadyBuf().back().getIndex());
		}
		else
			return Process();
	}
};

class Scheduler_SJF : public Scheduler{
public:
	Scheduler_SJF(vector<Process> processes){
		this->waitingList = processes;
	}
	Process selectNextProcess(){ //select the process with shortest remaining execution time
		if(!readyBuf.isEmpty()){
			vector<Process> processes = readyBuf.getReadyBuf();
			int index = readyBuf.getReadyBuf().front().getIndex();;
			int minRET = readyBuf.getReadyBuf().front().getRET();
			for(int i = 1; i < processes.size(); i++){
				if(processes[i].getRET() < minRET){
					index = processes[i].getIndex();
					minRET = processes[i].getRET();
				}
			}

			return readyBuf.pop(index);
		}
		else
			return Process();
	}
};

class Scheduler_RR : public Scheduler{
public:
	Scheduler_RR(vector<Process> processes, int quantum){
		for(vector<Process>::iterator it = processes.begin(); it != processes.end(); ++it)
			(*it).setQuantum(quantum);
		this->waitingList = processes;
	}

	Process selectNextProcess(){
		if(!readyBuf.isEmpty()){
			//pop the first element in vector
			return readyBuf.pop(readyBuf.getReadyBuf().front().getIndex());
		}
		else
			return Process();
	}
};
#endif