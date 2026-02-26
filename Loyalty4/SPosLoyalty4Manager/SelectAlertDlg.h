#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "AlertText.h"
//$$******************************************************************

class CSelectAlertDlg : public CSSDialog
{
public:
	CSelectAlertDlg(CWnd* pParent = NULL);   
	virtual ~CSelectAlertDlg();

	enum { IDD = IDD_SELECT_ALERT };
	CButton m_checkAlert[9];
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonNone();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

public:
	CString m_strAlertCodes;

private:
	CAlertText m_alertText;
};

//$$******************************************************************

