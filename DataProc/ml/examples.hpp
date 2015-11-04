#ifndef EXAMPLES_HPP_
#define EXAMPLES_HPP_
#include "comm.hpp"
#include "feature_scalling.hpp"
#include "io_manager.hpp"
#include "matrix.hpp"
#include "supervise\linear_regression.hpp"
#include "supervise\logistic_regression.hpp"
#include "supervise\neural_network.hpp"
#include "supervise\svm.hpp"
#include "clustering\level_cluster.hpp"
#include <iostream>
using namespace std;

//void eg_linear_regression(LinReg lg);
//void eg_logistic_regression_multi();
//void eg_logistic_regression();
//void eg_neural_network();
//void eg_svm();
void eg_level_cluster();

template<typename T>
void general_train()
{
	T lg;
	//step1: read traning datasets from txt file	
	cout<<"plz input the doc name that you want to load:"<<endl
		<<"if you enter -1, means you use default input doc:imput.txt"<<endl;
	string name=getStrFromUser();//"imput.txt"
	if(name.compare("-1\n"))
	{
		name = "imput.txt";
	}
	if(lg.loadData(name)==-1) //get X, Y
	{
		cout<<"load data error"<<endl;
		return;
	}

	cout<<"plz set the paras, num of iterators, alpha, lambda:"<<endl
	<<"if you enter -1, means that you will use default value"<<endl;
	int num_iters = (int)getParaFromUser();
	double alpha = getParaFromUser();
	double lambda = getParaFromUser();
	cout<<num_iters<<"alpha"<<alpha<<"lambda"<<lambda<<endl;
	if(num_iters ==-1)
	{
		num_iters = 1000;
	}
	if(alpha == -1)
	{
		alpha =0.001;
	}
	if(lambda ==-1)
	{
		lambda =0;
	}

	//step2: get training algorithms
    lg.setConstrainPara(num_iters,alpha,lambda);
	LINEAR_REG_TRAIN_METHOD method = BASIC_LINEAR;
	lg.trainLinear(method);

	//step 3:get the test rate
	cout<<"predict of the input\n"<<lg.predictLinear(lg.X);
	
	//step 3: predict data
	Matrix pre;
	cout<<"plz input the predict doc name that you want to load:"<<endl
		<<"if you enter -1, means you use default input doc:linear_predict.txt"<<endl;
	name.clear();
	name=getStrFromUser();//"imput.txt"
	if(name.compare("-1\n"))
	{
		name = "linear_predict.txt";
	}
	if(read_predict_file(pre,name)==-1)
	{
		cout<<"load data error"<<endl;
		return;
	}
	pre.addOneCol(0,1);
	cout<<"pre"<<endl<<pre;
	
	Matrix pre_y = lg.predictLinear(pre);
	cout<<"predict\n"<<pre_y;
	return;
}
#endif