/**********************************************************************/
#pragma once
/**********************************************************************/
#include "MyCommandLineInfo.h"
/**********************************************************************/

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

/**********************************************************************/

class CWebServerApp : public CWinApp
{
public:
	CWebServerApp();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

private:
	void RunService(CMyCommandLineInfo& cmdInfo);
	void RunDesktop(CMyCommandLineInfo& cmdInfo);
	void RunCommand(CMyCommandLineInfo& cmdInfo);

private:
	int m_nRetCode;
};

/**********************************************************************/

extern CWebServerApp theApp;

/**********************************************************************/
