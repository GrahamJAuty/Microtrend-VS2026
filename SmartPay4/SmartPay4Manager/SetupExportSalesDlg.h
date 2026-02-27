#pragma once
//**********************************************************************
#include "..\SmartPay4ManagerBgnd\DbReportHandler.h"
#include "..\SmartPay4ManagerBgnd\SalesExportData.h"
//**********************************************************************
#include "SmartPay4Manager.h"
//**********************************************************************

class CSetupExportSalesDlg : public CSSAutoShutdownDialog
{
public:
	CSetupExportSalesDlg ( CWnd* pParent = NULL);
	
	enum { IDD = IDD_SETUP_EXPORTSALES };
	CListBox m_listboxView1;
	CButton m_buttonFields;
	CButton m_buttonView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnButtonLog();
	afx_msg void OnButtonView();
	afx_msg void OnButtonFields();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void DisplayFieldList ( const char* szHeader );

private:
	CWnd* m_pParent;
	CDbReportHandler m_dbReporter;
	CString m_strReportLabel;

	CSalesExportData m_data;
};

//**************************************************************************
