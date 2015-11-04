#ifndef NEURAL_NETWOEK_HPP_
#define NEURAL_NETWOEK_HPP_

#include "../matrix.hpp"
#include "../math_function.hpp"
#include "../feature_scalling.hpp"
#include "../algo_basic.h"


const double MAXWEIGHT = 0.3 ;
const double SCALEWEIGHT = 32767 ;

using namespace std;



class TRAIN_PARA
{
public:
	TRAIN_PARA():theta(NULL),a(NULL),z(NULL),d(NULL),delta(NULL),theta_grad(NULL)
	{
	}
	~TRAIN_PARA()
	{
		if(a!= NULL)
		{
			delete[] a;
		}
		if(z!= NULL)
		{
			delete[] z;
		}
		if(d!= NULL)
		{
			delete[] d;
		}
		if(theta_grad!= NULL)
		{
			delete[] theta_grad;
		}
		if(delta!= NULL)
		{
			delete[] delta;
		}
		if(theta!= NULL)
		{
			delete[] theta;
		}
	}
	void init(int num_hidden_layer)
	{
		a = new Matrix[1+num_hidden_layer+1];//每层的输出量
		z = new Matrix[1+num_hidden_layer+1];//计算A的中间变量
		d = new Matrix[1+num_hidden_layer+1];//误差估计

		theta_grad = new Matrix[1+num_hidden_layer];//a2->z2
		delta =new Matrix[1+num_hidden_layer];//增量
		theta = new Matrix[1+num_hidden_layer];
	}
	Matrix* theta; //1+num_hidden_layer;
	Matrix* a;
	Matrix* z;//a2->z2
	Matrix* d;
	Matrix* delta;
	Matrix* theta_grad;//a2->z2， get gradient vector,used for gradient checking 

};


typedef enum
{
	BP_DM=0,
	BP_DM_SELF_ADJ=1,
}TRAIN_METHOD;


class NeuralNetwork:public Algo_Basic
{
public:
	NeuralNetwork(int num_in,int num_hl,int* numhno, int num_lab,int train);
	NeuralNetwork();
	~NeuralNetwork(){};
	void train_para_init();
	//void getParaFromTrainMethod(TRAIN_METHOD method);
	void alloc_res_for_train();

	void randomizeWeight(TRAIN_PARA& tp);

	void setNeuralPara(int num_hl, int* numhno,Input_Trans_Func f)
	{
		para.nn_struct.HiddenLayers = num_hl;
		para.nn_struct.HiddenNodes = numhno;	
		//setHiddenLayerPara(num_hl,numhno);
		trans_func = f;
		//setTransFunc(f);
	}

	/*void setRegularPara(double l)
	{
		para.lambda = l;
	}*/
	int loadData(string filename, bool brand, ALGO_TYPE algo_type = NEURAL_NET_TYPE);//step1
	void setRates(RATE_PARA pa);

	void setConstancePara(int num_i, double converge, double al, double lam=0)
	{
		para.alpha=al;
		para.lambda = lam;

		para.num_iters = num_i;
		converge_J = converge;
	}
	double computeCostFunction(TRAIN_METHOD train_method,Matrix h,Matrix tmpy);//step3
	Matrix trans_y_for_multilabels();
	Matrix computeHypoFunc(TRAIN_METHOD, Matrix x);//forward propagation, return h
	//void computeGradient(Matrix h,Matrix tmpy);//back propagation
	void backPropagation(TRAIN_METHOD,Matrix h,Matrix tmpy);
	Matrix sigmoidGradient(Matrix z);
	bool gradientDescenttrainBP_dm();
	bool gradientDescenttrainBP_dm_self_adjust();
	//void alloc_res_for_train(TRAIN_PARA train_para); 
	bool trainNN(); //used for outside interfaces
	Matrix gradientDescent();
	Matrix predictNN(Matrix x);

	//visual data
	XY_PARA getParasDrawInput();
	DECISION_PARAS getDesionBoundPara(int index = 0);

	void setPreProcessPara(PRE_PROCESS_PARA p, int index=0);
	void setTrainPara(SUPERVISE_PARA,int index =0);

	//MIN_COST_PARA_RECORD getMinCostPara(int index =0){return}
	bool trainData(int index =0);
	XY_PARA predict(std::string filename);
	Matrix predict(Matrix x);

public:
	//NNS_CreateInfo nn_struct;
	TRAIN_METHOD method;
	MIN_COST_PARA_RECORD minCostRecord;
	//net information
	//int num_input; X.length()
	//int num_hidden_layer;
	//int* num_per_hidden_layer;

	int num_labels;
	//int units_input_layer;

	//int m_save;
	//transfer function
	Input_Trans_Func trans_func;
	//Matrix X;//M*num_input
	//Matrix Y;//M*num_labels

	//train constrations
	Matrix tmp_Y; //if use SIGMOID_FUNC, we should transfer the if they dont equal to 1 or zero

	int max_iters;//can be used to limit the num_iters
	double converge_J;

    TRAIN_PARA para_dm;
	TRAIN_PARA para_dm_self_adj;
	bool is_delta_init;
	int inc_times;
};

#endif