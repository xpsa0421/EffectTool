
// EffectToolApplicationView.h : interface of the CEffectToolApplicationView class
//

#pragma once


class CEffectToolApplicationView : public CView
{
protected: // create from serialization only
	CEffectToolApplicationView() noexcept;
	DECLARE_DYNCREATE(CEffectToolApplicationView)

// Attributes
public:
	CEffectToolApplicationDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CEffectToolApplicationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // debug version in EffectToolApplicationView.cpp
inline CEffectToolApplicationDoc* CEffectToolApplicationView::GetDocument() const
   { return reinterpret_cast<CEffectToolApplicationDoc*>(m_pDocument); }
#endif

