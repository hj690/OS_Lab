#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>

using namespace std;

class request{
private:
	int index; //IO-op Num
	int addTime;
	int trackNum;
	int issueTime;
	int finishTime;
public:
	request(){this->index = -1;}
	request(int index, int addTime, int trackNum){
		this->index = index;
		this->addTime = addTime;
		this->trackNum = trackNum;
		this->issueTime = -1; //not issued
		this->finishTime = -1; //not finished
	}

	void setIssueTime(int issueTime){this->issueTime = issueTime;}
	void setFinishTime(int finishTime){this->finishTime = finishTime;}

	int getIndex(){return this->index;}
	int getaddTime(){return this->addTime;}
	int getTrackNum(){return this->trackNum;}
	int getIssueTime(){return this->issueTime;}
	int getFinishTime(){return this->finishTime;}
};

#endif