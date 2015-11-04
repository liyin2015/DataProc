#include "matrix.hpp"
#include <algorithm>
#include <cmath>
#include <assert.h>
#include "io_manager.hpp"
using namespace std;
int Matrix::MatrixCount=0;
Matrix::Matrix(int row, int col):row_(row),col_(col)
{
	size_ = row*col;
	if(size_>0)
	{
	++MatrixCount;
	matrix_ = new double[size_];
	assert(matrix_!=0);
	for(int i=0;i<size_;i++)
		matrix_[i]=0;
	}
}
Matrix::Matrix(double** map, int row, int col)
{
	
	//print(map,row,col);
	row_=row;
	col_=col;
	size_=row*col;
	if(size_>0)
	{
		++MatrixCount;
	matrix_= new double[size_];
	assert(matrix_!=0);
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			matrix_[i*col_+j]=map[i][j];
	}
}
Matrix::Matrix(double* map, int row, int col)
{
	row_=row;
	col_=col;
	size_=row*col;
	if(size_>0)
	{
		++MatrixCount;
	matrix_= new double[size_];
	assert(matrix_!=0);
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			matrix_[i*col_+j]=map[i*col_+j];
	}
}
Matrix::Matrix(int row, int col,double value)
{
	row_=row;
	col_=col;
	size_=row*col;
	if(size_>0)
	{
		++MatrixCount;
	matrix_= new double[size_];
	assert(matrix_!=0);
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			matrix_[i*col_+j]=value;
	}
}
Matrix::Matrix(const Matrix& m)
{
	if(this == &m)
	{
		++MatrixCount;
	}
	else
	{
		/*if(matrix_!= NULL && size_!=0)
		{
			delete[] matrix_;
		}*/
	row_=m.row_;
	col_=m.col_;
	size_=row_*col_;
	if(size_>0)
	{
		++MatrixCount;

		matrix_= new double[size_];
		for(int i=0;i<row_;i++)
		{
			for(int j=0;j<col_;j++)
				matrix_[i*col_+j]=m.matrix_[i*m.col_+j];
		}
	}
	}
	
}

Matrix::Matrix()
{
	++MatrixCount;
	row_=0;
	col_=0;
	size_=0;
}
Matrix::~Matrix()
{
	--MatrixCount;
	if(MatrixCount>=0)
	{
		if(size_!=0 && matrix_!=NULL)
		{
			//cout<<"free"<<endl;
			delete[] matrix_;
			//cout<<"free1"<<endl;
			matrix_=NULL;
		}
	}
}
bool Matrix::isEmpty()
{
	return (size_==0)?true:false;
}
const Matrix& Matrix::operator=(const Matrix& m) 
{	
	if(&m!=this)
	{
		if(size_!=0&& matrix_!=NULL)
		{
			delete[] matrix_;//before delete there should judge the matrix pointer
			//matrix_ = NULL;

		}
		row_=m.row_;
		col_=m.col_;
		size_ = m.row_*m.col_;
		matrix_=new double[size_];;
		//assert(matrix_!=0);
		for(int r=0;r<m.row_;r++)
		{
			for(int c=0;c<m.col_;c++)
			{
				matrix_[r*col_+c] = m.matrix_[r*col_+c];
			}
		}
		return *this;
	}
	return m;
}
double* Matrix::operator()(int i,int j)
{
	//assert(0<=i-1&&i-1<Row);
	//assert(0<=j-1&&j-1<Col);
	return &matrix_[(i)*col_+j];
}
Matrix Matrix::operator()(int i,JUDGE_ROW row_or_col)
{
	Matrix h;
	if(row_or_col == IS_ROW)
	{
		if(i<0||i>=row_)
		{
			cout<<"wrong row"<<endl;
			return h;
		}
		return this->getRow(i,i);
	}
	else if(row_or_col == IS_COL)
	{
		if(i<0||i>=col_)
		{
			cout<<"wrong col"<<endl;
			return h;
		}
		return this->getCol(i,i);
	}
	else
	{
		cout<<"wrong para2"<<endl;

		return h;
	}
}
bool Matrix::operator==(const Matrix& m)
{
	if(row_!=m.row_||col_!=m.col_)
	{
		return false;
	}
	for(int i=0;i<row_;i++)
	{
		for(int j=0;j<col_;j++)
		{
			if(matrix_[i*col_+j]!=m.matrix_[i*col_+j])
			{
				return false;
			}
		}
	}
	return true;
}
bool Matrix::operator!=(const Matrix& m)
{
	if(*this==m)
	{
		return false;
	}
	return true;
}

ostream& operator<<(ostream& out,const Matrix& m)
{
	if(m.row_<=0 || m.col_ <=0)
	{
		out<<"print matrix is empty!"<<endl;
		return out;
	}
	out<<endl;
	//cout<<"row"<<m.row_<<"\tcol"<<m.col_<<endl;
	for(int i=0;i<m.row_;i++)
	{
		for(int j=0;j<m.col_;j++)
		{
			out<<m.matrix_[i*m.col_+j]<<"\t";
		}
		out<<endl;
	}
	return out;
}
Matrix Matrix::trans()
{
	//cout<<"col"<<col_<<"row"<<row_<<endl;
	Matrix re(col_,row_);
	for(int i=0;i<row_;i++)
		for(int j=0;j<col_;j++)
		{
			re.matrix_[j*row_+i]=matrix_[i*col_+j];
		}
	return re;
}
Matrix operator*(const Matrix& m1, const Matrix& m2) 
{
	Matrix re(m1.row_,m2.col_);
	if(m1.col_!=m2.row_)
	{
		cout<<"can not multiply,m1 = "<<m1.row_<<"X"<<m1.col_<<"while m2 ="<<m2.row_<<"X"<<m2.col_<<endl;
		return re;
	}

	for(int i= 0;i<m1.row_;i++)
	{
		for(int j=0;j<m2.col_;j++)
		{
			for(int k=0;k<m1.col_;k++)
			{
				re.matrix_[i*m2.col_+j]+=(m1.matrix_[i*m1.col_+k]*m2.matrix_[k*m2.col_+j]);
			}
		}
	}
	return re;
}
Matrix Matrix::operator^( const Matrix& m1) //point multiply [h1,h2,hn]T, per row * per row of X
{
	Matrix re(row_,col_);
	if(m1.row_!=row_ || col_!=m1.col_)
	{
		cout<<"row or col not equal, can not point multiply,m1 ="<<row_<<"X"<<col_<<"m2="<<m1.row_<<"X"<<m1.col_<<endl;
		return re;
	}

	for(int i= 0;i<row_;i++)
	{
		for(int j=0;j<col_;j++)
		{
			re.matrix_[i*col_+j] = matrix_[i*col_+j]*m1.matrix_[i*col_+j];
		}
	}
	return re;
}
Matrix Matrix::diag()
{
	Matrix re = *this;
	for(int i=0;i<row_;i++)
	{
		for(int j=0;j<col_;j++)
		{
			if(i==j)
			{
				re.matrix_[i*col_+j] =1;
			}
			else
			{
				re.matrix_[i*col_+j] =0;
			}
		}
	}
	return re;
}
/*Matrix Matrix::inverse()
{
 int N;
 //printf("输入矩阵的阶数N:\n");
 //scanf("%d",&N);
 //float a[10][10],b[10][20],c[10][10],t;
 //int i,j,m;
 //printf("请输入行列式不为0的矩阵A(%d阶):\n",N); //矩阵A的各元素存入二维数组a中。
 //for(i=0;i<N;i++)
 // for(j=0;j<N;j++)
  // scanf("%f",&a[i][j]);
 Matrix b =*this;
 Matrix diag = b.diag();
   //增广矩阵（A|E）存入二维数组b中
 //for(i=0;i<N;i++)
  //for(j=0;j<N;j++)
  // b[i][j]=a[i][j];

 for(i=0;i<N;i++)
  for(j=N;j<2*N;j++)
   b[i][j]=0;

 for(i=0;i<N;i++)
  b[i][N+i]=1;

  
 for(m=0;m<N;m++)          //对每行进行处理。
 { 
 t=b[m][m];                  //预存b[m][m]。
 i=m;                        
 while(b[m][m]==0)
 {    
  b[m][m]=b[i+1][m];
  i++;
 }

 if(i>m)
 {
 b[i][m]=t;                  //实现交换。
  
                     
 //交换其它各列相应位置的元素
 for(j=0;j<m;j++)                     
 {
  t=b[m][j];
  b[m][j]=b[i][j];
  b[i][j]=t;
 }
 for(j=m+1;j<2*N;j++)                     
 {
  t=b[m][j];
  b[m][j]=b[i][j];
  b[i][j]=t;
 }
 
 }


 for(i=m+1;i<N;i++)
     for(j=2*N-1;j>=m;j--)
   b[i][j]-=b[i][m]*b[m][j]/b[m][m]; //m=0时，将第一行的-b[i][0]/b[0][0]倍加到以下各行。这样以下每行第一个元素b[i][0]就为0。

    for(j=2*N-1;j>=m;j--)
     b[m][j]/=b[m][m];   //对第m行作行变换，同除以b[m][m]，使b[m][m]为1。

 }


 printf("第一步变换后得到的增广矩阵为：\n");
 for(i=0;i<N;i++)
 {
  for(j=0;j<2*N;j++)
   printf("%3.5f   ",b[i][j]);
  printf("\n");    //实现了：每个i对应一个换行。
 }
  


 m=N-1;
 while(m>0)
 {
  for(i=0;i<m;i++)
   for(j=2*N-1;j>=m;j--)           //千万注意，此处j必须递减，否则b[i][m]先变为0，后面的计算就无效！
    b[i][j]-=b[i][m]*b[m][j];
   m--;
 }

 printf("最后得到的增广矩阵为：\n");
 for(i=0;i<N;i++)
 {
  for(j=0;j<2*N;j++)
   printf("%3.5f  ",b[i][j]);
  printf("\n");    //实现了：每个i对应一个换行。
 }
    
 for(i=0;i<N;i++)                         //将逆矩阵存入二维数组c中。
  for(j=0;j<N;j++)
   c[i][j]=b[i][N+j];


 printf("故逆矩阵为：\n");

 for(i=0;i<N;i++)
 {
  for(j=0;j<N;j++)
   printf("%3.5f  ",c[i][j]);
  printf("\n");    //实现了：每个i对应一个换行。
 }
 
} */

/*Matrix operator/(const double a, const Matrix& m1) //real double multiply matrix
{
	Matrix re=m1;
	for(int i= 0;i<m1.row_;i++)
	{
		for(int j=0;j<m1.col_;j++)
		{
			re.matrix_[i][j] = m1.matrix_[i][j]*a;
		}
	}
	return re;
}*/
Matrix Matrix::multiplyReal(const double a)
{
	Matrix re=*this;
	for(int i= 0;i<row_;i++)
	{
		for(int j=0;j<col_;j++)
		{
			re.matrix_[i*col_+j] = matrix_[i*col_+j]*a;
		}
	}
	return re;
}

Matrix Matrix::operator+(const Matrix& m2) 
{
	Matrix re(row_,col_);
	if(col_!=m2.col_ || row_ != m2.row_)
	{
		cout<<"can not plus,m1 ="<<row_<<"X"<<col_<<"m2="<<m2.row_<<"X"<<m2.col_<<endl;
		return re;
	}
	
	for(int i= 0;i<row_;i++)
	{
		for(int j=0;j<m2.col_;j++)
		{
			re.matrix_[i*col_+j]= matrix_[i*col_+j]+m2.matrix_[i*col_+j];
		}
	}
	return re;
}
Matrix Matrix::operator-(const Matrix& m2) 
{
	Matrix re(row_,col_);
	if(col_!=m2.col_ || row_ != m2.row_)
	{
		cout<<"can not miuns,m1 ="<<row_<<"X"<<col_<<"m2="<<m2.row_<<"X"<<m2.col_<<endl;
		return re;
	}

	for(int i= 0;i<row_;i++)
	{
		for(int j=0;j<col_;j++)
		{
			re.matrix_[i*col_+j]= matrix_[i*col_+j]-m2.matrix_[i*col_+j];
		}
	}
	return re;
}
Matrix Matrix::operator/(const Matrix& m)//m1/m2
{
	if(row_!=m.row_ || col_!=m.col_)
	{
		Matrix re;
		return re;
	}
	Matrix re = m;
	for(int i=0;i<row_;i++)
	{
		for(int j=0;j<col_;j++)
		{
			re.matrix_[i*col_+j] = matrix_[i*col_+j]/m.matrix_[i*col_+j];
		}
	}
	return re;

}
Matrix Matrix::operator/(double& d)
{
	Matrix re = *this;
	for(int i=0;i<row_;i++)
	{
		for(int j=0;j<col_;j++)
		{
			re.matrix_[i*col_+j] = matrix_[i*col_+j]/d;
		}
	}
	return re;
}

void Matrix::setMatrix(double** map,int row, int col)
{
		Matrix re(map,row,col);
		*this =re;
	return;
}
Matrix Matrix::sumPerRow()
{
	Matrix re(row_,1);
	for(int r=0;r<row_;r++)	
	{
		for(int c=0;c<col_;c++)
			re.matrix_[r]+= matrix_[r*col_+c];
	}
	return re;
}
Matrix Matrix::pointPow(const int n)
{
	Matrix re=*this;
	for(int r=0;r<row_;r++)	
	{
		for(int c=0;c<col_;c++)
			re.matrix_[r*col_+c]= pow(matrix_[r*col_+c],n);
	}
	return re;
}
Matrix Matrix::sumPerCol()
{
	Matrix re(1,col_);
	for(int c=0;c<col_;c++)	
	{
		for(int r=0;r<row_;r++)
			re.matrix_[c]+= matrix_[r*col_+c];
	}
	return re;
}
int Matrix::length()
{
	return row_;
}
int Matrix::brandth()
{
	return col_;
}
/*void Matrix::printMatrix()
{
		for(int r=0;r<row_;r++)	
	{
		for(int c=0;c<col_;c++)
		{
			cout<<matrix_[r*col_+c]<<"\t";
		}
		cout<<endl;
	}
		cout<<"................................"<<endl;
}*/
Matrix Matrix::getLog(const double n)
{
	Matrix re(row_,col_);
	for(int r=0;r<row_;r++)	
	{
		for(int c=0;c<col_;c++)
		{
			re.matrix_[r*col_+c]= log(matrix_[r*col_+c])/log(n);
		}
	}
	return re;

}
void Matrix::subsituteByIndex(const Matrix &m, int m_startcol, int m_endcol, int start_col, int end_col)//start from zero
{
	//Matrix re = *this;
	if(row_!=m.row_ || ((end_col-start_col) !=( m_endcol-m_startcol)))
	{
		cout<<"row not equal, can not subsitute"<<endl;
	}
	for(int i=0;i<=end_col-start_col;i++)
	{
		for(int j=0;j<row_;j++)
		{
			matrix_[j*col_+i+start_col] =m.matrix_[j*m.col_+i+m_startcol];
		}
	}

}
void Matrix::subsituteRow(const Matrix &m, int m_startrow, int m_endrow, int start_row, int end_row)
{
	if(col_!=m.col_ || ((end_row-start_row) !=( m_endrow-m_startrow)))
	{
		cout<<"col not equal, can not subsitute"<<endl;
	}
	for(int i=0;i<=end_row-start_row;i++) //row
	{
		for(int j=0;j<col_;j++)//col
		{
			matrix_[(i+start_row)*col_+j] =m.matrix_[(i+m_startrow)*m.col_+j];
		}
	}
}
Matrix Matrix::getCol(int start_col, int end_col)
{
	Matrix re(row_,end_col-start_col+1);
	if(start_col<0 || end_col>=col_)
	{
		cout<<"getCol, input para is illegal!"<<endl;
		return re;
	}
	//int old_col=col_;

	for(int i=0;i<=end_col-start_col;i++)
	{
		for(int j=0;j<row_;j++)
		{
			re.matrix_[j*re.col_+i] =matrix_[j*col_+i+start_col];
		}
	}
	return re;


}
Matrix Matrix::getRow(int start_row, int end_row)
{
	Matrix re(end_row-start_row+1,col_);
	if(start_row<0 || end_row>=row_)
	{
		cout<<"getRow, input para is illegal!"<<endl;
		return re;
	}
	//int old_col=col_;

	for(int i=0;i<=end_row-start_row;i++)
	{
		for(int j=0;j<col_;j++)
		{
			re.matrix_[i*col_+j] =matrix_[(i+start_row)*col_+j];
		}
	}
	return re;
}
void Matrix::addOneCol(int pos, int value)
{
	matrix_ = add_col(matrix_,value,row_,col_);
	size_ = row_*col_;
}
void Matrix::randomize(double scale, double max)
{
	for(int i=0;i<row_;i++)
		for(int j=0;j<col_;j++)
		{
			double drand=rand();    
			double a=max*(1.0-2*drand/scale);
			matrix_[i*col_+j] = a;
		}
                   

}
Matrix Matrix::getMean()
{
	Matrix mu(1,col_);
	for(int col=0;col<col_;col++)
	{
		//mu[col]=0;
		for(int row=0;row<row_;row++)
		{
			mu.matrix_[col]+=matrix_[row*col_+col];
		}
		mu.matrix_[col]/=row_;
	}
	return mu;
}
Matrix Matrix::getStd(Matrix* mu)
{
	Matrix mu_;
	if(mu==NULL)
	{
		mu_=this->getMean();
	}
	else
	{
		mu_=*mu;
	}
	Matrix sigma(1,col_);
	for(int c=0;c<col_;c++)
	{
		for(int r=0;r<row_;r++)
		{
			sigma.matrix_[c]+=pow(matrix_[r*col_+c]-mu_.matrix_[c],2);
		}
		sigma.matrix_[c]=sqrt(sigma.matrix_[c]/row_);
	}
	return sigma;
}
Matrix Matrix::getNorm(Matrix* m,Matrix* sig)
{
	Matrix mu=this->getMean();
	Matrix sigma = this->getStd(&mu);
	Matrix norm(row_,col_);
	for(int c=0;c<col_;c++)
	{
		for(int r=0;r<row_;r++)
		{
			norm.matrix_[r*col_+c]= (matrix_[r*col_+c]-*mu(0,c))/(*sigma(0,c));
		}
	}
	if(m!= NULL)
	{
		*m=mu;
	}
	if(sig!=NULL)
	{
		*sig = sigma;
	}
	return norm;
}
Matrix Matrix::recoverFmNorm(Matrix& mu, Matrix& sigma)
{
	Matrix re;
	if(mu.col_!=sigma.col_)
	{
		return re;
	}
	re=*this;
	for(int c=0;c<col_;c++)
	{
		for(int r=0;r<row_;r++)
		{
			re.matrix_[r*col_+c]= matrix_[r*col_+c]*sigma.matrix_[c]+mu.matrix_[c];
		}
	}
	return re;

}
Matrix Matrix::getNormWithMuSigma(Matrix& mu, Matrix& sigma)
{
	Matrix norm;
	if(mu.col_!=sigma.col_)
	{
		return norm;
	}
	norm=Matrix(row_,col_);
	for(int c=0;c<col_;c++)
	{
		for(int r=0;r<row_;r++)
		{
			norm.matrix_[r*col_+c]= (matrix_[r*col_+c]-*mu(0,c))/(*sigma(0,c));
		}
	}
	return norm;
}
double Matrix::getMin(INDEX* i)
{
	double min = (double)INT_MAX;
	for(int c=0;c<col_;c++)
	{
		for(int r=0;r<row_;r++)
		{
			if(matrix_[r*col_+c]<min)
			{
				min=matrix_[r*col_+c];
				if(i!= NULL)
				{
					i->row = r;
					i->col = c;
				}
			}
		}
	}
	return min;
}
double Matrix::getMax(INDEX* i)//per row
{
	double max = -(double)INT_MAX;
	for(int c=0;c<col_;c++)
	{
		for(int r=0;r<row_;r++)
		{
			if(matrix_[r*col_+c]>max)
			{
				max=matrix_[r*col_+c];
				if(i!= NULL)
				{
					i->row = r;
					i->col = c;
				}
			}
		}
	}
	return max;

}
Matrix Matrix::getMaxIndexPerCol()
{
	

	Matrix re(col_,1,-1);
	
	for(int c=0;c<col_;c++)
	{
		double max = -(double)INT_MAX;
		double index = -1;//return max row
		for(int r=0;r<row_;r++)
		{
			if(matrix_[r*col_+c]>max)
			{
				max=matrix_[r*col_+c];
				index = r;
			}
		}
		*re(c,0) = index;
	}
	return re;
}
Matrix Matrix::powm(double n)
{
	Matrix re=*this;
	for(int i=0;i<this->row_;i++)
	{
		for(int j=0;j<this->col_;j++)
		{
			re.matrix_[i*this->col_+j]=pow(this->matrix_[i*this->col_+j],n);
		}
	}
	return re;
}
Matrix Matrix::expm()
{
	Matrix re=*this;
	for(int i=0;i<this->row_;i++)
	{
		for(int j=0;j<this->col_;j++)
		{
			re.matrix_[i*this->col_+j]=exp(this->matrix_[i*this->col_+j]);
		}
	}
	return re;
}
Matrix Matrix::contactCol(const Matrix& m)
{
	if(m.row_ !=this->row_)
	{
		//ML_LIR_ERROR("contactCol, m1 row:"<<row_<<"!=m2 row:"<<m.row_);
		if(size_!=0)
		{
			cout<<"return"<<endl;
			return *this;
		}
		cout<<"continue"<<endl;
	}
	Matrix re(m.row_,col_+m.col_);
	for(int i=0;i<m.row_;i++)
	{
		for(int j=0;j<re.col_;j++)
		{
			if(j<col_)
			{
				re.matrix_[i*re.col_+j]=this->matrix_[i*col_+j];
			}
			else
			{
				re.matrix_[i*re.col_+j]=m.matrix_[i*m.col_+j-col_];
			}
		}
	}
	return re;

}
Matrix Matrix::contactRow(const Matrix& m)
{
	if(m.col_ !=this->col_)
	{
		//ML_LIR_ERROR("contactCol, m1 row:"<<row_<<"!=m2 row:"<<m.row_);
		if(size_!=0)
		{
			return *this;
		}
	}
	Matrix re(row_+m.row_,m.col_);
	for(int i=0;i<m.col_;i++)
	{
		for(int j=0;j<re.row_;j++)
		{
			if(j<row_)//just copy
			{
				re.matrix_[j*re.col_+i]=this->matrix_[j*col_+i];
			}
			else//append
			{
				re.matrix_[j*re.col_+i]=m.matrix_[(j-row_)*m.col_+i];
			}
		}
	}
	return re;
}

void Matrix::computeDistance(double* distance)//one point with each other in matrix x
{
	//Matrix re(row_,row_,(double)INT_MAX);
	int size = row_*(row_+1)/2;
	//double* re = new double[size]; //this way can save half space
	for(int i=0;i<size;i++)
	{
		distance[i] = (double)INT_MAX;
	}
	/*int start_pos = 0;
	int col = row_;
	for(int i=0;i<row_;i++) //dont compute distance 0
	{
		int k =1;
		for(int j = i+1;j<row_;j++) //row vector, this is for the original maxtirx x
		{
			int indexj = row_;
			
			Matrix  vec1 = this->getRow(i,i);
			Matrix  vec2 = this->getRow(j,j);

			distance[start_pos+k] =  sqrt(((vec1-vec2).powm(2)).sumPerRow().matrix_[0]);

			k++;
		}
		start_pos = start_pos +k;
	}

		int start_pos = 0;
	int col = row_;*/
	for(int i=0;i<row_;i++) //dont compute distance 0
	{
		int index_x = i*(row_+row_+1-i)/2;
		//int k =1;
		for(int j = i+1;j<row_;j++) //row vector, this is for the original maxtirx x
		{
			int indexj = j-i;
			
			Matrix  vec1 = this->getRow(i,i);
			Matrix  vec2 = this->getRow(j,j);

			distance[index_x+indexj] =  sqrt(((vec1-vec2).powm(2)).sumPerRow().matrix_[0]);
			int dis = (int)distance;
			if(dis ==13)
			{
				cout<<vec1;
				cout<<vec2;
			}
		}
	}
}
bool Matrix::MFindByValue(double val)
{
	for(int i=0;i<size_;i++)
	{
		if(matrix_[i] == val)
		{
			return true;
		}
	}
	return false;
}

//other matrix related funcion
double computeRowVectorDistance(Matrix vec1, Matrix vec2)
{
	//double rslt = (double)INT_MAX;
	return sqrt(((vec1-vec2).powm(2)).sumPerRow().matrix_[0]);
}

Matrix generateByStep(Matrix& m, int total_step)
{
	double min = (3.0/4)* m.getMin();
	double max = (4.0/3)*m.getMax();
	double space = max-min;
	double* x = new double[total_step];
	for(int i=0;i<total_step;i++)
	{
		x[i]=min+((double)i/total_step)*space;
	}
	Matrix xm(x,total_step,1);
	delete[] x;
	return xm;
}
