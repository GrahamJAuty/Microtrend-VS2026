#pragma once
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "MyCommandLineInfo.h"
/**********************************************************************/

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "SharedResources.h"       // main symbols

/**********************************************************************/

class CEcrmanApp : public CWinApp
{
public:
	CEcrmanApp();

	//{{AFX_VIRTUAL(CEcrmanApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CEcrmanApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void LogEntry( bool bLogOut );
	void LogPassword( bool bLogOut );

private:
	bool InitialiseSystem();

	bool m_bLogPassword;
	bool m_bLogEntry;
	CString m_strStartTime;

	int m_nCommsHandle;
	CString m_strTaskStatusFilename;
	CString m_strTaskStatusTimeCode;

private:
	COleDateTime m_timeNextCheck;
	COleDateTime m_oleNextPosTrayTime;
	int m_nNextIdleTask;
	DWORD m_dwStackBase;
};

/**********************************************************************/
//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif 
/**********************************************************************/
#endif
/**********************************************************************/
