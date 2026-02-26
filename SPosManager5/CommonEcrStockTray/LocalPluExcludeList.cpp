/**********************************************************************/
 
/**********************************************************************/
//#include "globalfunctions.h"
/**********************************************************************/
#include "LocalPluExcludeList.h"
/**********************************************************************/

CLocalPluIncludeExcludeList::CLocalPluIncludeExcludeList() : CFileStringArray() 
{
}

/**********************************************************************/

void CLocalPluIncludeExcludeList::Read( const char* szFilename )
{
	RemoveAll();
	CFileStringArray::Read ( szFilename );
}

/**********************************************************************/

void CLocalPluIncludeExcludeList::Write( const char* szFilename )
{
	CFileStringArray::Write ( szFilename );
}

/**********************************************************************/

void CLocalPluIncludeExcludeList::RemovePluNo ( __int64 nPluNo )
{
	int nIndex;
	if ( FindPluNo ( nPluNo, nIndex ) == TRUE )
		RemoveAt ( nIndex );
}

/**********************************************************************/

void CLocalPluIncludeExcludeList::AddPluNo ( __int64 nPluNo )
{
	int nIndex;
	if ( FindPluNo ( nPluNo, nIndex ) == FALSE )
	{
		CString strPluNo;
		::FormatPluNo( nPluNo, strPluNo );
		InsertAt ( nIndex, strPluNo );
	}
}

/**********************************************************************/

bool CLocalPluIncludeExcludeList::FindPluNo( __int64 nPluNo, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	CString strPluNoToFind;
	::FormatPluNo( nPluNo, strPluNoToFind );
	
	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;
		CString strPluNo = GetAt ( nIndex );
		
		if ( strPluNo < strPluNoToFind )
			nStart = nIndex + 1;

		if ( strPluNo > strPluNoToFind )
			nEnd = nIndex - 1;

		if ( strPluNo == strPluNoToFind )
			return TRUE;
	}
	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

