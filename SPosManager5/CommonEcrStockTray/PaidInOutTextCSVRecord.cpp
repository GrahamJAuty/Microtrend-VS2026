/**********************************************************************/
 
/**********************************************************************/
//#include "globalfunctions.h"
/**********************************************************************/
#include "PaidInOutTextCSVArray.h"
/**********************************************************************/

CPaidInOutTextCSVRecord::CPaidInOutTextCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CPaidInOutTextCSVRecord::ClearRecord()
{
	m_strEposText = "";
}

/**********************************************************************/

void CPaidInOutTextCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( PAIDINOUT_VERSION_NO );
	csv.Add ( m_strEposText );
}

/**********************************************************************/

void CPaidInOutTextCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	switch ( csv.GetInt(0) )			/* version number */
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	}
}

/**********************************************************************/

void CPaidInOutTextCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetEposText( csv.GetString(1) );
}
	
/**********************************************************************/
/*	Set Routines											*/
/**********************************************************************/

void CPaidInOutTextCSVRecord::SetEposText ( const char* szEposText )
{
	CString strEposText = szEposText;
	::TrimSpaces( strEposText, FALSE );
	Set ( m_strEposText, strEposText, PaidInOutText::EposText );
}

/**********************************************************************/
