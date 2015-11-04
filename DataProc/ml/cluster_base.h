//#ifndef CLUSTER_BASE_H_
//#define CLUSTER_BASE_H_
#pragma once

#include "matrix.hpp"
#include "algo_basic.h"



typedef struct
{
	int k;//cluster number
}CLUSTER_TRAIN_PARA;

class Cluster_Base
{
public:
	int virtual loadData(std::string name);

	XY_PARA virtual getParasDrawInput()=0;

	int virtual setTrainPara(CLUSTER_TRAIN_PARA)=0;

	int virtual exectCluster() =0;

public:
   Matrix X_orig;


};
//#endif