#include "io_manager.hpp"
#include "comm.hpp"

using namespace std;
void init_log()
{
	PropertyConfigurator::configure("log4cxx.properties");
	log4cxx::LoggerPtr ml_rir_logger(log4cxx::Logger::getLogger(""));
	return;
}

int read_input_file(Matrix& X,Matrix& Y,string filename)
{
	int m = 0;
	int n = 0;
	int init_m = 500, init_n=20;
	int init_n_y=10;
    double** x = malloc_2(0,init_m,init_n);
    double** y = malloc_2(0,init_m,init_n_y);
   ifstream  inputfile(filename);

   if(!inputfile)  
   {   
       cout << "cannot open file."<<endl;   
       return -1;   
   }  
   //used to test input and output data
   int col = 0;
   int lines = 0;
   //int labels =0;
   std::string str = "";
   char ch;
   
   inputfile.get(ch);
   bool is_n_set = false;
   while(!inputfile.eof())  
   {
        //ifile>>ch;//该方法不能读取空格回车 
		std::ostringstream  ss;
	    ss<<ch;
        if(ch=='\n') //transfer former input to num
        {
			if(!is_n_set)
			{
				n=col;//since there is result line
			    cout<<"n"<<n<<endl;
				is_n_set=true;
			}
			y[lines][0]=str2double(str.c_str());
			str.clear();
			lines++;
			col=0;       
        } 
        else if(ch==' '||ch == ',') 
        {
				
				x[lines][col]=str2double(str.c_str());
				str.clear();
				col++;

        }
		else if((ch>='0'&&ch <= '9') || (ch =='.'))
		{
			//strcat(ch_str,&ch); 
			str.append(ss.str());
		}
		else if(ch==';')
		{
			//labels_start = true;
			//labels_end = false;
		}
		inputfile.get(ch);
   }

   m=lines+1;
   cout<<"n"<<n<<endl;

   inputfile.close(); 
   y[lines][0]= str2double(str.c_str());

   X.setMatrix(x,m,n);
   Y.setMatrix(y,m,1);
   free_2(x,init_m,init_n);
   free_2(y,init_m,1);
   return 0;
}
int read_file_no_rslt(Matrix& X,std::string filename)//in
{
	int init_m = 500, init_n=20;
	int m=0,n=0;

   double** x = malloc_2(0,init_m,init_n);
   ifstream  inputfile(filename);

   if(!inputfile)  
   {   
       cout << "cannot open file."<<endl;   
       return -1;   
   }  
   //used to test input and output data
   int col = 0;
   int lines = 0;
   std::string str = "";
   char ch;
   
   inputfile.get(ch);
  bool is_n_set = false;
   while(!inputfile.eof())  
   {
        //ifile>>ch;//该方法不能读取空格回车 
		std::ostringstream  ss;
	    ss<<ch;
        if(ch=='\n') //transfer former input to num
        {
			if(!is_n_set)
			{
				n=col+1;
				is_n_set=true;
			}
			x[lines][col]=str2double(str.c_str());
			str.clear();
			lines++;
			col=0;       
        } 
        else if(ch==' '||ch == ',') 
        {				
			x[lines][col]=str2double(str.c_str());
			str.clear();
			col++;

        }
		else if((ch>='0'&&ch <= '9') || (ch =='.'))
		{
			str.append(ss.str());
		}

		inputfile.get(ch);
   }
   x[lines][col]=str2double(str.c_str());

   m=lines+1;
   //cout<<"n"<<n<<endl;
   inputfile.close(); 
   
   X.setMatrix(x,m,n);
   //cout<<"imput"<<endl;
   //X.printMatrix();
   free_2(x,init_m,init_n);
   return 0;
}

/*****************************************in functions***************************************/
int read_input_file_multiclass(Matrix& X,Matrix& Y,Matrix& Labels, string filename)
{
	int init_m = 500, init_n=20;
	int init_n_y=10;
    double** x = malloc_2(0,init_m,init_n);
    double** y = malloc_2(-1,init_m,1);
	double lab[20];
	for(int i=0;i<20;i++)
	{
		lab[i]=-1;
	}
   ifstream  inputfile(filename);

   if(!inputfile)  
   {   
       cout << "cannot open file."<<endl;   
       return -1;   
   }  
   //used to test input and output data
   int col = 0;
   int lines = 0;
   int n_lab =0;
   std::string str = "";
   char ch;
   
   int n = 0;
   int m =0;
   inputfile.get(ch);
   bool is_n_set = false;
   while(!inputfile.eof())  
   {
        //ifile>>ch;//该方法不能读取空格回车  
		std::ostringstream  ss;
	    ss<<ch;
        if(ch=='\n') //transfer former input to num
        {
			if(!is_n_set)
			{
				n=col;
				is_n_set = true;
			}

			y[lines][0]=str2double(str.c_str());

			str.clear();
			if(!findByValue(lab,y[lines][0],n_lab+1))
			{
				lab[n_lab]=y[lines][0];
				n_lab++;
			}
			lines++;

			col=0;

            
        } 
        else if((ch==' '||ch == ',')) 
        {
			x[lines][col]=str2double(str.c_str());
			str.clear();
			col++;
        }
		else if((ch>='0'&&ch <= '9') || (ch =='.'))
		{ 
			str.append(ss.str());
		}
		 inputfile.get(ch);
   }
   n_lab;
   m=lines+1;

   inputfile.close(); 
   y[lines][0]= str2double(str.c_str());

   X.setMatrix(x,m,n);
   Y.setMatrix(y,m,1);
   sortMin(lab,n_lab);
   Labels = Matrix(lab,n_lab,1);
   free_2(x,init_m,init_n);
   free_2(y,init_m,1);
   return 0;
}
/*static int time =0;
void output(std::string log)
{
	//std::string str;
	if(time==0)
	{
		ofstream out("out.txt",ios::out);
		out<<endl<<log;
	out.close();
		time++;
	}
	else
	{
	ofstream out("out.txt",ios::app);
			out<<endl<<log;
	out.close();
	}
	
}*/
int writePredict2File(Matrix& Y,std::string filename, int method)//in
{
	int m = Y.length();
	if(method == 0)
	{
		string* tmpV = new string[m];	
		ifstream in(filename);
		if(!in)  
		{   
			return -1;   
		}

		int i=0;
		while(!in.eof()) 
		{
			string tmp;
			getline(in,tmp,'\n');
			tmpV[i] =tmp;
			//ML_LIR_INFO(tmpV[i]);
			i++;
		}
		ofstream  out(filename,ios::out);
		if(!out)  
		{   
			return -1;   
		}
		for(int i=0;i<m;i++)
		{
			double num = Y.matrix_[i];
			if(i == m-1)
			{
				out<< tmpV[i]<<" "<<num; 
			}
			else
			{
				out<< tmpV[i]<<" "<<num<<endl; 
			}
		}
		in.close();
		out.close();
	}
	else
	{
		ofstream  out(filename,ios::out);
		if(!out)  
		{   
			return -1;   
		}
		for(int i=0;i<m;i++)
		{
			out<<Y.matrix_[i]<<endl;
		}
		out.close();
	}
	return 0;
}
double getParaFromUser()
{
	char re[200];
	cin>>re;
	return str2double(re);
}
string getStrFromUser()
{
	char re[200] = "";
	cin>>re;
	return re;
}