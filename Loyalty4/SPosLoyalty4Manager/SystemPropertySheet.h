#pragma once
//$$******************************************************************
#include "resource.h"
//*******************************************************************
#include "SystemPropertyPage1.h"
#include "SystemPropertyPage2.h"
#include "SystemPropertyPage3.h"
#include "SystemPropertyPage4.h"
#include "SystemPropertyPage5.h"
#include "SystemPropertyPage6.h"
#include "SystemPropertyPage7.h"
//$$******************************************************************

class CSystemPropertySheet : public CSSPropertySheet
{
public:
	CSystemPropertySheet( CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSystemPropertySheet() {}
	bool ChangedServerSettings();
	int GetChangedPaxtonMode();
	
public:
	CSystemPropertyPage1 m_Page1;
	CSystemPropertyPage2 m_Page2;
	CSystemPropertyPage3 m_Page3;
	CSystemPropertyPage4 m_Page4;
	CSystemPropertyPage5 m_Page5;
	CSystemPropertyPage6 m_Page6;
	CSystemPropertyPage7 m_Page7;

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};

//$$******************************************************************
