#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CProcessedOrderExceptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(CProcessedOrderExceptionsDlg)

public:
	CProcessedOrderExceptionsDlg( const char* szTitle, CWnd* pParent = NULL); 
	virtual ~CProcessedOrderExceptionsDlg();

// Dialog Data
	enum { IDD = IDD_ORDER_EXCEPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();

public:
	afx_msg void OnBnClickedButtonExceptions();
	afx_msg void OnBnClickedButtonProceed();
	virtual BOOL OnInitDialog();

private:
	CStatic m_staticOrder;
	CString m_strTitle;
};

/**********************************************************************/
#endif
/**********************************************************************/
