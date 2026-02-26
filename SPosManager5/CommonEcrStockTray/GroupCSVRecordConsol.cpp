/**********************************************************************/
 
/**********************************************************************/
#include "GroupCSVArrayConsol.h"
/**********************************************************************/

CGroupCSVRecordConsol::CGroupCSVRecordConsol() : CGroupCSVRecordBase()
{
	ClearRecord();
}

/**********************************************************************/

void CGroupCSVRecordConsol::ClearRecord()
{
	m_nGroupNo = 1;
	m_strRepText = "";
}

/************************************************************************/
/*	convert to csv														*/
/************************************************************************/

void CGroupCSVRecordConsol::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nGroupNo );
	csv.Add ( CONSOLGROUPDB_VERSION_NO );
	csv.Add ( m_strRepText );
}

/************************************************************************/

void CGroupCSVRecordConsol::ConvertFromCSV ( CCSV& csv )
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

void CGroupCSVRecordConsol::V1ConvertFromCSV ( CCSV& csv )
{
	SetRepText ( csv.GetString(2) );
	//V1 RECORD HAS ADDITIONAL OBSOLETE FIELDS
}

/************************************************************************/

void CGroupCSVRecordConsol::V2ConvertFromCSV ( CCSV& csv )
{
	SetRepText ( csv.GetString(2) );
}

/**********************************************************************/
/*	Set Routines													  */
/**********************************************************************/

void CGroupCSVRecordConsol::SetDefaultGroup( int nGroupNo )
{
	SetGroupNo( nGroupNo );
	SetRepText( GetDefaultText() );
}
	
/**********************************************************************/

void CGroupCSVRecordConsol::SetRepText ( const char* sz )
{
	Set ( m_strRepText, StripSpaces ( sz ), ConsolGroup::RepText );
}

/**********************************************************************/
