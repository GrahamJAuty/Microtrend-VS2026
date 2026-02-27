/**********************************************************************/
#include "..\SmartPay4Shared\CardCounterDatabase.h"
#include "..\SmartPay4Shared\RecSort.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDb.h"
/**********************************************************************/
#include "ReportWeeklySpend.h"
/**********************************************************************/

CReportWeeklySpend::CReportWeeklySpend ( const char* szLabel, CWnd* pParent )
	: m_dlgReport(szLabel)
	, m_Repman("Weekly Spend Report", pParent)
{
}

/**********************************************************************/

void CReportWeeklySpend::ResetTotals()
{
	for ( int i = nSPEND_TTL ; i <= nSPEND_SAT ; i++ )			// 0=ttl, 1=Sun,2=Mon.. 7==Sat
	{
		m_dTotalSpend[i] = 0.0;
		m_nTotalCount[i] = 0;
	}
}

//*******************************************************************

void CReportWeeklySpend::Show()
{
	int nErrorNo;
	if ((nErrorNo = Create()) != nREPORT_NOERROR)
	{
		m_Repman.DisplayError(nErrorNo);
	}
}

//*******************************************************************

int CReportWeeklySpend::Create()
{
	int nErrorNo = nREPORT_NOERROR;
	CSmartPayAuditFile auditFile ( Filenames.GetAuditFilename() );
	if ((nErrorNo = auditFile.ValidateFile()) != nREPORT_NOERROR)
	{
		return nErrorNo;
	}

	while ( m_dlgReport.DoModal() == IDOK )
	{
		if (m_Report.Open(m_dlgReport.GetReportFilename()) == FALSE)
		{
			return nREPORT_CREATEFAIL;
		}
		else
		{
			if ( auditFile.Open() == FALSE )						// reopen source file
			{
				m_Report.Close();									// close report file
				return nREPORT_OPENERROR;
			}
		}

		CWorkingDlg dlgWorking ( szMSG_CREATINGREPORT, TRUE );
		dlgWorking.Create();

		int nErrorNo = ReportSpend ( &auditFile, &dlgWorking );

		dlgWorking.DestroyWindow();

		m_Report.Close();
		auditFile.Close();											// close audit file open

		if (nErrorNo == nREPORT_NOERROR)
		{
			nErrorNo = m_Repman.DisplayReport(m_dlgReport.GetReportFilename(), m_dlgReport.GetParamsFilename(), m_dlgReport.GetReportKey());
		}

		m_Repman.DisplayError ( nErrorNo );
	}

	return nREPORT_NOERROR;
}

//***********************************************************************************

int CReportWeeklySpend::ReportSpend ( CSmartPayAuditFile* pAudit, CWorkingDlg* pWorkingDlg )
{
	int nReply = nREPORT_NOERROR;

	ResetTotals();
	TidyUpFiles();													// just in case stil have some ..\TEMPn\WKSP-*.DAT file still lurking

// compile array of days to report on

	CUIntArray arrayDay;															// temp array to hold all days begining with start of week day
	for (int nDayNo = System.GetStartOfWeek(); nDayNo <= nSPEND_SAT; nDayNo++)	// start of week, 1=Sun,2=Mon..7=Sat
	{
		arrayDay.Add(nDayNo);
	}

	for (int nDayNo = nSPEND_SUN; nDayNo < System.GetStartOfWeek(); nDayNo++)	// remaining days if start of week was not sunday
	{
		arrayDay.Add(nDayNo);
	}

	m_arrayDays.RemoveAll();									// clear main array of days to report on
	for ( int i = 0 ; i < arrayDay.GetSize() ; i++ )
	{
		int nDayNo = arrayDay.GetAt(i);
		if (m_dlgReport.m_bOnlyShowMonFri == TRUE && (nDayNo == nSPEND_SUN || nDayNo == nSPEND_SAT))// skip Sat & Sun ( Mon-Fri only)
		{
			continue;
		}

		m_arrayDays.Add ( nDayNo );
	}

//*******

	CWeeklySpendDatabase dbSpend;
	dbSpend.OpenForSharing ( CreateWeeklySpendFilename ( "dbase" ) );	// create a temp database (..\P33\TEMPn\WKSP-dbase.DAT) to hold spend values

// add all records so if not used will have an record with no entries ************************

	if ( m_dlgReport.m_bShowUnusedLines == TRUE )				// if not zero skip add all relevant database records
	{
		CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_NoParams{}, FALSE);

		CSQLRowAccountFull RowAccount;
		while (RecordSet.StepSelectAll(RowAccount) == TRUE)
		{ 
			if (  m_dlgReport.IsValid ( RowAccount ) == TRUE )
			{
				CString strUserID = RowAccount.GetUserIDString();
				dbSpend.AddBlankRecord ( strUserID );

				CString strText = RowAccount.GetUsername();
				if (strText == "")
				{
					strText = System.FormatCardNo(strUserID);
				}
				dbSpend.SetUserName ( strText );

				strText = RowAccount.GetSurname();
				if (strText == "")
				{
					strText = System.FormatCardNo(strUserID);
				}
				dbSpend.SetSurname ( strText );
			}
		}
	}

// work thru audit file for relevant accounts & total up values **********************

	CSQLAuditRecord atc;
	pAudit->JumpIn ( m_dlgReport.m_dateFrom.GetDate() );

	while ( TRUE )
	{
		if (pWorkingDlg->SetCaption2PercentDone(pAudit->GetPercentPosition() / 2, TRUE) == TRUE)
		{
			nReply = nREPORT_CANCEL;
			break;
		}

		if (pAudit->ReadLine(&atc) == FALSE)
		{
			break;
		}

		if (  m_dlgReport.IsValid ( &atc ) == TRUE )
		{
			double dSpend = 0.0;
			if ( ( dSpend = m_dlgReport.GetSpendValue ( &atc ) ) != 0.0 )
			{
				CString strUserID = atc.GetUserID();

				if ( dbSpend.FindRecord ( strUserID ) == FALSE )
					dbSpend.AddBlankRecord ( strUserID );

				dbSpend.SetDayNo ( atc.GetDateFiled() );			// Set day number for dd/mm/yyyy
				dbSpend.SaveSpend ( -(dSpend) );					// Spend is -ve so match +ve

				CCardCounterDatabase dbCounter;											// card counter database for given date
				dbCounter.Open ( CreateWeeklySpendFilename ( atc.GetCSVDateFiled() ) );	// ..\P33\TEMPnn\WKSP-yyyymmdd.DAT

				if ( dbCounter.AddCard ( strUserID ) == TRUE )		// returns TRUE if not already added
					dbSpend.IncrementCount();						// first time account used on this date
	
				dbCounter.Close();									// close card counter

				if ( dbSpend.GetUserName() == "" )
					dbSpend.SetUserName ( atc.GetUserName() );		// set name if not set already

				if ( dbSpend.GetSurname() == "" )					// see if surname set
				{
					CSQLRowAccountFull RowAccount;
					RowAccount.SetUserIDString(strUserID);

					CSQLRepositoryAccount RepoAccount;
					if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
					{
						CString strSurname = RowAccount.GetSurname();
						if (strSurname == "")
						{
							strSurname = System.FormatCardNo(strUserID);
						}

						dbSpend.SetSurname ( strSurname );			// save surname in weekly spend database
					}
				}
			}
		}
	}

	if ( nReply == nREPORT_NOERROR && dbSpend.GetRecordCount() == 0 )
		nReply = nREPORT_NOMATCH;

	if ( nReply != nREPORT_NOERROR )
	{
		dbSpend.Close();
		TidyUpFiles();
		return nReply;
	}

// sort spend database in required order ********************************************

	CRecordSorter sorter;
	for ( int nIndex = 0 ; nIndex < dbSpend.GetRecordCount() ; nIndex++ )
	{
		dbSpend.MoveTo ( nIndex );

		CString strLabel;
		switch ( m_dlgReport.m_nSortOn )
		{
		case nSORTON_SURNAME:	strLabel = dbSpend.GetSurname();						break;
		case nSORTON_USERID:	strLabel = System.FormatCardNo(dbSpend.GetUserID());	break;
		case nSORTON_USERNAME:	strLabel = dbSpend.GetUserName();						break;
		}
		strLabel.MakeUpper();
		sorter.Add ( strLabel, nIndex );
	}

// compile report *******************************************************************

	m_Report.SetStyle1 ( m_dlgReport.GetReportTitle() );
	AddColumnText( );

	for (int nIndex = 0; nIndex < sorter.GetSize(); nIndex++)
	{
		if (pWorkingDlg->SetCaption2PercentDone(50 + ((nIndex * 50L) / sorter.GetSize()), TRUE) == TRUE)
		{
			nReply = nREPORT_CANCEL;
			break;
		}

		dbSpend.MoveTo(sorter.GetRecordNo(nIndex));
		SaveLine(&dbSpend);
	}

	SaveTotals();

	dbSpend.Close();
	TidyUpFiles();
	return nReply;
}

//**********************************************************************

void CReportWeeklySpend::SaveLine ( CWeeklySpendDatabase* pDbSpend )
{
	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserIDString(pDbSpend->GetUserID());

	CSQLRepositoryAccount RepoAccount;
	bool bExists = (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE );
	
	if ((FALSE == bExists) && (FALSE == m_dlgReport.m_bShowDeletedLines))
	{
		return;
	}

	CString strForename	= "";
	CString strUserID	= System.FormatCardNo(pDbSpend->GetUserID());

	if (bExists == FALSE)
	{
		strUserID += " *";
	}
	else
	{
		strForename = RowAccount.GetForename();
	}

	CCSV csv ( '\t' );

	switch ( m_dlgReport.m_nSortOn )
	{
	case nSORTON_SURNAME:	csv.Add ( pDbSpend->GetSurname() );
							csv.Add ( strUserID );
							csv.Add ( pDbSpend->GetUserName() );
							break;

	case nSORTON_USERID:	csv.Add ( strUserID );
							csv.Add ( pDbSpend->GetUserName() );
							csv.Add ( pDbSpend->GetSurname() );
							break;

	case nSORTON_USERNAME:	csv.Add ( pDbSpend->GetUserName() );
							csv.Add ( strUserID  );
							csv.Add ( pDbSpend->GetSurname() );
							break;
	}

	csv.Add ( strForename );

	for ( int i = 0 ; i < m_arrayDays.GetSize() ; i++ )
	{
		int nDayNo = m_arrayDays.GetAt(i);
		SaveDaySpend ( &csv, pDbSpend, nDayNo );
	}

	m_Report.WriteLine ( csv );
}

//**********************************************************************
// nDayNo = 1=Sun,2=Mon.. 7==Sat

void CReportWeeklySpend::SaveDaySpend(CCSV* pCsvLine, CWeeklySpendDatabase* pDbSpend, int nDayNo)
{
	double dValue = pDbSpend->GetSpend(nDayNo);
	int nCount = pDbSpend->GetCount(nDayNo);

	SaveValues(pCsvLine, dValue, nCount);

	m_dTotalSpend[nDayNo] += dValue;							// day total spend
	m_nTotalCount[nDayNo] += nCount;							// day total count

	m_dTotalSpend[nSPEND_TTL] += dValue;						// report total spend
	m_nTotalCount[nSPEND_TTL] += nCount;						// report total count
}

//**********************************************************************

void CReportWeeklySpend::SaveValues ( CCSV* pCsvLine, double dValue, int nCount )
{
	if ( dValue == 0 && nCount == 0 )	pCsvLine->Add ( "-" );				// no spend
	else								pCsvLine->Add ( dValue, 2 );		// spend

	CString strCount = "";
	if ( nCount > 1 )	strCount.Format ( "(%d)", nCount );
	pCsvLine->Add ( strCount );
}

//**********************************************************************

void CReportWeeklySpend::SaveTotals()
{
	CCSV csv ( '\t' );
	CString strTabLine = "\t\t\t\t<LI>\t\t<LI>\t\t<LI>\t\t<LI>\t\t<LI>\t\t<LI>\t\t<LI>";

	CString strGroupName = m_dlgReport.m_strSingleGroupName;
	if ( strGroupName != "" )	strGroupName += " ";					// only set if single group has been selected

	CString strBuf;	strBuf.Format ( "<..>Report Total %s [  %.2lf / %d  ]", strGroupName, m_dTotalSpend[nSPEND_TTL], m_nTotalCount[nSPEND_TTL] );
	csv.Add ( strBuf );
	csv.Add  ( "" );
	csv.Add  ( "" );
	csv.Add  ( "" );

	for ( int i = 0 ; i < m_arrayDays.GetSize() ; i++ )
	{
		int nDayNo = m_arrayDays.GetAt(i);
		SaveValues ( &csv, m_dTotalSpend[nDayNo], m_nTotalCount[nDayNo] );
	}

	m_Report.WriteLine ( strTabLine );
	m_Report.WriteLine ( csv );
}

/**********************************************************************/

void CReportWeeklySpend::AddColumnText()
{
	CString strCol1 = "";
	CString strCol2 = "";
	CString strCol3 = "";

	switch ( m_dlgReport.m_nSortOn )
	{
	case nSORTON_SURNAME:	strCol1 = "Surname";	strCol2 = "UserID";		strCol3 = "UserName";	break;
	case nSORTON_USERID:	strCol1 = "UserID";		strCol2 = "UserName";	strCol3 = "Surname";	break;
	case nSORTON_USERNAME:	strCol1 = "UserName";	strCol2 = "UserID";		strCol3 = "Surname";	break;
	}

	m_Report.AddColumn ( strCol1, TA_LEFT, 300 );
	m_Report.AddColumn ( strCol2, TA_LEFT, 300 );
	m_Report.AddColumn ( strCol3, TA_LEFT, 300 );
	m_Report.AddColumn ( "Forename", TA_LEFT, 300 );

	for ( int i = 0 ; i < m_arrayDays.GetSize() ; i++ )
	{
		CString strDay = "";
		switch ( m_arrayDays.GetAt(i) )
		{
		case nSPEND_SUN:	strDay = "Sun";		break;
		case nSPEND_MON:	strDay = "Mon";		break;
		case nSPEND_TUE:	strDay = "Tue";		break;
		case nSPEND_WED:	strDay = "Wed";		break;
		case nSPEND_THU:	strDay = "Thu";		break;
		case nSPEND_FRI:	strDay = "Fri";		break;
		case nSPEND_SAT:	strDay = "Sat";		break;
		}

		m_Report.AddColumn ( strDay, TA_RIGHT, 180 );					// Day text
		m_Report.AddColumn ( "", TA_LEFT, 120 );						// Day counter
	}
}

//*****************************************************************************
// create temp path  '..\P33\tempn\wksp-yyyymmdd.dat' or '..\wkksp-dbase.dat' or '..\wksp-*.dat'

CString CReportWeeklySpend::CreateWeeklySpendFilename ( const char* szText )
{
	CString strFilename;	strFilename.Format ( "wksp-%s.dat", szText );
	CString strPath = Filenames.GetTempPathFilename ( strFilename );		// P33\TEMPn\WKSP-?????.DAT
	return strPath;
}

//****************************************************************************

void CReportWeeklySpend::TidyUpFiles()
{
	CFileRecycler::SendToRecycleBin ( CreateWeeklySpendFilename ( "*" ) );		// ..\P33\TEMPn\WKSP-*.DAT
}

//****************************************************************************
