#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CProcessedOrderAutoPrintDlg : public CDialog
{
public:
	CProcessedOrderAutoPrintDlg( const char* szPreviousOrderFilename, const char* szPreviousOrderReference, CWnd* pParent = NULL);   

	//{{AFX_DATA(CProcessedOrderAutoPrintDlg)
	enum { IDD = IDD_ORDER_AUTOPRINT };
	BOOL	m_bGenerate;
	BOOL	m_bPurchase;
	BOOL	m_bReports;
	BOOL	m_bEmail;
	CButton m_checkGenerate;
	CButton m_checkPurchase;
	CButton m_checkReports;
	CButton m_checkEmail;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CProcessedOrderAutoPrintDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CProcessedOrderAutoPrintDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CButton m_radioOne;
	CButton m_radioTwo;
	CButton m_radioThree;
	CEdit m_editReference;

private:
	CString m_strPreviousOrderFilename;
	CString m_strPreviousOrderReference;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedGenerate();
	afx_msg void OnBnClickedRadioOne();
	afx_msg void OnBnClickedRadioTwo();
	afx_msg void OnBnClickedRadioThree();
	
public:
	CString m_strReference;
	int m_nAction;
};

/**********************************************************************/
#endif
/**********************************************************************/

