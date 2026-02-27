/**********************************************************************/
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/
#include "ImportStatsDlg.h"
#include "ImportParentPayFile.h"
/**********************************************************************/

const char* szBLANK = "    --";
//const char* szDINERMATCHHEADER = "connectingPOSTxnID,parentPayAccountID,posInternalAccountID,externalAccountReference,openingBalance";

/**********************************************************************/

CImportParentPayFile::CImportParentPayFile(CParentPayData* pData, CWnd* pParent)
{
	m_pData = pData;
	m_pParent = pParent;
	m_nRead = 0;
	m_nIgnored = 0;
	m_nUsed = 0;
	m_nAdded = 0;
	m_nInvalid = 0;
	m_nExisting = 0;
	m_nCurrentRow = 0;
	m_nLocatedByRollNo = 0;
	m_nLocatedByUPN = 0;
	m_nLocatedByMisID = 0;
	m_nInactive = 0;
	m_strError = "";
	m_strExceptionComment = "";
	m_strReportTitle = "ParentPay Diner Data";

	m_strStartFromUserID = m_pData->m_strStartFromUserID;			// next free starting point
	m_strDinerMatchAdviceFile = m_pData->GetDinerMatchAdviceFilename();

	m_bForceMatchAccount = FALSE;							// can be set externally to always send diner match data
	m_bLocatedFlag = FALSE;							// set if unknown account located by using LocatedBy database

	m_strParentPayID = "";								// "accountID"
	m_strParentPayMisID = "";								// hold MisID
	m_strParentPayUPN = "";								// hold UPN
	m_strParentPayRollNo = "";								// hold RollNo
	m_strForename = "";								// "Forename"
	m_strSurname = "";								// "Surname"
	m_strGender = "";								// "gender"
	m_strDob = "";								// "dateOfBirth"
	m_strYearGroup = "";								// "yearGroup"
	m_strRegGroup = "";								// "regGroup"
	m_strStatus = "";								// "dinerStatus"	

	m_bBackgroundMode = FALSE;
	m_bImportReqd = FALSE;
}

/**********************************************************************/

CImportParentPayFile::~CImportParentPayFile()
{
	if (m_pData->m_nNewIntakeAccount == nPARENTPAY_NEWINTAKE_NEXTFREE_INDEX && m_nAdded > 0)
	{
		m_pData->m_strStartFromUserID = m_strStartFromUserID.TrimLeft('0');
		m_pData->Save();
	}
}

//***************************************************************
// checked import file to see if any data sent from ParentPay

bool CImportParentPayFile::ValidateFile()
{
	bool bReply = FALSE;
	if ( m_dbParentPayRecords.OpenSSDatabaseReadOnly ( m_pData->GetImportDataFilename(), 10 ) == DB_ERR_NONE )	// open import file
	{
		m_dbParentPayRecords.MoveTo ( 0 );

		m_strParentPayID = m_dbParentPayRecords.GetString ( "accountID" );
		if (m_strParentPayID != "-1")								// see if null record sent
		{
			bReply = TRUE;
		}

		m_dbParentPayRecords.Close();
	}

	return bReply;
}

//***************************************************************

bool CImportParentPayFile::ShowImports()
{
	CString strReportLabel	= "R214";
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
// ParentPay import

bool CImportParentPayFile::Import()
{
	ProcessFile(TRUE);													
	return (m_strError == "");
}

//*******************************************************************
//*** process file for display or import*****************************
//*******************************************************************

int CImportParentPayFile::ProcessFile ( bool bImportReqd )
{
	CString strTitle = "";
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

	if ( m_dbParentPayRecords.OpenSSDatabaseReadOnly ( m_pData->GetImportDataFilename(), 10 ) != DB_ERR_NONE )	// open intermediate csv importfile
	{
		m_strError = ::OpenFileFailureMessage ( m_pData->GetImportDataFilename(), DB_READONLY );		// "Cannot open file ' %s ' for Reading!"
		return nREPORT_CREATEFAIL;
	}

	for (int n = 0; n < m_dbParentPayRecords.GetRecordCount(); n++)
	{
		m_dbParentPayRecords.MoveTo(n);

		++m_nRead;
		dlgWorking.SetCaption2PercentDone((n * 100) / m_dbParentPayRecords.GetRecordCount(), FALSE);

		m_strParentPayID = m_dbParentPayRecords.GetString("accountID");
		m_strParentPayMisID = m_dbParentPayRecords.GetString("misID");
		m_strParentPayUPN = m_dbParentPayRecords.GetString("UPN");
		m_strParentPayRollNo = m_dbParentPayRecords.GetString("rollNumber");
		m_strForename = m_dbParentPayRecords.GetString("forename");
		m_strSurname = m_dbParentPayRecords.GetString("surname");
		m_strGender = m_dbParentPayRecords.GetString("gender");
		m_strYearGroup = m_dbParentPayRecords.GetString("yearGroup");
		m_strRegGroup = m_dbParentPayRecords.GetString("regGroup");
		m_strStatus = m_dbParentPayRecords.GetString("dinerStatus");			// 	if "Active";	

		CString strDate = "";
		CString strXmlDob = m_dbParentPayRecords.GetString("dateOfBirth");			// 2005-09-01T00:00:00.0000000+01:00
		
		if (strXmlDob.GetLength() > 10)
		{
			strDate.Format("%s%s%s", 
				(const char*) strXmlDob.Mid(8, 2),
				(const char*) strXmlDob.Mid(5, 2),
				(const char*) strXmlDob.Left(4));
		}

		CSSDate date(strDate);
		m_strDob = date.GetDate();
		//
		CString strCardNo = "";												// clear value
		m_strExceptionComment = "";											// clear any current exceptions
		m_bLocatedFlag = FALSE;												// set if unknown account found by LocatedBy database

		CSQLRepositoryAccount RepoAccount;
		__int64 nUserID = RepoAccount.LookupMemberID(m_strParentPayID, NULL).GetSQLResult();

		if (nUserID != 0)				// see if in  Members file 
		{
			strCardNo = FormatInt64Value(nUserID);
			MatchRecordUPN(strCardNo);									// will set exception msg on mismatch error
		}
		else
		{
			if (LocateRecord(strCardNo) == FALSE)						// see if in temp LocateBy database, will set exception msg on mismatch error
			{
				if (m_pData->m_bAddUnknowns == TRUE)
				{
					GetNewCardNo(strCardNo);								// get new card number - will set exceptions msg on error
				}
				else
				{
					m_strExceptionComment.Format("No account found for %s",
						(const char*) m_strParentPayID);
				}
			}
		}

		if (bImportReqd == FALSE)
		{
			ShowImportLine(strCardNo);
		}
		else
		{
			ImportLine(strCardNo);				// input line ( if no exception ) 
		}
	}

	m_dbParentPayRecords.Close();									// close parentpay file
	
	if (bImportReqd == TRUE)
	{
		m_pData->Save();											// save highest PosTxnID / LastDinerSnapshotID / nLastBalanceSnapshotID
	}

	return nREPORT_NOERROR;
}

//**********************************************************************
// search for unknown account using LocateBy database 

bool CImportParentPayFile::LocateRecord(CString& strCardNo)
{
	bool bReply = FALSE;

	bool bUPNMatch = FALSE;
	__int64 nUserID = 0;

	//MATCH BY ROLL NO
	{
		CString strField = m_pData->GetLocateByLabelRollNo();
		CString strMatch = m_strParentPayRollNo;

		if ((strField != "") && (strMatch != ""))
		{
			CSQLRepositoryAccount RepoAccount;
			nUserID = RepoAccount.LookupIndex(strField, strMatch, NULL).GetSQLResult();
		}

		if (nUserID != 0)
		{
			m_nLocatedByRollNo++;
		}
	}

	//MATCH BY UPN
	if (nUserID == 0)
	{
		CString strField = m_pData->GetLocateByLabelUPN();
		CString strMatch = m_strParentPayUPN;

		if ((strField != "") && (strMatch != ""))
		{
			CSQLRepositoryAccount RepoAccount;
			nUserID = RepoAccount.LookupIndex(strField, strMatch, NULL).GetSQLResult();
		}

		if (nUserID != 0)
		{
			m_nLocatedByUPN++;
			bUPNMatch = TRUE;
		}
	}

	//MATCH BY MisID
	if (nUserID == 0)
	{
		CString strField = m_pData->GetLocateByLabelMisID();
		CString strMatch = m_strParentPayMisID;

		if ((strField != "") && (strMatch != ""))
		{
			CSQLRepositoryAccount RepoAccount;
			nUserID = RepoAccount.LookupIndex(strField, strMatch, NULL).GetSQLResult();
		}
		if (nUserID != 0)
		{
			m_nLocatedByMisID++;
		}
	}

	if (nUserID != 0)
	{
		strCardNo = FormatInt64Value(nUserID);
		m_bLocatedFlag = TRUE;

		if (FALSE == bUPNMatch)	// if not locating by UPN - check UPN
		{
			MatchRecordUPN(strCardNo);								// will set exception msg on mismatch error
		}

		bReply = TRUE;
	}

	return bReply;
}

//******************************************************************
// get CardNo for new record, exception string set if no cardno set

void CImportParentPayFile::GetNewCardNo( CString& strCardNo )
{
	strCardNo = "";

	if ( m_pData->m_nNewIntakeAccount == nPARENTPAY_NEWINTAKE_MISID_INDEX )	// create new account using Person-id
	{
		if (System.IsValidCardNo(m_strParentPayMisID) == FALSE)
		{
			m_strExceptionComment.Format("Invalid misID reference '%s'", 
				(const char*) m_strParentPayMisID);
		}
		else
		{
			strCardNo = m_strParentPayMisID;
		}
	}
	else if ( m_pData->m_nNewIntakeAccount == nPARENTPAY_NEWINTAKE_ROLLNO_INDEX)
	{
		if (System.IsValidCardNo(m_strParentPayRollNo) == FALSE)
		{
			m_strExceptionComment.Format("Invalid rollNumber reference '%s'",
				(const char*) m_strParentPayRollNo);
		}
		else
		{
			strCardNo = m_strParentPayRollNo;
		}
	}
	else																// next free record
	{
		CSQLRepositoryAccount RepoAccount;
		__int64 nUserID = RepoAccount.GetNextFreeUserID(_atoi64(m_strStartFromUserID), _atoi64(System.GetHighCardNo()), NULL).GetSQLResult();
		
		if ( 0 == nUserID)
		{
			strCardNo = "";
			m_strExceptionComment = "No more accounts";
		}
		else
		{
			strCardNo = FormatInt64Value(nUserID);
			m_strStartFromUserID = CSQLHelpers::IncrementUserID(strCardNo, System.GetHighCardNo());		// next new start from position
		}
	}

	if (strCardNo != "")
	{
		if (MatchRecordUPN(strCardNo) == FALSE)					// see if fields match
		{
			m_strExceptionComment.Format("Account %s already exists", 
				(const char*) strCardNo);
		}
	}
}

//**********************************************************************

void CImportParentPayFile::AddColumnText()
{
	CSQLRepositoryUserText RepoText;

	m_Report.AddColumn ( RepoText.GetMemberIDText(), TA_LEFT, 300 );
	m_Report.AddColumn ( "Account", TA_LEFT, 300 );
	m_Report.AddColumn ( "Forename", TA_LEFT, 300 );
	m_Report.AddColumn ( "Surname", TA_LEFT, 300 );
	m_Report.AddColumn ( "DOB", TA_LEFT, 300 );
	m_Report.AddColumn ( "YearGroup", TA_LEFT, 300 );
	m_Report.AddColumn ( "RegGroup", TA_LEFT, 300 );

	int nField;
	if ( ( nField = m_pData->GetMisIDUserTextNo() ) > 0 )	m_Report.AddColumn ( RepoText.GetInfoText(nField), TA_LEFT, 300 ); 
	if ( ( nField = m_pData->GetUPNUserTextNo() ) > 0 )		m_Report.AddColumn ( RepoText.GetInfoText(nField), TA_LEFT, 300 );
	if ( ( nField = m_pData->GetRollNoUserTextNo() ) > 0)	m_Report.AddColumn ( RepoText.GetInfoText(nField), TA_LEFT, 300 );

	m_Report.AddColumn ( "Sex", TA_LEFT, 300 );
	m_Report.AddColumn ( "", TA_LEFT, 600 );
}

//********************************************************************

bool CImportParentPayFile::ShowImportLine ( const char* szCardNo )
{
	CString strCardNo = szCardNo;

	CSQLRepositoryAccount RepoAccount;
	bool bExists = (RepoAccount.RowExists(_atoi64(strCardNo), NULL).GetSQLResult() != 0);
	
	if (bExists == TRUE)
	{
		++m_nExisting;
	}

	bool bActive = IsStatusActive();

	bool bValid = ( m_strExceptionComment == "" ) ? TRUE : FALSE;	// see if any exception

	if ( bValid == TRUE && bExists == FALSE && m_bLocatedFlag == FALSE && bActive == TRUE )	// see if in database
	{
		++m_nAdded;													// will be new record
		strCardNo = "* " + strCardNo;								// not in database - set display flag
	}
	else
	{
		if ( bValid == TRUE )
		{
			++m_nUsed;												// inc number of valid lines 

			if (m_bLocatedFlag == TRUE && bActive == TRUE)
			{
				strCardNo = "!! " + strCardNo;
			}
		}
		else
		{
			if (strCardNo == "")
			{
				strCardNo = szBLANK;
			}

			if ( bActive == TRUE )
			{
				++m_nIgnored;											// will be ignored
				strCardNo = "x " + strCardNo;							// not in database - set display flag
			}
		}
	}

	CCSV csv ( '\t' );
	csv.Add ( m_strParentPayID );
	csv.Add ( strCardNo );
	csv.Add ( m_strForename );
	csv.Add ( m_strSurname );
	csv.Add ( m_strDob );
	csv.Add ( m_strYearGroup );
	csv.Add ( m_strRegGroup );
	if ( m_pData->GetMisIDUserTextNo() > 0 )	csv.Add ( m_strParentPayMisID ); 
	if ( m_pData->GetUPNUserTextNo() > 0 )		csv.Add ( m_strParentPayUPN ); 
	if ( m_pData->GetRollNoUserTextNo() > 0)	csv.Add ( m_strParentPayRollNo ); 
	csv.Add ( m_strGender );

	if ( bActive == FALSE )
	{
		++m_nInactive;
		csv.Add ( "Inactive - skipped" );
	}
	else
	{
		if (m_strExceptionComment == "")
		{
			csv.Add(m_strStatus);
		}
		else
		{
			csv.Add(m_strExceptionComment);
		}
	}

	return ( m_Report.WriteLine ( csv ) );
}

//***********************************************************

bool CImportParentPayFile::IsStatusActive()
{
	return ( m_strStatus.CompareNoCase ( "INACTIVE" ) == 0 ) ? FALSE : TRUE;
}

//***********************************************************

void CImportParentPayFile::AddStatistics()
{
	CString strLine;
	m_Report.WriteLine ( "" );

	strLine.Format ( "%d\t<..>Lines read", m_nRead );
	m_Report.WriteLine ( strLine );

	if ( m_pData->m_bAddUnknowns == TRUE || m_nAdded > 0 )
	{
		strLine.Format ( "%d\t<..>New accounts added by '%s' ( * )", m_nAdded, m_pData->GetNewIntakeAddedByText() );
		m_Report.WriteLine ( strLine );
	}

	strLine.Format ( "%d\t<..>Existing accounts", m_nExisting );
	m_Report.WriteLine ( strLine );

	{
		CString strLocBy = "Accounts located by ";

		if (m_nLocatedByRollNo > 0)
		{
			strLine.Format("%d\t<..>%s'%s' ( !! )",
				m_nLocatedByRollNo,
				(const char*)strLocBy,
				m_pData->GetLegacyLocateByText(nPARENTPAY_LOCATEBY_ROLLNO_INDEX));

			m_Report.WriteLine(strLine);
		}

		if (m_nLocatedByUPN > 0)
		{
			strLine.Format("%d\t<..>%s'%s' ( !! )",
				m_nLocatedByUPN,
				(const char*)strLocBy,
				(const char*)m_pData->GetLegacyLocateByText(nPARENTPAY_LOCATEBY_UPN_INDEX));

			m_Report.WriteLine(strLine);
		}

		if (m_nLocatedByMisID > 0)
		{
			strLine.Format("%d\t<..>%s'%s' ( !! )",
				m_nLocatedByMisID,
				(const char*)strLocBy,
				(const char*)m_pData->GetLegacyLocateByText(nPARENTPAY_LOCATEBY_MISID_INDEX));

			m_Report.WriteLine(strLine);
		}
	}

	if ( m_nInactive > 0 )
	{
		strLine.Format ( "%d\t<..>Inactive accounts skipped", m_nInactive );
		m_Report.WriteLine ( strLine );
	}

	strLine.Format ( "%d\t<..>Exceptions found ( x )", m_nIgnored );
	m_Report.WriteLine ( strLine );
}

//**********************************************************************

void CImportParentPayFile::ImportLine ( const char* szCardNo ) 
{
	if ( m_strExceptionComment == "" && IsStatusActive() == TRUE )
	{
		++m_nUsed;

		bool bSendMatchAdvice = TRUE;							// assume sending match advice

		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserIDString(szCardNo);
		bool bUpsertRow = FALSE;

		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
		{
			if (m_bForceMatchAccount == FALSE)				// if not forced - only send match if not done before
			{
				if (RowAccount.GetMemberID() != "")		// see if ParentPayID already set
				{
					bSendMatchAdvice = FALSE;					// yes - don't send match advice
				}
			}

			SetRecord(RowAccount);										// set record fields & save pending if required
			bUpsertRow = TRUE;
		}
		else
		{
			RowAccount.SetUserIDString(szCardNo);

			++m_nAdded;
			SetRecord(RowAccount);										// update new record
			RowAccount.CopyGroupMaxSpendSettings();
			bUpsertRow = TRUE;
			
			CSQLAuditRecord atc;
			atc.Set( &RowAccount );
			atc.SetApplicationNo ( APPNO_INSERTRECORDPPAY );
			atc.SaveNewLine();
		}

		if (TRUE == bUpsertRow)
		{
			RepoAccount.UpsertRow(RowAccount, NULL);
		}

		if (bSendMatchAdvice == TRUE)
		{
			SaveDinerMatchAdvice(RowAccount);
		}
	}
	else
	{
		++m_nIgnored;
	}
}
	
//**********************************************************************
// check parentpay data again record data to see if same record 

bool CImportParentPayFile::MatchRecordUPN ( const char* szCardNo )
{
	bool bResult = TRUE;

	if ( m_strParentPayUPN != "" )							//v1.03d
	{
		int nField = m_pData->GetUPNUserTextNo();
		if ( nField > 0 )
		{
			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserIDString(szCardNo);

			CSQLRepositoryAccount RepoAccount;
			if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
			{
				CString strCardData = RowAccount.GetInfo ( nField );	// 0-not defined, info1 - info8
				
				if ( strCardData != "" )
				{
					if ( strCardData != m_strParentPayUPN )
					{
						m_strExceptionComment.Format ( "Account UPN mismatch (%s)", 
							(const char*) strCardData );

						bResult = FALSE;
					}
				}
			}
		}
	}

	return bResult;
}

//**********************************************************************

void CImportParentPayFile::SetRecord(CSQLRowAccountFull& RowAccount)
{
	RowAccount.SetForename ( m_strForename ); 
	RowAccount.SetSurname ( m_strSurname );
	RowAccount.SetGender ( m_strGender );
	RowAccount.SetMemberID ( m_strParentPayID );					// Member#

	int nField = 0;
	if ((nField = m_pData->GetMisIDUserTextNo()) > 0 && m_strParentPayMisID != "")		//v1.03d
	{
		RowAccount.SetInfo(nField, m_strParentPayMisID);		// 0-not defined, info1 - info8
	}

	if ((nField = m_pData->GetUPNUserTextNo()) > 0 && m_strParentPayUPN != "")			//v1.03d
	{
		RowAccount.SetInfo(nField, m_strParentPayUPN);		// 0-not defined, info1 - info8
	}

	if ((nField = m_pData->GetRollNoUserTextNo()) > 0 && m_strParentPayRollNo != "")	//v1.03d
	{
		RowAccount.SetInfo(nField, m_strParentPayRollNo);		// 0-not defined, info1 - info8
	}

	if ((nField = m_pData->m_nYearInfoIndex) > 0)
	{
		RowAccount.SetInfo(nField, m_strYearGroup);			// 0-not defined, info1 - info8
	}

	if ((nField = m_pData->m_nRegInfoIndex) > 0)
	{
		RowAccount.SetInfo(nField, m_strRegGroup);			// 0-not defined, info1 - info8
	}

	if (m_pData->m_nDobInfoIndex == 1)							// 0-not defined, 1=Dob1
	{
		RowAccount.SetDOB(m_strDob);
	}
}

//**********************************************************************

void CImportParentPayFile::SaveDinerMatchAdvice(CSQLRowAccountFull& RowAccount)
{
	bool bAddHeader = ( ::GetFileLineCount ( m_strDinerMatchAdviceFile ) < 2 ) ? TRUE : FALSE;

	CSSFile file;
	if ( file.Open ( m_strDinerMatchAdviceFile, "ab" ) == TRUE )
	{
		if (bAddHeader == TRUE)
		{
			file.WriteLine(m_pData->GetDinerMatchHeader());
		}

		CCSV csv;
		csv.Add ( m_pData->GetNextPosTxnID ( TRUE ) );	// get next txn ID & increment
		csv.Add ( m_strParentPayID );
		csv.Add ( RowAccount.GetUserID() );
		csv.Add ( m_strParentPayUPN );
		csv.Add ( "0" );								// opening balance

		file.Write ( csv );
		file.Close();
	}
}

/**********************************************************************/

void CImportParentPayFile::ShowStatistics()
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

	CImportStatsDlg dlg ( Config );
	dlg.DoModal();
}

//**********************************************************************

