#include "algo_basic.h"
/***********************NNS_CreateInfo************************************************************/
NNS_CreateInfo::NNS_CreateInfo()
{
	InputNodes = 0; 
	HiddenLayers = 0;              // 隐层数   
	HiddenNodes = NULL;              // 各隐层的节点数   
	OutputNodes = 0;               // 输出层节点数 
	TransFunc = LINEAR_FUNC;
}
NNS_CreateInfo::NNS_CreateInfo(NNS_CreateInfo& n)
{
	this->HiddenLayers = n.HiddenLayers;
	if(this->HiddenNodes != NULL)
	{
		delete[] this->HiddenNodes;
	}
	this->HiddenNodes = new int[n.HiddenLayers];
	for(int i=0;i<n.HiddenLayers;i++)
	{
		this->HiddenNodes[i]=n.HiddenNodes[i];
	}

	this->InputNodes = n.InputNodes;
	this->OutputNodes = n.OutputNodes;
	this->TransFunc = n.TransFunc;
}
NNS_CreateInfo& NNS_CreateInfo::operator=(NNS_CreateInfo& m)
{
	if(this ==&m)
	{
		return m;
	}
	this->HiddenLayers = m.HiddenLayers;
	if(this->HiddenNodes != NULL)
	{
		delete[] this->HiddenNodes;
		this->HiddenNodes = NULL;
	}
	this->HiddenNodes = new int[m.HiddenLayers];
	if(m.HiddenLayers>0)
	{
		for(int i=0;i<m.HiddenLayers;++i)
		{
			this->HiddenNodes[i]=m.HiddenNodes[i];
		}
	}

	this->InputNodes = m.InputNodes;
	this->OutputNodes = m.OutputNodes;
	this->TransFunc = m.TransFunc;
	return *this;
}
NNS_CreateInfo::~NNS_CreateInfo()
{
	if(HiddenNodes != NULL)
	{
		delete[] HiddenNodes;
		HiddenNodes = NULL;
	}
}
/*********************************PRE PROCESS PARA**************************************************************/
PRE_PROCESS_PARA::PRE_PROCESS_PARA()
{
	bNorm = false;
	bScale = false;
	dim = 0;
}
PRE_PROCESS_PARA::~PRE_PROCESS_PARA()
{
}

/**********************************SUPERVISE_PARA***************************************************************/
SUPERVISE_PARA::SUPERVISE_PARA()
{
	converge = DEFAULT_CONVERG;
	//pre_para.bNorm = false;
	//pre_para.bScale = false;
	//pre_para.dim = 0;

	alpha = DEFAULT_ALPHA; //all the algorithms hace alpha
	lambda = 0;
	num_iters = DEFAULT_ITERS;

	nn_struct.HiddenLayers = 1;
	nn_struct.HiddenNodes = new int[1];
	nn_struct.HiddenNodes[0]=2;
	nn_struct.InputNodes = 0;
	nn_struct.OutputNodes = 0;
	nn_struct.TransFunc = LINEAR_FUNC;

	kernel_type = LINEAR;
	tol = 1e-3;
	C = 1.0;

	train_method = 0;
}
SUPERVISE_PARA::~SUPERVISE_PARA()
{
	if(nn_struct.HiddenNodes != NULL)
	{
		delete[] nn_struct.HiddenNodes;
		nn_struct.HiddenNodes = NULL;
	}
}
SUPERVISE_PARA& SUPERVISE_PARA::operator=(SUPERVISE_PARA& p)  //this has no use actually
{
	if(this == &p)
	{
		return p;
	}
	this->converge = p.converge;
	this->num_iters = p.num_iters;
	this->alpha = p.alpha;
	this->lambda = p.lambda;

	this->nn_struct = p.nn_struct;

	this->tol = p.tol;
	this->C = p.C;
	this->train_method = p.train_method;
	return *this;
}
SUPERVISE_PARA::SUPERVISE_PARA(SUPERVISE_PARA& p)
{
	this->converge = p.converge;
	this->num_iters = p.num_iters;
	this->alpha = p.alpha;
	this->lambda = p.lambda;

	this->nn_struct= p.nn_struct;

	this->tol = p.tol;
	this->C = p.C;
	this->train_method = p.train_method;
}
/***************************************Algo_Basic**********************************************************/

Algo_Basic::Algo_Basic()
{
	J = (double)INT_MAX;
	bRand = false;
	rate_para.train_rate = DEFAULT_TRAIN_RATE;
	rate_para.cv_rate = DEFAULT_CV_RATE;
	rate_para.test_rate = DEFAULT_TEST_RATE;
	
}
Algo_Basic::~Algo_Basic()
{
}
int Algo_Basic::loadData(std::string filename, bool Rand, ALGO_TYPE algo_type)
{
	//linear, get orignal: X_orig, Y_orig
	if(algo_type == LINEAR_REG_TYPE)
	{
		if(read_input_file(X_orig,Y_orig,filename)==-1)
		{
			return -1;
		}
	}
	//others
	else
	{
		if(read_input_file_multiclass(X_orig,Y_orig,labels,filename)==-1)
		{
			return -1;
		}
	}
	bRand =Rand;
	if(bRand)
	{
		randData(); //X and Y has been randomized, and save X_rand
	}
	return 0;
}
void Algo_Basic::randData()
{
	int m = X_orig.length();
	int* rand = new int[m];
	for(int i=0;i< m;i++)
	{
		rand[i] =i;
	}
	srand((unsigned int)time(NULL));
	random_shuffle(&rand[0],&rand[m-1]);
	//Matrix
	X_rand = X_orig;
	Y_rand = Y_orig;
	for(int i=0;i<m;i++)
	{
		X_rand.subsituteRow(X_orig,rand[i],rand[i],i,i);
		Y_rand.subsituteRow(Y_orig,rand[i],rand[i],i,i);
	}
}
void Algo_Basic::getTrainDatasets()
{
	int m = X_orig.length();
	int train_n= (int)(rate_para.train_rate*m);
	if(bRand)
	{
		randData(); //X and Y is randomized
		X_train = X_rand.getRow(0,train_n-1);
		Y_train = Y_rand.getRow(0,train_n-1);	
	}	
	else
	{
		X_train = X_orig.getRow(0,train_n-1);
		Y_train = Y_orig.getRow(0,train_n-1);
	}
}
void Algo_Basic::setRates(RATE_PARA r)
{
	rate_para = r;
	//should make sure x starts with the same as X_orig
	int m = X_orig.length();
	int train_n= (int)(rate_para.train_rate*m);
}
/*****************************************************this is after setting input paras and the setting will be different for different labels********************************/
XY_PARA Algo_Basic::getParasDrawInput()
{
	XY_PARA re;
	re.x_orig=X_orig;
	re.y=Y_orig;
	return re;
}
void Algo_Basic::setPreProcessPara(PRE_PROCESS_PARA p, int index)
{
	pre_para = p;
	//should make sure x starts with the same as X_orig
	getTrainDatasets();
	ML_LOR_INFO("X_train"<<X_train);
}

	//set paras
	//void virtual inputDataProc(bool bScale, bool bNorm, int dim,int index = 0)=0;//index is used for multi class labels
void Algo_Basic::setTrainPara(SUPERVISE_PARA p, int index)
{
	para = p;
	mJ = Matrix(para.num_iters,1,65536.0);	
	return;
}
double Algo_Basic::monitor()
{
	return J;
}
MIN_COST_PARA_RECORD Algo_Basic::getMinCostPara(int index)
{
	MIN_COST_PARA_RECORD record;
	record.minJ = J;
	record.num_iters = para.num_iters;
	return record;
}
void Algo_Basic::writeRslt(std::string filename)
{
	//writePredict2File(
}
double Algo_Basic::computeCostFunc(Matrix& hypo, Matrix& y)
{
	return 65536;
}
void Algo_Basic::getEvalRslt()
{
	return;
}