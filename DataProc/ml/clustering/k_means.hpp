#ifndef K_MEANS_HPP_
#define K_MEANS_HPP_

#include "../matrix.hpp"
#include "../io_manager.hpp"
#include "../cluster_base.h"

#include <iostream>

using namespace std;

class KMeans:public Cluster_Base
{
public:
	KMeans();
	~KMeans();//must realize these functions, since there has no default for these matrix type
	int loadData(string name);
	int setParas(int n);
	void initDistance(Matrix& distance);


	int exectCluster();
	XY_PARA getParasDrawInput();

	int setTrainPara(CLUSTER_TRAIN_PARA);
	//used for the interface

public:
	Matrix X_orig;
	vector<int>* cluster_rslt;
	int K;
};
#endif