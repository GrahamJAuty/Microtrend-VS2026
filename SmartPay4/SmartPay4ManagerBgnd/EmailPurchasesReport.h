#pragma once
//*******************************************************************
#include "..\SmartPay4Shared\TLogFile.h"
#include "..\SmartPay4Shared\TLogRecord.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//*******************************************************************

class CEmailPurchasesReport
{
public:
	CEmailPurchasesReport();
	bool Create ( CSQLRowAccountFull& RowAccount, const char* szReportFile );

private:
	void Reset();
	void AddColumnText();
	void ShowTotals();

	void AccountItemisedReport (CSQLRowAccountFull& RowAccount, CTLogFile* pTlogFile );
	void ShowItemisedSales ( CTLogRecord* pTLogRecord );

public:
	CSSDate m_dateFrom;
	CSSTime m_timeFrom;
	bool m_bExcludeZeroPriced;

	CSSDate m_dateHighest;			// hold highest date for emailing
	CSSTime m_timeHighest;

private:
	CReportFileWithEndText m_Report;

	CSSDate m_dateTo;
	CString m_strLastDate;

	int m_nLineCount;
	double m_dTotalCardSpend;
	double m_dTotalValue;
	double m_dTotalQty;
};

//*******************************************************************
