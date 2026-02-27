//*******************************************************************
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\WaitDlg.h"
//*******************************************************************
#include "WebPaymentDefines.h"
#include "WisePay.hpp"
//*******************************************************************
#include "WisePayImport.h"
//*******************************************************************
//
//static const char* szHEADER	 = "uID,cart_student_sims_id,cart_student_name,cart_timestamp,price_value";
//
//*******************************************************************

CWisePayImport::CWisePayImport(CWnd* pParent)
{
	m_pParent = pParent;
	m_bNewPayments = TRUE;

	m_data.Read();															// Read Tucasi System data
	m_strError = (m_data.IsSet() == FALSE) ? szINTERFACENOTSET : "";

	m_strTitle = "WisePay Web Payment Import";

	m_nExceptionCount = 0;
	m_nValidCount = 0;
	m_nHighestPaymentID = 0;
	m_nAccountPurse = 1;												// purse to sse for payments

	m_strLogFileName = "";
	m_strTempLogFile = "";
	m_strTempExceptionFile = "";
	m_strExceptionName = "";
	m_strExceptionIndicator = Filenames.GetWebPaymentExceptionIndicator();		// web payment exception indicator file
}

//*******************************************************************

bool CWisePayImport::Import ( bool bNewPayments )
{
	bool bReply			= FALSE;
	m_bNewPayments		= bNewPayments;

	if (m_strError != "")											// szINTERFACENOTSET 
	{
		return FALSE;
	}

	if ( m_data.ImportPayments() == FALSE )
	{
		if ( CModuleGlobals::IsBackgroundModule() == FALSE )							// started from menu option
		{
			m_strError = szMSG_IMPORTDISABLED;
			return FALSE;
		}
		return TRUE;
	}

	CString strFilename = "";

// Account 1

	if ( m_data.m_bImportAccount1 == TRUE )
	{
		int nAccountNo = 1;									// check Account1 payments \ exceptions
		m_nAccountPurse		= m_data.m_nAccount1Purse;
		m_nHighestPaymentID = m_data.m_nLastAccount1;		// hightest account1 Payment ID so far

		if ( m_bNewPayments == TRUE )	strFilename = GetWebPayments ( nAccountNo );		// access internet & get new payments
		else							strFilename = m_data.GetWebPaymentExceptionFilename ( nAccountNo );		// or process pending exceptions

		if ( m_strError == "" )
		{
			if ( ::FileExists ( strFilename ) == TRUE )
				bReply = ImportPayments ( nAccountNo, strFilename );
		}
	}

//Account 2

	if ( m_strError == "" && m_data.m_bImportAccount2 == TRUE )
	{
		int nAccountNo = 2;									// check Account2 payments \ exceptions
		m_nAccountPurse		= m_data.m_nAccount2Purse;
		m_nHighestPaymentID = m_data.m_nLastAccount2;		// highest account2 PaymentID so far

		if ( m_bNewPayments == TRUE )	strFilename = GetWebPayments ( nAccountNo );	// access internet & get new payments
		else							strFilename = m_data.GetWebPaymentExceptionFilename ( nAccountNo );	// or retry exceptions

		if ( m_strError == "" )
		{
			if ( ::FileExists ( strFilename ) == TRUE )
				bReply = ImportPayments ( nAccountNo, strFilename );
		}
	}

	return bReply;
}

//*******************************************************************
// get web payments & return filename

CString CWisePayImport::GetWebPayments ( int nAccountNo )
{
	m_strError = "";
	int nAccountID = ( nAccountNo == 1 ) ? m_data.m_nAccountID1 : m_data.m_nAccountID2;

// fiddle - for testing only
//CString strDestinationFilename = m_pDatabase->GetWebPaymentPath ( "PayAcc248-20110712-140005.dat" );
//return strDestinationFilename;

	CString strPaymentsFile = "";		// empty = if not valid AccountNo

	if ( nAccountID != 0 )
	{
		CString strAccountID;
		strAccountID.Format ( "%d", nAccountID );

		CString strLoginReply	= Filenames.GetWebPaymentImportFilename ( "xml", strAccountID );
		strPaymentsFile			= Filenames.GetWebPaymentImportFilename ( "dat", strAccountID );

		CWaitDlg dlgWait ( m_pParent );
		dlgWait.SetMessageText( szINTERNETMESSAGE );

		CSSWisePay wp ( &m_data );
		m_strError = wp.GetPayments ( nAccountNo, strLoginReply, strPaymentsFile );
	}

	return strPaymentsFile;
}

//*******************************************************************

bool CWisePayImport::ImportPayments ( int nAccountNo, const char* szImportFilename )
{
	int nAccountID = ( nAccountNo == 1 ) ? m_data.m_nAccountID1 : m_data.m_nAccountID2;

	CWebPaymentLogRecord LogRecord;
	LogRecord.SetServiceID ( nAccountID );

	CSSUnsortedDatabase dbPayments;
	if ( dbPayments.OpenSSDatabaseReadWrite( szImportFilename, "", 10 ) != DB_ERR_NONE )	// open intermediate csv importfile
	{
		m_strError.Format ( "Error in opening Import file ' %s ' for Read/Write", szImportFilename );
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

	CString strListFilename = m_data.GetWebPaymentListFilename ( nAccountNo );

	if ( m_WebPaymentList.Open ( strListFilename, DB_READWRITE ) != DB_ERR_NONE )	// ret 0=ok, 1=file error, 2=file in use
	{
		m_strError.Format ( "Error in opening the WisePay Payment List file ' %s ' for Read/Write", strListFilename );
		return FALSE;
	}

//** import payments

	m_nValidCount		= 0;
	m_nExceptionCount	= 0;
	int nRecordCount	= dbPayments.GetRecordCount();

	if ( nRecordCount > 0 )										// if empty if no new payments
	{
		{
			dbPayments.MoveFirst();

			for ( int i = 0 ; i < nRecordCount ; i++ )
			{
				if (SavePendingUpdate(&dbPayments, &LogRecord) == TRUE)
				{
					dbPayments.DeleteRecord();	// Delete line from payments file
				}
				else
				{
					dbPayments.MoveNext();		// no delete - get next line
				}
			}
		}
		/*
		else
		{
			CString strError = m_pDbSmartPay->GetError();
			::LogSysFileError ( strError, "", "WisePayImport:Open" );
			DisplayMessage ( strError, MB_ICONSTOP, IDOK );
			LogMessage( strError );
		}
		*/
	}

//** process imports ****************************

	DisplayImportResults( nRecordCount, nAccountID );

	if ( m_bNewPayments == TRUE )
	{
		if ( m_nValidCount > 0 || m_nExceptionCount > 0 )
		{
			if ( nAccountNo == 1 )	m_data.m_nLastAccount1 = m_nHighestPaymentID;	// Highest PaymentID may have changed
			else					m_data.m_nLastAccount2 = m_nHighestPaymentID;
			m_data.Save();															// save for next time
		}
	}

// close/save Tucasi processed listing

	m_WebPaymentList.Close();									// close file

// sort out imported payments file *******************************

	nRecordCount = dbPayments.GetRecordCount();				// get record count in imported payments file before closing will have changed due to deletes
	dbPayments.Close();										// close import Payments file

	if ( nRecordCount == 0 )								// empty file
		CFileRecycler::SendToRecycleBin ( szImportFilename );			// delete import file
	else
	{														// some lines left - need to save exceptions for processing later
		if ( m_bNewPayments == TRUE )						// but ONLY if processing new payments
		{
			CString strExceptions = m_data.GetWebPaymentExceptionFilename ( nAccountNo );
			::AppendDataFile ( szImportFilename, strExceptions, TRUE );	// append new exceptions to old exceptions ( skip header line )
			CFileRecycler::SendToRecycleBin ( szImportFilename );					// delete whats left of new imports file

			DisplayMessage ( szMSG_EXCEPTIONSAVED, MB_ICONINFORMATION, IDOK );
			LogMessage( szMSG_EXCEPTIONSAVED );
		}
	}
	
// log exceptions

	if ( m_bNewPayments == TRUE && m_nExceptionCount > 0 )
	{
		CString strFile;
		strFile.Format ( "%s.dat", m_strExceptionName );
		strFile = System.GetWebPaymentPath ( strFile );
		::AppendDataFile ( m_strTempExceptionFile, strFile );
	}

// tidy up temp files

	CFileRecycler::SendToRecycleBin ( m_strTempLogFile );				// delete session import log
	CFileRecycler::SendToRecycleBin ( m_strTempExceptionFile );			// delete session exceptions

	return TRUE;
}

//*******************************************************************
// "uID,cart_student_sims_id,cart_student_name,cart_timestamp,price_value";
// bReply TRUE = delete payment for payment file / FALSE = Save Exception for retry later

bool CWisePayImport::SavePendingUpdate ( CSSUnsortedDatabase* pDbPayments, CWebPaymentLogRecord* pLogRecord )
{
	bool bReply			= TRUE;
	__int64 nPaymentID	= pDbPayments->GetInt64("uID");

	if ( m_bNewPayments == TRUE )						// see if importing new payments
	{
		if ( nPaymentID > m_nHighestPaymentID )			// check if have new highest PaymentID 
			m_nHighestPaymentID = nPaymentID;			// save it 
	}

	CString strPupilID		= pDbPayments->GetString("cart_student_sims_id");
	CString strPupilName	= pDbPayments->GetString("cart_student_name");
	CString strDateTime		= pDbPayments->GetString("cart_timestamp");
	double dAmountPaid		= pDbPayments->GetDouble("price_value");

//	pLogRecord->SetServiceID ( strAccount );			// already set
//	pLogRecord->SetPrefix ( szText )					// done on saving log line
	pLogRecord->SetPaymentID ( nPaymentID );
	pLogRecord->SetPupilID ( strPupilID );
	pLogRecord->SetPupilName ( strPupilName );
	pLogRecord->SetPaidAmount ( dAmountPaid );	
	pLogRecord->SetPaidDate ( strDateTime );
//	pLogRecord->SetReceiptCode ( const char* szText );			// no data
//	pLogRecord->SetTransactionRef ( const char* szText );		// no data
//	pLogRecord->SetDOB ( const char* szText );					// no data
//	pLogRecord->SetNotes ( const char* szText );				// no data

	if ( m_WebPaymentList.FindRecord ( nPaymentID ) == FALSE )		// see if already processed
	{
		if ( dAmountPaid != 0.0 )
		{
			CSQLRepositoryAccount RepoAccount;
			__int64 nUserID = RepoAccount.LookupMemberID(strPupilID, NULL).GetSQLResult();

			if ( nUserID != 0 )			// get UserID from membership number
			{
				CString strUserID = FormatInt64Value(nUserID);

				CSQLRowAccountFull RowAccount;
				RowAccount.SetUserID(_atoi64(strUserID));

				if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
				{
					//if ( ::IsAccountInuse ( strUserID ) == FALSE )
					{
						CSQLAuditRecord atc;
						atc.Set ( &RowAccount );			// set audit is for a pc line
						atc.SetOperatorID ( Passwords.GetPasswordInUse() );
						atc.SetApplicationNo ( APPNO_WISEPAY );
						atc.SetWebPaymentInfo ( nPaymentID );

						switch ( m_nAccountPurse )
						{
						case 3:		RowAccount.AddToPurse3Liability ( dAmountPaid );	break;
						case 2:		RowAccount.AddToPurse2Balance ( dAmountPaid );		break;	// WhitGift don't have FSM vai Purse2
						default:	RowAccount.AddToPurse1Liability ( dAmountPaid );	break;
						}

						RepoAccount.UpdateRow(RowAccount,NULL);
						
						if (atc.AuditChanges(&RowAccount) == TRUE)				// audit line
						{
							atc.SaveNewLine();											// save audit line
						}

						pLogRecord->LogValidImport ( m_strTempLogFile, strUserID, m_nValidCount );	// add to import log

						m_WebPaymentList.AddBlankRecord ( nPaymentID );						// add new record to webpayment list
						m_WebPaymentList.SetServiceID ( pLogRecord->GetServiceID() );	// set new record fields
						m_WebPaymentList.SetAmount ( dAmountPaid );
						m_WebPaymentList.SetDate ( pLogRecord->GetDate() );
						m_WebPaymentList.SetTime ( pLogRecord->GetTime() );
						m_WebPaymentList.SetUserID ( strUserID );
						m_WebPaymentList.SetName ( strPupilName );
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
					LogException ( pLogRecord, szMSG_NOACCOUNT );				// "No account record" );
					bReply = FALSE;
				}
			}
			else
			{
				LogException ( pLogRecord, szMSG_NOMEMBER );
				bReply = FALSE;
			}
		}
		else
			LogException ( pLogRecord, szMSG_NOPAIDAMOUNT ) ;
	}
	else
		pLogRecord->LogAlreadyProcessed ( m_strTempExceptionFile, m_WebPaymentList.GetDate(), m_WebPaymentList.GetTime(), m_nExceptionCount );  // "Already processed ( %s %s )"

	return bReply;
}

//******************************************************************

void CWisePayImport::LogException ( CWebPaymentLogRecord* pLogRecord, const char* szErrorText ) 
{
	pLogRecord->LogException ( m_strTempExceptionFile, m_strExceptionIndicator, szErrorText, m_nExceptionCount );
}

//*******************************************************************

