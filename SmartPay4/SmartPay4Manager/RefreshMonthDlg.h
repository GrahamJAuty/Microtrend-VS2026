#pragma once

#include "resource.h"

// CRefreshMonthDlg dialog

class CRefreshMonthDlg : public CSSAutoShutdownDialog
{
public:
	CRefreshMonthDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRefreshMonthDlg();

// Dialog Data
	enum { IDD = IDD_REFRESHMONTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	int m_nDayNo;
public:
	afx_msg void OnDeltaposSpinDayno(NMHDR *pNMHDR, LRESULT *pResult);
};
