//$$******************************************************************
#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRowGroup.h"
#include "..\SPosLoyalty4Shared\SQLTable_GroupTopupBonus\SQLRowGroupTopupBonus.h"
//$$******************************************************************
#include "GroupPropertyPage1.h"
#include "GroupPropertyPage2.h"
#include "GroupPropertyPage4.h"
//$$******************************************************************

class CGroupPropertySheet : public CSSPropertySheet
{
public:
	CGroupPropertySheet( CSQLRowGroup& RowGroup, CReportConsolidationArray<CSQLRowGroupTopupBonus>& arrayGroupTopupBonus, CWnd* pParentWnd, UINT iSelectPage = 0);
	virtual ~CGroupPropertySheet();

public:
	CGroupPropertyPage1 m_Page1;
	CGroupPropertyPage2 m_Page2;
	CGroupPropertyPage4 m_Page4;

protected:
	DECLARE_MESSAGE_MAP()
};

//$$******************************************************************
