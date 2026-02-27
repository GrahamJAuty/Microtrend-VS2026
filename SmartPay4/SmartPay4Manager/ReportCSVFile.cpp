/******************************************************************************/
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
/******************************************************************************/
#include "ReportCSVFile.h"
/******************************************************************************/
#define		nTYPE_STRING			0
#define		nTYPE_INT				1
#define		nTYPE_DOUBLE			2
#define		nTYPE_NOSHOW			3
#define		nTYPE_IMPLIEDDOUBLE		4
/******************************************************************************/

CCSVFileReport::CCSVFileReport(const char* szLabel, const char* szFilename, CWnd* pParent)
{
	m_strReportTitle = "";
	m_strFilename = szFilename;
	m_strReportLabel = szLabel;
	m_pParent = pParent;
	m_bHaveHeader = TRUE;
	m_nMode = P_NOWAIT;

	AddFieldType(m_arrayIntFields, Account::Points.Label);
	AddFieldType(m_arrayIntFields, Account::PointsToDate.Label);

	AddFieldType(m_arrayDoubleFields, Account::Purse1Liability.Label);
	AddFieldType(m_arrayDoubleFields, Account::Purse1Credit.Label);
	AddFieldType(m_arrayDoubleFields, Account::Purse1LastSpend.Label);
	AddFieldType(m_arrayDoubleFields, Account::Purse1SpendToDate.Label);
	AddFieldType(m_arrayDoubleFields, Account::Purse2Balance.Label);
	AddFieldType(m_arrayDoubleFields, Account::Purse2LastSpend.Label);
	AddFieldType(m_arrayDoubleFields, Account::Purse2SpendToDate.Label);
	AddFieldType(m_arrayDoubleFields, Account::Purse3Liability.Label);
	AddFieldType(m_arrayDoubleFields, Account::Purse3Credit.Label);
	AddFieldType(m_arrayDoubleFields, Account::Purse3LastSpend.Label);
	AddFieldType(m_arrayDoubleFields, Account::Purse3SpendToDate.Label);
	AddFieldType(m_arrayDoubleFields, Account::CashSpendToDate.Label);
	AddFieldType(m_arrayDoubleFields, "Cash");
	AddFieldType(m_arrayDoubleFields, Account::MaxSpendPurse1Period1.Label);
	AddFieldType(m_arrayDoubleFields, Account::MaxOverdraftPurse1Period1.Label);
	AddFieldType(m_arrayDoubleFields, "Purse1Value");
	AddFieldType(m_arrayDoubleFields, "Purse2Value");
	AddFieldType(m_arrayDoubleFields, "Purse3Value");
	AddFieldType(m_arrayDoubleFields, "CashValue");
	AddFieldType(m_arrayDoubleFields, "Purse1Balance");
	AddFieldType(m_arrayDoubleFields, "Purse2Balance");
	AddFieldType(m_arrayDoubleFields, "Purse3Balance");

	switch (System.GetWebPaymentType())
	{
	case nWEBPAYMENT_PARENTPAY:
		// ParentPay Export transaction  "connectingPOSTxnID,mealSessionID,txnAmount,description,accountID,txnDateTime,txnType,originatingPOSID,flags,newBalance,posInternalTxnID,financialTransactionType,vatRate";
		AddFieldType(m_arrayDoubleFields, "txnAmount");
		AddFieldType(m_arrayDoubleFields, "newBalance");
		AddFieldType(m_arrayDoubleFields, "Balance");
		AddFieldType(m_arrayIntFields, "TransType");
		AddFieldType(m_arrayIntFields, "ProductID");
		AddFieldType(m_arrayIntFields, "PurseType");
		AddFieldType(m_arrayIntFields, "ReceiptNumber");
		AddFieldType(m_arrayIntFields, "vatRate");
		break;

	case nWEBPAYMENT_SCHOOLCOMMS:
		// Schoolcomms Export Log Labels  "a:ChargedAmount,a:CurrentMemberBalance,a:DescriptionOfPurchasedItem,a:FreeSchoolMeal,a:FullAmount,a:PartnerTransactionID,a:ReceiptID,a:SIMSPersonID,a:TransactionDate,a:TransactionType";
		AddFieldType(m_arrayDoubleFields, "a:ChargedAmount");
		AddFieldType(m_arrayDoubleFields, "a:CurrentMemberBalance");
		AddFieldType(m_arrayDoubleFields, "a:FullAmount");
		AddFieldType(m_arrayNoShowFields, "a:FreeSchoolMeal");
		//AddFieldType(m_arrayNoShowFields, "a:PartnerTransactionID");
		break;

	case nWEBPAYMENT_TUCASI:
	case nWEBPAYMENT_TUCASIv2:
		// Tucasi (Soap) Export labels	"description,paymentAmount,paymentDate,pupilName,pupilNumber,transactionItemNumber,transactionNumber"
		// Tucasi (Json) Export	labels	"transactionNumber,transactionItemNumber,paymentDate,paymentAmount,description,balance,accountID,accountName";
		AddFieldType(m_arrayImpliedDoubleFields, "paymentAmount");
		AddFieldType(m_arrayNoShowFields, "transactionItemNumber");
		// Tucasi (Soap )Export balance labels "balance,pupilName,pupilNum"
		// Tucasi (Json) Export balance labels "balance,accountID,accountName"
		AddFieldType(m_arrayImpliedDoubleFields, "balance"); //value in pence
break;

	case nWEBPAYMENT_PARENTMAIL:
		// ParentMail "TransID,AccountID,SalesValue,FreeValue,TransDateTime,TransType,EPosID,ProductID,PurseType,VatRate,Balance,ReceiptNumber";
		AddFieldType(m_arrayDoubleFields, "SalesValue");
		AddFieldType(m_arrayDoubleFields, "FreeValue");
		AddFieldType(m_arrayDoubleFields, "Balance");
		AddFieldType(m_arrayDoubleFields, "VatRate");
		AddFieldType(m_arrayIntFields, "ReceiptNumber");
		AddFieldType(m_arrayIntFields, "TransType");
		AddFieldType(m_arrayIntFields, "ProductID");
		AddFieldType(m_arrayIntFields, "PurseType");
		break;

	case nWEBPAYMENT_SQUID:
		// sQuid amount,closingBalance,dateAndTime,description,merchantId,purseAccountNumber,rpu,rpuSequence,transactionNumber,transactionType
		AddFieldType(m_arrayImpliedDoubleFields, "amount");
		AddFieldType(m_arrayImpliedDoubleFields, "closingBalance");
		AddFieldType(m_arrayIntFields, "rpuSequence");
		AddFieldType(m_arrayIntFields, "transactionNumber");
		break;

	case nWEBPAYMENT_WISEPAY:
		// WisePay  "student_id,description,value,transaction_id,balance,cart_date_time";
		// WisePay  "student_id,description,value,product_id,transaction_id,balance,cart_date_time";
		// WisePay  "student_id,balance,balance2";
		AddFieldType(m_arrayIntFields, "quantity");
		AddFieldType(m_arrayIntFields, "transaction_id");
		AddFieldType(m_arrayIntFields, "product_id");
		AddFieldType(m_arrayDoubleFields, "value");
		AddFieldType(m_arrayDoubleFields, "balance");
		AddFieldType(m_arrayDoubleFields, "balance2");
		break;

	case nWEBPAYMENT_PEBBLE:
		AddFieldType(m_arrayStringFields, "MemberID", 700);
		AddFieldType(m_arrayStringFields, "Date", 450);
		AddFieldType(m_arrayStringFields, "SmartPay Name", 450);
		AddFieldType(m_arrayStringFields, "Comment", 450);
		AddFieldType(m_arrayDoubleFields, "Amount");
		break;
	}
}

//*******************************************************************

void CCSVFileReport::AddFieldType(CReportConsolidationArray<CSortedIntByString>& array, CString strFieldName, int nWidth)
{
	CSortedIntByString item;
	item.m_strItem = strFieldName;
	item.m_nItem = nWidth;
	array.Consolidate(item);
}

//*******************************************************************

void CCSVFileReport::Show(const char* szTitle)
{
	m_strReportTitle = szTitle;

	if (m_strReportTitle == "")
	{
		m_strReportTitle = "CSV File";
	}

	int nErrorNo = 0;
	CString strReportFile = Filenames.GetReportFilename(m_strReportLabel);
	CString strParams = Filenames.GetReportParamsFilename(m_strReportLabel);

	CRepmanHandler repman(m_strReportTitle, m_pParent);
	repman.SetMode(m_nMode);

	if ((nErrorNo = Create(strReportFile)) == nREPORT_NOERROR)
	{
		nErrorNo = repman.DisplayReport(strReportFile, strParams, m_strReportLabel);
	}

	repman.DisplayError(nErrorNo);
}

//*******************************************************************

int CCSVFileReport::Create ( const char* szReportFile )
{
	CSSFile csvFile;
	if (csvFile.Open(m_strFilename, "rb") == FALSE)
	{
		return nREPORT_NOSOURCE;
	}

	if (m_Report.Open(szReportFile) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

// read csv file 

	CCSV csvLine;
	if (csvFile.Read(csvLine) == FALSE)			// get first line
	{
		return nREPORT_INVALIDSOURCE;
	}

	m_Report.SetStyle1 ( m_strReportTitle );

// scan header to create report header

	CUIntArray aiType;
	int nMaxFieldCount	= 0;
	int nCardNoIndex	= -1;

	if ( m_bHaveHeader == FALSE )						// no header, so first line is NOT a header
	{
		CUIntArray aiCharCount;

		nMaxFieldCount = csvLine.GetSize();				// get number of fields in first line
		for ( int i = 0 ; i < nMaxFieldCount ; i++ )
		{
			CString strField = csvLine.GetString(i);
			aiCharCount.Add ( strField.GetLength() );
		}

		while ( csvFile.Read ( csvLine ) == TRUE )		// second & subsequent lines
		{
			int nFieldCount = csvLine.GetSize();

			if (nFieldCount > nMaxFieldCount)
			{
				nMaxFieldCount = nFieldCount;			// inc max number of columns so far
			}

			for (int i = 0; i < nFieldCount; i++)
			{
				CString strField = csvLine.GetString(i);
				int nLength = strField.GetLength();

				if (i <= aiCharCount.GetSize())
				{
					if (nLength > (int)aiCharCount.GetAt(i))
					{
						aiCharCount.SetAt(i, nLength);	// new max length
					}
				}
				else
				{
					aiCharCount.Add(nLength);
				}
			}
		}

		for ( int i = 0 ; i < nMaxFieldCount ; i++ )
		{
			int nFieldWidth = 300;

			if (aiCharCount.GetSize() <= nMaxFieldCount)
			{
				nFieldWidth = aiCharCount.GetAt(i) * 25;
				if (nFieldWidth > 3000)
				{
					nFieldWidth = 3000;
				}
			}

			m_Report.AddColumn ( "", TA_LEFT, nFieldWidth );	// make up blank column headings
			aiType.Add ( nTYPE_STRING ); 
		}

		csvFile.Seek ( 0L, SEEK_SET );					// back to start of file
	}
	else												// have a header line
	{
		//FIXED HEADER LINE FOR BROMCOM SENT TRANSACTION FILE
		if (m_strReportLabel == "R231")
		{
			csvLine.RemoveAll();
			csvLine.Add(BromComTransaction::PersonID.Label);
			csvLine.Add(BromComTransaction::Amount.Label);
			csvLine.Add(BromComTransaction::Balance.Label);
			csvLine.Add(BromComTransaction::DateTime.Label);
			csvLine.Add(BromComTransaction::Type.Label);
			csvLine.Add(BromComTransaction::TransactionID.Label);
			csvLine.Add(BromComTransaction::WebComment.Label);
			csvFile.Seek(0L, SEEK_SET);
		}

		nMaxFieldCount = csvLine.GetSize();

		for ( int i = 0 ; i < nMaxFieldCount ; i++ )
		{
			CString strLabel = csvLine.GetString(i);

			int nType = 0;
			int nWidth = 0;
			GetFieldType ( strLabel, nType, nWidth );
			
			aiType.Add ( nType );

			int nAlignment = TA_LEFT;

			switch (nType)
			{
			case nTYPE_NOSHOW:
				continue;

			case nTYPE_STRING:
				if (nWidth == 0)
				{
					nWidth = 350;
				}
				nAlignment = TA_LEFT;
				break;
			
			default:
				if (nWidth == 0)
				{
					nWidth = 300;
				}
				nAlignment = TA_RIGHT;
				break;
			}

			m_Report.AddColumn ( strLabel, nAlignment, nWidth);

			if ( nCardNoIndex == -1 )
			{
				if (strLabel == Account::UserID.Label)
				{
					nCardNoIndex = i;
				}
			}
		}
	}

// add lines

	while ( csvFile.Read ( csvLine ) == TRUE )
	{
		if (csvLine.GetSize() < 2)
		{
			m_Report.WriteLine(csvLine.GetString(0));
		}
		else
		{
			CCSV csvOut ( '\t' );
			for ( int i = 0 ; i < nMaxFieldCount ; i++ )
			{
				int nType = aiType.GetAt(i);
				if (nType == nTYPE_NOSHOW)
				{
					continue;
				}

				if (csvLine.IsEmpty(i) == TRUE)
				{
					csvOut.Add("");
				}
				else
				{
					CString strField = csvLine.GetString(i);
					if ( nCardNoIndex == i )					// see if CardNo column
					{
						CSQLRepositoryAccount RepoAccount;
						if (RepoAccount.RowExists(_atoi64(strField), NULL).GetSQLResult() == 0 )
						{
							strField += " *";
						}
					}

					double dValue = atof(strField);
					switch ( nType )
					{
					case nTYPE_STRING:	
						csvOut.Add ( strField );
						break;

					case nTYPE_INT:	
						csvOut.Add ( dValue, 0 );
						break;

					case nTYPE_DOUBLE:	
						csvOut.Add ( dValue, System.GetDPValue() );	
						break;

					case nTYPE_IMPLIEDDOUBLE:
						csvOut.Add ( dValue / 100, System.GetDPValue() );
						break;
					}
				}
			}	
			m_Report.WriteLine ( csvOut );
		}
	}
	m_Report.Close();

	return nREPORT_NOERROR;
}

//**********************************************************************

void CCSVFileReport::GetFieldType(CString strLabel, int& nType, int& nWidth )
{
	nType = nTYPE_STRING;
	nWidth = 0;

	CSortedIntByString Lookup;
	Lookup.m_strItem = strLabel;
	Lookup.m_nItem = 0;

	int nPos = 0;
	if (m_arrayDoubleFields.Find(Lookup, nPos) == TRUE)
	{
		m_arrayDoubleFields.GetAt(nPos, Lookup);
		nType = nTYPE_DOUBLE;
	}
	else if (m_arrayImpliedDoubleFields.Find(Lookup, nPos) == TRUE)
	{
		m_arrayImpliedDoubleFields.GetAt(nPos, Lookup);
		nType = nTYPE_IMPLIEDDOUBLE;
	}
	else if (m_arrayIntFields.Find(Lookup, nPos) == TRUE)
	{
		m_arrayIntFields.GetAt(nPos, Lookup);
		nType = nTYPE_INT;
	}
	else if (m_arrayStringFields.Find(Lookup, nPos) == TRUE)
	{
		m_arrayStringFields.GetAt(nPos, Lookup);
		nType = nTYPE_STRING;
	}
	else if (m_arrayNoShowFields.Find(Lookup, nPos) == TRUE)
	{
		m_arrayNoShowFields.GetAt(nPos, Lookup);
		nType = nTYPE_NOSHOW;
	}

	nWidth = Lookup.m_nItem;
}

//**********************************************************************
