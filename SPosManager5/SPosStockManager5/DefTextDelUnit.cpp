/**********************************************************************/
 
/**********************************************************************/
//#include "globalfunctions.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DefTextDelUnit.h"
/**********************************************************************/

CDefTextDelUnit::CDefTextDelUnit() : CSharedStringArray()
{
}

/**********************************************************************/

const char* CDefTextDelUnit::GetText ( int nIndex )
{
	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		CCSV csv ( GetAt ( nIndex ) );
		m_strText = csv.GetString ( 0 );
		::TrimSpaces ( m_strText, FALSE );
		return m_strText;
	}
	return "Unknown";
}

/**********************************************************************/

const char* CDefTextDelUnit::GetComboText ( int nIndex )
{
	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		CCSV csv ( GetAt ( nIndex ) );
		CString strText = csv.GetString ( 0 );
		::TrimSpaces ( strText, FALSE );

		m_strComboText.Format ( "%s (%.3f)", ( const char* ) strText, csv.GetDouble(2) * csv.GetInt(1) );
		return m_strComboText;
	}
	return "Unknown";
}

/**********************************************************************/

int CDefTextDelUnit::GetDUItems ( int nIndex )
{
	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		CCSV csv ( GetAt ( nIndex ) );
		return csv.GetInt( 1 );
	}
	else
		return 1;
}

/**********************************************************************/

double CDefTextDelUnit::GetDUItemSize ( int nIndex )
{
	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		CCSV csv ( GetAt ( nIndex ) );
		return csv.GetDouble( 2 );
	}
	else
		return 1.0;
}

/**********************************************************************/

double CDefTextDelUnit::GetTotalSU ( int nIndex )
{
	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		CCSV csv ( GetAt ( nIndex ) );
		return csv.GetDouble(2) * csv.GetInt(1);
	}
	else
		return 1.0;
}

/**********************************************************************/

void CDefTextDelUnit::SetDelUnit ( int nIndex, const char* szText, int nDUItems, double dDUItemSize )
{
	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		CString strText = szText;
		::TrimSpaces ( strText, FALSE );
		
		if ( nDUItems < 1 )			nDUItems = 1;
		if ( dDUItemSize <= 0.0 )	dDUItemSize = 1.0;
		if ( strText == "" )		strText.Format ( "%d x %.3f", nDUItems, dDUItemSize );
		
		CCSV csv;
		csv.Add ( strText );
		csv.Add ( nDUItems );
		csv.Add ( dDUItemSize, 3 );
		SetAt ( nIndex, csv.GetLine() );
	}
}

/**********************************************************************/

void CDefTextDelUnit::AddDelUnit ( const char* szText, int nDUItems, double dDUItemSize )
{
	CCSV csv;

	CString strText = szText;
	::TrimSpaces ( strText, FALSE );
		
	if ( nDUItems < 1 )			nDUItems = 1;
	if ( dDUItemSize <= 0.0 )	dDUItemSize = 1.0;
	if ( strText == "" )		strText.Format ( "%d x %.3f", nDUItems, dDUItemSize );
		
	csv.Add ( strText );
	csv.Add ( nDUItems );
	csv.Add ( dDUItemSize, 3 );
	Add ( csv.GetLine() );
}

/**********************************************************************/

void CDefTextDelUnit::RemoveDelUnit ( int nIndex )
{
	if ( nIndex >= 0 && nIndex < GetSize() )
		RemoveAt ( nIndex );
}

/**********************************************************************/

void CDefTextDelUnit::CopyFrom( CDefTextDelUnit& Source )
{
	RemoveAt( 0, GetSize() );

	for ( int n = 0; n < Source.GetSize(); n++ )
		Add( Source.GetAt(n) );
}

/**********************************************************************/

void CDefTextDelUnit::SwapWithNext( int nIdx )
{
	if ( ( nIdx >= 0 ) && ( nIdx < GetSize() - 1 ) )
	{
		CString str = GetAt( nIdx );
		SetAt( nIdx, GetAt( nIdx + 1 ) );
		SetAt( nIdx + 1, str );
	}
}

/**********************************************************************/
