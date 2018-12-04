
// GraphicsBSplineCurveView.h: CGraphicsBSplineCurveView 类的接口
//

#pragma once
#include "math.h"
#include "stdafx.h"
#include "MapObj.h"
#include "Point3D.h"

class CGraphicsBSplineCurveView : public CView
{
protected: // 仅从序列化创建
	CGraphicsBSplineCurveView() noexcept;
	DECLARE_DYNCREATE(CGraphicsBSplineCurveView)

// 特性
public:
	CGraphicsBSplineCurveDoc* GetDocument() const;





// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CGraphicsBSplineCurveView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()



//属性
public:
	int type;//ToolBar菜单按钮类型
	CPoint startPoint;//画一段折线的起始点
	CPoint endPoint;//画一段折线的终止点
	bool boolLButtonDown;//标记鼠标左键是否按下
	int N; //B样条曲线次数
	int K; //B样条曲线阶数
	int nPoints; //划分点个数
	double mPM[4][4];//投影变换矩阵
	CArray<CPoint, CPoint> pointList;//折线顶点列表
	CArray<CPoint, CPoint> pointPrintList;	//画点2D数组
	CArray<CPoint, CPoint> pointPrint3DList;	//画3D点数组

// 操作
public:
	void Draw3ColorBSpline();
	void From3DTo2D(Point3D* c, int n);
	void BSplineToPoints();
	CPoint BSpline(double knot[], int num, double u, int t);
	double Base(int i, int k, double knot[], int num, double u);
	afx_msg void OnBspline();
	afx_msg void OnRotate();
	afx_msg void OnSymmetry();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // GraphicsBSplineCurveView.cpp 中的调试版本
inline CGraphicsBSplineCurveDoc* CGraphicsBSplineCurveView::GetDocument() const
   { return reinterpret_cast<CGraphicsBSplineCurveDoc*>(m_pDocument); }
#endif

