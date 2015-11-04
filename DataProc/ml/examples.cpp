#ifndef EXAMPLES_CPP_
#define EXAMPLES_CPP_
#include "examples.hpp"

void eg_level_cluster()
{
	std::string name1 = "data\\point.txt";
	LevelCluster lc;
	lc.loadData(name1);
	lc.setParas(3);
	lc.exectCluster();

}
#endif