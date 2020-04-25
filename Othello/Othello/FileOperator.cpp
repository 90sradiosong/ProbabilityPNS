
#include "stdafx.h"

#include "FileOperator.h"

#include <fstream>
#include <iostream>

int writeIntoFile(std::string filename, std::string toBeWritten){
 int res = 0;

 ofstream writtenObject(filename.c_str(), ios::app);

 if (writtenObject.is_open()) {
	 writtenObject << toBeWritten.c_str();
	 res = 1;
	 cout << "\nresult written to file " << filename << endl;
 }

 writtenObject.close();
 return res;

}