#pragma once


// CRefreshWeeklyDlg dialog

class CRefreshWeeklyDlg : public CDialog
{
public:
	CRefreshWeeklyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRefreshWeeklyDlg();

// Dialog Data
	enum { IDD = IDD_REFRESHWEEKLY };
	int m_nRefreshDay;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
