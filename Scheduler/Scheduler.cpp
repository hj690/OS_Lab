#include "Scheduler.h"
#include "Variable.h"

void Scheduler::run(){
	vector<Process> buf;
	Process tmpProcess;
	while(!waitingList.isEmpty() || !readyBuf.isEmpty() || !runningBuf.isEmpty() || !blockBuf.isEmpty()){
		//check waiting list
		if(!waitingList.isEmpty()){
			buf = waitingList.getAvailableProcesses();
			if(buf.size() > 0){
				readyBuf.push(buf);
			}
		}

		//check blocked buffer
		if(!blockBuf.isEmpty()){
			buf = blockBuf.pop();
			if(buf.size() > 0){
				readyBuf.push(buf);
			}
		}
		
		//check running buffer
		if(!runningBuf.isEmpty()){
			tmpProcess = runningBuf.pop();

			if(tmpProcess.getIndex() != -1){ // index = -1 when process is still running
				switch(tmpProcess.getState()){
				case Finished:
					finishBuf.push(tmpProcess);
					break;
				case Blocked:
					blockBuf.push(tmpProcess);
					break;
				case Ready:
					readyBuf.push(tmpProcess);
					break;
				}
			}
		}
		
		// running buffer is empty, scheduler select another one
		if(runningBuf.isEmpty()){ 
			tmpProcess = selectNextProcess();
			if(tmpProcess.getIndex() != -1){
				runningBuf.push(tmpProcess);
			}
		}

		timer++;
		
	}
	print();
//	showSize();
}

void Scheduler::print(){
	if(finishBuf.getSize() > 0){
		finishBuf.print();

	}
}

void Scheduler::showSum(){
	int lastFinishTime = finishBuf.getLastFinishTime();
	float CPUUtilization = (float)100*runningBuf.getTotalRunningTime()/lastFinishTime;
	float IOUtilization = (float)100*blockBuf.getTotalIoTime()/lastFinishTime;
	float averageTT = finishBuf.getAverageTT();
	float averageCW = finishBuf.getAverageCW();
	float space = finishBuf.getSpace();

	printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n", 
		lastFinishTime, CPUUtilization, IOUtilization, averageTT, averageCW, space);
}