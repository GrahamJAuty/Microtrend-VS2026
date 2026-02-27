//$$******************************************************************
#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "SetupAuditPeriodsDlg.h"
//$$******************************************************************

class CSystemPropertyPage2 : public CSSAutoShutdownPropertyPage
{
public:
	CSystemPropertyPage2();
	bool RestartServerReqd() { return m_bRestartServerReqd; }

	enum { IDD = IDD_PROPPAGE_SYSTEM2 };
	CSSComboBox m_comboStartofWeek;
	CSSComboBox m_comboFieldNames;
	CButton m_checkEnablePhotoID;
	CButton m_buttonPhotoID;
	CButton m_checkLeavers;
	CButton m_checkPurchaseTexts;
	CEdit m_editEndText;
	/*****/
	CEdit m_editEcrmanPath;
	CEdit m_editPrinter;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnButtonFieldNames();
	afx_msg void OnTogglePhotoId();
	afx_msg void OnButtonPhotoId();
	afx_msg void OnButtonEmailFunctions();
	/*****/
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonTest();
	/*****/
	afx_msg void OnButtonPrinter();

public:
	void ShowPrinterList( CSSPropertyPage* pParentWnd, CEdit& EditPrinter, CString& strDefaultPrinter, const char* szAction );
	void PrinterListError( int n );
	
private:
	void SetDisplay();

private:
	CString m_strSystemDefaultPrinter;
	bool m_bRestartServerReqd;
};

//$$******************************************************************
