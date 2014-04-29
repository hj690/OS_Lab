#ifndef IOQ_H
#define IOQ_H

#include <iostream>
#include <queue>
#include <vector>
#include "request.h"
#include "variables.h"

using namespace std;

class IOQ{
protected:
	queue<request> readyQ;
	vector<request> finishedQ;
	request running;
	int track;
public:
	IOQ(){
		track = 0;
	}
	IOQ(string filename){
		track = 0;
		getAllRequests(filename);
	}

	void getAllRequests(string filename); 
	void show();
	bool ifNewQeq();
	bool ifRunningOneFinish();
	void process();
	void printSum();
	virtual bool ifAllFinish(){return true;};
	virtual void addNewReq(){}; //add a new request into addedQ
	virtual request getNextReq(){return request();}; //schedule another IO request


};

class IOQ_FIFO : public IOQ{
private:
	queue<request> addedQ;
public:
	IOQ_FIFO(string filename){getAllRequests(filename);}
	void addNewReq(); 
	request getNextReq();
	bool ifAllFinish();
};

class IOQ_SSTF : public IOQ{
private:
	vector<request> addedQ;
public:
	IOQ_SSTF(string filename){getAllRequests(filename);}
	void addNewReq();
	request getNextReq();
	bool ifAllFinish();
};

class IOQ_SCAN : public IOQ{
private:
	vector<request> addedQ;
	bool asscend;
public:
	IOQ_SCAN(){}
	IOQ_SCAN(string filename){
		getAllRequests(filename);
		asscend = true;
	}
	void resetAsscend(){this->asscend = true;}
	void addNewReq();
	void addNewReq(request req);
	vector<request> getAddedQ(){return this->addedQ;} //used for FSCAN
	request getNextReq(int track); //used for FSCAN
	request getNextReq();
	bool ifAllFinish();
};

class IOQ_CSCAN : public IOQ{
private:
	vector<request> addedQ;
public:
	IOQ_CSCAN(string filename){
		getAllRequests(filename);
	}
	void addNewReq();
	request getNextReq();
	bool ifAllFinish();
};

class IOQ_FSCAN : public IOQ{
private:
	IOQ_SCAN scan1;
	IOQ_SCAN scan2;
	int ongoing; 
public:
	IOQ_FSCAN(string filename){
		getAllRequests(filename);
		scan1 = IOQ_SCAN();
		scan2 = IOQ_SCAN();
		ongoing = 2; // initially, neigher Q is ongoing
	}
	bool ifRunningOneFinish();
	void addNewReq();
	request getNextReq();
	bool ifAllFinish();
};

#endif