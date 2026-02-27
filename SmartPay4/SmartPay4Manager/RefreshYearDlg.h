#pragma once

#include "resource.h"

// CRefreshYearDlg dialog

class CRefreshYearDlg : public CSSAutoShutdownDialog
{
public:
	CRefreshYearDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRefreshYearDlg();

// Dialog Data
	enum { IDD = IDD_REFRESHYEAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	COleDateTime m_oleDateTime;
};
