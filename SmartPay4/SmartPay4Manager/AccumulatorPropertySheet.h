#pragma once
//*******************************************************************
#include "AccumulatorPropertyPage1.h"
#include "AccumulatorReportInfo.h"
#include "PropPageReportPlu.h"
//*******************************************************************

class CAccumulatorPropertySheet : public CSSAutoShutdownPropertySheet
{
public:
	CAccumulatorPropertySheet(const char* szLabel, bool bPluPage, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CAccumulatorPropertySheet();

public:
	void SetReportInfo(CAccumulatorReportInfo* pReportInfo);
	void SaveReportInfo(CAccumulatorReportInfo* pReportInfo){m_Page2.SaveAccumulatorReportInfo(pReportInfo);}

private:
	CAccumulatorPropertyPage1 m_Page1;
	CPropPageReportPlu m_Page2;

public:
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	CAccumulatorReportInfo* m_pReportInfo;
};

//*******************************************************************
