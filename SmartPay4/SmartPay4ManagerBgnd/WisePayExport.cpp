//$$******************************************************************
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//$$******************************************************************
#include "WebPaymentExportTnLog.h"
#include "WisePay.hpp"
//$$******************************************************************
#include "WisePayExport.h"
//$$******************************************************************

CWisePayExport::CWisePayExport ( CWnd* pParent )
{
	m_pParent = pParent;
	m_strError = "";
	m_data.Read();												// Read System data
}

//**********************************************************************
// csvfile in WisePay balamnce file format ( with header line )
//
//bool CWisePayExport::SendBalances ( const char* szWebCsvFile, const char* szLoginReply )
//{
//	CSSWisePay wp ( &m_data );
//	m_strError = wp.SendBalances ( szWebCsvFile, szLoginReply );
//
//	return ( m_strError == "" )	? TRUE : FALSE; 
//}
//
//**********************************************************************
// csvfile in WisePay transaction file format ( with header line )

bool CWisePayExport::SendTransactions1 ( const char* szWebCsvFile, const char* szLoginReply )
{
	CSSWisePay wp ( &m_data );
	m_strError = wp.SendTransactions ( szWebCsvFile, szLoginReply );

	return ( m_strError == "" )	? TRUE : FALSE; 
}

//**********************************************************************
// csvfile in WisePay transaction file format Service 2 ( with header line )

bool CWisePayExport::SendTransactions2 ( const char* szWebCsvFile, const char* szLoginReply )
{
	CSSWisePay wp ( &m_data );
	m_strError = wp.SendTransactions2 ( szWebCsvFile, szLoginReply );

	return ( m_strError == "" )	? TRUE : FALSE; 
}

//*******************************************************************
//	m_strUploadHeader1 = "student_id,description,value,transaction_id,balance,cart_date_time";
//	m_strUploadHeader2 = "student_id,description,value,product_id,transaction_id,balance,cart_date_time";

int CWisePayExport::CreateStartingBalancesFile ( int nService, const char* szListFilename, const char* szDestFilename )
{
	int nCount = -1;										// set error

	CSSFile fileList;
	if ( fileList.Open ( szListFilename, "rb" ) == TRUE )
	{
		CSSFile fileOut;
		if ( fileOut.Open ( szDestFilename, "wb" ) == TRUE )
		{
			nCount = 0;
			if (nService == 2)
			{
				fileOut.WriteLine(m_data.m_strUploadHeader2);		// Supplementary service
			}
			else
			{
				fileOut.WriteLine(m_data.m_strUploadHeader1);		// Diner service
			}

			CString strUserID = "";
			while ( fileList.ReadString ( strUserID ) == TRUE )
			{
				CSQLRepositoryAccount RepoAccount;
				CSQLRowAccountFull RowAccount;
				RowAccount.SetUserIDString(strUserID);
				if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
				{
					if (SaveStartingBalanceLine(RowAccount, nService, &fileOut) == TRUE)
					{
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

bool CWisePayExport::SaveStartingBalanceLine ( CSQLRowAccountFull& RowAccount, int nService, CSSFile* fp )
{
	CCSV csv;
	double dBalance = 0;
	CString strXmlDate = ::FormatXmlDate( CSSDate::GetCurrentDate().GetDate(), CSSTime::GetCurrentTime().GetTime() );	// set current date & time

	if ( nService == 2 )										// Service 2 trsanction
	{
		switch ( m_data.m_nAccount2Purse )
		{
		case 1: dBalance = RowAccount.GetPurse1Balance();	break;
		case 2: dBalance = RowAccount.GetPurse2Balance();	break;
		case 3: dBalance = RowAccount.GetPurse3Balance();	break;
		}

		csv.Add ( RowAccount.GetMemberID() );					// student_id
		csv.Add ( m_data.m_strBalanceText );					// description
		csv.Add ( 0, 2 );										// value of transaction
		csv.Add (  m_data.m_nAccountID2 );						// Service product_id
		csv.Add ( "0" );										// transaction_id
		csv.Add ( dBalance, 2 );								// opening balance
		csv.Add ( strXmlDate );									// cart_date_time
	}
	else														// standard diner transaction
	{
		switch ( m_data.m_nAccount1Purse )
		{
		case 1: dBalance = RowAccount.GetPurse1Balance();	break;
		case 2: dBalance = RowAccount.GetPurse2Balance();	break;
		case 3: dBalance = RowAccount.GetPurse3Balance();	break;
		}

		//FIDDLE : IS THIS CORRECT : MAY INCLUDE PURSE 2 TWICE ??
		if (m_data.m_bIncludePurse2 == TRUE)
		{
			dBalance += RowAccount.GetPurse2Balance();
		}

		csv.Add (RowAccount.GetMemberID() );				// student_id
		csv.Add ( m_data.m_strBalanceText);						// description
		csv.Add ( 0, 2 );										// value of transaction
		csv.Add ( "0" );										// transaction_id
		csv.Add ( dBalance, 2 );								// opening balance
		csv.Add ( strXmlDate );									// cart_date_time
	}

	return fp->Write ( csv );
}

//**********************************************************************
// Log the new webupdates (Tnyyyymm \ Toyyyymm)- File has header & fields are in Header order

bool CWisePayExport::LogBalanceUpdates ( int nServiceNo, const char* szNewUpdates, int nCount )
{
	bool bReply = FALSE;
	CString strPrefix = ( nServiceNo == 2 ) ? "To" : "Tn";
	CWebPaymentExportTnLog log ( nWEBPAYMENT_WISEPAY, strPrefix );
	CString strLogFile	= log.GetCurrentLogName();						// ..\{WebProvider}}\Tnyyyymm.dat

	if ((bReply = ::AppendDataFile(szNewUpdates, strLogFile, TRUE)) == TRUE)	// append updates & save in logfile in web based csv format
	{
		log.AddDateStamp(strLogFile, nCount);									// add date stamp line 'Sent : dd/mm/yy @ hh:mm ' ONLY if LineCount > 0
	}

	return bReply;
}

//*******************************************************************


