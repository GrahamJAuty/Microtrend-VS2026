/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSRoomCSVArray.h"
/**********************************************************************/
#include "PMSRoomGroup.h"
/**********************************************************************/

CPMSRoomGroup::CPMSRoomGroup() : CSharedStringArray()
{
}

/**********************************************************************/

bool CPMSRoomGroup::Read( const char* szFilename )
{
	RemoveAt( 0, GetSize() );
	bool bResult = CSharedStringArray::Read( szFilename );
	
	bool bNew = ( GetSize() == 0 );

	while( GetSize() < 5 )
		Add( "" );

	while( GetSize() > 5 )
		RemoveAt( GetSize() - 1 );

	if ( TRUE == bNew )
	{
		SetRoomGroupName( 1, "Standard" );
		SetRoomGroupName( 2, "Group 2" );
		SetRoomGroupName( 3, "Group 3" );
		SetRoomGroupName( 4, "Group 4" );
		SetRoomGroupName( 5, "Group 5" );
	}

	for ( int n = 1; n <= 5; n++ )
	{
		CString strName = GetRoomGroupName( n );

		if ( strName == "" )
		{
			if ( 1 == n )
				strName = "Standard";
			else
				strName.Format( "Group %d", n );

			SetRoomGroupName( n, strName );
		}
	}

	return bResult;
}

/**********************************************************************/

const char* CPMSRoomGroup::GetRoomGroupName( int n )
{
	m_strName = "";
	if ( ( n >= 1 ) && ( n <= GetSize() ) )
		m_strName = GetAt( n - 1 );

	return m_strName;
}

/**********************************************************************/

void CPMSRoomGroup::SetRoomGroupName( int n, const char* szName )
{
	if ( ( n >= 1 ) && ( n <= GetSize() ) )
		SetAt( n - 1, szName );
}

/**********************************************************************/
#endif
/**********************************************************************/
