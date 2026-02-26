//$$******************************************************************
#pragma once
//$$******************************************************************
#include "SharedResources.h"
//$$******************************************************************
#include "MiFareTerminalData.h"
#include "MyMiFare.h"
//$$******************************************************************

class CSetupReaderDlg : public CSSDialog
{
public:
	CSetupReaderDlg(CWnd* pParent = NULL);
	
	enum { IDD = IDD_SETUP_READER };
	CString m_strPrompt;
	CStatic m_staticPrompt;
	CSSComboBox m_comboReader;
	CSSComboBox m_comboPort;
	CSSComboBox m_comboBaud;
	CSSComboBox m_comboExtRef1;
	CSSComboBox m_comboExtRef2;
	CSSComboBox m_comboFormat1;
	CSSComboBox m_comboFormat2;
	CButton m_buttonTest;
	CButton m_buttonTest2;
	CStatic m_staticUIDType;
	CButton m_buttonOK;
	CString m_strReader;
	
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnButtonTest();
	afx_msg void OnButtonTest2();
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()

private:
	void TestReader ( int nFormat, int nDataFormat );

private:
	CMyMifare m_reader;
	CMiFareTerminalData m_terminal;
	bool m_bTestMode;
};

//$$******************************************************************
