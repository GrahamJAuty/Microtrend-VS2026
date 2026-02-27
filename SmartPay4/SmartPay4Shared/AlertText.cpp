//*******************************************************************
#include "alerttext.h"
/**********************************************************************/

int CAlertText::Open ( const char* szFilename, int nMode )
{
	int nReply;
	if ( ( nReply = CSharedStringArray::Open ( szFilename, nMode ) ) != DB_ERR_SHARE )
	{
		for ( int i = GetSize() ; i < ALERT_MAX ; i++ )	// make up shortfall
			Add ( "" );										// with default values

		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	return nReply;
}

/**********************************************************************/

CString CAlertText::GetText ( int nAlertCode )
{
	CString strText = "";

	int nIndex = nAlertCode - 1;
	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
		strText = GetAt ( nIndex );

	return strText;
}


CString CAlertText::GetHotlistText ( int nAlertCode )
{
	CString strText = GetText ( nAlertCode );

	if ( strText == "" )
		strText.Format ( "Hotlist%d", nAlertCode );

	return strText;
}

/**********************************************************************/

void CAlertText::SetText ( int nAlertCode, const char* szText )
{
	int nIndex = nAlertCode - 1;

	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
		SetAt ( nIndex, szText );
}

/**********************************************************************/

int CAlertText::GetAlertCount()
{
	int nCount = 0;
	for ( int nIndex = 0 ; nIndex < ALERT_MAX ; nIndex++ )
	{
		if ( GetText ( nIndex ) != "" )
			++nCount;
	}
	return nCount;
}

//*******************************************************************
