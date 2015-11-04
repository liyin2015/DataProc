#include "draw_lib.h"

void buildAxis(CChartCtrl& m_chart,AXIS_SCALE sca, CString& title, CString& left_label, CString& bottom_label)
{
	m_chart.EnableRefresh(false);
	//aixs
	CChartAxis *pAxis= NULL;   
	pAxis = m_chart.CreateStandardAxis(CChartCtrl::BottomAxis);  
	if(!sca.bAuto)
	{
		pAxis->SetMinMax(sca.min_bot,sca.max_bot);
	}
	else
	{
		pAxis->SetAutomatic(true);
	}
	//pAxis->SetAutomatic(true);  

	pAxis = m_chart.CreateStandardAxis(CChartCtrl::LeftAxis);  
	//pAxis->SetAutomatic(true);
	if(!sca.bAuto)
	{
		pAxis->SetMinMax(sca.min_lef,sca.max_lef);
	}
	else
	{
		pAxis->SetAutomatic(true);
	}


	//title
	//TChartString str1; 
	//str1 = _T("input");  
	m_chart.GetTitle()->AddString((TChartString)title); 

	//label
	pAxis = m_chart.GetLeftAxis();;
	CChartAxisLabel* pLabel = NULL;
	//str1=_T("Left Axis");
	if(pAxis!=NULL)
		pLabel = pAxis->GetLabel();
	if(pLabel!=NULL)
		pLabel->SetText((TChartString)left_label);

	//str1 = _T("x1");
	pAxis=m_chart.GetBottomAxis();
	if(pAxis!=NULL)
		pLabel = pAxis->GetLabel();
	if(pLabel!=NULL)
		pLabel->SetText((TChartString)bottom_label);

	//legend
	CChartLegend* pLeg = m_chart.GetLegend();
	pLeg->SetVisible(true);
	pLeg->SetHorizontalMode(true);
	pLeg->UndockLegend(80,40);


	m_chart.EnableRefresh(true);
	return;
}

void draw_line(CChartCtrl& m_chart, Matrix x, Matrix y, CString& line_name)
{
	m_chart.EnableRefresh(false);
	//test drawing pic 
	CChartLineSerie* pLineSerie1 = NULL;
	//TChartString name=_T("decison boundary");
	double* xd = x.matrix_;
	double* yd = y.matrix_; 
	int count = y.length();
	//m_chart.RemoveAllSeries();//clear

	pLineSerie1=m_chart.CreateLineSerie();
	pLineSerie1->SetSeriesOrdering(poNoOrdering);//set it to no order
	pLineSerie1->AddPoints(xd,yd,count);

	pLineSerie1->SetName((TChartString)line_name);

	m_chart.EnableRefresh(true);  //this is important , what its function
}
void draw_line_log(CChartCtrl& m_chart, Matrix x, Matrix y, CString& line_name)
{
	m_chart.EnableRefresh(false);
	//test drawing pic 
	CChartLineSerie* pLineSerie1 = NULL;
	//TChartString name=_T("decison boundary");
	double* x1 = x.getCol(0,0).matrix_;
	double* x2 =  x.getCol(1,1).matrix_;
	double* yd = y.matrix_; 
	int count = y.length();
	//m_chart.RemoveAllSeries();//clear

	pLineSerie1=m_chart.CreateLineSerie();
	pLineSerie1->SetSeriesOrdering(poNoOrdering);//set it to no order
	//pLineSerie1->AddPoints(xd,yd,count);
	pLineSerie1->SetName((TChartString)line_name);

	m_chart.EnableRefresh(true);  //this is important , what its function
}
void draw_point2(CChartCtrl& m_chart,Matrix& x, Matrix&y, Matrix& label)//used for multilabel
{
	m_chart.EnableRefresh(false);

	int row = x.length();
	int col = x.brandth();
	int n_lab = label.length();
	double* label_d = label.matrix_;

	double** tmpx_1= malloc_2(0,n_lab,row);//y==0
	double** tmpx_2 =malloc_2(0,n_lab,row);//y==1

	int* num_of_lab = malloc_1<int>(n_lab);//num of data for label i

	for(int i=0;i<row;i++)
	{
		int ind = (int)(*y(i,0));
		/*if(ind == -1)//since for svm, the negative train set will be -1
		{
			ind =0;
		}*/
		tmpx_1[ind][num_of_lab[ind]] = *x(i,0);

		if(col == 2)
		{
			tmpx_2[ind][num_of_lab[ind]] = *x(i,1);//2D
		}
		else if(col ==1)
		{
			tmpx_2[ind][num_of_lab[ind]] = 0;//1D
		}
		num_of_lab[ind]++;
	}
	COLOR c = GREEN;
	for(int i = 0;i<n_lab;i++)
	{
		std::string name = "label ";
		name.append(transToStr(label_d[i]));
		wstring wname(name.begin(),name.end());
	    //TChartString name=_T("label");
		CChartPointsSerie* pPointSerie = NULL;	
		pPointSerie = m_chart.CreatePointsSerie();

		CChartSerie* pSerie = m_chart.GetSerie(pPointSerie->GetSerieId());
		serFuncColor<CChartSerie>((COLOR)(c+i),*pSerie,&(CChartSerie::SetColor));
		pPointSerie->AddPoints(tmpx_1[i],tmpx_2[i],num_of_lab[i]);
		pPointSerie->SetName(wname);
		m_chart.EnableRefresh(true);	
		//pPointSerie->CreateBalloonLabel(i,wname);
	}
	free_2(tmpx_1, n_lab, row);
	free_2(tmpx_2,n_lab,row);
	free_1<int>(num_of_lab,n_lab);
	m_chart.EnableRefresh(true);
}
void draw_point(CChartCtrl& m_chart,Matrix x, Matrix y, CString& name)
{
	m_chart.EnableRefresh(false);

	double* xd = x.matrix_;
	double* yd = y.matrix_;
	int count = x.length();

	CChartPointsSerie* pPointSerie = NULL;
	//TChartString name=_T("input");
	pPointSerie = m_chart.CreatePointsSerie();


	//serFuncColor<CChartCtrl>(GREEN,m_chart,&(CChartCtrl::SetBorderColor));

	pPointSerie->AddPoints(xd,yd,count);
	pPointSerie->SetName((TChartString)name);
	m_chart.EnableRefresh(true);  
	//m_chart.hold
}
