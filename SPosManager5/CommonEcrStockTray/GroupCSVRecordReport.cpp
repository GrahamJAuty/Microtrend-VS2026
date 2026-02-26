/**********************************************************************/
 
/**********************************************************************/
//#include "DealerFlags.h"
/**********************************************************************/
#include "GroupCSVArrayReport.h"
/**********************************************************************/

CGroupCSVRecordReport::CGroupCSVRecordReport() : CGroupCSVRecordBase()
{
	ClearRecord();
}

/**********************************************************************/

void CGroupCSVRecordReport::ClearRecord()
{
	m_nGroupNo = 1;
	m_strRepText = "";
	m_nParentGroupNo = 0;
}

/************************************************************************/
/*	convert to csv														*/
/************************************************************************/

void CGroupCSVRecordReport::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nGroupNo );
	csv.Add ( REPORTGROUPDB_VERSION_NO );
	csv.Add ( m_strRepText );
	csv.Add ( m_nParentGroupNo );
}

/************************************************************************/

void CGroupCSVRecordReport::ConvertFromCSV ( CCSV& csv )
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

void CGroupCSVRecordReport::V1ConvertFromCSV ( CCSV& csv )
{
	SetRepText ( csv.GetString(2) );
	SetParentGroupNo( csv.GetInt(5) );
}

/************************************************************************/

void CGroupCSVRecordReport::V2ConvertFromCSV ( CCSV& csv )
{
	SetRepText ( csv.GetString(2) );
	SetParentGroupNo( csv.GetInt(3) );
}

/**********************************************************************/
/*	Set Routines											*/
/**********************************************************************/

void CGroupCSVRecordReport::SetParentGroupNo ( int nGroupNo )
{
	Set ( m_nParentGroupNo, nGroupNo, ReportGroup::ParentGroupNo );
}

/**********************************************************************/

void CGroupCSVRecordReport::SetDefaultGroup( int nGroupNo )
{
	SetGroupNo( nGroupNo );
	SetRepText( GetDefaultText() );
	SetParentGroupNo(0);
}
	
/**********************************************************************/

void CGroupCSVRecordReport::SetRepText ( const char* sz )
{
	Set ( m_strRepText, StripSpaces ( sz ), ReportGroup::RepText );
}

/**********************************************************************/