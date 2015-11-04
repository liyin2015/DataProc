#include "evaluate_hypo.hpp"
#include "matrix.hpp"
#include "math_function.hpp"
using namespace std;
RSLT_EVAL::RSLT_EVAL()
{
}
RSLT_EVAL::~RSLT_EVAL()
{
}
ostream& operator<<(ostream& out, const RSLT_EVAL& rslt)
{
	out<<"train accuracy:"<<rslt.train_accuracy<<endl;
	out<<"test accuracy:"<<rslt.test_accuracy<<endl;
	out<<"precision:"<<rslt.precision<<endl;
	out<<"recall:"<<rslt.recall<<endl;
	out<<"PR:"<<rslt.PR<<endl;
	out<<"Jtrain:"<<rslt.Jtrain<<endl;
	out<<"Jtest:"<<rslt.Jtest<<endl;
	out<<"Jcv:"<<rslt.Jcv<<endl;
	out<<"ac1_pre1:"<<rslt.ac1_pre1<<endl;
	out<<"ac1_pre0:"<<rslt.ac1_pre0<<endl;
	out<<"ac0_pre1:"<<rslt.ac0_pre1<<endl;
	out<<"ac0_pre0:"<<rslt.ac0_pre0<<endl;
	return out;
}
double RSLT_EVAL::getAccuracy(Matrix hypo, Matrix y)
{
	int m=y.length();
	int num_right =0;
	for(int i=0;i<m;i++)
	{
		if(*y(i,0) == *hypo(i,0))
		{
			num_right++;
		}
	}
	return (double)num_right/m;
}
void numeric(Matrix& y, Matrix& hypo, int& truePos, int& falseNeg, int& falsePos, int& trueNeg)
{
	int m = y.length();
		for(int i=0;i<m;i++)
		{
			if(*y(i,0) == 1&& (*hypo(i,0)==1))
			{
				truePos++;
			}
			else if(*y(i,0) == 1&& (*hypo(i,0)==0))
			{
				falseNeg++;
			}
			else if(*y(i,0) == 0&& (*hypo(i,0)==1))
			{
				falsePos++;
			}
			else
			{
				trueNeg++;
			}
		}
		return;
}
void RSLT_EVAL::getPrecision(Matrix hypo, Matrix y, Matrix label)//single label and used for test
{

	int m=y.length();
	int num_label = label.length();
	if(num_label==2)
	{
		int truePos = 0;
		int falseNeg=0;
		int falsePos = 0;
		int trueNeg = 0;
		numeric(y,hypo,truePos,falseNeg,falseNeg, trueNeg);
		ac1_pre1 = truePos;
		ac1_pre0=falseNeg;
		ac0_pre1 = falsePos;
		ac0_pre0=trueNeg;

		precision =(double)(truePos)/(truePos+falseNeg);
		recall = (double)(truePos)/(truePos+falsePos);
		PR = (precision*recall)/(precision+recall);
	}
	else if(num_label>2)
	{
		Matrix y_multi = trans_y_for_multilabels(y,label);
		Matrix hypo_multi = trans_y_for_multilabels(hypo,label);
		int* truePos = new int[num_label];
		int* falseNeg=new int[num_label];
		int* falsePos =new int[num_label];
		int* trueNeg = new int[num_label];

		for(int i =0;i<num_label;i++)
		{
			truePos[i] = 0;
			falseNeg[i]=0;
			falsePos[i] = 0;
			trueNeg[i] = 0;
			numeric(y_multi.getCol(i,i),hypo_multi.getCol(i,i),truePos[i],falseNeg[i],falseNeg[i], trueNeg[i]);
		}

		int truePos_avg = 0;
		int falseNeg_avg=0;
		int falsePos_avg = 0;
		int trueNeg_avg = 0;
		for(int i =0;i<num_label;i++)
		{
			truePos_avg+=truePos[i];
			falseNeg_avg+=falseNeg[i];
			falsePos_avg += falsePos[i];
			trueNeg_avg += trueNeg[i];
		}
		truePos_avg/=num_label;
		falseNeg_avg/=num_label;
		falsePos_avg /= num_label;
		trueNeg_avg /= num_label;

		ac1_pre1 = truePos_avg;
		ac1_pre0=falseNeg_avg;
		ac0_pre1 = falsePos_avg;
		ac0_pre0=trueNeg_avg;

		precision =(double)(truePos_avg)/(truePos_avg+falseNeg_avg);
		recall = (double)(truePos_avg)/(truePos_avg+falsePos_avg);
		PR = (precision*recall)/(precision+recall);
	}
	return;


}


/*void Jthree(pFunc f, Matrix& hypo, Matrix y, RSLT_EVAL& rslt, double train_rate, double cv_rate, double test_rate)
{
	int m = y.length();
	int train_n= (int)(train_rate*m);
	Matrix y_train = y.getRow(0,train_n-1);
	Matrix hypo_train = hypo.getRow(0,train_n-1);

	int cv_n = (int)((train_rate+cv_rate)*m);
	int cv_m = cv_n-train_n;
	Matrix y_cv =y.getRow(train_n,cv_n-1);
	Matrix hypo_cv =hypo.getRow(train_n,cv_n-1);

	int test_m = m-cv_n;
	Matrix y_test =y.getRow(cv_n,m-1);
	Matrix hypo_test =hypo.getRow(cv_n,m-1);

	//Matrix tmp1 = (y_train-hypo_train).powm(2);
	//Matrix tmp2 = (y_cv-hypo_cv).powm(2);
	//Matrix tmp3 = (y_test-hypo_test).powm(2);

	double Jtrain = f(hypo_train,y_train);
	double Jcv = f(hypo_cv,y_cv);
	double Jtest = f(hypo_test,y_test);

	//double Jtrain = (tmp1.sumPerCol()).multiplyReal(1.0/train_n).matrix_[0];
	//double Jcv = (tmp2.sumPerCol()).multiplyReal(1.0/cv_m).matrix_[0];
	//double Jtest = (tmp3.sumPerCol()).multiplyReal(1.0/test_m).matrix_[0];

	rslt.Jtrain=Jtrain;
	rslt.Jcv=Jcv;
	rslt.Jtest =Jtest;
	return;
}*/
bool isSkew(Matrix y)
{
	int m = y.length();
	int pos =0;
	for(int i=0;i<m;i++)
	{
		if(y.matrix_[i]==1)
		{
			pos++;
		}

	}
	return (double)pos/m < 0.1?true:false;	
}

/*void getAccuracy(Matrix hypo, Matrix y, RSLT_EVAL& rslt)
{
	int ac1_pre1 = 0;
	int ac1_pre0=0;
	int ac0_pre1 = 0;
	int ac0_pre0 = 0;
	int m=y.length();
	for(int i=0;i<m;i++)
	{
		if(*y(i,0) == 1&& (*hypo(i,0)==1))
		{
			ac1_pre1++;
		}
		else if(*y(i,0) == 1&& (*hypo(i,0)==0))
		{
			ac1_pre0++;
		}
		else if(*y(i,0) == 0&& (*hypo(i,0)==1))
		{
			ac0_pre1++;
		}
		else
		{
			ac0_pre0++;
		}
	}
	rslt.ac1_pre1 = ac1_pre1;
	rslt.ac1_pre0=ac1_pre0;
	rslt.ac0_pre1 = ac0_pre1;
	rslt.ac0_pre0=ac0_pre0;
	double accuracy = (double)(ac1_pre1+ac0_pre0)/double(m);
	double precision =(double)(ac1_pre1)/(ac1_pre1+ac1_pre0);
	double recall = (double)(ac1_pre1)/(ac1_pre1+ac0_pre1);
	double PR = (precision*recall)/(precision+recall);
	rslt.accuracy=accuracy;
	rslt.precision=precision;
	rslt.recall=recall;
	rslt.PR=PR;
	return;
}*/
/*RSLT_EVAL getRsltStruct(Matrix hypo, Matrix y, double train_rate, double cv_rate, double test_rate)
{
	RSLT_EVAL rslt;
	getAccuracy(hypo,y,rslt);
	
	//Jthree(f,hypo,y,rslt,train_rate,cv_rate, test_rate);
	return rslt;
}*/