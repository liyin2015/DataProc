#ifndef SVM_HPP_
#define SVM_HPP_
#include <iostream>
#include  "../math_function.hpp"
#include "../matrix.hpp"
#include "../kernels.h"
#include "../evaluate_hypo.hpp"
#include "../algo_basic.h"

//#include "../visualize_data.hpp"
using namespace std;


class SVM:public Algo_Basic
{
public:
	SVM();
	~SVM();
	//get train sets
	int loadData(string filename,bool brand, ALGO_TYPE algo_type = SVM_TYPE);
	void setRates(RATE_PARA pa);

	//training parameters
	void setConstancePara(int index, int num_ite,double C,double train_ra, double tol,KERNEL_TYPE kernel);
	void initWeight(int index);

	double computeCostFunc(Matrix h,Matrix y);//sigmoid h

	Matrix computeGradient(Matrix X,Matrix y,int index,double& J);

	//used for prediction
	Matrix predictSvmSigClass(int index,Matrix p_x,Matrix the, double bi);
	Matrix predictSvm(Matrix p_x);

	void svm_train();
	void trainForSigClass(int index);//index =-1, illustrate that its singal class

	//used to evaluate the system
	void getEvalRslt();

	//draw pic
	XY_PARA getParasDrawInput();
	DECISION_PARAS getDesionBoundPara(int index = 0);
	void setPreProcessPara(PRE_PROCESS_PARA p, int index=0);
	void setTrainPara(SUPERVISE_PARA, int index=0);
	bool trainData(int index =0);

	//MIN_COST_PARA_RECORD getMinCostPara(int index =0){}

	XY_PARA predict(std::string filename);
	Matrix predict(Matrix x);
public:
	//int m;
	//int train_m;
	//int n;
	//int n_save;
	Matrix* X_multi; //no need to add one col any more
	NORMALIZATION* normal_multi;

	//double train_rate;

	//used for multiclass
	bool* is_normed;
	bool* is_scaled;
	int* scale_dimension;

	Matrix* kernel;
	KERNEL_TYPE* kernel_type;

	//Matrix Y;
	//Matrix trans_Y;
	//Matrix Y_train;

	//Matrix y_train;

	//Matrix labels; //used for multiclass
    bool is_multiclass;
	int num_labels;
	//Matrix theta; //can improve it to multi-class
	Matrix* theta_mul;
	Matrix* alphas;
	double* b;

	//double C;
	//int max_iters;
	//double tol;
	RSLT_EVAL rslt_eval;
};
#endif