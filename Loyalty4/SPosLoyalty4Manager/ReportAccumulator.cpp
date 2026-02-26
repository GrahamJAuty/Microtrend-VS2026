/******************************************************************************/
// Low Points Usage
// Low Spend Usage
// Points Ranking
// Spend Ranking
// Active Cards
// Inactive Cards
// plu sales ranking
/******************************************************************************/
#include "..\SPOSLoyalty4Shared\Consolidation.h"
#include "..\SPOSLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPOSLoyalty4Shared\ReportConsolidationArray.h"
#include "..\SPOSLoyalty4Shared\SQLDb.h"
#include "..\SPOSLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPOSLoyalty4Shared\SQLTable_Account\SQLRecordSetAccountFull.h"
#include "..\SPOSLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
/******************************************************************************/
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "RepmanHandler.h"
#include "WorkingDlg.h"
#include "DbExportEditor.h"
#include "DbReportHandler.h"
/******************************************************************************/
#include "ReportAccumulator.h"
/******************************************************************************/

CReportAccumulator::CReportAccumulator ( const char* szLabel, const char* szAuditFile, CWnd* pParent )
	:  m_psReport(szLabel,FALSE)
	, m_ReportInfo(szLabel)
	, m_auditFile(szAuditFile)
{
	m_strReportLabel = szLabel;
	m_pParent = pParent;
	m_nReportType = m_ReportInfo.GetReportType();				// get report type from report label
	m_dGroupValue = 0.0;
	m_dReportValue = 0.0;
	m_nGroupQty = 0;
	m_nReportQty = 0;
}

/******************************************************************************/

CReportAccumulator::~CReportAccumulator()
{
}

/******************************************************************************/

bool CReportAccumulator::Show()
{
	m_psReport.SetReportInfo( &m_ReportInfo );
	if (m_psReport.DoModal() == IDCANCEL)
	{
		return FALSE;
	}

	CRepmanHandler repman ( m_DbReporter.GetReportTitle(), m_pParent );

	int nErrorNo = m_auditFile.ValidateFile();						// check main audit file ok to report on
	if ( nErrorNo == nREPORT_NOERROR )								// see if any errors so far
	{
		CDbExportEditor editor ( FALSE, NULL );		// get user added fields
		if (editor.EditLabel(m_strReportLabel) == FALSE)
		{
			nErrorNo = nREPORT_CANCEL;
		}
		else if (m_DbReporter.Read(m_strReportLabel) == FALSE)
		{
			nErrorNo = nREPORT_CANCEL;
		}
		if (nErrorNo == nREPORT_NOERROR)							// see if any errors so far
		{
			CString strReportFile = m_DbReporter.GetReportFilename();

			if (m_Report.Open(strReportFile) == TRUE)
			{
				if (m_auditFile.OpenReadOnly() == TRUE)					// reopen source audit file
				{
					CompileReport();
					m_auditFile.Close();
				}
				else
				{
					nErrorNo = nREPORT_OPENERROR;
				}

				m_Report.Close();

				if (nErrorNo == nREPORT_NOERROR)
				{
					repman.SetupSaveAs(m_DbReporter.GetSaveAsType(), m_DbReporter.GetSaveAsFile());
					nErrorNo = repman.DisplayReport(strReportFile, m_DbReporter.GetParamsFilename(), m_DbReporter.GetReportKey());
				}
			}
			else
			{
				nErrorNo = nREPORT_CREATEFAIL;
			}
		}
	}

	return repman.DisplayError ( nErrorNo );
}

//*******************************************************************

int CReportAccumulator::CompileReport()
{
	bool bCancelButton = FALSE;
	CWorkingDlg WorkingDlg("Processing Account Records for Report");
	WorkingDlg.Create();

	CReportConsolidationArray<CSortedDbReportItem> ConsolArray(FALSE, m_DbReporter.GetSortOn() == nSORTONCARDNO ? 1 : 0);

	m_dReportValue = m_dGroupValue = 0.0;
	m_nReportQty = m_nGroupQty = 0;

	// scan database first

	if (m_ReportInfo.GetNoZeroSkipFlag() == TRUE)				// require when < threshold requested
	{
		CString strWhere = "";
		CSQLBuilderBind BuilderBind{};
		CSQLRepositoryAccount repoAccount;
		repoAccount.PrepareExportWhere(m_DbReporter, -1, strWhere, BuilderBind);

		CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_ExportByParams{ &BuilderBind, strWhere });
		RecordSet.m_strSort = m_DbReporter.GetSQLOrderBy();
		
		int nCount = 0;

		CSQLRowAccountFull RowAccount;
		while (RecordSet.StepSelectAll(RowAccount) == TRUE)
		{
			if (m_DbReporter.NonSQLFilter(RowAccount) == TRUE)
			{
				CSortedDbReportItem item;
				m_DbReporter.GetSortItem(RowAccount, item, TRUE);
				item.m_nQty = 1;
				item.m_dVal = 0.0;
				ConsolArray.Consolidate(item);
			}

			if (WorkingDlg.SetCaption2RecordsChecked(++nCount) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}
		}

		if (TRUE == bCancelButton)
		{
			return nREPORT_CANCEL;
		}
	}

	// scan auditfile

	WorkingDlg.SetCaption1("Checking Audit Records");

	m_auditFile.JumpIn(m_ReportInfo.GetDateFromString());
	while (m_auditFile.ReadLine(&m_atc) == TRUE)
	{
		if (WorkingDlg.SetCaption2PercentDone(m_auditFile.GetPercentPosition()) == TRUE)
		{
			bCancelButton = TRUE;
			break;
		}

		{
			CServerDataFolderSetIndex FolderSetIndex = m_atc.GetFolderSetIndex();
			if (m_ReportInfo.CheckTerminalFilter(FolderSetIndex.m_nDbNo, FolderSetIndex.m_nSetNo, m_atc.GetTerminalNo()) == FALSE)
			{
				continue;
			}
		}

		if (m_ReportInfo.IsValid(&m_DbReporter, &m_atc) == TRUE)		// see if date in range
		{
			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(_atoi64(m_atc.GetCardNo()));
			RowAccount.SetGroupNo(m_atc.GetGroupNo());

			CSortedDbReportItem item;
			m_DbReporter.GetSortItem(RowAccount, item, FALSE);

			switch (m_nReportType)
			{
			case nACCUMULATE_LOWPOINTS:
			case nACCUMULATE_RANKPOINTS:
				item.m_nQty = 1;
				item.m_dVal = (double)m_atc.GetPointsTransaction();
				ConsolArray.Consolidate(item);
				break;

			case nACCUMULATE_LOWSPEND:
			case nACCUMULATE_RANKSPEND:
				item.m_nQty = 1;
				item.m_dVal = -(m_atc.GetTotalTransaction());
				ConsolArray.Consolidate(item);
				break;

			case nACCUMULATE_ACTIVE:
			case nACCUMULATE_INACTIVE:
				item.m_nQty = 1;
				item.m_dVal = 1.0;
				ConsolArray.Consolidate(item);
				break;
			}
		}
	}

	if ((ConsolArray.GetSize() == 0) || (TRUE == bCancelButton))
	{
		return nREPORT_NOMATCH;
	}

	// produce report

	int nTotalCount = 0;

	m_Report.SetStyle1(m_DbReporter.GetReportTitle(m_ReportInfo.GetThresholdText()));
	AddColumnText();

	CArray<int, int> arrayHeader;
	LoyaltyHeaderHelpers.CreateNumericHeader(m_DbReporter.m_strHeader, arrayHeader);

	if (m_nReportType == nACCUMULATE_RANKPOINTS || m_nReportType == nACCUMULATE_RANKSPEND)
	{
		WorkingDlg.SetCaption1("Preparing Ranking Positions");

		CReportConsolidationArray<CSortedDbReportRankingItem> ConsolArrayRanking(FALSE, m_DbReporter.GetSortOn() == nSORTONCARDNO ? 1 : 0);

		CString strRankingLabel = "";

		for (int nConsolIndex = 0; nConsolIndex < ConsolArray.GetSize(); nConsolIndex++)
		{
			WorkingDlg.SetCaption2PercentDone((nConsolIndex * 100L) / ConsolArray.GetSize(), FALSE);

			CSortedDbReportItem item;
			ConsolArray.GetAt(nConsolIndex, item);

			if (m_ReportInfo.CheckThreshold(item.m_dVal) == TRUE)
			{
				CSortedDbReportRankingItem rankingItem;
				rankingItem.m_nRanking = int(floor(item.m_dVal * 100.0));
				rankingItem.m_nGroupNo = item.m_nGroupNo;
				rankingItem.m_strSortField = item.m_strSortField;
				rankingItem.m_nUserID = item.m_nUserID;
				rankingItem.m_nConsolIndex = nConsolIndex;
				ConsolArrayRanking.Consolidate(rankingItem);
			}
		}

		WorkingDlg.SetCaption1("Preparing Report");

		double dLastValue = -999999.99;
		int nRank = 0;
		int nCount = 0;
		int nChartSize = m_ReportInfo.GetTop100ChartSize();

		for (int nRankingIndex = 0; nRankingIndex < ConsolArrayRanking.GetSize(); nRankingIndex++)
		{
			if (WorkingDlg.SetCaption2PercentDone((nRankingIndex * 100) / ConsolArrayRanking.GetSize()) == TRUE)
			{
				break;
			}

			CSortedDbReportRankingItem rankingItem;
			ConsolArrayRanking.GetAt(nRankingIndex, rankingItem);

			CSortedDbReportItem item;
			ConsolArray.GetAt(rankingItem.m_nConsolIndex, item);

			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(item.m_nUserID);

			CSQLRepositoryAccount repoAccount;
			bool bExist = (repoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE);
			
			bool bAllow = TRUE;

			if (FALSE == bExist)
			{
				bAllow = m_DbReporter.GetUnknownReqd();
			}
			else
			{
				CString strWhere = "";
				CSQLBuilderBind BuilderBind{};
				repoAccount.PrepareExportWhere(m_DbReporter, RowAccount.GetUserID(), strWhere, BuilderBind);

				CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_ExportByParams{ &BuilderBind, strWhere });
				RecordSet.m_strSort = m_DbReporter.GetSQLOrderBy();
				
				CSQLRowAccountFull RowTest;
				if (RecordSet.StepSelectAll(RowTest) == TRUE)
				{
					bAllow = m_DbReporter.NonSQLFilter(RowAccount, TRUE);
				}
				else
				{
					bAllow = FALSE;
				}
			}

			// invalid records may have been added from audit file
			// fiddle need to check for filters that would usually apply via SQL
			if (TRUE == bAllow)
			{
				if (item.m_dVal == dLastValue)
				{
					++nCount;
				}
				else
				{
					nRank = nRank + nCount + 1;

					if ((nChartSize != 0) && (nRank > nChartSize))
					{
						break;
					}

					dLastValue = item.m_dVal;
					nCount = 0;
				}

				SaveReportLine(arrayHeader, RowAccount, bExist, item.m_nQty, item.m_dVal, nRank);
			}
		}

		SaveTotals(arrayHeader, nREPORT_TOTAL, 0, nTotalCount, TRUE);
	}
	else
	{
		WorkingDlg.SetCaption1("Creating Report");

		int nGroupCount = 0;
		int nLastGroupNo = -1;

		for (int nConsolIndex = 0; nConsolIndex < ConsolArray.GetSize(); nConsolIndex++)
		{
			if (WorkingDlg.SetCaption2PercentDone((nConsolIndex * 100L) / ConsolArray.GetSize()) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}

			CSortedDbReportItem item;
			ConsolArray.GetAt(nConsolIndex, item);

			if (m_ReportInfo.CheckThreshold(item.m_dVal) == TRUE)
			{
				int nGroupNo = item.m_nGroupNo;
				__int64 nUserID = item.m_nUserID;
				CString strSortOn = item.m_strSortField;

				CSQLRowAccountFull RowAccount;
				RowAccount.SetUserID(item.m_nUserID);
				
				CSQLRepositoryAccount repoAccount;
				bool bExist = (repoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE);
				
				bool bAllow = TRUE;

				if (FALSE == bExist)
				{
					bAllow = m_DbReporter.GetUnknownReqd();
				}
				else
				{
					CString strWhere = "";
					CSQLBuilderBind BuilderBind{};
					repoAccount.PrepareExportWhere(m_DbReporter, RowAccount.GetUserID(), strWhere, BuilderBind);

					CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_ExportByParams{ &BuilderBind, strWhere });
					RecordSet.m_strSort = m_DbReporter.GetSQLOrderBy();
					
					CSQLRowAccountFull RowTest;
					if (RecordSet.StepSelectAll(RowTest) == TRUE)
					{
						bAllow = m_DbReporter.NonSQLFilter(RowAccount, TRUE);
					}
					else
					{
						bAllow = FALSE;
					}
				}

				if (TRUE == bAllow)
				{
					if (m_DbReporter.GetSortByGroup() == TRUE)
					{
						if (nConsolIndex == 0)
						{
							nLastGroupNo = nGroupNo;					// only need for first entry
						}

						if (nLastGroupNo != nGroupNo)
						{
							SaveTotals(arrayHeader, nREPORT_GROUP, nLastGroupNo, nGroupCount);
							nGroupCount = 0;
							nLastGroupNo = nGroupNo;
							m_dGroupValue = 0.0;
							m_nGroupQty = 0;
						}
					}

					SaveReportLine(arrayHeader, RowAccount, bExist, item.m_nQty, item.m_dVal);
					++nGroupCount;
					++nTotalCount;
				}
			}
		}

		if (m_DbReporter.GetSortByGroup() == TRUE)
		{
			SaveTotals(arrayHeader, nREPORT_GROUP, nLastGroupNo, nGroupCount);	// save last group if any
		}

		SaveTotals(arrayHeader, nREPORT_TOTAL, 0, nTotalCount);
	}

	return nREPORT_NOERROR;
}

//*******************************************************************

bool CReportAccumulator::SaveReportLine ( CArray<int,int>& arrayHeader, CSQLRowAccountFull& RowAccount, bool bExists, int nQty, double dValue, int nPos )
{
	if (m_ReportInfo.GetNoZeroSkipFlag() == TRUE)
	{
		--nQty;										// -1 to adjust consol array count, 1st entry forced due to non zero skip
	}

	CCSV csv ( '\t' );

	if (nPos > 0)									// ranking level
	{
		csv.Add(nPos);
	}

	if (bExists == FALSE)
	{
		m_DbReporter.AddOrderByData( &csv, RowAccount.GetFullname(), RowAccount.GetUserID());	// record not in database
	}
	else
	{
		m_DbReporter.AddOrderByData(&csv, RowAccount);				// record in database
	}

	switch ( m_nReportType )
	{
	case nACCUMULATE_LOWPOINTS:
	case nACCUMULATE_RANKPOINTS:
		csv.Add ( nQty );
		csv.Add ( dValue, 0 );
		break;

	case nACCUMULATE_LOWSPEND:
	case nACCUMULATE_RANKSPEND:
		csv.Add ( nQty );
		csv.Add ( dValue, System.GetDPValue() );
		break;

	case nACCUMULATE_ACTIVE:
	case nACCUMULATE_INACTIVE:
		break;
	}

	m_dGroupValue += dValue;
	m_dReportValue += dValue;

	m_nGroupQty += nQty;
	m_nReportQty += nQty;

	m_DbReporter.CreateReportLine( arrayHeader, &csv, RowAccount, bExists );

	return m_Report.WriteLine ( csv );
}

/**********************************************************************/

void CReportAccumulator::SaveTotals ( CArray<int,int>& arrayHeader, int nIndex, int nGroupNo, int nCount, bool bRankReport )
{
	if ((0 == nCount) && (nREPORT_GROUP == nIndex))
	{
		return;
	}

	CCSV csv ( '\t' );
	CCSV csvTabLine ( '\t' );
	CString strText = "";

	if ( nIndex == nREPORT_GROUP )
	{
		CSQLRowGroup RowGroup;
		RowGroup.SetGroupNo(nGroupNo);

		CString strGroupName = "";

		CSQLRepositoryGroup repoGroup;
		if (repoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			strGroupName = RowGroup.GetGroupName();
		}
		else
		{
			strGroupName.Format("Group %2.2d", nGroupNo);
		}

		strText.Format ( "<..>%s Total", 
			(const char*) strGroupName );
	}
	else
	{
		strText = "<..>Report Total";
	}

	if ( bRankReport == TRUE )
	{
		csv.Add ( "" );							// Pos.
		csvTabLine.Add ( "" );
	}

	m_DbReporter.AddOrderByTotalText ( strText, &csv, &csvTabLine, 0);

	switch ( m_nReportType )
	{
	case nACCUMULATE_LOWPOINTS:
	case nACCUMULATE_RANKPOINTS:
		if ( nIndex == nREPORT_GROUP )
		{
			csv.Add ( m_nGroupQty );
			csv.Add ( m_dGroupValue, 0 );
		}
		else
		{
			csv.Add ( m_nReportQty );
			csv.Add ( m_dReportValue, 0 );
		}
		csvTabLine.Add ( "<LI>" );
		csvTabLine.Add ( "<LI>" );
		break;

	case nACCUMULATE_LOWSPEND:
	case nACCUMULATE_RANKSPEND:
		if ( nIndex == nREPORT_GROUP )
		{
			csv.Add ( m_nGroupQty );
			csv.Add ( m_dGroupValue, System.GetDPValue() );
		}
		else
		{
			csv.Add ( m_nReportQty );
			csv.Add ( m_dReportValue, System.GetDPValue() );
		}
		csvTabLine.Add ( "<LI>" );
		csvTabLine.Add ( "<LI>" );
		break;

	case nACCUMULATE_ACTIVE:
	case nACCUMULATE_INACTIVE:
	default:
		break;
	}

	m_DbReporter.CreateTotalLine(arrayHeader, &csv, &csvTabLine, nIndex);

	m_Report.WriteLine(csvTabLine);
	m_Report.WriteLine(csv);
	
	CString strLineCount = "";
	strLineCount.Format("<..>(%d line%s)",
		nCount, 
		nCount == 1 ? "" : "s");

	m_Report.WriteLine(strLineCount);
	m_Report.WriteLine("");
}

/**************************************************************************/

void CReportAccumulator::AddColumnText()
{
	switch ( m_nReportType )
	{
	case nACCUMULATE_RANKPOINTS:
	case nACCUMULATE_RANKSPEND:
		m_Report.AddColumn ( "Rank", TA_LEFT, 150 );
		break;
	}

	m_DbReporter.AddOrderByColumnText ( &m_Report );		// add start of line column headers

	switch ( m_nReportType )
	{
	case nACCUMULATE_LOWPOINTS:
	case nACCUMULATE_RANKPOINTS:
		m_Report.AddColumn ( "Trans.", TA_RIGHT, 207 );
		m_Report.AddColumn ( "Points", TA_RIGHT, 207 );
		break;

	case nACCUMULATE_LOWSPEND:
	case nACCUMULATE_RANKSPEND:
		m_Report.AddColumn ( "Trans.", TA_RIGHT, 207 );
		m_Report.AddColumn ( "Spend", TA_RIGHT, 207 );
		break;

	case nACCUMULATE_ACTIVE:
	case nACCUMULATE_INACTIVE:
	default:
		break;
	}

	m_DbReporter.CreateReportHeader ( &m_Report );
}

/*****************************************************************************/
