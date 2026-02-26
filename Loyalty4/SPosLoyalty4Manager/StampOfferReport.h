#pragma once
//$$******************************************************************
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
#include "..\SPosLoyalty4Shared\SQLStampOfferBuffer.h"
//$$******************************************************************
#include "WorkingDlg.h"
//$$******************************************************************
#include "StampOfferReportConfig.h"
//$$******************************************************************

class CStampOfferReport
{
public:
	CStampOfferReport(CStampOfferReportConfig& ReportConfig);

public:
	int CreateReport();

private:
	void AddColumnText();
	void CreateFileDateArray();

private:
	int StampOfferReport(CWorkingDlg* pDlgWorking);
	int StampOfferReportByCardNo(CWorkingDlg* pDlgWorking);
	int StampOfferReportByDate(CWorkingDlg* pDlgWorking);

private:
	bool IsValid(CString strLine, CSQLStampOfferAuditLine& Line);
	void MergeLastFullRedeemLine(CSQLStampOfferAuditLine& Line);
	bool IsCardInRange(CSQLStampOfferAuditLine& Line);
	bool SaveLine(CSQLStampOfferAuditLine& Line, bool bExists);
	CString FormatDate(CString strDate, bool bIsExpiry);
	CString FormatTime(CString strTime);

private:
	CReportConsolidationArray<CSortedStringItem> m_arrayFileDates;
	CStampOfferReportConfig& m_ReportConfig;
	CReportFile m_Report;

private:
	CString m_strFileDateFrom;
	CString m_strFileDateTo;
	CString m_strFileTimeFrom;
	CString m_strFileTimeTo;

private:
	bool m_bGotValidFullRedeemLine;
	CSQLStampOfferAuditLine m_LastFullRedeemLine;
};

//$$******************************************************************