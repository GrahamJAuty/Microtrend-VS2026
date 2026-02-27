//$$******************************************************************
#include "GroupShiftReportConfig.h"
//$$******************************************************************

CGroupShiftReportConfig::CGroupShiftReportConfig(CString strLabel)
{
	m_strReportLabel = strLabel;
	m_strAuditFilename = Filenames.GetGroupShiftLogFilename();
	m_strReportFilename = Filenames.GetReportFilename(m_strReportLabel);
	m_strReportCardListFilename = Filenames.GetReportCardListFilename(m_strReportLabel);
	m_strParamsFilename = Filenames.GetReportParamsFilename(m_strReportLabel);
	m_strReportKey = "";

	SetSingleCardFlag(FALSE);
	SetStringCardNoFrom(System.GetLowCardNo());
	SetStringCardNoTo(System.GetHighCardNo());
	UpdateInt64CardNo(FALSE);
	SetSortByCardNoFlag(FALSE);

	SetReportRangeType(nREPORT_RANGE);
	SetNewPageFlag(FALSE);
	SetSingleGroupFlag(FALSE);
	SetSingleGroupNo(1);

	SetSearchType(0);
	SetSearchText("");
	SetSearchCardNo("");

	SetTimeRangeFlag(FALSE);
	SetTimeSelection(0);
	SetDateRangeType(szRANGE_TODAY);
	m_dateFrom.SetCurrentDate();
	m_dateTo.SetCurrentDate();
	SetContinuousFlag(FALSE);

	SetShowDeletedLinesFlag(FALSE);
	SetShowSourceManagerFlag(TRUE);
	SetShowSourceServerFlag(TRUE);
	SetShowSourceBackgroundFlag(TRUE);
	SetShowSourceManualFlag(TRUE);
	SetShowPreviousFlag(TRUE);

	SetReportTitle("Group Shift Log");
	SetReportType(nGROUPSHIFT_REPORT);

	SetLabelOptions();
}

//$$******************************************************************

void CGroupShiftReportConfig::UpdateInt64CardNo(bool bSingle)
{
	m_nToCardNo = _atoi64(m_strToCardNo);

	if (FALSE == bSingle)
	{
		m_nFromCardNo = _atoi64(m_strFromCardNo);
	}
	else
	{
		m_nFromCardNo = m_nToCardNo;
	}
}

//$$******************************************************************

CString CGroupShiftReportConfig::GetDateString()
{
	CString strBuf;

	if (m_dateFrom != m_dateTo)
	{
		strBuf.Format(" (%s - %s)", m_dateFrom.GetDate(), m_dateTo.GetDate());
	}
	else
	{
		strBuf.Format(" (%s)", m_dateFrom.GetDate());			/* start date */
	}

	return strBuf;
}

//$$******************************************************************

CString CGroupShiftReportConfig::GetDateFromString()
{
	m_strDateFrom = m_dateFrom.GetDate();
	return m_strDateFrom;
}

//$$******************************************************************

CString CGroupShiftReportConfig::GetDateToString()
{
	m_strDateTo = m_dateTo.GetDate();
	return m_strDateTo;
}

//$$******************************************************************

CString CGroupShiftReportConfig::GetTimeFromString()
{
	m_strTimeFrom = m_timeFrom.GetTime();
	return m_strTimeFrom;
}

//$$******************************************************************

CString CGroupShiftReportConfig::GetTimeToString()
{
	m_strTimeTo = m_timeTo.GetTime();
	return m_strTimeTo;
}

//$$******************************************************************

void CGroupShiftReportConfig::SetLabelOptions()
{
	if ((m_strReportLabel.GetLength() == 4) && (m_strReportLabel.Left(1) == "R"))
	{
		switch (atoi(m_strReportLabel.Right(3)))
		{
		case 141:
			SetReportRangeType(nREPORT_SINGLE);
			break;

		case 140:
		default:
			SetReportRangeType(nREPORT_RANGE); 
			break;
		}
	}
	else
	{
		SetReportRangeType(nREPORT_RANGE);
	}
}

//$$******************************************************************
