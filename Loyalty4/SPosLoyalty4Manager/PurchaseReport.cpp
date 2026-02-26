/******************************************************************************/
#include <math.h>
/******************************************************************************/
#include "resource.h"
/******************************************************************************/
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
#include "..\SPosLoyalty4Shared\SQLDb.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
/******************************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRecordSetAccountUserID.h"
#include "..\SPosLoyalty4Shared\SQLTable_EcrmanDatabase\SQLRepositoryEcrmanDatabase.h"
#include "..\SPosLoyalty4Shared\SQLTable_PluInfo\SQLRowPluInfo.h"
/******************************************************************************/
#include "LoyaltyManager.h"
#include "Recsort.h"
#include "TLogFile.h"
/******************************************************************************/
#include "PurchaseReport.h"
#include "ReportPurchaseHistoryDlg.h"
/******************************************************************************/
// R104 = Purchase History All
// R105 = Purchase History Single

CPurchaseReport::CPurchaseReport(const char* szLabel, CWnd* pParent)
	: m_Repman("Purchase History", pParent)
{
	m_strReportLabel = szLabel;
	m_pParent = pParent;

	m_nReportLines = 0;
	m_bNewPageReqd = FALSE;
	m_strReportTitle = "Purchase History";

	Server.Read(Filenames.GetServerDataFilename());
	m_bIncludePoints = Server.GetEnablePluBonusFlag();

	ResetTotals(0);
	ResetTotals(1);
	ResetTotals(2);
}

/******************************************************************************/

void CPurchaseReport::ResetTotals(int nLevel)
{
	m_strLastDate = "";

	switch (nLevel)
	{
	case 0:
	case 1:
	case 2:
		m_arrayTotals[nLevel].Reset();
		break;
	}
}

//*******************************************************************

void CPurchaseReport::AddTotalPoints(int n)
{
	m_arrayTotals[0].m_nTotalPoints += n;
	m_arrayTotals[1].m_nTotalPoints += n;
	m_arrayTotals[2].m_nTotalPoints += n;
}

//*******************************************************************

void CPurchaseReport::AddTotalValue(double d)
{
	m_arrayTotals[0].m_dTotalValue += d;
	m_arrayTotals[1].m_dTotalValue += d;
	m_arrayTotals[2].m_dTotalValue += d;
}

//*******************************************************************

void CPurchaseReport::AddTotalCardSpend(double d)
{
	m_arrayTotals[0].m_dTotalCardSpend += d;
	m_arrayTotals[1].m_dTotalCardSpend += d;
	m_arrayTotals[2].m_dTotalCardSpend += d;
}

//*******************************************************************

void CPurchaseReport::Show(__int64 nUserID)
{
	int nErrorNo;
	
	if (m_strReportLabel == "R105")
	{
		nErrorNo = DisplayOne(nUserID);	// single report
	}
	else
	{
		nErrorNo = DisplayAll();
	}

	m_Repman.DisplayError ( nErrorNo );
}

//*******************************************************************

int CPurchaseReport::DisplayAll()
{
	Server.UpdateEcrmanDbInfo();
	CReportInfoPurchaseHistory ReportInfo( m_strReportLabel );
	CReportPurchaseHistoryDlg dlg ( ReportInfo, m_pParent );

	while ( dlg.DoModal() != IDCANCEL)
	{
		CString strReportFile = ReportInfo.GetReportFilename();

		if (m_Report.Open(strReportFile) == FALSE)
		{
			return nREPORT_CREATEFAIL;
		}

		ResetTotals(0);
		ResetTotals(1);
		ResetTotals(2);

		CArray<__int64, __int64> arrayUserIDs;

		CWorkingDlg WorkingDlg("Filtering Records");
		WorkingDlg.Create();

		CString strWhere = "";
		CSQLBuilderBind BuilderBind;
		CSQLRepositoryAccount repoAccount;
		repoAccount.PrepareSearchWhere(ReportInfo, strWhere, BuilderBind);

		CSQLRecordSetAccountUserID RecordSet(NULL,
			RSParams_AccountUserId_NormalByParams{ &BuilderBind, strWhere });
	
		int nCount = 0;
		bool bCancelButton = FALSE;

		__int64 nUserID = 0;
		while (RecordSet.StepSelect(nUserID) == TRUE)
		{
			if (WorkingDlg.SetCaption2RecordsChecked(nCount++) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}
			
			if (ReportInfo.NonSQLFilter(nUserID) == TRUE)
			{
				arrayUserIDs.Add(nUserID);
			}
		}

		if ( bCancelButton == FALSE )
		{
			m_Report.SetStyle1 ( ReportInfo.GetReportTitle() );

			if (ReportInfo.GetSummaryFlag() == TRUE)
			{
				AddSummaryColumnText();
			}
			else
			{
				AddColumnText();
			}

			int nSize = arrayUserIDs.GetSize();
			bool bHaveSeparator = FALSE;
		
			WorkingDlg.SetCaption1("Creating Report");
			
			bool bFirstAccount = TRUE;

			for (int n = 0; n < nSize; n++)
			{
				__int64 nUserID = arrayUserIDs.GetAt(n);

				WorkingDlg.SetCaption2PercentDone((n * 100) / nSize, FALSE);

				if (ReportInfo.GetSummaryFlag() == TRUE)
				{
					ConsolidateTReport(ReportInfo, nUserID, &WorkingDlg, bCancelButton, bFirstAccount);
				}
				else
				{
					PrintTReport(ReportInfo, nUserID, &WorkingDlg, bCancelButton);
				}

				if (bCancelButton == TRUE)
				{
					break;
				}
			}

			if (ReportInfo.GetSummaryFlag() == TRUE)
			{
				PrintSummaryTotals(2);
			}
			else
			{
				PrintDetailTotals(1);
			}
		}

		m_Report.Close();
		WorkingDlg.DestroyWindow();

		if ( bCancelButton == FALSE )
		{
			CRepmanHandler repman ( ReportInfo.GetReportTitle(), m_pParent );

			int nErrorNo = nREPORT_NOMATCH;
			if (m_nReportLines > 0)
			{
				nErrorNo = repman.DisplayReport(strReportFile, ReportInfo.GetParamsFilename(), ReportInfo.GetReportKey());
			}

			repman.DisplayError ( nErrorNo );
		}
	}
	
	return nREPORT_NOERROR;
}

//*********************************************************************
// single card report

int CPurchaseReport::DisplayOne(__int64 nUserID)
{
	Server.UpdateEcrmanDbInfo();
	CReportInfoPurchaseHistory ReportInfo( m_strReportLabel );
	CReportPurchaseHistoryDlg dlg (ReportInfo, m_pParent );

	if ( dlg.DoModal() == IDOK )
	{
		CString strReportFile = ReportInfo.GetReportFilename();

		if (m_Report.Open(strReportFile) == FALSE)
		{
			return nREPORT_CREATEFAIL;
		}

		ResetTotals(0);
		ResetTotals(1);
		ResetTotals(2);

		CWorkingDlg WorkingDlg ( "Creating Report" );
		WorkingDlg.Create();
		bool bCancelButton = FALSE;

		m_Report.SetStyle1 ( ReportInfo.GetReportTitle() );
		if ( ReportInfo.GetSummaryFlag() == TRUE )
		{
			bool bFirstAccount = TRUE;
			AddSummaryColumnText();
			ConsolidateTReport ( ReportInfo, nUserID, &WorkingDlg, bCancelButton, bFirstAccount );
		}
		else
		{
			AddColumnText();
			PrintTReport ( ReportInfo, nUserID, &WorkingDlg, bCancelButton);
		}

		m_Report.Close();

		if ( bCancelButton == FALSE )
		{
			CRepmanHandler repman ( ReportInfo.GetReportTitle(), m_pParent );
			int nErrorNo = repman.DisplayReport( strReportFile, ReportInfo.GetParamsFilename(), ReportInfo.GetReportKey() );
			repman.DisplayError ( nErrorNo );
		}
	}

	return nREPORT_NOERROR;
}

//******************************************************************************

int CPurchaseReport::PrintTReport ( CReportInfoPurchaseHistory& ReportInfo, __int64 nUserID, CWorkingDlg* pWorkingDlg, bool& bCancelButton )
{
	CSQLRepositoryEcrmanDatabase repoDb;
	repoDb.GetDatabaseNameList(m_arrayDbNames, NULL);

	CTLogFile tlogFile (SimpleFormatInt64Value(nUserID));					// will import any new waiting updates
	if (tlogFile.Open() == FALSE)
	{
		return 0;
	}

	tlogFile.JumpIn ( ReportInfo.GetDateFromString() );
	CTLogRecord tlogRecord;

	ResetTotals(0);												// reset report totals

	int nCount = 0;

	while ( tlogFile.ReadLine ( &tlogRecord ) == TRUE )
	{
		if (pWorkingDlg->SetCaption2PercentDone(tlogFile.GetPercentPosition()) == TRUE)
		{
			bCancelButton = TRUE;
			break;
		}

		if (tlogRecord.m_date < ReportInfo.GetDateFrom())
		{
			continue;
		}

		if ( ReportInfo.GetTimeRangeFlag() == TRUE )
		{
			if (tlogRecord.m_time < ReportInfo.GetTimeFrom() || tlogRecord.m_time > ReportInfo.GetTimeTo())
			{
				continue;
			}
		}

		if (ReportInfo.CheckTerminalFilter(tlogRecord.GetDbNo(), tlogRecord.GetFolderSet(), tlogRecord.GetTerminalNo()) == FALSE)
		{
			continue;
		}

		if (tlogRecord.m_date > ReportInfo.GetDateTo())
		{
			break;
		}

		{
			int nFilterType = ReportInfo.GetSalesTypeFilter();
			if ((1 == nFilterType) || (2 == nFilterType))
			{
				bool bIsRenewal = IsRenewalTLogLine(&tlogRecord);

				if ((TRUE == bIsRenewal) && (1 == nFilterType))
				{
					continue;
				}

				if ((FALSE == bIsRenewal) && (2 == nFilterType))
				{
					continue;
				}
			}
		}

		if ( m_bNewPageReqd == TRUE )
		{
			m_Report.WriteLine ( "<NEWPAGE>,1" );
			m_bNewPageReqd = FALSE;
		}

		if (nCount == 0)									// need header before first line
		{
			AddCardDetails(nUserID);
		}
		
		PrintTLogRecord ( &tlogRecord, ReportInfo );
		++nCount;
	}

	if (bCancelButton == TRUE)
	{
		return 0;
	}

	if ( nCount > 0 )
	{
		PrintDetailTotals(0);
		if (ReportInfo.GetNewPageFlag() == TRUE)
		{
			m_bNewPageReqd = TRUE;
		}
		else
		{
			m_Report.WriteLine("");
			m_Report.WriteLine("");
		}
	}

	return nCount;
}

//*******************************************************************

bool CPurchaseReport::IsRenewalTLogLine( CTLogRecord* pTLogRecord )
{
	bool bRenewal = FALSE;

	for ( int nIndex = 0; nIndex < pTLogRecord->GetItemCounter(); nIndex++ )
	{
		pTLogRecord->ExtractItemData ( nIndex );

		if ( IsRenewalTLogItem( pTLogRecord ) == TRUE )	
		{
			bRenewal = TRUE;
			break;
		}
	}

	return bRenewal;	
}

//*******************************************************************

bool CPurchaseReport::IsRenewalTLogItem( CTLogRecord* pTLogRecord )
{
	bool bRenewal = FALSE;

	if (nTLOGTYPE_PLU == pTLogRecord->GetItemType())
	{
		if (pTLogRecord->GetItemFlags() >= 1)
		{
			if (((pTLogRecord->GetItemFlags()) & 1) == 1)
			{
				bRenewal = TRUE;
			}
		}
	}

	return bRenewal;
}

//*******************************************************************

CString CPurchaseReport::GetItemTextForReport(CTLogRecord* pTLogRecord)
{
	CString strItemText = pTLogRecord->GetItemText();

	if (strItemText == "")
	{
		switch (pTLogRecord->GetItemType())
		{
		case nTLOGTYPE_PLU:
			strItemText = "Plu ";
			strItemText += CSQLRowPluInfo::FormatPluNo(pTLogRecord->GetItemNo());
			break;

		case nTLOGTYPE_DISC_ITEM:
			strItemText = "Item Discount";
			break;

		case nTLOGTYPE_DEPT:
			strItemText = "Dept ";
			strItemText += pTLogRecord->GetItemNo();
			break;

		case nTLOGTYPE_DISC_SUBT:
			strItemText = "Subtotal Discount";
			break;
		}
	}
	
	return strItemText;
}

//*******************************************************************

void CPurchaseReport::PrintTLogRecord(CTLogRecord* pTLogRecord, CReportInfoPurchaseHistory& ReportInfo)
{
	for (int nIndex = 0; nIndex < pTLogRecord->GetItemCounter(); nIndex++)
	{
		CCSV csv('\t');

		if (m_strLastDate == pTLogRecord->GetDate())
		{
			csv.Add("");
		}
		else
		{
			m_strLastDate = pTLogRecord->m_date.GetDate();
			csv.Add(m_strLastDate);
		}

		if (nIndex == 0)
		{
			csv.Add(pTLogRecord->GetTime());

			int nDbNo = pTLogRecord->GetDbNo();
			if (0 == nDbNo)
			{
				nDbNo = 1;
			}

			int nSetNo = pTLogRecord->GetFolderSet();
			if (0 == nSetNo)
			{
				nSetNo = 1;
			}

			if (System.GetMaxEcrmanDatabases() > 1)
			{
				CString strDbName = "";
				if ((nDbNo >= 1) && (nDbNo <= m_arrayDbNames.GetSize()))
				{
					strDbName = m_arrayDbNames.GetAt(nDbNo - 1);
				}

				csv.Add(strDbName);
			}

			CServerDataFolderSetIndex FolderSetIndex;
			FolderSetIndex.m_nDbNo = nDbNo;
			FolderSetIndex.m_nSetNo = nSetNo;
			csv.Add(Server.GetEposPathLabel(FolderSetIndex));

			int nTNo = pTLogRecord->GetTerminalNo();
			if (nTNo != 0)
			{
				csv.Add(nTNo);
			}
			else
			{
				csv.Add("");
			}

			double dTotalSpend = pTLogRecord->GetTotalSpend();
			csv.Add(dTotalSpend, System.GetDPValue());
			AddTotalCardSpend(dTotalSpend);
		}
		else
		{
			csv.Add("");					// time
			csv.Add("");					// database
			csv.Add("");					// TNo
			csv.Add("");					// spend

			if (System.GetMaxEcrmanDatabases() > 1)
			{
				csv.Add("");
			}
		}

		pTLogRecord->ExtractItemData(nIndex);

		switch (pTLogRecord->GetItemType())
		{
		case nTLOGTYPE_PLU:
		case nTLOGTYPE_DISC_ITEM:
			csv.Add(CSQLRowPluInfo::FormatPluNo(pTLogRecord->GetItemNo()));	// plu no
			break;

		case nTLOGTYPE_DEPT:
			{
				CString strDeptNo = pTLogRecord->GetItemNo();
				::AddLeading(strDeptNo, 2, '0');		// dept no
				csv.Add(strDeptNo);
			}
			break;

		case nTLOGTYPE_DISC_SUBT:
			csv.Add("");								// discount						
			break;
		}

		csv.Add(GetItemTextForReport(pTLogRecord));

		double d = 0.0;
		if (modf(pTLogRecord->GetItemLineQty(), &d) != 0.0)
		{
			csv.Add(pTLogRecord->GetItemLineQty(), 1);
		}
		else
		{
			csv.Add(pTLogRecord->GetItemLineQty(), 0);
		}

		csv.Add(pTLogRecord->GetItemLineVal(), System.GetDPValue());
		AddTotalValue(pTLogRecord->GetItemLineVal());

		csv.Add(pTLogRecord->GetItemPoints());
		AddTotalPoints(pTLogRecord->GetItemPoints());

		m_Report.WriteLine(csv);
		++m_nReportLines;
	}
}

/*****************************************************************************/

int CPurchaseReport::ConsolidateTReport(CReportInfoPurchaseHistory& ReportInfo, __int64 nUserID, CWorkingDlg* pWorkingDlg, bool& bCancelButton, bool& bFirstAccount)
{
	CSQLRepositoryEcrmanDatabase repoDb;
	repoDb.GetDatabaseNameList(m_arrayDbNames, NULL);

	// will import any new waiting updates
	CTLogFile tlogFile(SimpleFormatInt64Value(nUserID));
	if (tlogFile.Open() == FALSE)
	{
		return 0;
	}

	CTLogRecord tlogRecord;
	CReportConsolidationArray<CConsolidatedThreeValuesByString> consolArray(FALSE, 1); // ascending ignorecase

	tlogFile.JumpIn(ReportInfo.GetDateFromString());

	while (tlogFile.ReadLine(&tlogRecord) == TRUE)
	{
		if (pWorkingDlg->SetCaption2PercentDone(tlogFile.GetPercentPosition() / 2) == TRUE)
		{
			bCancelButton = TRUE;
			break;
		}

		if (tlogRecord.m_date < ReportInfo.GetDateFrom())
		{
			continue;
		}

		if (ReportInfo.GetTimeRangeFlag() == TRUE)
		{
			if (tlogRecord.m_time < ReportInfo.GetTimeFrom() || tlogRecord.m_time > ReportInfo.GetTimeTo())
			{
				continue;
			}
		}

		if (tlogRecord.m_date > ReportInfo.GetDateTo())
		{
			break;
		}

		if (ReportInfo.CheckTerminalFilter(tlogRecord.GetDbNo(), tlogRecord.GetFolderSet(), tlogRecord.GetTerminalNo()) == FALSE)
		{
			continue;
		}

		for (int nIndex = 0; nIndex < tlogRecord.GetItemCounter(); nIndex++)
		{
			tlogRecord.ExtractItemData(nIndex);

			{
				int nFilterType = ReportInfo.GetSalesTypeFilter();
				if ((1 == nFilterType) || (2 == nFilterType))
				{
					bool bIsRenewal = IsRenewalTLogItem(&tlogRecord);

					if ((TRUE == bIsRenewal) && (1 == nFilterType))
					{
						continue;
					}

					if ((FALSE == bIsRenewal) && (2 == nFilterType))
					{
						continue;
					}
				}
			}

			CString strDbNo = "";
			strDbNo.Format("%3.3d", tlogRecord.GetDbNo());
			if (strDbNo.GetLength() != 3)
			{
				strDbNo = "001";
			}

			CConsolidatedThreeValuesByString itemConsol;
			itemConsol.m_strKey = strDbNo + GetItemTextForReport(&tlogRecord);
			itemConsol.m_dValue1 = tlogRecord.GetItemLineQty();
			itemConsol.m_dValue2 = tlogRecord.GetItemLineVal();
			itemConsol.m_dValue3 = (double)tlogRecord.GetItemPoints();
			consolArray.Consolidate(itemConsol);
		}
	}

	if (bCancelButton == TRUE)
	{
		return 0;
	}

	// see if any data to print
	int nCount = consolArray.GetSize();
	if (nCount > 0)
	{
		bFirstAccount = FALSE;

		if (m_bNewPageReqd == TRUE)					// see if start of new card
		{
			m_Report.WriteLine("<NEWPAGE>,1");
			m_bNewPageReqd = FALSE;
		}

		AddCardDetails(nUserID);
		ResetTotals(0);									// reset report totals
		ResetTotals(1);									// reset report totals

		int nLastDbNo = -1;
		int nSubTotals = 0;

		for (int nIndex = 0; nIndex < consolArray.GetSize(); nIndex++)
		{
			if (pWorkingDlg->SetCaption2PercentDone(50 + ((nIndex * 50L) / consolArray.GetSize())) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}

			CConsolidatedThreeValuesByString itemConsol;
			consolArray.GetAt(nIndex, itemConsol);

			double dQty = itemConsol.m_dValue1;
			double dValue = itemConsol.m_dValue2;
			int nPoints = (int)itemConsol.m_dValue3;

			if (dQty != 0.0 || dValue != 0.0 || nPoints != 0)
			{
				CCSV csv('\t');

				if (System.GetMaxEcrmanDatabases() != 1)
				{
					CString strDbName = "";
					int nDbNo = atoi(itemConsol.m_strKey.Left(3));
					if ((nDbNo >= 1) && (nDbNo <= m_arrayDbNames.GetSize()))
					{
						strDbName = m_arrayDbNames.GetAt(nDbNo - 1);
					}

					if (nDbNo != nLastDbNo)
					{
						if (nLastDbNo != -1)
						{
							PrintSummaryTotals(0);
							m_Report.WriteLine("");
							nSubTotals++;
						}

						nLastDbNo = nDbNo;
					}

					csv.Add(strDbName);
				}

				csv.Add(itemConsol.m_strKey.Mid(3));

				double n;
				if (modf(dQty, &n) != 0.0)
				{
					csv.Add(dQty, 1);
				}
				else
				{
					csv.Add(dQty, 0);
				}

				csv.Add(dValue, System.GetDPValue());
				csv.Add(nPoints);

				AddTotalValue(dValue);
				AddTotalPoints(nPoints);

				m_Report.WriteLine(csv);
				++m_nReportLines;
			}
		}

		if (nLastDbNo != -1)
		{
			PrintSummaryTotals(0);
			nSubTotals++;
		}
		else if (System.GetMaxEcrmanDatabases() == 1)
		{
			PrintSummaryTotals(1);
		}

		if (nSubTotals >= 2)
		{
			m_Report.WriteLine("");
			PrintSummaryTotals(1);
		}

		if (ReportInfo.GetNewPageFlag() == TRUE)
		{
			if (FALSE == bFirstAccount)
			{
				m_bNewPageReqd = TRUE;
			}
		}
		else
		{
			if (FALSE == bFirstAccount)
			{
				m_Report.WriteLine("");
				m_Report.WriteLine("");
			}
		}
	}

	return nCount;
}

//*******************************************************************

void CPurchaseReport::AddCardDetails(__int64 nUserID)
{
	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserID(nUserID);

	CSQLRepositoryAccount repoAccount;
	repoAccount.SelectRow(RowAccount,NULL);

	CString strHeader = "";
	strHeader.Format ( "<..>Account %s  - %s",
		(const char*) System.FormatCardNo(RowAccount.GetUserIDString()),
		(const char*) RowAccount.GetFullname() );

	m_Report.WriteLine ( strHeader );
	m_Report.WriteLine ( "<LI>" );
}

//*******************************************************************

void CPurchaseReport::AddColumnText()
{
	m_Report.AddColumn("Date", TA_LEFT, 220);
	m_Report.AddColumn("Time", TA_LEFT, 120);

	if (System.GetMaxEcrmanDatabases() > 1)
	{
		m_Report.AddColumn("Database", TA_LEFT, 300);
	}

	m_Report.AddColumn("Location", TA_LEFT, 300);
	m_Report.AddColumn("TNo", TA_LEFT, 120);
	m_Report.AddColumn("Card Spend", TA_RIGHT, 250);
	m_Report.AddColumn("Plu No.", TA_LEFT, 270);
	m_Report.AddColumn("Description", TA_LEFT, 500);
	m_Report.AddColumn("Quantity", TA_RIGHT, 200);
	m_Report.AddColumn("Value", TA_RIGHT, 250);

	if (m_bIncludePoints == TRUE)
	{
		m_Report.AddColumn("Points", TA_RIGHT, 200);
	}
}

//*******************************************************************

void CPurchaseReport::AddSummaryColumnText()
{
	if (System.GetMaxEcrmanDatabases() > 1)
	{
		m_Report.AddColumn("Database", TA_LEFT, 300);
	}

	m_Report.AddColumn("Description", TA_LEFT, 500);
	m_Report.AddColumn("Quantity", TA_RIGHT, 200);
	m_Report.AddColumn("Value", TA_RIGHT, 249);

	if (m_bIncludePoints == TRUE)
	{
		m_Report.AddColumn("Points", TA_RIGHT, 249);
	}
}

//*******************************************************************

void CPurchaseReport::PrintDetailTotals(int nLevel)
{
	CString strTabLine = "";

	if (System.GetMaxEcrmanDatabases() > 1)
	{
		strTabLine += "\t";
	}

	strTabLine += "\t\t\t\t<LI>\t\t\t\t<LI>";

	CString strTotal = "";
	switch (nLevel)
	{
	case 0:
		strTotal = "<..>Account Total";
		break;

	case 1:
		strTotal = "<..>Report Total";
		break;
	}

	CCSV csv('\t');
	
	csv.Add("");										// date
	csv.Add("");										// time
	csv.Add(strTotal);								// database	
	
	if (System.GetMaxEcrmanDatabases() > 1)
	{
		csv.Add("");									// location
	}
	
	csv.Add("");										// TNo
	csv.Add(m_arrayTotals[nLevel].m_dTotalCardSpend, System.GetDPValue());	// card spend
	csv.Add("");										// Plu No.
	csv.Add("");										// description
	csv.Add("");										// qty
	csv.Add(m_arrayTotals[nLevel].m_dTotalValue, System.GetDPValue());		// value

	if (m_bIncludePoints == TRUE)
	{
		csv.Add(m_arrayTotals[nLevel].m_nTotalPoints);						// points
		strTabLine += "\t<LI>";
	}

	m_Report.WriteLine(strTabLine);
	m_Report.WriteLine(csv);
}

//*******************************************************************

void CPurchaseReport::PrintSummaryTotals(int nLevel)
{
	CString strTabLine = "";

	if (System.GetMaxEcrmanDatabases() > 1)
	{
		strTabLine += "\t";
	}

	strTabLine += "\t\t<LI>";

	CCSV csv('\t');
	
	if (System.GetMaxEcrmanDatabases() > 1)
	{
		csv.Add("");									// database
	}

	switch (nLevel)
	{
	case 0:
		csv.Add("<..>Database Total");				// desc
		break;

	case 1:
		csv.Add("<..>Account Total");				// desc
		break;

	case 2:
		csv.Add("<..>Report Total");					// desc
		break;
	}
	
	csv.Add("");									// qty
	csv.Add(m_arrayTotals[nLevel].m_dTotalValue, System.GetDPValue());

	if (TRUE == m_bIncludePoints)
	{
		csv.Add(m_arrayTotals[nLevel].m_nTotalPoints);	// points
		strTabLine += "\t<LI>";
	}

	m_Report.WriteLine(strTabLine);
	m_Report.WriteLine(csv);

	if ( nLevel <= 1 )
	{
		m_arrayTotals[0].Reset();
	}

	if (1 == nLevel)
	{
		m_arrayTotals[1].Reset();
	}
}

//*******************************************************************


