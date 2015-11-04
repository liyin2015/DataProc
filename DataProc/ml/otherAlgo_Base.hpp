#ifndef OTHERALGO_BASE_HPP_
#define OTHERALGO_BASE_HPP_
#include <iostream>
using namespace std;
typedef struct
{
	int minSupport; //used for Apriori
}OTHER_ALGO_PARA;


class OtherAlgo_Base
{
public:
	void virtual loadData(std::string& filename)=0;

	void virtual setPara(OTHER_ALGO_PARA& para)=0;
	void virtual executeAlgo() = 0;
};

#endif