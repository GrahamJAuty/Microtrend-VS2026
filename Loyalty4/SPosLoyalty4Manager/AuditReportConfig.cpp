//$$******************************************************************
#include "AuditReportConfig.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\AuditReportTexts.h"
//$$******************************************************************

CAuditReportConfig::CAuditReportConfig(CString strLabel, CString strAuditFilename) :
	m_TerminalListHandler(Filenames.GetTerminalListFilename())
{
	m_strReportLabel = strLabel;
	m_strAuditFilename = strAuditFilename;
	m_strReportFilename = Filenames.GetReportFilename(m_strReportLabel);
	m_strReportCardListFilename = Filenames.GetReportCardListFilename(m_strReportLabel);
	m_strParamsFilename = Filenames.GetReportParamsFilename(m_strReportLabel);
	m_strReportKey = "";

	SetSingleCardFlag(FALSE);
	SetStringCardNoFrom(System.GetLowCardNo());
	SetStringCardNoTo(System.GetHighCardNo());
	UpdateInt64CardNo(FALSE);
	SetSortByCardNoFlag(FALSE);
	SetSortByGroupNoFlag(FALSE);

	SetShowServerLinesFlag(TRUE);
	SetShowPcLinesFlag(TRUE);
	SetShowPosLinesFlag(TRUE);
	SetShowEcrTextFlag(FALSE);
	SetShowAllActionsFlag(TRUE);
	SetShowActionsFlags1(0xFFFFFFFF);
	SetShowActionsFlags2(0xFFFFFFFF);
	SetShowActionsFlags3(0xFFFFFFFF);
	SetShowExtLinesFlag(TRUE);
	SetShowDeletedLinesFlag(FALSE);
	SetShowNonPurseFlag(TRUE);
	SetReportRangeType(nREPORT_RANGE);
	SetNewPageFlag(FALSE);
	SetDbNo(0);
	SetFolderSet(0);
	SetTerminalSelectType(0);
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

	SetLabelOptions();
}

//$$******************************************************************

void CAuditReportConfig::UpdateInt64CardNo(bool bSingle)
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

CString CAuditReportConfig::GetDateString()
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

CString CAuditReportConfig::SearchFilenamesGetPathname(int i)
{
	CString strFilename = "";

	if (i < m_arraySearchFilenames.GetSize())
	{
		strFilename = m_arraySearchFilenames.GetAt(i);			// 'dd/mm/yyyy' => p33\Sysfiles\Archive\yyyymmdd.ext

		if (strFilename == szCURRENTAUDIT)
		{
			strFilename = m_strAuditFilename;
		}
		else
		{
			CString strName;
			strName.Format("%s%s%s.%s", 
				(const char*) strFilename.Mid(6, 4), 
				(const char*) strFilename.Mid(3, 2),
				(const char*) strFilename.Left(2),
				(const char*) GetArchiveExtension());  // yyyymmdd.ext

			strFilename = Filenames.GetAtcArchivePath(strName);
		}
	}

	return strFilename;
}

//$$******************************************************************

CString CAuditReportConfig::GetDateFromString()
{
	m_strDateFrom = m_dateFrom.GetDate();
	return m_strDateFrom;
}

//$$******************************************************************

CString CAuditReportConfig::GetDateToString()
{
	m_strDateTo = m_dateTo.GetDate();
	return m_strDateTo;
}

//$$******************************************************************

CString CAuditReportConfig::GetTimeFromString()
{
	m_strTimeFrom = m_timeFrom.GetTime();
	return m_strTimeFrom;
}

//$$******************************************************************

CString CAuditReportConfig::GetTimeToString()
{
	m_strTimeTo = m_timeTo.GetTime();
	return m_strTimeTo;
}

//$$******************************************************************

void CAuditReportConfig::SetLabelOptions()
{
	switch (GetReportLabelNumber())
	{
	case 101:
		SetReportTitle("Audit Report");
		SetReportType(nAUDIT_REPORT);
		break;

	case 102:
		SetReportTitle("Revaluation Bonus Audit Report");
		SetReportType(nAUDIT_REVALUATIONBONUS);
		break;

	case 103:
		SetReportTitle("Spend Bonus Audit Report");
		SetReportType(nAUDIT_SPENDBONUS);
		break;

	case 109:
		SetReportTitle("Cash Taken Audit Report");
		SetReportType(nAUDIT_CASHTAKEN);
		SetShowServerLinesFlag(FALSE);
		SetShowPcLinesFlag(FALSE);
		break;

	case 110:
		SetReportTitle("Redeem Bonus Audit Report");
		SetReportType(nAUDIT_REDEEMBONUS);
		SetShowPosLinesFlag(FALSE);
		SetShowPcLinesFlag(FALSE);
		break;

	case 112:
		SetReportTitle("Points Audit Report");
		SetReportType(nAUDIT_POINTSAUDIT);
		SetShowPosLinesFlag(FALSE);
		break;

	case 113:
		SetReportTitle("Points Added Report");
		SetReportType(nAUDIT_POINTSADDED);
		SetShowPosLinesFlag(FALSE);
		break;

	case 114:
		SetReportTitle("Points Deducted Report");
		SetReportType(nAUDIT_POINTSDEDUCTED);
		SetShowPosLinesFlag(FALSE);
		break;

	case 115:
		SetReportTitle("Sales Audit Report");
		SetReportType(nAUDIT_SALE);
		break;

	case 119:
		SetReportTitle("Purse Additions Report");
		SetReportType(nAUDIT_PURSEADDITIONS);
		break;

	case 120:
		SetReportTitle("Refund Audit Report");
		SetReportType(nAUDIT_REFUND);
		break;

	case 123:
		SetReportTitle("Audit Report");				// single account
		SetReportType(nAUDIT_REPORT);
		SetReportRangeType(nREPORT_SINGLE);			// set single report only
		break;

	case 132:
		SetReportTitle("Transaction Summary Report");
		SetReportType(nAUDIT_SUMMARY);
		break;

	case 133:
		SetReportTitle("Transaction Summary Report");	// single account
		SetReportType(nAUDIT_SUMMARY);
		SetReportRangeType(nREPORT_SINGLE);				// set single report only
		break;

	default:
		SetReportTitle("Audit Report");
		SetReportType(nAUDIT_REPORT);
		break;
	}
}

//$$******************************************************************

int CAuditReportConfig::GetReportLabelNumber()
{
	int nResult = 0;
	if ((m_strReportLabel.Left(1) == "R") && (m_strReportLabel.GetLength() == 4))
	{
		nResult = atoi(m_strReportLabel.Right(3));
	}

	return nResult;
}

//$$******************************************************************

void CAuditReportConfig::SetDbNo(int n)
{
	m_nDbNo = n;
	m_TerminalListHandler.SetTargetDbNo(n);
}

//$$******************************************************************

void CAuditReportConfig::SetFolderSet(int n)
{
	m_nFolderSet = n;
	m_TerminalListHandler.SetTargetFolderSet(n);
}

//$$******************************************************************

void CAuditReportConfig::SetTerminalSelectType(int n)
{
	m_nTerminalSelectType = n;
	m_TerminalListHandler.SetTerminalSelectType(n);
}

//$$******************************************************************

bool CAuditReportConfig::GetShowActionFlag(int n)
{
	bool bResult = FALSE;

	if (TRUE == m_bShowAllActions)
	{
		return TRUE;
	}

	if ((n < 0) || (n >= APPNO_MAX) || (n >= 96))
	{
		return FALSE;
	}

	if (n < 32)
	{
		bResult = ((m_nShowActionsFlags1 & (1 << n)) != 0);
	}
	else if (n < 64)
	{
		bResult = ((m_nShowActionsFlags2 & (1 << (n - 32))) != 0);
	}
	else if (n < 96)
	{
		bResult = ((m_nShowActionsFlags3 & (1 << (n - 64))) != 0);
	}

	return bResult;
}

//$$******************************************************************

void CAuditReportConfig::SetShowActionFlag(int n, bool b)
{
	if ((n < 0) || (n >= APPNO_MAX) || (n >= 96))
	{
		return;
	}
	else if (n < 32)
	{
		if (b)
		{
			m_nShowActionsFlags1 |= (1 << n);
		}
		else
		{
			m_nShowActionsFlags1 &= ~(1 << n);
		}
	}
	else if (n < 64)
	{
		if (b)
		{
			m_nShowActionsFlags2 |= (1 << (n - 32));
		}
		else
		{
			m_nShowActionsFlags2 &= ~(1 << (n - 32));
		}
	}
	else if (n < 96)
	{
		if (b)
		{
			m_nShowActionsFlags3 |= (1 << (n - 64));
		}
		else
		{
			m_nShowActionsFlags3 &= ~(1 << (n - 64));
		}
	}
}

//$$******************************************************************