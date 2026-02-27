#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\EmailAlertData.h"
//$$******************************************************************
#include "EmailAlertPropertyPage1.h"
#include "EmailAlertPropertyPage2.h"
#include "EmailAlertPropertyPage3.h"
#include "EmailAlertPropertyPage4.h"
//$$******************************************************************

class CEmailAlertPropertySheet : public CSSAutoShutdownPropertySheet
{
public:
	CEmailAlertPropertySheet( CWnd* pParentWnd );
	
public:
	void SaveData();
	
public:
	CEmailAlertPropertyPage1 m_Page1;
	CEmailAlertPropertyPage2 m_Page2;
	CEmailAlertPropertyPage3 m_Page3;
	CEmailAlertPropertyPage4 m_Page4;
	
protected:
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	CEmailAlertData m_data;
};

//$$******************************************************************
