/******************************************************************************/
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\LoyaltySubs.h"
#include "..\SPosLoyalty4Shared\SQLDb.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
/******************************************************************************/
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "RepmanHandler.h"
#include "RecSort.h"
#include "DbExportEditor.h"
#include "WorkingDlg.h"
/******************************************************************************/
#include "ReportStartingBalance.h"
/******************************************************************************/

CReportStartingBalance::CReportStartingBalance ( const char* szAuditFilename, CWnd* pParent )
	: m_dlgFilter( pParent)
	, m_auditFile(szAuditFilename)
{
	m_pParent = pParent;
	m_strReportLabel = "R009";
	Reset();
}

/******************************************************************************/

void CReportStartingBalance::Reset()
{
	for (int nType = 0; nType <= GRAND_TOTAL; nType++)
	{
		m_lTotalPoints[nType] = 0;
		m_dTotalPurse1[nType] = 0.0;
		m_dTotalPurse2[nType] = 0.0;
		m_nTotalCount[nType] = 0;
	}
}

/******************************************************************************/

bool CReportStartingBalance::Show()
{
	m_ReportInfo.SetReportLabel( m_strReportLabel );
	m_dlgFilter.SetReportInfo(&m_ReportInfo);
	if (m_dlgFilter.DoModal() == IDCANCEL)
	{
		return FALSE;
	}

	int nErrorNo = m_auditFile.ValidateFile();						// check main audit file ok to report on
	if ( nErrorNo == nREPORT_NOERROR )								// see if any errors so far
	{
		CDbExportEditor editor ( FALSE, NULL );
		if (editor.EditLabel(m_strReportLabel) == FALSE)
		{
			nErrorNo = nREPORT_CANCEL;
		}
		else if (m_DbReporter.Read(m_strReportLabel) == FALSE)
		{
			nErrorNo = nREPORT_CANCEL;
		}
	}

	CRepmanHandler repman ( m_DbReporter.GetReportTitle(), m_pParent );

	if ( nErrorNo == nREPORT_NOERROR )								// see if any errors so far
	{
		CString strReportFile = m_DbReporter.GetReportFilename();

		if ( m_Report.Open ( strReportFile ) == TRUE )
		{
			if ( m_auditFile.OpenReadOnly() == TRUE )					// reopen source audit file
			{
				nErrorNo = CompileReport();							// make report
				m_auditFile.Close();								// close audit file
			}
			else
			{
				nErrorNo = nREPORT_OPENERROR;
			}
			m_Report.Close();

			if ( nErrorNo == nREPORT_NOERROR )
			{
				repman.SetupSaveAs ( m_DbReporter.GetSaveAsType(), m_DbReporter.GetSaveAsFile() );
				nErrorNo = repman.DisplayReport( strReportFile, m_DbReporter.GetParamsFilename(), m_DbReporter.GetReportKey() );
			}
		}
		else
		{
			nErrorNo = nREPORT_CREATEFAIL;
		}
	}

	return repman.DisplayError ( nErrorNo );
}

//*****************************************************************************

int CReportStartingBalance::CompileReport()
{
	bool bCancelButton = FALSE;

	CWorkingDlg WorkingDlg ( "Creating List of Accounts" );
	WorkingDlg.Create();

	Reset();

	//CREATE LIST OF ALL USER IDS IN DATABASE

	CReportConsolidationArray<CSortedInt64WithFlag> arrayUserIDs;

	CSQLRecordSetAccountUserID RecordSetUserID(NULL,
		RSParams_AccountUserId_NormalNoParams{});
	{
		int nChecked = 0;
		__int64 nUserID = 0;
		while (RecordSetUserID.StepSelect(nUserID, TRUE) == TRUE)
		{
			if (WorkingDlg.SetCaption2RecordsChecked(++nChecked, TRUE) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}

			CSortedInt64WithFlag entry;
			entry.m_nInt64 = nUserID;
			entry.m_nFlag = 0;
			arrayUserIDs.Consolidate(entry);
		}
	}

	if (TRUE == bCancelButton)
	{
		return nREPORT_CANCEL;
	}

	//FLAG ALL CURRENT USER IDS THAT ARE INCLUDED IN FILTER

	CString strWhere = "";
	CSQLBuilderBind BuilderBind{};

	CSQLRepositoryAccount repoAccount;
	repoAccount.PrepareExportWhere(m_DbReporter, -1, strWhere, BuilderBind);

	{
		CSQLRecordSetAccountFull RecordSet(NULL, 
			RSParams_AccountFull_ExportByParams{ &BuilderBind, strWhere });

		int nChecked = 0;
		CSQLRowAccountFull RowAccount;
		
		WorkingDlg.SetCaption1("Applying Account Filter");
		
		while (RecordSet.StepSelectAll(RowAccount) == TRUE)
		{
			if (WorkingDlg.SetCaption2RecordsChecked(++nChecked, TRUE) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}

			//IF ACCOUNT MATCHES FILTER THEN FLAG IT AS INCLUDED
			if (m_DbReporter.NonSQLFilter(RowAccount, TRUE))
			{
				CSortedInt64WithFlag entry;
				entry.m_nInt64 = RowAccount.GetUserID();
				entry.m_nFlag = 1;
				arrayUserIDs.Consolidate(entry);
			}
		}
	}

	if (TRUE == bCancelButton)
	{
		return nREPORT_CANCEL;
	}

	CReportConsolidationArray<CSortedIntByInt64> arrayBalancePos;

	if (FindStartingBalances(
		arrayUserIDs,
		m_DbReporter.GetUnknownReqd(),
		m_auditFile,
		m_ReportInfo.GetDateSearchFromString(),
		m_ReportInfo.GetDateSearchToString(),
		m_ReportInfo.GetDateBalanceString(),
		"00:00",
		arrayBalancePos,
		WorkingDlg) == FALSE )
	{
		return nREPORT_CANCEL;
	}

	if (arrayBalancePos.GetSize() == 0)
	{
		return nREPORT_NOMATCH;
	}

	CArray<int, int> arrayHeader;
	LoyaltyHeaderHelpers.CreateNumericHeader(m_DbReporter.m_strHeader, arrayHeader);
	
	WorkingDlg.SetCaption1 ( "Creating report" );

	CString strReportTitle;
	strReportTitle.Format ( "%s ( %s )", 
		(const char*) m_DbReporter.GetReportTitle(),
		m_ReportInfo.GetDateBalanceString() );

	m_Report.SetStyle1 ( strReportTitle );

	AddColumnText();

	for ( int nIndex = 0 ; nIndex < arrayBalancePos.GetSize() ; nIndex++ )
	{
		if (WorkingDlg.SetCaption2RecordsOfTotal(nIndex, arrayBalancePos.GetSize(), TRUE) == TRUE)
		{
			bCancelButton = TRUE;
			break;
		}

		CSortedIntByInt64 item;
		arrayBalancePos.GetAt(nIndex, item);

		long lFilePos = item.m_nVal;

		if (lFilePos < 0L)									// database record entry not found in audit
		{
			SaveDatabaseLine(arrayHeader,item.m_nKey);		// get record number
		}
		else
		{
			m_auditFile.SetFilePos ( lFilePos  );
			if (m_auditFile.ReadLine(&m_atc) == TRUE)		// should exist to get here
			{
				bool bCalcNearestBalance = (m_ReportInfo.IsBeforeReportDate(&m_atc) == 1);
				SaveStartingBalanceLine(arrayHeader, m_atc.GetCardNo(), bCalcNearestBalance);
			}
		}
	}

	for (int nType = 0; nType <= GRAND_TOTAL; nType++)
	{
		SaveTotal(nType);
	}
	
	if (bCancelButton == TRUE)
	{
		return nREPORT_CANCEL;
	}
	
	return nREPORT_NOERROR;
}

//*****************************************************************************
// use data from an audit line

void CReportStartingBalance::SaveStartingBalanceLine(CArray<int, int>& arrayHeader, CString strCardNo, bool bCalcBalance)
{
	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserIDString(strCardNo);

	CSQLRepositoryAccount repoAccount;
	bool bExists = (repoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE);
	
	if (m_DbReporter.NonSQLFilter(RowAccount, bExists) == FALSE)
	{
		return;
	}

	CString strDate;

	switch (m_atc.GetSourceType())
	{
	case AUDIT_POS:
	case AUDIT_EXT:
		strDate = m_atc.GetDateLastUsed();
		break;

	default:
		strDate = m_atc.GetDateFiled();
		break;
	}

	switch (m_atc.GetApplicationNo())
	{
	case APPNO_DELETE:
	case APPNO_BATCHDELETE:
		CSSDate date(strDate);
		if (date < m_ReportInfo.GetDateBalance())
		{
			return;											// card deleted before reference date
		}
		break;
	}

	CString strNearestDate = "";
	long   lPoints = m_atc.GetPoints();
	double dPurse1 = m_atc.GetPurse1();
	double dPurse2 = m_atc.GetPurse2();

	if (bCalcBalance == TRUE)
	{
		lPoints -= m_atc.GetPointsTransaction();
		dPurse1 -= m_atc.GetPurse1Transaction();
		dPurse2 -= m_atc.GetPurse2Transaction();
		strNearestDate = m_atc.GetDateFiled();
		strDate = "";
	}

	CCSV csv('\t');

	if (bExists == FALSE)
	{
		m_DbReporter.AddOrderByData(&csv, m_atc.GetCardName(), _atoi64(m_atc.GetCardNo()));	// record does not exist
	}
	else
	{
		m_DbReporter.AddOrderByData(&csv, RowAccount);											// records exist
	}

	csv.Add(strDate);
	csv.Add(strNearestDate);
	csv.Add(CAuditReportTexts::GetReportSourceText(m_atc.GetSourceType(), m_atc.GetApplicationNo(), m_atc.GetTerminalNo()));	// source text

	csv.Add(::SignedFormatLongValue(lPoints));
	csv.Add(::SignedFormatDoubleValue(dPurse1));

	if (System.GetEnablePurse2Flag() == TRUE)
	{
		csv.Add(::SignedFormatDoubleValue(dPurse2));
	}

	m_DbReporter.CreateReportLine(arrayHeader, &csv, RowAccount, bExists);

	if (m_Report.WriteLine(csv) == TRUE)
	{
		int nType = 0;
		if (bExists == TRUE)
		{
			nType = (bCalcBalance == TRUE) ? NORMAL_ESTIMATED : NORMAL;
		}
		else
		{
			nType = (bCalcBalance == TRUE) ? DELETED_ESTIMATED : DELETED;
		}

		m_lTotalPoints[nType] += lPoints;
		m_dTotalPurse1[nType] += dPurse1;
		m_dTotalPurse2[nType] += dPurse2;
		++m_nTotalCount[nType];

		m_lTotalPoints[GRAND_TOTAL] += lPoints;
		m_dTotalPurse1[GRAND_TOTAL] += dPurse1;
		m_dTotalPurse2[GRAND_TOTAL] += dPurse2;
		++m_nTotalCount[GRAND_TOTAL];
	}
}

//*****************************************************************************
// to get to here the 
// record is in database but no lines have been found in the audit

void CReportStartingBalance::SaveDatabaseLine ( CArray<int,int>& arrayHeader, __int64 nCardNo ) 
{
	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserID(nCardNo);
	
	CSQLRepositoryAccount repoAccount;
	bool bExists = (repoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE);
	
	if (m_DbReporter.NonSQLFilter( RowAccount, bExists) == FALSE)
	{
		return;
	}

	CCSV csv ( '\t' );

	m_DbReporter.AddOrderByData ( &csv, RowAccount );

	CSSDate date;
	CSSTime time;
	RowAccount.GetLastUsed ( date, time );

	if ( date >= m_ReportInfo.GetDateBalance() )				// so can't use as balances as they refer to or after reference date
	{
		csv.Add ( "" );									// no last date
		csv.Add ( "" );									// no next date
		csv.Add ( "<..>No audit data" );				// source text
		csv.Add ( "" );									// points
		csv.Add ( "" );									// purse
	}
	else
	{
		csv.Add ( date.GetDate() );						// date last used
		csv.Add ( "" );
		csv.Add ( "Database" );

		CSQLRowGroup RowGroup;
		RowGroup.SetGroupNo(RowAccount.GetGroupNo());

		long   lPoints = RowAccount.GetPoints();		// Points total
		double dPurse1 = RowAccount.GetPurse1Balance();	// Purse1 total
		double dPurse2 = LoyaltySubs.GetPurse2NextSpendBalance(RowAccount,RowGroup,TRUE);	// Purse2 total

		csv.Add ( ::SignedFormatLongValue ( lPoints ) );
		m_lTotalPoints[NORMAL] += lPoints;
		m_lTotalPoints[GRAND_TOTAL] += lPoints;

		csv.Add ( ::SignedFormatDoubleValue ( dPurse1 ) );
		m_dTotalPurse1[NORMAL] += dPurse1;
		m_dTotalPurse1[GRAND_TOTAL] += dPurse1;

		if ( System.GetEnablePurse2Flag() == TRUE )	
		{
			csv.Add ( ::SignedFormatDoubleValue ( dPurse2 ) );
			m_dTotalPurse2[NORMAL] += dPurse2;
			m_dTotalPurse2[GRAND_TOTAL] += dPurse2;
		}
	}

	m_DbReporter.CreateReportLine ( arrayHeader, &csv, RowAccount );

	if ( m_Report.WriteLine ( csv ) == TRUE )
	{
		++m_nTotalCount[NORMAL];
		++m_nTotalCount[GRAND_TOTAL];
	}
}

//**************************************************************************

void CReportStartingBalance::AddColumnText()
{
	m_DbReporter.AddOrderByColumnText ( &m_Report );		// add start of line column headers

	m_Report.AddColumn ( "LastUsed", TA_LEFT, 219 );
	m_Report.AddColumn ( "NextUsed", TA_LEFT, 219 );
	m_Report.AddColumn ( "Source",   TA_LEFT, 250 );
	m_Report.AddColumn ( "Points", TA_RIGHT, 180 );
	m_Report.AddColumn ( "Purse1", TA_RIGHT, 195 );
	if ( System.GetEnablePurse2Flag() == TRUE )	m_Report.AddColumn ( "Purse2", TA_RIGHT, 195 );

	m_DbReporter.CreateReportHeader ( &m_Report );
}

//**************************************************************************

void CReportStartingBalance::SaveTotal(int nType)
{
	int nCount = 0;
	CString strText;

	switch (nType)
	{
	case NORMAL:
		strText = "<..>Starting Balances";
		nCount = m_nTotalCount[NORMAL];
		break;

	case NORMAL_ESTIMATED:
		strText = "<..>Estimated Balances";
		nCount = m_nTotalCount[NORMAL_ESTIMATED];
		break;

	case DELETED:
		strText = "<..>Deleted Balances";
		nCount = m_nTotalCount[DELETED];
		break;

	case DELETED_ESTIMATED:
		strText = "<..>Estimated Deleted";
		nCount = m_nTotalCount[DELETED_ESTIMATED];
		break;

	case GRAND_TOTAL:
	{
		strText = "<..>Report Total";

		for (int i = 0; i < GRAND_TOTAL; i++)
		{
			nCount += m_nTotalCount[i];
		}
	}
	break;
	}

	CCSV csv('\t');
	CCSV csvTabLine('\t');

	m_DbReporter.AddOrderByTotalText(strText, &csv, &csvTabLine, nCount);

	csv.Add("");
	csv.Add("");
	csv.Add("");
	csvTabLine.Add("");
	csvTabLine.Add("");
	csvTabLine.Add("");

	csv.Add(::SignedFormatLongValue(m_lTotalPoints[nType]));
	csvTabLine.Add("<LI>");

	csv.Add(::SignedFormatDoubleValue(m_dTotalPurse1[nType]));
	csvTabLine.Add("<LI>");

	if (System.GetEnablePurse2Flag() == TRUE)
	{
		csv.Add(::SignedFormatDoubleValue(m_dTotalPurse2[nType]));
		csvTabLine.Add("<LI>");
	}

	if (nType == NORMAL)
	{
		m_Report.WriteLine(csvTabLine);
	}
	else if (nType == GRAND_TOTAL)
	{
		int nCount = 0;
		for (int i = 0; i < GRAND_TOTAL; i++)
		{
			if (m_nTotalCount[i] > 0)
			{
				++nCount;
			}
		}
		if (nCount < 2)
		{
			return;
		}

		m_Report.WriteLine(csvTabLine);				// need grand total line
	}
	else
	{
		// no data
		if (m_nTotalCount[nType] == 0)
		{
			return;
		}
	}

	m_Report.WriteLine(csv);
}

//**************************************************************************
//ALSO USED BY CASHLESS LIABILITY REPORT
//CURRENT IDS PASSED IN ARRAY WITH FLAG SET IF IN FILTER
//**************************************************************************

bool CReportStartingBalance::FindStartingBalances(
	CReportConsolidationArray<CSortedInt64WithFlag>& arrayCurrentIDs, 
	bool bIncludeUnknowns,
	CLoyaltyAuditFile& auditFile,
	CString strSearchStartDate,
	CString strSearchEndDate,
	CString strReportStartDate,
	CString strReportStartTime,
	CReportConsolidationArray<CSortedIntByInt64>& arrayBalancePos,
	CWorkingDlg& WorkingDlg)
{
	arrayBalancePos.RemoveAll();
	WorkingDlg.SetCaption1("Scanning audit file");
	auditFile.JumpIn(strSearchStartDate);

	CAuditRecord atc;

	CSSDate SearchStartDate(strSearchStartDate);
	CSSDate SearchEndDate(strSearchEndDate);
	CSSDate ReportStartDate(strReportStartDate);
	CSSTime ReportStartTime(strReportStartTime);

	while (TRUE)
	{
		if (WorkingDlg.SetCaption2PercentDone(auditFile.GetPercentPosition(), TRUE) == TRUE)
		{
			return FALSE;
		}

		long lStartFilePos = auditFile.GetFilePos();			// position in file to start from
		if (auditFile.ReadLine(&atc) == FALSE)
		{
			break;
		}

		CString strAuditDate;

		switch (atc.GetSourceType())
		{
		case AUDIT_POS:
		case AUDIT_EXT:
			strAuditDate = atc.GetDateLastUsed();
			break;

		default:
			strAuditDate = atc.GetDateFiled();
			break;
		}

		CSSDate dateAudit(strAuditDate);

		if ((dateAudit.IsSet() == FALSE ) || (dateAudit < SearchStartDate))
		{
			continue;
		}

		if (dateAudit > SearchEndDate)
		{
			continue;
		}

		bool bBeforeReportStart = FALSE;

		if ( dateAudit < ReportStartDate)
		{
			bBeforeReportStart = TRUE;
		}
		else if (dateAudit == ReportStartDate)
		{
			if (strReportStartTime != "00:00")
			{
				CSSTime timeAudit;
				switch (atc.GetSourceType())
				{
				case AUDIT_POS:
				case AUDIT_EXT:
					timeAudit = atc.GetTimeLastUsed();
					break;

				default:
					timeAudit = atc.GetTimeFiled();
					break;
				}
				
				if ((timeAudit.IsSet() == TRUE) && (timeAudit < ReportStartTime))
				{
					bBeforeReportStart = TRUE;
				}
			}
		}

		__int64 nCardNo = _atoi64(atc.GetCardNo());
		
		bool bInFilter = FALSE;

		CSortedInt64WithFlag item;
		item.m_nInt64 = nCardNo;

		int nPos = 0;
		if (arrayCurrentIDs.Find(item, nPos) == TRUE)
		{
			CSortedInt64WithFlag entry;
			arrayCurrentIDs.GetAt(nPos, entry);
			bInFilter = (entry.m_nFlag == 1);
		}
		else
		{
			bInFilter = bIncludeUnknowns;

			if (TRUE == bInFilter)
			{
				switch(atc.GetApplicationNo())
				{
				case APPNO_DELETE:
				case APPNO_BATCHDELETE:
					{
						int nPos = 0;
						CSortedIntByInt64 item;
						item.m_nKey = nCardNo;
						if (arrayBalancePos.Find(item, nPos) == TRUE)
						{
							arrayBalancePos.RemoveAt(nPos);
						}
						bInFilter = FALSE;
					}
					break;
				}
			}
		}

		if (TRUE == bInFilter)
		{
			CSortedIntByInt64 item;
			item.m_nKey = nCardNo;
			item.m_nVal = lStartFilePos;

			int nPos = 0;
			bool bExists = arrayBalancePos.Find(item, nPos);

			if ((TRUE == bBeforeReportStart) && (TRUE == bExists))
			{
				arrayBalancePos.SetAt(nPos, item);
			}

			if (FALSE == bExists)
			{
				arrayBalancePos.InsertAt(nPos, item);
			}
		}
	}

	//NOW ADD ANY MISSING CURRENT IDS TO SORTER
	for (int nIndex = 0; nIndex < arrayCurrentIDs.GetSize(); nIndex++)
	{
		CSortedInt64WithFlag entry;
		arrayCurrentIDs.GetAt(nIndex, entry);
		
		if (entry.m_nFlag == 1)			// only current ids
		{
			CSortedIntByInt64 item;
			item.m_nKey = entry.m_nInt64;
			item.m_nVal = -1;

			int nPos = 0;
			if (arrayBalancePos.Find(item, nPos) == FALSE)
			{
				arrayBalancePos.InsertAt(nPos, item);
			}
		}
	}

	return TRUE;
}

//**************************************************************************
