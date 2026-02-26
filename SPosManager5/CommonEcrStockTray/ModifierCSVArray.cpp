/**********************************************************************/
 
/**********************************************************************/
//#include "globalfunctions.h"
/**********************************************************************/
#include "ModifierCSVArray.h"
/**********************************************************************/

CModifierCSVRecord::CModifierCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CModifierCSVRecord::ClearRecord()
{
	m_strName = "";
	m_dMultiplierForQty = 1.0;
	m_dMultiplierForPoints = 1.0;
}

/**********************************************************************/

void CModifierCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	switch ( csv.GetInt(0) )
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	case 2:	V2ConvertFromCSV ( csv );	break;
	}
}

/**********************************************************************/

void CModifierCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetName ( csv.GetString(1) );
	SetMultiplierForQty ( csv.GetDouble(2) );
	SetMultiplierForPoints( 1.0 );
}

/**********************************************************************/

void CModifierCSVRecord::V2ConvertFromCSV ( CCSV& csv )
{
	SetName ( csv.GetString(1) );
	SetMultiplierForQty ( csv.GetDouble(2) );
	SetMultiplierForPoints( csv.GetDouble(3) );
}

/**********************************************************************/
	
void CModifierCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( MODIFIER_VERSION );
	csv.Add ( m_strName );
	csv.Add ( m_dMultiplierForQty, 6);	
	csv.Add ( m_dMultiplierForPoints, 2 );
}

/**********************************************************************/

void CModifierCSVRecord::SetName ( const char* szName )
{
	CString strName = szName;
	::TrimSpaces ( strName, FALSE );
	m_strName = strName;	 
}

/**********************************************************************/

void CModifierCSVRecord::SetMultiplierForQty( double dMultiplier )
{
	if ( dMultiplier >= 0.0 )
		m_dMultiplierForQty = dMultiplier;
}

/**********************************************************************/

void CModifierCSVRecord::SetMultiplierForPoints( double dMultiplier )
{
	if ( dMultiplier >= 0.0 )
		m_dMultiplierForPoints = dMultiplier;
}

/**********************************************************************/

CModifierCSVArray::CModifierCSVArray() 
{
}

/**********************************************************************/

CModifierCSVArray::~CModifierCSVArray() 
{
}

/**********************************************************************/

int CModifierCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	while ( GetSize() < MODIFIER_COUNT )
	{
		CModifierCSVRecord Modifier;
		Modifier.SetName ( "" );
		Modifier.SetMultiplierForQty ( 1.0 );
		Modifier.SetMultiplierForPoints( 1.0 );
		Add ( Modifier );
	}

	while ( GetSize() > MODIFIER_COUNT )
		RemoveAt ( GetSize() - 1 );
	
	return nReply;
}

/**********************************************************************/

double CModifierCSVArray::GetMultiplierForQty( int nModifier )
{
	double dMultiplier = 1.0;

	if ( nModifier >= 1 && nModifier <= GetSize() )
	{
		CModifierCSVRecord Modifier;
		GetAt ( nModifier - 1, Modifier );
		dMultiplier = Modifier.GetMultiplierForQty();
	}

	return dMultiplier;
}

/**********************************************************************/

double CModifierCSVArray::GetMultiplierForPoints( int nModifier )
{
	double dMultiplier = 1.0;

	if ( nModifier >= 1 && nModifier <= GetSize() )
	{
		CModifierCSVRecord Modifier;
		GetAt ( nModifier - 1, Modifier );
		dMultiplier = Modifier.GetMultiplierForPoints();
	}

	return dMultiplier;
}

/**********************************************************************/

const char* CModifierCSVArray::GetDisplayName( int nModifier )
{
	m_strDisplayName = "";

	if ( nModifier >= 1 && nModifier <= GetSize() )
	{
		CModifierCSVRecord Modifier;
		GetAt ( nModifier - 1, Modifier );
		CString strTemp = Modifier.GetName();
		::TrimSpaces ( strTemp, FALSE );

		if ( strTemp == "" )
			m_strDisplayName.Format ( "Modifier %d", nModifier );
		else
			m_strDisplayName = strTemp;
	}
	
	return m_strDisplayName;
}

/**********************************************************************/

void CModifierCSVArray::SetName( int nModifier, const char* szName )
{
	if ( nModifier >= 1 && nModifier <= GetSize() )
	{
		CModifierCSVRecord Modifier;
		GetAt ( nModifier - 1, Modifier );
		Modifier.SetName( szName );
		SetAt ( nModifier - 1, Modifier );
	}
}

/**********************************************************************/

void CModifierCSVArray::SetMultiplierForQty( int nModifier, double dMultiplier )
{
	if ( nModifier >= 1 && nModifier <= GetSize() )
	{
		CModifierCSVRecord Modifier;
		GetAt ( nModifier - 1, Modifier );
		Modifier.SetMultiplierForQty( dMultiplier );
		SetAt ( nModifier - 1, Modifier );
	}
}

/**********************************************************************/

void CModifierCSVArray::SetMultiplierForPoints( int nModifier, double dMultiplier )
{
	if ( nModifier >= 1 && nModifier <= GetSize() )
	{
		CModifierCSVRecord Modifier;
		GetAt ( nModifier - 1, Modifier );
		Modifier.SetMultiplierForPoints( dMultiplier );
		SetAt ( nModifier - 1, Modifier );
	}
}

/**********************************************************************/
