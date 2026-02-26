//$$******************************************************************
#include "..\SPosLoyalty4Shared\AuditReportTexts.h"
#include "..\SPosLoyalty4Shared\GlobalState.h"
#include "..\SPosLoyalty4Shared\RepmanDefines.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//$$******************************************************************
#include "StampOfferReport.h"
//$$******************************************************************

CStampOfferReport::CStampOfferReport(CStampOfferReportConfig& ReportConfig) : m_ReportConfig(ReportConfig)
{
	m_bGotValidFullRedeemLine = FALSE;
}

//$$******************************************************************

int CStampOfferReport::CreateReport()
{
	m_strFileDateFrom = "";
	m_strFileDateTo = "";
	m_strFileTimeFrom = "";
	m_strFileTimeTo = "";

	{
		CString strDateFrom = m_ReportConfig.GetDateFrom().GetDate();
		if (strDateFrom.GetLength() != 10)
		{
			return nREPORT_CREATEFAIL;
		}

		CString strDateTo = m_ReportConfig.GetDateTo().GetDate();
		if (strDateTo.GetLength() != 10)
		{
			return nREPORT_CREATEFAIL;
		}

		m_strFileDateFrom.Format("%4.4s%2.2s%2.2s",
			(const char*) strDateFrom.Right(4),
			(const char*) strDateFrom.Mid(3, 2),
			(const char*)strDateFrom.Left(2));

		m_strFileDateTo.Format("%4.4s%2.2s%2.2s",
			(const char*) strDateTo.Right(4),
			(const char*) strDateTo.Mid(3, 2),
			(const char*) strDateTo.Left(2));

		CString strTimeFrom = m_ReportConfig.GetTimeFrom().GetTime();
		if (strTimeFrom.GetLength() >= 5)
		{
			m_strFileTimeFrom = strTimeFrom.Left(2) + strTimeFrom.Mid(3, 2);
			m_strFileTimeFrom += "00";
		}

		CString strTimeTo = m_ReportConfig.GetTimeTo().GetTime();
		if (strTimeTo.GetLength() >= 5)
		{
			m_strFileTimeTo = strTimeTo.Left(2) + strTimeTo.Mid(3, 2);
			m_strFileTimeTo += "59";
		}
	}

	if (m_Report.Open(m_ReportConfig.GetReportFilename()) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	int nErrorNo = nREPORT_NOERROR;

	CWorkingDlg dlgWorking("Creating Report");
	dlgWorking.Create();

	CreateFileDateArray();

	CString strTitle = m_ReportConfig.GetReportTitle();
	strTitle += m_ReportConfig.GetDateString();
	m_Report.SetStyle1(strTitle);

	AddColumnText();

	StampOfferReport(&dlgWorking);

	m_Report.Close();

	return nErrorNo;
}

//$$******************************************************************

void CStampOfferReport::CreateFileDateArray()
{
	m_arrayFileDates.RemoveAll();

	CFileFind finder;
	CString strPath = "";
	strPath += Filenames.GetStampOfferLogFolder();
	strPath += "SO_*.dat";

	BOOL bWorking = finder.FindFile(strPath);

	while (bWorking == TRUE)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDirectory() == TRUE)
		{
			continue;
		}

		if (finder.IsDots() == TRUE)
		{
			continue;
		}

		CString strFilename = finder.GetFileName();
		if (strFilename.GetLength() == 15)
		{
			CString strDate = strFilename.Mid(3, 8);

			if ((strDate >= m_strFileDateFrom) && (strDate <= m_strFileDateTo))
			{
				if (::TestNumeric(strDate) == TRUE)
				{
					CSortedStringItem item;
					item.m_strItem = strDate;
					m_arrayFileDates.Consolidate(item);
				}
			}
		}
	}
}

//$$******************************************************************

void CStampOfferReport::AddColumnText()
{
	int nAuditType = m_ReportConfig.GetReportType();

	m_Report.AddColumn("Event Date", TA_LEFT, 230);
	m_Report.AddColumn("", TA_LEFT, 120);
	m_Report.AddColumn(System.GetCardNoText(), TA_LEFT, 210);
	m_Report.AddColumn("Name", TA_LEFT, 400);
	m_Report.AddColumn("Grp", TA_LEFT, 100);
	m_Report.AddColumn("Source", TA_LEFT, 230);
	m_Report.AddColumn("CCNo", TA_LEFT, 140);
	m_Report.AddColumn("OpNo", TA_LEFT, 140);
	m_Report.AddColumn("Offer", TA_LEFT, 150);
	m_Report.AddColumn("Edition", TA_LEFT, 150);
	m_Report.AddColumn("Offer Name", TA_LEFT, 400);
	m_Report.AddColumn("Plu No", TA_LEFT, 300);
	m_Report.AddColumn("Description", TA_LEFT, 400);
	m_Report.AddColumn("Plu Qty", TA_LEFT, 300);
	m_Report.AddColumn("Award Date", TA_LEFT, 230);
	m_Report.AddColumn("Expiry Date", TA_LEFT, 230);
	m_Report.AddColumn("+Stamps", TA_RIGHT, 200);
	m_Report.AddColumn("NewTTL", TA_RIGHT, 200);
}

//$$******************************************************************

int CStampOfferReport::StampOfferReport(CWorkingDlg* pDlgWorking)
{
	int nResult = 0;
	m_LastFullRedeemLine.Reset();
	m_bGotValidFullRedeemLine = FALSE;

	g_GlobalState.BuildUserIDCache();
	
	if (m_ReportConfig.GetSortByCardNoFlag() == TRUE)
	{
		nResult = StampOfferReportByCardNo(pDlgWorking);
	}
	else
	{
		nResult = StampOfferReportByDate(pDlgWorking);
	}

	g_GlobalState.ClearUserIDCache();

	m_Report.WriteLine("");
	
	return nResult;
}

//$$******************************************************************

int CStampOfferReport::StampOfferReportByCardNo(CWorkingDlg* pDlgWorking)
{
	int nCount = 0;
	bool bCancelButton = FALSE;

	CReportConsolidationArray<CSortedIntByInt64AndString> arrayLookup;

	int nChecked = 0;
	for (int n = 0; n < m_arrayFileDates.GetSize(); n++)
	{
		CSortedStringItem item;
		m_arrayFileDates.GetAt(n, item);

		CString strFilename = "";
		strFilename += Filenames.GetStampOfferLogFolder();
		strFilename += "SO_";
		strFilename += item.m_strItem;
		strFilename += ".dat";

		CSSFile fileStampLog;
		if (fileStampLog.Open(strFilename, "rb") == TRUE)
		{
			CString strLine = "";
			int nFilePos = fileStampLog.Tell();
			m_bGotValidFullRedeemLine = FALSE;
			while (fileStampLog.ReadString(strLine) == TRUE)
			{
				if (pDlgWorking -> SetCaption2RecordsChecked(++nChecked) == TRUE)
				{
					bCancelButton = TRUE;
					break;
				}

				CSQLStampOfferAuditLine Line;
				if (IsValid(strLine, Line) == TRUE)
				{
					switch (Line.m_nAppNo)
					{
					case APPNO_SO_REDEEM2:
						break;

					case APPNO_SO_REDEEM:
						m_bGotValidFullRedeemLine = TRUE;
						break;

					default:
						m_bGotValidFullRedeemLine = FALSE;
						break;
					}

					bool bUseLine = TRUE;
					if (m_ReportConfig.GetShowDeletedLinesFlag() == FALSE)
					{
						if (g_GlobalState.CheckUserIDCache(Line.m_nUserID) == FALSE)
						{
							bUseLine = FALSE;
						}
					}

					if (TRUE == bUseLine)
					{
						CSortedIntByInt64AndString itemSort;
						itemSort.m_nKey = Line.m_nUserID;
						itemSort.m_strKey = item.m_strItem;
						itemSort.m_nVal = nFilePos;
						arrayLookup.Consolidate(itemSort);
					}
				}
				else
				{
					m_bGotValidFullRedeemLine = FALSE;
				}

				nFilePos = fileStampLog.Tell();
			}

			fileStampLog.Close();
		}

		if (TRUE == bCancelButton)
		{
			break;
		}
	}

	if (FALSE == bCancelButton)
	{
		__int64 nCurrentUserID = -1;
		CString strCurrentDate = "";
		CSSFile fileData;
		m_bGotValidFullRedeemLine = FALSE;

		for (int n = 0; n < arrayLookup.GetSize(); n++)
		{
			if (pDlgWorking->SetCaption2RecordsOfTotal(n, arrayLookup.GetSize()) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}

			CSortedIntByInt64AndString itemSort;
			arrayLookup.GetAt(n, itemSort);

			CString strDate = itemSort.m_strKey;

			if (strDate != strCurrentDate)
			{
				if (strCurrentDate != "")
				{
					fileData.Close();
				}

				m_bGotValidFullRedeemLine = FALSE;

				CString strFilename = "";
				strFilename += Filenames.GetStampOfferLogFolder();
				strFilename += "SO_";
				strFilename += strDate;
				strFilename += ".dat";

				if (fileData.Open(strFilename, "rb") == TRUE)
				{
					strCurrentDate = strDate;
				}
				else
				{
					strCurrentDate = "";
				}
			}

			if (strCurrentDate != "")
			{
				fileData.Seek(itemSort.m_nVal, 0);

				CString strLine = "";
				if (fileData.ReadString(strLine) == TRUE)
				{
					CSQLStampOfferAuditLine Line;
					Line.ConvertFromString(strLine);

					if (Line.m_nUserID != nCurrentUserID)
					{
						if ((nCurrentUserID != 0) && (nCount != 0))
						{
							m_Report.WriteLine("");

							if (m_ReportConfig.GetNewPageFlag() == TRUE)
							{
								m_Report.WriteLine("<NEWPAGE>,1");
							}
						}

						nCount = 0;
						nCurrentUserID = Line.m_nUserID;
						m_bGotValidFullRedeemLine = FALSE;
					}

					bool bUseLine = TRUE;
					switch (Line.m_nAppNo)
					{
					case APPNO_SO_REDEEM2:
						if (TRUE == m_bGotValidFullRedeemLine)
						{
							MergeLastFullRedeemLine(Line);
						}
						else
						{
							bUseLine = FALSE;
						}
						break;

					case APPNO_SO_REDEEM:
						m_LastFullRedeemLine = Line;
						m_bGotValidFullRedeemLine = TRUE;
						break;

					default:
						m_bGotValidFullRedeemLine = FALSE;
						break;
					}

					bool bExists = g_GlobalState.CheckUserIDCache(Line.m_nUserID);

					if (SaveLine(Line, bExists) == TRUE)
					{
						++nCount;
					}
				}
			}
		}

		if (strCurrentDate != "")
		{
			fileData.Close();
		}
	}

	int nErrorNo = nREPORT_NOERROR;
	return (bCancelButton == TRUE) ? nREPORT_CANCEL : nErrorNo;
}

//$$******************************************************************

int CStampOfferReport::StampOfferReportByDate(CWorkingDlg* pDlgWorking)
{
	int nCount = 0;
	int nChecked = 0;
	bool bCancelButton = FALSE;

	for (int n = 0; n < m_arrayFileDates.GetSize(); n++)
	{
		CSortedStringItem item;
		m_arrayFileDates.GetAt(n, item);

		CString strFilename = "";
		strFilename += Filenames.GetStampOfferLogFolder();
		strFilename += "SO_";
		strFilename += item.m_strItem;
		strFilename += ".dat";

		CSSFile fileStampLog;
		if (fileStampLog.Open(strFilename, "rb") == TRUE)
		{
			m_bGotValidFullRedeemLine = FALSE;

			CString strLine = "";
			while (fileStampLog.ReadString(strLine) == TRUE)
			{
				if (pDlgWorking->SetCaption2RecordsChecked(++nChecked) == TRUE)
				{
					bCancelButton = TRUE;
					break;
				}

				CSQLStampOfferAuditLine Line;
				if (IsValid(strLine, Line) == TRUE)
				{
					switch (Line.m_nAppNo)
					{
					case APPNO_SO_REDEEM2:
						MergeLastFullRedeemLine(Line);
						break;

					case APPNO_SO_REDEEM:
						m_bGotValidFullRedeemLine = TRUE;
						m_LastFullRedeemLine = Line;
						break;

					default:
						m_bGotValidFullRedeemLine = FALSE;
						break;
					}

					bool bExists = g_GlobalState.CheckUserIDCache(Line.m_nUserID);

					if (SaveLine(Line, bExists) == TRUE)
					{
						++nCount;
					}
				}
				else
				{
					m_bGotValidFullRedeemLine = FALSE;
				}
			}

			fileStampLog.Close();
		}

		if (TRUE == bCancelButton)
		{
			break;
		}
	}

	int nErrorNo = nREPORT_NOERROR;
	return (bCancelButton == TRUE) ? nREPORT_CANCEL : nErrorNo;
}

//$$******************************************************************

void CStampOfferReport::MergeLastFullRedeemLine(CSQLStampOfferAuditLine& Line)
{
	Line.m_nUserID = m_LastFullRedeemLine.m_nUserID;
	Line.m_strUserName = m_LastFullRedeemLine.m_strUserName;
	Line.m_strEventDate = m_LastFullRedeemLine.m_strEventDate;
	Line.m_strEventTime = m_LastFullRedeemLine.m_strEventTime;
	Line.m_nTNo = m_LastFullRedeemLine.m_nTNo;
	Line.m_nCCNo = m_LastFullRedeemLine.m_nCCNo;
	Line.m_nServerNo = m_LastFullRedeemLine.m_nServerNo;
	Line.m_nUserID = m_LastFullRedeemLine.m_nUserID;
	Line.m_strUserName = m_LastFullRedeemLine.m_strUserName;
	Line.m_nGroupNo = m_LastFullRedeemLine.m_nGroupNo;
	Line.m_nOfferID = m_LastFullRedeemLine.m_nOfferID;
	Line.m_nEdition = m_LastFullRedeemLine.m_nEdition;
	Line.m_strOfferName = m_LastFullRedeemLine.m_strOfferName;
	Line.m_nPluNo = m_LastFullRedeemLine.m_nPluNo;
	Line.m_strPluDescription = m_LastFullRedeemLine.m_strPluDescription;
}

//$$******************************************************************

bool CStampOfferReport::IsValid(CString strLine, CSQLStampOfferAuditLine& Line)
{
	Line.ConvertFromString(strLine);

	if (APPNO_SO_REDEEM2 == Line.m_nAppNo)
	{
		return m_bGotValidFullRedeemLine;
	}
	
	if ((Line.m_strEventDate < m_strFileDateFrom) || (Line.m_strEventDate > m_strFileDateTo))
	{
		return FALSE;
	}

	if ((Line.m_strEventTime < m_strFileTimeFrom) || (Line.m_strEventTime > m_strFileTimeTo))
	{
		return FALSE;
	}

	if (AUDIT_POS == Line.m_nSourceType)
	{
		if ((APPNO_SO_AWARD == Line.m_nAppNo) && (m_ReportConfig.GetShowEPOSAwardFlag() == FALSE))
		{
			return FALSE;
		}

		if ((APPNO_SO_REDEEM == Line.m_nAppNo) && (m_ReportConfig.GetShowEPOSRedeemFlag() == FALSE))
		{
			return FALSE;
		}
	}

	if (AUDIT_PC == Line.m_nSourceType)
	{
		if ((APPNO_SO_MADD == Line.m_nAppNo) && (m_ReportConfig.GetShowManualAddFlag() == FALSE))
		{
			return FALSE;
		}

		switch (Line.m_nAppNo)
		{
		case APPNO_SO_MEDIT:
		case APPNO_SO_MFIX_EXPIRE:
		case APPNO_SO_MFIX_EDITION:
			if (m_ReportConfig.GetShowManualEditFlag() == FALSE)
			{
				return FALSE;
			}
			break;
		}

		switch (Line.m_nAppNo)
		{
		case APPNO_SO_MDEL:
		case APPNO_SO_MDEL_EXPIRE:
		case APPNO_SO_MDEL_EDITION:
			if (m_ReportConfig.GetShowManualDeleteFlag() == FALSE)
			{
				return FALSE;
			}
			break;
		}
		
		if ((APPNO_SO_ADEL == Line.m_nAppNo) && (m_ReportConfig.GetShowManualDeleteFlag() == FALSE))
		{
			return FALSE;
		}

		if ((APPNO_SO_HOTL == Line.m_nAppNo) && (m_ReportConfig.GetShowHotlistFlag() == FALSE))
		{
			return FALSE;
		}

		if ((APPNO_SO_XFER == Line.m_nAppNo) && (m_ReportConfig.GetShowTransferFlag() == FALSE))
		{
			return FALSE;
		}
	}
	
	if (IsCardInRange(Line) == FALSE)
	{
		return FALSE;
	}

	// check Epos terminal list
	if (AUDIT_POS == Line.m_nSourceType)
	{
		if (m_ReportConfig.CheckTerminalFilter(Line.m_nDbNo, Line.m_nFolderSet, Line.m_nTNo) == FALSE)
		{
			return FALSE;
		}
	}

	if (m_ReportConfig.GetSingleOfferNo() != 0)
	{
		if (m_ReportConfig.GetSingleOfferNo() != Line.m_nOfferID)
		{
			return FALSE;
		}
	}

	return TRUE;
}

//$$******************************************************************

bool CStampOfferReport::IsCardInRange(CSQLStampOfferAuditLine& Line)
{
	CString strCardNo = "";
	strCardNo.Format("%I64d", Line.m_nUserID);

	if (m_ReportConfig.GetReportRangeType() == nREPORT_LIST)
	{
		return m_ReportConfig.IsValidCard(strCardNo, Filenames.GetReportCardListFilename(m_ReportConfig.GetReportLabel()));
	}

	if ((Line.m_nUserID < m_ReportConfig.GetInt64CardNoFrom()) || (Line.m_nUserID > m_ReportConfig.GetInt64CardNoTo()))
	{
		return FALSE;
	}

	if ((m_ReportConfig.GetSingleGroupFlag() == TRUE) && (m_ReportConfig.GetSingleGroupNo() != Line.m_nGroupNo))
	{
		return FALSE;
	}

	return TRUE;
}

//$$******************************************************************

bool CStampOfferReport::SaveLine(CSQLStampOfferAuditLine& Line, bool bExists)
{
	if (FALSE == bExists)
	{
		if (m_ReportConfig.GetShowDeletedLinesFlag() == FALSE)
		{
			return FALSE;
		}
	}

	bool bResult = TRUE;

	switch (Line.m_nAppNo)
	{
	case APPNO_SO_AWARD:
	case APPNO_SO_REDEEM:
	{
		CString strAccountNo = "";
		strAccountNo = System.FormatCardNo(Line.m_nUserID, TRUE);

		if (FALSE == bExists)
		{
			strAccountNo += " *";
		}

		CString strCCNo = "";
		strCCNo.Format("%4.4d", Line.m_nCCNo);

		CString strPluNo = "";
		strPluNo.Format("%14.14I64d", Line.m_nPluNo);

		CCSV csv('\t');
		csv.Add(FormatDate(Line.m_strEventDate, FALSE));
		csv.Add(FormatTime(Line.m_strEventTime));
		csv.Add(strAccountNo);
		csv.Add(Line.m_strUserName);
		csv.Add(Line.m_nGroupNo);
		csv.Add(CAuditReportTexts::GetReportSourceText(Line.m_nSourceType, Line.m_nAppNo, Line.m_nTNo));
		csv.Add(strCCNo);
		csv.Add(Line.m_nServerNo);
		csv.Add(Line.m_nOfferID);
		csv.Add(Line.m_nEdition);
		csv.Add(Line.m_strOfferName);
		csv.Add(strPluNo);
		csv.Add(Line.m_strPluDescription);
		csv.Add(Line.m_nPluQty);
		csv.Add(FormatDate(Line.m_strAwardDate, FALSE));
		csv.Add(FormatDate(Line.m_strExpiryDate, TRUE));
		csv.Add(Line.m_nLineStamps);
		csv.Add(Line.m_nNewStampCount);
		bResult = m_Report.WriteLine(csv);
	}
	break;

	case APPNO_SO_REDEEM2:
	{
		CCSV csv('\t');
		csv.Add("");
		csv.Add("");
		csv.Add("");
		csv.Add("");
		csv.Add("");
		csv.Add("");
		csv.Add("");
		csv.Add("");
		csv.Add("");
		csv.Add("");
		csv.Add("");
		csv.Add("");
		csv.Add("");
		csv.Add("");
		csv.Add(FormatDate(Line.m_strAwardDate, FALSE));
		csv.Add(FormatDate(Line.m_strExpiryDate, TRUE));
		csv.Add(Line.m_nLineStamps);
		csv.Add(Line.m_nNewStampCount);
		bResult = m_Report.WriteLine(csv);
	}
	break;

	case APPNO_SO_MDEL:
	case APPNO_SO_MDEL_EXPIRE:
	case APPNO_SO_MDEL_EDITION:
	case APPNO_SO_MFIX_EXPIRE:
	case APPNO_SO_MFIX_EDITION:
	case APPNO_SO_MEDIT:
	case APPNO_SO_MADD:
	case APPNO_SO_XFER:
	case APPNO_SO_HOTL:
	case APPNO_SO_ADEL:
		{
			CString strAccountNo = "";
			strAccountNo = System.FormatCardNo(Line.m_nUserID, TRUE);

			if (FALSE == bExists)
			{
				strAccountNo += " *";
			}

			CString strCCNo = "";
			strCCNo.Format("%4.4d", Line.m_nCCNo);

			CString strPluNo = "";
			strPluNo.Format("%14.14I64d", Line.m_nPluNo);

			CCSV csv('\t');
			csv.Add(FormatDate(Line.m_strEventDate, FALSE));
			csv.Add(FormatTime(Line.m_strEventTime));
			csv.Add(strAccountNo);
			csv.Add(Line.m_strUserName);
			csv.Add(Line.m_nGroupNo);
			csv.Add(CAuditReportTexts::GetReportSourceText(Line.m_nSourceType, Line.m_nAppNo, Line.m_nTNo));
			csv.Add(strCCNo);
			csv.Add("");
			csv.Add(Line.m_nOfferID);
			csv.Add(Line.m_nEdition);
			csv.Add(Line.m_strOfferName);
			csv.Add("");
			csv.Add("");
			csv.Add("");
			csv.Add(FormatDate(Line.m_strAwardDate, FALSE));
			csv.Add(FormatDate(Line.m_strExpiryDate, TRUE));
			csv.Add(Line.m_nLineStamps);
			csv.Add(Line.m_nNewStampCount);
			bResult = m_Report.WriteLine(csv);
		}
		break;
	}

	return bResult;
}

//$$******************************************************************

CString CStampOfferReport::FormatDate(CString strDate, bool bIsExpiry)
{
	CString strResult = "";

	if ((strDate.GetLength() == 8) && ::TestNumeric(strDate))
	{
		if ((FALSE == bIsExpiry) || (strDate != CSQLRowOfferStamp::GetNeverExpireDate()))
		{
			strResult.Format("%2.2s/%2.2s/%4.4s",
				(const char*)strDate.Right(2),
				(const char*)strDate.Mid(4, 2),
				(const char*)strDate.Left(4));			
		}
		else
		{
			strResult = "----";
		}
	}

	return strResult;
}

//$$******************************************************************

CString CStampOfferReport::FormatTime(CString strTime)
{
	CString strResult = "";

	if (strTime.GetLength() >= 4)
	{
		strResult.Format("%2.2s:%2.2s",
			(const char*)strTime.Left(2),
			(const char*)strTime.Mid(2, 2));
	}

	return strResult;
}

//$$******************************************************************