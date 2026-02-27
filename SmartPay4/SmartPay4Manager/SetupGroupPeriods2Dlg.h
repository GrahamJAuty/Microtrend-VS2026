#pragma once
//**********************************************************************
#include "resource.h"
//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
//**********************************************************************

class CSetupGroupPeriods2Dlg : public CSSAutoShutdownDialog
{
public:
	CSetupGroupPeriods2Dlg ( CSQLRowGroupFull& RowGroup, CWnd* pParent = NULL);   // standard constructor
	
	enum { IDD = IDD_SETUP_GROUPPERIOD2 };
	CString	m_strPeriodText1;
	CString	m_strPeriodText2;
	CString	m_strPeriodText3;
	CString	m_strPeriodText4;
	CString	m_strPeriodText5;
	CString	m_strPeriodText6;
	CString m_strStartPeriod1;
	CString m_strStartPeriod2;
	CString m_strStartPeriod3;
	CString m_strStartPeriod4;
	CString m_strStartPeriod5;
	CString m_strStartPeriod6;

	BOOL m_bPeriod1Purse2;
	BOOL m_bPeriod2Purse2;
	BOOL m_bPeriod3Purse2;
	BOOL m_bPeriod4Purse2;
	BOOL m_bPeriod5Purse2;
	BOOL m_bPeriod6Purse2;

	CButton m_checkPeriod1Purse2;
	CButton m_checkPeriod2Purse2;
	CButton m_checkPeriod3Purse2;
	CButton m_checkPeriod4Purse2;
	CButton m_checkPeriod5Purse2;
	CButton m_checkPeriod6Purse2;

	CSSComboBox m_comboPeriod1Purse2;
	CSSComboBox m_comboPeriod2Purse2;
	CSSComboBox m_comboPeriod3Purse2;
	CSSComboBox m_comboPeriod4Purse2;
	CSSComboBox m_comboPeriod5Purse2;
	CSSComboBox m_comboPeriod6Purse2;
	int m_nPeriod1Purse2;
	int m_nPeriod2Purse2;
	int m_nPeriod3Purse2;
	int m_nPeriod4Purse2;
	int m_nPeriod5Purse2;
	int m_nPeriod6Purse2;
	double m_dRefreshPeriod1;
	double m_dRefreshPeriod2;
	double m_dRefreshPeriod3;
	double m_dRefreshPeriod4;
	double m_dRefreshPeriod5;
	double m_dRefreshPeriod6;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditRefresh1() { return GetEdit(IDC_EDIT_REFRESH1); }
	CEdit* GetEditRefresh2() { return GetEdit(IDC_EDIT_REFRESH2); }
	CEdit* GetEditRefresh3() { return GetEdit(IDC_EDIT_REFRESH3); }
	CEdit* GetEditRefresh4() { return GetEdit(IDC_EDIT_REFRESH4); }
	CEdit* GetEditRefresh5() { return GetEdit(IDC_EDIT_REFRESH5); }
	CEdit* GetEditRefresh6() { return GetEdit(IDC_EDIT_REFRESH6); }

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void SetDisplay();
	CString CheckPeriodRefresh();

private:
	CSQLRowGroupFull& m_RowGroup;
	CString m_strTitle;
};

//**********************************************************************
