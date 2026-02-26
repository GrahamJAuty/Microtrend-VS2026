#pragma once
/**********************************************************************/
#include "..\CommonEcrStock\ButtonEditDlg.h"
#include "..\CommonEcrStock\CommonDoc.h"
/**********************************************************************/

class CStockmanView : public CView
{
protected: 
	CStockmanView();
	DECLARE_DYNCREATE(CStockmanView)

public:
	CCommonDoc* GetDocument();

public:
	//{{AFX_VIRTUAL(CStockmanView)
	public:
	virtual void OnDraw(CDC* pDC);  
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

public:
	virtual ~CStockmanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//{{AFX_MSG(CStockmanView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonBarOption1();
	afx_msg void OnButtonBarOption2();
	afx_msg void OnButtonBarOption3();
	afx_msg void OnButtonBarOption4();
	afx_msg void OnButtonBarOption5();
	afx_msg void OnButtonBarOption6();
	afx_msg void OnButtonBarOption7();
	afx_msg void OnButtonBarOption8();
	afx_msg void OnButtonBarOption9();
	afx_msg void OnButtonBarOption10();
	afx_msg void OnButtonBarOption11();
	afx_msg void OnButtonBarOption12();
	afx_msg void OnButtonBarOption13();
	afx_msg void OnButtonBarOption14();
	afx_msg void OnButtonBarOption15();
	afx_msg void OnButtonBarOption16();
	afx_msg void OnButtonBarOption17();
	afx_msg void OnButtonBarOption18();
	afx_msg void OnButtonBarOption19();
	afx_msg void OnButtonBarOption20();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void ContextOption( int n );

private:
	CBrush m_KeyBrush;
};

#ifndef _DEBUG  // debug version in StockmanView.cpp
inline CCommonDoc* CStockmanView::GetDocument()
   { return (CCommonDoc*)m_pDocument; }
#endif

/**********************************************************************/
