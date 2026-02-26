/**********************************************************************/
#include "SPOSVersions.h"
#include "NetworkCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PaymentCSVArray.h"
/**********************************************************************/

CPaymentCSVRecord::CPaymentCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CPaymentCSVRecord::ClearRecord()
{
	m_strEPOSName = "";
	m_strReportName = "";
	m_nPaymentNo = 0;
	m_nPaymentType = PAYMENT_TYPE_CASH;
	m_nReportGroup = 1;
	m_dLimit = 0.0;
	m_bTender = TRUE;
	m_bOverTender = FALSE;
	m_bChange = TRUE;
	m_bGratuity = FALSE;
	m_bDrawer = FALSE;
	m_bEFTCashback = FALSE;
	m_bNonBnk = FALSE;
	m_bReceipt = TRUE;
	m_bAccountAllow = TRUE;
	m_bCashDeclaration = TRUE;
	m_bHandheld = TRUE;
	m_bChangeAsCashback = FALSE;
	m_nFunctionNo = 18;
	m_nForexNo = 0;
	m_bCashLimit = FALSE;
	m_nDepositChangeType = 0;
	m_nDepositChangeVoucherMin = 0;
	m_strExternalParams = "";
	m_arrayPaymentTexts.RemoveAll();
}

/**********************************************************************/

void CPaymentCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetPaymentNo( csv.GetInt(0) );
	
	switch ( csv.GetInt(1) )
	{
	case 1:		V1ConvertFromCSV ( csv );			break;
	case 2:		V2ConvertFromCSV ( csv );			break;
	case 3:		V3ConvertFromCSV ( csv );			break;
	case 4:		V4To12ConvertFromCSV ( csv, 4 );	break;
	case 5:		V4To12ConvertFromCSV ( csv, 5 );	break;
	case 6:		V4To12ConvertFromCSV ( csv, 6 );	break;
	case 7:		V4To12ConvertFromCSV ( csv, 7 );	break;
	case 8:		V4To12ConvertFromCSV ( csv, 8 );	break;
	case 9:		V4To12ConvertFromCSV ( csv, 9 );	break;
	case 10:	V4To12ConvertFromCSV ( csv, 10 );	break;
	case 11:	V4To12ConvertFromCSV ( csv, 11 );	break;
	case 12:	V4To12ConvertFromCSV ( csv, 12 );	break;
	}
}

/**********************************************************************/

void CPaymentCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetEPOSName		( csv.GetString(2) );
	SetLimit		( (double) csv.GetInt(3) / 100.0  );
	SetTender		( csv.GetBool(4) );
	SetOverTender	( csv.GetBool(5) );
	SetChange		( csv.GetBool(6) );
	SetGratuity		( csv.GetBool(7) );
	SetDrawer		( csv.GetBool(8) );
	
	switch( csv.GetInt(9) )
	{
	case 0:	//NO EFT
		SetPaymentType( PAYMENT_TYPE_CASH );
		SetEFTCashback( FALSE );
		break;

	case 1:	//EFT NO CASHBACK
	case 2:	//EFT WITH CASHBACK
		SetPaymentType( PAYMENT_TYPE_EFT );
		SetEFTCashback( csv.GetInt(9) == 2 );
		break;
	}
	
	SetNonBnk		( csv.GetBool(10) );
}

/**********************************************************************/

void CPaymentCSVRecord::V2ConvertFromCSV ( CCSV& csv )
{
	SetEPOSName		( csv.GetString(2) );
	SetLimit		( csv.GetDouble(3) );
	SetTender		( csv.GetBool(4) );
	SetOverTender	( csv.GetBool(5) );
	SetChange		( csv.GetBool(6) );
	SetGratuity		( csv.GetBool(7) );
	SetDrawer		( csv.GetBool(8) );

	switch( csv.GetInt(9) )
	{
	case 0:	//NO EFT
		SetPaymentType( PAYMENT_TYPE_CASH );
		SetEFTCashback( FALSE );
		break;

	case 1:	//EFT NO CASHBACK
	case 2:	//EFT WITH CASHBACK
		SetPaymentType( PAYMENT_TYPE_EFT );
		SetEFTCashback( csv.GetInt(9) == 2 );
		break;
	}
	
	SetNonBnk		( csv.GetBool(10) );
}

/**********************************************************************/

void CPaymentCSVRecord::V3ConvertFromCSV(CCSV& csv)
{
	SetEPOSName(csv.GetString(2));
	SetLimit(csv.GetDouble(3));
	SetTender(csv.GetBool(4));
	SetOverTender(csv.GetBool(5));
	SetChange(csv.GetBool(6));
	SetGratuity(csv.GetBool(7));
	SetDrawer(csv.GetBool(8));

	switch (csv.GetInt(9))
	{
	case 0:	//NO EFT
		SetPaymentType(PAYMENT_TYPE_CASH);
		SetEFTCashback(FALSE);
		break;

	case 1:	//EFT NO CASHBACK
	case 2:	//EFT CASHBACK
		if (csv.GetBool(12) == FALSE)
		{
			SetPaymentType(PAYMENT_TYPE_EFT);
		}
		else
		{
			SetPaymentType(PAYMENT_TYPE_EFT_CNP);
		}

		SetEFTCashback(csv.GetInt(9) == 2);

		break;
	}

	SetNonBnk(csv.GetBool(10));
	SetReceipt(csv.GetBool(11));

	int nCount = csv.GetInt(14);
	if (nCount > 20)
	{
		nCount = 20;
	}

	for (int n = 0; n < nCount; n++)
	{
		CString strText = csv.GetString(15 + n);
		if (strText.GetLength() > Payment::PaymentText1.Max)
		{
			strText = strText.Left(Payment::PaymentText1.Max);
		}
		m_arrayPaymentTexts.Add(strText);
	}
}

/**********************************************************************/

void CPaymentCSVRecord::V4To12ConvertFromCSV(CCSV& csv, int nVer)
{
	SetEPOSName(csv.GetString(2));

	int nOffset = 3;

	if (nVer >= 11)
	{
		SetReportName(csv.GetString(nOffset++));
	}

	SetPaymentType(csv.GetInt(nOffset++));

	if (nVer >= 10)
	{
		SetReportGroup(csv.GetInt(nOffset++));
	}

	SetLimit(csv.GetDouble(nOffset++));
	SetTender(csv.GetBool(nOffset++));
	SetOverTender(csv.GetBool(nOffset++));
	SetChange(csv.GetBool(nOffset++));
	SetGratuity(csv.GetBool(nOffset++));
	SetDrawer(csv.GetBool(nOffset++));
	SetEFTCashback(csv.GetBool(nOffset++));
	SetNonBnk(csv.GetBool(nOffset++));
	SetReceipt(csv.GetBool(nOffset++));

	if (nVer >= 8)
	{
		SetAccountAllow(csv.GetBool(nOffset++));
	}

	if (nVer >= 9)
	{
		SetCashDeclarationFlag(csv.GetBool(nOffset++));
	}

	if (nVer >= 12)
	{
		SetChangeAsCashbackFlag(csv.GetBool(nOffset++));
	}

	SetHandheld(csv.GetBool(nOffset++));
	SetFunctionNo(csv.GetInt(nOffset++));
	SetForexNo(csv.GetInt(nOffset++));

	if (nVer >= 5)
	{
		SetCashLimitFlag(csv.GetBool(nOffset++));
	}

	if (nVer >= 6)
	{
		SetDepositChangeType(csv.GetInt(nOffset++));
		SetDepositChangeVoucherMin(csv.GetInt(nOffset++));
	}

	if (nVer >= 7)
	{
		SetExternalParams(csv.GetString(nOffset++));
	}

	int nCount = csv.GetInt(nOffset++);
	if (nCount > 20)
	{
		nCount = 20;
	}

	for (int n = 0; n < nCount; n++)
	{
		CString strText = csv.GetString(nOffset++);
		if (strText.GetLength() > Payment::PaymentText1.Max)
		{
			strText = strText.Left(Payment::PaymentText1.Max);
		}
		m_arrayPaymentTexts.Add(strText);
	}
}

/**********************************************************************/
	
void CPaymentCSVRecord::ConvertToCSV(CCSV& csv)
{
	csv.Add(m_nPaymentNo);
	csv.Add(PAYMENT_VERSION);
	csv.Add(m_strEPOSName);
	csv.Add(m_strReportName);
	csv.Add(m_nPaymentType);
	csv.Add(m_nReportGroup);
	csv.Add(m_dLimit, SysInfo.GetDPValue());
	csv.Add(m_bTender);
	csv.Add(m_bOverTender);
	csv.Add(m_bChange);
	csv.Add(m_bGratuity);
	csv.Add(m_bDrawer);
	csv.Add(m_bEFTCashback);
	csv.Add(m_bNonBnk);
	csv.Add(m_bReceipt);
	csv.Add(m_bAccountAllow);
	csv.Add(m_bCashDeclaration);
	csv.Add(m_bChangeAsCashback);
	csv.Add(m_bHandheld);
	csv.Add(m_nFunctionNo);
	csv.Add(m_nForexNo);
	csv.Add(m_bCashLimit);
	csv.Add(m_nDepositChangeType);
	csv.Add(m_nDepositChangeVoucherMin);
	csv.Add(m_strExternalParams);

	int nCount = m_arrayPaymentTexts.GetSize();
	if (nCount > 20)
	{
		nCount = 20;
	}

	csv.Add(nCount);
	for (int n = 0; n < nCount; n++)
	{
		csv.Add(m_arrayPaymentTexts.GetAt(n));
	}
}

/**********************************************************************/

void CPaymentCSVRecord::SetPaymentNo( int n )
{
	Set ( m_nPaymentNo, n, Payment::PaymentNo );
}

/**********************************************************************/

void CPaymentCSVRecord::SetLimit( double d )
{
	Set ( m_dLimit, d, Payment::Limit );
}

/**********************************************************************/

void CPaymentCSVRecord::SetEPOSName ( const char* szName )
{
	CString strName = szName;
	::TrimSpaces ( strName, FALSE );
	Set( m_strEPOSName, strName, Payment::PaymentEPOSName );
}

/**********************************************************************/

void CPaymentCSVRecord::SetReportName ( const char* szName )
{
	CString strName = szName;
	::TrimSpaces ( strName, FALSE );
	Set( m_strReportName, strName, Payment::PaymentReportName );
}

/**********************************************************************/

void CPaymentCSVRecord::SetPaymentType( int n )
{
	//PAYMENT NUMBER 1 IS ALWAYS TYPE ZERO (CASH)
	if ( m_nPaymentNo != 1 )
	{
		switch( n )
		{
		case PAYMENT_TYPE_CASH:
		case PAYMENT_TYPE_CHEQUE:
		case PAYMENT_TYPE_EFT:
		case PAYMENT_TYPE_EFT_CNP:
		case PAYMENT_TYPE_EFT_SHARE:
		case PAYMENT_TYPE_ROOM_AUTO:
		case PAYMENT_TYPE_ROOM_MANUAL:
		case PAYMENT_TYPE_LOYALTY:
		case PAYMENT_TYPE_CURRENCY:
		case PAYMENT_TYPE_DEPOSIT:
		case PAYMENT_TYPE_ACCOUNT:
		case PAYMENT_TYPE_OTHER:
		case PAYMENT_TYPE_CREDITNOTE:
		case PAYMENT_TYPE_GIFTCARD:
		case PAYMENT_TYPE_EXTERNAL_APP:
		case PAYMENT_TYPE_EXTERNAL_PAY:
		case PAYMENT_TYPE_VOUCHERCART:
			m_nPaymentType = n;
			break;
		}
	}
}

/**********************************************************************/

void CPaymentCSVRecord::SetReportGroup( int n )
{
	Set ( m_nReportGroup, n, Payment::ReportGroup );
}

/**********************************************************************/

void CPaymentCSVRecord::SetFunctionNo( int n )
{
	Set ( m_nFunctionNo, n, Payment::FunctionNo );
}

/**********************************************************************/

void CPaymentCSVRecord::SetForexNo( int n )
{
	Set ( m_nForexNo, n, Payment::ForexNo );
}

/**********************************************************************/

void CPaymentCSVRecord::SetDepositChangeType( int n )
{
	Set( m_nDepositChangeType, n, Payment::DepositChangeType );
}

/**********************************************************************/

void CPaymentCSVRecord::SetDepositChangeVoucherMin( int n )
{
	Set( m_nDepositChangeVoucherMin, n, Payment::DepositChangeVoucherMin );
}

/**********************************************************************/

void CPaymentCSVRecord::SetExternalParams ( const char* sz )
{
	CString strParams = sz;
	::TrimSpaces ( strParams, FALSE );
	Set( m_strExternalParams, strParams, Payment::ExternalParams );
}

/**********************************************************************/

const char* CPaymentCSVRecord::GetDisplayName()
{
	CString strTemp = m_strReportName;
	::TrimSpaces(strTemp, FALSE);

	if (strTemp == "")
	{
		strTemp = m_strEPOSName;
		::TrimSpaces(strTemp, FALSE);
	}

	if (strTemp == "")
	{
		m_strDisplayName.Format("Payment %d", m_nPaymentNo);
	}
	else
	{
		m_strDisplayName = strTemp;
	}

	return m_strDisplayName;
}

/**********************************************************************/

const char* CPaymentCSVRecord::GetDisplayType()
{
	switch( m_nPaymentType )
	{
	case PAYMENT_TYPE_CASH:			m_strDisplayType = "Standard Cash"; break;
	case PAYMENT_TYPE_CHEQUE:		m_strDisplayType = "Cheque"; break;
	case PAYMENT_TYPE_EFT:			m_strDisplayType = "EFT"; break;
	case PAYMENT_TYPE_EFT_CNP:		m_strDisplayType = "EFT Customer Not Present"; break;
	case PAYMENT_TYPE_EFT_SHARE:	m_strDisplayType = "EFT Shared (Lodging)"; break;
	case PAYMENT_TYPE_ROOM_AUTO:	m_strDisplayType = "Post to Room (Auto)"; break;
	case PAYMENT_TYPE_ROOM_MANUAL:	m_strDisplayType = "Post to Room (Manual)"; break;
	case PAYMENT_TYPE_LOYALTY:		m_strDisplayType = "Loyalty Points"; break;
	case PAYMENT_TYPE_CURRENCY:		m_strDisplayType = "Currency"; break;
	case PAYMENT_TYPE_DEPOSIT:		m_strDisplayType = "From Deposit"; break;
	case PAYMENT_TYPE_ACCOUNT:		m_strDisplayType = "Charge to Customer A/C"; break;
	case PAYMENT_TYPE_OTHER:		m_strDisplayType = "Other"; break;
	case PAYMENT_TYPE_CREDITNOTE:	m_strDisplayType = "Issue credit note"; break;
	case PAYMENT_TYPE_GIFTCARD:		m_strDisplayType = "Gift card"; break;
	case PAYMENT_TYPE_EXTERNAL_APP:	m_strDisplayType = "External Application"; break;
	case PAYMENT_TYPE_EXTERNAL_PAY:	m_strDisplayType = "SharpPOS External Payment"; break;
	case PAYMENT_TYPE_VOUCHERCART:	m_strDisplayType = "Voucher Cart"; break;
	default:						m_strDisplayType = "Unknown"; break;
	}

	return m_strDisplayType;
}

/**********************************************************************/

void CPaymentCSVRecord::GetPaymentTexts(CStringArray& arrayTexts)
{
	arrayTexts.RemoveAll();
	for (int n = 0; n < m_arrayPaymentTexts.GetSize(); n++)
	{
		arrayTexts.Add(m_arrayPaymentTexts.GetAt(n));
	}
}

/**********************************************************************/

void CPaymentCSVRecord::SetPaymentTexts(CStringArray& arrayTexts)
{
	m_arrayPaymentTexts.RemoveAll();

	int nCount = arrayTexts.GetSize();
	if (nCount > 20)
	{
		nCount = 20;
	}

	for (int n = 0; n < nCount; n++)
	{
		m_arrayPaymentTexts.Add(arrayTexts.GetAt(n));
	}
}

/**********************************************************************/

void CPaymentCSVRecord::ImportSQLText(const char* szText)
{
	m_arrayPaymentTexts.RemoveAll();

	CString strText = szText;
	CString strCRLF = "\x0D\x0A";

	int nLineStart = 0;
	CWordArray arrayLineBreaks;

	int nPos = strText.Find(strCRLF, nLineStart);
	while ((nPos != -1) && (nLineStart < strText.GetLength() - 2))
	{
		arrayLineBreaks.Add(nPos);
		nLineStart = nPos + 2;
		nPos = strText.Find(strCRLF, nLineStart);
	}

	nLineStart = 0;
	for (int n = 0; n < arrayLineBreaks.GetSize(); n++)
	{
		int nLineEnd = arrayLineBreaks.GetAt(n);

		if (nLineStart <= nLineEnd)
		{
			m_arrayPaymentTexts.Add(strText.Mid(nLineStart, (nLineEnd - nLineStart)));
		}

		if (m_arrayPaymentTexts.GetSize() >= 20)
		{
			break;
		}

		nLineStart = nLineEnd + 2;
	}

	if (m_arrayPaymentTexts.GetSize() < 20)
	{
		if (nLineStart < strText.GetLength())
		{
			m_arrayPaymentTexts.Add(strText.Right(strText.GetLength() - nLineStart));
		}
	}
}

/**********************************************************************/

const char* CPaymentCSVRecord::GetSQLText()
{
	m_strSQLText = "";
	for (int n = 0; n < m_arrayPaymentTexts.GetSize(); n++)
	{
		m_strSQLText += m_arrayPaymentTexts.GetAt(n);

		if (n < m_arrayPaymentTexts.GetSize() - 1)
		{
			m_strSQLText += "\x0D\x0A";
		}
	}

	return m_strSQLText;
}

/**********************************************************************/

void CPaymentCSVRecord::CheckMinimumSPOSVersion( CMinimumSPOSVersion& version )
{
	version.m_nMinVer = SPOS_V4;
	version.m_strReason = "";
	version.m_bInvalidData = FALSE;

	switch( GetPaymentType() )
	{
	case PAYMENT_TYPE_DEPOSIT:		
		version.m_strReason = "Payment Type Deposit";
		break;

	case PAYMENT_TYPE_OTHER:		
		version.m_strReason = "Payment Type Other";
		break;

	case PAYMENT_TYPE_EFT_SHARE:	
		version.m_nMinVer = SPOS_V4_321;
		version.m_strReason = "Payment Type EFT Share";
		break;

	case PAYMENT_TYPE_ACCOUNT:		
		version.m_nMinVer = SPOS_V4_837;
		version.m_strReason = "Payment Type Account";
		break;

	case PAYMENT_TYPE_CREDITNOTE:
		version.m_nMinVer = SPOS_V4_992;
		version.m_strReason = "Payment Type Credit Note";
		break;

	case PAYMENT_TYPE_GIFTCARD:		
		version.m_nMinVer = SPOS_V4_992;
		version.m_strReason = "Payment Type Gift Card";
		break;

	case PAYMENT_TYPE_EXTERNAL_APP:	
		version.m_nMinVer = SPOS_V4_1300;
		version.m_strReason = "Payment Type External App";
		break;

	case PAYMENT_TYPE_EXTERNAL_PAY:	
		version.m_nMinVer = SPOS_V4_2_959;
		version.m_strReason = "Payment Type SharpPOS External";
		break;

	case PAYMENT_TYPE_VOUCHERCART:
		version.m_nMinVer = SPOS_V4_3_93;
		version.m_strReason = "Payment Type Voucher Cart";
		break;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPaymentCSVArray::CPaymentCSVArray() 
{
}

/**********************************************************************/

CPaymentCSVArray::~CPaymentCSVArray() 
{
}

/**********************************************************************/

int CPaymentCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if (nReply != DB_ERR_SHARE)
	{
		if (nMode == DB_READONLY)	// if file does not exist use default values
		{
			nReply = DB_ERR_NONE;
		}
	}

	int nLastPaymentNo = 0;
	int nValidRecords = 0;
	
	CPaymentCSVRecord PaymentRecord;
	for (; nValidRecords < GetSize(); nValidRecords++)
	{
		GetAt(nValidRecords, PaymentRecord);

		int nPaymentNo = PaymentRecord.GetPaymentNo();
		if (nPaymentNo <= nLastPaymentNo || nPaymentNo > Payment::PaymentNo.Max)
		{
			break;
		}

		nLastPaymentNo = nPaymentNo;
	}

	while (GetSize() > nValidRecords)
	{
		RemoveAt(GetSize() - 1);
	}

	if (SysInfo.IsPMSSystem() == TRUE)
	{
		SetPMSPaymentFlags();
	}
	
	return nReply;
}

/**********************************************************************/

bool CPaymentCSVArray::FindPaymentByNumber(int nPaymentNo, int& nIndex)
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while (nStart <= nEnd)
	{
		nIndex = (nStart + nEnd) / 2;

		CPaymentCSVRecord Payment;
		GetAt(nIndex, Payment);
		int nArrayNo = Payment.GetPaymentNo();

		if (nPaymentNo == nArrayNo)
		{
			return TRUE;
		}

		if (nPaymentNo < nArrayNo)
		{
			nEnd = nIndex - 1;
		}
		else
		{
			nStart = nIndex + 1;
		}
	}

	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

int CPaymentCSVArray::FindFirstFreeNumber()
{
	int nResult = 0;

	CPaymentCSVRecord Payment;
	for (int n = 0; n < GetSize(); n++)
	{
		GetAt(n, Payment);
		if (Payment.GetPaymentNo() > n + 1)
		{
			nResult = n + 1;
			break;
		}
	}

	if (nResult == 0)
	{
		nResult = GetSize() + 1;
	}

	if (nResult < Payment::PaymentNo.Min || nResult > Payment::PaymentNo.Max)
	{
		nResult = 0;
	}

	return nResult;
}

/**********************************************************************/

const char* CPaymentCSVArray::GetDisplayName( int nPaymentNo )
{
	int nPaymentIdx = 0;
	m_strDisplayName = "";

	if (FindPaymentByNumber(nPaymentNo, nPaymentIdx) == FALSE)
	{
		m_strDisplayName.Format("Payment %d", nPaymentNo);
	}
	else
	{
		CPaymentCSVRecord Payment;
		GetAt ( nPaymentIdx, Payment );
		m_strDisplayName = Payment.GetDisplayName();
	}

	return m_strDisplayName;
}

/**********************************************************************/

void CPaymentCSVArray::SetPMSPaymentFlags()
{
	m_arrayPMSPaymentFlag.RemoveAll();

	for (int n = 0; n <= Payment::PaymentNo.Max; n++)
	{
		m_arrayPMSPaymentFlag.Add(FALSE);
	}

	for (int nPaymentIdx = 0; nPaymentIdx < GetSize(); nPaymentIdx++)
	{
		CPaymentCSVRecord Payment;
		GetAt(nPaymentIdx, Payment);

		int nPaymentNo = Payment.GetPaymentNo();

		if ((nPaymentNo >= 1) && (nPaymentNo < m_arrayPMSPaymentFlag.GetSize()))
		{
			switch (Payment.GetPaymentType())
			{
			case PAYMENT_TYPE_ROOM_AUTO:
			case PAYMENT_TYPE_ROOM_MANUAL:
				m_arrayPMSPaymentFlag[nPaymentNo] = TRUE;
				break;
			}
		}
	}
}

/**********************************************************************/

bool CPaymentCSVArray::IsPMSPaymentType(int nPaymentNo)
{
	if ((nPaymentNo >= 0) && (nPaymentNo < m_arrayPMSPaymentFlag.GetSize()))
	{
		return m_arrayPMSPaymentFlag[nPaymentNo];
	}
	else
	{
		return FALSE;
	}
}

/**********************************************************************/

void CPaymentCSVArray::WriteSMPaymentFile()
{
	int nDbIdx = 0;
	if (EcrmanOptions.GetGlobalPaymentFlag() == FALSE)
	{
		if (dbDatabase.GetSize() > 1)
		{
			nDbIdx = DataManager.GetActiveDbIdx();
		}
	}

	CString strFilename;
	strFilename.Format( "%s\\smfile01.dat", 
		dbDatabase.GetFolderPathData( nDbIdx ) );

	CSSFile fileSM;
	if ( fileSM.Open( strFilename, "wb" ) == TRUE )
	{
		fileSM.WriteLine( "No,Text,Group" );

		for ( int n = 0; n < GetSize(); n++ )
		{
			CPaymentCSVRecord Payment;
			GetAt( n, Payment );

			CCSV csv;
			csv.Add( Payment.GetPaymentNo() );
			csv.Add( Payment.GetEPOSName() );
			csv.Add( Payment.GetReportGroup() );

			fileSM.WriteLine( csv.GetLine() );
		}
	}
}

/**********************************************************************/

void CPaymentCSVArray::WriteSMPaymentGroupFile()
{
	int nDbIdx = 0;
	if (EcrmanOptions.GetGlobalPaymentGroupFlag() == FALSE)
	{
		if (dbDatabase.GetSize() > 1)
		{
			nDbIdx = DataManager.GetActiveDbIdx();
		}
	}

	CString strFilename = "";
	strFilename.Format( "%s\\smfile02.dat", 
		dbDatabase.GetFolderPathData( nDbIdx ) );

	CSSFile fileSM;
	if ( fileSM.Open( strFilename, "wb" ) == TRUE )
	{
		fileSM.WriteLine( "No,Text" );

		for ( int n = 0; n < DataManager.PaymentGroup.GetSize(); n++ )
		{
			CGroupCSVRecordPayment PaymentGroup;
			DataManager.PaymentGroup.GetAt( n, PaymentGroup );

			CCSV csv;
			csv.Add( PaymentGroup.GetGroupNo() );
			csv.Add( PaymentGroup.GetRepText() );
			
			fileSM.WriteLine( csv.GetLine() );
		}
	}
}

/**********************************************************************/