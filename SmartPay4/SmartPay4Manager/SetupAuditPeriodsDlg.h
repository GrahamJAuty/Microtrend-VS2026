#pragma once
//**********************************************************************
#include "resource.h"
//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowAuditPeriod.h"
//**********************************************************************

class CSetupAuditPeriodsDlg : public CSSAutoShutdownDialog
{
public:
	CSetupAuditPeriodsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetupAuditPeriodsDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_PERIODS };
	CDateTimeCtrl m_dtcStartPeriod[6];
	CEdit m_editPeriodText[6];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();
};

//**********************************************************************
