//$$******************************************************************
#pragma once
//$$******************************************************************
#include "..\SPosLoyalty4Shared\UserTexts.h"
//$$******************************************************************
#include "resource.h"
#include "MyMifare.h"
#include "TerminalData.h"
//$$******************************************************************

class CSetupReaderDlg : public CSSDialog
{
public:
	CSetupReaderDlg(CWnd* pParent = NULL);
	virtual ~CSetupReaderDlg();

	enum { IDD = IDD_SETUP_READ };
	CStatic m_staticPrompt;
	CStatic m_staticUIDType;
	CSSComboBox m_comboReader;
	CSSComboBox m_comboPort;
	CSSComboBox m_comboBaud;
	CSSComboBox m_comboExtRef1;
	CSSComboBox m_comboFormat1;
	CButton m_buttonTest;
	CButton m_buttonOK;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonTest();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

public:
	int GetChangedPaxtonMode(){ return 0; }

private:
	void TestReader ( int nFormat, int nDataFormat );

private:
	CMyMifare m_reader;
	CTerminalData m_terminal;
	CUserTexts m_userTexts;
	int m_nOriginalDataFormat;
	bool m_bTestMode;
	int m_nChangedPaxtonMode;
};

//$$******************************************************************
