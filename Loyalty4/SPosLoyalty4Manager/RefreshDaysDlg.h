#pragma once


// CRefreshDaysDlg dialog

class CRefreshDaysDlg : public CDialog
{
public:
	CRefreshDaysDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRefreshDaysDlg();

// Dialog Data
	enum { IDD = IDD_REFRESHDAYS };
	BOOL m_bDay1;
	BOOL m_bDay2;
	BOOL m_bDay3;
	BOOL m_bDay4;
	BOOL m_bDay5;
	BOOL m_bDay6;
	BOOL m_bDay7;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedButton5days();
	afx_msg void OnBnClickedButton7days();
	afx_msg void OnBnClickedButtonClear();

	DECLARE_MESSAGE_MAP()

private:
	void DisplayRefreshDays();
	void SetAllDays ( bool bReqd );
};
