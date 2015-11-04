#include "svm.hpp"
#include "logistic_regression.hpp"
#include <algorithm>
#include <math.h>
using namespace std;
double max(double a, double b)
{
	return a>b?a:b;
}
double min(double a, double b)
{
	return a<b?a:b;
}
SVM::SVM()
{
	//m=0;
	//n=0;
	//C = 1.0; //c ==1
	//tol = 1e-3;//tol
	//max_iters = MAX_ITERS;
	//train_rate = 0.6;
	//b = 0;

	num_labels =0;
	theta_mul = NULL;
	is_multiclass = false;
	X_multi = NULL;
	is_normed = NULL;

	is_scaled = NULL;
	scale_dimension = NULL;
	b = NULL;
	kernel = NULL;
	kernel_type = NULL;
	alphas = NULL;
	normal_multi = NULL;
	//rslt_eval =-1; how to init the struct
}
SVM::~SVM()
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
	if(kernel!=NULL)
	{
		delete[] kernel;
		kernel=NULL;
	}
	if(kernel_type!=NULL)
	{
		delete[] kernel_type;
		kernel_type=NULL;
	}

	if(b!=NULL)
	{
		delete[] b;
		b=NULL;
	}
	if(alphas!=NULL)
	{
		delete[] alphas;
		alphas=NULL;
	}
	if(normal_multi != NULL)
	{
		delete[] normal_multi;
		normal_multi = NULL;
	}
}
int SVM::loadData(string filename, bool brand,ALGO_TYPE algo_type)
{
	if(Algo_Basic::loadData(filename,brand, algo_type)==-1)
	{
		ML_SVM_ERROR("load original data from file failed!");
		return -1;
	}
	//deal with num labels
	num_labels= labels.length();
	if(num_labels ==2)
	{
		num_labels =1;
	}
	ML_SVM_INFO("X_original"<<X_orig);
	ML_SVM_INFO("y_original"<<Y_orig);
	ML_SVM_INFO("label matrix "<<labels);
	ML_SVM_INFO("label number is "<<num_labels);
	//deal with brand
	if(bRand)
	{
		ML_SVM_INFO("x_rand"<<X_rand);
		ML_SVM_INFO("y_rand"<<Y_rand);
	}



	return num_labels;

}

void SVM::setRates(RATE_PARA pa)
{
	Algo_Basic::setRates(pa);
	ML_SVM_INFO("train_rate = "<<rate_para.train_rate<<", cv rate = "<<rate_para.cv_rate<<" , test rate = "<<rate_para.test_rate);

	//init for svm
	normal_multi= new NORMALIZATION[num_labels];
	//normal_multi->sigma = new Matrix[num_labels];
	X_multi = new Matrix[num_labels];
	theta_mul = new Matrix[num_labels];
	kernel = new Matrix[num_labels];
	alphas = new Matrix[num_labels];

	//init for audiuaty method
	is_normed = new bool[num_labels];
	is_scaled = new bool[num_labels];
	scale_dimension = new int[num_labels];
	b= new double[num_labels];
	//init data
	for(int i=0;i<num_labels;i++)
	{
		//X_multi[i] = X_train;
		is_normed[i] = false;

		is_scaled[i] = false;
		scale_dimension[i] = 0;
		b[i]=0;		
	}

}

void SVM::initWeight(int index)
{
	//alpha is got through algorithm
	//int train_m =(int)(X_multi[index].length()*para.train_rate);
	int m = X_multi[index].length();
	int n = X_multi[index].brandth();
	theta_mul[index]=Matrix(n,1);
	ML_SVM_INFO("initWight:");
	ML_SVM_INFO("theta mul to zeros");
	alphas[index] = Matrix(m,1);
	ML_SVM_INFO("alpha mul to zeros");
}


void SVM::trainForSigClass(int index)
{
	//x,y,c,kernelfunction,tol, max_passes;	
	//get paras from users
	//directForTrain(index);
	//init
	initWeight(index);
	//init variables;

	//int train_m = (int)(X_multi[index].length()*rate_para.train_rate);
	
	//Matrix y_train = Y.getRow(0,train_m-1);
	//Matrix x_train = X_multi[index].getRow(0,train_m-1);
	Matrix x_train = X_multi[index];
	int m = X_train.length();
	//get yindex
	Matrix y_train = Y_train;
	if(num_labels>2)
	{
		for(int i=0;i<Y_train.length();i++)
		{
			if(Y_train.matrix_[i] ==labels.matrix_[index])
			{
				y_train.matrix_[i]=1;
			}
			else
			{
				y_train.matrix_[i]=-1;
			}
		}
	}
	ML_SVM_INFO("x_train"<<x_train);
	ML_SVM_INFO("y_train"<<y_train);
	//set parameters
	Matrix error(m,1);
	double eta =0;
	double L=0,H=0;
	bool num_changed_alphas = false;
	double b_t=b[index];
	//Matrix kerneli = kernel[index].getCol();
	int pass =0;

	while(pass<para.num_iters)
	{
		num_changed_alphas = false;
		for(int i=0;i<m;i++)
		{
			Matrix kerneli = kernel[index].getCol(i,i);
			double yi=*y_train(i,0);
			//step1: calculate error i
			*error(i,0) = b_t+ (alphas[index]^y_train^kerneli).sumPerCol().matrix_[0] -yi;
			double errori= *error(i,0);
			//step2: testify alpha i
			if((yi*errori<-para.tol && *alphas[index](i,0)<para.C) || (yi*errori>para.tol && *alphas[index](i,0)>0))
			{
				//step3: choose alpha j
				int j = randint(m);
				while(j==i)
				{
					j=randint(m);
				}
				//step4:calculate error j
				double yj=*y_train(j,0);
				Matrix kernelj = kernel[index].getCol(j,j);
				*error(j,0) = b_t+ (alphas[index]^y_train^kernelj).sumPerCol().matrix_[0] -yj;
				double errorj= *error(j,0);
				//step5: save old alpha
				double alphai_old = *alphas[index](i,0);
				double alphaj_old = *alphas[index](j,0);
				//step 6: compute L and H
				if(yi==yj)
				{
					L=max(0,*alphas[index](i,0)+*alphas[index](j,0)-para.C);
					H=min(para.C,*alphas[index](i,0)+*alphas[index](j,0));
				}
				else
				{
					L=max(0,*alphas[index](j,0)-*alphas[index](i,0));
					H=min(para.C,para.C+*alphas[index](j,0)-*alphas[index](i,0));
				}
				//is L==H, continue to next i
				if(L==H)
				{
					continue;
				}
				//step 7: compute eta
				eta = 2*(*kernel[index](i,j)) -(*kernel[index](i,i)) -(*kernel[index](j,j));
				if(eta >=0)
				{
					continue;
				}
				//step 8: compute and clip new value for alpha j
				*alphas[index](j,0) = *alphas[index](j,0)-(yj*(errori-errorj))/eta;
				//clip
				*alphas[index](j,0) = min(H,*alphas[index](j,0));
				*alphas[index](j,0)=max(L,*alphas[index](j,0));
				//step 9:check if change in alpha is signficant
				if(abs(*alphas[index](j,0) - alphaj_old) <para.tol)
				{
					*alphas[index](j,0) = alphaj_old;
					continue; //did not change
				}

				//step10:compute alphai using alphaj
				*alphas[index](i,0) = *alphas[index](i,0)+yi*yj*(alphaj_old-*alphas[index](j,0));
				//step 11: compute b
				double b1 = b_t-errori\
					-yi*(*alphas[index](i,0)-alphai_old)*(*kernel[index](i,i))\
					-yj*(*alphas[index](j,0)-alphaj_old)*(*kernel[index](i,j));
				double b2 = b_t-errorj\
					-yi*(*alphas[index](i,0)-alphai_old)*(*kernel[index](i,j))\
					-yj*(*alphas[index](j,0)-alphaj_old)*(*kernel[index](j,j));
				if(0<*alphas[index](i,0) &&(*alphas[index](i,0)<para.C))
				{
					b_t=b1;
					ML_SVM_INFO("b_t1"<<b_t);
				}
				else if(0<*alphas[index](j,0) &&(*alphas[index](j,0)<para.C))
				{
					b_t = b2;
					ML_SVM_INFO("b_t2"<<b_t);
				}
				else
				{
					b_t=(b1+b2)/2;
					ML_SVM_INFO("b_t3"<<b_t);
				}
				
				b[index] = b_t;
				num_changed_alphas++;
			}//end of if yi;
		}//end of i
		if(num_changed_alphas ==0)
		{
			pass++;
		}
		else
		{
			pass =0;
		}
	}//end of for pass

	theta_mul[index]= x_train.trans()*(alphas[index]^y_train);
	ML_SVM_INFO("theta mul"<<index<<"\n"<<theta_mul[index]);
	ML_SVM_INFO("d "<<index<<"\n"<<b[index]);
	ML_SVM_INFO("alpha"<<index<<"\n"<<alphas[index]);//alpha have no necessary to store
}//end of func

void SVM::svm_train()
{
	for(int i=0;i<num_labels;i++)
	{
		cout<<"will train label "<<i+1<<endl;
		trainForSigClass(i);
	}
	return;
}
Matrix SVM::predictSvm(Matrix p_x)
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
				new_px=old_p_x.getNormWithMuSigma(normal_multi[i].mu,normal_multi[i].sigma);
			}
			if(is_scaled[i])
			{
				cout<<"index"<<i<<"dim"<<scale_dimension[i]<<endl;
				new_px=featureScale(old_p_x,scale_dimension[i]);
				new_px = new_px.getCol(1,new_px.brandth());
			}
			//new_px.addOneCol(0,1);
			Matrix tmph;
			//tmph =predictLogSigClass(X_multi[i],theta_mul[i]);
			tmph =predictSvmSigClass(i,new_px,theta_mul[i],b[i]);
			h.subsituteByIndex(tmph,0,0,i,i);
		}
		Matrix tmp = h.trans();		
		h = tmp.getMaxIndexPerCol();
		//return h;
	}
	else if(num_labels ==1)
	{
		Matrix new_px = p_x;

		if(is_normed[0])
		{
			new_px=old_p_x.getNormWithMuSigma(normal_multi[0].mu,normal_multi[0].sigma);
		}
		if(is_scaled[0])
		{
			new_px=featureScale(old_p_x,scale_dimension[0]);
			new_px = new_px.getCol(1,new_px.brandth());
		}
		//new_px.addOneCol(0,1);

		h = predictSvmSigClass(0,new_px,theta_mul[0],b[0]);
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
	}

	ML_SVM_INFO("h"<<h);
	return h;
}
Matrix SVM::predictSvmSigClass(int index, Matrix p_x, Matrix the, double bi)
{
	int m=p_x.length();
	//Matrix h(m,1);
	Matrix bm(m,1,bi);
	//h=p_x*the +bm;
	//Matrix tmp_y=trans_Y.getCol(index,index);//two differnet represent way of predict but the same answer
		/*for(int i=0;i<m;i++)
		{
			Matrix kerneli = kernel[index].getCol(i,i);
			//double yi=*trans_Y(i,0);
			//step1: calculate error i
			*h(i,0) = b[index]+ (alphas[index]^tmp_y^kerneli).sumPerCol().matrix_[0];
		}*/

		Matrix h2(m,1);
		h2 = p_x*the+bm;
		//Matrix hm=h2-h;
		//cout<<"hm-h"<<endl<<hm<<endl;
	return h2;
}

XY_PARA SVM::getParasDrawInput()
{
	XY_PARA re= Algo_Basic::getParasDrawInput();
	re.label = labels;
	return re;
}
DECISION_PARAS SVM::getDesionBoundPara(int index)
{
	DECISION_PARAS re;

	re.x = X_multi[index];
	//re.y=Y_orig;
	re.theta = theta_mul[index];
	re.b = b[index];
	re.bNorm = is_normed[index];
	re.normal = normal_multi[index];
	return re;
}
void SVM::setPreProcessPara(PRE_PROCESS_PARA p, int index)
{
	Algo_Basic::setPreProcessPara(p,index);
	if(pre_para.bNorm)
	{
		X_multi[index]=X_train.getNorm(&normal_multi[index].mu,&normal_multi[index].sigma);
		is_normed[index]=true;
	}
	if(pre_para.bScale)
	{
		X_multi[index] = featureScale(X_multi[index],pre_para.dim);
		is_scaled[index] =true;
		scale_dimension[index] = pre_para.dim;
	}

	X_multi[index].getCol(1,X_multi[index].length()-1);

	ML_LOR_INFO("X_train"<<X_multi[index]);
}
//this will start witl X_train, Y_train
void SVM::setTrainPara(SUPERVISE_PARA p, int index)
{
	Algo_Basic::setTrainPara(p,index); //get X_train and Y_train
	//ML_SVM_INFO("X_train"<<X_train);
	//ML_SVM_INFO("y_train"<<Y_train);
	//for svm, if is is 0, transfer it to -1, and extend to num_labels col
	if(labels.MFindByValue(0)) //find 0 in label, transfer them
	{
		for(int i=0;i<Y_train.length();i++)
		{
			if(Y_train.matrix_[i] ==1)
			{
				Y_train.matrix_[i]=1;
			}
			else
			{
				Y_train.matrix_[i]=-1;
			}
		}
	}

	ML_SVM_INFO("transfered y\n"<<Y_train);

	//no need of the first 1 col
	//X_multi[index] = X_train.getCol(1,X_train.length()-1);

	//setting the preprocessing
	/*if(para.bNorm)
	{
		X_multi[index] = X_multi[index].getNorm(&normal_multi[index].mu,&normal_multi[index].sigma);
		is_normed[index] = true;
	}
	if(para.bScale)
	{
		X_multi[index] = featureScale(X_multi[index],para.dim);
		X_multi[index] = X_multi[index].getCol(1,X_multi[index].length()-1);
		is_scaled[index]=true;
		scale_dimension[index]=para.dim;
	}*/

	ML_SVM_INFO("setTrainPara, "<<"X"<<index<<endl<<X_multi[index]);

	//deal with parameters
	para.kernel_type = LINEAR;
	if(para.kernel_type !=-1)
	{
		switch(para.kernel_type)
		{
			
		case LINEAR:
			kernel[index]=linearKernel(X_multi[index]);
			break;
		case GASSIAN:
			{
				double sigma = 2;
				kernel[index]=gassianKernel(X_multi[index],sigma);
			}
			break;
		default:
			break;
		}
	}
	ML_SVM_INFO("SVM related parameters are as follows:");
	ML_SVM_INFO("num_iterations = "<<para.num_iters<<", C = "<<para.C<<" , tol = "<<para.tol<<" ,kernel type = "<<para.kernel_type);
	//ML_SVM_INFO("kernel of X is:"<<kernel[index]);
	return;
}
bool SVM::trainData(int index)
{
	//svm_train();
	ML_SVM_INFO("will train label "<<index+1);
	trainForSigClass(index);
	return true;
}
XY_PARA SVM::predict(std::string filename)
{
	XY_PARA re;
	if(filename.empty())
	{
		re.x_orig = X_orig;
		re.y = predictSvm(X_orig);
		re.rslt = 0;
		ML_SVM_INFO("x_orig "<<re.x_orig);
		ML_LIR_INFO("01: predict of the input\n"<<re.y);
	}
	else
	{
		if(read_file_no_rslt(re.x_orig, filename)==-1)
		{
			ML_SVM_ERROR("SVM, predict, read file failed!");
			re.rslt = -1;
			return re;
		}
		re.label = labels;
		re.y = predictSvm(re.x_orig);
		re.rslt = 0;
	}
	return re;
}
Matrix SVM::predict(Matrix x)
{
	Matrix y = predictSvm(x);
	return y;
}
void SVM::getEvalRslt()
{
	Matrix x = X_orig;
	Matrix y = Y_orig;
	if(bRand)
	{
		x = X_rand;
		y = Y_rand;
	}

	Matrix hypo = predictSvm(x);
	//01. get relative matrixs
	int m = y.length();
	int train_n= (int)(rate_para.train_rate*m);
	Matrix y_train = y.getRow(0,train_n-1);
	Matrix hypo_train = hypo.getRow(0,train_n-1);

	ML_SVM_INFO("train from 0 to "<<train_n-1);

	int cv_n = (int)((rate_para.train_rate+rate_para.cv_rate)*m);
	//int cv_m = cv_n-train_n;
	Matrix y_cv =y.getRow(train_n,cv_n-1);
	Matrix hypo_cv =hypo.getRow(train_n,cv_n-1);
	ML_SVM_INFO("cv from "<<train_n<<" to "<<cv_n-1);

	int test_m = m-cv_n;
	Matrix y_test =y.getRow(cv_n,m-1);
	Matrix hypo_test =hypo.getRow(cv_n,m-1);
	ML_SVM_INFO("test from "<<cv_n<<" to "<<m-1);
	//01. get accuracy
	eval_rslt.train_accuracy = eval_rslt.getAccuracy(hypo_train,y_train);
	eval_rslt.test_accuracy = eval_rslt.getAccuracy(hypo.getRow(train_n,m-1),y.getRow(train_n,m-1));
	eval_rslt.getPrecision(hypo,y,labels);

    //02. get cost, these are used for model selection
	//eval_rslt.Jtrain=computeCostFunc(hypo_train,y_train);
	//eval_rslt.Jcv=computeCostFunc(hypo_cv,y_cv);
	//eval_rslt.Jtest =computeCostFunc(hypo_test,y_test);

	ML_SVM_INFO("the evaulation parameters are as follows:"<<eval_rslt);
	return;
}