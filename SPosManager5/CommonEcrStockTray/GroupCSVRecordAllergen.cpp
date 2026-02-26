/**********************************************************************/
 
/**********************************************************************/
//#include "globalfunctions.h"
/**********************************************************************/
#include "GroupCSVArrayAllergen.h"
/**********************************************************************/

CGroupCSVRecordAllergen::CGroupCSVRecordAllergen() : CGroupCSVRecordBase()
{
	ClearRecord();
}

/**********************************************************************/

void CGroupCSVRecordAllergen::ClearRecord()
{
	m_nGroupNo = 1;
	m_strEposText = "";
}

/************************************************************************/
/*	convert to csv														*/
/************************************************************************/

void CGroupCSVRecordAllergen::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nGroupNo );
	csv.Add ( ALLERGENDB_VERSION_NO );
	csv.Add ( m_strEposText );
}

/************************************************************************/

void CGroupCSVRecordAllergen::ConvertFromCSV ( CCSV& csv )
{
	SetGroupNo ( csv.GetInt(0) );
	switch ( csv.GetInt(1) )			/* version number */
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	}
}

/************************************************************************/
/*	Version 1															*/
/************************************************************************/

void CGroupCSVRecordAllergen::V1ConvertFromCSV ( CCSV& csv )
{
	SetEposText ( csv.GetString(2) );
}

/**********************************************************************/

const char* CGroupCSVRecordAllergen::GetReportText()
{
	m_strReportText = m_strEposText;
	::TrimSpaces ( m_strReportText, FALSE );
	return m_strReportText;
}

/**********************************************************************/
/*	Set Routines											*/
/**********************************************************************/

void CGroupCSVRecordAllergen::SetEposText ( const char* szEposText )
{
	Set ( m_strEposText, FilterEposText(szEposText), Allergen::EposText );
}

/**********************************************************************/

void CGroupCSVRecordAllergen::SetDefaultGroup( int nGroupNo )
{
	SetGroupNo( nGroupNo );
	SetEposText( GetDefaultText() );
}
	
/**********************************************************************/

const char* CGroupCSVRecordAllergen::GetDefaultText()
{
	m_strDefaultText.Format ( "Allergen %d", GetGroupNo() );
	return m_strDefaultText;
}

/**********************************************************************/

