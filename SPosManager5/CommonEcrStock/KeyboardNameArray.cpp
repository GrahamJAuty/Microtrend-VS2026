/**********************************************************************/
//#include "EcrmanOptionsIni.h"
//#include "globalfunctions.h"
#include "KeyboardSetCSVArray.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
/**********************************************************************/
#include "KeyboardNameArray.h"
/**********************************************************************/

CKeyboardNameArray::CKeyboardNameArray()
{
}

/**********************************************************************/

void CKeyboardNameArray::ReadNames()
{
	m_arrayNames.RemoveAll();

	for ( int nKbsIdx = 0; nKbsIdx < dbKeyboardSet.GetSize(); nKbsIdx++ )
	{
		int nNameIdx = m_arrayNames.GetSize();

		for ( int n = 0; n < MAX_KEYBOARDS_PER_LOCATION; n++ )
			m_arrayNames.Add( "" );

		CSSFile fileNames;
		CFilenameUpdater FnUp( SysFiles::KbSetNames, nKbsIdx );
		if ( fileNames.Open( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
		{
			CString strBuffer;
			if ( fileNames.ReadString( strBuffer ) == TRUE )
			{
				CCSV csv( strBuffer );

				if ( csv.GetInt(0) == 2 )
				{
					int nOffset = 1;
					for ( int x = 0; x < MAX_KEYBOARDS_PER_LOCATION; x++ )
						m_arrayNames.SetAt( nNameIdx++, csv.GetString( nOffset++ ) );
				}
			}
		}
	}

	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
	{
		int nNameIdx = m_arrayNames.GetSize();
		
		for ( int n = 0; n < MAX_KEYBOARDS_PER_LOCATION; n++ )
			m_arrayNames.Add( "" );

		CSSFile fileNames;
		CFilenameUpdater FnUp( SysFiles::KeyboardNames, nLocIdx );
		if ( fileNames.Open( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
		{
			CString strBuffer;
			if ( fileNames.ReadString( strBuffer ) == TRUE )
			{
				CCSV csv( strBuffer );

				if ( csv.GetInt(0) == 2 )
				{
					int nOffset = 1;
					for ( int x = 0; x < MAX_KEYBOARDS_PER_LOCATION; x++ )
						m_arrayNames.SetAt( nNameIdx++, csv.GetString( nOffset++ ) );
				}
			}
		}
	}
}

/**********************************************************************/

int CKeyboardNameArray::GetLocStartIdx( int nLocIdx )
{
	return MAX_KEYBOARDS_PER_LOCATION * ( dbKeyboardSet.GetSize() + nLocIdx );
}

/**********************************************************************/

int CKeyboardNameArray::GetKbSetStartIdx( int nKbsIdx )
{
	return MAX_KEYBOARDS_PER_LOCATION * nKbsIdx;
}

/**********************************************************************/

void CKeyboardNameArray::WriteKeyboardNames( CDataNodeInfoKeyboard& infoNode )
{
	if ( infoNode.m_nNodeType == NODE_LOCATION )
		WriteLocNames( infoNode.m_nLocIdx );
	else
		WriteKbSetNames( infoNode.m_nKbsIdx );
}

/**********************************************************************/

void CKeyboardNameArray::WriteLocNames( int nLocIdx )
{
	int nStartIdx = GetLocStartIdx( nLocIdx );
	if ( nStartIdx + MAX_KEYBOARDS_PER_LOCATION > m_arrayNames.GetSize() )
		return;

	CCSV csv;
	csv.Add(2);

	for ( int x = 0; x < MAX_KEYBOARDS_PER_LOCATION; x++ )
		csv.Add( m_arrayNames.GetAt( nStartIdx + x ) );

	CSSFile fileNames;
	CFilenameUpdater FnUp( SysFiles::KeyboardNames, nLocIdx );
	if ( fileNames.Open( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
		fileNames.WriteLine( csv.GetLine() );
}

/**********************************************************************/

void CKeyboardNameArray::WriteKbSetNames( int nKbsIdx )
{
	int nStartIdx = GetKbSetStartIdx( nKbsIdx );
	if ( nStartIdx + MAX_KEYBOARDS_PER_LOCATION > m_arrayNames.GetSize() )
		return;

	CCSV csv;
	csv.Add(2);

	for ( int x = 0; x < MAX_KEYBOARDS_PER_LOCATION; x++ )
		csv.Add( m_arrayNames.GetAt( nStartIdx + x ) );

	CSSFile fileNames;
	CFilenameUpdater FnUp( SysFiles::KbSetNames, nKbsIdx );
	if ( fileNames.Open( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
		fileNames.WriteLine( csv.GetLine() );
}

/**********************************************************************/

void CKeyboardNameArray::GetKeyboardName( CDataNodeInfoKeyboard& infoNode, CString& strName )
{
	if ( NODE_LOCATION == infoNode.m_nNodeType )
		GetLocKeyboardName( infoNode.m_nLocIdx, infoNode.m_nKbNo, strName );
	else
		GetKbSetKeyboardName( infoNode.m_nKbsIdx, infoNode.m_nKbNo, strName );
}

/**********************************************************************/

void CKeyboardNameArray::GetLocKeyboardName( int nLocIdx, int nKbNo, CString& strName )
{
	strName = "";

	if ( ( nLocIdx < 0 ) || ( nLocIdx >= dbLocation.GetSize() ) )
		return;

	if ( ( nKbNo < 1 ) || ( nKbNo > MAX_KEYBOARDS_PER_LOCATION ) )
		return;

	int nIdx = GetLocStartIdx( nLocIdx ) + nKbNo -1;

	if ( ( nIdx < 0 ) || ( nIdx >= m_arrayNames.GetSize() ) )
		return;

	strName = m_arrayNames.GetAt( nIdx );
}

/**********************************************************************/

void CKeyboardNameArray::GetKbSetKeyboardName( int nKbsIdx, int nKbNo, CString& strName )
{
	strName = "";

	if ( ( nKbsIdx < 0 ) || ( nKbsIdx >= dbKeyboardSet.GetSize() ) )
		return;

	if ( ( nKbNo < 1 ) || ( nKbNo > MAX_KEYBOARDS_PER_LOCATION ) )
		return;

	int nIdx = GetKbSetStartIdx( nKbsIdx ) + nKbNo -1;

	if ( ( nIdx < 0 ) || ( nIdx >= m_arrayNames.GetSize() ) )
		return;

	strName = m_arrayNames.GetAt( nIdx );
}

/**********************************************************************/

void CKeyboardNameArray::SetKeyboardName( CDataNodeInfoKeyboard& infoNode, CString& strName )
{
	if ( NODE_LOCATION == infoNode.m_nNodeType )
		SetLocKeyboardName( infoNode.m_nLocIdx, infoNode.m_nKbNo, strName );
	else
		SetKbSetKeyboardName( infoNode.m_nKbsIdx, infoNode.m_nKbNo, strName );
}

/**********************************************************************/

void CKeyboardNameArray::SetLocKeyboardName( int nLocIdx, int nKbNo, CString& strName )
{
	if ( ( nLocIdx < 0 ) || ( nLocIdx >= dbLocation.GetSize() ) )
		return;

	if ( ( nKbNo < 1 ) || ( nKbNo > MAX_KEYBOARDS_PER_LOCATION ) )
		return;

	int nIdx = GetLocStartIdx( nLocIdx ) + nKbNo - 1;

	if ( ( nIdx < 0 ) || ( nIdx >= m_arrayNames.GetSize() ) )
		return;

	m_arrayNames.SetAt( nIdx, strName );
}

/**********************************************************************/

void CKeyboardNameArray::SetKbSetKeyboardName( int nKbsIdx, int nKbNo, CString& strName )
{
	if ( ( nKbsIdx < 0 ) || ( nKbsIdx >= dbKeyboardSet.GetSize() ) )
		return;
	
	if ( ( nKbNo < 1 ) || ( nKbNo > MAX_KEYBOARDS_PER_LOCATION ) )
		return;

	int nIdx = GetKbSetStartIdx( nKbsIdx ) + nKbNo - 1;

	if ( ( nIdx < 0 ) || ( nIdx >= m_arrayNames.GetSize() ) )
		return;

	m_arrayNames.SetAt( nIdx, strName );
}

/**********************************************************************/

void CKeyboardNameArray::CopyKeyboardNames( CDataNodeInfoKeyboard& infoSource, CDataNodeInfoKeyboard& infoDest )
{
	int nSourceArrayIdx, nDestArrayIdx;

	switch( infoSource.m_nNodeType )
	{
	case NODE_DATABASE:
		nSourceArrayIdx = GetKbSetStartIdx( infoSource.m_nKbsIdx );
		break;

	case NODE_LOCATION:
	default:
		nSourceArrayIdx = GetLocStartIdx( infoSource.m_nLocIdx );
		break;
	}

	switch( infoDest.m_nNodeType )
	{
	case NODE_DATABASE:
		nDestArrayIdx = GetKbSetStartIdx( infoDest.m_nKbsIdx );
		break;

	case NODE_LOCATION:
	default:
		nDestArrayIdx = GetLocStartIdx( infoDest.m_nLocIdx );
		break;
	}

	if ( nSourceArrayIdx > m_arrayNames.GetSize() - MAX_KEYBOARDS_PER_LOCATION )
		return;

	if ( nDestArrayIdx > m_arrayNames.GetSize() - MAX_KEYBOARDS_PER_LOCATION )
		return;
		
	if ( nSourceArrayIdx != nDestArrayIdx )
		for ( int nKbNo = 1; nKbNo <= MAX_KEYBOARDS_PER_LOCATION; nKbNo++ )
			m_arrayNames.SetAt( nDestArrayIdx++, m_arrayNames.GetAt( nSourceArrayIdx++ ) );
}

/**********************************************************************/

const char* CKeyboardNameArray::GetKeyboardDisplayName( CDataNodeInfoKeyboard& infoNode )
{
	if ( NODE_LOCATION == infoNode.m_nNodeType )
		return GetLocKeyboardDisplayName( infoNode.m_nLocIdx, infoNode.m_nKbNo );
	else
		return GetKbSetKeyboardDisplayName( infoNode.m_nDbIdx, infoNode.m_nKbsIdx, infoNode.m_nKbNo );
}

/**********************************************************************/

const char* CKeyboardNameArray::GetLocKeyboardDisplayName( int nLocIdx, int nKbNo )
{
	m_strKeyboardDisplayName.Format ( "%s, Kbd %2.2d",
		dbLocation.GetName( nLocIdx ),
		nKbNo );

	CString strName = "";
	GetLocKeyboardName( nLocIdx, nKbNo, strName );
	::TrimSpaces( strName, FALSE );

	if ( strName != "" )
	{
		m_strKeyboardDisplayName += " ";
		m_strKeyboardDisplayName += strName;
	}
	
	return m_strKeyboardDisplayName;
}

/**********************************************************************/

const char* CKeyboardNameArray::GetKbSetKeyboardDisplayName( int nDbIdx, int nKbsIdx, int nKbNo )
{
	m_strKeyboardDisplayName.Format ( "%s, %s, Kbd %2.2d",
		dbDatabase.GetName( nDbIdx ),
		dbKeyboardSet.GetName( nKbsIdx ),
		nKbNo );

	CString strName = "";
	GetKbSetKeyboardName( nKbsIdx, nKbNo, strName );
	::TrimSpaces( strName, FALSE );

	if ( strName != "" )
	{
		m_strKeyboardDisplayName += " ";
		m_strKeyboardDisplayName += strName;
	}
	
	return m_strKeyboardDisplayName;
}

/**********************************************************************/