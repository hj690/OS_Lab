#include "IO.h"

int strToInt(string str){
		int result;
		stringstream convert(str);
		if ( !(convert >> result) )//give the value to Result using the characters in the string
	    	result = 0;//if that fails set Result to 0
	    return result;
}

Process convertLineToProcess(int index, string line){
	int parameters[4];
	char * str = new char[line.length() + 1];
	strcpy(str, line.c_str());
  	char * pch;
  	pch = strtok (str," ");
  	int pos = 0;
 	while (pch != NULL)
  	{
   		string s = string(pch);
   		parameters[pos++] = strToInt(s);
    	pch = strtok (NULL, " ");
  	}

  	return Process(index, parameters[0], parameters[1], parameters[2], parameters[3]);
}

queue<int> IO::getRandVals(string filename){
	queue<int> randvals;
	string line;
	bool firstLine = true;
	int size = 0;
	ifstream myfile(filename.c_str());
	if(myfile.is_open()){
		while( getline(myfile, line)){
			if(firstLine)
			{
				size = strToInt(line);
				firstLine = false;
			}
			else{
				if(line != "")
					randvals.push(strToInt(line));
			}
		}
		myfile.close();
	}
	if(size != randvals.size()){
		cout << "random numebr file format error!" << endl;
		cout << "size: " << size << " queue: " << randvals.size() << endl;
	}
	return randvals;
}

vector<Process> IO::getProcesses(string filename){
	vector<Process> processes;
	string line;
	int index = 0;
	ifstream myfile(filename.c_str());
	if(myfile.is_open()){
		while( getline(myfile, line)){ // a line is a process
			processes.push_back(convertLineToProcess(index, line));
			index++;
		}
		myfile.close();
	}
	return processes;
}

