
// GraphicsBSplineCurveView.cpp: CGraphicsBSplineCurveView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "GraphicsBSplineCurve.h"
#endif

#include "GraphicsBSplineCurveDoc.h"
#include "GraphicsBSplineCurveView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGraphicsBSplineCurveView

IMPLEMENT_DYNCREATE(CGraphicsBSplineCurveView, CView)

BEGIN_MESSAGE_MAP(CGraphicsBSplineCurveView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CGraphicsBSplineCurveView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BSpline, &CGraphicsBSplineCurveView::OnBspline)
	ON_COMMAND(ID_Rotate, &CGraphicsBSplineCurveView::OnRotate)
	ON_COMMAND(ID_Symmetry, &CGraphicsBSplineCurveView::OnSymmetry)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CGraphicsBSplineCurveView 构造/析构

CGraphicsBSplineCurveView::CGraphicsBSplineCurveView() noexcept
{
	// TODO: 在此处添加构造代码
	boolLButtonDown = false;
	type = 0;
	N = 3;
	K = 4;
	nPoints = 30;
	pointList.RemoveAll();
	 FirstPoint=false;//标记是否为第一个点
	 LastPoint=false;//标记是否为第最后一个点
}

CGraphicsBSplineCurveView::~CGraphicsBSplineCurveView()
{
}

BOOL CGraphicsBSplineCurveView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CGraphicsBSplineCurveView 绘图

void CGraphicsBSplineCurveView::OnDraw(CDC* pDC)
{
	CGraphicsBSplineCurveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	/*************************************************************************/
	CRect rect;
	this->GetClientRect(rect);
	pDC->FillSolidRect(&rect, RGB(255, 255, 255));//为防止残留曲线的干扰，重画前清空面板
	/*************************************************************************/

//之所以不用pointList进行重画，是因为在功能三中，会重复使用pointList这个顶点列表，为了保证在功能一二三中均重画，必须保留下最初始的顶点列表
	if (objList.GetSize() != 0)//要保证非空才能重画
	{
		MapObj* obj = (MapObj*)objList.GetAt(0);//只取图形列表中第一个图形
		int pointsize = obj->points.GetSize();
		pointList.RemoveAll();
		for (int j = 0; j < pointsize; j++)//将顶点集从obj中取至pointList中
		{
			CPoint p1 = obj->points.GetAt(j);
			pointList.Add(p1);
		}

		/************************************************************************************************/
		if (type == 1)//功能1重画
		{
			for (int j = 0; j < pointsize - 1; j++)//画出默认的控制曲线
			{
				CPoint p1 = obj->points.GetAt(j);
				CPoint p2 = obj->points.GetAt((j + 1) % pointsize);
				pDC->MoveTo(p1);
				pDC->LineTo(p2);
			}
			Draw3ColorBSpline();
		}
		else if (type == 2)//功能2重画
		{
			pointPrintList.RemoveAll();
			Draw3ColorBSpline();
			CPoint tempP = pointList.GetAt(0);
			int xcenter = tempP.x;
			for (int i = 1; i < pointList.GetSize(); i++)
			{
				tempP = pointList.GetAt(i);
				tempP.x = xcenter * 2 - tempP.x;
				pointList.SetAt(i, tempP);
			}
			pointPrintList.RemoveAll();
			Draw3ColorBSpline();
		}
		else if (type == 3)//功能3重画
		{
			double L = 0.5;
			double Alpha = 45;
			Alpha *= 3.1415926 / 180;
			double c = cos(Alpha);
			double s = sin(Alpha);
			mPM[0][0] = 1;
			mPM[0][1] = 0;
			mPM[0][2] = L * c;
			mPM[0][3] = 0;
			mPM[1][0] = 0;
			mPM[1][1] = 1;
			mPM[1][2] = L * s;
			mPM[1][3] = 0;
			mPM[2][0] = 0;
			mPM[2][1] = 0;
			mPM[2][2] = 0;
			mPM[2][3] = 0;
			mPM[3][0] = 0;
			mPM[3][1] = 0;
			mPM[3][2] = 0;
			mPM[3][3] = 1;
			// 坐标变换
			Point3D *curve = new Point3D[pointPrintList.GetSize()];
			CPoint tempP = pointList.GetAt(0);
			CClientDC pDC(this);
			CPen *oldpen;
			COLORREF color[3] = { RGB(255,0,0), RGB(0,255,0), RGB(0,0,255) };
			int xcenter = tempP.x;
			for (int i = 0; i < 36; i++)
			{
				// 计算3D
				Alpha = i * 10 * 3.1415926 / 180;
				for (int j = 0; j < pointPrintList.GetSize(); j++)
				{
					tempP = pointPrintList.GetAt(j);
					curve[j].x = (tempP.x - xcenter) * cos(Alpha) + xcenter;
					curve[j].y = tempP.y;
					curve[j].z = (tempP.x - xcenter) * sin(Alpha);
				}
				// 计算2D
				From3DTo2D(curve, pointPrintList.GetSize());
				// 画图
				pDC.MoveTo(pointPrint3DList.GetAt(0));
				for (int j = 1; j < pointPrint3DList.GetSize(); j++)
				{
					CPen pen;
					pen.CreatePen(PS_SOLID, 2, color[j / (nPoints + 1) % 3]);
					oldpen = pDC.SelectObject(&pen);
					pDC.LineTo(pointPrint3DList.GetAt(j));
					pDC.SelectObject(oldpen);
				}
			}
		}

		/************************************************************************************************/
	}
	


}


// CGraphicsBSplineCurveView 打印


void CGraphicsBSplineCurveView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CGraphicsBSplineCurveView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CGraphicsBSplineCurveView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CGraphicsBSplineCurveView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CGraphicsBSplineCurveView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGraphicsBSplineCurveView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGraphicsBSplineCurveView 诊断

#ifdef _DEBUG
void CGraphicsBSplineCurveView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphicsBSplineCurveView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphicsBSplineCurveDoc* CGraphicsBSplineCurveView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphicsBSplineCurveDoc)));
	return (CGraphicsBSplineCurveDoc*)m_pDocument;
}
#endif //_DEBUG


// CGraphicsBSplineCurveView 消息处理程序
void CGraphicsBSplineCurveView::Draw3ColorBSpline()// 画出三色B样条曲线
{
	// TODO: 在此处添加实现代码.
	CClientDC pDC(this);
	CPen *oldpen;
	COLORREF color[3] = { RGB(255,0,0), RGB(0,255,0), RGB(0,0,255) };
	BSplineToPoints();
	pDC.MoveTo(pointPrintList.GetAt(0));
	for (int i = 1; i < pointPrintList.GetSize(); i++)
	{
		CPen pen;
		pen.CreatePen(PS_SOLID, 3, color[i / (nPoints + 1) % 3]);
		oldpen = pDC.SelectObject(&pen);
		pDC.LineTo(pointPrintList.GetAt(i));
		pDC.SelectObject(oldpen);
	}

}


void CGraphicsBSplineCurveView::From3DTo2D(Point3D* c, int n)// 坐标变换
{
	// TODO: 在此处添加实现代码.
	double r;
	CPoint tempP;
	pointPrint3DList.RemoveAll();
	for (int i = 0; i < n; i++)
	{
		r = c[i].x * mPM[3][0] + c[i].y * mPM[3][1] + c[i].z * mPM[3][2] + mPM[3][3];
		tempP.x = long((c[i].x * mPM[0][0] + c[i].y * mPM[0][1] + c[i].z * mPM[0][2] + mPM[0][4]) / r);
		tempP.y = long((c[i].x * mPM[1][0] + c[i].y * mPM[1][1] + c[i].z * mPM[1][2] + mPM[1][4]) / r);
		pointPrint3DList.Add(tempP);
	}
}


void CGraphicsBSplineCurveView::BSplineToPoints()
{
	// TODO: 在此处添加实现代码.
	double *knots = new double[N + K + 1];
	double u;
	double delt;
	for (int j = 0; j < pointList.GetSize() - N; j++)
	{
		for (int i = 0; i < N + K + 1; i++)
		{
			knots[i] = i;
		}
		u = knots[K - 1];
		delt = (knots[N + 1] - knots[K - 1]) / (double)(nPoints);
		for (int i = 0; i < nPoints + 1; i++)
		{
			pointPrintList.Add(BSpline(knots, N + K + 1, u, j));
			u += delt;
		}
	}


}


CPoint CGraphicsBSplineCurveView::BSpline(double knot[], int num, double u, int t)
{
	// TODO: 在此处添加实现代码.
	CPoint ret = CPoint(0, 0);
	double temp;
	for (int i = 0; i < N + 1; i++)
	{
		temp = Base(i, K, knot, num, u);
		ret.x += long(pointList.GetAt(i + t).x * temp);
		ret.y += long(pointList.GetAt(i + t).y * temp);
	}
	return ret;
}


double CGraphicsBSplineCurveView::Base(int i, int k, double knot[], int num, double u)
{
	// TODO: 在此处添加实现代码.
	double temp1 = 0;
	double temp2 = 0;
	if (k == 1)
	{
		if (i == (num - 2))
		{
			if ((u >= knot[i]) && (u <= knot[i + 1])) return 1;
			else return 0;
		}
		else
		{
			if ((u >= knot[i]) && (u < knot[i + 1])) return 1;
			else return 0;
		}
	}
	else if (k > 1)
	{
		if (knot[i + k - 1] != knot[i])
		{
			temp1 = Base(i, k - 1, knot, num, u);
			temp1 = (u - knot[i])*temp1 / (knot[i + k - 1] - knot[i]);
		}
		if (knot[i + k] != knot[i + 1])
		{
			temp2 = Base(i + 1, k - 1, knot, num, u);
			temp2 = (knot[i + k] - u) * temp2 / (knot[i + k] - knot[i + 1]);
		}
		return temp1 + temp2;
	}
	return 0;
}


void CGraphicsBSplineCurveView::OnBspline()
{
	// TODO: 在此添加命令处理程序代码
	type = 1;
	pointList.RemoveAll();
	pointPrintList.RemoveAll();
}

void CGraphicsBSplineCurveView::OnSymmetry()
{
	// TODO: 在此添加命令处理程序代码
	/****************************************************************************************/
	/**
	CRect rect;//边界值
	this->GetClientRect(rect);
	int RightBorder = rect.right;
	int xcenter = RightBorder/2;//用作将对称轴改为视窗中央
	**/
	/****************************************************************************************/
	
	/*************************************************************************/
	CDC* pDC = this->GetDC();	//构造设备环境对象
	CRect rect;
	this->GetClientRect(rect);
	pDC->FillSolidRect(&rect, RGB(255, 255, 255));//清空面板
	/*************************************************************************/

	type = 2;
	pointPrintList.RemoveAll();
	Draw3ColorBSpline();
	CPoint tempP = pointList.GetAt(0);
	int xcenter = tempP.x;
	for (int i = 1; i < pointList.GetSize(); i++)
	{
		tempP = pointList.GetAt(i);
		tempP.x = xcenter * 2 - tempP.x;
		pointList.SetAt(i, tempP);
	}
	pointPrintList.RemoveAll();
	Draw3ColorBSpline();


}


void CGraphicsBSplineCurveView::OnRotate()
{
	// TODO: 在此添加命令处理程序代码
	/*************************************************************************/
	CClientDC pDC(this);	//构造设备环境对象
	CRect rect;
	this->GetClientRect(rect);
	pDC.FillSolidRect(&rect, RGB(255, 255, 255));//清空面板
	/*************************************************************************/

	type = 3;
	// 变换矩阵
	double L = 0.5;
	double Alpha = 45;
	Alpha *= 3.1415926 / 180;
	double c = cos(Alpha);
	double s = sin(Alpha);
	mPM[0][0] = 1;
	mPM[0][1] = 0;
	mPM[0][2] = L * c;
	mPM[0][3] = 0;
	mPM[1][0] = 0;
	mPM[1][1] = 1;
	mPM[1][2] = L * s;
	mPM[1][3] = 0;
	mPM[2][0] = 0;
	mPM[2][1] = 0;
	mPM[2][2] = 0;
	mPM[2][3] = 0;
	mPM[3][0] = 0;
	mPM[3][1] = 0;
	mPM[3][2] = 0;
	mPM[3][3] = 1;
	// 坐标变换
	Point3D *curve = new Point3D[pointPrintList.GetSize()];
	CPoint tempP = pointList.GetAt(0);
	//CClientDC pDC(this);
	CPen *oldpen;
	COLORREF color[3] = { RGB(255,0,0), RGB(0,255,0), RGB(0,0,255) };
	int xcenter = tempP.x;
	for (int i = 0; i < 36; i++)
	{
		// 计算3D
		Alpha = i * 10 * 3.1415926 / 180;
		for (int j = 0; j < pointPrintList.GetSize(); j++)
		{
			tempP = pointPrintList.GetAt(j);
			curve[j].x = (tempP.x - xcenter) * cos(Alpha) + xcenter;
			curve[j].y = tempP.y;
			curve[j].z = (tempP.x - xcenter) * sin(Alpha);
		}
		// 计算2D
		From3DTo2D(curve, pointPrintList.GetSize());
		// 画图
		pDC.MoveTo(pointPrint3DList.GetAt(0));
		for (int j = 1; j < pointPrint3DList.GetSize(); j++)
		{
			CPen pen;
			pen.CreatePen(PS_SOLID, 2, color[j / (nPoints + 1) % 3]);
			oldpen = pDC.SelectObject(&pen);
			pDC.LineTo(pointPrint3DList.GetAt(j));
			pDC.SelectObject(oldpen);
		}
	}
}





void CGraphicsBSplineCurveView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (type == 1)
	{
		/*****************************************************************/
		/*****************************************************************/
		if (!FirstPoint)
		{
			FirstPoint = true;
			pointList.Add(point);
			pointList.Add(point);
		}
		/*****************************************************************/
		/*****************************************************************/
		boolLButtonDown = true;
		this->SetCapture();	//捕捉鼠标
		startPoint = point;
		endPoint = point;
		pointList.Add(point);
	}


	CView::OnLButtonDown(nFlags, point);
}


void CGraphicsBSplineCurveView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	
	CView::OnLButtonUp(nFlags, point);
}


void CGraphicsBSplineCurveView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (type==1)
	{
		/*************************************************************************/
		/*************************************************************************/
		if (!LastPoint)
		{
			LastPoint = true;
			pointList.Add(point);
			pointList.Add(point);
		}
		/*************************************************************************/
		/*************************************************************************/

		ReleaseCapture();//释放鼠标
		boolLButtonDown = false;
		Draw3ColorBSpline();

		/*************************************************************/
		/*************************************************************/
		//为重画存下目前的顶点
		MapObj* obj = new MapObj();
		for (int i = 0; i < pointList.GetSize(); i++)
		{
			obj->points.Add(pointList.GetAt(i));
		}
		objList.Add(obj);//将此次图形的所有点保存下来
		/*************************************************************/
		/*************************************************************/
	}
	CView::OnLButtonDblClk(nFlags, point);
}


void CGraphicsBSplineCurveView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	if ((type == 1)&&boolLButtonDown)
	{
		CDC* pDC = this->GetDC();	//构造设备环境对象
		pDC->SetROP2(R2_NOT);		//设置绘图模式为R2_NOT
		pDC->MoveTo(startPoint);
		pDC->LineTo(endPoint);
		pDC->MoveTo(startPoint);
		pDC->LineTo(point);
		endPoint = point;
	}
	

	CView::OnMouseMove(nFlags, point);
}
