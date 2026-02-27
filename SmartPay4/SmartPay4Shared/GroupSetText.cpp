/**********************************************************************/

#include "GroupSetText.h"

/**********************************************************************/

int CGroupSetText::Open ( const char* szFilename, int nMode )
{
	int nReply;
	if ( ( nReply = CSharedStringArray::Open ( szFilename, nMode ) ) != DB_ERR_SHARE )
	{
		int nCount = GetSize();
		if ( nCount == 0 )
		{
			Add ( "Student" );										// default values
			Add ( "Staff" );										// default values
			Add ( "Visitor" );										// default values
			nCount = 3;
		}

		for ( int i = nCount ; i < GROUPSET_MAX ; i++ )				// make up shortfall
			Add ( "" );												// with default values

		if ( nMode == DB_READONLY )								// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	return nReply;
}

/**********************************************************************/

CString CGroupSetText::GetText ( int nSetNo )
{
	CString strText = "";

	int nIndex = nSetNo - 1;
	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
		strText = GetAt ( nIndex );

	return strText;
}

/**********************************************************************/

void CGroupSetText::SetText ( int nSetNo, const char* szText )
{
	int nIndex = nSetNo - 1;

	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
		SetAt ( nIndex, szText );
}

//**********************************************************************/

//int CGroupSetText::GetCount()
//{
//	int nCount = 0;
//	for ( int nIndex = 0 ; nIndex < GROUPSET_MAX ; nIndex++ )
//	{
//		if ( GetText ( nIndex ) != "" )
//			++nCount;
//	}
//	return nCount;
//}

//**********************************************************************/
