// Export Purchases to web site using ATC transaction updates
//
//    used by		nWEBPAYMENT_TUCASI
//    used by 		nWEBPAYMENT_SCHOOLCOMMS:
//    used by		nWEBPAYMENT_TUCASIv2
//    used by 		nWEBPAYMENT_EXPORTSALES:
//	  used by 		nWEBPAYMENT_WISEPAY:
//
//*******************************************************************
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//*******************************************************************
#include "SalesExportData.h"
#include "SchoolCommsData.h"
#include "TucasiJSONData.h"
#include "TucasiSOAPData.h"
#include "WebPaymentAtcTransactionFile.h"
#include "WebPaymentExportTnLog.h"
#include "WisePayData.h"
//*******************************************************************
#include "WebPaymentPurchaseFile.h"
//*******************************************************************

CWebPaymentPurchaseFile::CWebPaymentPurchaseFile(int nWebPaymentType)
{
	m_nWebPaymentType = nWebPaymentType;
	m_nService1Purse = 0;			// WisePay Diner purse
	m_bIncludePurse1 = FALSE;
	m_bIncludePurse2 = FALSE;
	m_bIncludePurse3 = FALSE;
	m_nAccountNameFormat = 1;		// 0=Forename Surname, 1=Account Name, 2= Surname only, 3= Surname Forename
	m_nService2ID = 0;
	m_nService2Purse = 0;			// Service2 purse ( if used )

	m_strError = "";
	m_nExportLineCount = 0;
	m_strDbBalanceList = "";							// hold list of account to send back balance for
	m_strAdditionalHeader = "";
}

//*******************************************************************

CString CWebPaymentPurchaseFile::NextReferenceNo(int nCCNo)
{
	__int64 nTransNo = 0;

	CSSFile file;
	CString strMyTransNoFilename = System.GetWebPaymentPath("transno.dat", m_nWebPaymentType);

	// see if can read file, won't be there on new system
	if (file.Open(strMyTransNoFilename, "rb") == TRUE)	
	{
		CCSV csv;
		if (file.Read(csv) == TRUE)
		{
			nTransNo = csv.GetInt64(0);
		}

		file.Close();									
	}

	// try & write new ccno
	if (file.Open(strMyTransNoFilename, "wb") == TRUE)	
	{
		CCSV csv;
		csv.Add(++nTransNo);
		file.Write(csv);
		file.Close();
	}

	CString strRef = "";

	if (nCCNo == -1)
	{
		strRef = ::FormatInt64Value(nTransNo);
	}
	else
	{
		strRef.Format("%I64d%4.4d", nTransNo, nCCNo);
	}

	return strRef;
}

//********************************************************************************
//*** Convert internal ATC web updates to csv web payment purchases & topups *****
//********************************************************************************

bool CWebPaymentPurchaseFile::ConvertWebUpdates ( const char* szWebAtcUpdateFile, const char* szWebCsvFile1, const char* szWebCsvFile2 )
{
	CString strHeader1			= "";								// Diner - Service 1
	CString strHeader2			= "";								// Secondary Service 2 ( so far only WisePay )
	CString strUpdatesFile		= "";								// outstanding unprocessed updates file

	if ( m_nWebPaymentType == nWEBPAYMENT_TUCASI )
	{
		CTucasiSoapData data;
		data.Read();

		strHeader1			= data.GetUploadPurchaseHeader();		// purchase file transaction header
		strUpdatesFile		= data.GetUpdatesFilename();			// outstanding updates (in internal web atc format )
	}

	else if ( m_nWebPaymentType == nWEBPAYMENT_WISEPAY )
	{
		CWisePayData data;
		data.Read();

		strHeader1			= data.m_strUploadHeader1;					// purchase file transaction header ( Service 1 )
		strUpdatesFile		= data.GetUpdatesFilename();				// outstanding updates (in internal web atc format )
		m_nService1Purse	= data.m_nAccount1Purse;					// Standard Diner purse ( service1 )
		m_bIncludePurse2	= data.m_bIncludePurse2;					// See if Add purse2 ro diner service balance

		switch ( data.m_nAccount1Purse )
		{
		case 3:		m_bIncludePurse3 = TRUE;	break;
		case 2:		m_bIncludePurse2 = TRUE;	break;
		default:	m_bIncludePurse1 = TRUE;	break;
		}

		if ( data.m_bImportAccount2 == TRUE )							// see if have a 2nd service 2
		{
			m_nService2ID		= data.m_nAccountID2;					// Service 2 ID
			m_nService2Purse	= data.m_nAccount2Purse;				// Service 2 purse
			strHeader2			= data.m_strUploadHeader2;				// purchase file transaction header ( Service 2 )
		}
	}

	else if (m_nWebPaymentType == nWEBPAYMENT_SCHOOLCOMMS)
	{
		CSchoolcommsData data;
		data.Read();

		strHeader1 = data.m_strUploadHeader;				// purchase file transaction header
		strUpdatesFile = data.GetUpdatesFilename();			// outstanding updates (in internal web atc format )
		m_bIncludePurse2 = data.m_bIncludePurse2;
		m_strDbBalanceList = data.GetBalanceDBListFilename();		// holds list of membership balances to send back

		switch (data.m_nAccountPurse)
		{
		case 3:		m_bIncludePurse3 = TRUE;	break;
		case 2:		m_bIncludePurse2 = TRUE;	break;
		default:	m_bIncludePurse1 = TRUE;	break;
		}
	}

	if ( m_nWebPaymentType == nWEBPAYMENT_TUCASIv2 )
	{
		CTucasiJsonData data;
		data.Read();

		strHeader1				= data.m_strUploadPurchaseHeader;		// purchase file transaction header
		strUpdatesFile			= data.GetUpdatesFilename();			// outstanding updates (in internal web atc format )
		m_bIncludePurse2		= data.m_bIncludePurse2;
		m_nAccountNameFormat	= data.m_nAccountNameFormat;

		switch ( data.m_nAccountPurse )
		{
		case 3:		m_bIncludePurse3 = TRUE;	break;
		case 2:		m_bIncludePurse2 = TRUE;	break;
		default:	m_bIncludePurse1 = TRUE;	break;
		}
	}


	else if (m_nWebPaymentType == nWEBPAYMENT_EXPORTSALES)
	{
		if (m_dbReporter.Read("EXP") == TRUE)
		{
			m_strAdditionalHeader = m_dbReporter.m_strHeader;				// additional database fields to add to output line
		}

		CSalesExportData data;
		data.Read();

		strHeader1 = data.GetUploadHeader(m_strAdditionalHeader);	// sales \ topup transaction header
		strUpdatesFile = data.GetUpdatesFilename();						// outstanding updates (in internal web atc format )
	}

//

	bool bDeleteReqd = FALSE;
	if (::FileExists(szWebAtcUpdateFile) == TRUE)									// see if have new webupdates lurking
	{
		bDeleteReqd = ::AppendDataFile(szWebAtcUpdateFile, strUpdatesFile, TRUE);	// append to service provides lurking updates ( with headers )
	}

	bool bDeleteTmp1 = FALSE;
	CString strTmpName = ::GetUniqueTempFilename ( "$31" );							// Diner service
	CString strTmpFile1 = System.GetWebPaymentPath ( strTmpName, m_nWebPaymentType );

	bool bDeleteTmp2 = FALSE;
	strTmpName = ::GetUniqueTempFilename ( "$32" );									// secondary service
	CString strTmpFile2 = System.GetWebPaymentPath ( strTmpName, m_nWebPaymentType );

//
	CMemberDatabase DbBalanceList(m_strDbBalanceList);

	if ( m_strDbBalanceList != "" )												// see if balance list reqd
	{
		DbBalanceList.OpenForSharing ( DB_READWRITE );							// yes - open balance list database
	}

	if ( SaveAtcWebFields ( DbBalanceList, strUpdatesFile, strTmpFile1, strHeader1, strTmpFile2, strHeader2 ) > 0 )			// create csvfile with web paymentfields
	{
		bool DeleteUpdateFile = FALSE;

		if ( ::FileExists ( strTmpFile1 ) == TRUE )
		{
			if ( ::AppendDataFile ( strTmpFile1, szWebCsvFile1, TRUE ) == TRUE )		// add new updates to web csv file
			{
				bDeleteTmp1 = LogAtcUpdates ( strTmpFile1, "Tn" );						// log new updates only then delete tmp new updates
				DeleteUpdateFile = TRUE;
			}
		}

		if ( ::FileExists ( strTmpFile2 ) == TRUE )
		{
			if ( ::AppendDataFile ( strTmpFile2, szWebCsvFile2, TRUE ) == TRUE )		// add new updates to web csv file
			{
				bDeleteTmp2 = LogAtcUpdates ( strTmpFile2, "To" );						// log new updates only Service 2 then delete tmp new updates
				DeleteUpdateFile = TRUE;
			}
		}

		if (DeleteUpdateFile == TRUE)												// delete lurking updates if anything has been save
		{
			CFileRecycler::SendToRecycleBin(strUpdatesFile);									// otherwise would duplicate entries
		}
//Above needs checking as it was added whilst doing .v6  version

	}
	else
	{
		bDeleteTmp1 = TRUE;															// delete empty diner service file
	}
	
	if (m_strDbBalanceList != "")													// see if balance list is in use
	{
		DbBalanceList.Close();													// yes - close it
	}

	if ( bDeleteReqd == TRUE )		CFileRecycler::SendToRecycleBin ( szWebAtcUpdateFile );		// delete new lurking web updates
	if ( bDeleteTmp1 == TRUE )		CFileRecycler::SendToRecycleBin ( strTmpFile1 );	
	if ( bDeleteTmp2 == TRUE )		CFileRecycler::SendToRecycleBin ( strTmpFile2 );	

	return TRUE;
}

//**********************************************************************
// convert an AtcWebUpdateTransaction file to web based csv file with xml fields

int CWebPaymentPurchaseFile::SaveAtcWebFields ( CMemberDatabase& DbBalanceList, const char* szUpdatesFile, const char* szWebCsvFile1, const char* szHeader1, const char* szWebCsvFile2, const char* szHeader2 )
{
	CWebPaymentAtcTransactionFile webFile;
	if (webFile.Open(szUpdatesFile) != DB_ERR_NONE)				// will create it if not found
	{
		return -1;
	}

	int nLineCount = 0;
	int nWebFileCount;

	if ( ( nWebFileCount = webFile.GetRecordCount() ) > 0 )			// check to see if any lines
	{
		webFile.MoveFirst();
		do
		{
			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserIDString(webFile.GetCardNo());

			CSQLRepositoryAccount RepoAccount;
			if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
			{
				CString strDinerServiceLine		= "";				// main diner service ( service 1 )
				CString strSecondServiceLine	= "";				// secondary service ( WisePay service 2 )

				CString strMemberID		= RowAccount.GetMemberID();
				bool bSaveAllowed		= ( strMemberID == "" ) ? FALSE : TRUE;

				switch ( m_nWebPaymentType)
				{
				case nWEBPAYMENT_TUCASI:		strDinerServiceLine = SaveTucasiSoapDetails ( RowAccount, &webFile );
												break;

				case nWEBPAYMENT_SCHOOLCOMMS:	strDinerServiceLine = SaveSchoolcommsDetails (RowAccount, &webFile );
												break;

				case nWEBPAYMENT_WISEPAY:		SaveWisePayDetails ( RowAccount, &webFile, strDinerServiceLine, strSecondServiceLine );
												break;

				case nWEBPAYMENT_TUCASIv2:		strDinerServiceLine = SaveTucasiJsonDetails (RowAccount, &webFile );
												break;

				case nWEBPAYMENT_EXPORTSALES:	strDinerServiceLine = SaveExportSalesDetails ( RowAccount, &webFile );
												bSaveAllowed = TRUE;	// allowed even if no member id
												break;

				default:						break;
				}
	
				if ( ( strDinerServiceLine != "" || strSecondServiceLine != "" ) && bSaveAllowed == TRUE )
				{
					bool bSaveBalanceList = FALSE; 
					if ( strDinerServiceLine != "" )
					{
						if ( SaveService1 ( szWebCsvFile1, szHeader1, strDinerServiceLine ) == TRUE )	// Diner service reqd
						{
							++nLineCount;
							bSaveBalanceList = TRUE;
						}
					}

					if ( strSecondServiceLine != "" )
					{
						if ( SaveService2 ( szWebCsvFile2, szHeader2, strSecondServiceLine ) == TRUE )	// Service2 save reqd
						{
							++nLineCount;
							bSaveBalanceList = TRUE;
						}
					}

					if ( bSaveBalanceList == TRUE )
					{
						if ( (m_strDbBalanceList != "") && (strMemberID != "") )
						{
							if (DbBalanceList.FindRecord(strMemberID) == FALSE)
							{
								DbBalanceList.AddBlankRecord(strMemberID);
							}

							DbBalanceList.SetUserID ( RowAccount.GetUserIDString() );
						}
					}
				}
			}
		}
		while ( webFile.MoveNext() == TRUE );
	}
	webFile.Close();

	if (nWebFileCount == 0)									// delete file if no records
	{
		CFileRecycler::SendToRecycleBin(szUpdatesFile);
	}

	return nLineCount;
}

//***************************************************************************
// Diner service

bool CWebPaymentPurchaseFile::SaveService1 ( const char* szWebCsvFile1, const char* szHeader1, const char* szLine )
{
	bool bReply;
	bool bFileExists = ::FileExists ( szWebCsvFile1 );			// csv output file containing csv xml fields

	CSSFile file;
	if ( ( bReply = file.Open ( szWebCsvFile1, "ab" ) ) == TRUE )
	{
		if ( bFileExists == FALSE )								// see if new file
			file.WriteLine ( szHeader1 );						// yes - save new haeder

		file.WriteLine ( szLine );
		file.Close();
	}
	return bReply;
}

//***************************************************************************
// secondary service 2 ( Wise Pay )

bool CWebPaymentPurchaseFile::SaveService2 ( const char* szWebCsvFile2, const char* szHeader2, const char* szLine )
{
	bool bReply;
	bool bFileExists = ::FileExists ( szWebCsvFile2 );			// csv output file containing csv xml fields

	CSSFile file;
	if ( ( bReply = file.Open ( szWebCsvFile2, "ab" ) ) == TRUE )
	{
		if (bFileExists == FALSE)								// see if new file
		{
			file.WriteLine(szHeader2);						// yes - save new haeder
		}

		file.WriteLine ( szLine );
		file.Close();
	}
	return bReply;
}

//**********************************************************************
//	Header = "description,paymentAmount,paymentDate,pupilName,pupilNumber,transactionItemNumber,transactionNumber";

CString CWebPaymentPurchaseFile::SaveTucasiSoapDetails ( CSQLRowAccountFull& RowAccount, CWebPaymentAtcTransactionFile* pFile )
{
	CCSV csv;
	csv.Add ( pFile->GetSalesText() );						// description
	csv.Add ( pFile->GetTransactionValue() * 100, 0 );		// paymentAmount (pence) - NOTE Tucasi require sales to be -ve
	csv.Add ( pFile->GetXmlDate ( nWEBPAYMENT_TUCASI ) );	// paymentDate special format for Tucasi
	csv.Add ( pFile->GetCardName() );						// pupilName
	csv.Add ( RowAccount.GetMemberID() );					// pupilNumber
	csv.Add ( "0" );										// transactionItemNumber
	csv.Add ( NextReferenceNo ( pFile->GetShortCCNo()) );	// transactionNumber

	CString strLine = csv.GetLine();
	return strLine;
}

//**********************************************************************
//	"transactionNumber,transactionItemNumber,paymentDate,paymentAmount,description,balance,accountID,accountName";

CString CWebPaymentPurchaseFile::SaveTucasiJsonDetails (CSQLRowAccountFull& RowAccount, CWebPaymentAtcTransactionFile* pFile )
{
	double dBalance = ( m_bIncludePurse1 == TRUE ) ? pFile->GetPurse1Balance() : 0;
	if ( m_bIncludePurse2 == TRUE )	dBalance += pFile->GetPurse2Balance();
	if ( m_bIncludePurse3 == TRUE ) dBalance += pFile->GetPurse3Balance();

	CCSV csv;
	csv.Add ( NextReferenceNo ( pFile->GetShortCCNo()) );	// transactionNumber
	csv.Add ( "0" );										// transactionItemNumber
	csv.Add ( pFile->GetXmlDate ( nWEBPAYMENT_TUCASIv2 ) );	// paymentDate special format for Tucasi
	csv.Add ( pFile->GetTransactionValue() * 100, 0 );		// paymentAmount (pence) - NOTE Tucasi require sales to be -ve
	csv.Add ( pFile->GetSalesText() );						// description
	csv.Add ( dBalance * 100, 0 );							// balance (pence) - NOTE Tucasi require sales to be -ve
	csv.Add ( RowAccount.GetMemberID()  );					// accountID
	csv.Add ( GetAccountName ( RowAccount, pFile ) );		// accountName

	CString strLine = csv.GetLine();
	return strLine;
}

//**********************************************************************

CString CWebPaymentPurchaseFile::GetAccountName (CSQLRowAccountFull& RowAccount, CWebPaymentAtcTransactionFile* pFile )
{
	CString strName = "";
	switch (  m_nAccountNameFormat )
	{
	case 0:	strName = ::CompileName ( RowAccount.GetForename(), RowAccount.GetSurname() );	break;
	case 1:	strName = ::CompileName ( RowAccount.GetUsername(), "" );							break;
	case 2:	strName = ::CompileName ( RowAccount.GetSurname(), "" );								break;
	case 3:	strName = ::CompileName (RowAccount.GetSurname(), RowAccount.GetForename() );	break;
	}

	return strName;
}

//**********************************************************************
// 	"ChargedAmount,CurrentMemberBalance,DescriptionOfPurchasedItem,FreeSchoolMeal,FullAmount,PartnerTransactionID,ReceiptID,SIMSPersonID,TransactionDate,TransactionType";

CString CWebPaymentPurchaseFile::SaveSchoolcommsDetails (CSQLRowAccountFull& RowAccount, CWebPaymentAtcTransactionFile* pFile )
{
	CString strLine = "";

	CString strFSM				= "false";								// assume not free school meal
	CString strTransactionType	= "";									// assume not reqd
	double dFullAmount			= pFile->GetTransactionValue();			// full amount charged for the meal
	double dChargeAmount		= dFullAmount;							// works out to be be FullAmount - FSMcontribution

	switch ( pFile->GetActionType() )
	{
	case nATCWEB_TOPUP:
	case nATCWEB_TOPUPREFUND:	
	case nATCWEB_SALEREFUND:
	case nATCWEB_TRANSFER:		strTransactionType = "TopUp";
								break;

	case nATCWEB_REFRESH:		if ( m_bIncludePurse2 == TRUE )				// see if Purse2 used
									strTransactionType = "TopUp";			// TransactionType (enum)
								break;


	case nATCWEB_SALE:			strTransactionType = "Sale";				// TransactionType
								dFullAmount = -(dFullAmount);				// sales are saved as -ve value
								dChargeAmount = -(dChargeAmount);			// sales are saved as -ve value

								if ( pFile->GetPurse2Value() < 0 )					// see if anything taken from Purse2
								{
									dChargeAmount += pFile->GetPurse2Value();	// reduce charged amount by FSM portion
									strFSM = "true";							// set whole or partially FSM transaction
								}
								break;
	}
	
	if ( strTransactionType != "" )
	{
		double dBalance = ( m_bIncludePurse1 == TRUE ) ? pFile->GetPurse1Balance() : 0;
		if ( m_bIncludePurse2 == TRUE )	dBalance += pFile->GetPurse2Balance();
		if ( m_bIncludePurse3 == TRUE ) dBalance += pFile->GetPurse3Balance();

		CString strTransactionID = NextReferenceNo ( pFile->GetShortCCNo() );	// transaction_id

		CCSV csv;
		csv.Add ( dChargeAmount, 2 );									// ChargedAmount 
		csv.Add ( dBalance, 2 );										// CurrentMemberBalance	
		csv.Add ( pFile->GetSalesText() );								// DescriptionOfPurchasedItem
		csv.Add ( strFSM );												// Free School Meal
		csv.Add ( dFullAmount, 2 );										// Full amount
		csv.Add ( strTransactionID );									// PartnerTransactionID
		csv.Add ( strTransactionID );									// ReceiptID
		csv.Add ( RowAccount.GetMemberID() );						// SIMSPersonID
		csv.Add ( pFile->GetXmlDate ( nWEBPAYMENT_SCHOOLCOMMS ) );		// TransactionDate
		csv.Add ( strTransactionType );									// TransactionType (enum) "Sale" / "Topup"

		strLine = csv.GetLine();
	}

	return strLine;
}

//*******************************************************************
// Service 1 - "student_id,description,value,transaction_id,balance,cart_date_time";
// Service 2 - "student_id,description,value,product_id,transaction_id,balance,cart_date_time";

void CWebPaymentPurchaseFile::SaveWisePayDetails (CSQLRowAccountFull& RowAccount, CWebPaymentAtcTransactionFile* pFile, CString& strService1, CString& strService2 )
{
	bool bSaveService1 = ( pFile->GetTransactionValue(m_nService1Purse) != 0 ) ? TRUE : FALSE;	// see if any value taken from diner purse1
	if (m_nService2Purse != 2)																// check service 2 is not using purse2 ( refresh purse )
	{
		if (pFile->GetTransactionValue(2) != 0)												// see if fresh purse used in conjunction with Diner Purse1
		{
			bSaveService1 = TRUE;																// will include purse2 value even if purse1 value was 0
		}
	}

	bool bSaveService2 = ( ( m_nService2Purse != 0 && pFile->GetTransactionValue(m_nService2Purse) ) != 0 ) ? TRUE : FALSE;

	double dTransactionValueService1 = 0;
	double dTransactionValueService2 = 0;

	if ( bSaveService1 == TRUE && bSaveService2 == TRUE )				// if both true - then split transaction value between diner service & service 2
	{
		dTransactionValueService2 = pFile->GetTransactionValue(m_nService2Purse);				// value deducted from service 2 purse
		dTransactionValueService1 = pFile->GetTransactionValue() - dTransactionValueService2;	// deduct what was taken from purse2 from total transaction value
	}
	else																// all taken from one service
	{
		if ( bSaveService1 == TRUE )	dTransactionValueService1 = pFile->GetTransactionValue();	// total transaction value from service1
		else							dTransactionValueService2 = pFile->GetTransactionValue();	// total transaction value from service2
	}


	if ( bSaveService1 == TRUE )										// standard diner transaction
	{
		CCSV csv;
		double dBalance = ( m_bIncludePurse1 == TRUE ) ? pFile->GetPurse1Balance() : 0;
		if ( m_bIncludePurse2 == TRUE )	dBalance += pFile->GetPurse2Balance();
		if ( m_bIncludePurse3 == TRUE ) dBalance += pFile->GetPurse3Balance();

		csv.Add ( RowAccount.GetMemberID() );				// student_id
		csv.Add ( pFile->GetSalesText() );						// description
		csv.Add ( dTransactionValueService1, 2 );				// value of transaction
		csv.Add ( NextReferenceNo ( pFile->GetShortCCNo()) );		// transaction_id
		csv.Add ( dBalance, 2 );
		csv.Add ( pFile->GetXmlDate(nWEBPAYMENT_WISEPAY ) );	// cart_date_time

		strService1 = csv.GetLine();
	}

	if ( bSaveService2 == TRUE )								// Service 2 transaction ( none diner service )
	{
		CCSV csv;
		double dBalance = 0;
		switch ( m_nService2Purse )
		{
		case 1: dBalance = pFile->GetPurse1Balance();	break;
		case 2: dBalance = pFile->GetPurse2Balance();	break;
		case 3: dBalance = pFile->GetPurse3Balance();	break;
		}

		csv.Add ( RowAccount.GetMemberID() );				// student_id
		csv.Add ( pFile->GetSalesText() );						// description
		csv.Add ( dTransactionValueService2, 2 );				// value of transaction of service 2
		csv.Add ( m_nService2ID );								// Service product_id
		csv.Add ( NextReferenceNo ( pFile->GetShortCCNo()) );	// transaction_id
		csv.Add ( dBalance, 2 );
		csv.Add ( pFile->GetXmlDate(nWEBPAYMENT_WISEPAY ) );	// cart_date_time

		strService2 = csv.GetLine();
	}

}

//**********************************************************************
// "SequenceNo,Date,Time,Type,Description,Purse1Value,Purse2Value,Purse3Value,CashValue,CCNo,Purse1Balance,Purse2Balance,Purse3Balance";

CString CWebPaymentPurchaseFile::SaveExportSalesDetails ( CSQLRowAccountFull& RowAccount, CWebPaymentAtcTransactionFile* pFile )
{
	CString strLine = "";
	CString strTransactionType	= "";									// assume not reqd

	switch ( pFile->GetActionType() )
	{
	case nATCWEB_TOPUP:			
	case nATCWEB_TOPUPREFUND:	
	case nATCWEB_REFRESH:
	case nATCWEB_TRANSFER:		strTransactionType = "TopUp";
								break;

	case nATCWEB_SALEREFUND:	
	case nATCWEB_SALE:			strTransactionType = "Sale";
								break;
	}
		
	if ( strTransactionType != "" )
	{
		CCSV csv;
		csv.Add ( NextReferenceNo() );							// unique incremental transactionNumber
		csv.Add ( pFile->GetDate() );							// Date
		csv.Add ( pFile->GetTime() );							// Time
		csv.Add ( strTransactionType );							// Sale \ Topup
		csv.Add ( pFile->GetSalesText() );						// description
		csv.Add ( pFile->GetPurse1Value(), 2 );					// Purse1 sale value
		csv.Add ( pFile->GetPurse2Value(), 2 );					// Purse2 sale value
		csv.Add ( pFile->GetPurse3Value(), 2 );					// Purse3 sale value
		csv.Add ( pFile->GetCashValue(), 2 );					// cash sale value
		csv.Add ( pFile->GetFullCCNo() );						// CCNO
		csv.Add ( pFile->GetPurse1Balance(), 2 );				// Purse1 balance
		csv.Add ( pFile->GetPurse2Balance(), 2 );				// Purse2 balance
		csv.Add ( pFile->GetPurse3Balance(), 2 );				// Purse3 balance

		if (m_strAdditionalHeader != "")
		{
			m_dbReporter.AppendDbFieldsToCSVLine(RowAccount,&csv);				// add database fields
		}
		
		strLine = csv.GetLine();
	}

	return strLine;
}

//**********************************************************************
// Log the new webupdates (Tnyyyymm \ Toyyyymm )- File has header & fields are in Header order

bool  CWebPaymentPurchaseFile::LogAtcUpdates ( const char* szNewUpdates, const char* szPrefix )
{
	CWebPaymentExportTnLog log ( m_nWebPaymentType, szPrefix );

	CString strLogFile	= log.GetCurrentLogName();						// ..\{WebProvider}}\Tnyyyymm.dat
	m_nExportLineCount = ::GetFileLineCount ( szNewUpdates ) - 1;		// number of lines less header line

	bool bReply = ::AppendDataFile ( szNewUpdates, strLogFile, TRUE );	// append updates & save in logfile in web based csv format

	log.AddDateStamp ( strLogFile, m_nExportLineCount );				// add date stamp line 'Sent : dd/mm/yy @ hh:mm ' ONLY if LineCount > 0

	return bReply;
}

//*******************************************************************
