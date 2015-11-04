#ifndef FEATURE_SCALLING_CPP
#define FEATURE_SCALLING_CPP
#include <iostream>
#include <algorithm>
using namespace std;
#include "feature_scalling.hpp"
#include "io_manager.hpp"
/*function [X_norm, mu, sigma] = featureNormalize(X)  
%FEATURENORMALIZE Normalizes the features in X   
%   FEATURENORMALIZE(X) returns a normalized version of X where  
%   the mean value of each feature is 0 and the standard deviation  
%   is 1. This is often a good preprocessing step to do when  
%   working with learning algorithms.  */
/*x=mXn,                      */
//X_norm = X;  
//disp(size(X_norm));  
//m = size(X,1);  
//mu = zeros(1, size(X, 2));  
//sigma = zeros(1, size(X, 2));  

//mu = mean(X);  
//sigma = std(X); //sigma = sqrt(1/N*(xi-u)^2)  
//disp('mu'),disp(mu);  
//disp('sigma'),disp(sigma);  
  
//for i=1:m;  
    //X_norm(i,:) = (X(i,:)-mu )./ sigma;  
//end; 
  
//You need to set these values correctly 
double** featureNormalize(double** x, int m, int n)
{

	double** X_norm=malloc_2(0,m,n);
	double* mu=malloc_1<double>(n);
	double *sigma=malloc_1<double>(n);
	mu= getMeanV(x,m,n);
	sigma = getStdV(x,mu,m,n);
	X_norm = getNormV(x,mu,sigma,m,n);
	return X_norm;
}
/*bool bNorm(Matrix x)
{
	Matrix mu =x.getMean();
	if(mu.getMax()-mu.getMin() >10)
	{
		return true;
	}
	return false;
}*/
/*Matrix featureScale(Matrix x,int degree)
{
	//Matrix re;
	if(degree<=1)
	{
		cout<<"degree is one";
		return x;
	}
	Matrix re(x.length(),degree);
	//Matrix tmp(x.length(),1);
	//if(dime
	for(int d=0;d<degree;d++)
	{
		for(int i=0;i<x.row_;i++)
		{
			*re(i,d)= pow(*x(i,0),d+1);
			
		}
	}
	return re;
}*/
Matrix contactColToVal(Matrix& x, int n)
{
	Matrix re = x;
	for(int k=0;k<n-1;k++)
	{
		if(x.col_ == n)
		{
			return re;
		}
		re = re.contactCol(x);
	}
	return re;
}
/****** 比如x1,x2变成6次， 每一列分别为原来的x1,x2: 1, x1, x2, x1x2, x1^2, x2^3, x1^3, x2^3, x1x1^2, x2^2X3....x2^6 总共28个。 
采用的策略为： 1 -- 0 度, no 1
              x1, x2 -- 1degree, no2
			  x1^2, x2^2, x1x2 -- 2 degree, no3
			  x1^3, x2^3, x1x2^2, x2x1^2 --3degree, no4
			  x1^4, x2^4, x1x 2^3, x1^2x2^2, x1^3x2, -- 4degree, no 5
			  total num = 1+2+3+4+5+6+7 = n(n+1)/2 = 28
			  
			  分配degree+1个matrix
			  d0 = 1 (列数)
			  d1 = d1+d0*x1+d0*x2 （x1) +x2
			  d2 = d2+d1*x1+d2*x2  (x1^2) + (x1 x2) * x2 = x1^2 x2x1 x2^2
			  dn = dn+dn-1*xi, i = 1..n 
			  *****/		

Matrix featureScale(Matrix x,int degree)
{
	Matrix re;
	if(degree <=1)
	{
		re = x;
		return re;
	}
	int m = x.length();
	
	int n = x.brandth();
	

	Matrix* d = new Matrix[degree+1];
	d[0]=Matrix(m,1,1); //第一度设置成m*1，值为1
	for(int i=1;i<degree+1;i++)
	{
		d[i]=Matrix(0,0);
	}

	for(int j=1;j<n;j++)//number of brandth
	{
		Matrix xCol = x.getCol(j,j);//取x1,....xn
		Matrix x_old = xCol;

		for(int i=1;i<degree+1;i++)
		{
			
			if(d[i-1].brandth() !=1)//不是第1度,eg i=2, x1 x1
			{
				xCol = contactColToVal(x_old,d[i-1].brandth()); //把x1或者x2有1列扩展成d[i-1].brandth()列
			}

			d[i]=d[i].contactCol(d[i-1]^xCol);	//d1 =d1+d0*x1, when j=2 d1 = d0*x2
			//d2 = d2+d1*x1, when j=2,d2=d2+d1*x2	
			
		}
	}
	//Matrix re;
	for(int i=0;i<degree+1;i++)
	{
		re = re.contactCol(d[i]);

	}
	delete[] d;
	return re;
}
#endif
