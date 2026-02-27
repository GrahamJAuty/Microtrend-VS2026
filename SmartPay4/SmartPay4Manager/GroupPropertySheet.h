#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
//$$******************************************************************
#include "GroupPropertyPage1.h"
#include "GroupPropertyPage2.h"
#include "GroupPropertyPage6.h"
#include "GroupPropertyPage7.h"
#include "GroupPropertyPage8.h"
//$$******************************************************************

class CGroupPropertySheet : public CSSAutoShutdownPropertySheet
{
public:
	CGroupPropertySheet( CSQLRowGroupFull& RowGroup, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CGroupPropertySheet();

public:
	CGroupPropertyPage1 m_Page1;
	CGroupPropertyPage2 m_Page2;
	CGroupPropertyPage6 m_Page6;
	CGroupPropertyPage7 m_Page7;
	CGroupPropertyPage8 m_Page8;

protected:
	DECLARE_MESSAGE_MAP()
};

//$$******************************************************************

