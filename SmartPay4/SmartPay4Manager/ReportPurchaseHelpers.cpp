//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//**********************************************************************
#include "ReportPurchaseHelpers.h"
//**********************************************************************

CReportPurchaseHelpers::CReportPurchaseHelpers() : m_TerminalListHandler(Filenames.GetTerminalListFilename())
{
	m_strReportLabel = "";
	m_bSelectivePlu = FALSE;
	m_strSelectivePluText = "";
	m_dateFrom.SetCurrentDate();
	m_dateTo.SetCurrentDate();
	m_strFromCardNo = System.GetLowCardNo();
	m_strToCardNo = System.GetHighCardNo();
	m_strInfoTextFilter = "";
	m_nInfoTextIndex = 0;
	m_nPurchaseReportType = nPURCHASE_ITEMISED;
	m_bTimeRangeSet = FALSE;
	m_nReportRangeType = nREPORT_RANGE;
	m_nRangeFromCardNo = 0;
	m_nRangeToCardNo = 0;
	m_bGroupList = FALSE;
	m_bSortByGroup = TRUE;
	m_nSingleGroupNo = 0;
	m_nGroupComboIndex = 0;
	m_nNewPageFlags = 0;
	m_nExcludeZeroPriceFlags = 0;
	m_bContinuous = FALSE;
	m_bShowAccountName = TRUE;
	m_bSkipOnBlank = TRUE;
	SetFolderSet(0);
	SetTerminalListType(0);
}

//**********************************************************************

CString CReportPurchaseHelpers::GetReportLabelExternal()
{
	CString strLabel = m_strReportLabel;

	if ( ( IsPurchaseHistoryReport() == TRUE) || ( IsPurchaseBreakdownReport() == TRUE ) )
	{
		//if (TRUE == m_bSelectivePlu)
		{
			strLabel += "a";									// R104 := R104a / R105 := R105a if selective plu reqd
		}
	}

	return strLabel;
}

/**********************************************************************/

CString CReportPurchaseHelpers::GetWindowTitle()
{
	CString strTitle = "";

	if (IsStatementReport() == TRUE)
	{
		strTitle = "Account Statement";
	}
	else if (IsPurchaseBreakdownReport() == TRUE)
	{
		strTitle = "Purchase Breakdown";
	}
	else
	{
		strTitle = "Purchase History";
	}

	return strTitle;
}

/**********************************************************************/

CString CReportPurchaseHelpers::GetPurchaseReportTitle()
{
	CString strReportTitle = GetWindowTitle();

	if ( nPURCHASE_ACCOUNTITEM == m_nPurchaseReportType)
	{
		strReportTitle += " Summary";
	}

	CString strBuf = "";

	if (m_nInfoTextIndex > 0)										// 0=don't care, info1-8
	{
		strBuf.Format(" %s",
			(const char*)m_strInfoTextFilter);

		strReportTitle += strBuf;
	}

	if ( m_dateFrom != m_dateTo)
	{
		strBuf.Format(" (%s - %s)",
			m_dateFrom.GetDate(),
			m_dateTo.GetDate());
	}
	else
	{
		strBuf.Format(" (%s)",
			m_dateFrom.GetDate());			/* start date */
	}

	strReportTitle += strBuf;

	if ( TRUE == m_bTimeRangeSet )
	{
		strBuf.Format(" (%s - %s)",
			m_timeFrom.GetTime(),
			m_timeTo.GetTime());

		strReportTitle += strBuf;
	}

	return strReportTitle;
}

/**********************************************************************/

CString CReportPurchaseHelpers::GetBreakdownReportTitle()
{
	CString strBuf = "";
	CString strReportTitle = GetWindowTitle();

	CString strOrderBy = "";
	strBuf = GetOrderByLabel();
	if (strBuf != "")
	{
		strOrderBy.Format(" Ordered by %s",
			(const char*)strBuf);
	}

	if (m_dateFrom != m_dateTo)
	{
		strBuf.Format("%s (%s - %s)",
			(const char*)strOrderBy,
			m_dateFrom.GetDate(),
			m_dateTo.GetDate());
	}
	else
	{
		strBuf.Format("%s (%s)",
			(const char*)strOrderBy,
			m_dateFrom.GetDate());			/* start date */
	}

	strReportTitle += strBuf;

	if (m_bTimeRangeSet == TRUE)
	{
		strBuf.Format(" (%s - %s)", m_timeFrom.GetTime(), m_timeTo.GetTime());
		strReportTitle += strBuf;
	}

	if ((TNoListGetSize() != 0 ) && (GetTerminalListType() == 1))
	{
		CCSV csv;
		for (int i = 0; i < TNoListGetSize(); i++)
		{
			csv.Add(TNoListGetItem(i));
		}

		strBuf.Format(" TNo: %s",
			(const char*)csv.GetLine());

		strReportTitle += strBuf;
	}

	return strReportTitle;
}

//*******************************************************************

void CReportPurchaseHelpers::GetSQLSearchInfoPurchase(CPurchaseHistorySearchInfo& infoSearch, __int64 nUserID)
{
	infoSearch.m_nUserIDFrom = m_nRangeFromCardNo;
	infoSearch.m_nUserIDTo = m_nRangeToCardNo;
	infoSearch.m_nUserIDExact = nUserID;

	if (m_nGroupComboIndex > 0)
	{
		infoSearch.m_bMatchGroup = TRUE;
		infoSearch.m_nGroupNo = m_nSingleGroupNo;
	}
	else
	{
		infoSearch.m_bMatchGroup = FALSE;
		infoSearch.m_nGroupNo = 0;
	}

	infoSearch.m_strFilterText = ::RemoveSpaces(m_strInfoTextFilter);
	infoSearch.m_nInfoTextIndex = m_nInfoTextIndex;
}

//*******************************************************************

void CReportPurchaseHelpers::GetSQLSearchInfoBreakdown(CPurchaseHistorySearchInfo& infoSearch, __int64 nUserID)
{
	infoSearch.m_nUserIDFrom = m_nRangeFromCardNo;
	infoSearch.m_nUserIDTo = m_nRangeToCardNo;
	infoSearch.m_nUserIDExact = nUserID;
	infoSearch.m_bMatchGroup = FALSE;
	infoSearch.m_strFilterText = "";
}

//*******************************************************************

void CReportPurchaseHelpers::SetNewPageFlag(int nReportNo, bool bFlag)
{
	unsigned int nMask = 0x01;
	nMask = nMask << nReportNo;

	if (TRUE == bFlag)
	{
		m_nNewPageFlags |= nMask;
	}
	else
	{
		m_nNewPageFlags &= ~nMask;
	}
}

/**********************************************************************/

bool CReportPurchaseHelpers::GetNewPageFlag(int nReportNo)
{
	if (nReportNo == -1)						// see if report type passed
	{
		nReportNo = m_nPurchaseReportType;		// no - used current setting
	}

	unsigned int nMask = 0x01;
	nMask = nMask << nReportNo;

	return ((m_nNewPageFlags & nMask) == nMask) ? TRUE : FALSE;
}

/**********************************************************************/

void CReportPurchaseHelpers::SetExcludeZeroPriceFlag(int nReportNo, bool bFlag)
{
	unsigned int nMask = 0x01;
	nMask = nMask << nReportNo;

	if (TRUE == bFlag)
	{
		m_nExcludeZeroPriceFlags |= nMask;
	}
	else
	{
		m_nExcludeZeroPriceFlags &= ~nMask;
	}
}

/**********************************************************************/

bool CReportPurchaseHelpers::GetExcludeZeroPriceFlag(int nReportNo)
{
	if (nReportNo == -1)						// see if report type passed
	{
		nReportNo = m_nPurchaseReportType;		// no - used current setting
	}

	unsigned int nMask = 0x01;
	nMask = nMask << nReportNo;

	return ((m_nExcludeZeroPriceFlags & nMask) == nMask) ? TRUE : FALSE;
}

/**********************************************************************/

bool CReportPurchaseHelpers::CheckUserIDAgainstList(__int64 nUserID)
{
	CString strUserID = System.FormatCardNo(nUserID, TRUE);
	return m_CardPicker.IsValid(strUserID, GetCardListFilename());
}

/**********************************************************************/

CString CReportPurchaseHelpers::GetOrderByLabel()
{
	CString strOrderBy = "";
	if ( m_nInfoTextIndex > 0)
	{
		CSQLRepositoryUserText RepoText;
		strOrderBy = RepoText.GetInfoText(m_nInfoTextIndex);
	}

	return strOrderBy;
}

/**********************************************************************/

bool CReportPurchaseHelpers::NonSQLFilterBreakdown(CSQLRowAccountFull& RowAccount)
{
	if (TRUE == m_bGroupList)
	{
		return m_GroupPicker.IsValid(RowAccount.GetGroupNo());
	}
	else
	{
		return TRUE;
	}
}

//*******************************************************************

CString CReportPurchaseHelpers::GetReportFilename()
{
	CString strReportName = Filenames.GetReportFilename(GetReportLabelExternal());
	return strReportName;
}

/**********************************************************************/

CString CReportPurchaseHelpers::GetParamsFilename()
{
	CString strParamsName = Filenames.GetReportParamsFilename(GetReportLabelExternal());
	return strParamsName;
}

/**********************************************************************/

CString CReportPurchaseHelpers::GetCardListFilename()
{
	return Filenames.GetReportCardListFilename(m_strReportLabel);
}

/**********************************************************************/

CString CReportPurchaseHelpers::GetGroupPickerFilename()
{
	return Filenames.GetReportCardListFilename(GetReportLabelInternal(), "a");		// R136a.lst
}

/**********************************************************************/

bool CReportPurchaseHelpers::IsStatementReport()
{
	return (IsStatementReportAll() || IsStatementReportSingle());
}

/**********************************************************************/

bool CReportPurchaseHelpers::IsStatementReportAll()
{
	return (m_strReportLabel == "R121");
}

/**********************************************************************/

bool CReportPurchaseHelpers::IsStatementReportSingle()
{
	return (m_strReportLabel == "R122");
}

/**********************************************************************/

bool CReportPurchaseHelpers::IsPurchaseHistoryReport()
{
	return (IsPurchaseHistoryReportAll() || IsPurchaseHistoryReportSingle());
}

/**********************************************************************/

bool CReportPurchaseHelpers::IsPurchaseHistoryReportAll()
{
	return (m_strReportLabel == "R104");
}

/**********************************************************************/

bool CReportPurchaseHelpers::IsPurchaseHistoryReportSingle()
{
	return (m_strReportLabel == "R105");
}

/**********************************************************************/

bool CReportPurchaseHelpers::IsSingleAccountReport()
{
	return (IsPurchaseHistoryReportSingle() || IsStatementReportSingle());
}

/**********************************************************************/

bool CReportPurchaseHelpers::IsPurchaseBreakdownReport()
{
	return (m_strReportLabel == "R136");
}

/**********************************************************************/

void CReportPurchaseHelpers::SetFolderSet(int n)
{
	m_nFolderSet = n;
	m_TerminalListHandler.SetTargetFolderSet(n);
}

//$$******************************************************************

void CReportPurchaseHelpers::SetTerminalListType(int n)
{
	m_nTerminalListType = n;
	m_TerminalListHandler.SetTerminalSelectType(n);
}

//$$******************************************************************