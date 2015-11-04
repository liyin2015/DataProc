#ifndef LOGISTIC_REGRESSION_HPP_
#define LOGISTIC_REGRESSION_HPP_
#include "../matrix.hpp"
#include "../math_function.hpp"
#include "../io_manager.hpp"
#include "../feature_scalling.hpp"
#include "../algo_basic.h"


//used for logistic regress
typedef enum
{
	BASIC=0,
	STOC=1,
}LG_TRAIN_METHOD;



class LogReg:public Algo_Basic
{
public:
	LogReg();
	~LogReg();
	int loadData(string filename, bool bRand, ALGO_TYPE algo_type = LOGISTIC_REG_TYPE);
	void setRates(RATE_PARA pa);
	void setConstrainPara(int num_ite,double al,double lam = 0);
	void initWeight(int index=0);
	void trainLog(LG_TRAIN_METHOD method);

	double computeCostFunc(Matrix& h,Matrix& y);//sigmoid h
	double computeCostFuncSingal(double sig_h,double y) ;//can the cost be negative

	Matrix computeGradient(Matrix X,Matrix y,int index);
	Matrix computeGradient_stoc(Matrix Xi,double yi,int index);

	Matrix predictLogSigClass(Matrix p_x,Matrix the);
	Matrix predictLog(Matrix p_x, bool bPercent =false);

	void gradientDescent_basic();
	bool trainForSigClass(int index =0);//index =-1, illustrate that its singal class

	void trainForSigClass_stoc(int index);
	void gradientDescent_stoc();

	//deal with pictures
	XY_PARA getParasDrawInput();
	DECISION_PARAS getDesionBoundPara(int index = 0);
	void getEvalRslt();

	void setTrainPara(SUPERVISE_PARA, int index =0);
	void setPreProcessPara(PRE_PROCESS_PARA p, int index=0);

	bool trainData(int index =0);

	MIN_COST_PARA_RECORD getMinCostPara(int index =0);

	XY_PARA predict(std::string filename);
	Matrix predict(Matrix x);
public:

	Matrix* X_multi;
	bool* is_normed;
	bool* is_scaled;
	int* scale_dimension;

	NORMALIZATION* normal_multi;
	MIN_COST_PARA_RECORD* minCostRecord;

	Matrix X_scale;

	Matrix Y_multilabel; //the Y matrix transferred because of multilabels
    bool is_multiclass;

	int num_labels;

	Matrix* theta_mul;

	int max_iters;

	int inc_times;
	double J_converge;
	LG_TRAIN_METHOD train_method;

	Matrix h_percent;
};

#endif