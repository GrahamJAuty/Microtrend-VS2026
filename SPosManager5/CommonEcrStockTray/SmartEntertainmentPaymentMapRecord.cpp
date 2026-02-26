/**********************************************************************/
 
/**********************************************************************/
//#include "globalfunctions.h"
/**********************************************************************/
#include "SmartEntertainmentPaymentMapArray.h"
/**********************************************************************/

CSmartEntPaymentMapEntry::CSmartEntPaymentMapEntry()
{
	Reset();
}

/**********************************************************************/

void CSmartEntPaymentMapEntry::Reset()
{
	m_nSmartEntPayNo = 0;
	m_strSmartEntPayName = "";
	m_nSharpPosPayNo = 0;
}

/**********************************************************************/

void CSmartEntPaymentMapEntry::Add ( CSmartEntPaymentMapEntry& source )
{
}

/**********************************************************************/

int CSmartEntPaymentMapEntry::Compare( CSmartEntPaymentMapEntry& source, int nHint )
{
	if ( m_nSmartEntPayNo != source.m_nSmartEntPayNo )
		return ( m_nSmartEntPayNo > source.m_nSmartEntPayNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSmartEntertainmentPaymentMapCSVRecord::CSmartEntertainmentPaymentMapCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CSmartEntertainmentPaymentMapCSVRecord::ClearRecord()
{
	m_nSmartEntPayNo = 1;
	m_strSmartEntPayName = "";
	m_nSharpPosPayNo = 1;
}

/**********************************************************************/

void CSmartEntertainmentPaymentMapCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( SEPAYMAP_VERSION_NO );
	csv.Add ( m_nSmartEntPayNo );
	csv.Add ( m_strSmartEntPayName );
	csv.Add ( m_nSharpPosPayNo );
}

/**********************************************************************/

void CSmartEntertainmentPaymentMapCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	switch ( csv.GetInt(0) )			/* version number */
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	}
}

/**********************************************************************/

void CSmartEntertainmentPaymentMapCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetSmartEntPayNo( csv.GetInt(1) );
	SetSmartEntPayName( csv.GetString(2) );
	SetSharpPosPayNo( csv.GetInt(3) );
}
	
/**********************************************************************/
/*	Set Routines											
/**********************************************************************/

void CSmartEntertainmentPaymentMapCSVRecord::SetSmartEntPayNo( int n )
{
	Set ( m_nSmartEntPayNo, n, SmartEntPaymentMap::SmartEntPayNo );
}

/**********************************************************************/

void CSmartEntertainmentPaymentMapCSVRecord::SetSmartEntPayName( const char* sz )
{
	Set ( m_strSmartEntPayName, sz, SmartEntPaymentMap::SmartEntPayName );
}

/**********************************************************************/

void CSmartEntertainmentPaymentMapCSVRecord::SetSharpPosPayNo( int n )
{
	Set ( m_nSharpPosPayNo, n, SmartEntPaymentMap::SharpPosPayNo );
}

/**********************************************************************/
