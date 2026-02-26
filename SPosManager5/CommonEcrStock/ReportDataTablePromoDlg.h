#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CCSVReportLine.h"
#include "ReportDataTableDlg.h"
/**********************************************************************/

class CReportDataTablePromoDlg : public CReportDataTableDlg
{
public:
	CReportDataTablePromoDlg(CWnd* pParent = NULL);   

public:
	void HandleReport1();
	void HandleReport2();
	void HandleReport3();
	void HandleReport4();

private:
	void CreateDetailReport(int nSubType);
	void AddBasicPromoHeaders( CReportFile& ReportFile, int nSubType );
	void AddBasicPromoSettings( CCSVReportLine& csvOut, CPromotionsCSVRecord& Promo, int nSubType );
};

/**********************************************************************/
#endif
/**********************************************************************/
