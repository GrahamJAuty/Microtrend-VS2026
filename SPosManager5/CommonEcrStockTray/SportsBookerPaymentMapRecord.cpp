/**********************************************************************/
 
/**********************************************************************/
//#include "globalfunctions.h"
/**********************************************************************/
#include "SportsBookerPaymentMapArray.h"
/**********************************************************************/

CSportsBookerPaymentMapCSVRecord::CSportsBookerPaymentMapCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CSportsBookerPaymentMapCSVRecord::ClearRecord()
{
	m_nSportsBookerPayNo = 1;
	m_nSharpPosPayNo = 1;
}

/**********************************************************************/

void CSportsBookerPaymentMapCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( SBPAYMAP_VERSION_NO );
	csv.Add ( m_nSportsBookerPayNo );
	csv.Add ( m_nSharpPosPayNo );
}

/**********************************************************************/

void CSportsBookerPaymentMapCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	switch ( csv.GetInt(0) )			/* version number */
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	}
}

/**********************************************************************/

void CSportsBookerPaymentMapCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetSportsBookerPayNo( csv.GetInt(1) );
	SetSharpPosPayNo( csv.GetInt(2) );
}
	
/**********************************************************************/
/*	Set Routines											
/**********************************************************************/

void CSportsBookerPaymentMapCSVRecord::SetSportsBookerPayNo( int n )
{
	Set ( m_nSportsBookerPayNo, n, SportsBookerPaymentMap::SportsBookerPayNo );
}

/**********************************************************************/

void CSportsBookerPaymentMapCSVRecord::SetSharpPosPayNo( int n )
{
	Set ( m_nSharpPosPayNo, n, SportsBookerPaymentMap::SharpPosPayNo );
}

/**********************************************************************/
