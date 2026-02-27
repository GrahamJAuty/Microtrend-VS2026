//********************************************************************
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\WaitDlg.h"
//********************************************************************
#include "ParentPay.hpp"
#include "ParentPayExport.h"
#include "WebPaymentDefines.h"
//********************************************************************
#include "ParentPayImport.h"
//********************************************************************

const char* szPAYMENTECHOHEADER = "connectingPOSTxnID,mealSessionID,txnAmount,description,accountID,txnDateTime,txnType,originatingPOSID,flags,newBalance,posInternalTxnID,parentPayPaymentID";

//*******************************************************************

CParentPayImport::CParentPayImport( CParentPayData* pData, CWnd* pParent )
{
	m_pData = pData;
	m_pParent = pParent;
	m_bNewPayments = TRUE;

	m_strTitle				= "ParentPay Web Payment Import";

	m_nExceptionCount		= 0;
	m_nValidCount			= 0;
	m_nHighestPaymentID		= 0;
	m_nAccountPurse			= 1;												// Purse1 or Purse3

	m_strLogFileName		= "";
	m_strTempLogFile		= "";
	m_strTempExceptionFile	= "";
	m_strExceptionName		= "";
	m_strExceptionIndicator = Filenames.GetWebPaymentExceptionIndicator();		// web payment exception indicator file

	m_strPaymentEchoFile	= m_pData->GetPaymentEchoFilename();				// payment echo file
}

//*******************************************************************

bool CParentPayImport::ImportPayments (  bool bNewPayments )
{
	bool bReply			= FALSE;
	m_bNewPayments		= bNewPayments;

	if (m_strError != "")											// szINTERFACENOTSET 
	{
		return FALSE;
	}

	if ( m_pData->ImportPayments() == FALSE )
	{
		if ( CModuleGlobals::IsBackgroundModule() == FALSE )							// started from menu option
		{
			m_strError = szMSG_IMPORTDISABLED;
			return FALSE;
		}
		return TRUE;
	}

// Service 1

	if ( m_pData->m_bImportAccount1 == TRUE )						// check for account1 import reqd
	{
		CString strFilename;
		int nServiceNo			= 1;							// check Account1 payments \ exceptions
		m_nHighestPaymentID		= m_pData->m_nLastAccount1;		// last payment reference no
		m_nAccountPurse			= m_pData->m_nAccount1Purse;		// pending purse to use

		if ( m_bNewPayments == TRUE )	strFilename = GetWebPayments ( nServiceNo );							// access internet & get new payments
		else							strFilename = m_pData->GetWebPaymentExceptionFilename ( nServiceNo );	// or process pending exceptions

		if ( ::FileExists(strFilename) == TRUE )
		{
			if ( m_strError == "" )		
			{
				bReply = ImportPayments ( nServiceNo, strFilename );
			}
			else						
			{
				DisplayMessage ( m_strError, MB_ICONSTOP );
				LogMessage( m_strError );
			}
		}
		else
		{
			CString strMsg = NoNewPaymentMessage(nServiceNo);
			DisplayMessage ( strMsg, MB_ICONINFORMATION );
			LogMessage( strMsg );
		}
	}

//Service 2

	if ( m_strError == "" && m_pData->m_bImportAccount2 == TRUE )	// check for account2 import reqd
	{
		CString strFilename;
		int nServiceNo			= 2;							// check Account2 payments \ exceptions
		m_nHighestPaymentID		= m_pData->m_nLastAccount2;		// last payment reference no
		m_nAccountPurse			= m_pData->m_nAccount2Purse;		// pending purse to use

		if ( m_bNewPayments == TRUE )	strFilename = GetWebPayments ( nServiceNo );		// access internet & return filename to import
		else							strFilename = m_pData->GetWebPaymentExceptionFilename ( nServiceNo );		// or process pending exceptions

		if ( ::FileExists(strFilename) == TRUE )
		{
			if ( m_strError == "" )		
			{
				bReply = ImportPayments ( nServiceNo, strFilename );
			}
			else						
			{
				DisplayMessage ( m_strError, MB_ICONSTOP );
				LogMessage( m_strError );
			}
		}
		else
		{
			CString strMsg = NoNewPaymentMessage(nServiceNo);
			DisplayMessage ( strMsg, MB_ICONINFORMATION );
			LogMessage( strMsg );
		}
	}

	return bReply;
}

//*******************************************************************

CString CParentPayImport::NoNewPaymentMessage ( int nServiceNo )
{
	CString strMsg;
	__int64 nAccountID = ( nServiceNo == 1 ) ? m_pData->m_nAccountID1 : m_pData->m_nAccountID2;	

	if (m_bNewPayments == TRUE)
	{
		strMsg.Format("No new payments have been received for Service %d : %I64d", nServiceNo, nAccountID);
	}
	else
	{
		strMsg.Format("No outstanding payments have been re-processed for Service %d : %I64d", nServiceNo, nAccountID);
	}

	return strMsg;
}

//*******************************************************************
// get web payments & return filename

CString CParentPayImport::GetWebPayments ( int nServiceNo )
{
//fiddle - for testing only
//CString strDestinationFilename = System.GetWebPaymentPath ( "PayAcc-20140509-110413.dat" );
//return strDestinationFilename;

	m_strError = "";
	CString strPaymentsFile = "";		// empty = if not valid AccountNo

	__int64 nAccountID = ( nServiceNo == 1 ) ? m_pData->m_nAccountID1 : m_pData->m_nAccountID2;	

	if ( nAccountID != 0 )
	{
		CString strAccountID = ::FormatInt64Value( nAccountID ); 

		CString strLoginReply	= Filenames.GetWebPaymentImportFilename ( "xml", strAccountID );
		strPaymentsFile			= Filenames.GetWebPaymentImportFilename ( "dat", strAccountID );
	
// access internet and get new payments

		CWaitDlg dlgWait ( m_pParent );
		dlgWait.SetMessageText ( szINTERNETMESSAGE );

		CSSParentPay pp ( m_pData );
		m_strError = pp.GetPayments ( nServiceNo, strLoginReply, strPaymentsFile );
	}

	return strPaymentsFile;
}

//*******************************************************************

bool CParentPayImport::ImportPayments ( int nServiceNo, const char* szImportFilename )
{
	__int64 nAccountID = ( nServiceNo == 1 ) ? m_pData->m_nAccountID1   : m_pData->m_nAccountID2;	
	int nMealSession   = ( nServiceNo == 1 ) ? m_pData->m_nMealSession1 : m_pData->m_nMealSession2;

	CWebPaymentLogRecord LogRecord;
	LogRecord.SetServiceID ( nAccountID );

	CSSUnsortedDatabase dbPayments;
	if ( dbPayments.OpenSSDatabaseReadWrite ( szImportFilename, "", 10 ) != DB_ERR_NONE )	// open intermediate csv importfile
	{
		CString strMsg;
		strMsg.Format ( "Error in opening Import file ' %s ' for Read/Write", szImportFilename );
		return FALSE;
	}

// ** create filenames & temp filenames

	m_strLogFileName	= LogRecord.GetLogFileName();			// "InYYYYMM"
	m_strExceptionName	= LogRecord.GetExceptionFileName();		// "ExYYYYMM"

	m_strTempLogFile.Format ( "%s.$$$", 
		(const char*) m_strLogFileName );

	m_strTempLogFile = System.GetWebPaymentPath ( m_strTempLogFile );

	m_strTempExceptionFile.Format ( "%s.$$$", 
		(const char*) m_strExceptionName );
	
	m_strTempExceptionFile = System.GetWebPaymentPath ( m_strTempExceptionFile );

//** Open WebPayment List file  **************************************

	CString strListFilename = m_pData->GetWebPaymentListFilename ( nServiceNo );

	if ( m_WebPaymentList.Open ( strListFilename, DB_READWRITE ) != DB_ERR_NONE )	// ret 0=ok, 1=file error, 2=file in use
	{
		m_strError.Format ( "Error in opening the ParentPay Payment List file ' %s ' for Read/Write",
			(const char*) strListFilename );
		
		return FALSE;
	}

//** import lines

	m_nValidCount		= 0;
	m_nExceptionCount	= 0;
	int nRecordCount	= dbPayments.GetRecordCount();

	if (nRecordCount > 0)										// is empty if no new payments
	{
		dbPayments.MoveFirst();

		for (int i = 0; i < nRecordCount; i++)
		{
			if (SavePendingUpdate(&dbPayments, &LogRecord, nMealSession) == TRUE)
			{
				dbPayments.DeleteRecord();	// Delete line from payments file
			}
			else
			{
				dbPayments.MoveNext();		// no delete - get next line
			}
		}

		if (m_bNewPayments == TRUE)
		{
			EchoNewPayments();								// send back new echo payments to Parentpay
		}
	}

//** see if any data has been imported

	DisplayImportResults( nServiceNo, nRecordCount );

	if ( m_bNewPayments == TRUE )
	{
		if ( m_nValidCount > 0 || m_nExceptionCount > 0 )
		{
			if (nServiceNo == 1)
			{
				m_pData->m_nLastAccount1 = m_nHighestPaymentID;	// Highest PaymentID may have changed
			}
			else
			{
				m_pData->m_nLastAccount2 = m_nHighestPaymentID;
			}
			
			m_pData->Save();															// save for next time
		}
	}

// close/save processed listing

	m_WebPaymentList.Close();									// close file

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
			CString strExceptions = m_pData->GetWebPaymentExceptionFilename ( nServiceNo );
			::AppendDataFile ( szImportFilename, strExceptions, TRUE );	// append new exceptions to old exceptions ( skip header line )
			CFileRecycler::SendToRecycleBin ( szImportFilename );					// delete whats left of new imports file

			DisplayMessage ( szMSG_EXCEPTIONSAVED, MB_ICONINFORMATION );
			LogMessage( szMSG_EXCEPTIONSAVED );
		}
	}
	
// log exceptions

	if ( m_bNewPayments == TRUE && m_nExceptionCount > 0 )
	{
		CString strFile;
		strFile.Format ( "%s.dat", 
			(const char*) m_strExceptionName );

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

// Header = "PaymentID,accountID,rollNumber,UPN,MISID,txnAmount,PayerInfo,PaidDate,ReceiptCode,paymentTypeCode,ConsumerName";

bool CParentPayImport::SavePendingUpdate(CSSUnsortedDatabase* pDbPayments, CWebPaymentLogRecord* pLogRecord, int nMealSession)
{
	__int64 nPaymentID = pDbPayments->GetInt64("PaymentID");

	if (m_bNewPayments == TRUE)							// see if importing new payments
	{
		if (nPaymentID > m_nHighestPaymentID)				// check if have new highest PaymentID 
		{
			m_nHighestPaymentID = nPaymentID;				// save it 
		}
	}

	//  Old ParentPay format
	//	double dPaidAmount		= pDbPayments->GetDouble("Amount");
	//	CString strReceiptCode	= pDbPayments->GetString("ReceiptCode");
	//	CString strNotes		= pDbPayments->GetString("PaymentType");
	//	CString strDetails		= pDbPayments->GetString("PayerInfo");
	//	CString strDateTime		= pDbPayments->GetString("PaidDate");
	//	CString strPupilID		= pDbPayments->GetString("Identifier");
	//	CString strPupilName	= pDbPayments->GetString("ConsumerName");
	//	CString strDob			= pDbPayments->GetString("DoB");
	//	dPaidAmount /= 100;

	double dAmountPaid = pDbPayments->GetDouble("txnAmount");
	CString strReceiptCode = pDbPayments->GetString("ReceiptCode");
	CString strDateTime = pDbPayments->GetString("PaidDate");
	CString strPupilID = pDbPayments->GetString("accountID");
	CString strDetails = pDbPayments->GetString("paymentTypeCode");
	CString strNotes = pDbPayments->GetString("PayerInfo");		// can be "Till software upgrade – please ignore"
	CString strPupilName = pDbPayments->GetString("ConsumerName");	// legacy data - may not be returned

	//	pLogRecord->SetPrefix ( szText )					// done on saving log
	pLogRecord->SetPaymentID(nPaymentID);
	pLogRecord->SetPaidAmount(dAmountPaid);
	pLogRecord->SetReceiptCode(strReceiptCode);
	pLogRecord->SetPaidDate(strDateTime);
	pLogRecord->SetPupilID(strPupilID);
	pLogRecord->SetPupilName(strPupilName);			// clear the name incase no membership entry
	pLogRecord->SetTransactionRef(strDetails);
	pLogRecord->SetNotes(strNotes);
	//	pLogRecord->SetDOB ( const char* szText );			// not used - no data


	//CHECK THAT THIS PAYMENT LINE HAS NOT ALREADY BEEN PROCESSED
	if (m_WebPaymentList.FindRecord(nPaymentID) == TRUE)
	{
		pLogRecord->LogAlreadyProcessed(m_strTempExceptionFile, m_WebPaymentList.GetDate(), m_WebPaymentList.GetTime(), m_nExceptionCount);  // "Already processed ( %s %s )"
		return TRUE;
	}

	//CHECK THAT WE HAVE AN ACCOUNT ID FOR THE MEMBER ID
	CSQLRowAccountFull RowAccount;
	CSQLRepositoryAccount RepoAccount;
	__int64 nUserID = RepoAccount.LookupMemberID(strPupilID, NULL).GetSQLResult();
	if (0 == nUserID)
	{
		LogException(pLogRecord, szMSG_NOMEMBER);

		if (TRUE == m_bNewPayments)
		{
			SaveEchoPaymentZeroBalance(pDbPayments, nMealSession);			
		}

		return CanIgnorePayment(strNotes);
	}

	//CHECK THAT ACCOUNT EXISTS
	RowAccount.SetUserID(nUserID);
	if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		LogException(pLogRecord, szMSG_NOACCOUNT);			
		
		if (TRUE == m_bNewPayments)
		{
			SaveEchoPaymentZeroBalance(pDbPayments, nMealSession);		
		}

		return CanIgnorePayment(strNotes);
	}

	bool bCanProceed = FALSE;
	bool bZeroPayment = (CompareDoubles(dAmountPaid, 0.0, 3) == 0);
	CString strUserID = RowAccount.GetUserIDString();

	while (FALSE == bCanProceed)
	{
		CSQLAuditRecord atc;
		atc.Set(&RowAccount);						// set audit is for a pc line
		atc.SetOperatorID(Passwords.GetPasswordInUse());
		atc.SetApplicationNo(APPNO_PARENTPAY);
		atc.SetWebPaymentInfo(nPaymentID);			// set PaymentID
		atc.SetLineComment(strNotes);					// set comment

		bCanProceed = TRUE;

		if (FALSE == bZeroPayment)
		{
			switch (m_nAccountPurse)
			{
			case 3:
				RowAccount.AddToPurse3Liability(dAmountPaid);
				break;

				/*
				case 2:
					RowAccount.AddToPurse2Balance ( dAmountPaid );
					break;
				*/

			default:
				RowAccount.AddToPurse1Liability(dAmountPaid);
				break;
			}

			CSQLRowAccountFull RowCheck;
			RowCheck.SetUserID(RowAccount.GetUserID());
			if (RepoAccount.SelectRow(RowCheck, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				bCanProceed = RowAccount.CompareRowVersion(RowCheck);
			}
		}

		if (TRUE == bCanProceed)
		{
			if (FALSE == bZeroPayment)
			{
				RepoAccount.UpsertRow(RowAccount, NULL);
			}

			if (atc.AuditChanges(&RowAccount,TRUE) == TRUE)	// audit line
			{
				atc.SaveNewLine();								// save audit line
			}

			pLogRecord->LogValidImport(m_strTempLogFile, strUserID, m_nValidCount);	// add to import log

			m_WebPaymentList.AddBlankRecord(nPaymentID);			// add new record to webpaymnet list
			m_WebPaymentList.SetServiceID(pLogRecord->GetServiceID());	// set new record fields
			m_WebPaymentList.SetAmount(dAmountPaid);
			m_WebPaymentList.SetDate(pLogRecord->GetDate());
			m_WebPaymentList.SetTime(pLogRecord->GetTime());
			m_WebPaymentList.SetUserID(strUserID);
			m_WebPaymentList.SetName(strPupilName);
		}
	}

	if (TRUE == m_bNewPayments)
	{
		SaveEchoPaymentRealBalance(pDbPayments, nMealSession, RowAccount);	
	}

	return TRUE;
}

//*******************************************************************

// false = payment will not get deleted so will be added to retry exception file
// true  = payment will be deleted so will not be saved as exception
bool CParentPayImport::CanIgnorePayment ( CString strPayerInfo )
{
	strPayerInfo.MakeUpper();												
	return (strPayerInfo.Find("PLEASE IGNORE") != -1);
}

//*******************************************************************
// echo payments back to ParentPay ( file may not exist )

void CParentPayImport::EchoNewPayments()
{
	if ( ::FileExists ( m_strPaymentEchoFile ) == TRUE )		//  see if any lines to echo
	{
		CParentPayExport echo ( m_pData, m_pParent );
		echo.SendPaymentEcho ( m_strPaymentEchoFile );

		m_strError = echo.GetError();
	}
}

//*******************************************************************
// save web csv echo payment line
// on entry m_pDbSmartPay already pointing to account as payment has been added to Purse1

void CParentPayImport::SaveEchoPaymentZeroBalance(CSSUnsortedDatabase* pDbPayments, int nMealSession)
{
	SaveEchoPaymentInternal(pDbPayments, nMealSession, 0.0);
}

//*******************************************************************

void CParentPayImport::SaveEchoPaymentRealBalance(CSSUnsortedDatabase* pDbPayments, int nMealSession, CSQLRowAccountFull& RowAccount)
{
	double dBalance = 0.0;

	switch (m_nAccountPurse)
	{
	case 3:
		dBalance = RowAccount.GetPurse3Balance();
		break;

		/*
		case 2:
			dBalance = RowAccount.GetPurse2Balance();
			break;
		*/

	case 1:
	default:
		dBalance = RowAccount.GetPurse1Balance();
		break;
	}

	SaveEchoPaymentInternal(pDbPayments, nMealSession, dBalance);
}

//*******************************************************************

void CParentPayImport::SaveEchoPaymentInternal(CSSUnsortedDatabase* pDbPayments, int nMealSession, double dBalance)
{
	__int64 nPaymentID = pDbPayments->GetInt64("PaymentID");
	double dAmountPaid = pDbPayments->GetDouble("txnAmount");
	CString strPupilID = pDbPayments->GetString("accountID");

	CString strDate = ::FormatXmlDate(CSSDate::GetCurrentDate().GetDate(), CSSTime::GetCurrentTime().GetTime());

	__int64 nPosTxnID = m_pData->GetNextPosTxnID(TRUE);	// get next txn ID & increment

	CCSV csv;
	csv.Add(nPosTxnID);								// "connectingPOSTxnID"
	csv.Add(nMealSession);							// "mealSessionID"
	csv.Add(dAmountPaid, 2);							// "txnAmount"
	csv.Add("Deposit received");						// "description"
	csv.Add(strPupilID);								// "accountID";
	csv.Add(strDate);								// "txnDateTime"
	csv.Add(0);										// "txnType"
	csv.Add(m_pData->m_strPosID);						// "originatingPOSID"
	csv.Add(10);										// "flags", 2=flags in use, +8=till source balance
	csv.Add(dBalance, 2);							// "newBalance"
	csv.Add(nPosTxnID);								// "posInternalTxnID"
	csv.Add(nPaymentID);								// "parentPayPaymentID", "128389851" );

	{
		CSSFile file;
		if (::FileExists(m_strPaymentEchoFile) == FALSE)
		{
			if (file.Open(m_strPaymentEchoFile, "wb") == TRUE)
			{
				file.WriteLine(szPAYMENTECHOHEADER);		// add file header
				file.Write(csv);
				file.Close();
			}
		}
		else
		{
			if (file.Open(m_strPaymentEchoFile, "ab") == TRUE)
			{
				file.Write(csv);
				file.Close();
			}
		}
	}
}

//******************************************************************

void CParentPayImport::LogException ( CWebPaymentLogRecord* pLogRecord, const char* szErrorText ) 
{
	pLogRecord->LogException ( m_strTempExceptionFile, m_strExceptionIndicator, szErrorText, m_nExceptionCount );
}

//*******************************************************************

