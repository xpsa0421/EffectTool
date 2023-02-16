
// EffectToolApplicationView.cpp : implementation of the CEffectToolApplicationView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "EffectToolApplication.h"
#endif

#include "EffectToolApplicationDoc.h"
#include "EffectToolApplicationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEffectToolApplicationView

IMPLEMENT_DYNCREATE(CEffectToolApplicationView, CView)

BEGIN_MESSAGE_MAP(CEffectToolApplicationView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEffectToolApplicationView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CEffectToolApplicationView construction/destruction

CEffectToolApplicationView::CEffectToolApplicationView() noexcept
{
	// TODO: add construction code here

}

CEffectToolApplicationView::~CEffectToolApplicationView()
{
}

BOOL CEffectToolApplicationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CEffectToolApplicationView drawing

void CEffectToolApplicationView::OnDraw(CDC* /*pDC*/)
{
	CEffectToolApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CEffectToolApplicationView printing


void CEffectToolApplicationView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CEffectToolApplicationView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CEffectToolApplicationView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEffectToolApplicationView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CEffectToolApplicationView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CEffectToolApplicationView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CEffectToolApplicationView diagnostics

#ifdef _DEBUG
void CEffectToolApplicationView::AssertValid() const
{
	CView::AssertValid();
}

void CEffectToolApplicationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEffectToolApplicationDoc* CEffectToolApplicationView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEffectToolApplicationDoc)));
	return (CEffectToolApplicationDoc*)m_pDocument;
}
#endif //_DEBUG


// CEffectToolApplicationView message handlers


void CEffectToolApplicationView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CEffectToolApplicationApp* app = (CEffectToolApplicationApp*)AfxGetApp();
	app->effect_tool_.ResizeWindow(cx, cy);
	// TODO: Add your message handler code here
}
