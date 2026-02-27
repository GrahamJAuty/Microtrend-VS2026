#pragma once
//*******************************************************
#include "..\SmartPay4Shared\BackgroundOptions.h"
#include "..\SmartPay4Shared\EmailAlertData.h"
//*******************************************************
#include "SmartPay4Manager.h"
//*******************************************************

class CSetupBackgroundOptionsDlg : public CSSAutoShutdownDialog
{
public:
	CSetupBackgroundOptionsDlg(CWnd* pParent = NULL);   
	
// Dialog Data
	enum { IDD = IDD_SETUP_BGNDOPTIONS };

	CButton m_checkEODExport;
	CButton m_checkEmailLowBalance;
	CButton m_checkEmailPurchases;
	CButton m_checkEmailPointsAchievement;
	CButton m_checkEmailWeeklyPoints;
	CButton m_checkMonday;
	CButton m_checkTuesday;
	CButton m_checkWednesday;
	CButton m_checkThursday;
	CButton m_checkFriday;
	CButton m_checkSaturday;
	CButton m_checkSunday;
	CButton m_checkWebPaymentSuspendOnError;
	CEdit m_editDate5;
	CEdit m_editDate6;
	CEdit m_editDate7;
	CEdit m_editDate10;
	CEdit m_editDate11;
	CEdit m_editInterval;

	int m_nWebPaymentMinutes;
	int m_nNewRecordsMinutes;
	int m_nExportSalesMinutes;
	BOOL m_bLogActions;
	CDateTimeCtrl	m_dtcTime3;
	CDateTimeCtrl	m_dtcTime4;
	CDateTimeCtrl	m_dtcTime5;
	CDateTimeCtrl	m_dtcTime6;
	CDateTimeCtrl	m_dtcTime7;
	CDateTimeCtrl	m_dtcTime8;
	CDateTimeCtrl	m_dtcTime9;
	CDateTimeCtrl	m_dtcTime10;
	CDateTimeCtrl	m_dtcTime11;
	CButton m_buttonAltKey1;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	afx_msg void SetDisplay();
	afx_msg void OnButtonAltKey1();
	DECLARE_MESSAGE_MAP()

private:
	virtual void CSSAutoShutdownPreInitDialog();
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	CBackgroundOptions m_options;
	CEmailAlertData m_emailAlert;

	COleDateTime m_timeTime3;
	COleDateTime m_timeTime4;
	COleDateTime m_timeTime5;
	COleDateTime m_timeTime6;
	COleDateTime m_timeTime7;
	COleDateTime m_timeTime8;
	COleDateTime m_timeTime9;
	COleDateTime m_timeTime10;
	COleDateTime m_timeTime11;

	CString m_strDateEOD;
	CString m_strDateLowBalance;
	CString m_strDatePurchases;
	CString m_strDatePointsAchievement;
	CString m_strDateWeeklyPoints;
};

//*******************************************************
