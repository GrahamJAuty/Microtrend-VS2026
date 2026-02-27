//$$******************************************************************
#include "..\SmartPay4Shared\GlobalState.h"
#include "..\SmartPay4Shared\RecSort.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLConnectionPool.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//$$******************************************************************
#include "GroupShiftReport.h"
//$$******************************************************************

CGroupShiftReport::CGroupShiftReport( CGroupShiftReportConfig& ReportConfig)
	: m_ReportConfig(ReportConfig)
{
}

//$$******************************************************************

int CGroupShiftReport::CreateReport()
{
	if (m_Report.Open(m_ReportConfig.GetReportFilename()) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	CSmartPayGroupShiftFile GroupShiftFile(m_ReportConfig.GetAuditFilename());
	if (GroupShiftFile.Open() == FALSE)
	{
		m_Report.Close();
		return nREPORT_OPENERROR;
	}

	int nErrorNo = nREPORT_NOERROR;

	CWorkingDlg progress(szMSG_CREATINGREPORT);
	progress.Create();

	CString strTitle = m_ReportConfig.GetReportTitle();
	strTitle += m_ReportConfig.GetDateString();
	m_Report.SetStyle1(strTitle);

	AddColumnText();

	GroupShiftFile.JumpIn(m_ReportConfig.GetDateFrom().GetDate());

	nErrorNo = GroupShiftReport(&GroupShiftFile, &progress);

	progress.DestroyWindow();

	m_Report.Close();
	GroupShiftFile.Close();

	return nErrorNo;
}

//$$******************************************************************

void CGroupShiftReport::AddColumnText()
{
	m_Report.AddColumn("Action Date", TA_LEFT, 250);
	m_Report.AddColumn("", TA_LEFT, 200);
	m_Report.AddColumn("Source", TA_LEFT, 200);
	m_Report.AddColumn("UserID", TA_LEFT, 250);
	m_Report.AddColumn("Name", TA_LEFT, 400);
	m_Report.AddColumn("Grp", TA_LEFT, 150);
	m_Report.AddColumn("", TA_LEFT, 100);

	if (m_ReportConfig.GetShowPreviousFlag() == TRUE)
	{
		m_Report.AddColumn("Old Status", TA_LEFT, 250);
		m_Report.AddColumn("Start Date", TA_LEFT, 300);
		m_Report.AddColumn("End Date", TA_LEFT, 300);
		m_Report.AddColumn("Switch Group", TA_LEFT, 300);
		m_Report.AddColumn("", TA_LEFT, 100);
	}

	m_Report.AddColumn("New Status", TA_LEFT, 250);
	m_Report.AddColumn("Start Date", TA_LEFT, 300);
	m_Report.AddColumn("End Date", TA_LEFT, 250);
	m_Report.AddColumn("Switch Group", TA_LEFT, 300);
}

//$$******************************************************************

int CGroupShiftReport::GroupShiftReport(CSmartPayGroupShiftFile* pGroupShift, CWorkingDlg* pDlgProgress)
{
	int nResult = 0;

	g_GlobalState.BuildUserIDCache();
	
	if (m_ReportConfig.GetSortByCardNoFlag() == TRUE)
	{
		nResult = GroupShiftReportByCardNo(pGroupShift, pDlgProgress);
	}
	else
	{
		nResult = GroupShiftReportByDate(pGroupShift, pDlgProgress);
	}

	g_GlobalState.ClearUserIDCache();

	return nResult;
}

//$$******************************************************************

int CGroupShiftReport::GroupShiftReportByDate(CSmartPayGroupShiftFile* pGroupShift, CWorkingDlg* pDlgProgress)
{
	int nCount = 0;
	int nProgressCount = 0;
	bool bCancelButton = FALSE;
	CString strLastDate = "";

	CSmartPayGroupShiftLogLine LogLine;
	while (pGroupShift->ReadLine(LogLine) == TRUE)
	{
		if (( ++nProgressCount % 100) == 0)
		{
			CString strThisDate = LogLine.GetActionDate();
			if (strThisDate != strLastDate)
			{
				CString strCaption = "Creating Report ";
				strCaption += strThisDate;
				pDlgProgress->SetCaption1(strCaption);
				strLastDate = strThisDate;
			}

			if (pDlgProgress->SetCaption2RecordsChecked(nProgressCount, TRUE) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}
		}

		if (IsValid(LogLine) == TRUE)
		{
			bool bExists = g_GlobalState.CheckUserIDCache( LogLine.GetUserID());
	
			if (SaveLine(LogLine, bExists) == TRUE)
			{
				++nCount;
			}
		}
	}

	int nErrorNo = nREPORT_NOERROR;

	if (nCount == 0)
	{
		nErrorNo = nREPORT_NOMATCH;
	}

	return (bCancelButton == TRUE) ? nREPORT_CANCEL : nErrorNo;
}

//$$******************************************************************

int CGroupShiftReport::GroupShiftReportByCardNo(CSmartPayGroupShiftFile* pGroupShift, CWorkingDlg* pDlgProgress)
{
	CRecordSorter sorter;
	CSmartPayGroupShiftLogLine LogLine;

	bool bCancelButton = FALSE;

	int nCount = 0;
	pDlgProgress->SetCaption1("Sorting lines for Audit");

	while (TRUE)
	{
		if ((nCount % 10) == 0)
		{
			if (pDlgProgress->SetCaption2RecordsChecked(nCount, TRUE) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}
		}

		nCount++;

		int nStartFilePos = pGroupShift->GetFilePos();			// position in file to start from
		if (pGroupShift->ReadLine(LogLine) == FALSE)
		{
			break;
		}

		if (IsValid(LogLine) == TRUE)
		{
			sorter.Add(System.FormatCardNo(LogLine.GetUserID(), TRUE), nStartFilePos);
		}
	}

	if (bCancelButton == TRUE)
	{
		return nREPORT_CANCEL;
	}

	if (sorter.GetSize() == 0)
	{
		return nREPORT_NOMATCH;
	}

	int nLineCount = 0;
	CString strLastCardNo = "";

	pDlgProgress->SetCaption1("Creating Audit Report");

	for (int nIndex = 0; nIndex < sorter.GetSize(); nIndex++)
	{
		if ((nIndex % 100) == 0)
		{
			if (pDlgProgress->SetCaption2RecordsChecked(nIndex, TRUE) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}
		}

		pGroupShift->SetFilePos(sorter.GetRecordNo(nIndex));
		if (pGroupShift->ReadLine(LogLine) == TRUE)
		{
			__int64 nUserID = LogLine.GetUserID();
			bool bExists = g_GlobalState.CheckUserIDCache(nUserID);

			CString strUserID = FormatInt64Value(nUserID);

			// on first time round loop
			// save balance line if line is not a balance line
			if (nIndex == 0)
			{
				strLastCardNo = strUserID;
			}
			else
			{
				// change of main label
				if ( strLastCardNo != strUserID)
				{
					if (nLineCount > 0)
					{
						if (m_ReportConfig.GetNewPageFlag() == TRUE)
						{
							m_Report.WriteLine("<NEWPAGE>,1");
						}
					}

					nLineCount = 0;
					strLastCardNo = strUserID;
				}
			}

			if (SaveLine(LogLine, bExists) == TRUE)
			{
				++nLineCount;
			}
		}
	}

	// single report
	if (m_ReportConfig.GetReportLabel() != "R141")
	{
		if (m_ReportConfig.GetNewPageFlag() == TRUE)
		{
			m_Report.WriteLine("<NEWPAGE>,1");
		}
	}

	return (bCancelButton == TRUE) ? nREPORT_CANCEL : nREPORT_NOERROR;
}

//$$******************************************************************

bool CGroupShiftReport::AppendGroupShiftLineFields(CCSV& csv, CSmartPayGroupShiftLogLine& LogLine, bool bCardExists)
{
	CString strAccountNo = System.FormatCardNo(LogLine.GetUserID(), TRUE);

	if (bCardExists == FALSE)
	{
		if (m_ReportConfig.GetShowDeletedLinesFlag() == FALSE)
		{
			return FALSE;
		}
		else
		{
			strAccountNo += " *";
		}
	}

	int nOldStatus = LogLine.GetOriginalStatus();
	int nNewStatus = LogLine.GetEditedStatus();

	csv.Add(LogLine.GetActionSourceString());
	csv.Add(strAccountNo);
	csv.Add(LogLine.GetUsername());
	csv.Add(LogLine.GetCurrentGroupNo());
	csv.Add("");
	
	if (m_ReportConfig.GetShowPreviousFlag() == TRUE)
	{
		csv.Add(LogLine.GetOriginalStatusString());

		if ((1 == nOldStatus) || (2 == nOldStatus))
		{
			csv.Add(LogLine.GetOriginalDateFrom());
			csv.Add(LogLine.GetOriginalDateTo());
			csv.Add(LogLine.GetOriginalGroupNo());
		}
		else
		{
			csv.Add("--");
			csv.Add("--");
			csv.Add("--");
		}

		csv.Add("");
	}

	csv.Add(LogLine.GetEditedStatusString());

	if ((1 == nNewStatus) || (2 == nNewStatus))
	{
		csv.Add(LogLine.GetEditedDateFrom());
		csv.Add(LogLine.GetEditedDateTo());
		csv.Add(LogLine.GetEditedGroupNo());
	}
	else
	{
		csv.Add("--");
		csv.Add("--");
		csv.Add("--");
	}

	return TRUE;
}

//$$******************************************************************

bool CGroupShiftReport::SaveLine(CSmartPayGroupShiftLogLine& LogLine, bool bCardExists)
{
	int nAuditType = m_ReportConfig.GetReportType();

	CCSV csv('\t');
	csv.Add(LogLine.GetActionDate());
	csv.Add(LogLine.GetActionTime());
	
	if (AppendGroupShiftLineFields(csv, LogLine, bCardExists) == FALSE)
	{
		return FALSE;
	}

	return m_Report.WriteLine(csv);
}

//$$******************************************************************

bool CGroupShiftReport::IsValid(CSmartPayGroupShiftLogLine& LogLine)
{
	CSSDate date(LogLine.GetActionDate());
	CSSTime time(LogLine.GetActionTime());

	if (date < m_ReportConfig.GetDateFrom())
	{
		return FALSE;
	}

	if (date > m_ReportConfig.GetDateTo())
	{
		return FALSE;
	}

	if (m_ReportConfig.GetTimeRangeFlag() == TRUE)
	{
		if (FALSE == m_ReportConfig.GetContinousFlag())
		{
			if (time < m_ReportConfig.GetTimeFrom())
			{
				return FALSE;
			}

			if (time > m_ReportConfig.GetTimeTo())
			{
				return FALSE;
			}
		}
		else
		{
			if (date == m_ReportConfig.GetDateFrom())
			{
				if (time < m_ReportConfig.GetTimeFrom())
				{
					return FALSE;
				}
			}

			if (date == m_ReportConfig.GetDateTo())
			{
				if (time > m_ReportConfig.GetTimeTo())
				{
					return FALSE;
				}
			}
		}
	}

	int nSourceType = LogLine.GetActionSource();

	switch (nSourceType)
	{
	case 0:
		if (m_ReportConfig.GetShowSourceManagerFlag() == FALSE)
		{
			return FALSE;
		}
		break;

	case 1:
		if (m_ReportConfig.GetShowSourceServerFlag() == FALSE)
		{
			return FALSE;
		}
		break;

	case 2:
		if (m_ReportConfig.GetShowSourceBackgroundFlag() == FALSE)
		{
			return FALSE;
		}
		break;

	case 3:
	case 4:
		if (m_ReportConfig.GetShowSourceManualFlag() == FALSE)
		{
			return FALSE;
		}
		break;

	default:
		return FALSE;
	}

	if (IsCardInRange(LogLine) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

//$$******************************************************************

bool CGroupShiftReport::IsCardInRange(CSmartPayGroupShiftLogLine& LogLine)
{
	CString strCardNo = "";
	strCardNo.Format("%I64d", LogLine.GetUserID());

	if (m_ReportConfig.GetReportRangeType() == nREPORT_LIST)
	{
		return m_ReportConfig.IsValidCard(strCardNo, Filenames.GetReportCardListFilename(m_ReportConfig.GetReportLabel()));
	}

	__int64 nCardNo = _atoi64(strCardNo);
	if (nCardNo < m_ReportConfig.GetInt64CardNoFrom() || nCardNo > m_ReportConfig.GetInt64CardNoTo())
	{
		return FALSE;
	}

	bool bResult = TRUE;
	if (m_ReportConfig.GetSingleGroupFlag() == TRUE)
	{
		bResult = FALSE;
		int nSingleGroupNo = m_ReportConfig.GetSingleGroupNo();
		
		if ( LogLine.GetCurrentGroupNo() == nSingleGroupNo)
		{
			bResult = TRUE;
		}
		else if (LogLine.GetOriginalGroupNo() == nSingleGroupNo)
		{
			bResult = TRUE;
		}
		else if (LogLine.GetEditedGroupNo() == nSingleGroupNo)
		{
			bResult = TRUE;
		}
	}

	return bResult;
}

//$$******************************************************************

