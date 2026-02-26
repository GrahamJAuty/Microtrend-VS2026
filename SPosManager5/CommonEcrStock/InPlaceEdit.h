#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/

class CInPlaceEdit : public CEditFilter
{
private:
	CInPlaceEdit();
	virtual ~CInPlaceEdit();

public:
	static CInPlaceEdit* GetInstance(); 
	static void DeleteInstance(); 

	BOOL ShowEditCtrl(DWORD dwStyle, const RECT& rCellRect, CWnd* pParentWnd, 
					  UINT uiResourceID, int iRowIndex, int iColumnIndex,
					  CString& rstrCurSelection);

	//{{AFX_VIRTUAL(CInPlaceEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:	
	//{{AFX_MSG(CInPlaceEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	DECLARE_MESSAGE_MAP()

private:
	int m_iRowIndex;
	int m_iColumnIndex;
	BOOL m_bESC;
	
private:
	static CInPlaceEdit* m_pInPlaceEdit;
	CString m_strWindowText;
};

/**********************************************************************/

class CInPlaceEditText : public CEdit
{
private:
	CInPlaceEditText();
	virtual ~CInPlaceEditText();

public:
	static CInPlaceEditText* GetInstance(); 
	static void DeleteInstance(); 

	BOOL ShowEditCtrl(DWORD dwStyle, const RECT& rCellRect, CWnd* pParentWnd, 
					  UINT uiResourceID, int iRowIndex, int iColumnIndex,
					  CString& rstrCurSelection);

	//{{AFX_VIRTUAL(CInPlaceEditText)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:	
	//{{AFX_MSG(CInPlaceEditText)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	DECLARE_MESSAGE_MAP()

private:
	int m_iRowIndex;
	int m_iColumnIndex;
	BOOL m_bESC;
	
private:
	static CInPlaceEditText* m_pInPlaceEdit;
	CString m_strWindowText;
};

/**********************************************************************/
#endif
/**********************************************************************/
