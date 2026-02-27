//*******************************************************************
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\WaitDlg.h"
//*******************************************************************
#include "ParentPay.hpp"
#include "WebPaymentExportCsvLog.h"
#include "WebPaymentExportTnLog.h"
//********************************************************************
#include "ParentPayExport.h"
//********************************************************************

const char* szFINANCIALHEADER = "connectingPOSTxnID,mealSessionID,txnAmount,description,accountID,txnDateTime,txnType,originatingPOSID,flags,newBalance,posInternalTxnID,financialTransactionType,vatRate";

extern const char* szINTERNETMESSAGE;					// "Accessing WebPayment internet site"

#define	nMAXLOOP	500
#define nMAXRETRY	3

//*******************************************************************

CParentPayExport::CParentPayExport( CParentPayData* pData, CWnd* pParent )
{
	m_pData = pData;
	m_pParent = pParent;
	m_strError = "";
	m_strBalanceXmlDate = ""; //USED BY SEND BALANCES
	m_bIsRetryPass = FALSE;
	ResetTransaction();
}

//*******************************************************************

void CParentPayExport::ResetTransaction(const char* szAccountID)
{
	m_strAccountID = szAccountID;
	m_dBalance = 0;
	m_dSalesValue = 0.0;
	m_strDescription = "";
	m_nMealSession = 1;									// assume meal session1 (lunch )
}

//*******************************************************************
// Send transaction update data

bool CParentPayExport::SendTransactionData ( const char* szTransactionFile, bool bLogReqd )
{
	if ( bLogReqd == TRUE )
	{
		CWebPaymentExportTnLog log ( nWEBPAYMENT_PARENTPAY );		// ..\{WebProvider}}\Tnyyyymm.dat
		log.SaveLog ( szTransactionFile );							// log the updates
	}

	return SendTxData ( nPARENTPAY_TXFINANCIAL, szTransactionFile );
}

//*******************************************************************
// confirm receipt of payments by sending echo payment records

bool CParentPayExport::SendPaymentEcho ( const char* szPaymentEchoFile )
{
	CWebPaymentExportCsvLog log;									// PaymentEcho log
	log.SaveLog ( "Te", szPaymentEchoFile );

	return SendTxData ( nPARENTPAY_TXPAYMENTECHO, szPaymentEchoFile );
}

//*******************************************************************
// confirm diner data by sending diner match records

bool CParentPayExport::SendDinerMatch ( const char* szDinerMatchFile )
{
	CWebPaymentExportCsvLog log;									// account echo log
	log.SaveLog ( "Ta", szDinerMatchFile );

	return SendTxData ( nPARENTPAY_TXDINERMATCH, szDinerMatchFile );
}

//*******************************************************************
// Send Atc updates ( doesnt include ParentPay topups )
// if reply = FALSE = m_strError is set

bool CParentPayExport::SendAnyUpdates()
{
	if ( SendAnyRetries() == FALSE )												// send any retry jobs
		return FALSE;																// m_strError set if false

	CString strWebCsvFilename	= m_pData->GetUpdatesFilename();						// outstanding csv xml format updates
	CString strAtcUpdates		= Filenames.GetWebPaymentUpdatesFilename ( "dat" );		// ..\{web}\webupdates.dat

	if ( ::FileExists ( strAtcUpdates ) == TRUE )									// see if atc updates file exist
	{
		CString strTmpWebFile = Filenames.GetWebPaymentUpdatesFilename ( "dbs" );	// intermedite file for atc lines to web file format 

		CWebPaymentAtcTransactionFile webFile;										// internal format atc web update file
		if ( webFile.Open( strTmpWebFile ) == DB_ERR_NONE )							// open intermediate file \ create if not found
		{
			webFile.AddNewAtcUpdates ( strAtcUpdates );								// convert new atc audits ( if found ) & add to internal web atc format
			webFile.Close();														// then delete file if all ok

			CFileRecycler::SendToRecycleBin ( strAtcUpdates );

			CString strNewUpdatesFile = m_pData->GetNewUpdatesFilename();				// temp file like "updates.csv" to hold new updates

			if ( ConvertWebUpdates ( strTmpWebFile, strNewUpdatesFile, szFINANCIALHEADER ) == -1 )	// convert web updates to parentpay export csv xml file format
				return FALSE;

			if ( ::FileExists ( strNewUpdatesFile ) == TRUE )
			{
				if ( ::AppendDataFile ( strNewUpdatesFile, strWebCsvFilename, TRUE ) == TRUE )		// apppend files ( have header )
				{
					CWebPaymentExportTnLog log ( nWEBPAYMENT_PARENTPAY );							// ..\{WebProvider}}\Tnyyyymm.dat
					log.SaveLog ( strNewUpdatesFile );							// Log the new updates

					CFileRecycler::SendToRecycleBin ( strNewUpdatesFile );
				}
			}
			CFileRecycler::SendToRecycleBin ( strTmpWebFile );
		}
	}

	return SendTransactionData ( strWebCsvFilename, FALSE );					// NB: strWebCsvfile may not exist -no log reqd as already done
}

//******************************************************************************************
// Internal function - send various transaction data formats ( NB szDataFile may not exist )
//******************************************************************************************

bool CParentPayExport::SendTxData ( int nTxnDataType, const char* szDataFile )
{
	m_strError = "";

	if ( m_bIsRetryPass == FALSE )								// ensure not doing a retry pass
	{
		if ( SendAnyRetries() == FALSE )						// send any retry jobs
			return FALSE;										// failed retry still has a problem
	}

	if ( ::FileExists ( szDataFile ) == FALSE )					// see if have data file to send (may not exist)
		return TRUE;											// ok to contiue

	bool bReply = FALSE;										// assume error

	CWaitDlg dlgWait( m_pParent );
	dlgWait.SetMessageText( szINTERNETMESSAGE );

	CString strLoginReplyFile = System.GetWebPaymentPath ( "TxnReply.xml" );			// reply to send updates

	CSSParentPay pp ( m_pData );

	int nLoopCounter = 0;

	while ( TRUE && nLoopCounter < nMAXLOOP )					// just in case don't want infinate loop
	{
		++nLoopCounter;

		CString strError = "";

		switch ( nTxnDataType )
		{
		case nPARENTPAY_TXDINERMATCH:	strError = pp.SendDinerMatchAdvice ( szDataFile, strLoginReplyFile );	break;
		case nPARENTPAY_TXPAYMENTECHO:	strError = pp.SendPaymentEchoData  ( szDataFile, strLoginReplyFile );	break;
		case nPARENTPAY_TXFINANCIAL:	strError = pp.SendFinancialData    ( szDataFile, strLoginReplyFile );	break;
		}

		if ( strError != "" )										// check for any errors
		{
			if (pp.GetDataValidationError() != "")
			{
				m_strError.Format("Rx %s", 
					(const char*) pp.GetDataValidationError());		// "Rx {SendData ( errorStatus nnn ) validation error text ...}
			}
			else
			{
				m_strError.Format("Tx %s", 
					(const char*) strError);						// "Tx {SendData: errorStatus nnn) error text..}
			}

			AddToRetryQueue ( nTxnDataType, szDataFile );			// add to retry queue (if not retry pass)
			break;
		}

		int nTotalLineCount = GetFileLineCount ( szDataFile ) - 1;	// skip header
		if ( nTotalLineCount > 0 )									// total number of lines to send in file
		{
			int nTxCount = pp.GetTxCount();									// no errors reported - get number of lines actually sent from file

			CSSUnsortedDatabase dbTx;
			if ( dbTx.OpenSSDatabaseReadWrite ( szDataFile, "", 10 ) == DB_ERR_NONE )		// open csv xml file
			{
				if ( dbTx.GetRecordCount() > 0 )							// just incase - it should have at least 1 line
				{
					if ( dbTx.MoveTo ( nTxCount - 1 ) == TRUE )
					{
						__int64 n = dbTx.GetInt64 ( "connectingPOSTxnID" );
						m_pData->SetLastPosTxnID ( n );
						m_pData->Save();
					}
				}
				dbTx.Close();
			}

			if ( nTxCount < nTotalLineCount )						// see if all file sent
			{
				::RemoveFileLines ( szDataFile, nTxCount, TRUE );
				continue;											// not all date sent - so keep going
			}
		}

		CFileRecycler::SendToRecycleBin ( szDataFile );							// all data file sent - so delete file & exit
		bReply = TRUE;
		break;
	}

	if ( m_strError == "" && m_pData->m_bKeepXmlFiles == FALSE )
		CFileRecycler::SendToRecycleBin ( strLoginReplyFile );

	return bReply;
}

//*******************************************************************

int CParentPayExport::ConvertWebUpdates ( const char* szUpdatesFile, const char* szCsvFilename, const char* szHeader )
{
	CWebPaymentAtcTransactionFile webFile;

	if ( webFile.Open( szUpdatesFile ) != DB_ERR_NONE )
		return -1;

	int nCount = 0;

	CSSFile fileOut;
	if ( fileOut.Open ( szCsvFilename, "wb" ) == TRUE )
	{
		fileOut.WriteLine ( szFINANCIALHEADER );

		webFile.MoveFirst();

		do
		{
			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(_atoi64(webFile.GetCardNo()));

			CSQLRepositoryAccount RepoAccount;
			if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
			{
				CString strMemberID = RowAccount.GetMemberID();
				if (  ::IsStringNumeric ( strMemberID, 1 ) == TRUE )		// check ParentPayID exists & is numeric
				{
					ResetTransaction ( strMemberID );

					if ( IsValidSpend ( &webFile ) == TRUE )
					{
						if (SaveTransactionLine(&webFile, &fileOut) == TRUE)
						{
							++nCount;
						}
					}
					else if ( IsValidCashAdded ( &webFile ) == TRUE )
					{
						if ( SaveTransactionLine ( &webFile, &fileOut ) == TRUE )
							++nCount;
					}
				}
			}
		}
		while ( webFile.MoveNext() == TRUE );
	}

	fileOut.Close();
	
	if ( nCount > 0 )	m_pData->Save();							// save LastPosTxnID
	else				CFileRecycler::SendToRecycleBin ( szCsvFilename );		// delete empty web csv file

	return nCount;
}

//*******************************************************************
// for ParentPay adjustments - sales are negative

bool CParentPayExport::IsValidSpend ( CWebPaymentAtcTransactionFile* pFile )
{
	if ( pFile->GetActionType() != nATCWEB_SALE && pFile->GetActionType() != nATCWEB_SALEREFUND )
		return FALSE;

	m_nMealSession	 = 0;											// set not required
	m_strDescription = pFile->GetSalesText();						// set description

	if ( ( m_dSalesValue = pFile->GetPurse3Value() ) != 0 )			// see if purse3 used
	{
		m_nMealSession	= m_pData->GetMealSessionNo(3);				// session meals session number for purse3	
		m_dBalance		= pFile->GetPurse3Balance();
	}
	else if ( ( m_dSalesValue = pFile->GetPurse1Value() ) != 0 )	// see if purse1 used
	{
		m_nMealSession	= m_pData->GetMealSessionNo(1);				// session meals session number for purse1	
		m_dBalance		= pFile->GetPurse1Balance();
	}
	else if ( ( m_pData -> GetExportPurse2SalesFlag() == TRUE ) && ( pFile->GetPurse2Value() != 0 ) ) // see if purse2 used on its own
	{
		m_nMealSession	= m_pData->GetMealSessionNo(1);				// session meals session number for purse1	
		m_dBalance		= pFile->GetPurse1Balance();				// ongoing balance for purse 1
		m_dSalesValue	= 0.0;										// no transaction value for purse 1
	}

	return ( m_nMealSession == 0 ) ? FALSE : TRUE;
}

//*******************************************************************

bool CParentPayExport::IsValidCashAdded ( CWebPaymentAtcTransactionFile* pFile )
{
	if ( pFile->GetActionType() != nATCWEB_TOPUP && pFile->GetActionType() != nATCWEB_TOPUPREFUND && pFile->GetActionType() != nATCWEB_TRANSFER )
		return FALSE;

	m_nMealSession	 = 0;											// set not required)
	m_strDescription = pFile->GetSalesText();

	if ( ( m_dSalesValue = pFile->GetPurse1Value() ) != 0 )			// see if purse1 used
	{
		m_nMealSession	= m_pData->GetMealSessionNo(1);;			// meals session number for purse1	
		m_dBalance		= pFile->GetPurse1Balance();
	}
	
	else if ( ( m_dSalesValue = pFile->GetPurse3Value() ) != 0 )	// see if purse3 used
	{
		m_nMealSession	= m_pData->GetMealSessionNo(3);				// session meals session number for purse3	
		m_dBalance		= pFile->GetPurse3Balance();
	}

	return ( m_nMealSession == 0 ) ? FALSE : TRUE;
}

//*******************************************************************
// CsvFilename - no header

bool CParentPayExport::SaveTransactionLine ( CWebPaymentAtcTransactionFile* pFile, CSSFile* fp )
{
	CString strXmlDate = pFile->GetXmlDate ( nWEBPAYMENT_PARENTPAY );	// get audit date & time in xml format

	__int64 nPosTxnID = m_pData->GetNextPosTxnID ( TRUE );

	CCSV csv;
	csv.Add ( nPosTxnID );							// "connectingPOSTxnID"
	csv.Add ( m_nMealSession );						// "mealSessionID"
	csv.Add ( m_dSalesValue, System.GetDPValue() );	// "txnAmount"
	csv.Add ( m_strDescription );					// "description"
	csv.Add ( m_strAccountID );						// "accountID"
	csv.Add ( strXmlDate );							// "txnDateTime"
	csv.Add ( 0 );									// "txnType"
	csv.Add ( m_pData->m_strPosID );					// "originatingPOSID"
	csv.Add ( 10 );									// "flags", 2=flags in use, +8=till source balance
	csv.Add ( m_dBalance, System.GetDPValue() );		// "newBalance"
	csv.Add ( nPosTxnID );							// "posInternalTxnID"	
	csv.Add ( 6 );									// "financialTransactionType" -  6=adjustment
	csv.Add ( 0 );									// Vat Rate

	return fp->Write ( csv );
}

//************************************************************************

int CParentPayExport::CreateStartingBalancesFile ( const char* szListFilename, const char* szDestFilename )
{
	int nCount = -1;										// set error

	CSSFile fileList;
	if ( fileList.Open ( szListFilename, "rb" ) == TRUE )
	{
		CSSFile fileOut;
		if ( fileOut.Open ( szDestFilename, "wb" ) == TRUE )
		{
			nCount = 0;
			fileOut.WriteLine ( szFINANCIALHEADER );

			CString strUserID;
			while ( fileList.ReadString ( strUserID ) == TRUE )
			{
				CSQLRowAccountFull RowAccount;
				RowAccount.SetUserID(_atoi64(strUserID));

				CSQLRepositoryAccount RepoAccount;
				if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
				{
					m_strAccountID = RowAccount.GetMemberID();
					if (  ::IsStringNumeric ( m_strAccountID, 1 ) == TRUE )		// check ParentPayID exists & is numeric
					{
						if ( m_pData->m_bImportAccount1 == TRUE )
						{
							m_nMealSession = m_pData->m_nMealSession1;
							switch ( m_pData->m_nAccount1Purse )
							{
							case 1:	m_dBalance = RowAccount.GetPurse1Balance();	break;
							case 2:	m_dBalance = RowAccount.GetPurse2Balance();	break;
							case 3:	m_dBalance = RowAccount.GetPurse3Balance();	break;
							}
							
							if (SaveStartingBalanceLine(&fileOut) == TRUE)	// increments LastPosTxnID
							{
								++nCount;
							}
						}	

						if ( m_pData->m_bImportAccount2 == TRUE )
						{
							m_nMealSession = m_pData->m_nMealSession2;
							switch ( m_pData->m_nAccount2Purse )
							{
							case 1:	m_dBalance = RowAccount.GetPurse1Balance();	break;
							case 2:	m_dBalance = RowAccount.GetPurse2Balance();	break;
							case 3:	m_dBalance = RowAccount.GetPurse3Balance();	break;
							}
							
							if (SaveStartingBalanceLine(&fileOut) == TRUE)	// increments LastPosTxnID
							{
								++nCount;
							}
						}
					}
				}
			}
	
			fileOut.Close();
		}
		fileList.Close();
	}
	return nCount;
}

//*********************************************************************

bool CParentPayExport::SaveStartingBalanceLine ( CSSFile* fp )
{
	if (m_strBalanceXmlDate == "")
	{
		m_strBalanceXmlDate = ::FormatXmlDate(
			CSSDate::GetCurrentDate().GetDate(), 
			CSSTime::GetCurrentTime().GetTime());	
	}

	__int64 nPosTxnID = m_pData->GetNextPosTxnID ( TRUE );	// get next txn ID & increment

	CCSV csv;
	csv.Add ( nPosTxnID );							// "connectingPOSTxnID"
	csv.Add ( m_nMealSession );						// "mealSessionID"
	csv.Add ( "0.00" );								// "txnAmount"
	csv.Add ( "Starting balance" );					// "description"
	csv.Add ( m_strAccountID );						// "accountID"
	csv.Add ( m_strBalanceXmlDate );				// "txnDateTime"
	csv.Add ( 0 );									// "txnType"
	csv.Add ( m_pData->m_strPosID );					// "originatingPOSID"
	csv.Add ( 10 );									// "flags", 2=flags in use, +8=till source balance
	csv.Add ( m_dBalance, System.GetDPValue() );		// "newBalance"
	csv.Add ( nPosTxnID );							// "posInternalTxnID"	
	csv.Add ( 1 );									// "financialTransactionType" -  1=StartingBalance
	csv.Add ( 0 );									// Vat Rate

	return fp->Write ( csv );
}

//*******************************************************************

int CParentPayExport::CreateDinerMatchFile ( const char* szListFilename, const char* szDestFilename )
{
	int nCount = -1;										// set error

	CSSFile fileList;
	if ( fileList.Open ( szListFilename, "rb" ) == TRUE )
	{
		CSSFile fileOut;
		if ( fileOut.Open ( szDestFilename, "wb" ) == TRUE )
		{
			nCount = 0;
			fileOut.WriteLine ( m_pData->GetDinerMatchHeader() );				// write Diner Match header

			CString strUserID;
			while ( fileList.ReadString ( strUserID ) == TRUE )				// get UserID from listfile
			{
				CSQLRowAccountFull RowAccount;
				RowAccount.SetUserID(_atoi64(strUserID));

				CSQLRepositoryAccount RepoAccount;
				if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
				{
					CString strAccountID = RowAccount.GetMemberID();
					if (  ::IsStringNumeric ( strAccountID, 1 ) == TRUE )		// check ParentPayID exists & is numeric
					{
						
						CString strParentPayUPN = "";
						int nField = m_pData->GetUPNUserTextNo();

						if ( nField > 0 )								// see if UPN field exists
						{
							strParentPayUPN = RowAccount.GetInfo(nField);
						}
						
						CCSV csv;
						csv.Add ( m_pData->GetNextPosTxnID ( TRUE ) );		// get next txn ID & increment
						csv.Add ( strAccountID );							// m_strParentPayID );
						csv.Add ( strUserID );								// account no
						csv.Add ( strParentPayUPN );						// UPN
						csv.Add ( "0" );									// opening balance

						if ( fileOut.Write ( csv ) == TRUE )
							++nCount;
					}
				}
			}
	
			fileOut.Close();
		}
		fileList.Close();
	}

	return nCount;
}

//*********************************************************************

CString CParentPayExport::GetListPromptMessage ( const char* szText, int nCount )
{
	CString strMsg;		strMsg.Format ( "%d %s updates ready to send.\n\nDo you wish to view the data?", nCount, szText );		// 123 balances ready to send
	return strMsg;
}

//*******************************************************************
//*******************************************************************
//*******************************************************************

void CParentPayExport::AddToRetryQueue ( int nTxnDataType, const char* szDataFile )
{
	if ( m_bIsRetryPass == TRUE )								// don't retry a retry
		return;
		
	CString strFilename = Filenames.GetWebPaymentPath( ::GetUniqueTempFilename("$17"), nWEBPAYMENT_PARENTPAY );
	::rename ( szDataFile, strFilename );					// rename failed datafile to resend in retry pass

	CSSFile file;
	if ( file.Open ( m_pData->GetRetryQueueFilename(), "wb" ) == TRUE )
	{
		CCSV csv;
		csv.Add ( 0 );										// retry counter
		csv.Add ( nTxnDataType );							// file type
		csv.Add ( strFilename );							// filename to resend
		csv.Add ( m_strError );								// add error message
		file.Write ( csv );
		file.Close();
	}
}

//**************************************************************************

bool CParentPayExport::SendAnyRetries()
{
	if ( ::FileExists ( m_pData->GetRetryQueueFilename() ) == FALSE )
		return TRUE;

	bool bReply = FALSE;
	int nRetryCounter = 0;										// number of times this file has been retried
	bool bDeleteFile = FALSE;

	CSSFile file;
	CString strMsg;

	if ( ( bReply = file.Open ( m_pData->GetRetryQueueFilename(), "r+b" ) ) == TRUE )
	{
		CCSV csv;
		if ( ( bReply = file.Read ( csv ) ) == TRUE )				// read retry line
		{
			nRetryCounter = csv.GetInt(0);							// number of times this file has been retried
			if ( nRetryCounter <= nMAXRETRY )
			{
				m_bIsRetryPass = TRUE;
				if ( ( bReply = SendTxData ( csv.GetInt(1), csv.GetString(2) ) ) == TRUE )	// resend job
					bDeleteFile = TRUE;								// retry sent ok
				else
				{
					file.Seek ( 0L, SEEK_SET );						// no - back to start of file
					csv.SetAt ( 0, ++nRetryCounter );				// update retry counter for next time
					file.Write ( csv );
					strMsg.Format ( "Send retries incomplete (pass %d)", nRetryCounter );
				}

				m_bIsRetryPass = FALSE;
			}
			else
			{
				bDeleteFile = TRUE;									/// too many retries
				strMsg = "Too many retries";
			}
		}

		file.Close();
	}
	else
		strMsg = "Retry file open failed";

	if ( bDeleteFile == TRUE )
		CFileRecycler::SendToRecycleBin ( m_pData->GetRetryQueueFilename() );	// lose empty file

	if ( strMsg != "" )											// see if may an retry message
	{
		if ( m_strError != "" )									// see if already have an error
			m_strError += " - ";								// add to it

		m_strError += strMsg;									// add retry message
	}

	return bReply;
}

//*******************************************************************

/*
	if ( ::FileExists ( m_pData->GetRetryQueueFilename() ) == FALSE )
		return TRUE;

	bool bReply = TRUE;

	CFileStringArray retryQueue;
	retryQueue.Read ( m_pData->GetRetryQueueFilename() );
	int nCount = retryQueue.GetSize();

	m_bIsRetryPass = TRUE;

	for ( int n = 0 ; n < nCount ; n++ )
	{
		CCSV csv ( retryQueue.GetAt(0) );						// get first line

		int nRetryCounter = csv.GetInt(0);						// number of times this file has been retried
		if ( nRetryCounter <= nMAXRETRY )
		{
			if ( ( bReply = SendTxData ( csv.GetInt(1), csv.GetString(2) ) ) == FALSE )
			{
				csv.SetAt ( 0, ++nRetryCounter );				// update rery counter
				retryQueue.SetAt ( 0, csv.GetLine() );			// update try list
				break;
			}
		}
		retryQueue.RemoveAt(0);									// loose first line - all lines move up 1
	}

	m_bIsRetryPass = FALSE;

	nCount = retryQueue.GetSize();								// get new size of retry queue
	retryQueue.Write ( m_pData->GetRetryQueueFilename() );		// save whats left

	if ( nCount == 0 )											// see if empty queue
		::SendToRecycleBin ( m_pData->GetRetryQueueFilename() );	// lose empty file

	if ( bReply == FALSE )										// not all reties sent
	{
		if ( m_strError != "" )									// see if alreday have an error
			m_strError += " - ";								// add to it

		m_strError += "Send retries incomplete!";				// not all retries sent
	}

	return bReply;

*/