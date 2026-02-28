/**********************************************************************/
#include "DefTextEmailAddress.h"
/**********************************************************************/

CDefTextEmailAddress::CDefTextEmailAddress() : CSharedStringArray()
{
}

/**********************************************************************/

const char* CDefTextEmailAddress::GetNickname ( int nIndex )
{
	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		CCSV csv ( GetAt ( nIndex ) );
		m_strNickname = csv.GetString ( 0 );
		SolutionGlobalFunctions::TrimSpacesFromString( m_strNickname, FALSE );
	}
	else
		m_strNickname = "Unknown";

	return m_strNickname;
}

/**********************************************************************/

int CDefTextEmailAddress::GetEmailAddressCount( int nIndex )
{
	int nCount = 0;

	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
	{
		CCSV csv ( GetAt ( nIndex ) );
		
		nCount = csv.GetSize() - 1;

		if ( nCount > MAX_ADDRESSES_PER_NICKNAME )
			nCount = MAX_ADDRESSES_PER_NICKNAME;
	}

	return nCount;
}

/**********************************************************************/

const char* CDefTextEmailAddress::GetEmailAddress ( int nIndex, int nPos )
{
	int nSize = GetEmailAddressCount( nIndex );

	if ( ( nPos >= 0 ) && ( nPos < nSize ) )
	{
		CCSV csv ( GetAt ( nIndex ) );
		m_strEmailAddress = csv.GetString ( nPos + 1 );
		SolutionGlobalFunctions::TrimSpacesFromString( m_strEmailAddress, FALSE );
	}
	else
		m_strEmailAddress = "Unknown";

	return m_strEmailAddress;
}

/**********************************************************************/

const char* CDefTextEmailAddress::GetDisplayEmailAddress ( int nIndex )
{
	int nSize = GetEmailAddressCount( nIndex );

	if ( nSize > 0 )
	{
		CCSV csv ( GetAt ( nIndex ) );
		m_strDisplayEmailAddress = csv.GetString ( 1 );
		
		if ( nSize > 1 )
			m_strDisplayEmailAddress += "...";
	}
	else
		m_strDisplayEmailAddress = "Unknown";

	return m_strDisplayEmailAddress;
}

/**********************************************************************/

const char* CDefTextEmailAddress::GetComboText ( int nIndex )
{
	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		CString strName = GetNickname(nIndex);
		CString strEmail = GetDisplayEmailAddress(nIndex);
		
		if ( "" != strName )
			m_strComboText.Format ( "%s (%s)", ( const char* ) strName, ( const char* ) strEmail );
		else
			m_strComboText = strEmail;
	}
	else
		m_strComboText = "Unknown";

	return m_strComboText;
}

/**********************************************************************/

void CDefTextEmailAddress::SetAddressBookEntry ( int nIndex, const char* szName, CStringArray& arrayEmail )
{
	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		CString strName = szName;
		SolutionGlobalFunctions::TrimSpacesFromString( strName, FALSE );

		CCSV csv;
		csv.Add ( strName );
		
		for ( int n = 0; ( n < arrayEmail.GetSize() ) && ( n < MAX_ADDRESSES_PER_NICKNAME ); n++ )
		{
			CString strEmail = arrayEmail.GetAt(n);
			SolutionGlobalFunctions::TrimSpacesFromString( strEmail, FALSE );
			csv.Add ( strEmail );
		}

		SetAt ( nIndex, csv.GetLine() );
	}
}

/**********************************************************************/

void CDefTextEmailAddress::AddAddressBookEntry ( const char* szName, CStringArray& arrayEmail )
{
	CString strName = szName;
	SolutionGlobalFunctions::TrimSpacesFromString( strName, FALSE );

	CCSV csv;
	csv.Add ( strName );
		
	for ( int n = 0; ( n < arrayEmail.GetSize() ) && ( n < MAX_ADDRESSES_PER_NICKNAME ); n++ )
	{
		CString strEmail = arrayEmail.GetAt(n);
		SolutionGlobalFunctions::TrimSpacesFromString( strEmail, FALSE );
		csv.Add ( strEmail );
	}

	Add( csv.GetLine() );
}

/**********************************************************************/

void CDefTextEmailAddress::RemoveAddressBookEntry ( int nIndex )
{
	if ( nIndex >= 0 && nIndex < GetSize() )
		RemoveAt ( nIndex );
}

/**********************************************************************/

void CDefTextEmailAddress::CopyFrom( CDefTextEmailAddress& Source )
{
	RemoveAt( 0, GetSize() );

	for ( int n = 0; n < Source.GetSize(); n++ )
		Add( Source.GetAt(n) );
}

/**********************************************************************/

void CDefTextEmailAddress::SwapWithNext( int nIdx )
{
	if ( ( nIdx >= 0 ) && ( nIdx < GetSize() - 1 ) )
	{
		CString str = GetAt( nIdx );
		SetAt( nIdx, GetAt( nIdx + 1 ) );
		SetAt( nIdx + 1, str );
	}
}

/**********************************************************************/

int CDefTextEmailAddress::FindNameOrFirstAddress( const char* szName )
{
	CString strNameToFind = szName;
	
	SolutionGlobalFunctions::TrimSpacesFromString( strNameToFind, FALSE );
	if ( "" == strNameToFind )
		return -1;
	
	strNameToFind.MakeUpper();

	for ( int n = 0; n < GetSize(); n++ )
	{
		CString strArrayName = GetNickname(n);
		strArrayName.MakeUpper();

		if ( strArrayName == strNameToFind )
			return n;

		CString strArrayEmail = GetEmailAddress(n,0);
		strArrayEmail.MakeUpper();

		if ( strArrayEmail == strNameToFind )
			return n;		
	}
		
	return -1;
}

/**********************************************************************/
