//*********************************************************************
//*** make csv file containing card balances for sending to web site **
//
// used By	TUCASI \ SCHOOLCOMMS \ TUCASI.v2
//
//*********************************************************************
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//*********************************************************************
#include "SchoolCommsData.h"
#include "TucasiJsonData.h"
#include "TucasiSoapData.h"
#include "WebPaymentBalanceFile.h"
//*********************************************************************

CWebPaymentBalanceFile::CWebPaymentBalanceFile ()
{
	m_strError = "";
	m_nBalanceCount = 0;
	m_nAccountNameFormat = 1;		// 0=Forename Surname, 1=Account Name, 2= Surname only, 3= Surname Forename
	m_bIncludePurse1 = FALSE;
	m_bIncludePurse2 = FALSE;		// - which purses to combine into a single balance field
	m_bIncludePurse2 = FALSE;
	m_nBalance1Purse = 0;			// } which purse to use if more than 1 balance field in xml file
	m_nBalance2Purse = 0;			// } used by WisePay

	m_strCsvFilename = System.GetWebPaymentPath ( "balances.csv" );

// Tucasi (SOAP) ***************

	if ( System.GetWebPaymentType() == nWEBPAYMENT_TUCASI )
	{
		CTucasiSoapData data;
		data.Read();

		m_bIncludePurse2 = data.m_bIncludePurse2;

		switch ( data.m_nAccountPurse )
		{
		case 3:		m_bIncludePurse3 = TRUE;	break;
		case 2:		m_bIncludePurse2 = TRUE;	break;
		default:	m_bIncludePurse1 = TRUE;	break;
		}
		m_strCsvHeader = "balance,pupilName,pupilNum";
	}

// WisePay ******************
//
//	else if ( System.GetWebPaymentType() == nWEBPAYMENT_WISEPAY )
//	{
//		CWisePayData data;
//		data.Read();
//
//		if ( data.m_bImportAccount1 == TRUE )				// dier service
//		{
//			m_nBalance1Purse = data.m_nAccount1Purse;
//			m_bIncludePurse2 = data.m_bIncludePurse2;
//		}
//
//		if ( data.m_bImportAccount2 == TRUE )				// supplementary service
//			m_nBalance2Purse = data.m_nAccount2Purse;
//
//		m_strCsvHeader = "student_id,balance,balance2";
//	}

// Schoolcomms ***********

	else if ( System.GetWebPaymentType() == nWEBPAYMENT_SCHOOLCOMMS )
	{
		CSchoolcommsData data;
		data.Read();

		m_bIncludePurse2 = data.m_bIncludePurse2;

		switch ( data.m_nAccountPurse )
		{
		case 3:		m_bIncludePurse3 = TRUE;	break;
		case 2:		m_bIncludePurse2 = TRUE;	break;
		default:	m_bIncludePurse1 = TRUE;	break;
		}
		m_strCsvHeader = data.m_strUploadHeader;	// 	"ChargedAmount,CurrentMemberBalance,DescriptionOfPurchasedItem,FreeSchoolMeal,FullAmount,PartnerTransactionID,ReceiptID,SIMSPersonID,TransactionDate,TransactionType";
	}

// Tucasi(JSON) ***************

	else if ( System.GetWebPaymentType() == nWEBPAYMENT_TUCASIv2 )
	{
		CTucasiJsonData data;
		data.Read();											// only send balances

		m_bIncludePurse2		= data.m_bIncludePurse2;
		m_nAccountNameFormat	= data.m_nAccountNameFormat;

		switch ( data.m_nAccountPurse )
		{
		case 3:		m_bIncludePurse3 = TRUE;	break;
		case 2:		m_bIncludePurse2 = TRUE;	break;
		default:	m_bIncludePurse1 = TRUE;	break;
		}
		m_strCsvHeader = "balance,accountID,accountName";
	}

	else if (System.GetWebPaymentType() == nWEBPAYMENT_BROMCOM)
	{
		m_bIncludePurse1 = TRUE;
		m_bIncludePurse2 = FALSE;
		m_bIncludePurse3 = FALSE;
	}
}

//*******************************************************************

bool CWebPaymentBalanceFile::Create ( const char* szDbBalanceList )
{
	CSSFile fpCsv;
	if (fpCsv.Open(m_strCsvFilename, "wb") == FALSE)
	{
		m_strError.Format("Unable to open output file ' %s ' for Write", (const char*) m_strCsvFilename);
		return FALSE;
	}

	CString strMemberList = szDbBalanceList;

	if (strMemberList != "")
	{
		CMemberDatabase members(strMemberList);
		
		if (members.OpenForSharing() == FALSE)				// ret 0=ok, 1=file error, 2=file in use
		{
			m_strError = members.GetError();
			return FALSE;
		}

		fpCsv.WriteLine(m_strCsvHeader);						

		members.MoveFirst();									// get first record
		do
		{
			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserIDString(members.GetUserID());

			CSQLRepositoryAccount RepoAccount;
			if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				SaveCSVBalance(RowAccount, &fpCsv);
			}
		} 
		while (members.MoveNext() == TRUE);

		members.Close();
	}
	else
	{
		fpCsv.WriteLine(m_strCsvHeader);
		
		CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_NoParams{}, FALSE);
		RecordSet.m_strFilter = "MemberID != '' AND NextUserID = 0";

		CSQLRowAccountFull RowAccount;
		while ( RecordSet.StepSelectAll(RowAccount) == TRUE )
		{
			SaveCSVBalance(RowAccount, &fpCsv);
		}
	}

	fpCsv.Close();
	return TRUE;
}

//*******************************************************************
// save intermediate csv balance line

void CWebPaymentBalanceFile::SaveCSVBalance ( CSQLRowAccountFull& RowAccount, CSSFile *fp )
{
	CString strMemberID = RowAccount.GetMemberID();			// Just in case no MemeberID in account set
	if (strMemberID == "")									// 11/07/2017 - v1.15b
	{
		return;
	}

	double dBalance1 = 0.0;										// set if multiple balances reqd
	double dBalance2 = 0.0;										// set if multiple balances reqd
	double dTotalBalance = 0.0;					

	if ( (m_nBalance1Purse == 0) && (m_nBalance2Purse == 0) )							// see if multiple balances reqd
	{																				// no 			
		if ( m_bIncludePurse1 == TRUE )	dTotalBalance  = RowAccount.GetPurse1Balance();	// create total purse balance value
		if ( m_bIncludePurse2 == TRUE )	dTotalBalance += RowAccount.GetPurse2Balance();
		if ( m_bIncludePurse3 == TRUE ) dTotalBalance += RowAccount.GetPurse3Balance();
	}

	CSSDate date;	date.SetCurrentDate();
	CSSTime time;	time.SetCurrentTime();
	CString strXmlDate = ::FormatXmlDate ( &date, &time );		// current date & time

// account name

	CString strName = "";
	switch (  m_nAccountNameFormat )
	{
	case 0:	strName = ::CompileName (RowAccount.GetForename(), RowAccount.GetSurname() );	break;
	case 1:	strName = ::CompileName (RowAccount.GetUsername(), "" );							break;
	case 2:	strName = ::CompileName (RowAccount.GetSurname(), "" );							break;
	case 3:	strName = ::CompileName (RowAccount.GetSurname(), RowAccount.GetForename() );	break;
	}
	
// has been used or have purse1 balance

	CCSV csv;
	
	if ( System.GetWebPaymentType() == nWEBPAYMENT_TUCASI )
	{
		csv.Add ( ( dTotalBalance * 100 ), 0 );			// <balance> in pence
		csv.Add ( strName );							// Name
		csv.Add ( strMemberID );						// ID No
	}
	
//	else if ( System.GetWebPaymentType() == nWEBPAYMENT_WISEPAY )
//	{
//		switch ( m_nBalance1Purse )						// Service 1 balance ( Diner purse0
//		{
//		case 3:		dBalance1 = pDbSmartPay->GetPurse3();	break;
//		case 2:		dBalance1 = pDbSmartPay->GetPurse2();	break;
//		case 1:		dBalance1 = pDbSmartPay->GetPurse1();	break;
//		default:	dBalance1 = 0;							break;
//		}
//
//		if ( m_bIncludePurse2 == TRUE )
//			dBalance1 += pDbSmartPay->GetPurse2();
//
//		switch ( m_nBalance2Purse )						// Service 2 balance
//		{
//		case 3:		dBalance2 = pDbSmartPay->GetPurse3();	break;
//		case 2:		dBalance2 = pDbSmartPay->GetPurse2();	break;
//		case 1:		dBalance2 = pDbSmartPay->GetPurse1();	break;
//		default:	dBalance2 = 0;							break;
//		}
//
//		csv.Add ( strMemberID );							// ID No
//
//		if ( m_nBalance1Purse == 0 )	csv.Add ( "" );		// <balance>
//		else							csv.Add ( dBalance1, 2 );
//
//		if ( m_nBalance2Purse == 0 )	csv.Add ( "" );		// <balance2>
//		else							csv.Add ( dBalance2, 2 );
//
//		dTotalBalance = dBalance1 + dBalance2;
//	}
	
	else if ( System.GetWebPaymentType() == nWEBPAYMENT_SCHOOLCOMMS )
	{
		csv.Add ( "0" );							// ChargedAmount,
		csv.Add ( dTotalBalance, 2 );				// CurrentMemberBalance
		csv.Add ( "" );								// DescriptionOfPurchasedItem
		csv.Add ( "false" );						// FreeSchoolMeal
		csv.Add ( "0" );							// FullAmount,
		csv.Add ( "0" );							// PartnerTransactionID
		csv.Add ( "0" );							// ReceiptID
		csv.Add ( strMemberID );					// SIMSPersonID
		csv.Add ( strXmlDate );						// TranactionDate
		csv.Add ( "BalanceOnly" );					// TransactionType
	}
	
	else if ( System.GetWebPaymentType() == nWEBPAYMENT_TUCASIv2 )
	{
		csv.Add ( ( dTotalBalance * 100 ), 0 );			// <balance> in pence
		csv.Add ( strMemberID );						// ID No
		csv.Add ( strName );							// Name
	}

//

	if ((RowAccount.GetLastUsed(date, time) == FALSE) && (dTotalBalance == 0.0))	// see if account has ever been used
	{
		return;																	// never used & no value - so ignore
	}

	fp->Write ( csv );
	++m_nBalanceCount;
}

//*******************************************************************
