//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
#include "..\SmartPay4Shared\WaitDlg.h"
//*******************************************************************
#include "Squid.hpp"
#include "WebPaymentDefines.h"
#include "WebPaymentExportTnLog.h"
//*******************************************************************
#include "sQuidExport.h"
//*******************************************************************

CSquidExport::CSquidExport( CWnd* pParent )
{
	m_pParent = pParent;
	m_strError = "";
	m_dBalance = 0.0;
	m_bRpuTransaction = FALSE;
	m_strTransType = "";
	m_strTransReference = "";
	m_nFailures = 0;
	m_strLinkError = "";

	m_data.Read();														// Read System data

	ResetTransaction();

	m_strTempLinkedFile = Filenames.GetTempReportPath ( "R221A", "dat" );
	m_strTempExceptionFile = Filenames.GetTempReportPath ( "R221B", "dat" );
}

//*******************************************************************

void CSquidExport::ResetTransaction(const char* szAccountID)
{
	m_strAccountID = szAccountID;
	m_dSalesValue = 0.0;
	m_strDescription = "";
	m_bRpuTransaction = FALSE;
	m_nRpuSequence = 0;
}

//*******************************************************************
// if reply = FALSE = m_strError is set

bool CSquidExport::SendUpdates()
{
	CString strWebCsvFilename	= m_data.GetUpdatesFilename();							// outstanding csv xml format updates
	CString strAtcUpdates		= Filenames.GetWebPaymentUpdatesFilename ( "dat" );		// ..\{web}\webupdates.dat

	if ( ::FileExists ( strAtcUpdates ) == TRUE )									// check for new audit updates pending
	{
		CString strTmpWebFile = Filenames.GetWebPaymentUpdatesFilename ( "dbs" );	// intermedite file for atc lines to web file format 

		CWebPaymentAtcTransactionFile webFile;										// internal format atc web update file
		if ( webFile.Open( strTmpWebFile ) == DB_ERR_NONE )							// open intermediate file \ create if not found
		{
			webFile.AddNewAtcUpdates ( strAtcUpdates );								// convert new atc audits ( if found ) & add to internal web atc format
			webFile.Close();														// then delete file if all ok

			CFileRecycler::SendToRecycleBin ( strAtcUpdates );

			CString strNewUpdatesFile = m_data.GetNewUpdatesFilename();				// temp file hold new updates

			if (ConvertWebUpdates(strTmpWebFile, strNewUpdatesFile) == -1)		// convert web updates to parentpay export csv xml file format
			{
				return FALSE;
			}

			if ( ::FileExists ( strNewUpdatesFile ) == TRUE )
			{
				if ( ::AppendDataFile ( strNewUpdatesFile, strWebCsvFilename, TRUE ) == TRUE )		// apppend files ( have header )
				{
					CWebPaymentExportTnLog log ( nWEBPAYMENT_SQUID );				// ..\{WebProvider}}\Tnyyyymm.dat
					log.SaveLog ( strNewUpdatesFile );								// Log the new updates

					CFileRecycler::SendToRecycleBin ( strNewUpdatesFile );
				}
			}
			CFileRecycler::SendToRecycleBin ( strTmpWebFile );
		}
	}

	if ( ::FileExists ( strWebCsvFilename ) == TRUE )								// check for any updates to send
	{
		CWaitDlg dlgWait;
		dlgWait.SetMessageText( szINTERNETMESSAGE );

		CSSSquid tx ( &m_data );
		m_strError = tx.UploadTransactions ( strWebCsvFilename, System.GetWebPaymentPath("TxnReply.xml") );

		if (m_strError == "")
		{
			CFileRecycler::SendToRecycleBin(strWebCsvFilename);
		}
	}

	return ( m_strError == "" )	? TRUE : FALSE; 
}

//*******************************************************************
// convert atc updats to csv xml format

int CSquidExport::ConvertWebUpdates ( const char* szUpdatesFile, const char* szCsvFilename )
{
	CWebPaymentAtcTransactionFile webFile;

	if ( webFile.Open( szUpdatesFile ) != DB_ERR_NONE )
		return -1;

	CString strListFilename = m_data.GetWebPaymentListFilename();

	if ( m_WebPaymentList.Open ( strListFilename, DB_READONLY ) != DB_ERR_NONE )	// ret 0=ok, 1=file error, 2=file in use
	{
		m_strError.Format ( "Error in opening the sQuid List file ' %s ' for Reading", strListFilename );
		return -1;
	}

	int nCount = 0;

	CSSFile fileOut;
	if ( fileOut.Open ( szCsvFilename, "wb" ) == TRUE )
	{
		fileOut.WriteLine ( m_data.m_strUploadHeader );					// "amount,closingBalance,dateAndTime,description,merchantId,purseAccountNumber,rpu,rpuSequence,transactionNumber,transactionType"

		m_TransRef.OpenForSharing ( m_data.GetTransRefFilename() );		// database holding last transID for each account

		webFile.MoveFirst();

		do
		{
			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserIDString(webFile.GetCardNo());

			CSQLRepositoryAccount RepoAccount;
			if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				CString strMemberID = RowAccount.GetMemberID();
				if (  ::IsStringNumeric ( strMemberID, 1 ) == TRUE )		// check sQuidID exists & is numeric
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
						if (m_bRpuTransaction == TRUE)					// see if sQuid topup
						{
							__int64 nPaymentID = _atoi64(webFile.GetWebPaymentIDString());

							if (m_WebPaymentList.FindRecord(nPaymentID) == TRUE)	// should exist
							{
								m_nRpuSequence = m_WebPaymentList.GetServiceID();		// sequence no 0 - 255
							}
						}

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

	m_TransRef.Close();

	return TRUE;
}

//*******************************************************************
// with sQuid don't include FSM value in transactions value

bool CSquidExport::IsValidSpend ( CWebPaymentAtcTransactionFile* pFile )
{
	int nActionType = pFile->GetActionType();

	if (nActionType != nATCWEB_SALE && nActionType != nATCWEB_SALEREFUND)
	{
		return FALSE;
	}

	switch ( m_data.m_nAccountPurse )
	{
	case 3:		m_dSalesValue	= pFile->GetPurse3Value();
				m_dBalance		= pFile->GetPurse3Balance();
				break;

	case 2:		m_dSalesValue	= pFile->GetPurse2Value();		// not FSM if using purse2 
				m_dBalance		= pFile->GetPurse2Balance();
				break;

	default:	m_dSalesValue	= pFile->GetPurse1Value();
				m_dBalance		= pFile->GetPurse1Balance();
				break;
	}

	m_strDescription = pFile->GetSalesText();

	if (nActionType == nATCWEB_SALEREFUND)			// see if sale refund	
	{
		m_strTransType = "REFUND";
	}
	else if ( nActionType == nATCWEB_SALE )				// see if sale
	{
		if (m_dSalesValue < 0)						// sales are -ve, check <0 as otherwise would get "-0.00"
		{
			m_dSalesValue = -(m_dSalesValue);			// make positive
		}
		m_strTransType = "PURCHASE";
	}

	return TRUE;
}

//*******************************************************************

bool CSquidExport::IsValidCashAdded ( CWebPaymentAtcTransactionFile* pFile )
{
	int nActionType = pFile->GetActionType();

	if ( nActionType != nATCWEB_TOPUP && nActionType != nATCWEB_TOPUPREFUND && nActionType != nATCWEB_STARTINGBALANCE && pFile->GetActionType() != nATCWEB_TRANSFER )
		return FALSE;

	switch ( m_data.m_nAccountPurse )
	{
	case 3:		m_dSalesValue	= pFile->GetPurse3Value();
				m_dBalance		= pFile->GetPurse3Balance();
				break;

	case 2:		m_dSalesValue	= pFile->GetPurse2Value();
				m_dBalance		= pFile->GetPurse2Balance();
				break;

	default:	m_dSalesValue	= pFile->GetPurse1Value();
				m_dBalance		= pFile->GetPurse1Balance();
				break;
	}

	if ( pFile->GetApplicationNo() == APPNO_SQUID )
		m_bRpuTransaction = TRUE;

	m_strDescription = pFile->GetSalesText();
	m_strTransType	= "TOPUP";

	return TRUE;
}

/**********************************************************************
 "amount,closingBalance,dateAndTime,description,merchantId,purseAccountNumber,rpu,rpuSequence,transactionNumber,transactionType"

  <xs:element name="amount" type="xs:int" minOccurs="0" /> 
  <xs:element name="closingBalance" type="xs:int" minOccurs="0" /> 
  <xs:element name="dateAndTime" type="xs:dateTime" minOccurs="0" /> 
  <xs:element name="description" type="xs:string" minOccurs="0" /> 
  <xs:element name="merchantId" type="xs:int" minOccurs="0" /> 
  <xs:element name="purseAccountNumber" type="xs:string" minOccurs="0" /> 
  <xs:element name="rpu" type="xs:boolean" /> 
  <xs:element name="rpuSequence" type="xs:short" minOccurs="0" /> 
  <xs:element name="samNumber" type="xs:long" minOccurs="0" /> 
  <xs:element name="transactionNumber" type="xs:int" minOccurs="0" /> 
  <xs:element name="transactionType" type="tns:purseTransactionTypeEnum" minOccurs="0" /> 
  <xs:element name="vat" type="xs:int" minOccurs="0" /> 
  ******************************************************************************/

bool CSquidExport::SaveTransactionLine ( CWebPaymentAtcTransactionFile* pFile, CSSFile* fp )
{
	if (m_TransRef.FindRecord(pFile->GetCardNo()) == FALSE)	// see if sent anything before
	{
		m_TransRef.AddBlankRecord(pFile->GetCardNo());				// no add record
	}

	CString strTransRef = m_TransRef.GetNextReference();			// get next SequenceNo / increment & save

	CCSV csv;
	CString strXmlDate = pFile->GetXmlDate(nWEBPAYMENT_SQUID);		// get audit date & time in xml format

	if ( m_bRpuTransaction == TRUE )
	{
		csv.Add ( "" );								//"amount" 
		csv.Add ( m_dBalance * 100, 0 );			//"closingBalance"  in pence
		csv.Add ( strXmlDate );						//"dateAndTime" 
		csv.Add ( m_strDescription );				//"description" 
		csv.Add ( m_data.m_strMerchantID );			//"merchantId" 
		csv.Add ( m_strAccountID );					//"purseAccountNumber" 
		csv.Add ( "true" );							//"rpu" 
		csv.Add ( m_nRpuSequence );					//"rpuSequence" 
		csv.Add ( strTransRef );					//"transactionNumber" 
		csv.Add ( "" );								//"transactionType" 
	}
	else
	{
		csv.Add ( m_dSalesValue * 100, 0 );			//"amount" 
		csv.Add ( m_dBalance * 100, 0 );			//"closingBalance"  in pence
		csv.Add ( strXmlDate );						//"dateAndTime" 
		csv.Add ( m_strDescription );				//"description" 
		csv.Add ( m_data.m_strMerchantID );			//"merchantId" 
		csv.Add ( m_strAccountID );					//"purseAccountNumber" 
		csv.Add ( "false" );						//"rpu" 
		csv.Add ( "" );								//"rpuSequence" 
		csv.Add ( strTransRef );					//"transactionNumber" 
		csv.Add ( m_strTransType );					//"transactionType" 
	}

	return fp->Write ( csv );
}

//**********************************************************************

void CSquidExport::SaveLinkReference(CSQLRowAccountFull& RowAccount)
{
	if ( m_data.m_bKeepLinkLog == TRUE )
	{
		bool bSaveHeaderReqd = FALSE;

		CString strLinkList = m_data.GetSquidLinkListFilename();				// file to list of link card accounts
		if (::FileExists(strLinkList) == FALSE)							// if not found - create file with header line
		{
			bSaveHeaderReqd = TRUE;
		}

		CSSFile file;
		if ( file.Open ( strLinkList, "ab" ) == TRUE )
		{
			if ( bSaveHeaderReqd == TRUE )
			{
				CCSV csvHeader;
				csvHeader.Add ( "Date" );
				csvHeader.Add ( "Time" );
				csvHeader.Add ( Account::UserID.Label );						// UserID
				csvHeader.Add ( System.GetWebPaymentDisplayName() );			// Member ID
				file.Write ( csvHeader );
			}

			CCSV csv;
			csv.Add ( CSSDate::GetCurrentDate().GetDate() );
			csv.Add ( CSSTime::GetCurrentTime().GetTime() );
			csv.Add ( RowAccount.GetUserID() );
			csv.Add ( RowAccount.GetMemberID() );
			file.Write ( csv );
			file.Close();
		}
	}
}

//**********************************************************************
// if no exception then its a proper linked account

void CSquidExport::SaveLinkNotification(CSQLRowAccountFull& RowAccount, const char* szException)
{
	CString strComment = szException;

	CString strFilename = (strComment == "") ? m_strTempLinkedFile : m_strTempExceptionFile;

	bool bSaveHeaderReqd = FALSE;
	if (::FileExists(strFilename) == FALSE)
	{
		bSaveHeaderReqd = TRUE;
	}

	CSSFile file;
	if (file.Open(strFilename, "ab") == TRUE)
	{
		if (bSaveHeaderReqd == TRUE)
		{
			CSQLRepositoryUserText RepoText;

			CCSV csvHeader;
			csvHeader.Add(Account::UserID.Label);
			csvHeader.Add(Account::MemberID.Label);
			csvHeader.Add(RepoText.GetDisplayInfoText(m_data.m_nSquidCardHolderField));
			csvHeader.Add(RepoText.GetDisplayInfoText(m_data.m_nSquidExtRefField));
			csvHeader.Add(Account::Username.Label);
			csvHeader.Add(Account::Forename.Label);
			csvHeader.Add(Account::Surname.Label);

			if (strComment != "")
			{
				csvHeader.Add("Exception");
			}

			file.Write(csvHeader);
		}

		CCSV csv;
		csv.Add(System.FormatCardNo(RowAccount.GetUserIDString()));
		csv.Add(RowAccount.GetMemberID());
		csv.Add(m_data.GetSquidLinkReference1(RowAccount));
		csv.Add(m_data.GetSquidLinkReference2(RowAccount));
		csv.Add(RowAccount.GetUsername());
		csv.Add(RowAccount.GetForename());
		csv.Add(RowAccount.GetSurname());

		if (strComment != "")
		{
			csv.Add(strComment);
		}

		file.Write(csv);
		file.Close();
	}
}

//*********************************************************************
