/**********************************************************************/
#include "EposReportConsolParams.h"
/**********************************************************************/

CEposReportConsolParamsPluSales::CEposReportConsolParamsPluSales()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolParamsPluSales::Reset()
{
	m_bConsolTerm = FALSE;
	m_bConsolLoc = FALSE;
	m_bConsolDb = FALSE;
}

/**********************************************************************/

void CEposReportConsolParamsPluSales::LoadSettings( CString& str )
{
	Reset();

	CCSV csv( str );
	
	switch( csv.GetInt(0) )
	{
	case 1:
		m_bConsolTerm = csv.GetBool(1);
		m_bConsolLoc = csv.GetBool(2);
		m_bConsolDb = csv.GetBool(3);
		break;
	}
}

/**********************************************************************/

void CEposReportConsolParamsPluSales::SaveSettings( CString& str )
{
	CCSV csv;
	csv.Add(1);
	csv.Add( m_bConsolTerm );
	csv.Add( m_bConsolLoc );
	csv.Add( m_bConsolDb );
	str = csv.GetLine();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolParamsStandard::CEposReportConsolParamsStandard()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolParamsStandard::Reset()
{
	m_bConsolTerm = FALSE;
	m_bConsolLoc = FALSE;
	m_bConsolDb = FALSE;
	m_bConsolSys = FALSE;
}

/**********************************************************************/

void CEposReportConsolParamsStandard::LoadSettings( CString& str )
{
	Reset();

	CCSV csv( str );
	
	switch( csv.GetInt(0) )
	{
	case 1:
		m_bConsolTerm = csv.GetBool(1);
		m_bConsolLoc = csv.GetBool(2);
		m_bConsolDb = csv.GetBool(3);
		m_bConsolSys = csv.GetBool(4);
		break;
	}
}

/**********************************************************************/

void CEposReportConsolParamsStandard::SaveSettings( CString& str )
{
	CCSV csv;
	csv.Add(1);
	csv.Add( m_bConsolTerm );
	csv.Add( m_bConsolLoc );
	csv.Add( m_bConsolDb );
	csv.Add( m_bConsolSys );
	str = csv.GetLine();
}

/**********************************************************************/
