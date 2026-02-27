//******************************************************************************
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\WaitDlg.h"
//******************************************************************************
#include "ParentMail.hpp"
#include "WebPaymentDefines.h"
#include "WebPaymentExportTnLog.h"
//******************************************************************************
#include "ParentMailExport.h"
//******************************************************************************
static int nTRANSIDM_INDEX = 0;
static const char* szTransHeader1 = "TransID,AccountID,SalesValue,FreeValue,TransDateTime,TransType,EPosID,ProductID,PurseType,VatRate,Balance,ReceiptNumber";
static const char* szTransHeader2 = "int,int,decimal,decimal,dateTime,int,string,int,decimal,decimal,decimal,int";
static const char* szLINKEDHEADER = "ProviderID,AccountID";
//******************************************************************************

CParentMailExport::CParentMailExport( CWnd* pParent )
{
	m_pParent = pParent;
	m_nFailedCounter = 0;
	m_nUpdatesCount = 0;
	m_strError = "";
	m_bEODSent = FALSE;

	m_data.Read();
	m_webOptions.LoadRow(NULL);

	ResetTransaction();

	m_strTempLinkedFile		= Filenames.GetTempReportPath ( "R221A", "dat" );
	m_strTempExceptionFile	= Filenames.GetTempReportPath ( "R221B", "dat" );
}

//*******************************************************************

void CParentMailExport::ResetTransaction ( const char* szAccountID )
{
	m_strAccountID	= szAccountID;
	m_dSalesValue	= 0.0;
	m_dFreeValue	= 0.0;
	m_nTransType	= 0;
	m_strEposID		= "";
	m_nProductID	= 0;
}

//*******************************************************************
// if reply = FALSE = m_strError is set

bool CParentMailExport::SendUpdates ( bool bPostData, bool bEODAllowed )
{
	bool bEODReqd = FALSE;															// if set log end balances  sent - so stop it doing until next day
	CString strWebCsvFilename	= m_data.GetUpdatesFilename();						// outstanding csv xml format updates
	CString strNewUpdatesFile	= m_data.GetNewUpdatesFilename();					// temp file like "updates.csv" to hold new updates ( NO HEADER LINES )
	CString strAtcUpdates		= Filenames.GetWebPaymentUpdatesFilename ( "dat" );	// ..\{web}\webupdates.dat

	if ( ::FileExists ( strAtcUpdates ) == TRUE )									// see if atc updates file exist
	{
		CString strTmpWebFile = Filenames.GetWebPaymentUpdatesFilename ( "dbs" );	// intermedite file for atc lines to web file format 

		CWebPaymentAtcTransactionFile webFile;										// internal format atc web update file
		if ( webFile.Open( strTmpWebFile ) == DB_ERR_NONE )							// open intermediate file \ create if not found
		{
			webFile.AddNewAtcUpdates ( strAtcUpdates );								// convert new atc audits ( if found ) & add to internal web atc format
			webFile.Close();														// then delete file if all ok

			CFileRecycler::SendToRecycleBin ( strAtcUpdates );

			int nReply = ConvertWebUpdates ( strTmpWebFile, strNewUpdatesFile );	// convert web updates to parentmail export csv xml file format

			CFileRecycler::SendToRecycleBin ( strTmpWebFile );

			if (nReply == -1)
			{
				return FALSE;
			}
		}
	}

	if ( m_webOptions.m_Row.GetEODAfterPaymentFlag() == TRUE)									// see if allowed to send EOD after payments
	{
		if ( m_webOptions.m_Row.TimeForEndOfDay() == TRUE )								// is it time for eod )
		{	
			if ( bEODAllowed == TRUE && bPostData == TRUE )							// it it allowed with current send request
			{
				AddEndOfDayTransaction ( strNewUpdatesFile );						// yes - add an end of day transaction record 
				bEODReqd = TRUE;													// log eod for today
			}
		}
	}

	if ( ::FileExists ( strNewUpdatesFile ) == TRUE )								// see if any new updates to process
	{
		if ( AppendUpdates ( strNewUpdatesFile, strWebCsvFilename ) == TRUE )		// NOTE: adds multiple 2 line header
		{
			CWebPaymentExportTnLog log ( nWEBPAYMENT_PARENTMAIL );					// ..\{WebProvider}}\Tnyyyymm.dat
			log.SaveLog ( strNewUpdatesFile, szTransHeader1 );						// Log the new updates

			CFileRecycler::SendToRecycleBin ( strNewUpdatesFile );
		}
	}

	bool bReply = TRUE;
	if ( bPostData == TRUE )
	{
		if ( ( bReply = ::FileExists ( strWebCsvFilename ) ) == TRUE )				// check for any updatse to send
		{
			if ( ( bReply = ExportUpdatesFile ( strWebCsvFilename ) ) == TRUE )
			{
				CFileRecycler::SendToRecycleBin ( strWebCsvFilename );

				if ( bEODReqd == TRUE )												// see if EOD was added to sent transation
					m_bEODSent = TRUE;
			}
		}
	}

	return bReply;
}

//*******************************************************************

bool CParentMailExport::ExportUpdatesFile ( const char* szUpdatesFile )
{
	m_strError = "";

	CString strLoginReplyFile = System.GetWebPaymentPath ( "TuReply.xml", nWEBPAYMENT_PARENTMAIL );			// reply to send updates

	CWaitDlg dlgWait;
	dlgWait.SetMessageText( szINTERNETMESSAGE );

	bool bReply = FALSE;

	CSSParentMail pm ( &m_data );

	while ( TRUE )
	{
		if ( ( m_strError = pm.GetLastProcessedSnapShotID ( strLoginReplyFile ) ) != "" )
			break;

		if ( RemovedProcessedUpdates ( szUpdatesFile, pm.LastSnapshotID() ) == TRUE )	// sets txUpdate Count with lines left to send
		{
			if ( ( m_strError = pm.SendUpdates ( szUpdatesFile, strLoginReplyFile ) ) != "" )
				break;

			if ( m_nUpdatesCount - pm.GetTxCount() <= 0  )
			{
				bReply = TRUE;
				break;
			}
		}
	}

	if ( m_data.m_bKeepXmlLogin == FALSE )
		CFileRecycler::SendToRecycleBin ( strLoginReplyFile );

	return bReply;
}

//*******************************************************************
// NOTE: output file has NO header lines

int CParentMailExport::ConvertWebUpdates ( const char* szUpdatesFile, const char* szCsvFilename )
{
	CWebPaymentAtcTransactionFile webFile;

	if ( webFile.Open( szUpdatesFile ) != DB_ERR_NONE )
		return -1;

	int nCount = 0;

	CSSFile fileOut;
	if ( fileOut.Open ( szCsvFilename, "wb" ) == TRUE )
	{
		webFile.MoveFirst();

		do
		{
			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserIDString(webFile.GetCardNo());

			CSQLRepositoryAccount RepoAccount;
			if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
			{
				CString strMemberID = RowAccount.GetMemberID();
				if (  ::IsStringNumeric ( strMemberID, 1 ) == TRUE )		// check ParentMailID exists & is numeric
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
						if (SaveTransactionLine(&webFile, &fileOut) == TRUE)
						{
							++nCount;
						}
					}
				}
			}
		}
		while ( webFile.MoveNext() == TRUE );
	}

	fileOut.Close();
	
	if ( nCount > 0 )	m_data.Save();								// save LastP{osTxnID
	else				CFileRecycler::SendToRecycleBin ( szCsvFilename );		// delete empty web csv file

	return nCount;
}

//*******************************************************************

bool CParentMailExport::IsValidSpend (  CWebPaymentAtcTransactionFile* pFile )
{
	if ( pFile->GetActionType() != nATCWEB_SALE && pFile->GetActionType() != nATCWEB_SALEREFUND )
		return FALSE;

	double dPurse	 = 0.0;
	double dFSMPurse = 0.0;
	double dBalance	 = 0.0;

	switch ( m_data.m_nAccountPurse )
	{
	case 3:		dPurse		= pFile->GetPurse3Value();
				dFSMPurse	= pFile->GetPurse2Value();
				dBalance	= pFile->GetPurse3Balance();
				break;

	case 2:		dPurse		= pFile->GetPurse2Value();			// if used - no FSM
				dBalance	= pFile->GetPurse2Balance();
				break;

	default:	dPurse		= pFile->GetPurse1Value();
				dFSMPurse	= pFile->GetPurse2Value();
				dBalance	= pFile->GetPurse1Balance();
				break;
	}

	bool bReply = FALSE;

	switch ( pFile->GetSourceType() )
	{
	case AUDIT_POS:		
		m_strEposID = "ecr";
		bReply = TRUE;
		break;

	case AUDIT_EXT:
		m_strEposID = "ext";
		bReply = TRUE;
		break;

	default:
		break;
	}

	if ( bReply == TRUE )
	{
		m_nTransType	= 1;								// Sale
		m_nProductID	= 9999;								// dummy Product
		m_dBalance		= dBalance;							// current Purse balance
		m_dFreeValue	= dFSMPurse;						// FSM sales values
		m_dSalesValue	= dPurse + dFSMPurse;				// Total sales value

		if ( m_dSalesValue != 0 )							// sales are negativee
			m_dSalesValue = -(m_dSalesValue);				// make positive

		if ( m_dFreeValue != 0 )
			m_dFreeValue = -( m_dFreeValue );				// make positive
	}

	return bReply;
}

//*******************************************************************

bool CParentMailExport::IsValidCashAdded (  CWebPaymentAtcTransactionFile* pFile )
{
	if ( pFile->GetActionType() != nATCWEB_TOPUP && pFile->GetActionType() != nATCWEB_TOPUPREFUND && pFile->GetActionType() != nATCWEB_STARTINGBALANCE && pFile->GetActionType() != nATCWEB_TRANSFER )
		return FALSE;

	double dBalance		= 0.0;
	double dCashAdded	= 0.0;

	switch ( m_data.m_nAccountPurse )
	{
	case 3:		dCashAdded	= pFile->GetPurse3Value();
				dBalance	= pFile->GetPurse3Balance();
				break;

	case 2:		dCashAdded	= pFile->GetPurse2Value();
				dBalance	= pFile->GetPurse2Balance();
				break;

	default:	dCashAdded	= pFile->GetPurse1Value();
				dBalance	= pFile->GetPurse1Balance();
				break;
	}

	m_nTransType = 2;										// default to till side adjustment
	bool bReply = FALSE;									// assume not use line

	int nAppNo = pFile->GetApplicationNo();
	
	switch ( pFile->GetSourceType() )
	{
	case AUDIT_SERVER:		if ( nAppNo == APPNO_REDEEM )
							{
								m_strEposID = "srv";
								bReply = TRUE;
							}
							break;

	case AUDIT_PC:			switch ( nAppNo )
							{
							case APPNO_ADDCASH:
							case APPNO_BATCHUPDATE:	
							case APPNO_BATCHDELETE:						// <== is this required ?????
							case APPNO_IMPORTTOPUP:
							case APPNO_REVALUEBONUSCASH:
							case APPNO_REWARDBONUSCASH:
							case APPNO_REFUNDREWARDBONUSCASH:
							case APPNO_XFEROUT:							//14/07/2016 - v1.12f
							case APPNO_XFERIN:
							case APPNO_HOTXFEROUT:
							case APPNO_HOTXFERIN:
							case APPNO_REDEEM:
							case APPNO_BATCHREDEEM:
								m_strEposID = "pc";
								bReply = TRUE;
								break;
	
//							case APPNO_PUFPARENTPAY:
//							case APPNO_PUFTUCASI:
//							case APPNO_PUFTUCASIv2:
//							case APPNO_PUFWISEPAY:
//							case APPNO_SCHOOLCOMMS:
//							case APPNO_SQUID:
							case APPNO_PARENTMAIL:
									m_strEposID = "pc";
									m_nTransType = 3;		// +Pay adjustment
									bReply = TRUE;
									break;

							case APPNO_WPOPENINGBALANCE:
									m_strEposID = "pc";
									m_nTransType = 5;		// Opening Balance
									dCashAdded = dBalance;	// set sales value to opening balance
									bReply = TRUE;
									break;
							}
							break;


	case AUDIT_KIOSKLINK:
		if (nAppNo == APPNO_ADDCASH)
		{
			m_strEposID = "ksk";
			bReply = TRUE;
		}
		break;

	case AUDIT_POS:		
		if (nAppNo == APPNO_ADDCASH)
		{
			m_strEposID = "ecr";
			bReply = TRUE;
		}
		break;

	case AUDIT_EXT:				
		if (nAppNo == APPNO_ADDCASH)
		{
			m_strEposID = "ext";
			bReply = TRUE;
		}
		break;
	}

	if ( bReply == TRUE )
	{
		m_dFreeValue	= 0;
		m_dSalesValue	= dCashAdded;
		m_dBalance		= dBalance;
	}
	return bReply;
}

//*******************************************************************
// CsvFilename - no header

bool CParentMailExport::SaveTransactionLine ( CWebPaymentAtcTransactionFile* pFile, CSSFile* fp )
{
	CString strXmlDate = pFile->GetXmlDate ( nWEBPAYMENT_PARENTMAIL );	// get audit date & time in xml format

	__int64 nTransactionID = m_data.GetNextTransactionID();				// get next transaction ID to use

	CCSV csv;
	csv.Add ( nTransactionID );
	csv.Add ( m_strAccountID );
	csv.Add ( m_dSalesValue, System.GetDPValue() );
	csv.Add ( m_dFreeValue, System.GetDPValue() );
	csv.Add ( strXmlDate );
	csv.Add ( m_nTransType );					// Transaction type
	csv.Add ( m_strEposID );
	csv.Add ( m_nProductID );					// dummy product ID
	csv.Add ( 1 );								// Purse Type 1=lunch, 2=morning tuck, 3 =after school tuck
	csv.Add ( 0 );								// Vat Rate
	csv.Add ( m_dBalance, System.GetDPValue() );
	csv.Add ( nTransactionID );

	return fp->Write ( csv );
}

//********************************************************************
// CsvFilename - no header

void CParentMailExport::AddEndOfDayTransaction ( const char* szCsvFilename )
{
	CSSDate date;	date.SetCurrentDate();
	CSSTime time;	time.SetCurrentTime();

	CString strXmlDate = ::FormatXmlDate ( &date, &time );

	__int64 nTransactionID = m_data.GetNextTransactionID();			// get next transaction ID to use

	CCSV csv;
	csv.Add ( nTransactionID );
	csv.Add ( 0 );							// AccountID
	csv.Add ( 0 );							// SalesValue
	csv.Add ( 0 );							// FreeValue
	csv.Add ( strXmlDate );					// date
	csv.Add ( 4 );							// Transaction type EndOfDay
	csv.Add ( "eod" );						// Type
	csv.Add ( 0 );							// dummy product ID
	csv.Add ( 0 );							// Purse Type 1=lunch, 2=morning tuck, 3 =after school tuck
	csv.Add ( 0 );							// Vat Rate
	csv.Add ( 0 );							// Balance
	csv.Add ( nTransactionID );

	CSSFile fileOut;
	if ( fileOut.Open ( szCsvFilename, "ab" ) == TRUE )
	{
		fileOut.Write ( csv );
		fileOut.Close();

		m_data.Save();						// save new highest TransactionID so far
	}
}

//**********************************************************************

bool CParentMailExport::AppendUpdates ( const char* szWebUpdatesFile, const char* szUpdatesFile )
{
	if ( ::GetFileLineCount ( szUpdatesFile ) < 2 )				// file must have 2 header lines
	{
		CSSFile fileOut;
		if ( fileOut.Open ( szUpdatesFile, "wb" ) == FALSE )
			return FALSE;

		fileOut.WriteLine ( szTransHeader1 );
		fileOut.WriteLine ( szTransHeader2 );
		fileOut.Close();
	}

	return ::AppendDataFile ( szWebUpdatesFile, szUpdatesFile ); 
}

//********************************************************************

bool CParentMailExport::RemovedProcessedUpdates ( const char* szUpdateFilename, __int64 nLastProcessedID )
{
	CSSFile fileIn;
	if ( fileIn.Open ( szUpdateFilename, "rb" ) == FALSE )
		return FALSE;

	bool bReply = FALSE;
	m_nUpdatesCount = 0;

	CSSFile fileOut;
	CString strTempFilename = System.GetWebPaymentPath ( ::GetUniqueTempFilename("$09"), nWEBPAYMENT_PARENTMAIL );

	if ( fileOut.Open ( strTempFilename, "ab" ) == TRUE )
	{
		CCSV csv;
		if ( fileIn.Read ( csv ) == TRUE )					// read header line1
		{
			fileOut.Write ( csv );							// save header line1

			if ( ( bReply = fileIn.Read ( csv ) ) == TRUE )	// read header line2
			{
				fileOut.Write ( csv );						// save header line2

				while ( fileIn.Read ( csv ) == TRUE )
				{
					__int64 nTransID = csv.GetInt64(nTRANSIDM_INDEX);	// get TranID value from line

					if ( nTransID > nLastProcessedID )
					{
						if ( ( bReply = fileOut.Write(csv) ) == TRUE )	// save new transaction
							++m_nUpdatesCount;
					}
				}
			}
		}
		fileOut.Close();
	}
	fileIn.Close();

	if ( bReply == TRUE )
	{
		CFileRecycler::SendToRecycleBin (  szUpdateFilename );
		::rename ( strTempFilename, szUpdateFilename );
	}

	return bReply;
}

//*******************************************************************
// Get any Manually linked accounts
// Also called before getting any new payments

int CParentMailExport::RequestLinkedAccounts ( CWorkingDlg* pProgress )
{
	int nTotalCount  = 0;
	int nLinkedCount = 0;

	CString strLoginReplyFile = System.GetWebPaymentPath ( "RequestLinkedAccountsReply.xml" );
	CString strNewMemberFile = System.GetWebPaymentPath ( "LinkedAccounts.csv" ); 

	CSSParentMail pm ( &m_data );
	if ( ( m_strError = pm.RequestLinkedAccounts ( strLoginReplyFile, strNewMemberFile, szLINKEDHEADER ) ) == "" )
	{
		CSSUnsortedDatabase db;
		if (db.OpenSSDatabaseReadOnly(strNewMemberFile, 10) == DB_ERR_NONE)
		{
			if (db.GetRecordCount() > 0)
			{
				db.MoveFirst();

				do
				{
					if (pProgress != NULL)
					{
						if (pProgress -> SetCaption2PercentDone((db.GetCurrentRow() * 100L) / db.GetRecordCount()) == TRUE)
						{
							break;
						}
					}

					++nTotalCount;
					CString strCardNo = db.GetString("ProviderID");
					if (strCardNo != "")
					{
						CString strMemberNo = db.GetString("AccountID");
						if (strMemberNo != "")
						{
							CSQLRowAccountFull RowAccount;
							RowAccount.SetUserIDString(strCardNo);

							CSQLRepositoryAccount RepoAccount;
							if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
							{
								RowAccount.SetMemberID(strMemberNo);						// Set MemberID
								RowAccount.SetInfo(m_data.m_nFormNameRefField, "");		// clear reference FormName once MemberID is set
								
								RepoAccount.UpdateRow(RowAccount, NULL);
								
								AuditOpeningBalance(RowAccount);
								++nLinkedCount;
							}
						}
					}
				} 
				while (db.MoveNext() == TRUE);
			}
			db.Close();
		}
	}

	CFileRecycler::SendToRecycleBin ( strNewMemberFile );
	if (m_data.m_bKeepXmlFiles == FALSE)
	{
		CFileRecycler::SendToRecycleBin(strLoginReplyFile);
	}
 
	return nLinkedCount;
}

//*******************************************************************

void CParentMailExport::AuditOpeningBalance(CSQLRowAccountFull& RowAccount)
{
	CSQLAuditRecord atc;
	atc.Set( &RowAccount );
	atc.SetApplicationNo ( APPNO_WPOPENINGBALANCE );
	atc.SaveNewLine();
}

//**********************************************************************
// if no exception then its a proper linked account

void CParentMailExport::SaveLinkNotification ( CSQLRowAccountFull& RowAccount, const char* szException )
{
	CString strComment = szException;

	CString strFilename = ( strComment == "" ) ? m_strTempLinkedFile : m_strTempExceptionFile;

	bool bSaveHeaderReqd = FALSE;
	if (::FileExists(strFilename) == FALSE)
	{
		bSaveHeaderReqd = TRUE;
	}

	CSSFile file;
	if ( file.Open ( strFilename, "ab" ) == TRUE )
	{
		if ( bSaveHeaderReqd == TRUE )
		{
			CCSV csvHeader;
			csvHeader.Add ( Account::UserID.Label );
			csvHeader.Add ( Account::MemberID.Label );	
			csvHeader.Add ( Account::Username.Label );	
			csvHeader.Add ( Account::Forename.Label );	
			csvHeader.Add ( Account::Surname.Label );	

			if (strComment != "")
			{
				csvHeader.Add("Exception");
			}

			file.Write ( csvHeader );
		}

		CCSV csv;
		csv.Add ( System.FormatCardNo(RowAccount.GetUserIDString()) );
		csv.Add ( RowAccount.GetMemberID() );
		csv.Add ( RowAccount.GetUsername() );
		csv.Add ( RowAccount.GetForename() );
		csv.Add ( RowAccount.GetSurname() );

		if (strComment != "")
		{
			csv.Add(strComment);
		}

		file.Write ( csv );
		file.Close();
	}
}

//**********************************************************************
