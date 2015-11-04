#include "math_function.hpp"
#include <iostream>
#include "matrix.hpp"
;using namespace std;
double sigmoid(double x)
{
	double y=0.0;
	y=1.0/(1+pow(e,-x));
	return y;
}
Matrix sigmoid(Matrix map)
{
	Matrix re(map.row_,map.col_);
	for(int r=0;r<map.row_;r++)
	{
		for(int c=0;c<map.col_;c++)
			re.matrix_[r*map.col_+c]=sigmoid(map.matrix_[r*map.col_+c]);
	}
	return re;
}
void sortMin(double a[], int n)
{
	for(int i=0;i<n;i++)
	{
		for(int j=i+1;j<n;j++)
		{
			if(a[i]>a[j])
			{
				double tmp=a[i];
				a[i]=a[j];
				a[j]=tmp;
			}
		}
	}
}
bool findByValue(double a[],double value,int size)
{
	for(int i=0;i<size;i++)
	{
		if(a[i]==value)
		{
			return true;
		}
	}
	return false;
}
double randf()
{
	return (double)((rand() % 1001) * 0.001f-0.5);
}
//高斯随机数产生函数
double gaussrand()
{   static double V1, V2, S;
     static int phase = 0;
     double X;

     if(phase == 0) {
   do {
      double U1 = (double)rand() / RAND_MAX;
      double U2 = (double)rand() / RAND_MAX;

      V1 = 2 * U1 - 1;
      V2 = 2 * U2 - 1;
      S = V1 * V1 + V2 * V2;
   } while(S >= 1 || S == 0);

   X = V1 * sqrt(-2 * log(S) / S);
     } else
   X = V2 * sqrt(-2 * log(S) / S);

     phase = 1 - phase;

     return X;
}
const double MinProb=1.0/(RAND_MAX+1);
bool happened(double probability)//probability 0~1
{
    if(probability<=0)
    {
return false;
    }
    if(probability<MinProb)
    {
        return rand()==0&&happened(probability*(RAND_MAX+1));
    }
    if(rand()<=probability*(RAND_MAX+1))
    {
        return true;
    }
    return false;
}


int randint(int n)//产生0~n-1之间的等概率随机数
{
    int t=0;
	//int r=0;
    if(n<=RAND_MAX)
    {
        long r=RAND_MAX-(RAND_MAX+1)%n;//尾数
        t = rand();
        while ( t > r )
       {
            t = rand();
       }
        return t % n;
    }
    else
    {
        int r = n%(RAND_MAX+1);//余数
        if( happened( (double)r/n ) )//取到余数的概率
       {
            return n-r+randint(r);
       }
        else
       {
            return rand()+randint(n/(RAND_MAX+1))*(RAND_MAX+1);
       }
    }
}

Matrix trans_y_for_multilabels(Matrix& Y_train, Matrix& labels) //change Y to m*num_labels
{
	int num_labels = labels.length();
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




