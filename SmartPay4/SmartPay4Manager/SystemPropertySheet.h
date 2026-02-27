#pragma once
//$$******************************************************************
#include "SystemPropertyPage1.h"
#include "SystemPropertyPage2.h"
#include "SystemPropertyPage3.h"
#include "SystemPropertyPage5.h"
#include "SystemPropertyPage6.h"
#include "SystemPropertyPage7.h"
#include "SystemPropertyPage8.h"
#include "SystemPropertyPage9.h"
#include "SystemPropertyPage10.h"
//$$******************************************************************

class CSystemPropertySheet : public CSSAutoShutdownPropertySheet
{
public:
	CSystemPropertySheet( CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	bool RestartServerReqd();
	bool HasDisplayTab(){ return m_bHasDisplayTab; }

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

	virtual void CSSAutoShutdownLogOff()
	{
		TRACE("Auto Shutdown LogOff System Property Sheet\n");
		EndDialog(IDCANCEL);
	}
	
public:
	CSystemPropertyPage1 m_Page1;
	CSystemPropertyPage2 m_Page2;
	CSystemPropertyPage3 m_Page3;
	CSystemPropertyPage5 m_Page5;
	CSystemPropertyPage6 m_Page6;
	CSystemPropertyPage7 m_Page7;
	CSystemPropertyPage8 m_Page8;
	CSystemPropertyPage9 m_Page9;
	CSystemPropertyPage10 m_Page10;

private:
	bool m_bHasDisplayTab;
};

//$$******************************************************************
