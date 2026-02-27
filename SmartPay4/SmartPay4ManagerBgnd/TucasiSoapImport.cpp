//*******************************************************************
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\WaitDlg.h"
//*******************************************************************
#include "TucasiSOAP.hpp"
#include "WebPaymentDefines.h"
//*******************************************************************
#include "TucasiSoapImport.h"
//*******************************************************************

static const char* szHeader	 = "transactionNumber,transactionItemNumber,paymentDate,paymentAmount,description,pupilNumber,pupilName,schoolID";

//*******************************************************************

CTucasiSoapImport::CTucasiSoapImport(CWnd* pParent)
{
	m_pParent = pParent;
	m_bNewPayments = TRUE;

	m_data.Read();												// Read Tucasi System data
	m_strError = (m_data.IsSet() == FALSE) ? szINTERFACENOTSET : "";

	m_strTitle = "Tucasi Web Payment Import";

	m_nExceptionCount = 0;
	m_nValidCount = 0;
	m_nHighestTransactionNo = 0;

	m_strLogFileName = "";
	m_strTempLogFile = "";
	m_strExceptionName = "";
	m_strTempExceptionFile = "";
	m_strExceptionIndicator = Filenames.GetWebPaymentExceptionIndicator();		// web payment exception indicator file
}

//*******************************************************************

bool CTucasiSoapImport::Import ( bool bNewPayments )
{
	bool bReply			= FALSE;
	m_bNewPayments		= bNewPayments;

	if ( m_strError != "" )											// szINTERFACENOTSET 
		return FALSE;

	if ( m_data.m_bImportPayments == FALSE )
	{
		if ( CModuleGlobals::IsBackgroundModule() == FALSE )						// started from menu option
		{
			m_strError = szMSG_IMPORTDISABLED;
			return FALSE;
		}
		return TRUE;
	}

	m_nHighestTransactionNo = m_data.m_nLastPaymentID;		// hightest account1 Payment ID so far

	CString strFilename;

	if ( m_bNewPayments == TRUE )	strFilename = GetWebPayments();							// access internet & get new payments
	else							strFilename = m_data.GetWebPaymentExceptionFilename();	// or process pending exceptions

	if ( m_strError == "" )
	{
		if ( ::FileExists ( strFilename ) == TRUE )
		bReply = ImportPayments ( strFilename );
	}

	return bReply;
}

//*******************************************************************
// get web payments & return filename

CString CTucasiSoapImport::GetWebPayments()
{
	m_strError = "";

// fiddle - for testing only
//CString strPaymentsFile = m_pDatabase->GetWebPaymentPath ( "PayAcc-20110916-123205.dat" );
//return strPaymentsFile;

	CString strLoginReply	= Filenames.GetWebPaymentImportFilename ( "xml" );
	CString strPaymentsFile	= Filenames.GetWebPaymentImportFilename ( "dat" );

	CWaitDlg dlgWait ( m_pParent );
	dlgWait.SetMessageText ( szINTERNETMESSAGE );

	CSSTucasiSoap tucasi ( &m_data );
	m_strError = tucasi.GetPayments ( strPaymentsFile, strLoginReply, szHeader );

	return strPaymentsFile;
}

//*******************************************************************

bool CTucasiSoapImport::ImportPayments(const char* szImportFilename)
{
	CWebPaymentLogRecord LogRecord;
	LogRecord.SetServiceID(0);		// not used

	CSSUnsortedDatabase dbPayments;
	if (dbPayments.OpenSSDatabaseReadWrite(szImportFilename, "", 10) != DB_ERR_NONE)	// open intermediate csv importfile
	{
		CString strMsg;
		strMsg.Format("Error in opening Import file ' %s ' for Read/Write", szImportFilename);
		return FALSE;
	}

	// ** create filenames & temp filenames

	m_strLogFileName = LogRecord.GetLogFileName();			// "InYYYYMM"
	m_strExceptionName = LogRecord.GetExceptionFileName();		// "ExYYYYMM"

	m_strTempLogFile.Format("%s.$$$", (const char*) m_strLogFileName);
	m_strTempLogFile = System.GetWebPaymentPath(m_strTempLogFile);

	m_strTempExceptionFile.Format("%s.$$$", (const char*) m_strExceptionName);
	m_strTempExceptionFile = System.GetWebPaymentPath(m_strTempExceptionFile);

	//** Open WebPayment List file  **************************************

	CString strListFilename = m_data.GetWebPaymentListFilename();

	if (m_WebPaymentList.Open(strListFilename, DB_READWRITE) != DB_ERR_NONE)	// ret 0=ok, 1=file error, 2=file in use
	{
		m_strError.Format("Error in opening the Tucasi Payment List file ' %s ' for Read/Write", (const char*) strListFilename);
		return FALSE;
	}

	//** import lines

	m_nValidCount = 0;
	m_nExceptionCount = 0;
	int nRecordCount = dbPayments.GetRecordCount();

	if (nRecordCount > 0)										// if empty if no new payments
	{
		dbPayments.MoveFirst();

		for (int i = 0; i < nRecordCount; i++)
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

//** see if any data has been imported

	DisplayImportResults( nRecordCount );

	if ( m_bNewPayments == TRUE )
	{
		if ( m_nValidCount > 0 || m_nExceptionCount > 0 )
		{
			m_data.m_nLastPaymentID = m_nHighestTransactionNo;	// Highest PaymentID may have changed
			m_data.Save();										// save for next time
		}
	}

// close/save Tucasi processed listing

	m_WebPaymentList.Close();								// close file

// sort out imported payments file *******************************

	nRecordCount = dbPayments.GetRecordCount();				// get record count in imported payments file before closing will have changed due to deletes
	dbPayments.Close();										// close import Payments file

	if (nRecordCount == 0)								// empty file
	{
		CFileRecycler::SendToRecycleBin(szImportFilename);			// delete import file
	}
	else
	{														// some lines left - need to save exceptions for processing later
		if ( m_bNewPayments == TRUE )						// but ONLY if processing new payments
		{
			CString strExceptions = m_data.GetWebPaymentExceptionFilename();
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
		strFile.Format ( "%s.dat", (const char*) m_strExceptionName );
		strFile = System.GetWebPaymentPath ( strFile );
		::AppendDataFile ( m_strTempExceptionFile, strFile );
	}

// tidy up temp files

	CFileRecycler::SendToRecycleBin ( m_strTempLogFile );				// delete session import log
	CFileRecycler::SendToRecycleBin ( m_strTempExceptionFile );			// delete session exceptions

	return TRUE;
}

//*******************************************************************
// bReply TRUE = delete payment for payment file / FALSE = Save Exception for retry later

bool CTucasiSoapImport::SavePendingUpdate ( CSSUnsortedDatabase* pDbPayments, CWebPaymentLogRecord* pLogRecord )
{
	bool bReply				= TRUE;
	__int64 nTransactionNo	= pDbPayments->GetInt64("transactionNumber");

	if ( m_bNewPayments == TRUE )						// see if importing new payments
	{
		if (nTransactionNo > m_nHighestTransactionNo)	// check if have new highest PaymentID 
		{
			m_nHighestTransactionNo = nTransactionNo;	// save it 
		}
	}

	int nItemNo				= pDbPayments->GetInt("transactionItemNumber");
	CString strPupilID		= pDbPayments->GetString("pupilNumber");
	CString strPupilName	= pDbPayments->GetString("pupilName");
	CString strDateTime		= pDbPayments->GetString("paymentDate");
	CString strNotes		= pDbPayments->GetString("description");
	double dAmountPaid		= pDbPayments->GetDouble("paymentAmount");

	__int64 nPaymentID = ( nTransactionNo * 100 ) + nItemNo;
	dAmountPaid /= 100;

//	pLogRecord->SetServiceID ( strAccount );			// not used - no data
//	pLogRecord->SetPrefix ( szText )					// done on saving log
	pLogRecord->SetPaymentID ( nTransactionNo );
	pLogRecord->SetPupilID ( strPupilID );
	pLogRecord->SetPupilName ( strPupilName );
	pLogRecord->SetPaidAmount ( dAmountPaid );	
	pLogRecord->SetPaidDate ( strDateTime );
	pLogRecord->SetNotes ( strNotes );
//	pLogRecord->SetReceiptCode ( const char* szText );		// not used - no data
//	pLogRecord->SetTransactionRef ( const char* szText );	// not used - no data
//	pLogRecord->SetDOB ( const char* szText );				// not used - no data

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

				if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
				{
					//if ( ::IsAccountInuse ( strUserID ) == FALSE )
					{
						CSQLAuditRecord atc;
						atc.Set ( &RowAccount );								// set audit is for a pc line
						atc.SetOperatorID ( Passwords.GetPasswordInUse() );
						atc.SetApplicationNo ( APPNO_TUCASI );
						atc.SetWebPaymentInfo ( nTransactionNo );				// Set PaymnetID
						atc.SetLineComment ( strNotes );

						switch ( m_data.m_nAccountPurse )
						{
						case 3:		RowAccount.AddToPurse3Liability ( dAmountPaid );		break;	// add value to purse
//						case 2:		RowAccount.AddToPurse2 ( dAmountPaid );		break;	// add value to purse
						default:	RowAccount.AddToPurse1Liability ( dAmountPaid );		break;	// add value to purse
						}

						RepoAccount.UpdateRow(RowAccount,NULL);
						
						if (atc.AuditChanges(&RowAccount) == TRUE)		// audit line
						{
							atc.SaveNewLine();									// save audit line
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
					LogException ( pLogRecord, szMSG_NOACCOUNT );
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
		{
			LogException(pLogRecord, szMSG_NOPAIDAMOUNT);
		}
	}
	else
	{
		pLogRecord->LogAlreadyProcessed(m_strTempExceptionFile, m_WebPaymentList.GetDate(), m_WebPaymentList.GetTime(), m_nExceptionCount);  // "Already processed ( %s %s )"
	}

	return bReply;
}

//******************************************************************

void CTucasiSoapImport::LogException ( CWebPaymentLogRecord* pLogRecord, const char* szErrorText ) 
{
	pLogRecord->LogException ( m_strTempExceptionFile, m_strExceptionIndicator, szErrorText, m_nExceptionCount );
}

//*******************************************************************
