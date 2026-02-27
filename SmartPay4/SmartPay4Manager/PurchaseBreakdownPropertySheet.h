#pragma once
//**********************************************************************
#include "PropPageReportPlu.h"
#include "PurchasePropertyPage2.h"
//**********************************************************************

class CPurchaseBreakdownPropertySheet : public CSSAutoShutdownPropertySheet
{
public:
	CPurchaseBreakdownPropertySheet(CReportPurchaseHelpers& ReportPurchaseHelpers, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CPurchaseBreakdownPropertySheet();

public:
	void SaveReportPurchaseHelpers() { m_Page2.SaveReportPurchaseHelpers(m_ReportPurchaseHelpers); }

private:
	CPurchasePropertyPage2 m_Page1;
	CPropPageReportPlu m_Page2;

protected:
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	CReportPurchaseHelpers& m_ReportPurchaseHelpers;
};

//**********************************************************************
