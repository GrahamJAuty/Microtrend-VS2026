#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "MixMatchCSVArray.h"
#include "ReportDataTableDlg.h"
/**********************************************************************/

class CReportDataTableMixMatchDlg : public CReportDataTableDlg
{
public:
	CReportDataTableMixMatchDlg(CWnd* pParent = NULL);   

private:
	void HandleReport1();
	void HandleReport2();
	void HandleReport3();

private:
	void CreateDetailReport(int nSubType);
	void GetDetailAndValueString( CMixMatchCSVRecord& record );
	const char* GetBucketString( int nTrip, int nOfferGroup );

private:
	CString m_strValue;
	CString m_strDetail;
	CString m_strBucket;
};

/**********************************************************************/
#endif
/**********************************************************************/
