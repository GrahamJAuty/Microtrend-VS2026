#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "afxwin.h"
//$$******************************************************************

class CSimulateReaderDlg : public CSSAutoShutdownDialog
{
public:
	CSimulateReaderDlg( CWnd* pParent = NULL);
	virtual ~CSimulateReaderDlg() {};
	
	enum { IDD = IDD_SIMULATE_READER };
	CEdit m_editMiFareNo;
	CString m_strInputMiFareNo;
	
private:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnButtonAccept();
	afx_msg void OnButtonError();
	afx_msg void OnButtonTerminate();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();
	
public:
	int m_nResult;
};

//$$******************************************************************

