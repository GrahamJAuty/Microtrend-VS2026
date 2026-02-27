//$$******************************************************************
#include "..\SmartPay4Shared\Defines.h"
//$$******************************************************************
#include "PurchaseControlReportConfig.h"
//$$******************************************************************

CPurchaseControlReportConfig::CPurchaseControlReportConfig(CString strLabel) : m_TerminalListHandler(Filenames.GetTerminalListFilename())
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
	SetShowAutoDeleteFlag(TRUE);
	SetShowManualAddFlag(TRUE);
	SetShowManualEditFlag(TRUE);
	SetShowManualDeleteFlag(TRUE);
	SetShowHotlistFlag(TRUE);
	SetShowTransferFlag(TRUE);
	SetSingleRuleNo(0);

	SetSearchType(0);
	SetSearchText("");
	SetSearchCardNo("");

	SetTimeRangeFlag(FALSE);
	SetTimeSelection(0);
	SetDateRangeType(szRANGE_TODAY);
	m_dateFrom.SetCurrentDate();
	m_dateTo.SetCurrentDate();

	SetFolderSet(0);
	SetTerminalListType(0);

	SetLabelOptions();
}

//$$******************************************************************

void CPurchaseControlReportConfig::UpdateInt64CardNo(bool bSingle)
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

CString CPurchaseControlReportConfig::GetDateString()
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

CString CPurchaseControlReportConfig::GetDateFromString()
{
	m_strDateFrom = m_dateFrom.GetDate();
	return m_strDateFrom;
}

//$$******************************************************************

CString CPurchaseControlReportConfig::GetDateToString()
{
	m_strDateTo = m_dateTo.GetDate();
	return m_strDateTo;
}

//$$******************************************************************

CString CPurchaseControlReportConfig::GetTimeFromString()
{
	m_strTimeFrom = m_timeFrom.GetTime();
	return m_strTimeFrom;
}

//$$******************************************************************

CString CPurchaseControlReportConfig::GetTimeToString()
{
	m_strTimeTo = m_timeTo.GetTime();
	return m_strTimeTo;
}

//$$******************************************************************

void CPurchaseControlReportConfig::SetLabelOptions()
{
	if ((m_strReportLabel.GetLength() == 4) && (m_strReportLabel.Left(1) == "R"))
	{
		switch (atoi(m_strReportLabel.Right(3)))
		{
		case 142:
			SetReportTitle("Purchase Control Log Report");
			SetReportType(nPURCHASECONTROL_REPORT);
			break;

		case 143:
			SetReportTitle("Purchase Control Log Report");				// single account
			SetReportType(nPURCHASECONTROL_REPORT);
			SetReportRangeType(nREPORT_SINGLE);			// set single report only
			break;

		default:
			SetReportTitle("Purchase Control Log Report");
			SetReportType(nPURCHASECONTROL_REPORT);
			break;
		}
	}
	else
	{
		SetReportTitle("Purchase Control Log Report");
		SetReportType(nPURCHASECONTROL_REPORT);
	}
}

//$$******************************************************************

void CPurchaseControlReportConfig::SetFolderSet(int n)
{
	m_nFolderSet = n;
	m_TerminalListHandler.SetTargetFolderSet(n);
}

//$$******************************************************************

void CPurchaseControlReportConfig::SetTerminalListType(int n)
{
	m_nTerminalListType = n;
	m_TerminalListHandler.SetTerminalSelectType(n);
}

//$$******************************************************************