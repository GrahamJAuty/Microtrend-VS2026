//$$******************************************************************
#include "StampOfferReportConfig.h"
//$$******************************************************************

CStampOfferReportConfig::CStampOfferReportConfig(CString strLabel) : 
	m_TerminalListHandler(Filenames.GetTerminalListFilename())
{
	m_strReportLabel = strLabel;
	m_strReportFilename = Filenames.GetReportFilename(m_strReportLabel);
	m_strReportCardListFilename = Filenames.GetReportCardListFilename(m_strReportLabel);
	m_strParamsFilename = Filenames.GetReportParamsFilename(m_strReportLabel);
	m_strReportKey = "";

	SetSingleCardFlag(FALSE);
	SetStringCardNoFrom(System.GetLowCardNo());
	SetStringCardNoTo(System.GetHighCardNo());
	UpdateInt64CardNo(FALSE);
	SetSortByCardNoFlag(FALSE);

	SetShowDeletedLinesFlag(FALSE);
	SetReportRangeType(nREPORT_RANGE);
	SetNewPageFlag(FALSE);
	SetSingleGroupFlag(FALSE);
	SetSingleGroupNo(1);
	SetShowEPOSAwardFlag(TRUE);
	SetShowEPOSRedeemFlag(TRUE);
	SetShowManualAddFlag(TRUE);
	SetShowManualEditFlag(TRUE);
	SetShowManualDeleteFlag(TRUE);
	SetShowHotlistFlag(TRUE);
	SetShowTransferFlag(TRUE);
	SetSingleOfferNo(0);

	SetSearchType(0);
	SetSearchText("");
	SetSearchCardNo("");

	SetTimeRangeFlag(FALSE);
	SetTimeSelection(0);
	SetDateRangeType(szRANGE_TODAY);
	m_dateFrom.SetCurrentDate();
	m_dateTo.SetCurrentDate();

	SetDbNo(0);
	SetFolderSet(0);
	SetTerminalListType(0);

	SetLabelOptions();
}

//$$******************************************************************

void CStampOfferReportConfig::UpdateInt64CardNo(bool bSingle)
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

CString CStampOfferReportConfig::GetDateString()
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

CString CStampOfferReportConfig::GetDateFromString()
{
	m_strDateFrom = m_dateFrom.GetDate();
	return m_strDateFrom;
}

//$$******************************************************************

CString CStampOfferReportConfig::GetDateToString()
{
	m_strDateTo = m_dateTo.GetDate();
	return m_strDateTo;
}

//$$******************************************************************

CString CStampOfferReportConfig::GetTimeFromString()
{
	m_strTimeFrom = m_timeFrom.GetTime();
	return m_strTimeFrom;
}

//$$******************************************************************

CString CStampOfferReportConfig::GetTimeToString()
{
	m_strTimeTo = m_timeTo.GetTime();
	return m_strTimeTo;
}

//$$******************************************************************

void CStampOfferReportConfig::SetLabelOptions()
{
	if ((m_strReportLabel.GetLength() == 4) && (m_strReportLabel.Left(1) == "R"))
	{
		switch (atoi(m_strReportLabel.Right(3)))
		{
		case 129:
			SetReportTitle("Stamp Offer Log Report");
			SetReportType(nSTAMPOFFER_REPORT);
			break;

		case 130:
			SetReportTitle("Stamp Offer Log Report");				// single account
			SetReportType(nSTAMPOFFER_REPORT);
			SetReportRangeType(nREPORT_SINGLE);			// set single report only
			break;

		default:
			SetReportTitle("Stamp Offer Log Report");
			SetReportType(nSTAMPOFFER_REPORT);
			break;
		}
	}
	else
	{
		SetReportTitle("Stamp Offer Log Report");
		SetReportType(nSTAMPOFFER_REPORT);
	}
}

//$$******************************************************************

void CStampOfferReportConfig::SetDbNo(int n)
{
	m_nDbNo = n;
	m_TerminalListHandler.SetTargetDbNo(n);
}

//$$******************************************************************

void CStampOfferReportConfig::SetFolderSet(int n)
{
	m_nFolderSet = n;
	m_TerminalListHandler.SetTargetFolderSet(n);
}

//$$******************************************************************

void CStampOfferReportConfig::SetTerminalListType(int n)
{
	m_nTerminalListType = n;
	m_TerminalListHandler.SetTerminalSelectType(n);
}

//$$******************************************************************