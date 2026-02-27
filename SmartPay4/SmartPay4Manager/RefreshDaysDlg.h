#pragma once
//$$******************************************************************

class CRefreshDaysDlg : public CSSAutoShutdownDialog
{
public:
	CRefreshDaysDlg(CWnd* pParent = NULL);   
	virtual ~CRefreshDaysDlg();

// Dialog Data
	enum { IDD = IDD_REFRESHDAYS };
	CEdit m_editGroupDayNumber[7];
	BOOL m_bDayFlag[7];
	int m_nGroupDayNumber[7];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	afx_msg void OnButton5days();
	afx_msg void OnButton7days();
	afx_msg void OnButtonClear();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	bool HaveDaysSet();

public:
	bool m_bGroupsAllowed;
};

//$$******************************************************************
