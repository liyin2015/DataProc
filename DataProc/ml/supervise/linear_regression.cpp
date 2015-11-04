/*function [theta, J_history] = gradientDescentMulti(X, y, theta, alpha, num_iters)  
%GRADIENTDESCENTMULTI Performs gradient descent to learn theta  
%   theta = GRADIENTDESCENTMULTI(x, y, theta, alpha, num_iters) updates theta by  
%   taking num_iters gradient steps with learning rate alpha  
  
% Initialize some useful values  
m = length(y); % number of training examples  
J_history = zeros(num_iters, 1);  
  
for iter = 1:num_iters  
    h= zeros(m,1);  
    disp('computeCostMulti');  
    J_history(iter) = computeCostMulti(X, y, theta);%只是为了展示J的变化，计算实际的值时可以不需要  
      
    h=X*theta; %计算hypothesis function  
    tmp1 = zeros(size(X,2),1);  
    for i=1:m  
       tmp1= tmp1+(h(i)-y(i)).*X(i,:)'; %计算sum((hi-yi)*xi)  
    end;  
    theta = theta - (alpha/m)*tmp1;%计算Jtheta  
    disp(J_history(iter));  
    disp(theta);  
end; 
theta = n+1, x=MX(n+1),y=m,theta =0, alapha = 0.01,numiters = 600)
h=X*theta*/
#include "linear_regression.hpp"

#include "../feature_scalling.hpp"
LinReg::LinReg()
{
	max_iters =MAX_ITERS;
	J_converge = DEFAULT_CONVERG;
	inc_times = 0;
}
LinReg::~LinReg(){}
int LinReg::loadData(string filename, bool Rand, ALGO_TYPE algo_type)
{
	if(Algo_Basic::loadData(filename,Rand, algo_type) == -1)
	{
		ML_LIR_INFO("load original data from file failed!");
		return -1;
	}
	ML_LIR_INFO("x_original"<<X_orig);
	ML_LIR_INFO("y_original"<<Y_orig);
	if(bRand)
	{
		ML_LIR_INFO("x_rand"<<X_rand);
		ML_LIR_INFO("y_rand"<<Y_rand);
	}
	return 0;
}
void LinReg::setRates(RATE_PARA pa)
{
	Algo_Basic::setRates(pa);
	ML_LIR_INFO("train_rate = "<<rate_para.train_rate<<", cv rate = "<<rate_para.cv_rate<<" , test rate = "<<rate_para.test_rate);

}

//return J_history
/*function J = computeCostMulti(X, y, theta)  
%COMPUTECOSTMULTI Compute cost for linear regression with multiple variables  
%   J = COMPUTECOSTMULTI(X, y, theta) computes the cost of using theta as the  
%   parameter for linear regression to fit the data points in X and y  
  
% Initialize some useful values  
m = length(y); % number of training examples  
  
% You need to return the following variables correctly   
J = 0;  
  
h= zeros(m,1);  
h = X*theta;  
J = (1/(2*m))*sum((h-y).^2);  
disp('J'),disp(J);*/

double LinReg::computeCostFunc(Matrix& h, Matrix& y)
{
	//compute J
	double J = 65535;
	int m = y.length();
	Matrix tmp = ((h-y).pointPow(2).sumPerCol());
	double tmp1 = tmp.matrix_[0];

	J = (1.0/(2*m))*tmp1;
	return J;

}
Matrix LinReg::computeGradient(Matrix X)
{
	//compute grad
	int m=X.length();
	int n=X.brandth();
	Matrix h=X*theta;
	J = computeCostFunc(h,Y_train);

	Matrix tmp_hy(m,n);
	Matrix error=h-Y_train;
	/*Matrix grad;
	for(int c=0;c<n;c++)
	{
		for(int r=0;r<m;r++)
		{
			tmp_hy.matrix_[r*n+c] = error.matrix_[r*1];
		}
	}
	Matrix tmp1 =((tmp_hy^X).sumPerCol()).trans(); //(1,X.brandth())
	grad = tmp1.multiplyReal(alpha/m);*/

	Matrix grad=(X.trans()*error).multiplyReal(1.0/m);
	return grad;
}



/*function [J, grad] = costFunction(theta, X, y)  
% Initialize some useful values  
m = length(y); % number of training examples  
  
% You need to return the following variables correctly   
J = 0;  
 
  
Jtmp=0;  
h= zeros(m,1);  
  
%step1:compute hx  
hx = X*theta;  
  
%step2:compute h(hx)  
h = sigmoid(hx);  
  
%step3:compute cost function's sum part  
for i=1:m,  
    Jtmp=Jtmp+(-y(i)*log(h(i))-(1-y(i))*log(1-h(i)));  
end;  
J=(1/m)*Jtmp; */ 
  
bool LinReg::gradientDescent_basic()
{	
	//double J=(double) INT_MAX;
	int m = X_train.length();
	Matrix grad;
	double J_pre =J, minJ = J;
	Matrix h;
	for(int i=0;i<para.num_iters;i++)
	{
		grad=computeGradient(X_train);
		ML_LIR_INFO("J"<<i<<"="<<J);
		mJ.matrix_[i]=J;
		if(J<minJ)
		{
			minCostRecord.minJ = J;
			minCostRecord.num_iters = i+1;
		}

		if(J<para.converge)
		{
			ML_LIR_INFO("theta converge\n"<<theta);
			mJ=mJ.getRow(0,i);
			return true;
		}
		if(i==para.num_iters-1)
		{
			//para.num_iters = (int)(1.5*para.num_iters);
			//Matrix pre=mJ;
			//mJ = Matrix(1.5*para.num_iters,1,65536.0);
			//mJ.subsituteRow(pre,0,i,0,i);
			ML_LIR_INFO("it has trained "<< para.num_iters<<", and stoped without convergence");
			ML_LIR_INFO("J"<<i<<"="<<J);
			//ML_LIR_INFO("mJ"<<mJ);
			ML_LIR_INFO("theta "<<theta);
			mJ=mJ.getRow(0,i);
			
		}
		if(J_pre<J)
		{
			para.alpha =0.75*para.alpha;
		}
		else
		{
			para.alpha = 1.05*para.alpha;
		}
		theta=theta - grad.multiplyReal(para.alpha); //negative gradient direction
		J_pre = J;
	}
	return false;
	
}
MIN_COST_PARA_RECORD LinReg::getMinCostPara(int index)
{
	return minCostRecord;
}

Matrix LinReg::predictLinear(Matrix p_x)
{
	if(pre_para.bNorm)
	{
		p_x = p_x.getNormWithMuSigma(normal.mu, normal.sigma);
		//p_x = p_x.getNorm(&normal.mu,&normal.sigma); //the fisrt should be x_norm
	}
	if(pre_para.bScale)
	{
		p_x = featureScale(p_x,pre_para.dim);
	}
	else
	{
		p_x.addOneCol(0,1);
	}
	return p_x*theta;
}
//decision bound should use the total X
DECISION_PARAS LinReg::getDesionBoundPara(int index)
{
	DECISION_PARAS re;
	re.theta = theta;
	re.x = X_orig;
	re.bNorm = pre_para.bNorm;
	re.normal = normal;
	return re;
}
void LinReg::setPreProcessPara(PRE_PROCESS_PARA p, int index)
{
	Algo_Basic::setPreProcessPara(p,index);
	if(pre_para.bNorm)
	{
		X_train = X_train.getNorm(&normal.mu,&normal.sigma); //the fisrt should be x_norm
	}
	if(pre_para.bScale)
	{
		X_train = featureScale(X_train,pre_para.dim);
	}
	else
	{
		X_train.addOneCol(0,1);
	}

	ML_LIR_INFO("X_train"<<X_train);
}
//self get self needs 各取所需 参数
void LinReg::setTrainPara(SUPERVISE_PARA p, int index)
{
	Algo_Basic::setTrainPara(p,index);
	//X = X;

	ML_LIR_INFO("after data preprocessingm X : \n"<<X_train);
	//no data preprocessing maybe
	if(para.num_iters ==-1)
	{
		para.num_iters = 1000;
	}
	if(para.alpha == -1)
	{
		para.alpha =0.001;
	}
	if(para.lambda ==-1)
	{
		para.lambda =0;
	}

	ML_LIR_INFO("the training parameters is as follows:");
	ML_LIR_INFO("alpha"<<para.alpha);
	ML_LIR_INFO("lambda"<<para.lambda);
	ML_LIR_INFO("num of iterations"<<para.num_iters);
	return ;
}
//add the 1 col when train data;
bool LinReg::trainData(int index)  
{
	return trainLinear(BASIC_LINEAR);
}

XY_PARA LinReg::predict(std::string filename)
{
	XY_PARA re;
	if(filename.empty())
	{
		re.x_orig = X_orig;
		re.y = predictLinear(X_orig);
		re.rslt = 0;
		ML_LIR_INFO("01: predict of the input\n"<<re.y);
	}
	else
	{
		if(read_file_no_rslt(re.x_orig, filename)==-1)
		{
			ML_LIR_ERROR("LinReg, predict, read file failed!");
			re.rslt = -1;
			return re;
		}
		re.label = labels;
		re.y = predictLinear(re.x_orig);
		re.rslt = 0;
	}
	return re;
}
Matrix LinReg::predict(Matrix x)
{
	Matrix y = predictLinear(x);
	return y;
}
void LinReg::getEvalRslt()
{
	Matrix x = X_orig;
	Matrix y = Y_orig;
	if(bRand)
	{
		x = X_rand;
		y = Y_rand;
	}

	Matrix hypo = predict(x);
	//01. get relative matrixs
	int m = y.length();
	int train_n= (int)(rate_para.train_rate*m);
	Matrix y_train = y.getRow(0,train_n-1);
	Matrix hypo_train = hypo.getRow(0,train_n-1);

	ML_LIR_INFO("train from 0 to "<<train_n-1);

	int cv_n = (int)((rate_para.train_rate+rate_para.cv_rate)*m);
	//int cv_m = cv_n-train_n;
	Matrix y_cv =y.getRow(train_n,cv_n-1);
	Matrix hypo_cv =hypo.getRow(train_n,cv_n-1);
	ML_LIR_INFO("cv from "<<train_n<<" to "<<cv_n-1);

	int test_m = m-cv_n;
	Matrix y_test =y.getRow(cv_n,m-1);
	Matrix hypo_test =hypo.getRow(cv_n,m-1);
	ML_LIR_INFO("test from "<<cv_n<<" to "<<m-1);

    //02. get cost, these are used for model selection
	double Jtrain = computeCostFunc(hypo_train,y_train);
	double Jcv = computeCostFunc(hypo_cv,y_cv);
	double Jtest = computeCostFunc(hypo_test,y_test);

	eval_rslt.Jtrain=Jtrain;
	eval_rslt.Jcv=Jcv;
	eval_rslt.Jtest =Jtest;

	ML_LIR_INFO("the evaulation parameters are as follows:"<<eval_rslt);
	return;
}




