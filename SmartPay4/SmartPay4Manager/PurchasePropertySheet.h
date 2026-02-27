#pragma once
/**********************************************************************/
#include "PurchasePropertyPage1.h"
#include "PropPageReportPlu.h"
#include "ReportPurchaseHelpers.h"
/**********************************************************************/

class CPurchasePropertySheet : public CSSAutoShutdownPropertySheet
{
public:
	CPurchasePropertySheet( CReportPurchaseHelpers& ReportPurchaseHelpers, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CPurchasePropertySheet();

public:
	void SaveReportPurchaseHelpers() { m_Page2.SaveReportPurchaseHelpers(m_ReportPurchaseHelpers); }

private:
	CPurchasePropertyPage1 m_Page1;
	CPropPageReportPlu m_Page2;

protected:
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	CReportPurchaseHelpers& m_ReportPurchaseHelpers;
};


/**********************************************************************/
