/**********************************************************************/
 
/**********************************************************************/
#include "GroupCSVArrayPayment.h"
/**********************************************************************/

CGroupCSVRecordPayment::CGroupCSVRecordPayment() : CGroupCSVRecordBase()
{
	ClearRecord();
}

/**********************************************************************/

void CGroupCSVRecordPayment::ClearRecord()
{
	m_nGroupNo = 1;
	m_strRepText = "";
}

/************************************************************************/
/*	convert to csv														*/
/************************************************************************/

void CGroupCSVRecordPayment::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nGroupNo );
	csv.Add ( PAYMENTGROUPDB_VERSION_NO );
	csv.Add ( m_strRepText );
}

/************************************************************************/

void CGroupCSVRecordPayment::ConvertFromCSV ( CCSV& csv )
{
	SetGroupNo ( csv.GetInt(0) );
	switch ( csv.GetInt(1) )			/* version number */
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	case 2:	V2ConvertFromCSV ( csv );	break;
	}
}

/************************************************************************/
/*	Version 1															*/
/************************************************************************/

void CGroupCSVRecordPayment::V1ConvertFromCSV ( CCSV& csv )
{
	SetRepText ( csv.GetString(2) );
	//V1 RECORD HAS SOME OBSOLETE FIELDS
}

/************************************************************************/

void CGroupCSVRecordPayment::V2ConvertFromCSV ( CCSV& csv )
{
	SetRepText ( csv.GetString(2) );
}

/**********************************************************************/
/*	Set Routines											*/
/**********************************************************************/

void CGroupCSVRecordPayment::SetDefaultGroup( int nGroupNo )
{
	SetGroupNo( nGroupNo );
	SetRepText( GetDefaultText() );
}
	
/**********************************************************************/

void CGroupCSVRecordPayment::SetRepText ( const char* sz )
{
	Set ( m_strRepText, StripSpaces ( sz ), PaymentGroup::RepText );
}

/**********************************************************************/