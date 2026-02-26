/**********************************************************************/
 
/**********************************************************************/
//#include "globalfunctions.h"
#include "ProcessedOrder.h"
/**********************************************************************/
#include "ProcessedOrderHeader.h"
/**********************************************************************/

CProcessedOrderHeader::CProcessedOrderHeader()
{
	ClearRecord();
}

/**********************************************************************/

void CProcessedOrderHeader::ClearRecord()
{
	m_nOrderNum = 0;
	m_nSuppNo = 0;
	m_nOrderType = ORDER_TYPE_OPEN;
	m_bModified = FALSE;
	SetDate();
}

/**********************************************************************/

CProcessedOrderHeader::~CProcessedOrderHeader()
{	
}

/**********************************************************************/

void CProcessedOrderHeader::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( "H" );
	csv.Add ( ORDERHEADER_VERSION );
	csv.Add ( m_nOrderNum );
	csv.Add ( m_nSuppNo );
	csv.Add ( m_strDate );
	csv.Add ( m_strTime );
	csv.Add ( m_nOrderType );
	csv.Add ( m_bModified );
}

/**********************************************************************/

void CProcessedOrderHeader::ConvertToCSVForFile ( CCSV& csv )
{
	csv.Add ( "H" );
	csv.Add ( ORDERHEADER_VERSION );
	csv.Add ( m_nOrderNum );
	csv.Add ( m_nSuppNo );
	csv.Add ( m_strDate );
	csv.Add ( m_strTime );
}

/**********************************************************************/

void CProcessedOrderHeader::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();
	switch ( csv.GetInt(1) )
	{
	case 1:	V1ConvertFromCSV( csv );	break;
	}
}

/**********************************************************************/

void CProcessedOrderHeader::V1ConvertFromCSV ( CCSV& csv )
{
	int n = 2;
	SetOrderNum ( csv.GetInt(n++) );
	SetSuppNo( csv.GetInt(n++) );
	SetDateString ( csv.GetString(n++) );
	SetTimeString ( csv.GetString(n++) );
	SetOrderType( csv.GetInt(n++) );
	SetModifiedFlag( csv.GetBool(n++) );
}

/**********************************************************************/

void CProcessedOrderHeader::SetDate()
{
	COleDateTime date = COleDateTime::GetCurrentTime();

	m_strDate.Format( "%4.4d%2.2d%2.2d",
		date.GetYear(),
		date.GetMonth(),
		date.GetDay() );

	m_strTime.Format ( "%2.2d%2.2d%2.2d",
		date.GetHour(),
		date.GetMinute(),
		date.GetSecond() );
}

/**********************************************************************/

void CProcessedOrderHeader::SetDateString( const char* szDate )
{
	CString strTemp = szDate;
	
	if ( strTemp.GetLength() == 8 )
		if ( ::TestNumeric( strTemp ) == TRUE )
			m_strDate = strTemp;
}

/**********************************************************************/

void CProcessedOrderHeader::SetTimeString( const char* szTime )
{
	CString strTemp = szTime;
	
	if ( strTemp.GetLength() == 6 )
		if ( ::TestNumeric( strTemp ) == TRUE )
			m_strTime = strTemp;
}

/**********************************************************************/

const char* CProcessedOrderHeader::GetDisplayDate()
{
	m_strDisplayDate = "";

	if ( m_strDate.GetLength() == 8 )
	{
		m_strDisplayDate.Format( "%s/%s/%s",
			m_strDate.Right(2),
			m_strDate.Mid(4,2),
			m_strDate.Left(4) );
	}

	return m_strDisplayDate;
}

/**********************************************************************/

CProcessedOrderHeader& CProcessedOrderHeader::operator= ( CProcessedOrderHeader& source )
{
	SetOrderNum		( source.GetOrderNum() );
	SetSuppNo		( source.GetSuppNo() );
	SetDateString	( source.GetDateString() );
	SetTimeString	( source.GetTimeString() );
	SetOrderType	( source.GetOrderType() );
	
	return *this;
}

/**********************************************************************/

