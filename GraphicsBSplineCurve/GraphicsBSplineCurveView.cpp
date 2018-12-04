
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
END_MESSAGE_MAP()

// CGraphicsBSplineCurveView 构造/析构

CGraphicsBSplineCurveView::CGraphicsBSplineCurveView() noexcept
{
	// TODO: 在此处添加构造代码

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

void CGraphicsBSplineCurveView::OnDraw(CDC* /*pDC*/)
{
	CGraphicsBSplineCurveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
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
