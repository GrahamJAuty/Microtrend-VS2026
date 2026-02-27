/*********************************************************************/
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/*********************************************************************/
#include "ImportStatsDlg.h"
#include "ImportWisePayFile.h"
/**********************************************************************/
extern const char* szBLANK;					//  = "    --";
/**********************************************************************/

CImportWisePayFile::CImportWisePayFile(CWisePayData* pData, CWnd* pParent)
{
	m_pData = pData;
	m_pParent = pParent;
	m_nRead = 0;
	m_nIgnored = 0;
	m_nUsed = 0;
	m_nAdded = 0;
	m_strError = "";
	m_strExceptionComment = "";
	m_strReportTitle = "WisePay Account Data";

	m_strStartFromUserID = m_pData->m_strStartFromUserID;	// next free starting point

	m_strForename = "";								// "Forename"
	m_strSurname = "";								// "Surname"
	m_strGender = "";								// "gender"
	m_strDob = "";								// "dateOfBirth"
	m_strEmail = "";								// "Email"
	m_strYearGroup = "";								// "yearGroup"
	m_strRegGroup = "";								// "regGroup"

	m_bBackgroundMode = FALSE;
	m_bImportReqd = FALSE;
	m_nCurrentRow = 0;
	m_nInvalid = 0;
}

/**********************************************************************/

CImportWisePayFile::~CImportWisePayFile()
{
	if ( (m_pData->m_nNewIntakeAccount == nWISEPAY_NEWINTAKE_NEXTFREE_INDEX) && (m_nAdded > 0) )
	{
		m_pData->m_strStartFromUserID = m_strStartFromUserID.TrimLeft ( '0' );
		m_pData->Save();
	}
}

//***************************************************************

bool CImportWisePayFile::ShowImports()
{
	CString strReportLabel	= "R215";
	CString strReportFile = Filenames.GetReportFilename ( strReportLabel );
	CString strParamsFile = Filenames.GetReportParamsFilename ( strReportLabel );

	CRepmanHandler repman ( m_strReportTitle, m_pParent );

	int nErrorNo;
	if ( m_Report.Open ( strReportFile ) == TRUE )				// open report output file
	{
		m_Report.SetStyle1 ( m_strReportTitle );
		AddColumnText();

		nErrorNo = ProcessFile ( FALSE );						// no import reqd					

		AddStatistics();

		m_Report.Close();										// close report file
	}
	else
	{
		nErrorNo = nREPORT_CREATEFAIL;
	}

	if ( m_strError == "" )
	{
		if ( nErrorNo == nREPORT_NOERROR )
		{
			if (m_nIgnored > 0)
			{
				repman.DisplayMessage(szMSG_EXCEPTIONSDETECTED, MB_ICONEXCLAMATION);		// "Exceptions were detected with the import data!\n\nPlease see end of report for details.";
			}

			nErrorNo = repman.DisplayReport ( strReportFile, strParamsFile, strReportLabel );
		}

		repman.DisplayError ( nErrorNo );
		return TRUE;
	}
	else
	{
		repman.DisplayMessage(m_strError, MB_ICONEXCLAMATION);
	}

	return FALSE;
}

//*******************************************************************
// WisePay import

bool CImportWisePayFile::Import()
{
	bool bReply = FALSE;

	ProcessFile(TRUE);								// import reqd					

	bReply = (m_strError == "") ? TRUE : FALSE;

	return bReply;
}

//*******************************************************************
//*** process file for display or import*****************************
//*******************************************************************

int CImportWisePayFile::ProcessFile ( bool bImportReqd )
{
	CString strTitle;
	if (bImportReqd == TRUE)
	{
		strTitle = "Import " + m_strReportTitle;
	}
	else
	{
		strTitle = "Display " + m_strReportTitle;
	}

	CWorkingDlg dlgWorking ( strTitle, FALSE );									//create import progress bar
	dlgWorking.Create();

	if ( m_dbWisePayRecords.OpenSSDatabaseReadOnly ( m_pData->GetImportDinerDataFilename(), 10 ) != DB_ERR_NONE )	// open intermediate csv importfile
	{
		m_strError = ::OpenFileFailureMessage ( m_pData->GetImportDinerDataFilename(), DB_READONLY );	// "Cannot open file ' %s ' for Reading"
		return nREPORT_CREATEFAIL;
	}

	for ( int n = 0 ; n < m_dbWisePayRecords.GetRecordCount() ; n++ )
	{
		m_dbWisePayRecords.MoveTo ( n );

		++m_nRead;
		dlgWorking.SetCaption2PercentDone((n * 100) / m_dbWisePayRecords.GetRecordCount(), FALSE);

		m_strExceptionComment	= "";											// clear any current exceptions
		m_strUID				= m_dbWisePayRecords.GetString ( "uID" );
		m_strWisePayID			= m_dbWisePayRecords.GetString ( "acc_student_SIMS_unique_id" );
		m_strForename			= m_dbWisePayRecords.GetString ( "acc_student_first_name" );
		m_strSurname			= m_dbWisePayRecords.GetString ( "acc_student_last_name" );
		m_strGender				= m_dbWisePayRecords.GetString ( "acc_student_gender" );
		m_strYearGroup			= m_dbWisePayRecords.GetString ( "acc_student_year" );
		m_strRegGroup			= m_dbWisePayRecords.GetString ( "acc_student_class" );
		m_strEmail				= m_dbWisePayRecords.GetString ( "acc_user_email" );

		CString strDate		= "";
		CString strXmlDob	= m_dbWisePayRecords.GetString ( "acc_student_dob" );			// 1996-08-02T00:00:00+01:00
		if (strXmlDob.GetLength() > 10)
		{
			strDate.Format("%s%s%s", 
				(const char*) strXmlDob.Mid(8, 2), 
				(const char*) strXmlDob.Mid(5, 2),
				(const char*) strXmlDob.Left(4));
		}

		CSSDate date ( strDate );
		m_strDob = date.GetDate();

//
		CString strWiseID = m_strWisePayID;									// check have a valid WisePay ID
		strWiseID.MakeUpper();
		if (strWiseID.Left(3) == "TBC")									// 14/04/2016
		{
			m_strWisePayID = "";											// set no WisePay ID
		}
		else
		{
			strWiseID.TrimLeft('0');										// remove leading '0's
			if (strWiseID == "")											// check have anything
			{
				m_strWisePayID = "";										// no - set no WisePay ID
			}
		}

		__int64 nUserID = 0;

		if (m_strWisePayID != "")											// see if have WisePay ID 
		{	
			CSQLRepositoryAccount RepoAccount;
			nUserID = RepoAccount.LookupMemberID(m_strWisePayID, NULL).GetSQLResult();
		}

		if ( 0 == nUserID )												// no number so far
		{
			if (m_pData->m_bAddUnknowns == TRUE)
			{
				GetNewCardNo(nUserID);									// get next new card number - will set exceptions msg & cardno = "   --" on error
			}
			else
			{
				m_strExceptionComment.Format("No account found for %s", 
					(const char*) m_strWisePayID);
			}
		}

		if (bImportReqd == FALSE)
		{
			ShowImportLine(nUserID);
		}
		else
		{
			ImportLine(nUserID);				// input line ( if no exception ) 
		}
	}

	m_dbWisePayRecords.Close();												// close wisepay file

	if (bImportReqd == TRUE)
	{
		m_pData->Save();													// save highest PosTxnID / LastDinerSnapshotID / nLastBalanceSnapshotID
	}

	return nREPORT_NOERROR;
}

//******************************************************************
// get CardNo for new record, exception string set if no cardno set

void CImportWisePayFile::GetNewCardNo( __int64& nCardNo )
{
	if (m_pData->m_nNewIntakeAccount == nWISEPAY_NEWINTAKE_MISID_INDEX)	// create new account using Person-id
	{
		if (System.IsValidCardNo(m_strUID) == FALSE)
		{
			m_strExceptionComment.Format("Invalid uID reference '%s'", 
				(const char*) m_strUID);
		}
		else
		{
			nCardNo = _atoi64(m_strUID);
		}
	}
	else																	// next free
	{
		CSQLRepositoryAccount RepoAccount;
		nCardNo = RepoAccount.GetNextFreeUserID(_atoi64(m_strStartFromUserID), _atoi64(System.GetHighCardNo()), NULL).GetSQLResult();
			
		if (0 == nCardNo)
		{
			m_strExceptionComment = "No more accounts";
		}
		else
		{
			// next new start from position
			m_strStartFromUserID = CSQLHelpers::IncrementUserID( nCardNo, _atoi64(System.GetHighCardNo()));
		}
	}
}

//**********************************************************************

void CImportWisePayFile::AddColumnText()
{	
	CSQLRepositoryUserText RepoText;

	m_Report.AddColumn ( RepoText.GetMemberIDText(), TA_LEFT, 300 );
	m_Report.AddColumn ( "Account", TA_LEFT, 300 );
	m_Report.AddColumn ( "Forename", TA_LEFT, 300 );
	m_Report.AddColumn ( "Surname", TA_LEFT, 300 );
	m_Report.AddColumn ( "Email", TA_LEFT, 300 );
	m_Report.AddColumn ( "Year", TA_LEFT, 300 );
	m_Report.AddColumn ( "DOB", TA_LEFT, 300 );
	m_Report.AddColumn ( "Class", TA_LEFT, 300 );
	m_Report.AddColumn ( "Sex", TA_LEFT, 300 );
	m_Report.AddColumn ( "", TA_LEFT, 600 );
}

//**********************************************************************

bool CImportWisePayFile::ShowImportLine ( __int64 nCardNo )
{
	CString strCardNo = FormatInt64Value(nCardNo);

	bool bValid = ( 0 == nCardNo ) ? FALSE : TRUE;			// see if valid account number

	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.RowExists(nCardNo, NULL).GetSQLResult() == 0)		// see if in database
	{
		if ( bValid == TRUE )
		{
			++m_nAdded;											// will be new record
			strCardNo += " *";									// not in database - set display flag
		}
		else
		{
			++m_nIgnored;										// will be ignored
			strCardNo = szBLANK;
		}
	}

	if (bValid == TRUE)
	{
		++m_nUsed;							// inc number of valid lines 
	}

	CCSV csv ( '\t' );
	csv.Add ( m_strWisePayID );
	csv.Add ( strCardNo );
	csv.Add ( m_strForename );
	csv.Add ( m_strSurname );

	if (HaveEmailAddress() == TRUE)
	{
		csv.Add(m_strEmail);
	}
	else
	{
		csv.Add("");
	}

	csv.Add ( m_strYearGroup );
	csv.Add ( m_strDob );
	csv.Add ( m_strRegGroup );
	csv.Add ( m_strGender );
	if ( m_strExceptionComment != "" )	csv.Add ( m_strExceptionComment );

	return ( m_Report.WriteLine ( csv ) );
}

//**********************************************************************

void CImportWisePayFile::AddStatistics()
{
	CString strLine = "";
	m_Report.WriteLine ( "" );

	strLine.Format ( "%d\t<..>Lines read", m_nRead );
	m_Report.WriteLine ( strLine );

	strLine.Format ( "%d\t<..>New accounts * ( added by '%s' )", m_nAdded, m_pData->GetNewIntakeAddedByText() );
	m_Report.WriteLine ( strLine );

	strLine.Format ( "%d\t<..>Existing accounts", m_nRead - ( m_nAdded + m_nIgnored ) );
	m_Report.WriteLine ( strLine );

	strLine.Format ( "%d\t<..>Exceptions found", m_nIgnored );
	m_Report.WriteLine ( strLine );
}

//**********************************************************************

void CImportWisePayFile::ImportLine ( __int64 nCardNo ) 
{
	if ( 0 == nCardNo )
	{
		++m_nIgnored;
		return;
	}

	++m_nUsed;

	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserID(nCardNo);

	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
	{
		SetRecord(RowAccount);										// set record fields & save pending if required
		RepoAccount.UpdateRow(RowAccount,NULL);
	}
	else
	{
		++m_nAdded;
		RepoAccount.InsertRow(RowAccount,NULL);
		
		SetRecord(RowAccount);										// update new record
		RowAccount.CopyGroupMaxSpendSettings();
		RepoAccount.UpdateRow(RowAccount,NULL);
		
		CSQLAuditRecord atc;
		atc.Set( &RowAccount );
		atc.SetApplicationNo ( APPNO_INSERTRECORDWISEPAY );
		atc.SaveNewLine();
	}
}
	
/**********************************************************************/

void CImportWisePayFile::SetRecord(CSQLRowAccountFull& RowAccount)
{
	RowAccount.SetMemberID(m_strWisePayID);					// Member#
	RowAccount.SetForename(m_strForename);
	RowAccount.SetSurname(m_strSurname);
	RowAccount.SetGender(m_strGender);

	int nField = 0;

	if ((nField = m_pData->m_nYearInfoIndex) > 0)
	{
		RowAccount.SetInfo(nField, m_strYearGroup);			// 0-not defined, info1 - info8
	}

	if ((nField = m_pData->m_nRegInfoIndex) > 0)
	{
		RowAccount.SetInfo(nField, m_strRegGroup);			// 0-not defined, info1 - info8
	}

	if (m_pData->m_bSaveEmail == TRUE)
	{
		if (HaveEmailAddress() == TRUE)
		{
			if ((nField = m_pData->m_nEmailInfoIndex) == 0)
			{
				RowAccount.SetEmail(m_strEmail);			// 0 - email field
			}
			else
			{
				RowAccount.SetInfo(nField, m_strEmail);	// info1 - info8
			}
		}
	}

	if (m_pData->m_nDobInfoIndex == 1)							// 0-not defined, 1=Dob1
	{
		RowAccount.SetDOB(m_strDob);
	}
}

/**********************************************************************/

bool CImportWisePayFile::HaveEmailAddress()
{
	return ( m_strEmail.Find ( '@' ) == -1 ) ? FALSE : TRUE;
}

/**********************************************************************/

void CImportWisePayFile::ShowStatistics()
{
	CImportStatsDlgConfig Config;
	Config.m_nLinesRead = m_nRead;
	Config.m_nLinesIgnored = m_nIgnored;
	Config.m_nLinesUsed = m_nUsed;
	Config.m_nRecordsAdded = m_nAdded;
	Config.m_nLinesReadPos = 0;
	Config.m_nLinesIgnoredPos = 1;
	Config.m_nLinesUsedPos = 2;
	Config.m_nRecordsAddedPos = 4;
	Config.m_nLinesInvalidPos = -1;
	Config.m_nRecordsDeletedPos = -1;
	Config.m_nLastLinePos = 4;
	Config.m_strCaption = m_strReportTitle;

	CImportStatsDlg dlg(Config);
	dlg.DoModal();
}

//**********************************************************************

