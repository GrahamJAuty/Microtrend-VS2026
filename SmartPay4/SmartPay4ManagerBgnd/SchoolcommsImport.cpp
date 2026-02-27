//*******************************************************************
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\WaitDlg.h"
//*******************************************************************
#include "SchoolComms.hpp"
#include "WebPaymentDefines.h"
//*******************************************************************
#include "SchoolcommsImport.h"
//*******************************************************************

static const char* szHEADER = "a:BatchID,a:AccountPaymentID,a:PaymentAmount,a:PaymentDate,a:SIMSPersonID";

//*******************************************************************

CSchoolcommsImport::CSchoolcommsImport(CWnd* pParent)
{
	m_pParent = pParent;
	m_bNewPayments = TRUE;
	m_bMoreRecordsAvailable = FALSE;

	m_data.Read();													// Read Schoolcomms System data
	m_strError = (m_data.IsSet() == FALSE) ? szINTERFACENOTSET : "";

	m_strTitle = "Schoolcomms Web Payment Import";

	m_nExceptionCount = 0;
	m_nValidCount = 0;
	m_nPaymentBatchID = 0;
	m_nAccountPurse = 1;

	m_strLogFileName = "";
	m_strTempLogFile = "";
	m_strExceptionName = "";
	m_strTempExceptionFile = "";
	m_strExceptionIndicator = Filenames.GetWebPaymentExceptionIndicator();		// web payment exception indicator file
}

//*******************************************************************

bool CSchoolcommsImport::Import(bool bNewPayments)
{
	bool bReply = FALSE;
	m_bNewPayments = bNewPayments;

	if (m_strError != "")											// szINTERFACENOTSET 
	{
		return FALSE;
	}

	// import payments

	int nLoop = (m_bNewPayments == TRUE) ? 3 : 1;

	for (int i = 0; i < nLoop; i++)								// loop a maximum number of time as a safey precaution
	{																// will allow max of allow 3 x 4000 imports - should be enough for anyone!
		m_bMoreRecordsAvailable = FALSE;							// set if more new web payments to come

		m_nAccountPurse = m_data.m_nAccountPurse;				// purse to add to
		m_nPaymentBatchID = m_data.m_nLastPaymentID;				// last batch payment ID

		CString strFilename;
		if (m_bNewPayments == TRUE)
		{
			strFilename = GetWebPayments();							// import new payments
		}
		else
		{
			strFilename = m_data.GetWebPaymentExceptionFilename();	// or process pending exceptions
		}

		if (::FileExists(strFilename) == TRUE)
		{
			bReply = ImportPayments(strFilename);
		}

		if (m_strError != "")									// check for any errors
		{
			DisplayMessage(m_strError, MB_ICONSTOP, IDOK);
			LogMessage(m_strError);
			break;
		}

		if (m_bMoreRecordsAvailable == TRUE)					// see if more web payments still to come
		{
			if (CModuleGlobals::IsBackgroundModule() == FALSE)
			{
				if (Prompter.YesNo(szMSG_HAVEMOREPAYMENTS, m_strTitle, TRUE, TRUE) == IDNO)
				{
					break;
				}
			}
		}
		else
		{
			break;
		}
	}

	return bReply;
}

//*******************************************************************
// get web payments & return filename

CString CSchoolcommsImport::GetWebPayments()
{
	m_strError = "";

	CString strLoginReply = Filenames.GetWebPaymentImportFilename("xml");
	CString strPaymentsFile = Filenames.GetWebPaymentImportFilename("dat");

	// fiddle - for testing only
	//strPaymentsFile = m_pDatabase->GetWebPaymentPath ( "PayAcc-20110916-123205.dat" );
	//return strPaymentsFile;

	CWaitDlg dlgWait(m_pParent);
	dlgWait.SetMessageText(szINTERNETMESSAGE);

	CSSSchoolcomms schoolcomms(&m_data);
	m_strError = schoolcomms.GetPayments(strLoginReply, strPaymentsFile, szHEADER);
	if (m_strError == "")
	{
		m_bMoreRecordsAvailable = schoolcomms.MoreRecordsAvailable();
	}

	return strPaymentsFile;
}

//*******************************************************************

bool CSchoolcommsImport::ImportPayments ( const char* szImportFilename )
{
	CWebPaymentLogRecord LogRecord;

	CSSUnsortedDatabase dbPayments;
	if (dbPayments.OpenSSDatabaseReadWrite(szImportFilename, "", 10) != DB_ERR_NONE)	// open intermediate csv importfile
	{
		m_strError.Format("Error in opening Import file ' %s ' for Read/Write", szImportFilename);
		return FALSE;
	}

// ** create filenames & temp filenames 

	m_strLogFileName	= LogRecord.GetLogFileName();			// "InYYYYMM"
	m_strExceptionName	= LogRecord.GetExceptionFileName();		// "ExYYYYMM"

	m_strTempLogFile.Format ( "%s.$$$", m_strLogFileName );
	m_strTempLogFile = System.GetWebPaymentPath ( m_strTempLogFile );

	m_strTempExceptionFile.Format ( "%s.$$$", m_strExceptionName );
	m_strTempExceptionFile = System.GetWebPaymentPath ( m_strTempExceptionFile );

//** Open WebPayment List file  **************************************

	CString strListFilename = m_data.GetWebPaymentListFilename();

	if ( m_WebPaymentList.Open ( strListFilename, DB_READWRITE ) != DB_ERR_NONE )	// ret 0=ok, 1=file error, 2=file in use
	{
		m_strError.Format ( "Error in opening the Schoolcomms Payment List file ' %s ' for Read/Write", strListFilename );
		return FALSE;
	}

// import payments

	m_nValidCount		= 0;
	m_nExceptionCount	= 0;
	int nRecordCount	= dbPayments.GetRecordCount();

	if ( nRecordCount > 0 )										// if empty if no new payments
	{
		//int nRecordIndex = m_pDbSmartPay->GetCurrentRow();		// get current record index

		//m_pDbSmartPay->Close();
		//if ( m_pDbSmartPay->ReOpenReadWriteForSharing ( Filenames.GetDatabaseBackupFolderInfo() ) == DB_ERR_NONE )
		{
			CMemberDatabase dbBalanceList(m_data.GetBalanceDBListFilename());											// holds list of membership balances to send back
			dbBalanceList.OpenForSharing ( DB_READWRITE );

			dbPayments.MoveFirst();

			for ( int i = 0 ; i < nRecordCount ; i++ )
			{
				if (SavePendingUpdate(&dbPayments, &LogRecord, &dbBalanceList) == TRUE)
				{
					dbPayments.DeleteRecord();	// Delete line from payments file
				}
				else
				{
					dbPayments.MoveNext();		// no delete - get next line
				}
			}

			dbBalanceList.Close();

			/*
			if ( m_pDbSmartPay->Close() == FALSE )
				::LogSysFileError ( m_pDbSmartPay->GetError(), "", "SchoolcommsImport:Close" );
			*/
		}
		/*
		else
		{
			CString strError = m_pDbSmartPay->GetError();
			::LogSysFileError ( strError, "", "SchoolcommsImport:Open" );
			DisplayMessage ( strError, MB_ICONSTOP, IDOK );
			LogMessage( strError );
		}

		m_pDbSmartPay->ReOpenReadOnlyForSharing();
		m_pDbSmartPay->MoveTo(nRecordIndex);
		*/
	}

//** process imports *************************************************

	DisplayImportResults( nRecordCount );

	if (m_bNewPayments == TRUE)
	{
		if (m_nValidCount > 0 || m_nExceptionCount > 0)
		{
			m_data.m_nLastPaymentID = m_nPaymentBatchID;		// Highest TransactionID may have changed
			m_data.Save();										// save for next time
		}
	}

// close/save Schoolcomms processed listing

	m_WebPaymentList.Close();									// close file

// sort out imported payments file *******************************

	bool bDeleteImportFile = FALSE;								// set if import file can be deleted

	nRecordCount = dbPayments.GetRecordCount();					// get record count in imported payments file before closing will have changed due to deletes
	dbPayments.Close();											// close import Payments file

	if (nRecordCount == 0)									// empty file
	{
		bDeleteImportFile = TRUE;							// yes - delete empty file
	}
	else
	{														// some lines left - need to save exceptions for processing later
		if (m_bNewPayments == TRUE)							// but ONLY if processing new payments
		{
			CString strExceptions = m_data.GetWebPaymentExceptionFilename();
			::AppendDataFile(szImportFilename, strExceptions, TRUE);	// append new exceptions to old exceptions ( skip header line )
			bDeleteImportFile = TRUE;									// delete import file

			DisplayMessage(szMSG_EXCEPTIONSAVED, MB_ICONINFORMATION, IDOK);// "The unprocessed payments have been added to the Outstanding Exceptions List", MB_ICONINFORMATION );
			LogMessage(szMSG_EXCEPTIONSAVED);
		}
	}

// save exceptions log if required

	if (m_bNewPayments == TRUE && m_nExceptionCount > 0)	// only log exceptions on importing New Payments
	{
		CString strFile;	strFile.Format("%s.dat", m_strExceptionName);
		strFile = System.GetWebPaymentPath(strFile);
		::AppendDataFile(m_strTempExceptionFile, strFile);
	}

// tidy up temp files

	if (bDeleteImportFile == TRUE)						// see if delete import or exception file allowed
	{
		CFileRecycler::SendToRecycleBin(szImportFilename);			// delete import/exception file
	}

	CFileRecycler::SendToRecycleBin ( m_strTempLogFile );				// delete session import log
	CFileRecycler::SendToRecycleBin ( m_strTempExceptionFile );			// delete session exceptions

	return TRUE;
}

//*******************************************************************
// bReply TRUE = delete payment for payment file / FALSE = Save Exception for retry later

bool CSchoolcommsImport::SavePendingUpdate ( CSSUnsortedDatabase* pDbPayments, CWebPaymentLogRecord* pLogRecord, CMemberDatabase* pDbBalanceList )
{
	bool bReply = TRUE;									// set payment line to be deleted

	if ( m_bNewPayments == TRUE )						// see if importing new payments
	{
		__int64 nBatchID = pDbPayments->GetInt64 ( "a:BatchID" );

		if (nBatchID > m_nPaymentBatchID)
		{
			m_nPaymentBatchID = nBatchID;				// save it 
		}
	}

	CString strPupilName	= "";						// get name from card database
	CString strBatchID		= pDbPayments->GetString ( "a:BatchID" );
	__int64 nPaymentID		= pDbPayments->GetInt64  ( "a:AccountPaymentID" );
	CString strPupilID		= pDbPayments->GetString ( "a:SIMSPersonID" );
	double dAmountPaid		= pDbPayments->GetDouble ( "a:PaymentAmount" );
	CString strDateTime		= pDbPayments->GetString ( "a:PaymentDate" );

//	pLogRecord->SetServiceID ( strAccount );			// already set
//	pLogRecord->SetPrefix ( szText )					// done on saving log
	pLogRecord->SetPaymentID ( nPaymentID );
	pLogRecord->SetPupilID ( strPupilID );
	pLogRecord->SetPaidAmount ( dAmountPaid );	
	pLogRecord->SetPaidDate ( strDateTime );
	pLogRecord->SetTransactionRef ( strBatchID );
	pLogRecord->SetPupilName ( strPupilName );			// clear the name incase no membership entry
//	pLogRecord->SetReceiptCode ( const char* szText );	// not used - no data
//	pLogRecord->SetDOB ( const char* szText );			// not used - no data
//	pLogRecord->SetNotes ( const char* szText );		// not used - no data

	if ( m_WebPaymentList.FindRecord ( nPaymentID ) == FALSE )		// see if already processed
	{
		if ( dAmountPaid != 0.0 )
		{
			CSQLRepositoryAccount RepoAccount;
			__int64 nUserID = RepoAccount.LookupMemberID(strPupilID, NULL).GetSQLResult();
			
			if ( nUserID != 0 )		// get card number for matching pupilID
			{
				CString strUserID = FormatInt64Value(nUserID);

				CSQLRowAccountFull RowAccount;
				RowAccount.SetUserID(_atoi64(strUserID));

				if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
				{
					//if ( ::IsAccountInuse ( strUserID ) == FALSE )
					{
						strPupilName = RowAccount.GetUsername();
						pLogRecord->SetPupilName ( strPupilName );

						CSQLAuditRecord atc;
						atc.Set ( &RowAccount );							// set audit is for a pc line
						atc.SetOperatorID ( Passwords.GetPasswordInUse() );
						atc.SetApplicationNo ( APPNO_SCHOOLCOMMS );
						atc.SetWebPaymentInfo ( nPaymentID );

						switch ( m_nAccountPurse )
						{
						case 3:		RowAccount.AddToPurse3Liability ( dAmountPaid );	break;
//						case 2:		RowAccount.AddToPurse2 ( dAmountPaid );	break;
						default:	RowAccount.AddToPurse1Liability ( dAmountPaid );	break;
						}

						RepoAccount.UpdateRow(RowAccount,NULL);
						
						if (atc.AuditChanges(&RowAccount) == TRUE)		// audit line
						{
							atc.SaveNewLine();									// save audit line
						}
						pLogRecord->LogValidImport ( m_strTempLogFile, strUserID, m_nValidCount );

						m_WebPaymentList.AddBlankRecord ( nPaymentID );						// add new record to webpayment list
						m_WebPaymentList.SetServiceID ( pLogRecord->GetServiceID() );	// set new record fields
						m_WebPaymentList.SetAmount ( dAmountPaid );
						m_WebPaymentList.SetDate ( pLogRecord->GetDate() );
						m_WebPaymentList.SetTime ( pLogRecord->GetTime() );
						m_WebPaymentList.SetUserID ( strUserID );
						m_WebPaymentList.SetName ( strPupilName );

// as payment received add to balance list database so send back balances of accounts that have changed

						if ( m_bNewPayments == TRUE )						// see if importing new payments
						{
							if (pDbBalanceList->FindRecord(strPupilID) == FALSE)
							{
								pDbBalanceList->AddBlankRecord(strPupilID);
							}
							pDbBalanceList->SetUserID ( strUserID );		// add to list of balances to send back
						}
					}
					/*
					else
					{
						LogException ( pLogRecord, szMSG_ACCOUNTINUSE );	// Account in use
						bReply = FALSE;										// payment will not get deleted so will be added to retry exception file
					}
					*/
				}
				else
				{
					LogException ( pLogRecord, szMSG_NOACCOUNT );			// "No account record" );
					bReply = FALSE;											// payment will not get deleted so will be added to retry exception file
				}
			}
			else
			{
				LogException ( pLogRecord, szMSG_NOMEMBER );
				bReply = FALSE;
			}
		}
		else
			LogException ( pLogRecord, szMSG_NOPAIDAMOUNT );				// "No paid amount" );
	}
	else
		pLogRecord->LogAlreadyProcessed ( m_strTempExceptionFile, m_WebPaymentList.GetDate(), m_WebPaymentList.GetTime(), m_nExceptionCount );  // "Already processed ( %s %s )"

	return bReply;													// TRUE = delete line \ FALSE - no delete 
}

//******************************************************************

void CSchoolcommsImport::LogException ( CWebPaymentLogRecord* pLogRecord, const char* szErrorText ) 
{
	pLogRecord->LogException ( m_strTempExceptionFile, m_strExceptionIndicator, szErrorText, m_nExceptionCount );
}

//*******************************************************************
