#pragma once


// CRefreshYearlyDlg dialog

class CRefreshYearlyDlg : public CDialog
{
public:
	CRefreshYearlyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRefreshYearlyDlg();

// Dialog Data
	enum { IDD = IDD_REFRESHYEARLY };
	COleDateTime m_oleDateTime;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
