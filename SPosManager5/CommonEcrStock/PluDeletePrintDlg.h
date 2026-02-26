#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPluDeletePrintDlg : public CSSDialog
{
public:
	CPluDeletePrintDlg( bool bDeleteItems, CWnd* pParent = NULL);   

	//{{AFX_DATA(CPluDeletePrintDlg)
	enum { IDD = IDD_PLU_DELETE_PRINT };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluDeletePrintDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL
	afx_msg void OnButtonSave();

public:
	bool m_bLines;
	bool m_bMarked;
	bool m_bStockCode;
	bool m_bPrice;
	bool m_bEmpty;

public:
	CButton m_checkLines;
	CButton m_checkMarked;
	CButton m_checkStockCode;
	CButton m_checkPrice;
	CButton m_checkEmpty;
	CButton m_buttonSave;
	CButton m_buttonOK;
	CButton m_buttonCancel;

protected:
	//{{AFX_MSG(CPluDeletePrintDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool m_bDeleteItems;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
#endif
/**********************************************************************/
