#pragma once
//********************************************************************
#include "resource.h"
//********************************************************************
#include "..\SmartPay4Shared\TNoDatabase.h"
//********************************************************************

class CTerminalSetsDlg : public CSSAutoShutdownDialog
{
public:
	CTerminalSetsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_TERMINALSETS };
	CEdit m_editList2;
	CEdit m_editList3;
	CEdit m_editList4;
	CString m_strText1;
	CString m_strList1;
	CString m_strText2;
	CString m_strList2;
	CString m_strText3;
	CString m_strList3;
	CString m_strText4;
	CString m_strList4;

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	afx_msg void OnButtonClear();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

private:
	CString ValidateLists();
	CString ValidateTerminals ( CTNoDatabase* db, int nSetNo, const char* szList );	
};

//********************************************************************
