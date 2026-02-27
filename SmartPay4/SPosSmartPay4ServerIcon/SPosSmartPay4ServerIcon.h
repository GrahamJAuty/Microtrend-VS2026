//$$******************************************************************
#pragma once
//$$******************************************************************

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

//$$******************************************************************

#include "resource.h"		// main symbols

//$$******************************************************************
#define WM_MY_TRAY_NOTIFICATION ( WM_USER + 0 )
//$$******************************************************************

class CSPosSmartPay4ServerIconApp : public CWinApp
{
public:
	CSPosSmartPay4ServerIconApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

public:
	ULONGLONG m_AppStartTime;
	bool m_bFailIconChange;
};

//$$******************************************************************
extern CSPosSmartPay4ServerIconApp theApp;
//$$******************************************************************
