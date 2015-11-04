#ifndef COMM_HPP_
#define COMM_HPP_
#include <iostream>
#include <vector>
#include "matrix.hpp"
using namespace std;
const double e = 2.718281828;

typedef enum
{
    TYPE_LOGISTIC = 0,
    TYPE_REGRSSSION = 1,    
	NEURAL_NETWORK =2,
}OUTPUT_TYPE;

//You need to set these values correctly 
template<typename T>
T* malloc_1(int col)
{
	T* r = new T[col];
	for(int i=0;i<col;i++)
	{
		r[i]=0;
	}
	return r;
}
template<typename T>
void free_1(T* matrix, int& col)
{
		if(col!=0)
		{
			free(matrix);
			col=0;
		}
}
double** malloc_2(double value,int row,int col);
void free_2(double** matrix,const int row, const int col);


/* change 2 dimenstional matrix to double ***/
template <int C>
double** chg_v_to_matrix(double (*map)[C],int row,int col)
{
	
	double** re = malloc_2(0,row,col);

			for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			re[i][j]=map[i][j];		

	return re;
}

double** add_col(double** map, int value, int row, int& col);
double* add_col(double* map, int value, int row,int& col);

void print(double** map, int row, int col);
double str2double(char const *  str);
//deal with matrix
double* getMeanV(double** x, int m, int n);

double* getStdV(double** x, double* mu, int m, int n);

double** getNormV(double** x, double* mu, double* sigma, int m, int n);

bool FindInVector(vector<double> v, double value);

template<typename T>
T getMin(T& a, T& b)
{
	return (a<b)? a:b;
}



#endif