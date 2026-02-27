#pragma once
//*******************************************************************
#include "Cardlink.h"
#include "afxwin.h"
#include "MyMifare.h"
#include "TerminalData.h"
//*******************************************************************

class CSetupReaderDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupReaderDlg)

public:
	CSetupReaderDlg(CWnd* pParent);
	virtual ~CSetupReaderDlg();

// Dialog Data
	enum { IDD = IDD_SETUPREADERDLG };
	CString m_strPrompt = "";
	CStatic m_staticPrompt;
	CButton m_buttonOK;
	CComboBox m_comboReader;
	CComboBox m_comboPort;
	CComboBox m_comboBaud;
	CString m_strReader = "";
	CString m_strPort = "";
	CString m_strBaud = "";
	int m_nUIDFormat = 0;
	int m_nDataFormat = 0;

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonTest();
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()

public:
	int m_nLocateOn = 0;

private:
	CMyMifare m_reader;
	CTerminalData m_terminal;
	bool m_bTestMode = FALSE;
};

//*******************************************************************
