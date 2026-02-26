/**********************************************************************/
#include "SPOSVersions.h"
/**********************************************************************/
#include "LoyaltySchemeCSVArray.h"
/**********************************************************************/

CLoyaltySchemeCSVRecord::CLoyaltySchemeCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::ClearRecord()
{
	m_nSchemeNo = 1;
	m_bActive = FALSE;
	m_strSchemeName = "";
	m_strFormat = "";
	m_strNumberStartLower = "";
	m_strNumberStartUpper = "";
	m_bCheckDigit = FALSE;
	m_bStaffCard = FALSE;
	m_bStaffCredit = FALSE;
	m_nStaffLimitDay = 0;
	m_nStaffLimitWeek = 0;

	for (int n = 0; n < 12; n++)
	{
		m_nStaffLimitMonth[n] = 0;
	}

	m_nActionType = 0;
	m_nSelection = 0;

	for (int n = 0; n < 5; n++)
	{
		m_nPromoNo[n] = 0;
	}

	m_nPencePerPoint = 0;
	m_nMacroNo = 0;

	m_strDeniedPayments = "";
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetSchemeNo( csv.GetInt(0) );
	
	switch ( csv.GetInt(1) )
	{
	case 1:		V1ConvertFromCSV ( csv );			break;
	}
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetSchemeName		( csv.GetString(2) );
	SetActiveFlag		( csv.GetBool(3) );
	SetFormat			( csv.GetString(4) );
	SetNumberStartLower ( csv.GetString(5) );
	SetNumberStartUpper ( csv.GetString(6) );
	SetCheckDigitFlag	( csv.GetBool(7) );
	SetStaffCardFlag	( csv.GetBool(8) );
	SetStaffCreditFlag	( csv.GetBool(9) );
	SetStaffLimitDay	( csv.GetInt(10) );
	SetStaffLimitWeek	( csv.GetInt(11) );

	int nPos = 12;
	for ( int n = 0; n < 12; n++ )
	{
		SetStaffLimitMonth( n, csv.GetInt( nPos++) );
	}

	SetActionTypeAndSelection( csv.GetInt(nPos), csv.GetInt(nPos+1) );
	nPos += 2;

	for ( int n = 1; n < 5; n++ )
	{
		SetPromoNo( n, csv.GetInt(nPos++ ) );
	}

	SetPencePerPoint	( csv.GetInt(nPos++) );
	SetMacroNo			( csv.GetInt(nPos++) );
	SetDeniedPayments	( csv.GetString(nPos++) );
}

/**********************************************************************/
	
void CLoyaltySchemeCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nSchemeNo );
	csv.Add ( LOYALTYSCHEME_VERSION );
	csv.Add ( m_strSchemeName );
	csv.Add ( m_bActive );
	csv.Add ( m_strFormat );
	csv.Add ( m_strNumberStartLower );
	csv.Add ( m_strNumberStartUpper );
	csv.Add ( m_bCheckDigit );
	csv.Add ( m_bStaffCard );
	csv.Add ( m_bStaffCredit );
	csv.Add ( m_nStaffLimitDay );
	csv.Add ( m_nStaffLimitWeek );

	for ( int n = 0; n < 12; n++ )
	{
		csv.Add( m_nStaffLimitMonth[n] ) ;
	}

	csv.Add ( m_nActionType );
	csv.Add ( m_nSelection );

	for ( int n = 1; n < 5; n++ )
	{
		csv.Add( m_nPromoNo[n] );
	}

	csv.Add ( m_nPencePerPoint );
	csv.Add ( m_nMacroNo );
	csv.Add ( m_strDeniedPayments );
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::SetSchemeNo( int n )
{
	Set ( m_nSchemeNo, n, LoyaltyScheme::SchemeNo );
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::SetSchemeName ( const char* sz )
{
	CString strName = sz;
	::TrimSpaces ( strName, FALSE );
	Set( m_strSchemeName, strName, LoyaltyScheme::SchemeName );
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::SetFormat ( const char* sz )
{
	CString strFormat = sz;
	::TrimSpaces ( strFormat, FALSE );
	Set( m_strFormat, strFormat, LoyaltyScheme::Format );
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::SetNumberStartLower( const char* sz )
{
	CString strNumber = sz;
	::TrimSpaces ( strNumber, FALSE );
	::AddLeading( strNumber, LoyaltyScheme::NumberStartLower.Max, '0' );
	Set( m_strNumberStartLower, strNumber, LoyaltyScheme::NumberStartLower );
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::SetNumberStartUpper( const char* sz )
{
	CString strNumber = sz;
	::TrimSpaces ( strNumber, FALSE );
	::AddLeading( strNumber, LoyaltyScheme::NumberStartUpper.Max, '0' );
	Set( m_strNumberStartUpper, strNumber, LoyaltyScheme::NumberStartUpper );
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::SetStaffLimitDay( int n )
{
	Set( m_nStaffLimitDay, n, LoyaltyScheme::StaffLimitDay );
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::SetStaffLimitWeek( int n )
{
	Set( m_nStaffLimitWeek, n, LoyaltyScheme::StaffLimitWeek );
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::SetStaffLimitMonth( int nMth, int n )
{
	if ( ( nMth >= 0 ) && ( nMth < 12 ) )
	{
		Set( m_nStaffLimitMonth[nMth], n, LoyaltyScheme::StaffLimitMonth );
	}
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::SetActionTypeAndSelection( int t, int s )
{
	switch( t )
	{
	case 0:	//NO ACTION
		m_nActionType = 0;
		m_nSelection = 0;
		break;

	case 1: //ITEM PROMOTION
	case 2: //SUBTOTAL PROMOTION
	case 4: //MULTIPLE SUBTOTAL PROMOTIONS
		
		m_nActionType = t;

		if ( ( s >= 0 ) && ( s <= Promotions::PromoNo.Max ) )
			m_nSelection = s;
		else
			m_nSelection = 0;
		
		break;

	case 3: //PRICE BAND
		m_nActionType = 3;

		if ( ( s >= 0 ) && ( s <= MAX_PLU_PRICES ) )
			m_nSelection = s;
		else
			m_nSelection = 0;

		break;
	}
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::SetPromoNo( int nIdx, int n )
{
	switch( nIdx )
	{
	case 0:
		switch( m_nActionType )
		{
		case 1:
		case 2:
		case 3:
		case 4:
			m_nSelection = n;
			break;
		}
		break;

	case 1:
	case 2:
	case 3:
	case 4:
		m_nPromoNo[nIdx] = n;
		break;
	}
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::SetPencePerPoint( int n )
{
	Set( m_nPencePerPoint, n, LoyaltyScheme::PencePerPoint );
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::SetMacroNo( int n )
{
	Set( m_nMacroNo, n, LoyaltyScheme::MacroNo );
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::SetDeniedPayments( const char* sz )
{
	Set( m_strDeniedPayments, sz, LoyaltyScheme::DeniedPayments );
}

/**********************************************************************/

const char* CLoyaltySchemeCSVRecord::GetDisplayName()
{
	CString strTemp = m_strSchemeName;
	::TrimSpaces ( strTemp, FALSE );

	if ( strTemp == "" )
		m_strDisplayName.Format ( "Loyalty Scheme %d", m_nSchemeNo );
	else
		m_strDisplayName = strTemp;

	return m_strDisplayName;
}

/**********************************************************************/

int CLoyaltySchemeCSVRecord::GetStaffLimitMonth( int nMth )
{
	if ( ( nMth >= 0 ) && ( nMth < 12 ) )
		return m_nStaffLimitMonth[nMth];
	else
		return 0;
}

/**********************************************************************/

int CLoyaltySchemeCSVRecord::GetPromoNo( int nIdx )
{
	int nResult = 0;

	switch( nIdx )
	{
	case 0:
		nResult = m_nSelection;
		break;

	case 1:
	case 2:
	case 3:
	case 4:
		nResult = m_nPromoNo[nIdx];
		break;
	}

	return nResult;
	
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::CheckMinimumSPOSVersion( CMinimumSPOSVersion& version )
{
	version.m_nMinVer = SPOS_V4;
	version.m_strReason = "";
	version.m_bInvalidData = FALSE;
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::TidyPromos()
{
	CReportConsolidationArray<CSortedIntItem> arrayInUse;

	for ( int n = 0; n < 5; n++ )
	{
		int nPromoNo = GetPromoNo(n);

		if ( nPromoNo > 0 )
		{
			CSortedIntItem item;
			item.m_nItem = nPromoNo;

			int nIdx = 0;
			if ( arrayInUse.Find( item, nIdx ) == FALSE )
			{
				arrayInUse.InsertAt( nIdx, item );
			}
		}
	}
	
	int x = 0;
	for ( x = 0; x < arrayInUse.GetSize(); x++ )
	{
		CSortedIntItem item;
		arrayInUse.GetAt( x, item );
		SetPromoNo( x, item.m_nItem );
	}

	for ( ; x < 5; x++ )
	{
		SetPromoNo( x, 0 );
	}
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::GetSafeDeniedPaymentRanges( CArray<CSortedIntAndInt>& arrayRanges )
{
	arrayRanges.RemoveAll();

	int nLastPayNo = 0;
	bool bValid = TRUE;

	CCSV csvDeny( m_strDeniedPayments );
	
	for ( int n = 0; ( n < csvDeny.GetSize() ) && ( TRUE == bValid ); n++ )
	{
		int nPayNo = atoi( csvDeny.GetString(n) );

		if ( ( nPayNo < 1 ) || ( nPayNo > 999999 ) )
		{
			bValid = FALSE;
		}
		else if ( nPayNo <= 999 )
		{
			if ( nPayNo <= nLastPayNo )
			{
				bValid = FALSE;
			}
			else
			{
				CSortedIntAndInt item;
				item.m_nInt1 = nPayNo;
				item.m_nInt2 = nPayNo;
				arrayRanges.Add( item );
				
				nLastPayNo = nPayNo;
			}
		}
		else
		{
			int nPayNo1 = nPayNo / 1000;
			int nPayNo2 = nPayNo % 1000;

			if ( ( nPayNo1 <= nLastPayNo ) || ( nPayNo1 > nPayNo2 ) )
			{
				bValid = FALSE;
			}
			else
			{
				CSortedIntAndInt item;
				item.m_nInt1 = nPayNo1;
				item.m_nInt2 = nPayNo2;
				arrayRanges.Add( item );
				
				nLastPayNo = nPayNo2;
			}
		}
	}
}

/**********************************************************************/

const char* CLoyaltySchemeCSVRecord::GetDeniedPaymentsForVPM()
{
	m_strPaymentDenyListVPM = "";

	CArray<CSortedIntAndInt> arrayRanges;
	GetSafeDeniedPaymentRanges( arrayRanges );

	int nSize = arrayRanges.GetSize();
	if ( nSize > 0 )
	{
		CCSV csvVPM( ',', '"', FALSE, FALSE, FALSE ); //NEED TO ADD BLANK DUMMY FIELD FOR VPM
		csvVPM.Add( "D" );

		for ( int n = 0; n < nSize; n++ )
		{
			for ( int nPayNo = arrayRanges[n].m_nInt1; nPayNo <= arrayRanges[n].m_nInt2; nPayNo++ )
			{
				csvVPM.Add( nPayNo );
			}
		}
	
		csvVPM.Add( "" );
		m_strPaymentDenyListVPM = csvVPM.GetLine();
	}

	return m_strPaymentDenyListVPM;
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::SetDeniedPaymentsFromVPM( const char* sz )
{
	CCSV csvDeny( sz );

	if ( csvDeny.GetSize() >= 2 )
	{
		CString str = csvDeny.GetString(0);
		str.MakeUpper();

		if ( str == "D" )
		{
			m_strDeniedPayments = "";

			CReportConsolidationArray<CSortedIntWithFlag> arrayPayNo;
			
			for ( int n = 1; n < csvDeny.GetSize(); n++ )
			{
				int nPayNo = csvDeny.GetInt(n);

				if ( ( nPayNo >= Payment::PaymentNo.Min ) && ( nPayNo <= Payment::PaymentNo.Max ) )
				{
					CSortedIntWithFlag item;
					item.m_nInt = nPayNo;
					item.m_nFlag = FALSE;
					arrayPayNo.Consolidate( item );
				}
			}

			CopyDeniedPaymentsFromArray( arrayPayNo, FALSE );
		}
	}
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::CopyDeniedPaymentsToArray( CReportConsolidationArray<CSortedIntWithFlag>& arrayAllow, CReportConsolidationArray<CSortedIntWithFlag>& arrayDeny )
{
	arrayAllow.RemoveAll();
	arrayDeny.RemoveAll();

	CArray<CSortedIntAndInt> arrayRanges;
	GetSafeDeniedPaymentRanges( arrayRanges );

	for ( int n = 0; n < arrayRanges.GetSize(); n++ )
	{
		for ( int nPayNo = arrayRanges[n].m_nInt1; nPayNo <= arrayRanges[n].m_nInt2; nPayNo++ )
		{
			int nPayIdx;
			if ( DataManager.Payment.FindPaymentByNumber( nPayNo, nPayIdx ) == TRUE )
			{
				CSortedIntWithFlag item;
				item.m_nInt = nPayIdx;
				item.m_nFlag = FALSE;
				arrayDeny.Consolidate( item );
			}
		}
	}

	for ( int nPayIdx = 0; nPayIdx < DataManager.Payment.GetSize(); nPayIdx++ )
	{
		CSortedIntWithFlag item;
		item.m_nInt = nPayIdx;
		item.m_nFlag = FALSE;

		int nPos;
		if ( arrayDeny.Find( item, nPos ) == FALSE )
		{
			arrayAllow.DirectAdd( item );
		}
	}
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::CopyDeniedPaymentsFromArray( CReportConsolidationArray<CSortedIntWithFlag>& arrayDeny, bool bIdxArray )
{
	CCSV csvDeniedPayments;

	int nLastPayNo = -1;
	int nSeqStart = -1;

	for ( int n = 0; n < arrayDeny.GetSize(); n++ )
	{
		CSortedIntWithFlag item;
		arrayDeny.GetAt( n, item );
		
		int nPayNo = 0;
		if ( FALSE == bIdxArray )
		{
			nPayNo = item.m_nInt; 
		}
		else
		{
			CPaymentCSVRecord Payment;
			DataManager.Payment.GetAt( item.m_nInt, Payment );
			nPayNo = Payment.GetPaymentNo();
		}

		if ( nPayNo <= nLastPayNo )
			break;

		if ( nPayNo > nLastPayNo + 1 )
		{
			AddPaymentRange( csvDeniedPayments, nSeqStart, nLastPayNo );
			nSeqStart = nPayNo;
		}

		nLastPayNo = nPayNo;
	}

	AddPaymentRange( csvDeniedPayments, nSeqStart, nLastPayNo );
	m_strDeniedPayments = csvDeniedPayments.GetLine();
}

/**********************************************************************/

void CLoyaltySchemeCSVRecord::AddPaymentRange( CCSV& csv, int nSeqStart, int nSeqEnd )
{
	if ( nSeqStart > 0 )
	{
		if ( nSeqEnd != nSeqStart )
		{
			CString str;
			str.Format( "%3.3d%3.3d", nSeqStart, nSeqEnd );
			csv.Add( str );
		}
		else
		{
			csv.Add( nSeqStart );
		}
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CLoyaltySchemeCSVArray::CLoyaltySchemeCSVArray() 
{
}

/**********************************************************************/

CLoyaltySchemeCSVArray::~CLoyaltySchemeCSVArray() 
{
}

/**********************************************************************/

int CLoyaltySchemeCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	int nLastSchemeNo = 0;
	int nValidRecords = 0;
	
	CLoyaltySchemeCSVRecord SchemeRecord;
	for ( ; nValidRecords < GetSize(); nValidRecords++ )
	{
		GetAt( nValidRecords, SchemeRecord );

		int nSchemeNo = SchemeRecord.GetSchemeNo();
		if ( nSchemeNo <= nLastSchemeNo || nSchemeNo > LoyaltyScheme::SchemeNo.Max )
			break;

		nLastSchemeNo = nSchemeNo;
	}

	while ( GetSize() > nValidRecords )
		RemoveAt( GetSize() - 1 );
	
	return nReply;
}

/**********************************************************************/

bool CLoyaltySchemeCSVArray::FindSchemeByNumber( int nSchemeNo, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;
		
		CLoyaltySchemeCSVRecord Scheme;
		GetAt ( nIndex, Scheme );
		int nArrayNo = Scheme.GetSchemeNo();
			
		if ( nSchemeNo == nArrayNo )
			return TRUE;

		if ( nSchemeNo < nArrayNo )
			nEnd = nIndex - 1;
		else
			nStart = nIndex + 1;
	}

	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

int CLoyaltySchemeCSVArray::FindFirstFreeNumber()
{
	int nResult = 0;

	CLoyaltySchemeCSVRecord Scheme;
	for ( int n = 0; n < GetSize(); n++ )
	{
		GetAt( n, Scheme );
		if ( Scheme.GetSchemeNo() > n + 1 )
		{
			nResult = n + 1;
			break;
		}
	}

	if ( nResult == 0 )
		nResult = GetSize() + 1;

	if ( nResult < LoyaltyScheme::SchemeNo.Min || nResult > LoyaltyScheme::SchemeNo.Max )
		nResult = 0;

	return nResult;
}

/**********************************************************************/

const char* CLoyaltySchemeCSVArray::GetDisplayName( int nSchemeNo )
{
	int nSchemeIdx;
	m_strDisplayName = "";

	if ( FindSchemeByNumber( nSchemeNo, nSchemeIdx ) == FALSE )
		m_strDisplayName.Format ( "Loyalty Scheme %d", nSchemeNo );
	else
	{
		CLoyaltySchemeCSVRecord Scheme;
		GetAt ( nSchemeIdx, Scheme );
		m_strDisplayName = Scheme.GetSchemeName();
	}

	return m_strDisplayName;
}

/**********************************************************************/
