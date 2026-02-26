/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
//#include "EcrmanOptionsIni.h"
//#include "globalfunctions.h"
#include "PluCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SportsBookerPriceBandCSVArray.h"
/**********************************************************************/

CSportsBookerPriceBandCSVRecord::CSportsBookerPriceBandCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CSportsBookerPriceBandCSVRecord::ClearRecord()
{
	m_strReportText = "";
	m_nSharpPosBand = 0;
}

/**********************************************************************/

void CSportsBookerPriceBandCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	switch ( csv.GetInt(0) )
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	}
}

/**********************************************************************/

void CSportsBookerPriceBandCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetReportText ( csv.GetString(1) );
	SetSharpPosBand ( csv.GetInt(2) );
}

/**********************************************************************/
	
void CSportsBookerPriceBandCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( SBPRICETEXT_VERSION );
	csv.Add ( m_strReportText );
	csv.Add ( m_nSharpPosBand );
}

/**********************************************************************/

void CSportsBookerPriceBandCSVRecord::SetReportText ( const char* szText )
{
	CString strText = szText;
	::TrimSpaces ( strText, FALSE );
	strText = strText.Left( MAX_SBPRICETEXT_LEN );
	m_strReportText = strText;	 
}

/**********************************************************************/

void CSportsBookerPriceBandCSVRecord::SetSharpPosBand( int nBand )
{
	if ( ( nBand >= 0 ) && ( nBand <= MAX_PLU_PRICES ) )
		m_nSharpPosBand = nBand;
}

/**********************************************************************/

void CSportsBookerPriceBandCSVRecord::MakeDefaultRecord( int nBand )
{
	if ( ( nBand >= 1 ) && ( nBand <= MAX_PLU_PRICES ) )
	{
		m_nSharpPosBand = nBand;
		m_strReportText = "";
	}
	else
	{
		m_nSharpPosBand = 0;
		m_strReportText.Format( "SptBook P.Band %d", nBand );
	}
}

/**********************************************************************/

CSportsBookerPriceBandCSVArray::CSportsBookerPriceBandCSVArray() 
{
	m_bGotReportMap = FALSE;
}

/**********************************************************************/

CSportsBookerPriceBandCSVArray::~CSportsBookerPriceBandCSVArray() 
{
}

/**********************************************************************/

int CSportsBookerPriceBandCSVArray::Open( const char* szFilename, int nMode )
{
	m_bGotReportMap = FALSE;
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	int nSize = GetSize();
	while ( nSize < MAX_SBPLU_PRICES )
	{
		CSportsBookerPriceBandCSVRecord PriceBand;
		PriceBand.MakeDefaultRecord( nSize + 1 );
		Add ( PriceBand );
		nSize++;
	}

	while ( GetSize() > MAX_SBPLU_PRICES )
		RemoveAt ( GetSize() - 1 );
	
	return nReply;
}

/**********************************************************************/

void CSportsBookerPriceBandCSVArray::GetRecord( int nLevel, CSportsBookerPriceBandCSVRecord& Record )
{
	if ( ( nLevel >= 0 ) && ( nLevel < GetSize() ) )
		GetAt ( nLevel, Record );
}

/**********************************************************************/

void CSportsBookerPriceBandCSVArray::SetRecord( int nLevel, CSportsBookerPriceBandCSVRecord& Record )
{
	if ( ( nLevel >= 0 ) && ( nLevel < GetSize() ) )
		SetAt ( nLevel, Record );
}

/**********************************************************************/

void CSportsBookerPriceBandCSVArray::BuildReportMap()
{
	for ( int n = 0; n < MAX_SBPLU_PRICES; n++ )
		m_nPriceBand[n] = n + 10;
	
	for ( int n = 0; ( n < GetSize() ) && ( n < MAX_SBPLU_PRICES ); n++ )
	{
		CSportsBookerPriceBandCSVRecord Record;
		GetRecord( n, Record );

		int nSharpPosBand = Record.GetSharpPosBand();
		if ( ( nSharpPosBand >= 1 ) && ( nSharpPosBand <= MAX_PLU_PRICES ) )
			m_nPriceBand[n] = nSharpPosBand - 1;
	}
	
	m_bGotReportMap = TRUE;
}

/**********************************************************************/

int CSportsBookerPriceBandCSVArray::GetReportMapPriceBand( int nLevel )
{
	if ( FALSE == m_bGotReportMap )
		BuildReportMap();

	if ( ( nLevel >= 0 ) && ( nLevel < MAX_SBPLU_PRICES ) )
		return m_nPriceBand[ nLevel ];
	else
		return MAX_PLU_PRICES + MAX_SBPLU_PRICES;
}

/**********************************************************************/

const char* CSportsBookerPriceBandCSVArray::GetDisplayText( int nBand )
{
	if ( ( nBand >= 0 ) && ( nBand < MAX_PLU_PRICES ) )
		m_strDisplayText = DataManager.PriceText.GetReportText( nBand );
	else if ( ( nBand >= MAX_PLU_PRICES ) && ( nBand < MAX_PLU_PRICES + MAX_SBPLU_PRICES ) )
	{
		int nSBLevel = nBand - MAX_PLU_PRICES; 
		
		CSportsBookerPriceBandCSVRecord Record;
		GetRecord( nSBLevel, Record );
		
		m_strDisplayText = Record.GetReportText();
		if ( m_strDisplayText == "" )
		{
			Record.MakeDefaultRecord( nSBLevel + 1 );
			m_strDisplayText = Record.GetReportText();
		}
	}
	else if ( MAX_PLU_PRICES + MAX_SBPLU_PRICES == nBand )
		m_strDisplayText = "Other SptBook P.Band";
	else
		m_strDisplayText.Format ( "Unknown P.Band (%d)", nBand );

	return m_strDisplayText;
}

/**********************************************************************/

bool CSportsBookerPriceBandCSVArray::IsActivePriceBand( int nBand )
{
	bool bResult = FALSE;

	if ( ( nBand >= 0 ) && ( nBand < MAX_PLU_PRICES ) )
		bResult = TRUE;
	else if ( SysInfo.IsSportsBookerSystem() == TRUE )
	{
		if ( FALSE == m_bGotReportMap )
			BuildReportMap();

		if ( ( nBand >= MAX_PLU_PRICES ) && ( nBand < MAX_PLU_PRICES + MAX_SBPLU_PRICES ) )
			bResult = ( m_nPriceBand[ nBand - MAX_PLU_PRICES ] == nBand );
	}
	
	return bResult;
}

/**********************************************************************/
