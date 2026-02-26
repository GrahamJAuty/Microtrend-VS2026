#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "MyCommandLineInfo.h"
/**********************************************************************/

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "SharedResources.h"       // main symbols

/**********************************************************************/
extern const char* szVERSION_ABOUT;
/**********************************************************************/

class CPosTrayApp : public CWinApp
{
public:
	CPosTrayApp();

	//{{AFX_VIRTUAL(CPosTrayApp)
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CPosTrayApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void ListenForTasks(bool bShowErrors, int nSpawnType);
	void EmailReports(CMyCommandLineInfo& cmdInfo);
	void PrintReports(CMyCommandLineInfo& cmdInfo);
	void ExportReports(CMyCommandLineInfo& cmdInfo);
	void FNBExport(CMyCommandLineInfo& cmdInfo);
	void ChartwellsExport(CMyCommandLineInfo& cmdInfo);
	void ChartwellsDbCheck(CMyCommandLineInfo& cmdInfo);
	void FTPExport(CMyCommandLineInfo& cmdInfo);
	void S4LabourExport(CMyCommandLineInfo& cmdInfo);
	void CheckSystemBackup(CMyCommandLineInfo& cmdInfo);
	bool TestRegistration(CMyCommandLineInfo& cmdInfo);

#ifdef POSTRAY_SOCKET

private:
	void CreateSocketReportEpos(CMyCommandLineInfo& cmdInfo);
	bool SocketLoadSystem(CString& strError);

#endif

private:
	int m_nPosTrayExitCode;
};

/**********************************************************************/
//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif 
/**********************************************************************/

