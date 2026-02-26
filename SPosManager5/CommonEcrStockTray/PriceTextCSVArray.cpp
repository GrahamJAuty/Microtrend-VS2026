/**********************************************************************/
#include "PluCSVArray.h"
/**********************************************************************/
#include "PriceTextCSVArray.h"
/**********************************************************************/

CPriceTextCSVRecord::CPriceTextCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CPriceTextCSVRecord::ClearRecord()
{
	m_strReportText = "";
}

/**********************************************************************/

void CPriceTextCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	switch ( csv.GetInt(0) )
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	}
}

/**********************************************************************/

void CPriceTextCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetReportText ( csv.GetString(1) );
}

/**********************************************************************/
	
void CPriceTextCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( PRICETEXT_VERSION );
	csv.Add ( m_strReportText );
}

/**********************************************************************/

void CPriceTextCSVRecord::SetReportText ( const char* szText )
{
	CString strText = szText;
	::TrimSpaces ( strText, FALSE );
	strText = strText.Left( MAX_PRICETEXT_LEN );
	m_strReportText = strText;	 
}

/**********************************************************************/

CPriceTextCSVArray::CPriceTextCSVArray() 
{
}

/**********************************************************************/

CPriceTextCSVArray::~CPriceTextCSVArray() 
{
}

/**********************************************************************/

int CPriceTextCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	while ( GetSize() < MAX_PLU_PRICES )
	{
		CPriceTextCSVRecord PriceText;
		PriceText.SetReportText ( "" );
		Add ( PriceText );
	}

	while ( GetSize() > MAX_PLU_PRICES )
		RemoveAt ( GetSize() - 1 );
	
	return nReply;
}

/**********************************************************************/

const char* CPriceTextCSVArray::GetReportText( int nLevel )
{
	m_strReportText = "";

	if ( ( nLevel >= 0 ) && ( nLevel < GetSize() ) )
	{
		CPriceTextCSVRecord PriceText;
		GetAt ( nLevel, PriceText );
		CString strTemp = PriceText.GetReportText();
		::TrimSpaces ( strTemp, FALSE );

		if ( strTemp == "" )
			m_strReportText.Format ( "Price %d", nLevel + 1 );
		else
			m_strReportText = strTemp;
	}
	
	return m_strReportText;
}

/**********************************************************************/

const char* CPriceTextCSVArray::GetTabbedComboText( int nLevel )
{
	CString strNum;
	strNum.Format ( "B%2.2d", nLevel + 1 );
		
	CCSV csv;
	csv.Add( strNum );
	csv.Add( GetReportText( nLevel ) );
	
	m_strTabbedComboText = csv.GetLine();
	return m_strTabbedComboText;
}

/**********************************************************************/

void CPriceTextCSVArray::SetReportText( int nLevel, const char* szText )
{
	if ( ( nLevel >= 0 ) && ( nLevel < GetSize() ) )
	{
		CPriceTextCSVRecord PriceText;
		GetAt ( nLevel, PriceText );
		PriceText.SetReportText( szText );
		SetAt ( nLevel, PriceText );
	}
}

/**********************************************************************/
