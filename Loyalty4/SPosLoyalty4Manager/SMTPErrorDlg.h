#pragma once
/**********************************************************************/
#include "resource.h"
/**********************************************************************/
#include "SMTPEmailHelpers.h"
/**********************************************************************/

class CSMTPErrorDlg : public CDialog
{
	DECLARE_DYNAMIC(CSMTPErrorDlg)

public:
	CSMTPErrorDlg( int nSessionType, CSMTPEmailHelpers& SMTPHelpers, int nIndex, CWnd* pParent = NULL ); 
	virtual ~CSMTPErrorDlg();

	enum { IDD = IDD_SMTP_ERROR };
	CSSListCtrlVirtual m_listResults;
	CEdit m_editError;

public:
	bool GotErrorFile(){ return m_bGotErrorFile; }
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

private:
	bool m_bGotErrorFile;
	CString m_strErrorFileName;
	
private:
	CSMTPEmailHelpers& m_SMTPHelpers;
	int m_nSessionType;
	int m_nIndex;
};

/**********************************************************************/

