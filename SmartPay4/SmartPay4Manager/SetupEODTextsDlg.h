#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\EODTextData.h"
//$$******************************************************************
#include "SmartPay4Manager.h"
//$$******************************************************************

class CSetupEODTextsDlg : public CSSAutoShutdownDialog
{
public:
	CSetupEODTextsDlg(CWnd* pParent = NULL);  
	virtual ~CSetupEODTextsDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_EODTEXTS };
	CString m_strPosTopup = "";
	CString m_strPosTopupRefund = "";
	CString m_strOfficeTopup = "";
	CString m_strOfficeTopupRefund = "";
	CString m_strImportTopup = "";
	CString m_strImportTopupRefund = "";
	CString m_strCreditAdjustUp = "";
	CString m_strCreditAdjustDown = "";
	CString m_strOfficeAdjustment = "";
	CString m_strKioskTopup = "";
	CString m_strWebpaymentTopup = "";
	CString m_strWebpaymentRefund = "";
	CString m_strPosSaleRefund = "";
	CString m_strOfficeSaleAdjustment = "";
	CString m_strOfficeDeletion = "";
	CString m_strPurse1Text = "";
	CString m_strPurse2Text = "";
	CString m_strPurse3Text = "";
	CString m_strCashText = "";
	CString m_strPebbleOrderText = "";
	CString m_strPebbleOrderRefundText = "";
	BOOL m_bZeroSkip = FALSE;
	BOOL m_bZeroSkipVAT = FALSE;
	BOOL m_bExcludePeriodSales = FALSE;
	BOOL m_bExcludeEposPayments = FALSE;
	BOOL m_bExcludeSalesBreakdown = FALSE;
	CString m_strOfficeMeal = FALSE;
	int m_nSaveAsType = 0;
	CSSComboBox m_comboOutput;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual void OnOK();
	afx_msg void OnButtonSet();
	DECLARE_MESSAGE_MAP()

private:
	void CSSAutoShutdownPreInitDialog();
	BOOL CSSAutoShutdownPostInitDialog();

private:
	CEODTextData m_texts;	
};

//$$******************************************************************
