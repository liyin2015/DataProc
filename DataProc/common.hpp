#ifndef COMMON_HPP_
#define COMMON_HPP_
#include "stdafx.h"

//#include <iostrem>
#include <string>


using namespace std;

template<typename T> std::string transToStr(T in)
{
	ostringstream ss;
	ss<<in;
	string out = ss.str();
	return out;
}

CString  transFile2Path(CString cs);

string CStr2Str(CString cs);

CString generateText(char* str1, double value, char* str2);

//string getDefFileName(ALGO_TYPE type);

#endif