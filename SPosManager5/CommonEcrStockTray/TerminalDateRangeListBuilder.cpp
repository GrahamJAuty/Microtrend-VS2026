/**********************************************************************/
#include "LocationCSVArray.h"
/**********************************************************************/
#include "TerminalDateRangeListBuilder.h"
/**********************************************************************/

CTerminalDateRangeListBuilder::CTerminalDateRangeListBuilder(CEposSelectArray& SelectArray, bool bEODMode, int nTermDateType, bool bGuestToTerminal, int nPMSAllowMode, int nPMSDateMode) : m_SelectArray(SelectArray)
{
	m_bEODMode = bEODMode;
	m_nTermDateType = nTermDateType;
	m_strGuestAccountStatus = "";
	m_bGuestToTerminal = bGuestToTerminal;
	m_nPMSAllowMode = nPMSAllowMode;
	m_nPMSDateMode = nPMSDateMode;
}

/**********************************************************************/

CString CTerminalDateRangeListBuilder::GetLine(int n)
{
	CString strResult = "";

	if ((n >= 0) && (n < m_arrayLines.GetSize()))
	{
		strResult = m_arrayLines.GetAt(n);
	}

	return strResult;
}

/**********************************************************************/

bool CTerminalDateRangeListBuilder::SetGuestAccountStatus()
{
	m_strGuestAccountStatus = "";

	if (TRUE == m_bGuestToTerminal)
	{
		m_strGuestAccountStatus = "<..>(Guest account sales have been assigned to EPOS terminals)";
	}
	else
	{
		if (TRUE == m_bEODMode)
		{
			m_strGuestAccountStatus = "<..>(Guest account sales cannot be viewed in EOD mode)";
		}
		else
		{
			switch (m_nPMSAllowMode)
			{
			case PMS_ALLOWMODE_NONE:
				m_strGuestAccountStatus = "<..>(This report is not available for guest accounts)";
				break;

			case PMS_ALLOWMODE_ACCOUNT:
				if (PMS_DATEMODE_ACTUAL == m_nPMSDateMode)
				{
					m_strGuestAccountStatus = "<..>(This report is not available for guest accounts by actual transaction time)";
				}
				break;
			}
		}
	}

	return (m_strGuestAccountStatus != "");
}

/**********************************************************************/

void CTerminalDateRangeListBuilder::CreateTerminalList()
{
	CString strGuestAccountStatus = "";
	m_SelectArray.BuildLocSortArray();

	if (FALSE == m_bEODMode)
	{
		switch (m_nTermDateType)
		{
		case 2:
			CreateTerminalListSystem();
			break;

		case 1:
			CreateTerminalListLocation();
			break;

		case 0:
		default:
			CreateTerminalListTerminal();
			break;
		}
	}
	else
	{
		switch (m_nTermDateType)
		{
		case 2:
			CreateTerminalListEODSystem();
			break;

		case 1:
			CreateTerminalListEODLocation();
			break;

		case 0:
		default:
			CreateTerminalListEODTerminal();
			break;
		}
	}
}

/**********************************************************************/

void CTerminalDateRangeListBuilder::CreateTerminalListTerminal()
{
	for (int n = 0; n < m_SelectArray.GetLocSortArraySize(); n++)
	{
		int nIndex = m_SelectArray.GetLocSortIdx(n);

		CEposSelectArrayListItem ListItem;
		m_SelectArray.GetListItem(nIndex, ListItem);

		if (EPOSSELECT_TERMINAL != ListItem.m_nLineType)
		{
			continue;
		}

		if (dbLocation.IsPMSPseudoLocation(ListItem.m_nLocIdx) == TRUE)
		{
			if (SetGuestAccountStatus() == TRUE)
			{
				continue;
			}
		}

		CString strLine;
		bool bHideNoData = FALSE;
		strLine.Format("<..>%s, %s %s",
			dbLocation.GetName(ListItem.m_nLocIdx),
			dbLocation.GetTerminalName(ListItem.m_nLocIdx, ListItem.m_nTermIdx),
			m_SelectArray.GetDateInfo(ListItem, bHideNoData));

		if (dbLocation.IsPMSPseudoLocation(ListItem.m_nLocIdx) == FALSE)
		{
			if (FALSE == bHideNoData)
			{
				m_arrayLines.Add(strLine);
			}
		}
		else
		{
			if (FALSE == bHideNoData)
			{
				m_strGuestAccountStatus = strLine;
			}
			else
			{
				m_strGuestAccountStatus = "";
			}
		}
	}
}

/**********************************************************************/

void CTerminalDateRangeListBuilder::CreateTerminalListLocation()
{
	int nCurrentLocIdx = -1;
	CEposSelectArrayListItem LocationListItem;
	int nTerminals = 0;
	int nWithData = 0;

	for (int n = 0; n < m_SelectArray.GetLocSortArraySize(); n++)
	{
		int nIndex = m_SelectArray.GetLocSortIdx(n);

		CEposSelectArrayListItem ListItem;
		m_SelectArray.GetListItem(nIndex, ListItem);

		if (EPOSSELECT_TERMINAL != ListItem.m_nLineType)
		{
			continue;
		}

		if (dbLocation.IsPMSPseudoLocation(ListItem.m_nLocIdx) == TRUE)
		{
			if (SetGuestAccountStatus() == TRUE)
			{
				continue;
			}
		}

		if (ListItem.m_nLocIdx != nCurrentLocIdx)
		{
			if (nCurrentLocIdx != -1)
			{
				ProcessEposDetailsLocationLine(LocationListItem, nTerminals, nWithData, FALSE);
			}

			nCurrentLocIdx = ListItem.m_nLocIdx;
			LocationListItem = ListItem;
			nTerminals = 0;
			nWithData = 0;
		}

		m_SelectArray.MergeDateInfo(ListItem, LocationListItem, nTerminals, nWithData);
	}

	if (nCurrentLocIdx != -1)
	{
		ProcessEposDetailsLocationLine(LocationListItem, nTerminals, nWithData, FALSE);
	}
}

/**********************************************************************/

void CTerminalDateRangeListBuilder::ProcessEposDetailsLocationLine(CEposSelectArrayListItem& LocationListItem, int nTerminals, int nWithData, bool bEOD)
{
	CString strTerminals = "";
	if (nTerminals == 1)
	{
		strTerminals = "1 terminal";
	}
	else
	{
		strTerminals.Format("%d terminals",
			nTerminals);
	}

	CString strWithData = "";
	if ((nWithData > 0) && (nWithData != nTerminals))
	{
		strWithData.Format(" (%d with data)",
			nWithData);
	}

	CString strLine = "";
	bool bHideNoData = FALSE;

	if (FALSE == bEOD)
	{
		strLine.Format("<..>%s, %s%s %s",
			dbLocation.GetName(LocationListItem.m_nLocIdx),
			(const char*)strTerminals,
			(const char*)strWithData,
			m_SelectArray.GetDateInfo(LocationListItem, bHideNoData));
	}
	else
	{
		strLine.Format("<..>==> %s%s %s",
			(const char*)strTerminals,
			(const char*)strWithData,
			m_SelectArray.GetDateInfo(LocationListItem, bHideNoData));
	}

	if (dbLocation.IsPMSPseudoLocation(LocationListItem.m_nLocIdx) == FALSE)
	{
		if (FALSE == bHideNoData)
		{
			m_arrayLines.Add(strLine);
		}
	}
	else
	{
		if (FALSE == bHideNoData)
		{
			m_strGuestAccountStatus = strLine;
		}
		else
		{
			m_strGuestAccountStatus = "";
		}
	}
}

/**********************************************************************/

void CTerminalDateRangeListBuilder::CreateTerminalListSystem()
{
	CEposSelectArrayListItem SystemListItem;
	int nTerminals = 0;
	int nWithData = 0;

	for (int n = 0; n < m_SelectArray.GetLocSortArraySize(); n++)
	{
		int nIndex = m_SelectArray.GetLocSortIdx(n);

		CEposSelectArrayListItem ListItem;
		m_SelectArray.GetListItem(nIndex, ListItem);

		if (EPOSSELECT_TERMINAL != ListItem.m_nLineType)
		{
			continue;
		}

		if (dbLocation.IsPMSPseudoLocation(ListItem.m_nLocIdx) == TRUE)
		{
			if (SetGuestAccountStatus() == TRUE)
			{
				continue;
			}
		}

		m_SelectArray.MergeDateInfo(ListItem, SystemListItem, nTerminals, nWithData);
	}

	CString strLine = GetSystemTerminalSummaryString(SystemListItem, nTerminals, nWithData);
	if (strLine != "")
	{
		m_arrayLines.Add("");
		m_arrayLines.Add(strLine);
	}
}

/**********************************************************************/

void CTerminalDateRangeListBuilder::CreateTerminalListEODTerminal()
{
	bool bGotEOD = FALSE;
	int nCurrentLocIdx = -1;

	for (int n = 0; n < m_SelectArray.GetLocSortArraySize(); n++)
	{
		int nIndex = m_SelectArray.GetLocSortIdx(n);

		CEposSelectArrayListItem ListItem;
		m_SelectArray.GetListItem(nIndex, ListItem);

		if (EPOSSELECT_TERMINAL != ListItem.m_nLineType)
		{
			continue;
		}

		if (dbLocation.IsPMSPseudoLocation(ListItem.m_nLocIdx) == TRUE)
		{
			SetGuestAccountStatus();
			bGotEOD = FALSE;
			nCurrentLocIdx = -1;
		}
		else if (ListItem.m_nLocIdx != nCurrentLocIdx)
		{
			CString strDateInfo = "";
			if (DataManagerNonDb.EODDateTimeFilterArray.GetLastResult(ListItem.m_nLocIdx) == FALSE)
			{
				strDateInfo = ("(NO EOD FOUND)");
				bGotEOD = FALSE;
			}
			else
			{
				CSortedDateTimeItem DateTimeStart = DataManagerNonDb.EODDateTimeFilterArray.GetCheckTimeFrom(ListItem.m_nLocIdx);
				CSortedDateTimeItem DateTimeEnd = DataManagerNonDb.EODDateTimeFilterArray.GetCheckTimeTo(ListItem.m_nLocIdx);
				strDateInfo = GetEODDateRangeString(DateTimeStart, DateTimeEnd);
				bGotEOD = TRUE;
			}

			CString strLine;
			strLine.Format("<..>%s %s",
				dbLocation.GetName(ListItem.m_nLocIdx),
				(const char*)strDateInfo);

			m_arrayLines.Add("");
			m_arrayLines.Add(strLine);

			nCurrentLocIdx = ListItem.m_nLocIdx;
		}

		if (TRUE == bGotEOD)
		{
			CString strLine = "";
			bool bHideNoData = FALSE;
			strLine.Format("<..>==> %s %s",
				dbLocation.GetTerminalName(ListItem.m_nLocIdx, ListItem.m_nTermIdx),
				m_SelectArray.GetDateInfo(ListItem, bHideNoData));

			if (FALSE == bHideNoData)
			{
				m_arrayLines.Add(strLine);
			}
		}
	}
}

/**********************************************************************/

void CTerminalDateRangeListBuilder::CreateTerminalListEODLocation()
{
	bool bGotEOD = FALSE;
	int nCurrentLocIdx = -1;
	CEposSelectArrayListItem LocationListItem;
	int nTerminals = 0;
	int nWithData = 0;

	for (int n = 0; n < m_SelectArray.GetLocSortArraySize(); n++)
	{
		int nIndex = m_SelectArray.GetLocSortIdx(n);

		CEposSelectArrayListItem ListItem;
		m_SelectArray.GetListItem(nIndex, ListItem);

		if (EPOSSELECT_TERMINAL != ListItem.m_nLineType)
		{
			continue;
		}

		if (dbLocation.IsPMSPseudoLocation(ListItem.m_nLocIdx) == TRUE)
		{
			SetGuestAccountStatus();
			bGotEOD = FALSE;
			nCurrentLocIdx = -1;
		}
		else if (ListItem.m_nLocIdx != nCurrentLocIdx)
		{
			if ((nCurrentLocIdx != -1) && (TRUE == bGotEOD))
			{
				ProcessEposDetailsLocationLine(LocationListItem, nTerminals, nWithData, TRUE);
			}

			CString strDateInfo = "";
			if (DataManagerNonDb.EODDateTimeFilterArray.GetLastResult(ListItem.m_nLocIdx) == FALSE)
			{
				strDateInfo = ("(NO EOD FOUND)");
				bGotEOD = FALSE;
			}
			else
			{
				CSortedDateTimeItem DateTimeStart = DataManagerNonDb.EODDateTimeFilterArray.GetCheckTimeFrom(ListItem.m_nLocIdx);
				CSortedDateTimeItem DateTimeEnd = DataManagerNonDb.EODDateTimeFilterArray.GetCheckTimeTo(ListItem.m_nLocIdx);
				strDateInfo = GetEODDateRangeString(DateTimeStart, DateTimeEnd);
				bGotEOD = TRUE;
			}

			CString strLine = "";
			strLine.Format("<..>%s %s",
				dbLocation.GetName(ListItem.m_nLocIdx),
				(const char*)strDateInfo);

			m_arrayLines.Add("");
			m_arrayLines.Add(strLine);

			nCurrentLocIdx = ListItem.m_nLocIdx;
			nTerminals = 0;
			nWithData = 0;
		}

		if (TRUE == bGotEOD)
		{
			m_SelectArray.MergeDateInfo(ListItem, LocationListItem, nTerminals, nWithData);
		}
	}

	if ((nCurrentLocIdx != -1) && (TRUE == bGotEOD))
	{
		ProcessEposDetailsLocationLine(LocationListItem, nTerminals, nWithData, TRUE);
	}
}

/**********************************************************************/

void CTerminalDateRangeListBuilder::CreateTerminalListEODSystem()
{
	bool bGotEOD = FALSE;
	int nCurrentLocIdx = -1;
	CEposSelectArrayListItem SystemListItem;
	int nTerminals = 0;
	int nWithData = 0;
	bool bFirstLocationLine = TRUE;

	for (int n = 0; n < m_SelectArray.GetLocSortArraySize(); n++)
	{
		int nIndex = m_SelectArray.GetLocSortIdx(n);

		CEposSelectArrayListItem ListItem;
		m_SelectArray.GetListItem(nIndex, ListItem);

		if (EPOSSELECT_TERMINAL != ListItem.m_nLineType)
		{
			continue;
		}

		if (dbLocation.IsPMSPseudoLocation(ListItem.m_nLocIdx) == TRUE)
		{
			SetGuestAccountStatus();
			bGotEOD = FALSE;
			nCurrentLocIdx = -1;
		}
		else if (ListItem.m_nLocIdx != nCurrentLocIdx)
		{
			CString strDateInfo = "";
			if (DataManagerNonDb.EODDateTimeFilterArray.GetLastResult(ListItem.m_nLocIdx) == FALSE)
			{
				strDateInfo = ("(NO EOD FOUND)");
				bGotEOD = FALSE;
			}
			else
			{
				CSortedDateTimeItem DateTimeStart = DataManagerNonDb.EODDateTimeFilterArray.GetCheckTimeFrom(ListItem.m_nLocIdx);
				CSortedDateTimeItem DateTimeEnd = DataManagerNonDb.EODDateTimeFilterArray.GetCheckTimeTo(ListItem.m_nLocIdx);
				strDateInfo = GetEODDateRangeString(DateTimeStart, DateTimeEnd);
				bGotEOD = TRUE;
			}

			CString strLine = "";
			strLine.Format("<..>%s %s",
				dbLocation.GetName(ListItem.m_nLocIdx),
				(const char*)strDateInfo);

			if (TRUE == bFirstLocationLine)
			{
				m_arrayLines.Add("");
				bFirstLocationLine = FALSE;

			}
			m_arrayLines.Add(strLine);

			nCurrentLocIdx = ListItem.m_nLocIdx;
		}

		if (TRUE == bGotEOD)
		{
			m_SelectArray.MergeDateInfo(ListItem, SystemListItem, nTerminals, nWithData);
		}
	}

	CString strLine = GetSystemTerminalSummaryString(SystemListItem, nTerminals, nWithData);
	if (strLine != "")
	{
		m_arrayLines.Add("");
		m_arrayLines.Add(strLine);
	}
}

/**********************************************************************/

CString CTerminalDateRangeListBuilder::GetEODDateRangeString(CSortedDateTimeItem& timeStart, CSortedDateTimeItem& timeEnd)
{
	CString strResult = "";

	strResult.Format("(%2.2d:%2.2d:%2.2d on %2.2d/%2.2d/%4.4d - %2.2d:%2.2d:%2.2d on %2.2d/%2.2d/%4.4d)",
		timeStart.m_oleDateTime.GetHour(),
		timeStart.m_oleDateTime.GetMinute(),
		timeStart.m_oleDateTime.GetSecond(),
		timeStart.m_oleDateTime.GetDay(),
		timeStart.m_oleDateTime.GetMonth(),
		timeStart.m_oleDateTime.GetYear(),
		timeEnd.m_oleDateTime.GetHour(),
		timeEnd.m_oleDateTime.GetMinute(),
		timeEnd.m_oleDateTime.GetSecond(),
		timeEnd.m_oleDateTime.GetDay(),
		timeEnd.m_oleDateTime.GetMonth(),
		timeEnd.m_oleDateTime.GetYear());

	return strResult;
}

/**********************************************************************/

CString CTerminalDateRangeListBuilder::GetSystemTerminalSummaryString(CEposSelectArrayListItem& SystemListItem, int nTerminals, int nWithData)
{
	CString strResult = "";

	if (nTerminals > 0)
	{
		CString strTerminals = "";
		if (nTerminals == 1)
		{
			strTerminals = "1 terminal";
		}
		else
		{
			strTerminals.Format("%d terminals",
				nTerminals);
		}

		CString strWithData = "";
		if ((nWithData > 0) && (nWithData != nTerminals))
		{
			strWithData.Format(" (%d with data)",
				nWithData);
		}

		CString strLine = "";
		bool bHideNoData = FALSE;
		strLine.Format("<..>Transactions from %s%s %s",
			(const char*)strTerminals,
			(const char*)strWithData,
			m_SelectArray.GetDateInfo(SystemListItem, bHideNoData));

		if (FALSE == bHideNoData)
		{
			strResult = strLine;
		}
	}

	return strResult;
}

/**********************************************************************/

