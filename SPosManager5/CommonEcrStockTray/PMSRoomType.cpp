/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSRoomCSVArray.h"
/**********************************************************************/
#include "PMSRoomType.h"
/**********************************************************************/

CPMSRoomType::CPMSRoomType() : CSharedStringArray()
{
}

/**********************************************************************/

bool CPMSRoomType::Read( const char* szFilename )
{
	RemoveAt( 0, GetSize() );
	bool bResult = CSharedStringArray::Read( szFilename );
	
	bool bNew = ( GetSize() == 0 );

	while( GetSize() < 6 )
		Add( "" );

	while( GetSize() > 6 )
		RemoveAt( GetSize() - 1 );

	for ( int n = 1; n <= 6; n++ )
		SetComboFlag( n, 0, TRUE );

	if ( TRUE == bNew )
	{
		SetText( 1, 0, "Single" );
		SetText( 2, 0, "Double" );
		SetText( 2, 1, "Twin" );
		SetComboFlag( 2, 1, TRUE );
	}

	for ( int n = 1; n <= 6; n++ )
	{
		CString strText = GetText( n, 0 );

		if ( strText == "" )
		{
			strText.Format( "Room for %d", n );
			SetText( n, 0, strText );
		}
	}

	return bResult;
}

/**********************************************************************/

const char* CPMSRoomType::GetRoomTypes( int nSleeps )
{
	m_strRoomTypes = "";
	if ( ( nSleeps >= 1 ) && ( nSleeps <= GetSize() ) )
		m_strRoomTypes = GetAt( nSleeps - 1 );

	return m_strRoomTypes;
}

/**********************************************************************/

void CPMSRoomType::SetRoomTypes( int nSleeps, const char* szTypes )
{
	if ( ( nSleeps >= 1 ) && ( nSleeps <= GetSize() ) )
		SetAt( nSleeps - 1, szTypes );
}

/**********************************************************************/

const char* CPMSRoomType::GetText ( int nSleeps, int nSubType )
{
	m_strText = "Unknown";
	if ( ( nSleeps >= 1 ) && ( nSleeps <= GetSize() ) )
	{
		CCSV csv ( GetAt ( nSleeps - 1 ) );
		m_strText = csv.GetString ( nSubType * 2 );
		::TrimSpaces ( m_strText, FALSE );
	}
	else if ( 0 == nSleeps )
		m_strText = "Any";

	return m_strText;
}

/**********************************************************************/

bool CPMSRoomType::GetComboFlag ( int nSleeps, int nSubType )
{
	if ( ( nSleeps >= 1 ) &&  ( nSleeps <= GetSize() ) )
	{
		CCSV csv ( GetAt ( nSleeps - 1 ) );
		return csv.GetBool( ( nSubType * 2 ) + 1 );
	}
	else
		return FALSE;
}

/**********************************************************************/

void CPMSRoomType::SetText ( int nSleeps, int nSubType, const char* szText )
{
	if ( ( nSleeps >= 1 ) && ( nSleeps <= GetSize() ) )
	{
		CString strText = szText;
		if ( strText.GetLength() > PMSRoom::RoomTypeText.Max )
			strText = strText.Left( PMSRoom::RoomTypeText.Max );

		CCSV csv ( GetAt ( nSleeps - 1 ) );
		csv.SetAt ( nSubType * 2, strText );
		SetAt( nSleeps - 1, csv.GetLine() );
	}
}

/**********************************************************************/

void CPMSRoomType::SetComboFlag ( int nSleeps, int nSubType, bool bFlag )
{
	if ( ( nSleeps >= 1 ) &&  ( nSleeps <= GetSize() ) )
	{
		CCSV csv ( GetAt ( nSleeps - 1 ) );
		csv.SetAt ( ( nSubType * 2 ) + 1, bFlag );
		SetAt( nSleeps - 1, csv.GetLine() );
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
