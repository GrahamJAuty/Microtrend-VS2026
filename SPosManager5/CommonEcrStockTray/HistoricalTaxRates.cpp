/**********************************************************************/
#include "HistoricalTaxRates.h"
/**********************************************************************/

CTaxDateInfo::CTaxDateInfo()
{
	m_strDate = "20080101";
	for ( int n = 0; n < MAX_TAX_BANDS; n++ )
		m_dRate[n] = 0.0;
}

/**********************************************************************/

CHistoricalTaxRates::CHistoricalTaxRates()
{
	m_bGotTaxDateInfoArray = FALSE;
}

/**********************************************************************/

int CHistoricalTaxRates::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );
	int nReply = CSharedStringArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}
	
	m_bGotTaxDateInfoArray = FALSE;
	return nReply;
}

/**********************************************************************/

void CHistoricalTaxRates::BuildTaxDateInfoArray()
{
	if ( TRUE == m_bGotTaxDateInfoArray )
		return;

	m_arrayTaxDateInfo.RemoveAll();
	
	CString strLastDate = "";
	for ( int n = 0; n < GetSize(); n++ )
	{
		CCSV csvFile( GetAt(n) );

		if ( csvFile.GetSize() < 2 )
			continue;

		CString strDate = csvFile.GetString(0);
		if ( strDate <= strLastDate )
			continue;

		if ( strDate.GetLength() != 8 )
			continue;

		if ( ::TestNumeric( strDate ) == FALSE )
			continue;

		COleDateTime oleDate( atoi( strDate.Left(4) ), atoi( strDate.Mid( 4, 2 ) ), atoi( strDate.Right( 2 ) ), 0, 0, 0 );
		if ( oleDate.m_status != COleDateTime::valid )
			continue;

		CTaxDateInfo info;
		info.m_strDate = strDate;
		info.m_nPMSDayNumber = GetDayNumberSince2011( oleDate );

		for ( int x = 1; ( x < csvFile.GetSize() ) && ( x <= MAX_TAX_BANDS ); x++ )
			info.m_dRate[x - 1] = csvFile.GetDouble(x);

		m_arrayTaxDateInfo.Add( info );
			
		strLastDate = strDate;
	}

	m_bGotTaxDateInfoArray = TRUE;
}

/**********************************************************************/

void CHistoricalTaxRates::PrepareForWrite()
{
	RemoveAt(0, GetSize() );

	for ( int n = 0; n < GetTaxDateCount(); n++ )
	{
		CCSV csv;
		
		csv.Add( m_arrayTaxDateInfo[n].m_strDate );
		for ( int r = 0; r < MAX_TAX_BANDS; r++ )
			csv.Add( m_arrayTaxDateInfo[n].m_dRate[r], 4 );
	
		Add( csv.GetLine() );
	}
}

/**********************************************************************/

int CHistoricalTaxRates::GetDateCode( const char* szDate )
{
	if ( FALSE == m_bGotTaxDateInfoArray )
		BuildTaxDateInfoArray();

	CString strDate = szDate;

	int nSize = GetTaxDateCount();
	
	if ( nSize == 0 )
		return 0;

	for ( int n = 0; n < nSize; n++ )
	{
		if ( strDate < m_arrayTaxDateInfo[n].m_strDate )
			return n;
	}

	return nSize;
}

/**********************************************************************/

double CHistoricalTaxRates::GetTaxRate( int nDateCode, int nTaxBand )
{
	if ( 999 == nDateCode )
		return 0.0;

	if ( FALSE == m_bGotTaxDateInfoArray )
		BuildTaxDateInfoArray();

	if ( ( nDateCode <= 0 ) || ( nDateCode > GetTaxDateCount() ) )
	{
		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo( nTaxBand, TaxRateInfo );
		return TaxRateInfo.m_dTaxRate;
	}
	else if ( ( nTaxBand >= 1 ) && ( nTaxBand <= MAX_TAX_BANDS ) )
		return m_arrayTaxDateInfo[nDateCode - 1].m_dRate[nTaxBand - 1];
	else
		return 0.0;
}

/**********************************************************************/

void CHistoricalTaxRates::SetTaxRate( int nDateCode, int nTaxBand, double dRate )
{
	if ( ( nDateCode >= 1 ) && ( nDateCode <= GetTaxDateCount() ) )
		if ( ( nTaxBand >= 1 ) && ( nTaxBand <= MAX_TAX_BANDS ) )
			m_arrayTaxDateInfo[nDateCode - 1].m_dRate[nTaxBand - 1] = dRate;
}

/**********************************************************************/

const char* CHistoricalTaxRates::GetDisplayLine( int nDateCode, bool bForDelete )
{
	if ( ( nDateCode >= 1 ) && ( nDateCode <= GetTaxDateCount() ) )
	{
		if ( FALSE == bForDelete )
		{
			m_strDisplayLine.Format( "%s/%s/%s\t%.4f\t%.4f\t%.4f\t%.4f",
				(const char*) m_arrayTaxDateInfo[nDateCode - 1].m_strDate.Right(2),
				(const char*) m_arrayTaxDateInfo[nDateCode - 1].m_strDate.Mid(4,2),
				(const char*) m_arrayTaxDateInfo[nDateCode - 1].m_strDate.Left(4),
				m_arrayTaxDateInfo[nDateCode - 1].m_dRate[0],
				m_arrayTaxDateInfo[nDateCode - 1].m_dRate[1],
				m_arrayTaxDateInfo[nDateCode - 1].m_dRate[2],
				m_arrayTaxDateInfo[nDateCode - 1].m_dRate[3] );
		}
		else
		{
			m_strDisplayLine.Format( "%s/%s/%s  %.4f",
				(const char*) m_arrayTaxDateInfo[nDateCode - 1].m_strDate.Right(2),
				(const char*) m_arrayTaxDateInfo[nDateCode - 1].m_strDate.Mid(4,2),
				(const char*) m_arrayTaxDateInfo[nDateCode - 1].m_strDate.Left(4),
				m_arrayTaxDateInfo[nDateCode - 1].m_dRate[0] );
		}
	}
	else
		m_strDisplayLine = "";

	return m_strDisplayLine;
}

/**********************************************************************/

void CHistoricalTaxRates::DeleteDate( int nDateCode )
{
	if ( ( nDateCode >= 1 ) && ( nDateCode <= GetTaxDateCount() ) )
		m_arrayTaxDateInfo.RemoveAt( nDateCode - 1 );
}

/**********************************************************************/

int CHistoricalTaxRates::SetTaxDateInfo( CTaxDateInfo& info )
{
	CString strNewDate = info.m_strDate;

	int n = 0;
	for ( ; n < GetTaxDateCount(); n++ )
	{
		CString strArrayDate = m_arrayTaxDateInfo[n].m_strDate;

		if ( strNewDate == strArrayDate )
		{
			m_arrayTaxDateInfo.SetAt( n, info );
			return n;
		}

		if ( strNewDate < strArrayDate )
			break;
	}

	m_arrayTaxDateInfo.InsertAt( n, info );

	return n;
}

/**********************************************************************/

void CHistoricalTaxRates::GetTaxDateInfo( int nDateCode, CTaxDateInfo& info )
{
	if ( ( nDateCode >= 1 ) && ( nDateCode <= GetTaxDateCount() ) )
		info = m_arrayTaxDateInfo.GetAt( nDateCode - 1 );
}

/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/

int CHistoricalTaxRates::GetPMSDateCode( int nPMSDayNumber )
{
	if ( FALSE == m_bGotTaxDateInfoArray )
		BuildTaxDateInfoArray();

	int nSize = GetTaxDateCount();
	
	if ( nSize == 0 )
		return 0;

	for ( int n = 0; n < nSize; n++ )
	{
		if ( nPMSDayNumber < m_arrayTaxDateInfo[n].m_nPMSDayNumber )
			return n;
	}

	return nSize;
}

/**********************************************************************/

void CHistoricalTaxRates::SplitPMSDateRange( int nStartDay, int nNights, CArray<CPMSBookingPartInfo,CPMSBookingPartInfo>& arrayParts )
{
	arrayParts.RemoveAll();

	if ( nNights <= 1 )
	{
		CPMSBookingPartInfo infoPart;
		infoPart.SetStartDay( nStartDay );
		infoPart.SetNights( nNights );
		arrayParts.Add( infoPart );
		return;
	}

	int nPart1 = GetPMSDateCode( nStartDay );
	int nPart2 = GetPMSDateCode( nStartDay + nNights - 1 );

	while ( nPart1 < nPart2 )
	{
		int nTaxChangeDay = m_arrayTaxDateInfo[ nPart1 ].m_nPMSDayNumber;
		int nPartLength = nTaxChangeDay - nStartDay;

		CPMSBookingPartInfo infoPart;
		infoPart.SetStartDay( nStartDay );
		infoPart.SetNights( nPartLength );
		arrayParts.Add( infoPart );

		nPart1++;
		nStartDay += nPartLength;
		nNights -= nPartLength;
	}

	CPMSBookingPartInfo infoPart;
	infoPart.SetStartDay( nStartDay );
	infoPart.SetNights( nNights );
	arrayParts.Add( infoPart );	
}

/**********************************************************************/
#endif
/**********************************************************************/
