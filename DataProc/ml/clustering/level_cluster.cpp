#include "level_cluster.hpp"
//in this algo, every point in matrix x is a clusteriod, the algo will converge when there has k centeroid left


typedef struct
{
	int cluster_id;
	bool bClose;
}Direct;

double getMin(INDEX& min, double* sigmap, int row, Direct* x_assign)
{
	int col = row;
	double minv = (double)INT_MAX;
	for(int i=0;i<row;i++) //dont compute distance 0
	{
		int index_x = i*(row+row+1-i)/2;
		if(x_assign[i].cluster_id!=-1 && x_assign[i].bClose)//row index_x has been closed
		{
			continue;
		}
		//int k =1;
		for(int j = i+1;j<row;j++) //row vector, this is for the original maxtirx x
		{
			int indexj = j-i;
			if(x_assign[j].cluster_id!=-1 && x_assign[j].bClose) //row index_x has been closed
			{
				continue;
			}
			if(sigmap[index_x+indexj] <minv)
			{
				minv = sigmap[index_x+indexj];
				min.row = i;
				min.col =j;
			}
		}
	}
	return minv;
}
void subsitueDistance(int p, double*sigmap, int row, Matrix x, Matrix center)//center subsitute x
{
	//heri
	int start_pos = p*(row+row+1-p)/2;
	for(int c = p+1;c<row;c++) //col change
	{
		Matrix  vec2 = x.getRow(c,c);
		int index_j = c-p;
		sigmap[start_pos+index_j] = computeRowVectorDistance(center,vec2);
	}
	//verical
	for(int r=0;r<p;r++) //row change
	{
		start_pos = r*(row+row+1-r)/2;
		Matrix  vec2 = x.getRow(r,r);
		//int index_j = c-p;
		int index_j = p-r;
		sigmap[start_pos+index_j] = computeRowVectorDistance(center,vec2);
	}
}

void initDirectMap(Direct* x_assign, int n)
{
	for(int i=0;i<n;i++)
	{
		x_assign[i].cluster_id = -1;
		x_assign[i].bClose = false;
	}
}
LevelCluster::LevelCluster()
{
	cluster_rslt = NULL;
}
LevelCluster::~LevelCluster()
{
	if(cluster_rslt!= NULL)
	{
		delete[] cluster_rslt;
		cluster_rslt = NULL;
	}
}
/*int LevelCluster::loadData(string name)
{
	if(read_file_no_rslt(X_orig, name) != -1)
	{
		ML_HC_INFO("original X"<<X_orig);
		return 0;
	}

	return -1;
}*/
int LevelCluster::setParas(int n)
{
	if(n>X_orig.length()||n<0)
	{
		ML_HC_INFO("setParas, wrong paras K");
		return -1;
	}
	K = n;
	cluster_rslt = new vector<int>[K];
	return 0;
}

int LevelCluster::exectCluster()
{
	int m = X_orig.length();
	int n = X_orig.brandth();
	Matrix x = X_orig;
	//01. init assign matrix
	Direct* x_assign = new Direct[m];
	initDirectMap(x_assign,m);

	//init distance
	double* distance = new double[m*(m+1)/2]; //row*col_+col; col --
	x.computeDistance(distance);

	int i = m;
	int index = 0; //from 0 to k
	Matrix new_center(1,n,0);
	while(i>K)
	{
		INDEX min;
		getMin(min,distance,m,x_assign); //col is larger than row, only consider the 

		//case 1: both row has not been assigned before, so give it a new index
		if(x_assign[min.row].cluster_id == -1 && x_assign[min.col].cluster_id == -1)
		{
			ML_HC_INFO("case1");
			x_assign[min.row].cluster_id = index; 
			x_assign[min.col].cluster_id = index; 

			x_assign[min.col].bClose = true;

			cluster_rslt[index].push_back(min.row);
			cluster_rslt[index].push_back(min.col);

			//delete col in x, and replace row in x with the new centeroid
			new_center =( x.getRow(min.row, min.row) +x.getRow(min.col,min.col)).multiplyReal(1.0/2);

			//replace row in x with the new centeroid
			//x.subsituteRow(new_center,0,0,min.row,min.row);
	
			//modify the distance
			subsitueDistance(min.row,distance,m,x,new_center);

			index++; //next time, will assine new index
		}
		//case 2: y has been assigned
		else if(x_assign[min.row].cluster_id == -1 &&  x_assign[min.col].cluster_id != -1) //the bigger one has been assigned, smaller one assine to later
		{
			ML_HC_INFO("case2");
			int cluster_id = x_assign[min.col].cluster_id;

			//x will assigned to y
			x_assign[min.row].cluster_id = cluster_id;
			x_assign[min.row].bClose = true;

			cluster_rslt[cluster_id].push_back(min.row);
			//new center
			new_center =( x.getRow(min.row, min.row) +x.getRow(min.col,min.col)).multiplyReal(1.0/2);

			//x.subsituteRow(new_center,0,0,min.row,min.row);
			//modify the distance
			subsitueDistance(min.col,distance,m,x,new_center);
		}
		//case 3: x has alreay be assigned
		else if(x_assign[min.row].cluster_id != -1 && x_assign[min.col].cluster_id == -1)//it will never exist this situation
		{
			ML_HC_INFO("case3");
			int cluster_id = x_assign[min.row].cluster_id;

			x_assign[min.col].cluster_id = cluster_id;
			x_assign[min.col].bClose = true;

			cluster_rslt[cluster_id].push_back(min.col);
			//new center
			new_center =( x.getRow(min.row, min.row) +x.getRow(min.col,min.col)).multiplyReal(1.0/2);

			//x.subsituteRow(new_center,0,0,min.col,min.col);
			//modify the distance
			subsitueDistance(min.row,distance,m,x,new_center);
		}
		else//both have been assigned before
		{
			ML_HC_INFO("case4");
			//put y's index into x
			int cluster_id_min = x_assign[min.row].cluster_id;
			int cluster_id_max = x_assign[min.col].cluster_id;
			int min_1 = min.row;
			int max_1 = min.col;
			if(x_assign[min.row].cluster_id > x_assign[min.col].cluster_id)
			{
				cluster_id_min = x_assign[min.col].cluster_id;
				cluster_id_max= x_assign[min.row].cluster_id;
				min_1 = min.col;
				max_1 = min.row;
			}
			
			vector<int>::iterator ite = cluster_rslt[cluster_id_max].begin();
			for(;ite!=cluster_rslt[cluster_id_max].end();ite++)
			{
				cluster_rslt[cluster_id_min].push_back(*ite);
			}
			cluster_rslt[cluster_id_max].clear();
			 x_assign[max_1].bClose = true;

			//new center
			new_center =( x.getRow(min.row, min.row) +x.getRow(min.col,min.col)).multiplyReal(1.0/2);

			//x.subsituteRow(new_center,0,0,min.col,min.col);

			//modify the distance
			subsitueDistance(min_1,distance,m,x,new_center);
		}
		i--;
	}
	 int total_size =0;  
	 for(int i=0;i<K;i++)  //if k cluster sum togeher, but smaller than the total size
	 {  
		 total_size+=cluster_rslt[i].size();  
		
	 }  
	 if(total_size<m)  
	 {
		 ML_HC_INFO("total size smaller than m, need extra dealing");
		 int j=0;  
		 for(int i=0;i<m;i++)  
		 {  
			 if(!x_assign[i].bClose && x_assign[i].cluster_id == -1)  
			 {  
				 cluster_rslt[K-1-j].push_back(i);  
				 j++;  
			 }  
		 }  
	 } 
	 //output
	 for(int i=0;i<K;i++)
	 {
		 ML_HC_INFO("cluster "<<i);
		 vector<int>::iterator ite = cluster_rslt[i].begin();
			for(;ite!=cluster_rslt[i].end();ite++)
			{
				ML_HC_INFO("\t"<<*ite);
			}
			cout<<endl;
	 }

	 //free x_assign
	 delete[] x_assign;
	 delete[] distance;
	 return 0;
}
XY_PARA LevelCluster::getParasDrawInput()
{
	XY_PARA re;
	re.x_orig = X_orig;
	return re;
}
int LevelCluster::setTrainPara(CLUSTER_TRAIN_PARA p)
{
	return setParas(p.k);	
}