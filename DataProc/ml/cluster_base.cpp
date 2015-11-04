#include "cluster_base.h"
int Cluster_Base::loadData(std::string name)
{
	if(read_file_no_rslt(X_orig, name) != -1)
	{
		return 0;
	}
	return -1;
}