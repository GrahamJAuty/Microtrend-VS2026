#pragma once
//**********************************************************************
#include "resource.h"
//**********************************************************************

class CRefreshWeeklyDlg : public CSSAutoShutdownDialog
{
public:
	CRefreshWeeklyDlg(CWnd* pParent = NULL); 
	virtual ~CRefreshWeeklyDlg();

	enum { IDD = IDD_REFRESHWEEKLY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

public:
	int m_nRefreshDay;
};

//**********************************************************************
