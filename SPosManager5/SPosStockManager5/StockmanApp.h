#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "SharedResources.h"       // main symbols

/**********************************************************************/
extern const char* szVERSION_ABOUT;
/**********************************************************************/

class CStockmanApp : public CWinApp
{
public:
	CStockmanApp();

	//{{AFX_VIRTUAL(CStockmanApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CStockmanApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool InitialiseSystem();
	void LogEntry( bool bLogOut );
	void LogPassword( bool bLogOut );

private:
	CString m_strStartTime;
	bool m_bLogPassword;
	bool m_bLogEntry;

private:
	COleDateTime m_timeNextCheck;
	int m_nNextIdleTask;
	DWORD m_dwStackBase;
};

/**********************************************************************/
//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif 
/**********************************************************************/
