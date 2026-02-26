#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "AlertText.h"
//$$******************************************************************

class CSetupAlertTextsDlg : public CSSDialog
{
public:
	CSetupAlertTextsDlg(CWnd* pParent = NULL);  
	virtual ~CSetupAlertTextsDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_ALERTTEXTS };
	CEdit m_editText[9];
	CButton m_checkEPOS[9];
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedButtonClear();

	DECLARE_MESSAGE_MAP()

private:
	CAlertText m_alertText;
};

//$$******************************************************************
