#include "apriori.hpp"

void insertByAscendOrder(int item[],int n,int value)
{
	int i=0;
	for(;i<n;)
	{
		if(item[i]!=-1&&value>item[i])
		{
			i++;
		}
		else
		{
			break;//insert at the 0th pos
		}
	}
	if(item[i]==-1)//insert directly
	{
		item[i]=value;
		return;
	}
	else//i之后的位置往后移动
	{
		for(int j=n-1;j>i;j--)
		{
			if(item[j-1]!=-1)
			{
				item[j]=item[j-1];

			}
		}
		item[i]=value;
		return;
	}
}
/*********************************DVector*********************************************/
DVector::DVector(int total)
{
	totalD = total;
	D_orig = new vector<int>[totalD];
}
DVector::DVector()
{
	totalD=0;
	D_orig = NULL;
}
DVector::~DVector()
{
	if(D_orig!= NULL)
	{
		delete[] D_orig;
		D_orig = NULL;
	}
}
DVector::DVector(const DVector& m)
{
	if(this != &m)
	{
		totalD = m.totalD;
		D_orig= new vector<int>[totalD];
		for(int i=0;i<totalD;i++)
		{
			D_orig[i].clear();
			vector<int>::iterator  ite = m.D_orig[i].begin();
			for(;ite!=m.D_orig[i].end();ite++)
			{
				D_orig[i].push_back(*ite);
			}
		}
	}
	
}
const DVector& DVector::operator=(const DVector& m)
{
	if(this != &m)
	{
		if(totalD!=0&&D_orig!=NULL)
		{
			delete[] D_orig;
			D_orig = NULL;
		}
		totalD = m.totalD;
		D_orig= new vector<int>[totalD];
		for(int i=0;i<totalD;i++)
		{
			D_orig[i].clear();
			vector<int>::iterator  ite = m.D_orig[i].begin();
			for(;ite!=m.D_orig[i].end();ite++)
			{
				D_orig[i].push_back(*ite);
			}
		}
	}
	return *this;
}
void DVector::print()
{
	for(int i=0;i<totalD;i++)
	{
		cout<<endl;
		cout<<"row = "<<i<<endl;
		vector<int>::iterator ite = D_orig[i].begin();
		for(;ite!= D_orig[i].end();ite++)
		{
			cout<<*ite<<"\t";
		}
		cout<<endl;
	}
}
	//void AddOneRow(string& item);
void DVector::AddOneItem(int item, int index)
{
	if(index>=totalD)
	{
		cout<<"sorry, no enough space"<<endl;
		return;
	}
	D_orig[index].push_back(item);
}
bool DVector::FindInSingleD(int index, int& item)
{
	vector<int>::iterator ite = D_orig[index].begin();
	for(;ite!= D_orig[index].end();ite++)
	{
		if(*ite==item)
		{
			return true;
		}
	}
	return false;
}
/**************************************AItem********************************************************/
AItem::AItem()
{
	item = NULL;
	count =0;
	k=0;
}
AItem::AItem(int v)
{
	k=v;
	count =0;
	if(k>0)
	{
		item = new int[k];
		for(int i=0;i<k;i++)
		{
			item[i]=-1;
		}
	}
}
AItem::~AItem()
{
	if(item!=NULL&&k>0)
	{
		delete[] item;
		item = NULL;
	}
}
AItem::AItem(const AItem& m)
{
	if(this!=&m)
	{
		count =m.count;
		k=m.k;
		if(k>0)
		{
			item = new int[k];
			for(int i=0;i<k;i++)
			{
				item[i]=m.item[i];
			}
		}
	}
}
const AItem AItem::operator=(const AItem& m)
{
	if(&m!=this)
	{
		count=m.count;
		if(k>0&& item!=NULL)
		{
			delete[] item;
			item = NULL;
		}
		k=m.k;
		if(k>0)
		{
			item = new int[k];
			for(int i=0;i<k;i++)
			{
				item[i]=m.item[i];
			}
		}
	}
	return *this;
}
void AItem::insertElem(int value) //it will insert by order
{
	insertByAscendOrder(item,k,value);
}
bool AItem::operator==(const AItem& m)//only comapre the item not the count
{
	for(int i=0;i<k;i++)
	{
		if(item[i]!=m.item[i])
		{
			return false;
		}
	}
	return true;
}
bool AItem::IsExisted(int value)
{
	for(int i=0;i<k;i++)
	{
		if(item[i]==value)
		{
			return true;
		}
	}
	return false;
}
AItem AItem::operator-(AItem& m2)
{
	AItem re(k-m2.k);
	if((k-m2.k)<=0)
	{
		return re;
	}
	for(int i=0;i<k;i++)
	{
		if(!m2.IsExisted(item[i]))
		{
			re.insertElem(item[i]);
		}
	}
	return re;
}
ostream& operator<<(ostream& out,const AItem& m)
{
	out<<endl;
	for(int i=0;i<m.k;i++)
	{
		out<<m.item[i]<<"\t";
	}
	out<<"count"<<m.count;
	out<<endl;
	return out;
}
/*************************Rule Item************************************************/

RuleItem::RuleItem()
{
	item_f = NULL;
	item_l=NULL;
	num_f=0;
	num_l=0;
	conf=0;
}
RuleItem::RuleItem(int f, int l)
{
	num_f=f;
	num_l=l;
	item_f = new int[num_f];
	item_l=new int[num_l];
	//init
	for(int i=0;i<num_f;i++)
	{
		item_f[i]=-1;
	}
	for(int i=0;i<num_l;i++)
	{
		item_l[i]=-1;
	}
	conf =0;
}
RuleItem::~RuleItem()
{
	if(item_f!=NULL)
	{
		delete[] item_f;
		item_f = NULL;
	}

	if(item_l!=NULL)
	{
		delete[] item_l;
		item_l = NULL;
	}
}

RuleItem::RuleItem(const RuleItem& m)
{
	if(this!=&m)
	{
		conf =m.conf;
		num_f=m.num_f;
		num_l=m.num_l;
		item_f = new int[num_f];
		item_l=new int[num_l];
		for(int i=0;i<num_f;i++)
		{
			item_f[i]=m.item_f[i];
		}

		for(int i=0;i<num_l;i++)
		{
			item_l[i]=m.item_l[i];
		}
	}
}
const RuleItem RuleItem::operator=(const RuleItem& m)
{
	RuleItem re;
	conf =m.conf;
	num_f=m.num_f;
	num_l=m.num_l;
	if(num_f!=0)
	{
		item_f = new int[num_f];	
		for(int i=0;i<num_f;i++)
		{
			item_f[i]=m.item_f[i];
		}
	}
	if(num_l!=0)
	{
		item_l=new int[num_l];
		for(int i=0;i<num_l;i++)
		{
			item_l[i]=m.item_l[i];
		}
	}
	return re;
}
ostream& operator<<(ostream& out,const RuleItem& m)
{
	out<<endl;
	for(int i=0;i<m.num_f;i++)
	{
		out<<m.item_f[i];
	}
	out<<"-->";
	for(int j=0;j<m.num_l;j++)
	{
		out<<m.item_l[j];
	}
	out<<" conf: "<<m.conf<<endl;
	return out;
}
void RuleItem::insertElemF(int value)
{
	insertByAscendOrder(this->item_f,num_f,value);
}
void RuleItem::insertElemL(int value)
{
	insertByAscendOrder(this->item_l,num_l,value);
}

bool RuleItem::operator==(const RuleItem& m)
{
	if(num_f!=m.num_f || num_l!=m.num_l)
	{
		return false;
	}
	if(num_f>0)
	{
		for(int i=0;i<num_f;i++)
		{
			if(item_f[i]!=m.item_f[i])
			{
				return false;
			}
		}
	}
	if(num_l>0)
	{
		for(int i=0;i<num_l;i++)
		{
			if(item_l[i]!=m.item_l[i])
			{
				return false;
			}
		}
	}
	return true;
}

/******************************************AVector****************************************/
AVector::AVector(int v):k(v)
{
}
AVector::AVector()
{
}
AVector::~AVector()
{
}
bool AVector::IsEmpty()
{
	return V.empty()? true: false;
}
AVector::AVector(AVector& m)
{
	if(this!=&m)
	{
		k=m.k;
		V.clear();
		vector<AItem>::iterator ite = m.V.begin();
		for(;ite!=m.V.end();ite++)
		{
			V.push_back(*ite);
		}
	}
}
AVector AVector::operator=(AVector& m)
{
	if(this!=&m)
	{
		this->k=m.k;
		this->V.clear();
		vector<AItem>::iterator ite = m.V.begin();
		for(;ite!=m.V.end();ite++)
		{
			V.push_back(*ite);
		}
	}
	return *this;
}
ostream& operator<<(ostream& out,AVector& m)
{
	vector<AItem>::iterator ite = m.V.begin();
	for(;ite!=m.V.end();ite++)
	{
		out<<*ite;
	}
	return out;
}

bool AVector::IsItemStored(AItem& value)
{
	if(value.k!=k)
	{
		cout<<"size not same"<<endl;
		return true;
	}
	vector<AItem>::iterator ite = V.begin();
	for(;ite!=V.end();ite++)
	{
		if(*ite==value)
		{
			return true;
		}
	}
	return false;
}
double AVector::FindCountByItemValue(AItem& value)
{
	if(value.k!=k)
	{
		cout<<"size not same"<<endl;
		return -1;
	}
	vector<AItem>::iterator ite = V.begin();
	for(;ite!=V.end();ite++)
	{
		if(*ite==value)
		{
			return ite->count;
		}
	}
	return -1;
}

void AVector::AddItem(AItem value)
{
	if(value.k !=k)
	{
		ML_AP_INFO("non avail k");
		return;
	}
	if(IsItemStored(value))
	{
		//cout<<"it has been stored before"<<endl;
		return;
	}
	V.push_back(value);
}
void AVector::AddCountForItem(AItem& value)
{
	vector<AItem>::iterator ite = V.begin();
	for(;ite!=V.end();ite++)
	{
		if(*ite==value)
		{
			ite->count++;
			return;
		}
	}
	//cout<<"not found"<<endl;
}
/*******************************class RVector*********************************************************/
RVector::RVector(){}
RVector::RVector(RVector& m)
{
	if(this!=&m)
	{
		V.clear();
		vector<RuleItem>::iterator ite = m.V.begin();
		for(;ite!=m.V.end();ite++)
		{
			V.push_back(*ite);
		}
	}
}
RVector::~RVector(){}
RVector RVector::operator=(RVector& m)
{
	if(this!=&m)
	{
		vector<RuleItem>::iterator ite = m.V.begin();
		for(;ite!=m.V.end();ite++)
		{
			V.push_back(*ite);
		}
	}
	return *this;
}
ostream& operator<<(ostream& out,RVector& m)
{
	vector<RuleItem>::iterator ite = m.V.begin();
	for(;ite!=m.V.end();ite++)
	{
		out<<*ite;
	}
	return out;
}
bool RVector::IsItemStored(RuleItem& value)
{
	vector<RuleItem>::iterator ite = V.begin();
	for(;ite!=V.end();ite++)
	{
		if(*ite==value)
		{
			return true;
		}
	}
	return false;
}
void RVector::AddItem(RuleItem value)
{
	if(IsItemStored(value))
	{
		ML_AP_INFO("it has been stored before");
		return;
	}
	V.push_back(value);
}
/********************************class Apriori**********************************************************/
Apriori::Apriori()
{
	numD = 0;
	L=NULL;
	minSupport = NULL;
	minConf = 0.6;
}
Apriori::~Apriori()
{
	if(L!=NULL)
	{
		delete[] L;
		L=NULL;
	}
	if(minSupport!=NULL)
	{
		delete[] minSupport;
		minSupport = NULL;
	}
}
void Apriori::setMinSupport(int m)
{
	minSupport = new double;
	*minSupport = m;
	return;
}
void Apriori::loadData(std::string& filename)//load data and generate C1
{
	read_strfile(X_orig,numD,filename);
	X_orig.print();
}
AVector Apriori::generateCanidate(const AVector& Hk_1,int k)
{
	AVector Ck(k);
	vector<AItem>::const_iterator iteo = Hk_1.V.begin();
	for(;iteo!=Hk_1.V.end();iteo++)
	{
		vector<AItem>::const_iterator itei = iteo+1;
		for(;itei!=Hk_1.V.end();itei++)
		{
			bool bEqual = true;
			for(int i=1;i<k-1;i++)
			{
				if(iteo->item[i]!=itei->item[i-1])
				{
					bEqual = false;
					break;
				}
			}
			if(bEqual)
			{
				AItem tmp(k);
				for(int i=0;i<k-1;i++)
				{
					tmp.insertElem(iteo->item[i]);
				}
				tmp.insertElem(itei->item[k-2]);
				Ck.AddItem(tmp);
			}
		}
	}
	ML_AP_INFO("C"<<k<<Ck);
	return Ck;
}
void Apriori::countCK(AVector& C, int k)
{
	if(C.k!=k)
	{
		cout<<"you has inputed wrong k"<<endl;
		return;
	}
	if(C.IsEmpty())
	{
		cout<<"C"<<k<<"is empty"<<endl;
		return;
	}

	vector<AItem>::iterator c_ite = C.V.begin();
	for(;c_ite!=C.V.end();c_ite++)
	{
		for (int i = 0; i<numD; i++)
		{
			bool bAdd = true;
			for(int m=0;m<k;m++)
			{
				if(!X_orig.FindInSingleD(i,c_ite->item[m]))
				{
					bAdd = false;
					break;
				}
			}
			if(bAdd)
			{
				C.AddCountForItem(*c_ite);
			}
		}
	}
	ML_AP_INFO("Ltmp"<<k<<C);
	return;
}
void Apriori::createC1andGenerateL1(DVector& x_orig)
{
	AVector C1(1);
	for (int i = 0; i<numD; i++)
	{
		vector<int>::iterator ite = x_orig.D_orig[i].begin();
		for(;ite!=x_orig.D_orig[i].end();ite++)
		{
			AItem a(1);
			a.insertElem(*ite);
			if(!C1.IsItemStored(a))
			{
				a.count = 1;
				C1.AddItem(a);
			}
			else
			{
				C1.AddCountForItem(a);
			}

		}
	}
	//init L and the max dimension is (int)C1.V.size()
	totalItem =(int)C1.V.size();
	L= new AVector[(int)C1.V.size()+1];
	ML_AP_INFO("C"<<1<<endl<<C1);
	scanD(C1,1);
	return;
}
void Apriori::scanD(AVector& C, int k) //used for k>=1, count for C
{
	if(C.k!=k)
	{
		cout<<" not the same dimension"<<endl;
		return;
	}
	if(C.IsEmpty())
	{
		cout<<"C"<<k<<"is empty"<<endl;
		return;
	}
	L[k].k=k;
	vector<AItem>::iterator ite = C.V.begin();
	for(;ite!=C.V.end();ite++)
	{
		if(ite->count>=*minSupport)
		{
			L[k].V.push_back(*ite);
		}
	}
	ML_AP_INFO("L"<<k<<endl<<L[k]);
}
void Apriori::generateL(int k)
{
	AVector Ck = generateCanidate(L[k-1],k);
	countCK(Ck,k);
	scanD(Ck,k);
}
void Apriori::runApriori()
{
	createC1andGenerateL1(X_orig);
	int k=1;

	for(;!L[k].IsEmpty();k++)
	{
		generateL(k+1);
	}
	endK = k-1;

	//delete extra space
	//for(int i=k;i<totalItem+1;i++)
	//{
		//delete &L[i];
	//}
	ML_AP_INFO("endK"<<endK);
}
/***************************rules********************************************/
AVector Apriori::calcConf(AItem& freqset, AVector& H)
{
	AVector prunedH(H.k);

	int k_former = freqset.k-H.k;
	int k_later = H.k;

	vector<AItem>::iterator ite = H.V.begin(); 
	for(;ite!=H.V.end();ite++)
	{
		int tmpk=ite->k;
		AItem former = freqset-*ite;
		if(former.k==0) //just in case
		{
			continue;
		}		
		double count_subseq = L[k_former].FindCountByItemValue(former);
		if(count_subseq!=-1)
		{
			double conf = (double)freqset.count/(double)count_subseq;
			if(conf>=minConf)
			{
				RuleItem a(k_former,k_later);
				//add former part
				for(int i=0;i<k_former;i++)
				{
					a.insertElemF(former.item[i]);
				}
				//add later part
				for(int i=0;i<k_later;i++)
				{
					a.insertElemL(ite->item[i]);
				}
				a.conf = conf;	
				R.AddItem(a);
				//add for prunedH
				prunedH.AddItem(*ite);
			}
		}
	}
	return prunedH;
}
void Apriori::generateRulesFromConseq(AItem& freqset, AVector& H) //unprunedH3
{
	int m=H.k;
	if(freqset.k>m)
	{		
		AVector prunedh = calcConf(freqset,H);//compute h1, and returen pruned h1
		AVector htmp = generateCanidate(prunedh,m+1); //generate the later 
		if(htmp.k>1 && !htmp.IsEmpty())
		{
			generateRulesFromConseq(freqset,htmp);
		}
	}
}
void Apriori::generateRules()
{
	R.V.clear();
	if(endK<2)
	{
		ML_AP_INFO("last freqset is smaller than 2, has no associate rules");
		return;
	}
	//calc for i==2
	//对每一个频繁项集
	vector<AItem>::iterator  ite1 = L[2].V.begin();
	for(;ite1!=L[2].V.end();ite1++)
	{
		AVector H1(1);
		for(int i=0;i<ite1->k;i++)
		{
			AItem tmp(1);
			tmp.insertElem(ite1->item[i]);
			H1.AddItem(tmp);
		}
		ML_AP_INFO("H"<<1<<H1);
		calcConf(*ite1,H1);//use L1==H1 and freqset,H1在这里是没有用的
	}
	ML_AP_INFO("RVector"<<1<<R);	
	//计算i>=3的情况
	for(int i=3;i<=endK;i++)
	{
		//对每一个频繁项集
		vector<AItem>::iterator  ite2 = L[i].V.begin(); //unpruned H3
		for(;ite2!=L[i].V.end();ite2++)
		{
			AVector H2(1);
			for(int i=0;i<ite2->k;i++)
			{
				AItem tmp(1);
				tmp.insertElem(ite2->item[i]);
				H2.AddItem(tmp);
			}
			ML_AP_INFO("H"<<2<<H2);
			generateRulesFromConseq(*ite2,H2); //首先ite和H1的长度是相等的,开始H0的长度为1，后来会增加
		}		
	}
	ML_AP_INFO("RVector"<<2<<R);	
}
//interface
void Apriori::setPara(OTHER_ALGO_PARA& para)
{
	setMinSupport(para.minSupport);
	return;
}
void Apriori::executeAlgo()
{
	runApriori();
	generateRules();
	return;
}