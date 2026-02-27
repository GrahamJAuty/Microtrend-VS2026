#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\WisePayData.h"
//*******************************************************************

class CAccountLinkWisePayDlg : public CSSAutoShutdownDialog
{
public:
	CAccountLinkWisePayDlg( CWnd* pParent = NULL);   // standard constructor
	virtual ~CAccountLinkWisePayDlg();

// Dialog Data
	enum { IDD = IDD_ACCOUNTLINK_WISEPAY };
	CButton m_buttonStart;
	CButton m_buttonRaw;
	CButton m_buttonDisplay;
	CButton m_buttonImport;
	CButton m_buttonBalances;
	CButton m_buttonAltKey1;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL DestroyWindow();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonRaw();
	afx_msg void OnButtonDisplay();
	afx_msg void OnButtonImport();
	afx_msg void OnButtonBalances();
	afx_msg void OnButtonAltKey1();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void SetDisplay();
	CString AccessWisePay();
	CString ImportWisePay();

private:
	CWnd* m_pParent;
	CWisePayData m_dataWisePay;

private:
	CString m_strError;
	CString m_strImportFilename;
	CString m_strLoginReply;
	CString m_strBalanceListFile;
};

//*******************************************************************
