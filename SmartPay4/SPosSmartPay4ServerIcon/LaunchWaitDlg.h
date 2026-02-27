#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CLaunchWaitDlg : public CSSDialog
{
public:
	CLaunchWaitDlg( CWnd* pParent = NULL); 
	~CLaunchWaitDlg();
	
	enum { IDD = IDD_LAUNCH_WAIT };

public:
	CStatic m_staticRequest;
	CStatic m_staticCountdown;
	
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);  
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

private:
	CString m_strServiceStopFilename;
	UINT m_hTimer;
	bool m_bInTimer;
	int m_nCountdown;
};

//$$******************************************************************
