#pragma once
//*********************************************************************
#include "Cardlink.h"
#include "afxdtctl.h"
#include "ServiceTimeData.h"
//*********************************************************************

class CServiceTimesDlg : public CDialog
{
	DECLARE_DYNAMIC(CServiceTimesDlg)

public:
	CServiceTimesDlg(CWnd* pParent); 
	
	enum { IDD = IDD_SERVICETIMESDLG };
	BOOL m_bInService1 = FALSE;
	BOOL m_bInService2 = FALSE;
	BOOL m_bInService3 = FALSE;
	BOOL m_bInService4 = FALSE;
	BOOL m_bInService5 = FALSE;
	BOOL m_bBalanceOnly1 = FALSE;
	BOOL m_bBalanceOnly2 = FALSE;
	BOOL m_bBalanceOnly3 = FALSE;
	BOOL m_bBalanceOnly4 = FALSE;
	BOOL m_bBalanceOnly5 = FALSE;
	CButton m_checkBalanceOnly1;
	CButton m_checkBalanceOnly2;
	CButton m_checkBalanceOnly3;
	CButton m_checkBalanceOnly4;
	CButton m_checkBalanceOnly5;
	CDateTimeCtrl m_dtcStart1;
	CDateTimeCtrl m_dtcEnd1;
	CDateTimeCtrl m_dtcStart2;
	CDateTimeCtrl m_dtcEnd2;
	CDateTimeCtrl m_dtcStart3;
	CDateTimeCtrl m_dtcEnd3;
	CDateTimeCtrl m_dtcStart4;
	CDateTimeCtrl m_dtcEnd4;
	CDateTimeCtrl m_dtcStart5;
	CDateTimeCtrl m_dtcEnd5;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEnableService1();
	afx_msg void OnButtonEnableService2();
	afx_msg void OnButtonEnableService3();
	afx_msg void OnButtonEnableService4();
	afx_msg void OnButtonEnableService5();	
	virtual void OnOK();

private:
	void SetDisplay();

private:
	CServiceTimeData m_ServiceTimes;
};

//*********************************************************************