#ifndef _PROCESS
#define _PROCESS

#include <iostream>
#include <fstream>

using namespace std;

//empty means before entering the ready queue
//finished means fisihed its CPU time
enum State {Ready, Running, Blocked, Finished, Waiting};

class Process{
private:
	int index;//the index in file
	int AT; //arrival time
	int TC; //total time
	int CB; //CPU burst, need to be computed the first time enter Running State
	int IO; //IO burst, need to be computed the every time enter Blocked State
	int remainingCB;
	int remainingIO;
	int RET;//remaining execution time

	int FT; //finishing time
	int TT; //turnaround time
	int IT; //IO time
	int CW; //CPU waiting time

	State state; //the state of the process
	int eventStartTime;

	//for Round Robin only
	int quantum;
//	int remainingQuantum;
	

public:
	Process(){
		this->index = -1;
	}
	Process(int index, int at, int tc, int cb, int io){
		this->index = index;
		this->AT = at;
		this->TC = tc;
		this->CB = cb;
		this->IO = io;
		this->RET = tc; //when instantiate, remaining execution time is total cpu time
		this->state = Waiting;
		this->TT = 0;
		this->FT = 0;
		this->IT = 0;
		this->CW = 0;
		this->remainingCB = 0;
		this->remainingIO = 0;
		this->quantum = -1;
	}

	void setRET(int ret){this->RET = ret;}
	void setFT(int ft){this->FT = ft;}
	void setTT(int tt){this->TT = tt;}
	void setIT(int it){this->IT = it;}
	void setCW(int cw){this->CW = cw;}
	void setState(State state){this->state = state;}
	void setEventStartTime(int time){this->eventStartTime = time;}
	void setRemainCB(int cb){this->remainingCB = cb;}
	void setRemainIO(int io){this->remainingIO = io;}
	void setQuantum(int quantum){this->quantum = quantum;}
//	void setRemainQuantum(int quantum){this->remainingQuantum = quantum;}

	int getIndex(){return this->index;}
	int getAT(){return this->AT;}
	int getTC(){return this->TC;}
	int getCB(){return this->CB;}
	int getIO(){return this->IO;}
	int getRET(){return this->RET;}
	int getFT(){return this->FT;}
	int getTT(){return this->TT;}
	int getIT(){return this->IT;}
	int getCW(){return this->CW;}
	State getState(){return this->state;}
	int getEventStartTime(){return this->eventStartTime;}
	int getRemainCB(){return this->remainingCB;}
	int getRemainIO(){return this->remainingIO;}
	int getQuantum(){return this->quantum;}
//	int getRemainQuantum(){return this->remainingQuantum;}
	

};


#endif