#pragma once
//$$******************************************************************

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

//$$******************************************************************

#define WM_MY_TRAY_NOTIFICATION ( WM_USER + 0 )

//$$******************************************************************

class CSmartPayManagerApp : public CWinApp
{
public:
	CSmartPayManagerApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	DECLARE_MESSAGE_MAP()

private:
	bool StartStandardVersion();
};

//$$******************************************************************
extern CSmartPayManagerApp theApp;
//$$******************************************************************
