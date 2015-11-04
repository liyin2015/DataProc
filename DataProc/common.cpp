#include "common.hpp"
#include <iostream>
#include <sstream>
using namespace std;

CString transFile2Path(CString cs)
{
	CString re=cs;
	int srt_pos = 0;
	int find_pos = -1;
	while(1)
	{
		find_pos =re.Find('\\',srt_pos);
		if(find_pos ==-1)
		{
			return re;
		}
		re.Insert(find_pos,'\\');
		srt_pos=find_pos+2;
	}
	return re;
}
string CStr2Str(CString cs)
{
	string re = "";
	for(int i=0;i<cs.GetLength();i++)
	{
		char c=(char)cs.GetAt(i);
		re.append(transToStr(c));
	}
	return re;
}
CString generateText(char* str1, double value, char* str2)
{
	std::string str = str1;
	str.append(transToStr(value));
	str.append(str2);
	wstring str3(str.begin(),str.end());

	CString cstr = str3.c_str();
	return cstr;
}
/*string getDefFileName(ALGO_TYPE algo_type)
{
	string name = "";
	if(algo_type == LINEAR_REG_TYPE)
	{
		name = "data\\imput.txt";
	}
	else if(algo_type == LOGISTIC_REG_TYPE)
	{
		name = "data\\imput.txt";
	}
	else if(algo_type == NEURAL_NET_TYPE)
	{
		name = "data\\neural_input.txt";
	}
	else if(algo_type == SVM_TYPE)
	{
		name = "data\\svm.txt";
	}
	return name;
}*/