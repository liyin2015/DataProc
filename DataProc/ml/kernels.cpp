#include "kernels.h"

Matrix linearKernel(Matrix X)//m*m
{
	Matrix kernel = X*(X.trans());
	return kernel;
}
Matrix gassianKernel(Matrix x, double sigma)
{
	int m=x.length();
	Matrix kernel(x.length(),x.length());
	for(int i=0;i<m;i++)
	{
		for(int j=i;j<m;j++)
		{
			Matrix xi = x.getRow(i,i);
			Matrix xj=x.getRow(j,j);
			double tmp =((xi-xj).powm(2)).sumPerRow().matrix_[0]*(-1.0/(2*pow(sigma,2))); //if m row, then m row
			*kernel(i,j) = exp(tmp);
			*kernel(j,i)=exp(tmp);
		}
	}
	cout<<"kernel\n"<<kernel;
	return kernel;

}