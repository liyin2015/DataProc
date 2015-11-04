#ifndef ALGO_BASIC_H_
#define ALGO_BASIC_H_
#include <algorithm>
#include <time.h>
#include "math_function.hpp"
#include "matrix.hpp"
#include "io_manager.hpp"
#include "evaluate_hypo.hpp"
#include "math_function.hpp"
#include "feature_scalling.hpp"
typedef enum
{
	LINEAR_REG_TYPE = 0,
	LOGISTIC_REG_TYPE =1,
	NEURAL_NET_TYPE = 2,
	SVM_TYPE = 3,

	//clustering
	HIER_CLUSTER = 51,
	TYPE_BUTT = -1,
}ALGO_TYPE;

typedef struct
{
	Matrix mu;
	Matrix sigma;
}NORMALIZATION;

typedef enum
{
	MIX = 0,
	PURITY = 1,
}STORE_METHOD;

typedef struct
{
	double train_rate;
	double cv_rate;
	double test_rate;
}RATE_PARA;


//defining default parameters
#define MAX_ITERS 100000
#define DEFAULT_CONVERG 1.0e-3
#define DEFAULT_ALPHA 0.01
#define DEFAULT_ITERS 1000
#define DEFAULT_TRAIN_RATE 0.6
#define DEFAULT_CV_RATE 0.2
#define DEFAULT_TEST_RATE 0.2
typedef struct
{
	Matrix x_orig;
	Matrix y;
	Matrix label;

	int rslt;
}XY_PARA; //used for input and predict paras, for linear regression, this is enough to draw the decision boundary

typedef struct
{
	Matrix x; //after all kinds of dealing, including adding one col
	//Matrix y; //generally, the same as y_orig
	Matrix theta; //for logistic regression , svm to draw decision boundary
	double b; //used for svm
	bool bNorm;
	NORMALIZATION normal;
}DECISION_PARAS;

//used for neural network
typedef enum
{
	LINEAR_FUNC=0,
	SIGMOID_FUNC=1,
}Input_Trans_Func;

class NNS_CreateInfo
{ 
public:
	NNS_CreateInfo();
	NNS_CreateInfo(NNS_CreateInfo& n);
	NNS_CreateInfo& operator=(NNS_CreateInfo& m);
	~NNS_CreateInfo();
    
public:  
     int   InputNodes;                // 输入层节点数   
     int   HiddenLayers;              // 隐层数   
     int   *HiddenNodes;              // 各隐层的节点数   
     int   OutputNodes;               // 输出层节点数 
	 Input_Trans_Func TransFunc;           
};

//kerneal types
typedef enum
{
	LINEAR = 0,
	GASSIAN = 1,
}KERNEL_TYPE;

class PRE_PROCESS_PARA
{
	//prepressoring paras
public:

	PRE_PROCESS_PARA();
	~PRE_PROCESS_PARA();
public:

	bool bNorm;
	bool bScale;
	int dim;
};

class SUPERVISE_PARA
{
public:
	SUPERVISE_PARA();
	~SUPERVISE_PARA();
	SUPERVISE_PARA& operator=(SUPERVISE_PARA& p);
    SUPERVISE_PARA(SUPERVISE_PARA& p);

public:
	//PRE_PROCESS_PARA pre_para;
	double converge; //control when to converge

	//paras to control train process
	int num_iters;
	double alpha;
	double lambda;	

	NNS_CreateInfo nn_struct;
	//svm
	KERNEL_TYPE kernel_type;
	double tol;
	double C; //same as 1/lambda

	int train_method; //to choose which training method to use
};

typedef struct
{
	double minJ;
	int num_iters;
}MIN_COST_PARA_RECORD;

//namespace ALGO_BASIC
//{

class Algo_Basic
{
public:
	Algo_Basic();
	~Algo_Basic();
	virtual int loadData(std::string filename, bool bRand, ALGO_TYPE algo_type);
    virtual void randData();

	virtual void setRates(RATE_PARA r);
	virtual void getTrainDatasets();
    
	virtual XY_PARA getParasDrawInput();
	DECISION_PARAS virtual getDesionBoundPara(int index = 0)=0;
	//set paras
	void virtual setPreProcessPara(PRE_PROCESS_PARA p, int index = 0);
	//void virtual inputDataProc(bool bScale, bool bNorm, int dim,int index = 0)=0;//index is used for multi class labels
	void virtual setTrainPara(SUPERVISE_PARA p, int index = 0);

	bool virtual trainData(int index = 0)=0; //bconverge

	MIN_COST_PARA_RECORD virtual getMinCostPara(int index =0);

	XY_PARA virtual predict(std::string filename ="")=0;
	Matrix  virtual predict(Matrix x)=0;
	double virtual monitor();

	void writeRslt(std::string filename);

	virtual double computeCostFunc(Matrix& hypo, Matrix& y);
	virtual void getEvalRslt();


public:
	//save the original input
	Matrix X_orig;
	Matrix Y_orig;
	RATE_PARA rate_para;
	//only used when bRand
	bool bRand;
	Matrix X_rand;
	Matrix Y_rand;
	//used for normalization
	NORMALIZATION normal;

	//Matrix X_train; //this is used for training
	//Matrix Y_train;

	//save the changed X, this is used for train, train_rate number of train sets
	Matrix X_train; //first is random, second is normalize, third is scalling
	Matrix Y_train;
	Matrix labels;
    double J;//current J
    Matrix mJ; //used to instore J
	RSLT_EVAL eval_rslt;
	SUPERVISE_PARA para;
    PRE_PROCESS_PARA pre_para;
};
//}
#endif