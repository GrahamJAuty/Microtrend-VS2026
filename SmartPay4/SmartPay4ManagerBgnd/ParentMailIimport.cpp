//*******************************************************************
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\WaitDlg.h"
//*******************************************************************
#include "ParentMail.hpp"
#include "ParentMailExport.h"
#include "WebPaymentDefines.h"
//*******************************************************************
#include "ParentMailImport.h"
//*******************************************************************

static const char* szHEADER	 = "PaymentID,AccountID,PurseType,Value,PaymentType,PaymentDate";

//*******************************************************************

CParentMailImport::CParentMailImport(CWnd* pParent)
{
	m_pParent = pParent;
	m_bNewPayments = TRUE;
	m_nAccountPurse = 1;
	m_nHighestTransactionID = 0;

	m_data.Read();												// Read ParentMail System data
	m_strError = (m_data.IsSet() == FALSE) ? szINTERFACENOTSET : "";

	m_strTitle = "ParentMail +Pay Internet Access";

	m_nValidCount = 0;
	m_strLogFileName = "";
	m_strTempLogFile = "";

	m_nExceptionCount = 0;
	m_strExceptionName = "";
	m_strTempExceptionFile = "";
	m_strExceptionIndicator = Filenames.GetWebPaymentExceptionIndicator();		// web payment exception indicator file
}

//*******************************************************************

bool CParentMailImport::Import(bool bNewPayments)
{
	bool bReply = FALSE;
	m_bNewPayments = bNewPayments;

	if (m_strError != "")											// szINTERFACENOTSET 
	{
		return FALSE;
	}

	m_nAccountPurse = m_data.m_nAccountPurse;			// purse to add to
	m_nHighestTransactionID = m_data.m_nLastPaymentID;			// hightest account Payment ID so far

	CString strFilename;
	if (m_bNewPayments == TRUE)
	{
		strFilename = GetWebPayments();
	}
	else
	{
		strFilename = m_data.GetWebPaymentExceptionFilename();	// or process pending exceptions
	}

	if (::FileExists(strFilename) == TRUE)
	{
		if (m_strError == "")
		{
			bReply = ImportPayments(strFilename);
		}
		else
		{
			DisplayMessage(m_strError, MB_ICONSTOP, IDOK);
			LogMessage(m_strError);
		}
	}
	else
	{
		DisplayMessage(szMSG_NONEWPAYMENTS, MB_ICONINFORMATION, IDOK);
		LogMessage(szMSG_NONEWPAYMENTS);
	}

	return bReply;
}

//*******************************************************************
// get web payments & return filename

CString CParentMailImport::GetWebPayments()
{
	m_strError = "";

	CString strLoginReply	= Filenames.GetWebPaymentImportFilename ( "xml" );
	CString strPaymentsFile	= Filenames.GetWebPaymentImportFilename ( "dat" );

// fiddle - for testing only
//strPaymentFile = m_pDatabase->GetWebPaymentPath ( "TestPayments.dat" );
//return strPaymentsFile;
	
	CWaitDlg dlgWait ( m_pParent );
	dlgWait.SetMessageText ( szINTERNETMESSAGE );

	CParentMailExport pme ( m_pParent );
	pme.RequestLinkedAccounts ( NULL );							// see if any manual iinks have been done & are waiting to be collected

	if ( ( m_strError = pme.GetError() ) == "" )
	{
		CSSParentMail pm ( &m_data );
		m_strError = pm.GetPayments ( strLoginReply, strPaymentsFile, szHEADER );
	}

	return strPaymentsFile;
}

//*******************************************************************

bool CParentMailImport::ImportPayments(const char* szImportFilename)
{
	CWebPaymentLogRecord LogRecord;

	CSSUnsortedDatabase dbPayments;
	if (dbPayments.OpenSSDatabaseReadWrite(szImportFilename, "", 10) != DB_ERR_NONE)	// open intermediate csv importfile
	{
		CString strMsg;	strMsg.Format("Error in opening Import file ' %s ' for Read/Write", szImportFilename);
		return FALSE;
	}

	// ** create filenames & temp filenames **( log files not used if processing exceptions )*****************

	m_strLogFileName = LogRecord.GetLogFileName();			// "InYYYYMM"
	m_strExceptionName = LogRecord.GetExceptionFileName();		// "ExYYYYMM"

	m_strTempLogFile.Format("%s.$$$", m_strLogFileName);
	m_strTempLogFile = Filenames.GetWebPaymentPath(m_strTempLogFile);

	m_strTempExceptionFile.Format("%s.$$$", m_strExceptionName);
	m_strTempExceptionFile = Filenames.GetWebPaymentPath(m_strTempExceptionFile);

	//** Open WebPayment List file  **************************************

	CString strListFilename = m_data.GetWebPaymentListFilename();

	if (m_WebPaymentList.Open(strListFilename, DB_READWRITE) != DB_ERR_NONE)	// ret 0=ok, 1=file error, 2=file in use
	{
		m_strError.Format("Error in opening the ParentMail Payment List file ' %s ' for Read/Write", strListFilename);
		return FALSE;
	}

	// import payments

	m_nValidCount = 0;
	m_nExceptionCount = 0;
	int nRecordCount = dbPayments.GetRecordCount();

	if (nRecordCount > 0)										// is empty if no new payments
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

	//** process imports *************************************************

	DisplayImportResults(nRecordCount);

	// sort out WebPayment list file & update main pending files if required

	if (m_bNewPayments == TRUE)
	{
		if (m_nValidCount > 0 || m_nExceptionCount > 0)
		{
			m_data.m_nLastPaymentID = m_nHighestTransactionID;		// Highest TransactionID may have changed
			m_data.Save();											// save for next time
		}

		// sort out WebPayment list file & update main pending files if required

		CString strFile;	strFile.Format("%s.dat", m_strLogFileName);				// master import log file
		strFile = System.GetWebPaymentPath(strFile);
		::AppendDataFile(m_strTempLogFile, strFile);
	}

	// close/save ParentMail processed listing

	m_WebPaymentList.Close();								// close file

// sort out imported payments file *******************************

	bool bDeleteImportFile = FALSE;								// set if import file can be deleted

	nRecordCount = dbPayments.GetRecordCount();					// get record count in imported payments file before closing will have changed due to deletes
	dbPayments.Close();											// close import Payments file

	if (nRecordCount == 0)								// empty file
	{
		bDeleteImportFile = TRUE;							// yes - delete empty file
	}
	else
	{														// some lines left - need to save exceptions for processing later
		if (m_bNewPayments == TRUE)						// but ONLY if processing new payments
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
		strFile = Filenames.GetWebPaymentPath(strFile);
		::AppendDataFile(m_strTempExceptionFile, strFile);
	}

	// tidy up temp files

	if (bDeleteImportFile == TRUE)						// see if delete import or exception file allowed
	{
		CFileRecycler::SendToRecycleBin(szImportFilename);			// delete import/exception file
	}

	CFileRecycler::SendToRecycleBin(m_strTempLogFile);				// delete session import log
	CFileRecycler::SendToRecycleBin(m_strTempExceptionFile);			// delete session exceptions

	return TRUE;
}

//*******************************************************************
// bReply TRUE = delete payment for payment file / FALSE = Save Exception for retry later

bool CParentMailImport::SavePendingUpdate ( CSSUnsortedDatabase* pDbPayments, CWebPaymentLogRecord* pLogRecord )
{
	bool bReply			= TRUE;							// set payment line to be deleted
	__int64 nPaymentID	= pDbPayments->GetInt64 ( "PaymentID" );

	if (m_bNewPayments == TRUE)						// see if importing new payments
	{
		if (nPaymentID > m_nHighestTransactionID)		// check if have new highest PaymentID 
		{
			m_nHighestTransactionID = nPaymentID;		// save it 
		}
	}

	CString strPupilName	= "";						// get name from card database
	CString strPupilID		= pDbPayments->GetString ( "AccountID" );
	double dAmountPaid		= pDbPayments->GetDouble ( "Value" );
	CString strDateTime		= pDbPayments->GetString ( "PaymentDate" );

//	pLogRecord->SetServiceID ( strAccount );			// already set
//	pLogRecord->SetPrefix ( szText )					// done on saving log
	pLogRecord->SetPaymentID ( nPaymentID );
	pLogRecord->SetPupilID ( strPupilID );
	pLogRecord->SetPaidAmount ( dAmountPaid );	
	pLogRecord->SetPaidDate ( strDateTime );
	pLogRecord->SetPupilName ( strPupilName );			// clear the name incase no membership entry
//	pLogRecord->SetTransactionRef ( strTransRef );		// not used - no data
//	pLogRecord->SetReceiptCode ( const char* szText );	// not used - no data
//	pLogRecord->SetDOB ( const char* szText );			// not used - no data
//	pLogRecord->SetNotes ( const char* szText );		// not used - no data

	if ( m_WebPaymentList.FindRecord ( nPaymentID ) == FALSE )		// see if already processed
	{
		CSQLRepositoryAccount RepoAccount;

		bool bExists = FALSE;
		__int64 nUserID = RepoAccount.LookupMemberID(strPupilID, NULL).GetSQLResult();
		
		if ( nUserID != 0 )			// get number for matching pupilID
		{
			if ( dAmountPaid != 0.0 )
			{
				CSQLRowAccountFull RowAccount;
				RowAccount.SetUserID(nUserID);

				bool bExists = (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE);
				
				if ( TRUE == bExists )
				{
					//if ( ::IsAccountInuse ( strUserID ) == FALSE )
					{
						strPupilName = RowAccount.GetUsername();
						pLogRecord->SetPupilName ( strPupilName );

						CSQLAuditRecord atc;
						atc.Set (&RowAccount );						// set audit is for a pc line
						atc.SetOperatorID ( Passwords.GetPasswordInUse() );
						atc.SetApplicationNo ( APPNO_PARENTMAIL );
						atc.SetWebPaymentInfo ( nPaymentID );			// set PaymentID
//						atc.SetComment ( strNotes );					// set comment
	
						switch ( m_nAccountPurse )
						{
						case 3:		
							RowAccount.AddToPurse3Liability ( dAmountPaid );	
							break;
						
						/*
						case 2:		
							m_pDbSmartPay->AddToPurse2 ( dAmountPaid );	
							break;
						*/

						default:	
							RowAccount.AddToPurse1Liability ( dAmountPaid );	
							break;
						}

						RepoAccount.UpdateRow(RowAccount,NULL);
						
						if (atc.AuditChanges(&RowAccount) == TRUE)	// audit line
						{
							atc.SaveNewLine();								// save audit line
						}

						pLogRecord->LogValidImport ( m_strTempLogFile, RowAccount.GetUserIDString(), m_nValidCount );

						m_WebPaymentList.AddBlankRecord ( nPaymentID );						// add new record to webpayment list
//						m_WebPaymentList.SetServiceID ( pLogRecord->GetServiceID() );	// set new record fields
						m_WebPaymentList.SetAmount ( dAmountPaid );
						m_WebPaymentList.SetDate ( pLogRecord->GetDate() );
						m_WebPaymentList.SetTime ( pLogRecord->GetTime() );
						m_WebPaymentList.SetUserID ( RowAccount.GetUserIDString() );
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
					LogException ( pLogRecord, szMSG_NOACCOUNT );			// "No account record"
					bReply = FALSE;											// payment will not get deleted so will be added to retry exception file
				}
			}
			else
				LogException ( pLogRecord, szMSG_NOPAIDAMOUNT );		// "No paid amount" );
		}
		else
		{
			LogException ( pLogRecord, szMSG_NOMEMBER );				// "No member record" );
			bReply = FALSE;												// payment will not get deleted so will be added to retry exception file
		}
	}
	else
		pLogRecord->LogAlreadyProcessed ( m_strTempExceptionFile, m_WebPaymentList.GetDate(), m_WebPaymentList.GetTime(), m_nExceptionCount );  // "Already processed ( %s %s )"

	return bReply;													// TRUE = delete line \ FALSE - no delete 
}

//******************************************************************

void CParentMailImport::LogException ( CWebPaymentLogRecord* pLogRecord, const char* szErrorText ) 
{
	pLogRecord->LogException ( m_strTempExceptionFile, m_strExceptionIndicator, szErrorText, m_nExceptionCount );
}

//*******************************************************************
