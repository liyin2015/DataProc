#ifndef IO_MANAGER_HPP_
#define IO_MANAGER_HPP_
#include "comm.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream> //0stringsream
#include <string>

#include "matrix.hpp"
#include "math_function.hpp"
#include <log4cxx/logstring.h>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <locale.h>
//#include <log4cxx/consoleappender.h>
#include <log4cxx/basicconfigurator.h>
using namespace log4cxx;
using namespace log4cxx::helpers;

static log4cxx::LoggerPtr lir_logger(log4cxx::Logger::getLogger("LIR"));
static log4cxx::LoggerPtr lor_logger(log4cxx::Logger::getLogger("LOR"));
static log4cxx::LoggerPtr nn_logger(log4cxx::Logger::getLogger("NN"));
static log4cxx::LoggerPtr svm_logger(log4cxx::Logger::getLogger("SVM"));

static log4cxx::LoggerPtr hc_logger(log4cxx::Logger::getLogger("HCLUSTER"));//hierarchical clustering


static log4cxx::LoggerPtr apriori_logger(log4cxx::Logger::getLogger("APRIORI"));

//static log4cxx::LoggerPtr root_logger(log4cxx::Logger::getRootLogger());

//#define ML_ROOT_DEBUG(A)  LOG4CXX_DEBUG(root_logger, A)

#define ML_LIR_DEBUG(A)  LOG4CXX_DEBUG(lir_logger, A)
#define ML_LIR_INFO(A)   LOG4CXX_INFO(lir_logger, A)
#define ML_LIR_WARN(A)   LOG4CXX_WARN(lir_logger, A)
#define ML_LIR_ERROR(A)  LOG4CXX_ERROR(lir_logger, A)
#define ML_LIR_FATAL(A)  LOG4CXX_FATAL(lir_logger, A)

#define ML_LOR_DEBUG(A)  LOG4CXX_DEBUG(lor_logger, A)
#define ML_LOR_INFO(A)   LOG4CXX_INFO(lor_logger, A)
#define ML_LOR_WARN(A)   LOG4CXX_WARN(lor_logger, A)
#define ML_LOR_ERROR(A)  LOG4CXX_ERROR(lor_logger, A)
#define ML_LOR_FATAL(A)  LOG4CXX_FATAL(lor_logger, A)

#define ML_NN_DEBUG(A)  LOG4CXX_DEBUG(nn_logger, A)
#define ML_NN_INFO(A)   LOG4CXX_INFO(nn_logger, A)
#define ML_NN_WARN(A)   LOG4CXX_WARN(nn_logger, A)
#define ML_NN_ERROR(A)  LOG4CXX_ERROR(nn_logger, A)
#define ML_NN_FATAL(A)  LOG4CXX_FATAL(nn_logger, A)

#define ML_SVM_DEBUG(A)  LOG4CXX_DEBUG(svm_logger, A)
#define ML_SVM_INFO(A)   LOG4CXX_INFO(svm_logger, A)
#define ML_SVM_WARN(A)   LOG4CXX_WARN(svm_logger, A)
#define ML_SVM_ERROR(A)  LOG4CXX_ERROR(svm_logger, A)
#define ML_SVM_FATAL(A)  LOG4CXX_FATAL(svm_logger, A)

#define ML_HC_DEBUG(A)  LOG4CXX_DEBUG(hc_logger, A)
#define ML_HC_INFO(A)   LOG4CXX_INFO(hc_logger, A)
#define ML_HC_WARN(A)   LOG4CXX_WARN(hc_logger, A)
#define ML_HC_ERROR(A)  LOG4CXX_ERROR(hc_logger, A)
#define ML_HC_FATAL(A)  LOG4CXX_FATAL(hc_logger, A)

#define ML_AP_DEBUG(A)  LOG4CXX_DEBUG(apriori_logger, A)
#define ML_AP_INFO(A)   LOG4CXX_INFO(apriori_logger, A)

void init_log();

template<typename T>
int read_strfile(T& D, int& totalD, std::string filename)//in
{
	totalD=0;
	ifstream  inputfile(filename);

	if(!inputfile)  
	{   
		cout << "cannot open file."<<endl;   
		return -1;  
	}  
	
	char ch;   

	inputfile.get(ch);
	while(!inputfile.eof())  
	{
		std::ostringstream  ss;
		ss<<ch;
        if(ch=='\n')
		{
			totalD++;
		}
		inputfile.get(ch);
	}
	inputfile.close(); 
	totalD+=1;

	cout<<"totalD"<<totalD<<endl;
	D = DVector(totalD);

	//read the second time
	ifstream  in(filename);

	if(!in)  
	{   
		cout << "cannot open file."<<endl;   
		return -1;  
	}  

	int lines = 0;
	std::string str = "";
	in.get(ch);
	while(!in.eof())  
	{
		std::ostringstream  ss;
	    ss<<ch;
        if(ch=='\n') //transfer former input to num
        {
			D.AddOneItem((int)str2double(str.c_str()),lines);
			str.clear();
			lines++;      
        } 
        else if(ch==' '||ch == ',') 
        {
			D.AddOneItem((int)str2double(str.c_str()),lines);
			str.clear();
        }
		else if((ch>='0'&&ch <= '9'))
		{
			str.append(ss.str());
		}
		in.get(ch);
   }
   D.AddOneItem((int)str2double(str.c_str()),lines);
   in.close(); 
   return 0;
}



int read_input_file(Matrix& X,Matrix& Y,string filename);
int read_input_file_multiclass(Matrix& X,Matrix& Y,Matrix& Labels, string filename);

int read_file_no_rslt(Matrix& X,std::string filename);
int writePredict2File(Matrix& Y,std::string filename, int method);
void output(std::string log);
double getParaFromUser();
std::string getStrFromUser();
#endif