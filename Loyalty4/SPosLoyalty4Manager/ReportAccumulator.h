#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\LoyaltyAuditFile.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRecordSetAccountFull.h"
//*******************************************************************
#include "AccumulatorPropertySheet.h"
#include "DbReportHandler.h"
#include "ReportInfoAccumulator.h"
//*******************************************************************

class CReportAccumulator
{
public:
	CReportAccumulator ( const char* szLabel, const char* szAuditFile, CWnd* pParent );
	~CReportAccumulator();

public:
	bool Show();

private:
	int CompileReport();
	void AddColumnText();
	bool SaveReportLine( CArray<int,int>& arrayHeader, CSQLRowAccountFull& RowAccount, bool bExist, int nQty, double dValue, int nPos = 0 );
	void SaveTotals (CArray<int, int>& arrayHeader, int nIndex, int nGroupNo, int nCount, bool bRankReport = FALSE );

private:
	CWnd* m_pParent;
	CAccumulatorPropertySheet m_psReport;
	CReportInfoAccumulator m_ReportInfo;
	CDbReportHandler m_DbReporter;

	CLoyaltyAuditFile m_auditFile;
	CAuditRecord m_atc;

	CReportFile m_Report;

	CString m_strReportLabel;
	int m_nReportType;

	double m_dGroupValue;
	double m_dReportValue;
	int m_nGroupQty;
	int m_nReportQty;
};

//*******************************************************************
