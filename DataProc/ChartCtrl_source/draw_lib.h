#ifndef DRAW_LIB_H_
#define DRAW_LIB_H_
#include "../stdafx.h"
#include "ChartCtrl.h"
#include "ChartAxis.h"
#include "ChartAxisLabel.h"
#include "ChartLineSerie.h"
#include "ChartPointsSerie.h"
#include "ChartBalloonLabel.h"
#include "ChartLegend.h"
#include "ChartSerie.h"
#include "../ml/io_manager.hpp"
#include <WinGDI.h>

#include <WinDef.h>

#include "../common.hpp"
//#include "../targetver.h"

#include <cmath>
#include <algorithm>
#include "../ml/comm.hpp"
typedef enum
{
	BLACK = 0,//color 0: black 000
	WHITE =1,//color 1: white  255 255 255
	BLUE =2,//color 2:  blue 255 00
	GREEN = 3,//color 3: green 0 250 0
	RED = 4,//color 4: red 0 0 250
	YELLOW = 5,//color 5:yellow 0 255 255
}COLOR;

template<typename T>
void serFuncColor(COLOR c,T& t, void (T::*pFunc)(COLORREF) )//make it a function pointer
{
  switch(c)
  {
  case BLACK:
	  (t.*pFunc)(RGB(0,0,0));
	  break;
  case WHITE:
	 (t.*pFunc)(RGB(255,255,255));
	  break;
  case BLUE:
	  (t.*pFunc)(RGB(255,0,0));
	  break;
  case GREEN:
	  (t.*pFunc)(RGB(0,250,0));
	  break;
  case RED:
	  (t.*pFunc)(RGB(0,0,255));
	  break;
  case YELLOW:
	  (t.*pFunc)(RGB(0,255,255));
	  break;
  default:
	  break;
  }
  return;
}
typedef struct
{
	double min_bot;
	double max_bot;
	double min_lef;
	double max_lef;
	bool bAuto;
}AXIS_SCALE;

void buildAxis(CChartCtrl& m_chart,AXIS_SCALE sca, CString& title, CString& left_label, CString& bottom_label);

void draw_line(CChartCtrl& m_chart, Matrix x, Matrix y, CString& line_name);
void draw_line_log(CChartCtrl& m_chart, Matrix x, Matrix y, CString& line_name);
void draw_point2(CChartCtrl& m_chart,Matrix& x, Matrix&y, Matrix& label);//used for multilabel
void draw_point(CChartCtrl& m_chart,Matrix x, Matrix y, CString& name);

#endif