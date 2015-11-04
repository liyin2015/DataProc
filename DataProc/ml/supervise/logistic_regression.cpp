#include "logistic_regression.hpp"
#include <algorithm>
#include <time.h>
using namespace std;
	LogReg::LogReg()
	{
		max_iters =MAX_ITERS;
		//J_converge = DEFAULT_CONVERG;
		J_converge = 0.01;
		inc_times = 0;
		num_labels =0;
		theta_mul = NULL;

		X_multi = NULL;
		is_normed = NULL;
		is_scaled = NULL;
		scale_dimension = NULL;
		minCostRecord = NULL;
		normal_multi = NULL;
	}
	LogReg::~LogReg()
	{
		if(is_normed!= NULL)
		{
			delete[] is_normed;
			is_normed = NULL;
		}
		if(X_multi!= NULL)
		{
			delete[] X_multi;
			X_multi = NULL;
		}
		if(is_scaled!= NULL)
		{
			delete[] is_scaled;
			is_scaled = NULL;
		}
		if(scale_dimension!= NULL)
		{
			delete[] scale_dimension;
			scale_dimension = NULL;
		}
		if(minCostRecord != NULL)
		{
			delete[] minCostRecord;
			minCostRecord = NULL;
		}
		if(normal_multi!= NULL)
		{
			delete[] normal_multi;
			normal_multi = NULL;
		}
	};

int LogReg::loadData(string filename, bool Rand, ALGO_TYPE algo_type)
{
	//get X_orig, Y_orig, X_rand, Y_Rand
	if(Algo_Basic::loadData(filename, Rand, algo_type) == -1)
	{
		ML_LOR_ERROR("load original data from file failed!");
		return -1;
	}

	//do the extra initization for logistic regression
	num_labels = labels.length();

	if(num_labels ==2)
	{
		num_labels =1;
	}
	ML_LOR_INFO("X_original"<<X_orig);
	ML_LOR_INFO("y_original"<<Y_orig);
	ML_LOR_INFO("label matrix "<<labels);
	ML_LOR_INFO("label number is "<<num_labels);

	if(bRand)
	{
		ML_LOR_INFO("x_rand"<<X_rand);
		ML_LOR_INFO("y_rand"<<Y_rand);
	}
	
	return num_labels;
}
void LogReg::setRates(RATE_PARA pa)
{
	Algo_Basic::setRates(pa);
	ML_LOR_INFO("train_rate = "<<rate_para.train_rate<<", cv rate = "<<rate_para.cv_rate<<" , test rate = "<<rate_para.test_rate);

	X_multi = new Matrix[num_labels];
	theta_mul = new Matrix[num_labels];

	//init for audiuaty method
	normal_multi = new NORMALIZATION[num_labels];
	is_normed = new bool[num_labels];
	is_scaled = new bool[num_labels];
	scale_dimension = new int[num_labels];
	minCostRecord = new MIN_COST_PARA_RECORD[num_labels];
	//init data
	for(int i=0;i<num_labels;i++)
	{
		X_multi[i] = X_train;
		is_normed[i] = false;
		is_scaled[i] = false;
		scale_dimension[i] = 0;

	}
}
void LogReg::setConstrainPara(int num_ite,double al,double lam)
{
	para.num_iters = num_ite;
	para.alpha = al;
	para.lambda = lam;
}
	void LogReg::initWeight(int index)
	{
		theta_mul[index]=Matrix(X_multi[index].brandth(),1);
		ML_LOR_INFO("inited theta "<<index<<endl<<theta_mul[index]);
		return;
	}
	void LogReg::trainLog(LG_TRAIN_METHOD method)
	{
		//initWeight();
		switch(method)
		{
		case BASIC:
			gradientDescent_basic();
			break;
		case STOC:
			gradientDescent_stoc();
			break;
		default:
			return;
		}
	}
/*%step1:compute hx  
hx = X*theta;  
  
%step2:compute h(hx)  
h = sigmoid(hx);  
  
%step3:compute cost function's sum part  
for i=1:m,  
    Jtmp=Jtmp+(-y(i)*log(h(i))-(1-y(i))*log(1-h(i)));  
end;  
J=(1/m)*Jtmp; */
double  LogReg::computeCostFunc(Matrix& sig_h,Matrix& y) //can the cost be negative
{
	double J = 65535;
	int m=sig_h.length();
	Matrix ones(malloc_2(1,m,1),m,1);

	Matrix m1= y^(sig_h.getLog(e));
	Matrix m2 = (ones-sig_h).getLog(e);
	Matrix m3 = (ones -y)^m2;

	Matrix tmp = (m1+m3).sumPerCol();

	double tmp1 = tmp.matrix_[0];

	J = ((-1.0/m))*tmp1;
	return J;

}
double LogReg::computeCostFuncSingal(double sig_hi,double yi) //can the cost be negative
{
	double J = (-yi*log(sig_hi)-(1-yi)*log(1-sig_hi));
	return J;
}

/*%step4:compute gradient's sum part 
grad = zeros(size(theta)); 
sum1 =zeros(size(X,2),1);%#features row  
for i=1:m  
    sum1 = sum1+(h(i)-y(i)).*X(i,:)';  
end;  
      
grad= (1/m)*sum1;*/

Matrix LogReg::computeGradient(Matrix x,Matrix y,int index) //in this step if compute grad will save the compute J step
{	
	int m=x.length();
	int n=x.brandth();

	Matrix h=sigmoid(x*theta_mul[index]);
	J = computeCostFunc(h,y);
	Matrix error =h-y;

	Matrix grad = (x.trans()*error).multiplyReal(para.alpha/m);
	return grad;
	
}
Matrix LogReg::computeGradient_stoc(Matrix xi,double yi,int index)
{
	int n=xi.brandth();

	Matrix grad;
	double hi=sigmoid(xi*theta_mul[index]).matrix_[0];
	ML_LOR_INFO("hi"<<hi<<"yi"<<yi);

	J=computeCostFuncSingal(hi,yi);
	double error =hi-yi;


	grad = xi.trans().multiplyReal(para.alpha*error);
	return grad;
}
void LogReg::trainForSigClass_stoc(int index)
{
	//directForTrain(index);
	initWeight(index);

	double J=(double) INT_MAX;
	double alpha_1=para.alpha;

	//Matrix grad;
	Matrix grad;
	double J_pre =J;
	//01. transfer y if it is multiclass
	//Matrix tmpy = Y_train
	Matrix tmpy(Y_train.length(),1);
	for(int i=0;i<Y_train.length();i++)
	{
		if(Y_train.matrix_[i] ==labels.matrix_[index])
		{
			tmpy.matrix_[i]=1;
		}
		else
		{
			tmpy.matrix_[i]=0;
		}
	}
	int m=X_multi[index].length();

	int* rand = new int[m];
	//vector<int>* rand=new vector<int>[m];
	for(int i =0;i<m;i++)
	{
		rand[i]=i; //randomize the index
		//rand->push_back(i);
	}
	srand((unsigned int)time(NULL));
	//vector<int>::iterator ite1= rand->begin();
	//vector<int>::iterator ite2= rand->end();
	//vector<int>::iterator ite;
	random_shuffle(&rand[0],&rand[m-1]);
	for(int i = 0;i<max_iters;)
	{
		//random_shuffle(ite1,ite2);
		//for(ite=ite1;ite!=ite2;ite++,i++)
		//{
		for(int j=0;j<m;j++,i++)
		{			
			Matrix xi=X_multi[index](rand[j],(JUDGE_ROW)0); //get row in rand
			grad = computeGradient_stoc(xi,tmpy.matrix_[rand[j]],index);
			ML_LOR_INFO("grad"<<grad);
			if(J<J_converge)
			{
				ML_LOR_INFO("theta"<<index<<"converged, J"<<i<<"="<<J);
				return;
			}
			ML_LOR_INFO("J"<<i<<"="<<J);
			theta_mul[index]=theta_mul[index] - grad.multiplyReal(alpha_1/m); //negative gradient direction
			//compute for the next alpha
			alpha_1 = 4.0/(1.0+i+rand[j])+0.01;
		}
	}
}

bool LogReg::trainForSigClass(int index)
{
	ML_LOR_INFO("gradientDescent_basic, will train label "<<index+1);
	//init
	initWeight(index);

	double alpha_1=para.alpha;

	Matrix grad;
	double J_pre =J,minJ = J;

	int m = Y_train.length();
	//01. transfer y if it is multiclass
	Matrix tmpy(Y_train.length(),1);
	for(int i=0;i<Y_train.length();i++)
	{
		if(Y_train.matrix_[i] ==labels.matrix_[index])
		{
			tmpy.matrix_[i]=1;
		}
		else
		{
			tmpy.matrix_[i]=0;
		}
	}
	//02. interators start
	for(int i=0;i<para.num_iters;i++)
	{
		ML_LOR_INFO("J"<<i<<"="<<J);
		grad=computeGradient(X_multi[index],tmpy,index);
		if(i ==para.num_iters -1)
		{
			mJ=mJ.getRow(0,i);
			ML_LOR_INFO("it has trained "<< para.num_iters<<", and stoped without convergence");
			ML_LOR_INFO("J"<<i<<"="<<J);
			//ML_LOR_INFO("mJ"<<mJ);
			ML_LOR_INFO("theta mul"<<index<<theta_mul[index]);
			return false;
		}
		*mJ(i,0) = J;
		if(J<minJ)
		{
			minCostRecord[index].num_iters = i+1;
			minCostRecord[index].minJ = J;			
		}

		if(J<para.converge)
		{
			ML_LOR_INFO("J converged"<<i<<"="<<J);

			ML_LOR_INFO("theta"<<index<<" converge\n"<<theta_mul[index]);
			mJ=mJ.getRow(0,i);
			return true;
		}
	
		if(J_pre<J)
		{
			alpha_1 =0.75*alpha_1;
			inc_times++;
		}
		else
		{
			alpha_1 = 1.05*alpha_1;
		}

		J_pre = J;
		theta_mul[index]=theta_mul[index] - grad.multiplyReal(alpha_1/m); //negative gradient direction
	}

	return false;
}
void LogReg::gradientDescent_stoc()
{
	for(int i=0;i<num_labels;i++)
	{
		cout<<"will train label "<<i+1<<endl;
		trainForSigClass_stoc(i);
	}
	return;
}
void LogReg::gradientDescent_basic()
{
		for(int i=0;i<num_labels;i++)
		{
			ML_LOR_INFO("gradientDescent_basic, will train label "<<i+1);
			trainForSigClass(i);
		}
		return;
}
Matrix LogReg::predictLog(Matrix p_x, bool bPercent)
{	
	Matrix old_p_x = p_x;
	Matrix h(p_x.length(),num_labels,-1);
	Matrix re(p_x.length(),1,-1);

	if(num_labels >1)
	{
		for(int i=0;i<num_labels;i++)
		{
			Matrix new_px = p_x;
			if(is_normed[i])
			{
				new_px=old_p_x.getNorm(&normal_multi[i].mu,&normal_multi[i].sigma);
			}
			if(is_scaled[i])
			{
				cout<<"index"<<i<<"dim"<<scale_dimension[i]<<endl;
				new_px.addOneCol(0,1);
				new_px=featureScale(new_px,scale_dimension[i]);
			}
			else
			{			
				new_px.addOneCol(0,1);
			}
			Matrix tmph;
			//tmph =predictLogSigClass(X_multi[i],theta_mul[i]);
			tmph =predictLogSigClass(new_px,theta_mul[i]);
			h.subsituteByIndex(tmph,0,0,i,i);
		}

		//ML_LOR_INFO("hypo"<<h);
		Matrix tmp = h.trans();	
		if(bPercent)
		{
			return h;
		}
		h = tmp.getMaxIndexPerCol();
		return h;
	}
	else if(num_labels ==1)
	{
		Matrix new_px = p_x;

		if(is_normed[0])
		{
			new_px=old_p_x.getNorm(&normal_multi[0].mu,&normal_multi[0].sigma);
		}
		if(is_scaled[0])
		{
			new_px.addOneCol(0,1);
			new_px=featureScale(new_px,scale_dimension[0]);
		}
		else
		{
			new_px.addOneCol(0,1);
		}
		Matrix h(p_x.length(),1);

		h = predictLogSigClass(new_px,theta_mul[0]);
		if(bPercent)
		{
			return h;
		}
		for(int i=0;i<h.length();i++)
		{
			if(h.matrix_[i]>0.5)
			{
				h.matrix_[i]=1;
			}
			else
			{
				h.matrix_[i]=0;
			}
		}
		return h;
	}
	else
	{
		Matrix h;
		return h;
	}
}
Matrix LogReg::predictLogSigClass(Matrix p_x, Matrix the)
{
	int m=p_x.length();
	Matrix h(m,1);
	h=p_x*the;
	h=sigmoid(h);
	return h;
}


XY_PARA LogReg::getParasDrawInput()
{
	XY_PARA re =Algo_Basic::getParasDrawInput();
	re.label = labels;
	return re;
}
DECISION_PARAS LogReg::getDesionBoundPara(int index)
{
	DECISION_PARAS re;
	re.x = X_multi[index];
	//re.y=Y_orig;
	re.theta = theta_mul[index];
	re.normal = normal_multi[index];
	re.bNorm = is_normed[index];
	return re;
}
void LogReg::setPreProcessPara(PRE_PROCESS_PARA p, int index)
{
	Algo_Basic::setPreProcessPara(p,index);
	if(pre_para.bNorm)
	{
		X_multi[index]=X_train.getNorm(&normal_multi[index].mu,&normal_multi[index].sigma);
		is_normed[index]=true;
	}
	if(pre_para.bScale)
	{
		X_multi[index].addOneCol(0,1);
		X_multi[index] = featureScale(X_multi[index],pre_para.dim);
		is_scaled[index] =true;
		scale_dimension[index] = pre_para.dim;
	}
	else
	{
		X_multi[index].addOneCol(0,1);
	}

	ML_LOR_INFO("X_train"<<X_multi[index]);
}
//starts from X_train
void LogReg::setTrainPara(SUPERVISE_PARA p, int index)
{
	Algo_Basic::setTrainPara(p,index); //already has copied all the parameters
	//X_multi[index] = X_train;
	/*if(para.bNorm)
	{
		X_multi[index]=X_multi[index].getNorm(&normal_multi[index].mu,&normal_multi[index].sigma);
		is_normed[index]=true;
	}
	if(para.bScale)
	{
		X_multi[index].addOneCol(0,1);
		X_multi[index] = featureScale(X_multi[index],para.dim);
		is_scaled[index] =true;
		scale_dimension[index] = para.dim;
	}
	else
	{
		//add one col for x
		X_multi[index].addOneCol(0,1);
	}*/
	ML_LOR_INFO("X_multi that will be  used to train data"<<"\n"<<X_multi[index]);
	ML_LOR_INFO("setTrainPara, num_iters = "<<para.num_iters<<", alpha = "<<para.alpha<<", lamda = "<<para.lambda<<"dimension ="<<pre_para.dim);
}

MIN_COST_PARA_RECORD LogReg::getMinCostPara(int index)
{
	return minCostRecord[index];
}
bool LogReg::trainData(int index)
{
	//trainLog(BASIC);
	return trainForSigClass(index);
}
XY_PARA LogReg::predict(std::string filename)
{
	XY_PARA re;
	if(filename.empty())
	{
		re.x_orig = X_orig;
		re.y = predictLog(re.x_orig);
		re.rslt = 0;
		//ML_LOR_INFO("01: predict of the input\n"<<re.y);
	}
	else
	{
		if(read_file_no_rslt(re.x_orig, filename)==-1)
		{
			ML_LOR_ERROR("LogReg, predict, read file failed!");
			re.rslt = -1;
			return re;
		}

		re.label = labels;
		re.y = predictLog(re.x_orig);
		//ML_LOR_INFO("predict rslt is " <<re.y<<"x:"<<re.x_orig);
		re.rslt = 0;
	}
	return re;
}
Matrix LogReg::predict(Matrix x)
{
	Matrix y = predictLog(x);
	return y;
}
void LogReg::getEvalRslt()
{
	Matrix x = X_orig;
	Matrix y = Y_orig;
	if(bRand)
	{
		x = X_rand;
		y = Y_rand;
	}

	//Matrix hypo_percent = predictLog(x,true);
	Matrix hypo = predictLog(x,false);
	//01. get relative matrixs
	int m = y.length();
	int train_n= (int)(rate_para.train_rate*m);
	Matrix y_train = y.getRow(0,train_n-1);
	Matrix hypo_train = hypo.getRow(0,train_n-1);

	ML_LOR_INFO("train"<<y_train);
	ML_LOR_INFO("hypo train"<<hypo_train);

	ML_LOR_INFO("train from 0 to "<<train_n-1);

	int cv_n = (int)((rate_para.train_rate+rate_para.cv_rate)*m);
	Matrix y_cv =y.getRow(train_n,cv_n-1);
	Matrix hypo_cv =hypo.getRow(train_n,cv_n-1);
	ML_LOR_INFO("cv from "<<train_n<<" to "<<cv_n-1);

	int test_m = m-cv_n;
	Matrix y_test =y.getRow(cv_n,m-1);
	Matrix hypo_test =hypo.getRow(cv_n,m-1);
	ML_LOR_INFO("test from "<<cv_n<<" to "<<m-1);
	//get cost
	eval_rslt.Jtrain = computeCostFunc(hypo_train,y_train);
	eval_rslt.Jcv = computeCostFunc(hypo_cv,y_cv);
	eval_rslt.Jtest = computeCostFunc(hypo_test,y_test);

	//01. get accuracy
	
	eval_rslt.train_accuracy = eval_rslt.getAccuracy(hypo.getRow(0,train_n-1),y_train);
	eval_rslt.test_accuracy = eval_rslt.getAccuracy(hypo.getRow(train_n,m-1),y.getRow(train_n,m-1));
	eval_rslt.getPrecision(hypo,y,labels);

    //02. get cost, these are used for model selection
	ML_LOR_INFO("the evaulation parameters are as follows:"<<eval_rslt);
	return;
}