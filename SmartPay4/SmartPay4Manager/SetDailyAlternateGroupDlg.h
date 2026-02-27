#pragma once
//$$******************************************************************
#include "SmartPay4Manager.h"
//$$******************************************************************

class CSetDailyAlternateGroupDlg : public CSSAutoShutdownDialog
{
public:
	CSetDailyAlternateGroupDlg(CWnd* pParent = NULL);   
	virtual ~CSetDailyAlternateGroupDlg();

	enum { IDD = IDD_SETDAYGROUPS };
	int m_nGroupDayNumber[7];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	
	DECLARE_MESSAGE_MAP()
};

//$$******************************************************************
