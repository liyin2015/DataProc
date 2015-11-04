#ifndef APRIORI_HPP_
#define APRIORI_HPP_

#include<iostream>
#include<vector>
#include "../matrix.hpp"
#include "../io_manager.hpp"
#include "../otherAlgo_Base.hpp"

using namespace std;
/**************************class DVector**********************************************************/
class DVector //used for transaction
{
public:
	DVector(int totalD);
	DVector();
	~DVector();
	DVector::DVector(const DVector& m);
	const DVector& operator=(const DVector& m);
	void AddOneItem(int item, int index);
	bool FindInSingleD(int index, int& item);
	void print();
public:
	vector<int>* D_orig;
	int totalD;	
};
/***************************class AItem*************************************************/
class AItem
{
public:
	AItem();
	AItem(int v);
	~AItem();
	AItem(const AItem& m);
	AItem operator-(AItem& m2);
	friend ostream& operator<<(ostream& out,const AItem& m);
	const AItem operator=(const AItem& m);
	void insertElem(int value);
	bool IsExisted(int value);
	bool operator==(const AItem& m);
public:
	int* item;
	int k;
	int count;
};
/***************************************RuleItem********************************/
class RuleItem
{
public:
	RuleItem();
	RuleItem(int f, int l);
	~RuleItem();
	RuleItem(const RuleItem& m);
	friend ostream& operator<<(ostream& out,const RuleItem& m);
	const RuleItem operator=(const RuleItem& m);
	void insertElemF(int value);
	void insertElemL(int value);
	bool operator==(const RuleItem& m);
public:
	int* item_f;
	int* item_l;
	int num_f;
	int num_l;
	double conf;
};
/************************class AVector*************************************************************/
class AVector//used for candidates and l
{
public:
	AVector(int v);
	AVector();
	AVector(AVector& m);
	~AVector();
	AVector operator=(AVector& m);
	friend ostream& operator<<(ostream& out,AVector& m);
	bool IsItemStored(AItem& value);
	double FindCountByItemValue(AItem& value);
	void AddItem(AItem value);
	void AddCountForItem(AItem& value);
	bool IsEmpty();
public:
	vector<AItem> V;
	int k;
};
/**************************RVector************************************************************/
class RVector//used for candidates and l
{
public:
	RVector();
	RVector(RVector& m);
	~RVector();
	RVector operator=(RVector& m);
	friend ostream& operator<<(ostream& out,RVector& m);
	bool IsItemStored(RuleItem& value);
	void AddItem(RuleItem value);
public:
	vector<RuleItem> V;
};
/************************class Apriori*****************************************************/

class Apriori:public OtherAlgo_Base
{
public:
	Apriori();
	~Apriori();
	void setMinSupport(int m);
	void loadData(std::string& filename); //interface
	AVector generateCanidate(const AVector& Hk_1,int k);//generate L1 to C2,
	void createC1andGenerateL1(DVector& x_orig);

	AVector generateC();//used to generate C2,C3,C4.....
	
	void countCK(AVector& C, int k);//used to get count for C2,C3,C4...

	void scanD(AVector& C, int k); //used to get those who has minsupport for C2,C3,C4....

	void generateL(int k);
	void runApriori(); //interface

	//关联规则
	void generateRules();//use Lk and conf
	void generateRulesFromConseq(AItem& freqset, AVector& H);
	AVector calcConf(AItem& freqset, AVector& H);//计算置信度

	//interface

	void setPara(OTHER_ALGO_PARA& para);
	void executeAlgo();
public:
	DVector X_orig;//change to matrix 
	AVector* L;
	RVector R;
	//int minSupport;//from 1 to k, at first, make it all the same
	double* minSupport;
	double minConf;
	int numD;
	int endK;
	int totalItem;
};

#endif
