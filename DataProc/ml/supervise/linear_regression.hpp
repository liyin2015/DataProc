#ifndef LINEAR_REGRESSION_HPP_
#define LINEAR_REGRESSION_HPP_

#include <iostream>
#include "../matrix.hpp"
#include "../math_function.hpp"
#include "../io_manager.hpp"
#include "../algo_basic.h"


//for linear regression, no preprocessing like normalize, but can have scalling
using namespace std;

//used for logistic regress
typedef enum
{
	BASIC_LINEAR=0,
}LINEAR_REG_TRAIN_METHOD;

class LinReg:public Algo_Basic
{
public:
	LinReg();
	~LinReg();
	int loadData(string filename, bool bRand, ALGO_TYPE algo_type = LINEAR_REG_TYPE);
	void setRates(RATE_PARA pa);
	void setConstrainPara(int num_ite,double al,double lam = 0)
	{
		para.num_iters = num_ite;
		para.alpha = al;
		para.lambda = lam;
	}
	void initWeight()
	{
		theta = Matrix(X_train.brandth(),1);
		ML_LIR_INFO("init theta"<<theta);
	}
	bool trainLinear(LINEAR_REG_TRAIN_METHOD method)
	{
		bool rslt = false;
		initWeight();
		switch(method)
		{
		case BASIC_LINEAR:
			rslt = gradientDescent_basic();
			break;
		default:
			return false;
		}
		return rslt;
		
	}

	double computeCostFunc(Matrix& h,Matrix& y);//sigmoid h
	Matrix computeGradient(Matrix X);
	Matrix predictLinear(Matrix p_x);
	bool gradientDescent_basic();
	//viusal data
	//set train para
	void setTrainPara(SUPERVISE_PARA, int index =0);
	void setPreProcessPara(PRE_PROCESS_PARA p, int index =0);
	DECISION_PARAS getDesionBoundPara(int index = 0);
	MIN_COST_PARA_RECORD getMinCostPara(int index=0);

	bool trainData(int index =0);

	XY_PARA predict(std::string filename ="");
	Matrix predict(Matrix x);

	void getEvalRslt();

public:
	Matrix theta; //can improve it to multi-class
	MIN_COST_PARA_RECORD minCostRecord;

	int max_iters;

	int inc_times;
	double J_converge;
	LINEAR_REG_TRAIN_METHOD train_method;
    //this is used to minotor the whole process in the training
};

#endif
