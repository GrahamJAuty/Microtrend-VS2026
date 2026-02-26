#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CServerPropertyPage1 : public CSSPropertyPage
{
public:
	CServerPropertyPage1();
	virtual ~CServerPropertyPage1();

	enum { IDD = IDD_PROPPAGE_SERVER1 };
	CButton m_checkLogAction;
	CEdit m_editLoyReqPath;
	CButton m_checkAllowOverSpend;
	CButton m_checkRealTimePlus;
	/*****/
	CEdit m_editLoyLogPath;
	CButton m_checkLoyLogNewBalance;
	CButton m_checkLogSave;
	CButton m_checkLogSave2;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	/*****/
	afx_msg void OnButtonFolders();
	/*****/
	afx_msg void OnButtonLoyReqBrowse();
	afx_msg void OnButtonLoyReqTest();
	afx_msg void OnToggleLogSave();
	afx_msg void OnClickStaticLogSave2();
	/*****/
	afx_msg void OnButtonLoyLogBrowse();
	afx_msg void OnButtonLoyLogTest();
	
	DECLARE_MESSAGE_MAP()
};

//$$******************************************************************

