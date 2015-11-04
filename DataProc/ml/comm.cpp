#include <iostream>
#include <algorithm>
#include <vector>

#include "comm.hpp"
using namespace std;


void free_2(double** matrix,const int row, const int col)
{
	if(row!=0 || col!=0)
	{
		for(int r=0;r<row;r++)
		{
			delete[] matrix[r];
		}
		delete[] matrix;
	}
	//for(int i=0;i<4;i++)     delete  []  p[i];delete [] p;
}
double** malloc_2(double value, int row,int col)
{
	double** X_norm; 
	/*X_norm = (double**)malloc(row*sizeof(double*));
	for(int i=0;i<row;i++)
	{
		X_norm[i]=(double*)malloc(col*sizeof(double));
		for(int j=0;j<col;j++)
		{
			X_norm[i][j]=0;
		}
	}
	return X_norm;*/
	X_norm = new double*[row];
	for(int i=0;i<row;i++)
	{
		X_norm[i] = new double[col];
		for(int j=0;j<col;j++)
		{
			X_norm[i][j]=value;
		}
	}
	return X_norm;
}

double** add_col(double** map, int value, int row, int& col)
{
	//cout<<"add1"<<endl;
	col=col+1;
	double** re=malloc_2(value,row,col);
	for(int c=1;c<col;c++)
		for(int r=0;r<row;r++)
		{
			re[r][c]=map[r][c-1];
		}
		//cout<<"add2"<<endl;
	return re;
}
double* add_col(double* map, int value, int row,int& col)
{
		col=col+1;
	double* re= new double[row*col];
	for(int i=0;i<row;i++)
		re[i*col]=value;
	//malloc_2(value,row,col);
	for(int c=1;c<col;c++)
		for(int r=0;r<row;r++)
		{
			re[r*col+c]=map[r*(col-1)+c-1];
		}
		//cout<<"add2"<<endl;
	return re;
}
void print(double** map, int row, int col)
{
	if(row<=0 || col <=0)
	{
		cout<<"print matrix is empty!"<<endl;
		return;
	}
	for(int i=0;i<row;i++)
	{
		for(int j=0;j<col;j++)
		{
			cout<<map[i][j]<<"\t";
		}
		cout<<endl;
	}
	cout<<endl;
}
double str2double(char const *  str)
{	
	int x=0;	
	double y=0;
	int j;	
	int decimalnum=1;	
	bool pointExist=false;
	bool bFirst = true;
	bool bNegative = false;
	while(*str)	
	{		
		if (*str>='0' && *str<='9')		
		{			
			if(pointExist)			
			{				
				j=(int)(*(str))-48;				
				y=y+j*pow(0.1,decimalnum);				
				decimalnum++;			
			}			
			else			
			{				
				x=x*10+*str-48;			
			}		
		}		
		else if(*str=='.')		
		{			
			pointExist=true;
		}
		else if(*str=='-'&&bFirst)
		{
			bNegative = true;
			bFirst = false;
		}
		else
		{
			return -1;//unrecognize str
		}
		str++;	
	}
	if(bNegative)
	{
		return -(x+y);
	}
	return x+y;
}
 

double* getMeanV(double** x, int m, int n)
{
	double* mu=malloc_1<double>(n);
	for(int col=0;col<n;col++)
	{
		mu[col]=0;
		for(int row=0;row<m;row++)
		{
			mu[col]+=x[row][col];
		}
		mu[col]/=m;
	}
	return mu;
}
double* getStdV(double** x, double* mu, int m, int n)
{

	double* sigma=malloc_1<double>(n);
	for(int col=0;col<n;col++)
	{
		sigma[col]=0;
		for(int row=0;row<m;row++)
		{
			sigma[col]+=pow(x[row][col]-mu[col],2);
		}
		sigma[col]=sqrt(sigma[col]/m);
	}
	return sigma;
}
double** getNormV(double** x, double* mu, double* sigma, int m, int n)
{

	double** X_norm=malloc_2(0,m,n);
	for(int c=0;c<n;c++)
	{
		for(int r=0;r<m;r++)
		{
			X_norm[r][c]= (x[r][c]-mu[c])/sigma[c];
		}
	}
	return X_norm;
}
bool FindInVector(vector<double> v, double value)
{
	vector<double>::iterator ite = v.begin();
	for(;ite!=v.end();ite++)
	{
		if(*ite==value)
		{
			return true;
		}
	}
	return false;
}
