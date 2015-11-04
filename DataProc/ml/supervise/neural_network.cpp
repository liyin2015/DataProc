#include "neural_network.hpp"
#include "../io_manager.hpp"
#include "../comm.hpp"
using namespace std;
/*function [J grad] = nnCostFunction(nn_params, ...  
                                   input_layer_size, ...  
                                   hidden_layer_size, ...  
                                   num_labels, ...  
                                   X, y, lambda)  
%NNCOSTFUNCTION Implements the neural network cost function for a two layer  
%neural network which performs classification  
%   [J grad] = NNCOSTFUNCTON(nn_params, hidden_layer_size, num_labels, ...  
%   X, y, lambda) computes the cost and gradient of the neural network. The  
%   parameters for the neural network are "unrolled" into the vector  
%   nn_params and need to be converted back into the weight matrices.   
%   
%   The returned parameter grad should be a "unrolled" vector of the  
%   partial derivatives of the neural network.  
%  
  
% Reshape nn_params back into the parameters Theta1 and Theta2, the weight matrices  
% for our 2 layer neural network  
Theta1 = reshape(nn_params(1:hidden_layer_size * (input_layer_size + 1)), ...  
                 hidden_layer_size, (input_layer_size + 1));  
  
Theta2 = reshape(nn_params((1 + (hidden_layer_size * (input_layer_size + 1))):end), ...  
                 num_labels, (hidden_layer_size + 1));  
  
% Setup some useful variables  
m = size(X, 1);  
           
% You need to return the following variables correctly   
J = 0;  
Theta1_grad = zeros(size(Theta1));  
Theta2_grad = zeros(size(Theta2));  
  
% ====================== YOUR CODE HERE ======================  
% Instructions: You should complete the code by working through the  
%               following parts.  
%  
% Part 1: Feedforward the neural network and return the cost in the  
%         variable J. After implementing Part 1, you can verify that your  
%         cost function computation is correct by verifying the cost  
%         computed in ex4.m  
disp(size(y));  
%transfer y  
tmpy =[y zeros(m,num_labels-1)];  
for i=1:num_labels,  
   for j=1:m,  
        if y(j)==i  
            tmpy(j,i)=1;  
        else  
            tmpy(j,i)=0;  
        end;  
    end;  
end;  
  
%tmpy =[y zeros(m,num_labels-1)];  
%tmpy=eye(num_  */

NeuralNetwork::NeuralNetwork(int num_in,int num_hl,int* numhno, int num_lab,int train)
{
	num_labels=num_lab;
	trans_func = LINEAR_FUNC;
	//lambda = 0;//default without regularization
	//alpha = DEFAULT_ALPHA;
	max_iters = MAX_ITERS;
	//num_iters =0;
	converge_J = DEFAULT_CONVERG;
	train_para_init();
	is_delta_init = false;
	inc_times=0;
	method = BP_DM;
	minCostRecord.minJ = 65535;
	minCostRecord.num_iters = -1;
}
NeuralNetwork::NeuralNetwork()
{
	num_labels=0;
	//para get through train model or set by yourself
	trans_func = LINEAR_FUNC;
	max_iters = MAX_ITERS;
	converge_J = DEFAULT_CONVERG;
	train_para_init();
	is_delta_init = false;
	inc_times=0;
	minCostRecord.minJ = 65535;
	minCostRecord.num_iters = -1;
}
void NeuralNetwork::train_para_init()
{
	//parameters used for DM method
	para_dm.a = NULL; 
	para_dm.z = NULL;
	para_dm.d=NULL;
	para_dm.delta = NULL;
	para_dm.theta = NULL;
	para_dm.theta_grad = NULL;

	//parameters used for DM_SEF_ADJ method
	para_dm_self_adj.a=NULL;
	para_dm_self_adj.z = NULL;
	para_dm_self_adj.d=NULL;
	para_dm_self_adj.delta = NULL;
	para_dm_self_adj.theta = NULL;
	para_dm_self_adj.theta_grad = NULL;
}
	//void getParaFromTrainMethod(TRAIN_METHOD method);
void NeuralNetwork::alloc_res_for_train()
{
	TRAIN_PARA tp;
	switch(method)
	{
	case BP_DM:
		para_dm.init(para.nn_struct.HiddenLayers);
		randomizeWeight(para_dm);
		break;
	case BP_DM_SELF_ADJ:
		para_dm_self_adj.init(para.nn_struct.HiddenLayers);
		randomizeWeight(para_dm_self_adj);
		break;
	default:
		cout<<"wrong method"<<method<<"use the dm_self_adjust method"<<endl;
		para_dm_self_adj.init(para.nn_struct.HiddenLayers);
		break;

	}
}
void NeuralNetwork::randomizeWeight(TRAIN_PARA& tp)
{
	for(int i=0;i<1+para.nn_struct.HiddenLayers;i++)
	{
		Matrix t;
		if(i==0)
		{
			t=Matrix(para.nn_struct.HiddenNodes[i],para.nn_struct.InputNodes+1);
		}
		else if(i==para.nn_struct.HiddenLayers)
		{
			t=Matrix(num_labels,para.nn_struct.HiddenNodes[i-1]+1);
		}
		else
		{
			//num_per_hidden_layer[i]
			t=Matrix(para.nn_struct.HiddenNodes[i],1+para.nn_struct.HiddenNodes[i-1],0.4);
		}
		t.randomize(SCALEWEIGHT,MAXWEIGHT);
		tp.theta[i]=t;
		ML_NN_INFO("theta : "<<i<<endl<<tp.theta[i]);
	}
}
int NeuralNetwork::loadData(string filename,bool brand, ALGO_TYPE algo_type)
{
	if(Algo_Basic::loadData(filename,brand, algo_type)==-1)
	{
		ML_NN_ERROR("load original data from file failed!");
		return -1;
	}
	num_labels = labels.length();
	if(num_labels ==2)
	{
		num_labels = 1;
	}
	ML_NN_INFO("X_original"<<X_orig);
	ML_NN_INFO("y_original"<<Y_orig);
	ML_NN_INFO("label matrix "<<labels);
	ML_NN_INFO("label number is "<<num_labels);
	if(bRand)
	{
		ML_NN_INFO("x_rand"<<X_rand);
		ML_NN_INFO("y_rand"<<Y_rand);
	}

	return 0;
}
Matrix NeuralNetwork::trans_y_for_multilabels() //change Y to m*num_labels
{
	Matrix tmpy;
	tmpy = Matrix(Y_train.length(),num_labels);

	for(int i=0;i<num_labels;i++)
	{
		for(int j=0;j<Y_train.length();j++) //this should be 0
		{
			if(Y_train.matrix_[j]==labels.matrix_[i])
				tmpy.matrix_[j*num_labels+i]=1;
			else
				tmpy.matrix_[j*num_labels+i]=0;
		}
	}
	return tmpy;
}


double NeuralNetwork::computeCostFunction(TRAIN_METHOD train_method, Matrix h,Matrix y)
{
	//step 1 compute tmpy MXnum_labels
	//double J=(double)INT_MAX;
			//step 1 compute tmpy MXnum_labels

		/*%Step1: compute cost fuction  
Jtmp=0;  
for i=1:m,  
    for k=1:size(tmpy,2),  
        Jtmp=Jtmp+(-tmpy(i,k)*log(h(i,k))-(1-tmpy(i,k))*log(1-h(i,k)));  
    end;  
end;  
  
J=(1/m)*Jtmp;  
J1=0;  
J2=0;  
for i=1:size(Theta1,1)  
    for k=2:size(Theta1,2)  
        J1=J1+Theta1(i,k)^2;  
    end;  
end;  
  
%J2 = sum(sum((Theta1(:,2:size(Theta1,2))').^2   )')+sum(sum((Theta2(:,2:size(Theta2,2))').^2   )');  
J1=((lambda)/(2*m))*J1;  
  
for i=1:size(Theta2,1)  
    for k=2:size(Theta2,2)  
        J2=J2+Theta2(i,k)^2;  
    end;  
end;  
J2=((lambda)/(2*m))*J2;  
J=J+J1+J2;  */
	//cout<<"1"<<endl;
	double J = 65535;
	TRAIN_PARA* tp;
	switch(train_method)
	{
	case BP_DM:
		tp = &para_dm;
		break;
	case BP_DM_SELF_ADJ:
		tp=&para_dm_self_adj;
		break;
	default:
		cout<<"wrong method"<<train_method<<"use the dm_self_adjust method"<<endl;
		tp=&para_dm_self_adj;
		break;

	}
	int m = y.length();
	Matrix ones(malloc_2(1,h.length(),h.brandth()),h.length(),h.brandth());

	Matrix tmp1= y^(h.getLog(e));
	Matrix tmp2=((ones-y)^((ones-h).getLog(e)));

	Matrix sum = (tmp1+tmp2).sumPerCol().sumPerRow();

	J = (-1.0/m)*sum.matrix_[0];

	//if reguarization
	double J1=tp->theta[0].pointPow(2).sumPerCol().sumPerRow().matrix_[0];
	double J2=tp->theta[1].pointPow(2).sumPerCol().sumPerRow().matrix_[0];

	J+=((para.lambda)/(2*m))*(J1+J2);
	return J;
}
Matrix NeuralNetwork::computeHypoFunc(TRAIN_METHOD train_method,Matrix x)//forward propagation
{
	//compute hx  
/*a1=X;  
a1=[ones(m,1) X];  
  
z2=a1*Theta1';  
a2=sigmoid(z2);  
a2=[ones(size(a2,1),1) a2];  
  
z3=a2*Theta2';  
a3=sigmoid(z3);  
  
h=a3;  
disp(size(h)); */
		//a1= X;
		//Matrix* a = new Matrix[1+num_hidden_layer+1];
		//Matrix* z = new Matrix[1+num_hidden_layer+1];//a2->z2
	//if(x.brandth()!=num_input)
	//{
		//x.addOneCol(0,1);
	//}
	TRAIN_PARA* tp;
	switch(train_method)
	{
	case BP_DM:
		tp = &para_dm;
		break;
	case BP_DM_SELF_ADJ:
		tp=&para_dm_self_adj;
		break;
	default:
		cout<<"wrong method"<<train_method<<"use the dm_self_adjust method"<<endl;
		tp=&para_dm_self_adj;
		break;

	}
	//Matrix x_tmp = x;
	//x_tmp.addOneCol(0,1);
	//x.addOneCol(0,1);
	for(int i=0;i<1+para.nn_struct.HiddenLayers+1;i++)
	{
		if(i==0) //deal with input layer
		{				
			tp->a[i]=x;
		}
		else if(i==para.nn_struct.HiddenLayers+1)//deal with output layer
		{
			tp->z[i]=tp->a[i-1]*(tp->theta[i-1].trans());
			tp->a[i]=sigmoid(tp->z[i]);
		}
		else//deal with hidden layer
		{
			tp->z[i]=tp->a[i-1]*(tp->theta[i-1].trans());
			tp->a[i]=sigmoid(tp->z[i]);
			tp->a[i].addOneCol(0,1);
		}

	}
	Matrix h=tp->a[1+para.nn_struct.HiddenLayers];
	return h;
}
/*Matrix NeuralNetwork::gradientDescent()
{

}*/
/*% Part 2: Implement the backpropagation algorithm to compute the gradients  
%         Theta1_grad and Theta2_grad. You should return the partial derivatives of  
%         the cost function with respect to Theta1 and Theta2 in Theta1_grad and  
%         Theta2_grad, respectively. After implementing Part 2, you can check  
%         that your implementation is correct by running checkNNGradients  
%  
%         Note: The vector y passed into the function is a vector of labels  
%               containing values from 1..K. You need to map this vector into a   
%               binary vector of 1's and 0's to be used with the neural network  
%               cost function.  
%  
d3=a3-tmpy;  
disp("d3");  
%disp(size(d3));  
  
d2=(d3*Theta2(:,2:size(Theta2,2))).*sigmoidGradient(z2);  
Theta1_grad = (1/m).*((d2)'*a1);  
Theta2_grad=(1/m).*((d3)'*a2);  
  
Theta1_grad(:,2:end) = Theta1_grad(:,2:end)+(lambda/m).*Theta1(:,2:end);  
Theta2_grad(:,2:end)=Theta2_grad(:,2:end)+(lambda/m).*Theta2(:,2:end);  

//nnCostFunction(nn_params, input_layer_size,hidden_layer_size, num_labels,   X, y, lambda)*/
void NeuralNetwork::backPropagation(TRAIN_METHOD train_method, Matrix h,Matrix tmpy)//back propagation
{
	//cout<<"lambda"<<lambda<<endl;
	TRAIN_PARA* tp;
	switch(train_method)
	{
	case BP_DM:
		tp = &para_dm;
		break;
	case BP_DM_SELF_ADJ:
		tp=&para_dm_self_adj;
		break;
	default:
		cout<<"wrong method"<<train_method<<"use the dm_self_adjust method"<<endl;
		tp=&para_dm_self_adj;
		break;

	}		
	for(int i=para.nn_struct.HiddenLayers+1;i>=0;i--)
	{
		if(i==para.nn_struct.HiddenLayers+1)//deal with output layer
		{
			tp->d[i] = h-tmpy;
		}
		else if(i==0)//deal with input layer, when at input layer, no need to compute anything
		{
		}
		else //deal with hidden layers
		{
			int row = tp->theta[i].length();
			int col= tp->theta[i].brandth();
			Matrix tmp=tp->d[i+1]*(tp->theta[i].getCol(1,col-1));
			tp->d[i] = tmp^sigmoidGradient(tp->z[i]);
		}
	}

	if(!is_delta_init)
	{
		for(int i=0;i<para.nn_struct.HiddenLayers+1;i++)//the last layer has no deleta[i]
		{
			tp->delta[i]=Matrix(tp->d[i+1].brandth(),tp->a[i].brandth());
		}
		is_delta_init = true;
	}
}
Matrix NeuralNetwork::sigmoidGradient(Matrix z)
{
	Matrix ones(z.row_,z.col_,1);
	Matrix re= sigmoid(z)^(ones-sigmoid(z));
	return re;

}
bool NeuralNetwork::gradientDescenttrainBP_dm()
{
	ML_NN_INFO("train bp");

	//double J= (double)INT_MAX;
	Matrix h;
	Matrix y = trans_y_for_multilabels();
	double J_pre = (double)INT_MAX,minJ=65535;

	//the input layer add one col
	Matrix x =X_train;
	//x.addOneCol(0,1);

	int m = X_train.length();
	for(int i=0;i<para.num_iters;i++)
	{
		h=computeHypoFunc(method,x);//forward propagation

		J = computeCostFunction(method,h,y);
		*mJ(i,0) = J;
		ML_NN_INFO("J"<<i<<":"<<J);

		if(i ==para.num_iters -1)
		{
			mJ=mJ.getRow(0,i);
			ML_NN_INFO("it has trained "<< para.num_iters<<", and stoped without convergence");
			ML_NN_INFO("J"<<i<<"="<<J);
			for(int i = 0;i<para.nn_struct.HiddenLayers+1;i++)
			{
				ML_NN_INFO("theta"<<i<<":"<<para_dm.theta[i]);
			}
			return false;
		}
		*mJ(i,0) = J;
		if(J<minJ)
		{
			minCostRecord.num_iters = i+1;
			minCostRecord.minJ = J;			
		}
		if(J<converge_J)
		{
			mJ=mJ.getRow(0,i);
			for(int i = 0;i<para.nn_struct.HiddenLayers+1;i++)
			{
				ML_NN_INFO("theta"<<i<<":"<<para_dm.theta[i]);
			}
			return true;
		}
		if(J_pre<J)
		{			
			//inc_times++;
			//if(inc_times>2)
			//{
				//gradientDescenttrainBP_dm((para.alpha)/10);
			//}
		}
		backPropagation(method,h,y);
		Matrix* tmp_theta = new Matrix[1+para.nn_struct.HiddenLayers];
		for(int i=0;i<1+para.nn_struct.HiddenLayers;i++)
		{
			//get 增量
			cout<<"1"<<endl;
			para_dm.delta[i]= (para_dm.delta[i]+(para_dm.d[i+1].trans())*para_dm.a[i]);
			cout<<"12"<<endl;
			//增量均分
			para_dm.theta_grad[i]=para_dm.delta[i].multiplyReal(1.0/m);
			int row = para_dm.theta_grad[i].length();
			int col = para_dm.theta_grad[i].brandth();
			//regularization处理
			cout<<"13"<<endl;
			Matrix tmp1 = para_dm.theta_grad[i].getCol(1,col-1);
			Matrix tmp2 = para_dm.theta[i].getCol(1,para_dm.theta[i].brandth()-1);//can improve thete i-1 to i, in the later
			tmp_theta[i]= tmp1+tmp2.multiplyReal(para.lambda/m);
	cout<<"14"<<endl;
			para_dm.theta_grad[i].subsituteByIndex(tmp_theta[i],0,tmp_theta[i].brandth()-1,1,col-1);
			//core of the optimization part, thethai = thetai+alpha*(d(i-1)-d(i))
			if(i==0)
			{
				cout<<"15"<<endl;
			    para_dm.theta[i]=para_dm.theta[i]-para_dm.theta_grad[i].multiplyReal(para.alpha);//this shouble be negative gradient direction
				cout<<"17"<<endl;
			}
			else
			{
				cout<<"16"<<endl;
				//para_dm.theta[i]=para_dm.theta[i]+((para_dm.theta_grad[i-1]-para_dm.theta_grad[i])).multiplyReal(alpha);
				para_dm.theta[i]=para_dm.theta[i]+((para_dm.theta_grad[i])).multiplyReal(para.alpha);
				cout<<"18"<<endl;
			}

		}

		delete[] tmp_theta;
	}
	for(int i=0;i<1+para.nn_struct.HiddenLayers;i++)
	{
		cout<<endl<<para_dm.theta[i];
	}
	return false; //since it has not converged

}

bool NeuralNetwork::trainNN()
{
	bool rslt = false;
	alloc_res_for_train();
	switch(method)
	{
		
	case BP_DM:	
		rslt = gradientDescenttrainBP_dm();//可以使用函数指针，以后升级
		break;
		
	case BP_DM_SELF_ADJ:
		rslt =gradientDescenttrainBP_dm_self_adjust();
		break;
default:
	cout<<"no such train method"<<method<<endl;
	return false;

	}
	return rslt;
	
}
bool NeuralNetwork::gradientDescenttrainBP_dm_self_adjust()
{
	TRAIN_METHOD method= BP_DM_SELF_ADJ;
	Matrix h;
	//get y = m*num_labels
	Matrix y = trans_y_for_multilabels();
	double J_pre =(double)INT_MAX,minJ = 65535;

	//get x
	Matrix x =X_train;
	x.addOneCol(0,1);
	int m = X_train.length();

	double alpha = para.alpha;
	for(int i=0;i<para.num_iters;i++)
	{
		h=computeHypoFunc(method,x);//forward propagation

		J = computeCostFunction(method,h,y);
		*mJ(i,0) = J;
		ML_NN_INFO("J"<<i<<":"<<J);
		if(i ==para.num_iters -1)
		{
			mJ=mJ.getRow(0,i);
			ML_NN_INFO("it has trained "<< para.num_iters<<", and stoped without convergence");
			ML_NN_INFO("J"<<i<<"="<<J);
			for(int i = 0;i<para.nn_struct.HiddenLayers+1;i++)
			{
				ML_NN_INFO("theta"<<i<<":"<<para_dm_self_adj.theta[i]);
			}
			return false;
		}
		*mJ(i,0) = J;
		if(J<minJ)
		{
			minCostRecord.num_iters = i+1;
			minCostRecord.minJ = J;			
		}
		if(J<para.converge)
		{
			mJ=mJ.getRow(0,i);
			for(int i = 0;i<para.nn_struct.HiddenLayers+1;i++)
			{
				ML_NN_INFO("theta"<<i<<":"<<para_dm_self_adj.theta[i]);
			}
			return true;
		}
		if(J_pre<J)
		{
			
			//inc_times++;
			//if(inc_times>2)
			//{
				//inc_times = 0;
				//gradientDescenttrainBP_dm(init_alpha/5);
			//}
			//else
			//{
				//init_alpha = 0.7*init_alpha;
			//}
		}
		else
		{
			alpha = 1.002*alpha;//this increse factor is especial sensitive
		}
		//BP算法得deleta[i]
		backPropagation(method,h,y);

		Matrix* tmp_theta = new Matrix[1+para.nn_struct.HiddenLayers];
		for(int i=0;i<1+para.nn_struct.HiddenLayers;i++)
		{
			//get 增量
			para_dm_self_adj.delta[i]= (para_dm_self_adj.delta[i]+(para_dm_self_adj.d[i+1].trans())*para_dm_self_adj.a[i]);

			//增量均分
			para_dm_self_adj.theta_grad[i]=para_dm_self_adj.delta[i].multiplyReal(1.0/m);
			//para_dm_self_adj.theta_grad[i].printMatrix();

			int row = para_dm_self_adj.theta_grad[i].length();
			int col = para_dm_self_adj.theta_grad[i].brandth();
			//regularization处理
			Matrix tmp1 = para_dm_self_adj.theta_grad[i].getCol(1,col-1);
			Matrix tmp2 = para_dm_self_adj.theta[i].getCol(1,para_dm_self_adj.theta[i].brandth()-1);//can improve thete i-1 to i, in the later
	
			tmp_theta[i]= tmp1+tmp2.multiplyReal(para.lambda/m);
			//tmp_theta[i].printMatrix();

	        //获得Regular的增量
			para_dm_self_adj.theta_grad[i].subsituteByIndex(tmp_theta[i],0,tmp_theta[i].brandth()-1,1,col-1);
			//para_dm_self_adj.theta_grad[i].printMatrix();
			//core of the optimization part, thethai = thetai+alpha*(d(i-1)-d(i))
			if(i==0)
			{
			    para_dm_self_adj.theta[i]=para_dm_self_adj.theta[i]-para_dm_self_adj.theta_grad[i].multiplyReal(alpha);//this shouble be negative gradient direction
			}
			else
			{
				para_dm_self_adj.theta[i]=para_dm_self_adj.theta[i]+((para_dm_self_adj.theta_grad[i-1]-para_dm_self_adj.theta_grad[i])).multiplyReal(alpha);
				//para_dm_self_adj.theta[i]=para_dm_self_adj.theta[i]-((para_dm_self_adj.theta_grad[i])).multiplyReal(init_alpha);
			}

		}

		delete[] tmp_theta;
	}
	for(int i=0;i<1+para.nn_struct.HiddenLayers;i++)
	{
		cout<<endl<<para_dm_self_adj.theta[i];
	}
	return false;

}

Matrix NeuralNetwork::predictNN(Matrix x)
{
	//Matrix z;
	if(pre_para.bNorm)
	{
		//x=x.getNorm(NULL,NULL);
		x = x.getNormWithMuSigma(normal.mu,normal.sigma);
	}
	if(pre_para.bScale)
	{
		x=featureScale(x,pre_para.dim);
	}
	else
	{
		x.addOneCol(0,1);
	}
	Matrix h=computeHypoFunc(method,x);
	return h;
}
XY_PARA NeuralNetwork::getParasDrawInput()
{
	XY_PARA re =Algo_Basic::getParasDrawInput();
	re.label = labels;
	return re;
}
DECISION_PARAS NeuralNetwork::getDesionBoundPara(int index)
{
	DECISION_PARAS re;
	//re.theta
	return re;
}
void NeuralNetwork::setRates(RATE_PARA pa)
{
	Algo_Basic::setRates(pa);
	ML_NN_INFO("train_rate = "<<rate_para.train_rate<<", cv rate = "<<rate_para.cv_rate<<" , test rate = "<<rate_para.test_rate);
}
void NeuralNetwork::setPreProcessPara(PRE_PROCESS_PARA p, int index)
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

	ML_NN_INFO("X_train"<<X_train);
}
//no distinguishment between different index, start from x_train, X_train get from X_rand or X_orig
void NeuralNetwork::setTrainPara(SUPERVISE_PARA pa, int index)
{
	//here start with X
	Algo_Basic::setTrainPara(pa,index); 
	//para.train_rate = 1;
	//X = X_orig;
	//data processing


	/*if(para.bNorm)
	{
		X_train= X_train.getNorm(NULL,NULL);
	}
	if(para.bScale)
	{
		X_train = featureScale(X_train,para.dim); //already add one col
	}
	else
	{
		X_train.addOneCol(0,1);
	}*/
	ML_NN_INFO("after data preprocessingm X : \n"<<X_train);
	//input and output layer
	para.nn_struct.InputNodes = X_train.brandth();
	para.nn_struct.OutputNodes = Y_train.brandth();

	//converge_J = 1.0e-7;

	//set method that used to train
	method = (TRAIN_METHOD)(pa.train_method);

	ML_NN_INFO("the parameters are as follows: ");
	ML_NN_INFO("neural network parameters: inputNodes = "<< para.nn_struct.InputNodes<<", outputNodes = "<<para.nn_struct.OutputNodes<<" ,hidden layers = "<<para.nn_struct.HiddenLayers);
	for(int i = 0;i <para.nn_struct.HiddenLayers;i++)
	{
		ML_NN_INFO("Layer "<<i<<" = "<<para.nn_struct.HiddenNodes[i]);
	}
	ML_NN_INFO("num_iters = "<<para.num_iters<<", lambda = "<<para.lambda<<" , alpha = "<<para.alpha<<" , converge = "<<para.converge);
}

bool NeuralNetwork::trainData(int index)
{
	return trainNN();
	//return true;
}
XY_PARA NeuralNetwork::predict(std::string filename)
{
	XY_PARA re;
	//TRAIN_METHOD method = BP_DM;
	if(filename.empty())
	{
		re.x_orig = X_orig;
		re.y = predictNN(X_orig);
		re.rslt = 0;
		ML_LIR_INFO("01: predict of the input\n"<<re.y);
	}
	else
	{
		if(read_file_no_rslt(re.x_orig, filename)==-1)
		{
			ML_NN_ERROR("NeuralNetwork, predict, read file failed!");
			re.rslt = -1;
			return re;
		}
		re.label = labels;
		re.y = predictNN(re.x_orig);
		re.rslt = 0;
	}
	return re;
}
Matrix NeuralNetwork::predict(Matrix x)
{
	Matrix y = predictNN(x);
	return y;
}