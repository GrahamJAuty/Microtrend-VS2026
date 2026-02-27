//*******************************************************************
#include "AccumulatorReportInfo.h"
//*******************************************************************

CAccumulatorReportInfo::CAccumulatorReportInfo() : m_TerminalListHandler(Filenames.GetTerminalListFilename())
{
	Reset();
}

//*******************************************************************

void CAccumulatorReportInfo::Reset()
{
	m_nRankBy = 0;
	m_bNewPage = FALSE;
	m_nInfoSort = 0;
	m_nTop100 = 100;
	m_dateFrom.SetCurrentDate();
	m_dateTo.SetCurrentDate();	
	m_bTimeRange = FALSE;
	m_strDateString = "";
	m_strTimeString = "";
	m_dThreshold = 0.0;
	m_bNoZeroSkip = FALSE;
	m_bContinuous = FALSE;
	SetFolderSet(0);
	SetTerminalListType(0);
	/*****/
	SetReportLabel("R003");
	m_strReportTitle = "";
	m_strThresholdText = "";
}

//*******************************************************************

CString CAccumulatorReportInfo::GetDateFromString()
{
	m_strDateFrom = m_dateFrom.GetDate();
	return m_strDateFrom;
}

//$$******************************************************************

CString CAccumulatorReportInfo::GetDateToString()
{
	m_strDateTo = m_dateTo.GetDate();
	return m_strDateTo;
}

//$$******************************************************************

void CAccumulatorReportInfo::SetDateFrom(CTime& date)
{
	CTime time(date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0);
	m_dateFrom.SetDate(time);
}

//$$******************************************************************

void CAccumulatorReportInfo::SetDateTo(CTime& date)
{
	CTime time(date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0);
	m_dateTo.SetDate(time);
}

//$$******************************************************************

void CAccumulatorReportInfo::SetTimeFrom(CTime& time)
{
	m_timeFrom.SetTime(time);
}

//$$******************************************************************

void CAccumulatorReportInfo::SetTimeTo(CTime& time)
{
	m_timeTo.SetTime(time);
}

//$$******************************************************************

CString CAccumulatorReportInfo::GetTimeFromString()
{
	m_strTimeFrom = m_timeFrom.GetTime();
	return m_strTimeFrom;
}

//$$******************************************************************

CString CAccumulatorReportInfo::GetTimeToString()
{
	m_strTimeTo = m_timeTo.GetTime();
	return m_strTimeTo;
}

//$$******************************************************************

void CAccumulatorReportInfo::SetReportLabel(CString str)
{
	m_strReportLabel = str;
	m_nReportType = 0;
	m_strCaption = "";

	if (m_strReportLabel == "R003")
	{
		m_nReportType = nACCUMULATE_LOWPOINTS;
		m_strCaption = "Low Points Usage";
	}
	else if (m_strReportLabel == "R004")
	{
		m_nReportType = nACCUMULATE_LOWSPEND;
		m_strCaption = "Low Spend Usage";
	}
	else if (m_strReportLabel == "R005")
	{
		m_nReportType = nACCUMULATE_RANKPOINTS;
		m_strCaption = "Points Ranking";
	}
	else if (m_strReportLabel == "R006")
	{
		m_nReportType = nACCUMULATE_RANKSPEND;
		m_strCaption = "Spend Ranking";
	}
	else if (m_strReportLabel == "R007")
	{
		m_nReportType = nACCUMULATE_ACTIVE;
		m_strCaption = "Active";
	}
	else if (m_strReportLabel == "R008")
	{
		m_nReportType = nACCUMULATE_INACTIVE;
		m_strCaption = "Inactive";
	}
	else if (m_strReportLabel == "R106")
	{
		m_nReportType = nACCUMULATE_RANKPURCHASES;
		m_strCaption = "Purchase Ranking";
	}
}

//*******************************************************************

CString CAccumulatorReportInfo::GetReportTitle()
{
	m_strReportTitle = "";
	m_strReportTitle += GetCaption();
	m_strReportTitle += GetThresholdText();
	return m_strReportTitle;
}

//*******************************************************************

CString CAccumulatorReportInfo::GetThresholdText()
{
	m_strThresholdText = "";

	switch (m_nReportType)
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
			m_strThresholdText.Format(" below %.*lf", 
				System.GetDPValue(),
				m_dThreshold);
		}
		break;

	case nACCUMULATE_RANKPOINTS:
		m_strThresholdText.Format(" above %.0lf", 
			m_dThreshold);
		break;

	case nACCUMULATE_RANKSPEND:
		m_strThresholdText.Format(" above %.*lf", 
			System.GetDPValue(), 
			m_dThreshold);
		break;

	case nACCUMULATE_ACTIVE:
	case nACCUMULATE_INACTIVE:
	case nACCUMULATE_RANKPURCHASES:
		break;
	}

	m_strThresholdText += m_strDateString;			// add date range

	if (TRUE == m_bTimeRange)
	{
		m_strThresholdText += " ";;
		m_strThresholdText += m_strTimeString;		// add time range if required
	}

	return m_strThresholdText;
}

//*********************************************************************

bool CAccumulatorReportInfo::CheckThreshold(double dValue) const
{
	bool bReply = FALSE;

	switch (m_nReportType)
	{
	case nACCUMULATE_RANKPOINTS:
	case nACCUMULATE_RANKSPEND:
		bReply = (dValue > m_dThreshold);
		break;

	case nACCUMULATE_ACTIVE:
		bReply = (dValue > 0.0);
		break;

	case nACCUMULATE_INACTIVE:
		bReply = (dValue == 0.0);
		break;

	case nACCUMULATE_LOWPOINTS:
	case nACCUMULATE_LOWSPEND:
		bReply = (dValue < m_dThreshold);
		break;
	}

	return bReply;
}

//**********************************************************************

bool CAccumulatorReportInfo::IsValid(CDbExportHandler* pExport, CSQLAuditRecord* atcRecord)
{
	CString strDate = "";
	CString strTime = "";

	switch (atcRecord->GetSourceType())
	{
	case AUDIT_POS:
	case AUDIT_KIOSKLINK:
	case AUDIT_VENDING:
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
	if ((dateAudit < m_dateFrom) || (dateAudit > m_dateTo))		/* not in range */
	{
		return FALSE;
	}

	if ( TRUE == m_bTimeRange)
	{
		CSSTime timeAudit(strTime);

		if (FALSE == m_bContinuous)
		{
			if ((timeAudit < m_timeFrom) || (timeAudit > m_timeTo))
			{
				return FALSE;
			}
		}
		else
		{
			if (dateAudit == m_dateFrom)
			{
				if (timeAudit < m_timeFrom)
				{
					return FALSE;
				}
			}

			if (dateAudit == m_dateTo)
			{
				if (timeAudit > m_timeTo)
				{
					return FALSE;
				}
			}
		}
	}

	if (pExport->IsCardInRange(atcRecord->GetUserID()) == FALSE)
	{
		return FALSE;
	}

	if (m_TerminalListHandler.CheckTerminalFilter(atcRecord->GetFolderSet(), atcRecord->GetTerminalNo()) == FALSE)
	{
		return FALSE;
	}

	bool bReply = FALSE;
	switch (m_nReportType)
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

//**********************************************************************

bool CAccumulatorReportInfo::IsValidUsed(CSQLAuditRecord* atcRecord)
{
	return (IsValidPoints(atcRecord) || (atcRecord->GetTransactionTotal() != 0.0));
}

//**********************************************************************

bool CAccumulatorReportInfo::IsValidPoints(CSQLAuditRecord* atcRecord)
{
	bool bOK = FALSE;

	switch (atcRecord->GetApplicationNo())
	{
	case APPNO_REDEEM:
	case APPNO_BATCHREDEEM:
		break;

	default:
		bOK = (atcRecord->GetTransactionPoints() != 0L);
		break;
	}

	return bOK;
}

//**********************************************************************

bool CAccumulatorReportInfo::IsValidSpend(CSQLAuditRecord* atcRecord)
{
	bool bReply = FALSE;

	switch (atcRecord->GetApplicationNo())
	{
	case APPNO_SALE:
	case APPNO_MEAL:
		bReply = (atcRecord->GetTransactionTotal() != 0.0);
		break;
	}

	return bReply;
}

//**********************************************************************

void CAccumulatorReportInfo::SetFolderSet(int n)
{
	m_nFolderSet = n;
	m_TerminalListHandler.SetTargetFolderSet(n);
}

//$$******************************************************************

void CAccumulatorReportInfo::SetTerminalListType(int n)
{
	m_nTerminalListType = n;
	m_TerminalListHandler.SetTerminalSelectType(n);
}

//$$******************************************************************
