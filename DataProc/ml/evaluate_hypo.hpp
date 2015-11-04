#ifndef EVALUATE_HYPO_
#define EVALUATE_HYPO_
#include <iostream>
#include "matrix.hpp"

using namespace std;
class RSLT_EVAL
{
public:
	RSLT_EVAL();
	~RSLT_EVAL();
	friend ostream& operator<<(ostream& out,const RSLT_EVAL& m);
	double getAccuracy(Matrix hypo, Matrix y);//can for train or test
	//double get   //can for train or test
	void getPrecision(Matrix hypo, Matrix y, Matrix label);
	
public:
	double train_accuracy;
	double test_accuracy;
	double precision;
	double recall;
	double PR;
	double Jtest;
	double Jtrain;
	double Jcv;//cross-validation

	int ac1_pre1;//11
	int ac1_pre0;//10
	int ac0_pre1;
	int ac0_pre0;
};


//typedef double(*pFunc)(Matrix, Matrix);
template <typename T>
void Jthree(T& t, double(T::*pFunc)(Matrix&, Matrix&), Matrix& hypo, Matrix y, RSLT_EVAL& rslt, double train_rate, double cv_rate, double test_rate)
{
	int m = y.length();
	int train_n= (int)(train_rate*m);
	Matrix y_train = y.getRow(0,train_n-1);
	Matrix hypo_train = hypo.getRow(0,train_n-1);

	int cv_n = (int)((train_rate+cv_rate)*m);
	int cv_m = cv_n-train_n;
	Matrix y_cv =y.getRow(train_n,cv_n-1);
	Matrix hypo_cv =hypo.getRow(train_n,cv_n-1);

	int test_m = m-cv_n;
	Matrix y_test =y.getRow(cv_n,m-1);
	Matrix hypo_test =hypo.getRow(cv_n,m-1);

	double Jtrain = (t.*pFunc)(hypo_train,y_train);
	double Jcv = (t.*pFunc)(hypo_cv,y_cv);
	double Jtest = (t.*pFunc)(hypo_test,y_test);

	rslt.Jtrain=Jtrain;
	rslt.Jcv=Jcv;
	rslt.Jtest =Jtest;
	return;
}
bool isSkew(Matrix y);
//double getAccuracy(Matrix hypo, Matrix y);
//void getAccuracy(Matrix hypo, Matrix y, RSLT_EVAL& rslt);

//RSLT_EVAL getRsltStruct(Matrix hypo, Matrix y,double train_rate, double cv_rate, double test_rate);
//ostream& operator<<(ostream& out, RSLT_EVAL& rslt);
#endif