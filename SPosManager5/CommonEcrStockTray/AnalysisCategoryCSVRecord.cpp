/**********************************************************************/
#include "AnalysisCategoryCSVArray.h"
/**********************************************************************/

CAnalysisCategoryCSVRecord::CAnalysisCategoryCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CAnalysisCategoryCSVRecord::ClearRecord()
{
	m_strEposText = "";
	m_nType = 3;
}

/**********************************************************************/

void CAnalysisCategoryCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( ACAT_VERSION_NO );
	csv.Add ( m_strEposText );
	csv.Add ( m_nType );
}

/**********************************************************************/

void CAnalysisCategoryCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	switch ( csv.GetInt(0) )			/* version number */
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	}
}

/**********************************************************************/

void CAnalysisCategoryCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetEposText( csv.GetString(1) );
	SetType( csv.GetInt(2) );
}
	
/**********************************************************************/
/*	Set Routines											*/
/**********************************************************************/

void CAnalysisCategoryCSVRecord::SetEposText ( const char* szEposText )
{
	CString strEposText = szEposText;
	::TrimSpaces( strEposText, FALSE );
	Set ( m_strEposText, strEposText, AnalysisCategory::EposText );
}

/**********************************************************************/

void CAnalysisCategoryCSVRecord::SetType ( int nType )
{
	Set ( m_nType, nType, AnalysisCategory::Type );
}

/**********************************************************************/
