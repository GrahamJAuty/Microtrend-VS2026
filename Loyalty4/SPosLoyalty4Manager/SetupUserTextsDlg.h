#pragma once
//$$******************************************************************
#include "..\SPosLoyalty4Shared\UserTexts.h"
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CSetupUserTextsDlg : public CSSDialog
{
public:
	CSetupUserTextsDlg( CWnd* pParent = NULL);   
	virtual ~CSetupUserTextsDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_USERTEXTS };
	CStatic m_staticExtRef;
	CEdit m_editExtRef;
	CEdit m_editAdd[5];
	CEdit m_editTel[2];
	CEdit m_editInfo[4];
	CEdit m_editDob;
	CButton m_checkAddCompulsory[5];
	CButton m_checkTelCompulsory[2];
	CButton m_checkInfoCompulsory[4];
	CButton m_checkInfoDate[4];
	CButton m_checkDobCompulsory;
	CButton m_checkForenameCompulsory;
	CButton m_checkSurnameCompulsory;
	CButton m_checkUsernameCompulsory;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonDefault();
	DECLARE_MESSAGE_MAP()

private:
	void SaveText(CUserTexts& texts, CEdit& edit, int nField);

private:
	void DeleteFile();
};

//$$******************************************************************
