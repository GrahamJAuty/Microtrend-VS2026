//$$******************************************************************
#include "AccumulatorPropertySheet.h"
//$$******************************************************************
#include "ReportInfoAccumulator.h"
//$$******************************************************************

CReportInfoAccumulator::CReportInfoAccumulator(const char* szLabel) : m_TerminalListHandler(Filenames.GetTerminalListFilename())
{
	m_strReportLabel = szLabel;
	Reset();
}

//$$******************************************************************

void CReportInfoAccumulator::Reset()
{
	m_strReportTitle = "";
	m_strReportName = Filenames.GetReportFilename(m_strReportLabel);
	m_strParamsName = Filenames.GetReportParamsFilename(m_strReportLabel);
	m_strReportKey = "";

	//criteria
	m_dThreshold = 0.0;
	m_nRankByType = 1;
	m_nTop100ChartSize = 100;
	m_bNoZeroSkip = FALSE;
	m_bExcludeUnsold = TRUE;
	//date
	m_dateFrom.SetCurrentDate();
	m_dateTo.SetCurrentDate();
	//time
	m_bTimeRange = FALSE;
	m_timeFrom.SetTime("00:00");
	m_timeTo.SetTime("23:59");
	//terminals
	SetDbNo(0);
	SetFolderSet(0);
	SetTerminalListType(0);
	//plu list
	m_bNewPage = FALSE;
	m_bIncludeUnknownItems = FALSE;
}

//$$******************************************************************

const char* CReportInfoAccumulator::GetDateFromString()
{
	m_strDateFrom = m_dateFrom.GetDate();
	return m_strDateFrom;
}

//$$******************************************************************

const char* CReportInfoAccumulator::GetDateToString()
{
	m_strDateTo = m_dateTo.GetDate();
	return m_strDateTo;
}

//$$******************************************************************

void CReportInfoAccumulator::SetDateFrom(CTime& date)
{
	CTime time(date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0);
	m_dateFrom.SetDate(time);
}

//$$******************************************************************

void CReportInfoAccumulator::SetDateTo(CTime& date)
{
	CTime time(date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0);
	m_dateTo.SetDate(time);
}

//$$******************************************************************

void CReportInfoAccumulator::SetTimeFrom(CTime& time)
{
	m_timeFrom.SetTime(time);
}

//$$******************************************************************

void CReportInfoAccumulator::SetTimeTo(CTime& time)
{
	m_timeTo.SetTime(time);
}

//$$******************************************************************

const char* CReportInfoAccumulator::GetTimeFromString()
{
	m_strTimeFrom = m_timeFrom.GetTime();
	return m_strTimeFrom;
}

//$$******************************************************************

const char* CReportInfoAccumulator::GetTimeToString()
{
	m_strTimeTo = m_timeTo.GetTime();
	return m_strTimeTo;
}

//$$******************************************************************

const char* CReportInfoAccumulator::GetThresholdText()
{
	m_strThresholdText = "";

	switch (GetReportType())
	{
	case nACCUMULATE_LOWPOINTS:
		if (m_dThreshold > 0.0)
		{
			m_strThresholdText.Format(" below %.0lf", m_dThreshold);
		}
		break;

	case nACCUMULATE_LOWSPEND:
		if (m_dThreshold > 0.0)
		{
			m_strThresholdText.Format(" below %.*lf", System.GetDPValue(), m_dThreshold);
		}
		break;

	case nACCUMULATE_RANKPOINTS:
		m_strThresholdText.Format(" above %.0lf", m_dThreshold);
		break;

	case nACCUMULATE_RANKSPEND:
		m_strThresholdText.Format(" above %.*lf", System.GetDPValue(), m_dThreshold);
		break;

	case nACCUMULATE_ACTIVE:
	case nACCUMULATE_INACTIVE:
		break;

	case nACCUMULATE_RANKPURCHASES:
		switch (m_nRankByType)
		{
		case 0:
			m_strThresholdText = " by Quantity";
			break;

		case 1:	
			m_strThresholdText = " by Value";
			break;

		case 2:	
			m_strThresholdText = " by Points";	
			break;
		}
		break;
	}

	m_strThresholdText += GetDateRangeString();		// add date range

	if (TRUE == m_bTimeRange)
	{
		m_strThresholdText += " ";;
		m_strThresholdText += GetTimeRangeString();		// add time range if required
	}

	return m_strThresholdText;
}

//$$******************************************************************
// done as an int - easier to switch on than a string

int CReportInfoAccumulator::GetReportType()
{
	int nType = 0;
	
	if (m_strReportLabel == "R003")
	{
		nType = nACCUMULATE_LOWPOINTS;
	}
	else if (m_strReportLabel == "R004")
	{
		nType = nACCUMULATE_LOWSPEND;
	}
	else if (m_strReportLabel == "R005")
	{
		nType = nACCUMULATE_RANKPOINTS;
	}
	else if (m_strReportLabel == "R006")
	{
		nType = nACCUMULATE_RANKSPEND;
	}
	else if (m_strReportLabel == "R007")
	{
		nType = nACCUMULATE_ACTIVE;
	}
	else if (m_strReportLabel == "R008")
	{
		nType = nACCUMULATE_INACTIVE;
	}
	else if (m_strReportLabel == "R106")
	{
		nType = nACCUMULATE_RANKPURCHASES;
	}

	return nType;
}

//$$******************************************************************

const char* CReportInfoAccumulator::GetDateRangeString()
{
	if (m_dateFrom != m_dateTo)
	{
		m_strDateRange.Format(" (%s - %s)", m_dateFrom.GetDate(), m_dateTo.GetDate());
	}
	else
	{
		m_strDateRange.Format(" (%s)", m_dateFrom.GetDate());
	}

	return m_strDateRange;
}

//$$******************************************************************

const char* CReportInfoAccumulator::GetTimeRangeString()
{
	if (TRUE == m_bTimeRange)
	{
		m_strTimeRange.Format(" (%s - %s)", m_timeFrom.GetTime(), m_timeTo.GetTime());
	}
	else
	{
		m_strTimeRange = "";
	}

	return m_strTimeRange;
}

//$$******************************************************************

const char* CReportInfoAccumulator::GetCaption()
{
	m_strCaption = "";

	switch (GetReportType())
	{
	case nACCUMULATE_LOWPOINTS:
		m_strCaption = "Low Points Usage";
		break;

	case nACCUMULATE_LOWSPEND:
		m_strCaption = "Low Spend Usage";
		break;

	case nACCUMULATE_RANKPOINTS:
		m_strCaption = "Points Ranking";
		break;

	case nACCUMULATE_RANKSPEND:
		m_strCaption = "Spend Ranking";
		break;

	case nACCUMULATE_ACTIVE:
		m_strCaption = "Active Cards";
		break;

	case nACCUMULATE_INACTIVE:
		m_strCaption = "Inactive Cards";
		break;

	case nACCUMULATE_RANKPURCHASES:
		m_strCaption = "Purchase Ranking";
		break;
	}

	return m_strCaption;
}

//$$******************************************************************

const char* CReportInfoAccumulator::GetReportTitle()
{
	m_strReportTitle = "";
	m_strReportTitle += GetCaption();
	m_strReportTitle += GetThresholdText();
	return m_strReportTitle;
}

//*******************************************************************

bool CReportInfoAccumulator::IsValid(CDbExportHandler* pExport, CAuditRecord* atcRecord)
{
	CString strDate = "";
	CString strTime = "";

	switch (atcRecord->GetSourceType())
	{
	case AUDIT_POS:
	case AUDIT_EXT:
		strDate = atcRecord->GetDateLastUsed();
		strTime = atcRecord->GetTimeLastUsed();
		break;

	default:
		strDate = atcRecord->GetDateFiled();
		strTime = atcRecord->GetTimeFiled();
		break;
	}

	CSSDate dateAudit(strDate);
	if (dateAudit < m_dateFrom || dateAudit > m_dateTo)
	{
		return FALSE;
	}

	if (TRUE == m_bTimeRange)
	{
		CSSTime timeAudit(strTime);
		if (timeAudit < m_timeFrom || timeAudit > m_timeTo)
		{
			return FALSE;
		}
	}

	if (pExport->IsCardInRange(_atoi64(atcRecord->GetCardNo())) == FALSE)
	{
		return FALSE;
	}

	bool bReply = FALSE;
	switch (GetReportType())
	{
	case nACCUMULATE_LOWPOINTS:
	case nACCUMULATE_RANKPOINTS:
		bReply = IsValidPoints(atcRecord);
		break;

	case nACCUMULATE_LOWSPEND:
	case nACCUMULATE_RANKSPEND:
		bReply = IsValidSpend(atcRecord);
		break;

	case nACCUMULATE_ACTIVE:
	case nACCUMULATE_INACTIVE:
		bReply = IsValidUsed(atcRecord);
		break;
	}

	return bReply;
}

//$$******************************************************************

bool CReportInfoAccumulator::IsValidUsed ( CAuditRecord* atcRecord  )
{
	if (IsValidPoints(atcRecord) == FALSE)
	{
		return IsValidSpend(atcRecord);
	}

	return TRUE;
}

//$$******************************************************************

bool CReportInfoAccumulator::IsValidPoints(CAuditRecord* atcRecord)
{
	bool bOK = FALSE;

	switch (atcRecord->GetApplicationNo())
	{
	case APPNO_REDEEM:
	case APPNO_BATCHREDEEM:
		break;

	default:
		if (atcRecord->GetPointsTransaction() != 0)
		{
			bOK = TRUE;
		}
		break;
	}

	return bOK;
}

//$$******************************************************************
 
bool CReportInfoAccumulator::IsValidSpend ( CAuditRecord* atcRecord  )
{
	bool bOK = FALSE;

	switch (atcRecord->GetApplicationNo())
	{
	case APPNO_SALE:
	case APPNO_MANUAL_SPEND:
		bOK = (atcRecord->GetTotalTransaction() != 0.0);
		break;
	}
	return bOK;
}

//$$******************************************************************

bool CReportInfoAccumulator::CheckThreshold ( double dValue )
{
	bool bReply = FALSE;

	switch (GetReportType() )
	{
	case nACCUMULATE_RANKPOINTS:
	case nACCUMULATE_RANKSPEND:
		if (dValue > m_dThreshold)
		{
			bReply = TRUE;
		}						
		break;

	case nACCUMULATE_ACTIVE:
		if (dValue > 0.0)
		{
			bReply = TRUE;
		}
		break;

	case nACCUMULATE_INACTIVE:
		if (dValue == 0.0)
		{
			bReply = TRUE;
		}
		break;

	case nACCUMULATE_LOWPOINTS:
	case nACCUMULATE_LOWSPEND:	
		if (dValue < m_dThreshold)
		{
			bReply = TRUE;
		}					
		break;
	}

	return bReply;
}

//$$******************************************************************

void CReportInfoAccumulator::SetDbNo(int n)
{
	m_nDbNo = n;
	m_TerminalListHandler.SetTargetDbNo(n);
}

//$$******************************************************************

void CReportInfoAccumulator::SetFolderSet(int n)
{
	m_nFolderSet = n;
	m_TerminalListHandler.SetTargetFolderSet(n);
}

//$$******************************************************************

void CReportInfoAccumulator::SetTerminalListType(int n)
{
	m_nTerminalListType = n;
	m_TerminalListHandler.SetTerminalSelectType(n);
}

//$$******************************************************************
