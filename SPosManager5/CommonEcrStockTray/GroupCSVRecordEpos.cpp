/**********************************************************************/
#include "GroupCSVArrayEpos.h"
/**********************************************************************/

CGroupCSVRecordEpos::CGroupCSVRecordEpos()
{
	ClearRecord();
}

/**********************************************************************/

void CGroupCSVRecordEpos::ClearRecord()
{
	m_nGroupNo = 1;
	m_strRepText = "";
	m_strEposText = "";
	m_nSageNominalCode = 0;
	m_bSportsBooker = FALSE;
}

/************************************************************************/
/*	convert to csv														*/
/************************************************************************/

void CGroupCSVRecordEpos::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nGroupNo );
	csv.Add ( EPOSGROUPDB_VERSION_NO );
	csv.Add ( m_strRepText );
	csv.Add ( m_strEposText );
	csv.Add ( m_nSageNominalCode );
	csv.Add ( m_bSportsBooker );
}

/************************************************************************/

void CGroupCSVRecordEpos::ConvertFromCSV ( CCSV& csv )
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

void CGroupCSVRecordEpos::V1ConvertFromCSV ( CCSV& csv )
{
	SetRepText ( csv.GetString(2) );
	SetEposText ( csv.GetString(3) );
	SetSageNominalCode ( csv.GetInt(4) );
	SetSportsBookerFlag( csv.GetBool(6) );
}

/************************************************************************/

void CGroupCSVRecordEpos::V2ConvertFromCSV ( CCSV& csv )
{
	SetRepText ( csv.GetString(2) );
	SetEposText ( csv.GetString(3) );
	SetSageNominalCode ( csv.GetInt(4) );
	SetSportsBookerFlag( csv.GetBool(5) );
}

/************************************************************************/

const char* CGroupCSVRecordEpos::GetReportText()
{
	m_strReportText = m_strRepText;
	::TrimSpaces ( m_strReportText, FALSE );
	
	if ( m_strReportText == "" )
		m_strReportText = m_strEposText;

	return m_strReportText;
}

/**********************************************************************/
/*	Set Routines											*/
/**********************************************************************/

void CGroupCSVRecordEpos::SetSageNominalCode ( int n )
{
	Set ( m_nSageNominalCode, n, EposGroup::SageNominalCode );
}

/**********************************************************************/

void CGroupCSVRecordEpos::SetDefaultGroup( int nGroupNo )
{
	SetGroupNo( nGroupNo );
	SetEposText( GetDefaultText() );
	SetRepText( "" );
	SetSageNominalCode(0);
}
	
/**********************************************************************/

void CGroupCSVRecordEpos::SetSportsBookerFlag ( bool b )
{
	Set ( m_bSportsBooker, b );
}

/**********************************************************************/

void CGroupCSVRecordEpos::SetEposText ( const char* sz )
{
	Set ( m_strEposText, FilterEposText(sz), EposGroup::EposText );
}

/**********************************************************************/

void CGroupCSVRecordEpos::SetRepText ( const char* sz )
{
	Set ( m_strRepText, sz, EposGroup::RepText );
}

/**********************************************************************/
