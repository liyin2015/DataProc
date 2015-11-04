#ifndef MATH_FUNCTION_HPP_
#define MATH_FUNCTION_HPP_
#include "comm.hpp"
#include "matrix.hpp"


double sigmoid(double x);
Matrix sigmoid(Matrix map);

void sortMin(double a[], int n);
bool findByValue(double a[],double value,int size);
double randf();
int randint(int rand);
//高斯随机数产生函数
double gaussrand();

//used to
Matrix trans_y_for_multilabels(Matrix& Y_train, Matrix& labels);


#endif