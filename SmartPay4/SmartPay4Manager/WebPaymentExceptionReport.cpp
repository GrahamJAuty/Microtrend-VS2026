//*******************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
//*******************************************************************
#include "WebPaymentExceptionReport.h"
//*******************************************************************

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//*******************************************************************

//WisePay		"uID , cart_student_sims_id , cart_student_name , cart_timestamp , price_value														Pounds
//ParentPay		"PaymentID , accountID , rollNumber , UPN , MISID , txnAmount , PayerInfo , PaidDate , ReceiptCode, paymentTypeCode , ConsumerName	Pounds
//Tucasi		"transactionNumber , transactionItemNumber , paymentDate , paymentAmount , description , pupilNumber , pupilName , schoolID			Pence
//Schoolcomms	a:BatchID , a:AccountPaymentID , a:PaymentAmount , a:PaymentDate , a:SIMSPersonI"													Pounds
//Tucasiv2		transactionNumber , transactionItemNumber , paymentDate , paymentAmount , description , balance , accountID , accountName			Pence
//sQuid			operation , purseAccountNumber , rpuID , sequenceNumber , value																		Pence
//ParentMail	PaymentID , AccountID , PurseType , Value , PaymentType , PaymentDate																Pounds

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWebPaymentExceptionReport::CWebPaymentExceptionReport ( const char* szLabel, const char* szFilename, CWnd* pParent )
	: m_strReportLabel(szLabel)
	, m_strFilename(szFilename)
	, m_pParent(pParent)
	, m_dTotalValue(0)
	, m_bValueInPence(FALSE)
{
	m_strReportTitle.Format ( "%s Web Payment Outstanding Exceptions", System.GetWebPaymentDisplayName() );

	if ( System.GetWebPaymentType() == nWEBPAYMENT_PARENTPAY )
	{
		m_nSortColumnIndex	= 1;			// accountID
		m_nValueColumnIndex	= 5;			// txnAmount
	}
	
	else if ( System.GetWebPaymentType() == nWEBPAYMENT_TUCASI )
	{
		m_nSortColumnIndex	= 5;			// pupilNumber
		m_nValueColumnIndex = 3;			//paymentAmount
		m_bValueInPence		= TRUE;
	}

	else if ( System.GetWebPaymentType() ==  nWEBPAYMENT_WISEPAY )
	{
		m_nSortColumnIndex	= 1;			// cart_student_sims_id
		m_nValueColumnIndex = 4;			// price_value
	}

	else if ( System.GetWebPaymentType() == nWEBPAYMENT_SCHOOLCOMMS )
	{
		m_nSortColumnIndex	= 4;			// a:SIMSPersonID
		m_nValueColumnIndex = 2;			// value
	}

	else if ( System.GetWebPaymentType() == nWEBPAYMENT_TUCASIv2 )
	{
		m_nSortColumnIndex	= 6;			// accountID
		m_nValueColumnIndex = 3;			// paymentAmount
		m_bValueInPence		= TRUE;
	}

	else if ( System.GetWebPaymentType() == nWEBPAYMENT_SQUID )
	{
		m_nSortColumnIndex	= 1;			// purseAccountNumbe
		m_nValueColumnIndex = 4;			// a:PaymentAmount
		m_bValueInPence		= TRUE;
	}

	else if ( System.GetWebPaymentType() == nWEBPAYMENT_PARENTMAIL )
	{
		m_nSortColumnIndex	= 1;			// AccountID
		m_nValueColumnIndex = 3;			// Value
	}

	else if (System.GetWebPaymentType() == nWEBPAYMENT_BROMCOM)
	{
		m_nSortColumnIndex = 0;			// StudentID
		m_nValueColumnIndex = 1;		// Value
	}
}

//*******************************************************************

void CWebPaymentExceptionReport::Show()
{
	int nErrorNo;
	CString strReportFile = Filenames.GetReportFilename ( m_strReportLabel );
	CString strParamsFile = Filenames.GetReportParamsFilename ( m_strReportLabel );

	CRepmanHandler repman ( m_strReportTitle, m_pParent );

	if ( ( nErrorNo = Create ( strReportFile ) ) == nREPORT_NOERROR )
		nErrorNo = repman.DisplayReport ( strReportFile, strParamsFile, m_strReportLabel );
	
	repman.DisplayError ( nErrorNo );
}

//*******************************************************************

int CWebPaymentExceptionReport::Create ( const char* szReportFile )
{
	if ( m_dbPayments.OpenSSDatabaseReadOnly ( m_strFilename, 10 ) != DB_ERR_NONE )
		return nREPORT_NOSOURCE;

	if ( m_Report.Open ( szReportFile ) == FALSE )
		return nREPORT_CREATEFAIL;

	bool bCancelButton = FALSE;

	m_Report.SetStyle1 ( m_strReportTitle );
	AddColumnText();
		
	CString strSortLabel = m_dbPayments.GetHeaderLabel ( m_nSortColumnIndex );

	m_dbPayments.ClearSort();
	m_dbPayments.AddSort ( strSortLabel, DB_SORT_TEXT_NOCASE, DB_SORT_ASCENDING );	// sort on UserID
	m_dbPayments.Sort();

	if ( m_dbPayments.GetRecordCount() > 0 )
	{
		m_dbPayments.MoveFirst();
		do
		{
			SaveLine();
		}
		while ( m_dbPayments.MoveNext() == TRUE );

		ShowTotalValue();
	}

	m_dbPayments.Close();
	m_Report.Close();

	return nREPORT_NOERROR;
}

//**********************************************************************

void CWebPaymentExceptionReport::AddColumnText()
{
	for ( int i = 0 ; i < m_dbPayments.GetHeaderSize() ; i++ )
	{
		int nAlignment = ( i == m_nValueColumnIndex ) ? TA_RIGHT : TA_LEFT;

		m_Report.AddColumn ( m_dbPayments.GetHeaderLabel(i), nAlignment, 300 );
	}
}

//**********************************************************************

void CWebPaymentExceptionReport::SaveLine()
{
	CCSV csv ( '\t' );

	for ( int i = 0 ; i < m_dbPayments.GetHeaderSize() ; i++ )
	{
		CString strLabel = m_dbPayments.GetHeaderLabel(i);

		if ( i == m_nValueColumnIndex )
		{
			double dValue = m_dbPayments.GetDouble( strLabel );
			if ( m_bValueInPence == TRUE )						// value in pence
				dValue /= 100;									// conver to Pounds\Pence

			csv.Add ( dValue, System.GetDPValue() );
			m_dTotalValue += dValue;
		}
		else
			csv.Add ( m_dbPayments.GetString ( strLabel ) );
	}

	m_Report.WriteLine ( csv );
}

//**********************************************************************

void CWebPaymentExceptionReport::ShowTotalValue()
{
	CCSV csv ( '\t' );
	CCSV csvTabLine  ( '\t' );

	for ( int i = 0 ; i < m_dbPayments.GetHeaderSize() ; i++ )
	{
		if ( i == m_nValueColumnIndex )
		{
			csvTabLine.Add ( "<LI>" );
			csv.Add ( m_dTotalValue, System.GetDPValue() );
			break;
		}

		csvTabLine.Add ( "" );
		csv.Add ( "" );
	}

	m_Report.WriteLine ( csvTabLine );
	m_Report.WriteLine ( csv );
}

//**********************************************************************
