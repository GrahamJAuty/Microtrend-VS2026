/**********************************************************************/
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
/**********************************************************************/
#include "ReportPebbleLinkTimes.h"
/**********************************************************************/

CReportPebbleLinkTimes::CReportPebbleLinkTimes(CWnd* pParent)
	: m_dlgReport(m_ReportInfo, pParent),
	m_Repman(m_ReportInfo.m_strReportTitle, pParent)
{
	m_pParent = pParent;
}

//*******************************************************************

void CReportPebbleLinkTimes::ShowFromDialog()
{
	int nErrorNo = CreateFromDialog();
	if (nErrorNo != nREPORT_NOERROR)
	{
		m_Repman.DisplayError(nErrorNo);
	}
}

//*******************************************************************

void CReportPebbleLinkTimes::ShowDirect()
{
	int nErrorNo = CreateDirect();
	if (nErrorNo != nREPORT_NOERROR)
	{
		m_Repman.DisplayError(nErrorNo);
	}
}

//*******************************************************************

int CReportPebbleLinkTimes::CreateDirect()
{
	int nErrorNo = nREPORT_NOERROR;

	if (m_Report.Open(m_ReportInfo.m_strReportFilename) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	CWorkingDlg dlgWorking(szMSG_CREATINGREPORT, TRUE);
	dlgWorking.Create();

	CString strTitle = m_ReportInfo.m_strReportTitle;
	strTitle += m_ReportInfo.GetDateString();

	m_Report.SetStyle1(strTitle);
	AddColumnText();

	nErrorNo = CreateReport(dlgWorking);

	dlgWorking.DestroyWindow();

	m_Report.Close();

	if (nErrorNo == nREPORT_NOERROR)
	{
		nErrorNo = m_Repman.DisplayReport(m_ReportInfo.m_strReportFilename, m_ReportInfo.m_strParamsFilename, m_ReportInfo.m_strReportKey, &m_dlgReport);
	}
	else
	{
		m_Repman.DisplayError(nErrorNo);
	}

	return nREPORT_NOERROR;
}

//*******************************************************************

int CReportPebbleLinkTimes::CreateFromDialog()
{
	int nErrorNo = nREPORT_NOERROR;
	
	while (m_dlgReport.DoModal() == IDOK)
	{
		if (m_Report.Open(m_ReportInfo.m_strReportFilename) == FALSE)
		{
			return nREPORT_CREATEFAIL;
		}

		CWorkingDlg dlgWorking(szMSG_CREATINGREPORT, TRUE);
		dlgWorking.Create();

		CString strTitle = m_ReportInfo.m_strReportTitle;
		strTitle += m_ReportInfo.GetDateString();

		m_Report.SetStyle1(strTitle);
		AddColumnText();

		nErrorNo = CreateReport(dlgWorking);

		dlgWorking.DestroyWindow();

		m_Report.Close();

		if (nErrorNo == nREPORT_NOERROR)
		{
			nErrorNo = m_Repman.DisplayReport(m_ReportInfo.m_strReportFilename, m_ReportInfo.m_strParamsFilename, m_ReportInfo.m_strReportKey, &m_dlgReport);
		}
		else
		{
			m_Repman.DisplayError(nErrorNo);
		}
	}

	return nREPORT_NOERROR;
}

//*******************************************************************

int CReportPebbleLinkTimes::CreateReport(CWorkingDlg& dlgWorking)
{
	CString strDateFrom = "", strDateTo = "", strTimeFrom = "", strTimeTo = "";

	if (ValidateDateOrTime(TRUE, m_ReportInfo.m_dateFrom.GetCSVDate(), strDateFrom) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	if (ValidateDateOrTime(TRUE, m_ReportInfo.m_dateTo.GetCSVDate(), strDateTo) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	if (ValidateDateOrTime(FALSE, m_ReportInfo.m_timeStart.GetCSVTime(), strTimeFrom) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	if (ValidateDateOrTime(FALSE, m_ReportInfo.m_timeEnd.GetCSVTime(), strTimeTo) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	if (TRUE == m_ReportInfo.m_bContinuous)
	{
		CSQLRecordSetPebbleLinkTime recordSet(NULL,
			RSParams_PebbleLinkTime_NormalNoParams{});

		recordSet.m_strFilter.Format("(%s + %s) BETWEEN ? AND ?",
			(const char*)PebbleLinkTime::Date.Label,
			(const char*)PebbleLinkTime::Time.Label);

		//PARAM LABELS DON'T MATTER HERE AS WE ARE BUILDING WHERE CLAUSE MANUALLY
		recordSet.m_MD.AddTextParam("A", strDateFrom + strTimeFrom);
		recordSet.m_MD.AddTextParam("B", strDateTo + strTimeTo);
		recordSet.m_strFilter += GetAccountWhereFilter(recordSet);
		recordSet.UpdateParamCount();

		recordSet.m_strSort = GetOrderBy(recordSet);

		return CreateReportFromRecordSet(recordSet, dlgWorking);
	}
	else
	{
		CSQLRecordSetPebbleLinkTime recordSet(NULL,
			RSParams_PebbleLinkTime_NormalNoParams{});

		recordSet.m_strFilter.Format(
			"%s BETWEEN ? AND ? AND %s BETWEEN ? AND ?",
			(const char*)PebbleLinkTime::Date.Label,
			(const char*)PebbleLinkTime::Time.Label );

		//PARAM LABELS DON'T MATTER HERE AS WE ARE BUILDING WHERE CLAUSE MANUALLY
		recordSet.m_MD.AddTextParam("A", strDateFrom);
		recordSet.m_MD.AddTextParam("B", strDateTo);
		recordSet.m_MD.AddTextParam("C", strTimeFrom);
		recordSet.m_MD.AddTextParam("D", strTimeTo);
		recordSet.m_strFilter += GetAccountWhereFilter(recordSet);
		recordSet.UpdateParamCount();

		recordSet.m_strSort = GetOrderBy(recordSet);

		return CreateReportFromRecordSet(recordSet, dlgWorking);
	}
}

//*******************************************************************

CString CReportPebbleLinkTimes::GetAccountWhereFilter(CSQLRecordSetPebbleLinkTime& recordSet)
{
	CString strFilter = "";

	CString strUserIDFrom = "";
	strUserIDFrom.Format("%I64d", _atoi64(m_ReportInfo.m_strCardNoFrom));

	CString strUserIDTo = "";
	strUserIDTo.Format("%I64d", _atoi64(m_ReportInfo.m_strCardNoTo));

	if ( 0 == m_ReportInfo.m_nReportRangeType )
	{
		strFilter.Format( " AND %s BETWEEN ? AND ?",
			(const char*)PebbleLinkTime::UserID.Label);

		recordSet.m_MD.AddTextParam("E", strUserIDFrom);
		recordSet.m_MD.AddTextParam("F", strUserIDTo);
	}
	else
	{
		strFilter.Format( " AND %s = ?",
			(const char*)PebbleLinkTime::UserID.Label);

		recordSet.m_MD.AddTextParam("F", strUserIDTo);
	}

	if (m_ReportInfo.m_nSingleGroupNo > 0 )
	{
		strFilter.AppendFormat( " AND %s = ?",
			(const char*)PebbleLinkTime::GroupNo.Label);

		recordSet.m_MD.AddIntParam("G", m_ReportInfo.m_nSingleGroupNo);
	}

	return strFilter;
}

//*******************************************************************

CString CReportPebbleLinkTimes::GetOrderBy(CSQLRecordSetPebbleLinkTime& recordSet)
{
	CCSV csvOrderBy;
	
	if (TRUE == m_ReportInfo.m_bSortByDate)
	{
		csvOrderBy.Add(PebbleLinkTime::Date.Label);
		csvOrderBy.Add(PebbleLinkTime::Time.Label);
	}

	if (TRUE == m_ReportInfo.m_bSortByGroup)
	{
		csvOrderBy.Add(PebbleLinkTime::GroupNo.Label);

		if ((FALSE == m_ReportInfo.m_bSortByDate) && (FALSE == m_ReportInfo.m_bSortByID))
		{
			csvOrderBy.Add(PebbleLinkTime::Date.Label);
			csvOrderBy.Add(PebbleLinkTime::Time.Label);
		}
	}

	if (TRUE == m_ReportInfo.m_bSortByID)
	{
		csvOrderBy.Add(PebbleLinkTime::UserID.Label);
	}

	return csvOrderBy.GetLine();
}

//*******************************************************************

int CReportPebbleLinkTimes::CreateReportFromRecordSet(CSQLRecordSetPebbleLinkTime& recordSet, CWorkingDlg& dlgWorking)
{
	bool bGotData = FALSE;

	CString strLastReportDate = "";

	int nCount = 0;
	CSQLRowPebbleLinkTime rowPB;
	while(recordSet.StepSelectAll(rowPB) == TRUE)
	{
		if ( dlgWorking.SetCaption2RecordsChecked(++nCount, TRUE) == TRUE )
		{
			return nREPORT_CANCEL;
		}

		CCSV csv('\t');

		CString strReportDate = "", strReportTime = "";

		{
			CString strDate = rowPB.GetDate();

			if (strDate.GetLength() == 8)
			{
				strReportDate = strDate.Right(2) + CString("/") + strDate.Mid(4, 2) + CString("/") + strDate.Left(4);
			}

			CString strTime = rowPB.GetTime();

			if (strTime.GetLength() == 6)
			{
				strReportTime = strTime.Left(2) + CString(":") + strTime.Mid(2, 2) + CString(":") + strTime.Right(2);
			}
		}

		CString strMatchBy = "";
		switch (rowPB.GetMatchType())
		{
		case 0:
			strMatchBy = "MIS";
			break;

		case 1:
			strMatchBy = "UPN";
			break;

		default:
			strMatchBy = "Unknown";
			break;
		}

		CString strProcess = "";
		switch (rowPB.GetProcessType())
		{
		case 0:
			strProcess = "Manager";
			break;

		case 1:
			strProcess = "Bgnd";
			break;

		default:
			strProcess = "Unknown";
			break;
		}

		if (TRUE == m_ReportInfo.m_bSortByDate)
		{
			if (strLastReportDate != strReportDate)
			{
				if ( strLastReportDate != "")
				{
					m_Report.WriteLine("");
				}

				m_Report.WriteLine(CString("<..>") + strReportDate);
				m_Report.WriteLine("<LI>");
				strLastReportDate = strReportDate;
			}
		}

		csv.Add(strReportDate);
		csv.Add(strReportTime);
		csv.Add(System.FormatCardNo(rowPB.GetUserID(), TRUE));
		csv.Add(rowPB.GetGroupNo());
		csv.Add(rowPB.GetUsername());
		csv.Add(rowPB.GetForename());
		csv.Add(rowPB.GetSurname());
		csv.Add(rowPB.GetPebbleFirstName());
		csv.Add(rowPB.GetPebbleLastName());
		csv.Add(strProcess);
		csv.Add(strMatchBy);
		csv.Add(rowPB.GetMIS());
		csv.Add(rowPB.GetUPN());

		m_Report.WriteLine(csv.GetLine());
		bGotData = TRUE;
	}

	if (FALSE == bGotData)
	{
		return nREPORT_NOMATCH;
	}

	return nREPORT_NOERROR;
}

//*******************************************************************

bool CReportPebbleLinkTimes::ValidateDateOrTime(bool bDate, CString strIn, CString& strOut)
{
	int nValidLen = bDate ? 8 : 4;

	if (strIn.GetLength() != nValidLen)
	{
		return FALSE;
	}

	if (FALSE == bDate)
	{
		strOut = strIn;
	}
	else
	{
		strOut = strIn.Right(4) + strIn.Mid(2, 2) + strIn.Left(2);
	}

	return TRUE;
}

//*******************************************************************

void CReportPebbleLinkTimes::AddColumnText()
{
	m_Report.AddColumn("Date", TA_LEFT, 300);
	m_Report.AddColumn("Time", TA_LEFT, 250);
	m_Report.AddColumn("UserID", TA_LEFT, 250);
	m_Report.AddColumn("GroupNo", TA_LEFT, 200);
	m_Report.AddColumn("Username", TA_LEFT, 300);
	m_Report.AddColumn("Forename", TA_LEFT, 300);
	m_Report.AddColumn("Surname", TA_LEFT, 300);
	m_Report.AddColumn("PbFirstName", TA_LEFT, 300);
	m_Report.AddColumn("PbLastName", TA_LEFT, 300);
	m_Report.AddColumn("Process", TA_LEFT, 250);
	m_Report.AddColumn("MatchBy", TA_LEFT, 250);
	m_Report.AddColumn("MIS", TA_LEFT, 300);
	m_Report.AddColumn("UPN", TA_LEFT, 300);
}

//**********************************************************************
