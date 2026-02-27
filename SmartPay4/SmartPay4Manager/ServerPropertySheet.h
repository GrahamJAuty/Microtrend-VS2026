#pragma once
//**********************************************************************
#include "..\SmartPay4Shared\ServerData.h"
//**********************************************************************
#include "ServerPropertyPage1.h"
#include "ServerPropertyPage2.h"
#include "ServerPropertyPage3.h"
#include "ServerPropertyPage5.h"
#include "ServerPropertyPage6.h"
//**********************************************************************

class CServerPropertySheet : public CSSAutoShutdownPropertySheet
{
public:
	CServerPropertySheet( CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CServerPropertySheet() {}
	bool Save();

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	CServerPropertyPage1 m_Page1;
	CServerPropertyPage2 m_Page2;
	CServerPropertyPage3 m_Page3;
	CServerPropertyPage5 m_Page5;
	CServerPropertyPage6 m_Page6;
};

//**********************************************************************
