#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//$$******************************************************************
#include "GroupShiftReportConfig.h"
#include "CardArchiveDlg.h"
#include "SmartPayGroupShiftFile.h"
//$$******************************************************************

class CGroupShiftReport
{
public:
	CGroupShiftReport(CGroupShiftReportConfig& ReportConfig);
	int CreateReport();
	
private:
	void AddColumnText();
	bool SaveLine(CSmartPayGroupShiftLogLine& LogLine, bool bCardExists);
	
	int GroupShiftReport(CSmartPayGroupShiftFile* pGroupShift, CWorkingDlg* pDlgWorking);
	int GroupShiftReportByDate(CSmartPayGroupShiftFile* pGroupShift, CWorkingDlg* pDlgWorking);
	int GroupShiftReportByCardNo(CSmartPayGroupShiftFile* pGroupShift, CWorkingDlg* pDlgWorkings);

private:
	bool IsValid(CSmartPayGroupShiftLogLine& LogLine);
	bool IsCardInRange(CSmartPayGroupShiftLogLine& LogLine);

private:
	bool AppendGroupShiftLineFields(CCSV& csv, CSmartPayGroupShiftLogLine& LogLine, bool bCardExists);
	
private:
	CGroupShiftReportConfig& m_ReportConfig;
	CReportFileWithEndText m_Report;
};

//$$******************************************************************
