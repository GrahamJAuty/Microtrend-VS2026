/**********************************************************************/
#include "SPOSKeyboardDefines.h"
/**********************************************************************/
#include "SPOSKeyboardKeyHelpers.h"
/**********************************************************************/

CSPOSKeyboardKeyHelpers::CSPOSKeyboardKeyHelpers()
{
}

/**********************************************************************/

const char* CSPOSKeyboardKeyHelpers::GetCombiText( int nType, __int64 nValue )
{
	CString strText;
	GetDefaultText( nType, nValue, strText, FALSE );

	CString strPrefix;
	switch ( nType )
	{
	case KEY_TYPE_FUNCTION:		strPrefix.Format ( "F%3.3d, ", nValue );	break;
	case KEY_TYPE_PAYMENT:		strPrefix.Format ( "P%2.2d, ", nValue );	break;
	case KEY_TYPE_CONTROL:		strPrefix.Format ( "C%2.2d, ", nValue );	break;
	case KEY_TYPE_MODIFIER:		
		if ( nValue == -1 )
			strPrefix = "M00, ";
		else
			strPrefix.Format ( "M%2.2d, ", nValue );	
		break;
	case KEY_TYPE_SVRPRESET:	strPrefix.Format ( "S%2.2d, ", nValue );	break;
	case KEY_TYPE_MACRO:		strPrefix.Format ( "M%4.4d, ", nValue );	break;
	case KEY_TYPE_EMPTY:		strPrefix.Format ( "E%2.2d, ", nValue );	break;
	case KEY_TYPE_LIST:			strPrefix.Format ( "L%5.5d, ", nValue );	break;
	default: strPrefix = "";	break;
	}
	
	::TrimSpaces ( strText, FALSE );
		
	m_strCombiText = strPrefix;
	m_strCombiText += strText;

	for ( int n = 0; n < m_strCombiText.GetLength(); n++ )
	{
		if ( m_strCombiText.GetAt(n) == '|' )
			m_strCombiText.SetAt(n, ' ' );
	}
	
	return m_strCombiText;
}

/**********************************************************************/

void CSPOSKeyboardKeyHelpers::GetDefaultText( int nType, __int64 nValue, CString& strText, bool bSharpPos )
{
	strText = "";
		
	CSPOSKeyboardDefaultTextArray* pFunctionTable = NULL;

	switch ( nType )
	{
	case KEY_TYPE_FUNCTION:			pFunctionTable = SPOSKeyboardManager.GetFunctionTable();	break;
	case KEY_TYPE_PAYMENT:			pFunctionTable = SPOSKeyboardManager.GetPaymentTable();		break;
	case KEY_TYPE_CONTROL:			pFunctionTable = SPOSKeyboardManager.GetControlTable();		break;
	case KEY_TYPE_MODIFIER:			pFunctionTable = SPOSKeyboardManager.GetModifierTable();	break;
	case KEY_TYPE_SVRPRESET:		pFunctionTable = SPOSKeyboardManager.GetSvrPresetTable();	break;
	case KEY_TYPE_MACRO:												
	case KEY_TYPE_LIST:													
	case KEY_TYPE_PLUNUMBER:		pFunctionTable = NULL;				break;
	case KEY_TYPE_EMPTY:	
	default:						pFunctionTable = SPOSKeyboardManager.GetEmptyTable();		break;
	}

	if ( pFunctionTable != NULL )
	{
		int nTextIdx;
		if ( pFunctionTable -> FindTextByValue ( nValue, nTextIdx ) == TRUE )
		{
			CSPOSKeyboardDefaultTextRecord TextRecord;
			pFunctionTable -> GetTextRecord( nTextIdx, TextRecord );
			TextRecord.GetDefaultText( strText );
		}
	}
	else if ( nType == KEY_TYPE_PLUNUMBER )
	{			
		int nPluIdx;
		if ( SPOSKeyboardManager.GetPluDatabase() -> FindPluItem( nValue, nPluIdx ) )
		{
			CSPOSKeyboardImportPluCSVRecord Plu;
			SPOSKeyboardManager.GetPluDatabase() -> GetAt ( nPluIdx, Plu );

			CString strTemp;
			if ( TRUE == bSharpPos )
			{
				strTemp = Plu.GetSharpPosDefaultText();
			}
			else
			{
				strTemp = Plu.GetBackOfficeDefaultText();
			}

			if ( ( "" == strTemp ) || ( "Plu" == strTemp ) )
				strText.Format ( "Plu|%I64d", nValue );
			else
				MakeEditableKeyText( strTemp, strText );
		}
		else
			strText.Format ( "Plu|%I64d", nValue );
	}
	else if ( nType == KEY_TYPE_LIST )
	{
		bool bGotName = FALSE;

		int nListIdx;
		if ( SPOSKeyboardManager.GetListManager() -> FindList( nValue, nListIdx ) == TRUE )
		{
			CSPOSKeyboardListRecord ListRecord;
			SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );
			
			CString strName = ListRecord.GetName();
			::TrimSpaces( strName, FALSE );

			if ( strName != "" )
			{
				bGotName = TRUE;
				MakeEditableKeyText( strName, strText );
			}
		}
		
		if ( bGotName == FALSE )
			strText.Format ( "List|%I64d", nValue );
	}
	else if ( nType == KEY_TYPE_MACRO )
	{
		bool bGotName = FALSE;

		int nMacroIdx;
		if ( DataManagerNonDb.CCMacroActive.FindMacroByNumber( (int)nValue, nMacroIdx ) == TRUE )
		{
			CCascadingMacroCSVRecord Macro;
			DataManagerNonDb.CCMacroActive.GetAt( nMacroIdx, Macro );
			
			CString strName = Macro.GetParentMacroName();
			::TrimSpaces( strName, FALSE );

			if ( strName != "" )
			{
				bGotName = TRUE;
				MakeEditableKeyText( strName, strText );
			}
		}
		
		if ( bGotName == FALSE )
			strText.Format ( "Macro|%I64d", nValue );
	}
}

/**********************************************************************/

void CSPOSKeyboardKeyHelpers::ValidateKeyText( CString& strText )
{
	::TrimSpaces( strText, FALSE );
}

/**********************************************************************/

void CSPOSKeyboardKeyHelpers::GetDefaultTextRecord( CSPOSKeyboardKeyRecord& KeyRecord, CSPOSKeyboardDefaultTextRecord& TextRecord )
{
	//SMALL WHITE TEXT ON BLACK BACKGROUND
	TextRecord.SetForeground( 1 );
	TextRecord.SetBackground( 4 );
	TextRecord.SetFontSize( 8 );
	TextRecord.SetBoldFlag( FALSE );
	TextRecord.SetUserDefaultText( "" );
	TextRecord.SetSystemDefaultText( "" );

	int nKeyType = KeyRecord.GetType();
	__int64 nValue = KeyRecord.GetValue();

	CSPOSKeyboardDefaultTextArray* pFunctionTable = NULL;

	switch ( nKeyType )
	{
	case KEY_TYPE_FUNCTION:		pFunctionTable = SPOSKeyboardManager.GetFunctionTable();	break;
	case KEY_TYPE_PAYMENT:		pFunctionTable = SPOSKeyboardManager.GetPaymentTable();		break;
	case KEY_TYPE_CONTROL:		pFunctionTable = SPOSKeyboardManager.GetControlTable();		break;
	case KEY_TYPE_MODIFIER:		pFunctionTable = SPOSKeyboardManager.GetModifierTable();	break;
	case KEY_TYPE_SVRPRESET:	pFunctionTable = SPOSKeyboardManager.GetSvrPresetTable();	break;
	case KEY_TYPE_LIST:			pFunctionTable = SPOSKeyboardManager.GetListTable();		break;	
	case KEY_TYPE_MACRO:		pFunctionTable = SPOSKeyboardManager.GetMacroTable();		break;
	case KEY_TYPE_PLUNUMBER:	pFunctionTable = NULL;										break;
	case KEY_TYPE_EMPTY:	
	default:					pFunctionTable = SPOSKeyboardManager.GetEmptyTable();		break;
	}

	if ( pFunctionTable != NULL )
	{
		int nIndex;
		if ( pFunctionTable -> FindTextByValue( nValue, nIndex ) == TRUE )
			pFunctionTable -> GetTextRecord ( nIndex, TextRecord );
	}
	
	switch( nKeyType )
	{
	case KEY_TYPE_PLUNUMBER:
	case KEY_TYPE_MACRO:
	case KEY_TYPE_LIST:
		CString strText;
		GetDefaultText( nKeyType, nValue, strText, FALSE );
		TextRecord.SetUserDefaultText( strText );
		TextRecord.SetSystemDefaultText( strText );
		break;
	}

	switch( nKeyType )
	{
	case KEY_TYPE_PLUNUMBER:
		{
			TextRecord.SetBoldFlag( SPOSKeyboardManager.GetLayoutOptions() -> GetDefaultBoldFlag() );
			
			int nIdx;
			if ( SPOSKeyboardManager.GetPluDatabase() -> FindPluItem( nValue, nIdx ) == TRUE )
			{
				CSPOSKeyboardImportPluCSVRecord PluRecord;
				SPOSKeyboardManager.GetPluDatabase() -> GetAt( nIdx, PluRecord );

				CString strImageFilename = PluRecord.GetImageFilename();

				if ( strImageFilename != "" )
				{
					TextRecord.SetImageFilename( strImageFilename );
				}
				else
				{
					int nDeptIdx;
					int nDeptNo = PluRecord.GetDeptNo();
					if ( DataManager.Department.FindDeptByNumber( nDeptNo, nDeptIdx ) == TRUE )
					{
						CDepartmentCSVRecord DeptRecord;
						DataManager.Department.GetAt( nDeptIdx, DeptRecord );
						TextRecord.SetImageFilename( DeptRecord.GetImageFilename() );
					}
				}
			}
		}
		break;
	}
}

/**********************************************************************/

void CSPOSKeyboardKeyHelpers::MakeEditableKeyText( const char* sz, CString& strText )
{
	//FIDDLE : EXPERIMENTAL CHANGE TO NOT ADD LINE BREAKS TO DEFAULT TEXT
	strText = sz;
	strText = strText.Left(78);

	/*
	CCSV csv ( sz, ' ', '"' );

	strText = "";

	int nTotalLen = 0;
	for ( int n = 0; n < csv.GetSize(); n++ )
	{
		CString str = csv.GetString(n);

		int nLength = str.GetLength();
		if ( nTotalLen + nLength > 80 )
		{
			strText += str.Left( 80 - nTotalLen );
			break;
		}

		strText += str;
		nTotalLen += nLength;

		if ( nTotalLen > 78 )
			break;

		strText += "|";
		nTotalLen += 2;
	}
	*/
}

/**********************************************************************/
