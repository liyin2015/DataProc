#ifndef LEVEL_CLUSTER_HPP_
#define LEVEL_CLUSTER_HPP_
#include "../matrix.hpp"
#include "../io_manager.hpp"
#include "../cluster_base.h"

#include <iostream>

using namespace std;

class LevelCluster:public Cluster_Base
{
public:
	LevelCluster();
	~LevelCluster();//must realize these functions, since there has no default for these matrix type
	//int loadData(string name);
	int setParas(int n);
	void initDistance(Matrix& distance);


	int exectCluster();
	XY_PARA getParasDrawInput();

	//used for the interface
	int setTrainPara(CLUSTER_TRAIN_PARA);

public:
	//Matrix X_orig;
	vector<int>* cluster_rslt;
	int K;
};


#endif