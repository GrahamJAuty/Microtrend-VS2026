#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "MyMifare.h"
#include "TerminalData.h"
//$$******************************************************************

class CPasswordDlg : public CSSDialog
{
public:
	CPasswordDlg(CWnd* pParent = NULL);   
	virtual ~CPasswordDlg();

// Dialog Data
	enum { IDD = IDD_PASSWORD };
	CEdit m_editPassword;
	CButton m_buttonCard;
	CStatic m_staticPrompt;
	CButton m_buttonOK;
	bool m_bReadCardAllowed;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonReadCard();
	virtual void OnCancel();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

public:
	CString m_strPassword;
	CString m_strCardNo;

private:
	CMyMifare m_reader;
	CTerminalData m_terminal;
	bool m_bTestMode;
};

//$$******************************************************************
