/******************************************************************************/
#include "..\SmartPay4Shared\RecSort.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_BioRegister\SQLRepositoryBioRegister.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/******************************************************************************/
#include "..\SmartPay4ManagerBgnd\DbReportFields.h"
/******************************************************************************/
#include "DbExportEditor.h"
#include "ReportStartingBalance.h"
/******************************************************************************/

CReportStartingBalance::CReportStartingBalance ( const char* szAuditFilename, CWnd* pParent )
	: m_dlgFilter ( pParent )
	, m_auditFile(szAuditFilename)
{
	m_pParent = pParent;
	m_strReportLabel = "R009";
	m_bAuditFileChecked = FALSE;
	ResetGroupTotals();
	ResetGrandTotals();
}

/******************************************************************************/

void CReportStartingBalance::ResetGroupTotals()
{
	for (int nType = 0; nType <= GRAND_TOTAL; nType++)
	{
		m_arrayTotals[0][nType].m_nPoints = 0;
		m_arrayTotals[0][nType].m_dPurse1 = 0.0;
		m_arrayTotals[0][nType].m_dPurse2 = 0.0;
		m_arrayTotals[0][nType].m_dPurse3 = 0.0;
		m_arrayTotals[0][nType].m_nCount = 0;
	}
}

/******************************************************************************/

void CReportStartingBalance::ResetGrandTotals()
{
	for (int nType = 0; nType <= GRAND_TOTAL; nType++)
	{
		m_arrayTotals[1][nType].m_nPoints = 0;
		m_arrayTotals[1][nType].m_dPurse1 = 0.0;
		m_arrayTotals[1][nType].m_dPurse2 = 0.0;
		m_arrayTotals[1][nType].m_dPurse3 = 0.0;
		m_arrayTotals[1][nType].m_nCount = 0;
	}
}

/******************************************************************************/

bool CReportStartingBalance::Show()
{
	int nErrorNo = nREPORT_NOERROR;

	if (m_bAuditFileChecked == FALSE)						// see if audit has been check yet
	{
		if (m_auditFile.Open() == FALSE)
		{
			nErrorNo = nREPORT_NOSOURCE;					// no source data found
		}
		else
		{
			CSSDate date;
			m_bAuditFileChecked = m_auditFile.GetFirstDate(date);	// try & get first date in audit file
			m_auditFile.Close();							// lose audit file

			if (m_bAuditFileChecked == FALSE)					// see if have first date in audit file
			{
				nErrorNo = nREPORT_INVALIDSOURCE;				// exit - invalid source
			}
		}
	}

// get user added fields

	if (nErrorNo == nREPORT_NOERROR)							// see if any error so far
	{
		m_dlgFilter.SetLabel(m_strReportLabel);
		if (m_dlgFilter.DoModal() == IDCANCEL)
		{
			return FALSE;
		}

		CDbExportEditor editor{};
		if (editor.EditExportLabel(m_strReportLabel) == FALSE)
		{
			return FALSE;
		}

		if (m_DbReporter.Read(m_strReportLabel) == FALSE)
		{
			return FALSE;
		}
	}

// setup repman

	CRepmanHandler repman ( m_DbReporter.GetReportTitle(), m_pParent );
	if ( nErrorNo == nREPORT_NOERROR )							// see if any errors so far
	{
		CString strReportFile = m_DbReporter.GetReportFilename();

		if ( ( nErrorNo = Create ( strReportFile ) ) == nREPORT_NOERROR )
		{
			repman.SetupSaveAs ( m_DbReporter.GetSaveAsType(), m_DbReporter.GetSaveAsFile() );
			nErrorNo = repman.DisplayReport ( strReportFile, m_DbReporter.GetParamsFilename(), m_DbReporter.GetReportKey() );
		}
	}

	return repman.DisplayError ( nErrorNo );
}

//*******************************************************************

int CReportStartingBalance::Create ( const char* szReportFile )
{
	if (m_Report.Open(szReportFile) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	int nReply = nREPORT_NOERROR;

	if (m_auditFile.Open() == FALSE)							// reopen audit source file
	{
		nReply = nREPORT_OPENERROR;
	}
	else
	{
		nReply = CompileReport();
		m_auditFile.Close();									// close audit file
	}
	m_Report.Close();

	return nReply;
}

//*****************************************************************************

int CReportStartingBalance::CompileReport()
{
	bool bCancelButton = FALSE;

	CWorkingDlg progress ( szMSG_SCANNINGAUDIT );
	progress.Create();

	ResetGroupTotals();
	ResetGrandTotals();

	CReportConsolidationArray<CStartingBalanceSortItem> accountsByUserID(FALSE, 0);
	CReportConsolidationArray<CStartingBalanceSortItem> accountsByGroupNo(FALSE, 1);

	m_auditFile.JumpIn ( m_dlgFilter.m_dateBegin.GetDate() );

	progress.SetCaption1("Checking Audit Records");

	while ( TRUE )
	{
		if (progress.SetCaption2PercentDone(m_auditFile.GetPercentPosition()) == TRUE)
		{
			bCancelButton = TRUE;
			break;
		}

		int nStartFilePos = m_auditFile.GetFilePos();			// position in file to start from
		if (m_auditFile.ReadLine(&m_atc) == FALSE)
		{
			break;
		}

		__int64 nUserID = _atoi64(m_atc.GetUserID());
		CStartingBalanceSortItem SortItem;

		if ( nUserID != 0 )
		{
			SortItem.m_nUserID = nUserID;
			SortItem.m_nFilePos = nStartFilePos;
			SortItem.m_nGroupNo = m_atc.GetGroupNo();

			switch ( m_dlgFilter.IsValidAtc ( &m_DbReporter, &m_atc ) )
			{
			case 1:
				// date before reference date, overwrite entry if exists
				accountsByUserID.Consolidate(SortItem);
				break;

			case 2:											// date = reference date
			case 3:											// date after reference date
				{
					// only add if new entry
					int nPos = 0;
					if (accountsByUserID.Find(SortItem,nPos) == FALSE )
					{
						accountsByUserID.InsertAt(nPos,SortItem);
					}
				}
				break;
				
			case -1:										// date 1 Year before reference date
			case -2:										// date 1 year after reference date
			case 0:											// CardNo / GroupNo out of range
				break;
			}
		}
	}

	if (bCancelButton == TRUE)
	{
		return nREPORT_CANCEL;
	}

	progress.SetCaption1 ( szMSG_SCANNINGDATABASE );

	CString strWhere = "";
	CSQLBuilderBind BuilderBind{};
	CSQLRepositoryAccount RepoAccount;
	RepoAccount.PrepareExportWhere(m_DbReporter, -1, strWhere, BuilderBind);

	CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_ByParams{ &BuilderBind, strWhere });
	RecordSet.m_strSort = m_DbReporter.GetSQLOrderBy();
	
	int nChecked = 0;
	CSQLRowAccountFull RowAccount;
	while (RecordSet.StepSelectAll(RowAccount) == TRUE)
	{
		if (progress.SetCaption2RecordsChecked(++nChecked) == TRUE)
		{
			bCancelButton = TRUE;
			break;
		}

		switch ( m_dlgFilter.IsValidRecord ( RowAccount, &m_DbReporter ) )		//IssuedDate & LastUsedDate < reference date
		{
		case 1:														// lastused before Reference date
		case 2:														// lastused = Reference date
		case 3:														// lastused after Reference date
			{
				CStartingBalanceSortItem SortItem;
				SortItem.m_nUserID = RowAccount.GetUserID();

				// only add if no previous entry
				int nPos = 0;
				if (accountsByUserID.Find(SortItem, nPos) == FALSE)
				{
					SortItem.m_nGroupNo = RowAccount.GetGroupNo();
					SortItem.m_nFilePos = -1;
					accountsByUserID.InsertAt(nPos, SortItem);
				}
			}
			break;
			
		case -3:													// Hotlisted card was replaced before reference date
//		case -2:													// issued after starting balance reference date
		case 0:														// CardNo / GroupNo out of range
//		default:
			break;
		}
	}
	
	if ((TRUE == bCancelButton) || (accountsByUserID.GetSize() == 0))
	{
		return nREPORT_NOMATCH;
	}

	//SORT ACCOUNTS BY GROUP NO
	if (TRUE == m_dlgFilter.m_bGroupSort)
	{
		progress.SetCaption1("Sorting Accounts by Group");

		CStartingBalanceSortItem SortItem;
		for (int nIndex = 0; nIndex < accountsByUserID.GetSize(); nIndex++)
		{
			{
				int nPercentPosition = (nIndex * 100) / accountsByUserID.GetSize();

				if (progress.SetCaption2PercentDone(nPercentPosition) == TRUE)
				{
					bCancelButton = TRUE;
					break;
				}
			}

			accountsByUserID.GetAt(nIndex, SortItem);
			accountsByGroupNo.Consolidate(SortItem);
		}
		accountsByUserID.RemoveAll();
	}

	if ( TRUE == bCancelButton)
	{
		return nREPORT_NOMATCH;
	}

// create report

	CReportConsolidationArray<CStartingBalanceSortItem>* pAccountList;
	
	if (TRUE == m_dlgFilter.m_bGroupSort)
	{
		pAccountList = &accountsByGroupNo;
	}
	else
	{
		pAccountList = &accountsByUserID;
	}

	progress.SetCaption1 ( szMSG_CREATINGREPORT );

	CString strReportTitle = "";
	strReportTitle.Format ( "%s ( %s )", 
		(const char*) m_DbReporter.GetReportTitle(), 
		(const char*) m_dlgFilter.m_dateReference.GetDate() );

	m_Report.SetStyle1 ( strReportTitle );

	AddColumnText();

	bool bNeedGroupTotal = FALSE;
	int nLastGroupNo = -1;

	for (int nIndex = 0; nIndex < pAccountList -> GetSize(); nIndex++)
	{
		{
			int nPercentPosition = (nIndex * 100) / pAccountList->GetSize();

			if (progress.SetCaption2PercentDone(nPercentPosition) == TRUE)
			{
				bCancelButton = TRUE;
				break;
			}
		}

		CStartingBalanceSortItem SortItem;
		pAccountList->GetAt(nIndex, SortItem);

		int nGroupNo = SortItem.m_nGroupNo;
		if ( nGroupNo != nLastGroupNo )
		{
			if ( TRUE == bNeedGroupTotal )
			{
				for ( int nType = 0; nType <= GRAND_TOTAL; nType++ )
				{
					SaveTotal ( 0, nType );
				}

				bNeedGroupTotal = FALSE;
				ResetGroupTotals();
				m_Report.WriteLine("");
				m_Report.WriteLine("");
			}

			nLastGroupNo = nGroupNo;
		}

		if (SortItem.m_nFilePos < 0)						// database record entry not found in audit
		{
			if (SaveDatabaseLine(SortItem.m_nUserID, nGroupNo) == TRUE)
			{
				bNeedGroupTotal = TRUE;
			}
		}
		else
		{
			m_auditFile.SetFilePos(SortItem.m_nFilePos);
			if (m_auditFile.ReadLine(&m_atc) == TRUE)			// should exist to get here
			{
				int nAtcDateRange = m_dlgFilter.IsValidAtc(&m_DbReporter, &m_atc);	// must be valid audit line to get here
				bool bCalcNearestBalance = (nAtcDateRange == 2 || nAtcDateRange == 3) ? TRUE : FALSE;		// (1)date < Reference Date, (2)date = referenecdate, (3)date > ReferenceDate(need to reverse calc what balance was before transaction )		

				if (SaveStartingBalanceLine(m_atc.GetUserID(), bCalcNearestBalance, nGroupNo) == TRUE)
				{
					bNeedGroupTotal = TRUE;
				}
			}
		}
	}

	if ( (TRUE == m_dlgFilter.m_bGroupSort) && (TRUE == bNeedGroupTotal) )
	{
		for (int nType = 0; nType <= GRAND_TOTAL; nType++)
		{
			SaveTotal(0, nType);
		}
		m_Report.WriteLine("");
		m_Report.WriteLine("");
	}
	
	for (int nType = 0; nType <= GRAND_TOTAL; nType++)
	{
		SaveTotal(1,nType);
	}

	return ( bCancelButton == TRUE ) ? nREPORT_CANCEL : nREPORT_NOERROR;
}

//*****************************************************************************
// use data from an audit line

bool CReportStartingBalance::SaveStartingBalanceLine(const char* szCardNo, bool bCalcBalance, int nGroupNo)
{
	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserIDString(szCardNo);

	CSQLRepositoryAccount RepoAccount;
	bool bExists = (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE);

	if (m_DbReporter.NonSQLFilter(RowAccount, bExists) == FALSE)
	{
		return FALSE;
	}

	CString strDate = "";
	switch (m_atc.GetSourceType())
	{
	case AUDIT_POS:
	case AUDIT_KIOSKLINK:
	case AUDIT_VENDING:
	case AUDIT_EXT:
		strDate = m_atc.GetDateLastUsed();
		break;

	default:
		strDate = m_atc.GetDateFiled();
		break;
	}

	if (m_atc.IsDeleteAppNo() == TRUE)							// Apno = delete \ batch delete \ import delete
	{
		// card deleted before reference date
		CSSDate date(strDate);
		if (date < m_dlgFilter.m_dateReference)
		{
			return FALSE;
		}
	}

	CString strNearestDate = "";
	int   nPoints = m_atc.GetCurrentPoints();
	double dPurse1 = m_atc.GetCurrentPurse1Total();
	double dPurse3 = m_atc.GetCurrentPurse3Total();

	if (bCalcBalance == TRUE)
	{
		nPoints -= m_atc.GetTransactionPoints();
		dPurse1 -= m_atc.GetTransactionPurse1Total();
		dPurse3 -= m_atc.GetTransactionPurse3Total();
		strNearestDate = m_atc.GetDateFiled();
		strDate = "";
	}

	
	m_DbReporter.ResetSpecialFields();
	m_DbReporter.AddSpecialField(strDate);
	m_DbReporter.AddSpecialField(strNearestDate);
	m_DbReporter.AddSpecialField(CAuditReportTexts::GetReportSourceText(m_atc.GetSourceType(), m_atc.GetApplicationNo(), m_atc.GetTerminalNo(), m_atc.GetAuditLineExtraText()));

	if (System.GetActiveEnablePointsFlag() == TRUE)
	{
		m_DbReporter.AddSpecialField(::FormatIntValue(nPoints));
	}

	m_DbReporter.AddSpecialField(::FormatDoubleValue(dPurse1));

	if (System.GetEnablePurse3Flag() == TRUE)
	{
		m_DbReporter.AddSpecialField(::FormatDoubleValue(dPurse3));
	}

	CCSV csv('\t');
	m_DbReporter.AppendDbFieldsToCSVLine(RowAccount, &csv, bExists, _atoi64(szCardNo), m_atc.GetUserName(),nGroupNo);

	bool bResult = m_Report.WriteLine(csv);

	if ( TRUE == bResult )
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

		for (int n = 0; n <= 1; n++)
		{
			m_arrayTotals[n][nType].m_nPoints += nPoints;
			m_arrayTotals[n][nType].m_dPurse1 += dPurse1;
			m_arrayTotals[n][nType].m_dPurse3 += dPurse3;
			m_arrayTotals[n][nType].m_nCount++;

			m_arrayTotals[n][GRAND_TOTAL].m_nPoints += nPoints;
			m_arrayTotals[n][GRAND_TOTAL].m_dPurse1 += dPurse1;
			m_arrayTotals[n][GRAND_TOTAL].m_dPurse3 += dPurse3;
			m_arrayTotals[n][GRAND_TOTAL].m_nCount++;
		}
	}

	return bResult;
}

//*****************************************************************************
// to get to here the 
// record is in database but no lines have been found in the audit

bool CReportStartingBalance::SaveDatabaseLine ( __int64 nUserID, int nGroupNo ) 
{
	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserID(nUserID);

	CSQLRepositoryAccount RepoAccount;
	bool bExists = ( RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE );

	if (m_DbReporter.NonSQLFilter(RowAccount, bExists) == FALSE)
	{
		return FALSE;
	}

	m_DbReporter.ResetSpecialFields();

	CSSDate date(RowAccount.GetPurse1LastSpendDate());				// has been used after reference date
	if (date >= m_dlgFilter.m_dateReference)						// so can't use as balances as they refer to or after reference date
	{
		m_DbReporter.AddSpecialField("");								// no last date
		m_DbReporter.AddSpecialField("");								// no next date
		m_DbReporter.AddSpecialField("<..>No audit data");				// source text

		if (System.GetActiveEnablePointsFlag() == TRUE)
		{
			m_DbReporter.AddSpecialField("");								// points
		}

		m_DbReporter.AddSpecialField("");									// purse1

		if (System.GetEnablePurse3Flag() == TRUE)
		{
			m_DbReporter.AddSpecialField("");								// purse3
		}
	}
	else
	{
		m_DbReporter.AddSpecialField(date.GetDate());						// date last used
		m_DbReporter.AddSpecialField("");
		m_DbReporter.AddSpecialField("Database");

		int    nPoints = RowAccount.GetPoints();	// Points total
		double dPurse1 = RowAccount.GetPurse1Balance();	// Purse1 total
		double dPurse3 = RowAccount.GetPurse3Balance();	// Purse3 total

		if (System.GetActiveEnablePointsFlag() == TRUE)
		{
			m_DbReporter.AddSpecialField(::FormatIntValue(nPoints));

			for (int n = 0; n <= 1; n++)
			{
				m_arrayTotals[n][NORMAL].m_nPoints += nPoints;
				m_arrayTotals[n][GRAND_TOTAL].m_nPoints += nPoints;
			}
		}

		m_DbReporter.AddSpecialField(::FormatDoubleValue(dPurse1));
		
		for (int n = 0; n <= 1; n++)
		{
			m_arrayTotals[n][NORMAL].m_dPurse1 += dPurse1;
			m_arrayTotals[n][GRAND_TOTAL].m_dPurse1 += dPurse1;
		}
		
		if (System.GetEnablePurse3Flag() == TRUE)
		{
			m_DbReporter.AddSpecialField(::FormatDoubleValue(dPurse3));
			
			for (int n = 0; n <= 1; n++)
			{
				m_arrayTotals[n][NORMAL].m_dPurse3 += dPurse3;
				m_arrayTotals[n][GRAND_TOTAL].m_dPurse3 += dPurse3;
			}
		}
	}

	CCSV csv('\t');
	m_DbReporter.AppendDbFieldsToCSVLine( RowAccount, &csv, TRUE, 0, "", nGroupNo);

	bool bResult = m_Report.WriteLine(csv);

	if ( TRUE == bResult )
	{
		for (int n = 0; n <= 1; n++)
		{
			m_arrayTotals[n][NORMAL].m_nCount++;
			m_arrayTotals[n][GRAND_TOTAL].m_nCount++;
		}
	}

	return bResult;
}

//**************************************************************************

void CReportStartingBalance::AddColumnText()
{
	m_DbReporter.AddOrderByColumnText();		// add start of line column headers
	m_DbReporter.AddSpecialColumn(DbReportFieldsSpecial::StartBalLastUsed);
	m_DbReporter.AddSpecialColumn(DbReportFieldsSpecial::StartBalNextUsed);
	m_DbReporter.AddSpecialColumn(DbReportFieldsSpecial::StartBalSource);
	
	if (TRUE == System.GetActiveEnablePointsFlag())
	{
		m_DbReporter.AddSpecialColumn(DbReportFieldsSpecial::StartBalPoints);
	}

	m_DbReporter.AddSpecialColumn(DbReportFieldsSpecial::StartBalPurse1);

	if (TRUE == System.GetEnablePurse3Flag())
	{
		m_DbReporter.AddSpecialColumn(DbReportFieldsSpecial::StartBalPurse3);
	}

	m_DbReporter.CreateReportHeader ( &m_Report );
}

//**************************************************************************

void CReportStartingBalance::SaveTotal ( int nLevel, int nType )
{
	int nCount = 0;
	CString strText = "";

	switch (nType)
	{
	case NORMAL:
		strText = "<..>Starting Balances";
		nCount = m_arrayTotals[nLevel][NORMAL].m_nCount;
		break;

	case NORMAL_ESTIMATED:	
		strText = "<..>Estimated Balances";
		nCount = m_arrayTotals[nLevel][NORMAL_ESTIMATED].m_nCount;
		break;

	case DELETED:			
		strText = "<..>Deleted Balances";
		nCount = m_arrayTotals[nLevel][DELETED].m_nCount;
		break;

	case DELETED_ESTIMATED:	
		strText = "<..>Estimated Deleted";
		nCount = m_arrayTotals[nLevel][DELETED_ESTIMATED].m_nCount;
		break;

	case GRAND_TOTAL:	
		{
			if (1 == nLevel)
			{
				strText = "<..>Report Total";
			}
			else
			{
				strText = "<..>Group Total";
			}

			for (int i = 0; i < GRAND_TOTAL; i++)
			{
				nCount += m_arrayTotals[nLevel][i].m_nCount;
			}
		}
		break;
	}

	CCSV csv ( '\t' );
	CCSV csvTabLine ( '\t' );

	m_DbReporter.ResetSpecialFields();
	m_DbReporter.AddSpecialField("");
	m_DbReporter.AddSpecialField("");
	m_DbReporter.AddSpecialField("");

	if (TRUE == System.GetActiveEnablePointsFlag())
	{
		m_DbReporter.AddSpecialField(::FormatIntValue(m_arrayTotals[nLevel][nType].m_nPoints));
	}

	m_DbReporter.AddSpecialField(::FormatDoubleValue(m_arrayTotals[nLevel][nType].m_dPurse1));

	if (System.GetEnablePurse3Flag() == TRUE)
	{
		m_DbReporter.AddSpecialField(::FormatDoubleValue(m_arrayTotals[nLevel][nType].m_dPurse3));
	}

	m_DbReporter.CreateTotalLine( 0, strText, &csv, &csvTabLine, nType, nCount );

	if ( nType == NORMAL )
	{
		m_Report.WriteLine ( csvTabLine );
	}
	else if ( nType == GRAND_TOTAL )
	{
		int nCount = 0;
		for ( int i = 0 ; i < GRAND_TOTAL; i++ )
		{
			if (m_arrayTotals[nLevel][i].m_nCount > 0)
			{
				++nCount;
			}
		}
		if (nCount < 2)
		{
			return;
		}

		m_Report.WriteLine ( csvTabLine );				// need grand total line
	}
	else
	{
		if (m_arrayTotals[nLevel][nType].m_nCount == 0)
		{
			return;	// no data
		}
	}

	m_Report.WriteLine ( csv );
}

//**************************************************************************
