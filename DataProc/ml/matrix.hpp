#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <iostream>
#include <stdio.h>
#include "comm.hpp"

typedef enum
{
	IS_ROW =0,
	IS_COL =1,
}JUDGE_ROW;
typedef struct
{
	int row;
	int col;
}INDEX;

using namespace std;
class Matrix
{
public:
	Matrix(int row, int col);
	Matrix(double** map, int row, int col);
	Matrix(double* map, int row, int col);
	Matrix(int row, int col,double value);
	Matrix();
	~Matrix();
	const Matrix& operator=(const Matrix& m);
	Matrix(const Matrix& m);
	Matrix trans();//transfer
	Matrix inverse();
	Matrix diag();
	double* operator()(int i,int j);
	Matrix operator()(int i,JUDGE_ROW row_or_col);
	bool operator==(const Matrix& m);
	bool operator!=(const Matrix& m);
	//double& operator[](int start_row,int end_row, int start_col, int end_col);
	friend ostream& operator<<(ostream& out,const Matrix& m);
	friend Matrix operator*( const Matrix& m1,const Matrix& m2);
	Matrix operator^( const Matrix& m1); //point multiply
	Matrix operator/(const Matrix& m2);//m1/m2
	Matrix operator/(double& d);//m/d
	//friend Matrix operator/(const double a, const Matrix& m1);//real multiplty
	
	Matrix operator+(const Matrix& m2);
	Matrix operator-(const Matrix& m2);
	void setMatrix(double** map,int row, int col);
	Matrix sumPerRow();
	Matrix sumPerCol();
	Matrix pointPow(const int n);
	Matrix multiplyReal(const double a);
	void addOneCol(int pos, int value);
	void randomize(double scale,double max);

	Matrix getCol(int start_col, int end_col);
	Matrix getRow(int start_row, int end_row);
	//a.subsitte byindex(a,b,c,d); return a with a b c d be subsituted
	void subsituteByIndex(const Matrix &m, int m_startcol, int m_endcol, int start_col, int end_col);
	void subsituteRow(const Matrix &m, int m_startrow, int m_endrow, int start_row, int end_row);
	//used for logistic regression
	Matrix getLog(const double n);
	int length();
	int brandth();
	bool isEmpty();

	Matrix getMean();//1*n
	Matrix getStd(Matrix* mu=NULL);
	Matrix getNorm(Matrix* m,Matrix* sig);
	Matrix recoverFmNorm(Matrix& mu, Matrix& sigma);
	Matrix getNormWithMuSigma(Matrix& mu, Matrix& sigma);
	double getMin(INDEX* i = NULL);
	double getMax(INDEX* i = NULL);
	Matrix getMaxIndexPerCol();
	Matrix powm(double);
	Matrix expm();
	Matrix contactCol(const Matrix& m);
	Matrix contactRow(const Matrix& m);

	void computeDistance(double* distance);
    bool MFindByValue(double val);

	public:

	double* matrix_;
	int row_;
	int col_;
	int size_;
	static int MatrixCount;	
};

double computeRowVectorDistance(Matrix vec1, Matrix vec2);
Matrix generateByStep(Matrix& m, int total_step);

#endif






