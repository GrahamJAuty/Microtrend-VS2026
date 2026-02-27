#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CSystemPropertyPage5 : public CSSAutoShutdownPropertyPage
{
public:
	CSystemPropertyPage5();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_SYSTEM5 };
	CButton m_checkExportExit;
	CButton m_buttonSetupExit1;
	CButton m_buttonSetupExit2;
	CButton m_buttonSetupExit3;
	/*****/
	CButton m_checkExportDelete;
	CButton m_buttonSetupDelete;
	/*****/
	CButton m_checkExportSales;
	CButton m_buttonSetupSales;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	/*****/
	afx_msg void OnToggleExportExit();
	afx_msg void OnButtonSetupExit1();
	afx_msg void OnButtonSetupExit2();
	afx_msg void OnButtonSetupExit3();
	/*****/
	afx_msg void OnToggleExportDelete();
	afx_msg void OnButtonSetupDelete();
	/*****/
	afx_msg void OnToggleExportSales();
	afx_msg void OnButtonSetupSales();
	
	DECLARE_MESSAGE_MAP()

private:
	void SetDisplay();
	void SetButtonTexts();
	void EditLabel ( const char* szLabel );
};

//$$******************************************************************
