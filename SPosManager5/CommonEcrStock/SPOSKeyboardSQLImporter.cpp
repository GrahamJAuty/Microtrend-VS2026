/**********************************************************************/
#include "SPOSKeyboardDefines.h"
#include "SQLIn.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "SPOSKeyboardSQLImporter.h"
/**********************************************************************/

CSPOSKeyboardSQLImporter::CSPOSKeyboardSQLImporter()
{
	Reset();
}

/**********************************************************************/

void CSPOSKeyboardSQLImporter::Reset()
{
	m_bGotFile = FALSE;
	m_nTotalListHeader = 0;
	m_nTotalListItem = 0;
	m_nTotalScreenMode = 0;
	m_nTotalSysValues = 0;
	m_nTotalMacros = 0;
	m_MacroBuffer.RemoveAll();
}

/**********************************************************************/

void CSPOSKeyboardSQLImporter::ImportFile( const char* szFilename, bool bImport, bool bWantLayout, bool bWantMacros )
{
	Reset();

	CSSFile fileSQL;
	if ( fileSQL.Open ( szFilename, "rb" ) == FALSE )
		return;

	m_bGotFile = TRUE;

	if ( FALSE == bImport )
	{
		if ( TRUE == bWantLayout )
		{
			CheckListHeaders( fileSQL );
			CheckListItems( fileSQL );
			CheckScreenModes( fileSQL );
			CheckSysValues( fileSQL );
		}
		
		if ( TRUE == bWantMacros )
		{
			CheckMacros( fileSQL );
		}
	}
	else
	{
		if ( TRUE == bWantLayout )
		{
			ImportListHeaders( fileSQL );
			ImportListItems( fileSQL );
			ImportScreenModes( fileSQL );
			ImportSysValues( fileSQL );
		}

		if ( TRUE == bWantMacros )
		{
			ImportMacros( fileSQL );
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardSQLImporter::CheckListHeaders(CSSFile& fileSQL)
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add("[ListNo]");
	csvHeader.Add("[Location]");
	csvHeader.Add("[Rows]");
	csvHeader.Add("[Cols]");
	csvHeader.Add("[NPages]");
	csvHeader.Add("[Description]");
	csvHeader.Add("[Staydown]");
	csvHeader.Add("[Link]");
	csvHeader.Add("[SelMin]");
	csvHeader.Add("[SelMax]");
	csvHeader.Add("[Foreground86]");
	csvHeader.Add("[Background86]");
	csvHeader.Add("[Border86]");
	csvHeader.Add("[FontSpecifier86]");
	csvHeader.Add("[Width86]");
	csvHeader.Add("[Height86]");
	csvHeader.Add("[ListType]");
	csvHeader.Add("[TableNoLo]");
	csvHeader.Add("[TableNoHi]");
	csvHeader.Add("[TableArea]");
	csvHeader.Add("[Priority]");
	csvHeader.Add("[StackMode]");
	csvHeader.Add("[GJA$NoOfferPluLink]");

	CSQLIn SQLIn(fileSQL, "ListHdrs", csvHeader.GetLine());

	StatusProgress.Lock(TRUE, "Checking List Headers");

	CString strBuffer;
	while (SQLIn.ReadLine(strBuffer, TRUE))
		m_nTotalListHeader++;

	StatusProgress.Unlock();
}

/**********************************************************************/

void CSPOSKeyboardSQLImporter::CheckListItems( CSSFile& fileSQL )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[ListNo]" );
	csvHeader.Add( "[PageNo]" );
	csvHeader.Add( "[ItemNo]" );
	csvHeader.Add( "[FuncType]" );
	csvHeader.Add( "[FuncValue]" );
	csvHeader.Add( "[ItemText]" );
	csvHeader.Add( "[Foreground]" );
	csvHeader.Add( "[Background]" );
	csvHeader.Add( "[Size]" );
	csvHeader.Add( "[mx]" );
	csvHeader.Add( "[my]" );
	csvHeader.Add( "[FontSpecifier]" );
	csvHeader.Add( "[AlignmentH]" );
	csvHeader.Add( "[AlignmentV]" );
	csvHeader.Add( "[Image]" );
	csvHeader.Add( "[ImageScale]" );
	csvHeader.Add( "[HideText]" );
	csvHeader.Add( "[TextDefaultOnly]" );
	csvHeader.Add( "[GJA$NoSort]" );

	CSQLIn SQLIn ( fileSQL, "ListItems", csvHeader.GetLine() );

	StatusProgress.Lock( TRUE, "Checking List Items" );

	CString strBuffer;
	while ( SQLIn.ReadLine( strBuffer, TRUE ) )
		m_nTotalListItem++;

	StatusProgress.Unlock();
}

/**********************************************************************/

void CSPOSKeyboardSQLImporter::CheckScreenModes( CSSFile& fileSQL )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[ScrnNo]" );
	csvHeader.Add( "[Description]" );
	csvHeader.Add( "[ListTop]" );
	csvHeader.Add( "[FListLoggedOff]" );
	csvHeader.Add( "[FListTrading]" );
	csvHeader.Add( "[FListPayment]" );
	csvHeader.Add( "[ListFastBar]" );
	csvHeader.Add( "[ScreenStyle]" );
	csvHeader.Add( "[ListTopPayment]" );
	csvHeader.Add( "[ListLoggedOff]" );
	csvHeader.Add( "[ScreenStyleLoggedOff]" );
	
	CSQLIn SQLIn ( fileSQL, "ScreenModes", csvHeader.GetLine() );

	StatusProgress.Lock( TRUE, "Checking Screen Modes" );

	CString strBuffer;
	while ( SQLIn.ReadLine( strBuffer, TRUE ) )
		m_nTotalScreenMode++;

	StatusProgress.Unlock();
}

/**********************************************************************/

void CSPOSKeyboardSQLImporter::CheckSysValues( CSSFile& fileSQL )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[FieldName]" );
	csvHeader.Add( "[FieldIndex]" );
	csvHeader.Add( "[iValue]" );
	
	CSQLIn SQLIn ( fileSQL, "SysValues", csvHeader.GetLine() );

	StatusProgress.Lock( TRUE, "Checking Colours" );

	CString strBuffer;
	while ( SQLIn.ReadLine( strBuffer, TRUE ) )
	{
		CCSV csvIn( strBuffer );
		
		CString strType = csvIn.GetString(0);
		
		if ( strType == "Colour" )
			m_nTotalSysValues++;
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CSPOSKeyboardSQLImporter::CheckMacros( CSSFile& fileSQL )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[MacNo]" );
	csvHeader.Add( "[MacText]" );
	csvHeader.Add( "[Description]" );
	
	CSQLIn SQLIn ( fileSQL, "Macros", csvHeader.GetLine() );

	StatusProgress.Lock( TRUE, "Checking Macros" );

	CString strBuffer;
	while ( SQLIn.ReadLine( strBuffer, TRUE ) )
		m_nTotalMacros++;

	StatusProgress.Unlock();
}

/**********************************************************************/

void CSPOSKeyboardSQLImporter::ImportListHeaders(CSSFile& fileSQL)
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add("[ListNo]");
	csvHeader.Add("[Location]");
	csvHeader.Add("[Rows]");
	csvHeader.Add("[Cols]");
	csvHeader.Add("[NPages]");
	csvHeader.Add("[Description]");
	csvHeader.Add("[Staydown]");
	csvHeader.Add("[Link]");
	csvHeader.Add("[SelMin]");
	csvHeader.Add("[SelMax]");
	csvHeader.Add("[Foreground86]");
	csvHeader.Add("[Background86]");
	csvHeader.Add("[Border86]");
	csvHeader.Add("[FontSpecifier86]");
	csvHeader.Add("[Width86]");
	csvHeader.Add("[Height86]");
	csvHeader.Add("[ListType]");
	csvHeader.Add("[TableNoLo]");
	csvHeader.Add("[TableNoHi]");
	csvHeader.Add("[TableArea]");
	csvHeader.Add("[Priority]");
	csvHeader.Add("[StackMode]");
	csvHeader.Add("[GJA$NoOfferPluLink]");

	CSQLIn SQLIn(fileSQL, "ListHdrs", csvHeader.GetLine());

	StatusProgress.Lock(TRUE, "Processing List Headers");

	CString strBuffer;
	while (SQLIn.ReadLine(strBuffer, TRUE))
	{
		CCSV csvIn(strBuffer);

		CSPOSKeyboardListRecord ListRecord;

		__int64 nListNo = csvIn.GetInt64(0);
		if (nListNo < 1 || nListNo > MAX_LIST_NO)
			continue;

		ListRecord.SetListNo(nListNo);

		CString strFlag = csvIn.GetString(1);

		if (strFlag == "L")		ListRecord.SetListType(LISTTYPE_ITEM_STANDARD);
		else if (strFlag == "L2")	ListRecord.SetListType(LISTTYPE_ITEM_LARGE);
		else if (strFlag == "F")	ListRecord.SetListType(LISTTYPE_FUNCTION_STANDARD);
		else if (strFlag == "F2")	ListRecord.SetListType(LISTTYPE_FUNCTION_LARGE);
		else if (strFlag == "F3")	ListRecord.SetListType(LISTTYPE_FUNCTION_BOTTOM);
		else						ListRecord.SetListType(LISTTYPE_ITEM_STANDARD);

		int nRows = csvIn.GetInt(2);
		int nCols = csvIn.GetInt(3);
		if (ListRecord.ValidateFormat(nRows, nCols) == FALSE)
			continue;

		ListRecord.SetFormat(nRows, nCols);

		int nListIdx;
		if (SPOSKeyboardManager.GetListManager()->FindOrInsertList(ListRecord.GetListNo(), nListIdx) == FALSE)
			continue;

		int nPageCount = csvIn.GetInt(4);
		if (nPageCount > MAX_LIST_PAGES)
			nPageCount = MAX_LIST_PAGES;

		ListRecord.SetName(csvIn.GetString(5));
		ListRecord.SetPageCount(nPageCount);
		ListRecord.SetStaydownFlag(csvIn.GetBool(6));
		ListRecord.SetNextList(csvIn.GetInt(7));
		ListRecord.SetMinItems(csvIn.GetInt(8));
		ListRecord.SetMaxItems(csvIn.GetInt(9));

		//V3 FIELDS WILL NOT BE PRESENT IN LEGACY DATA
		if (!csvIn.IsEmpty(10)) ListRecord.SetForeground86(csvIn.GetInt(10) + 1);
		if (!csvIn.IsEmpty(11)) ListRecord.SetBackground86(csvIn.GetInt(11) + 1);
		if (!csvIn.IsEmpty(12)) ListRecord.SetBorder86(csvIn.GetBool(12));

		if (!csvIn.IsEmpty(13))
		{
			CString strFont = csvIn.GetString(13);
			strFont.MakeUpper();

			int nLen = strFont.GetLength();
			if (nLen > 1)
			{
				ListRecord.SetBold86(strFont.Right(1) == "B");
				ListRecord.SetFontSize86(atoi(strFont.Left(nLen - 1)));
			}
		}

		if (!csvIn.IsEmpty(14)) ListRecord.SetWidth86(csvIn.GetInt(14));
		if (!csvIn.IsEmpty(15)) ListRecord.SetHeight86(csvIn.GetInt(15));
		if (!csvIn.IsEmpty(16)) ListRecord.SetListActionType(csvIn.GetInt(16));
		if (!csvIn.IsEmpty(17)) ListRecord.SetTableNoLo(csvIn.GetInt(17));
		if (!csvIn.IsEmpty(18)) ListRecord.SetTableNoHi(csvIn.GetInt(18));
		if (!csvIn.IsEmpty(19)) ListRecord.SetTableArea(csvIn.GetInt(19));
		if (!csvIn.IsEmpty(20)) ListRecord.SetPriorityFlag(csvIn.GetBool(20));
		if (!csvIn.IsEmpty(21)) ListRecord.SetStackMode(csvIn.GetInt(21));
		if (!csvIn.IsEmpty(22)) ListRecord.SetOfferPluLinkFlag(csvIn.GetBool(22) == FALSE);

		SPOSKeyboardManager.GetListManager()->SetListRecord(nListIdx, ListRecord);
		SPOSKeyboardManager.GetListManager()->InitialiseKeysForList(nListIdx);

		m_nTotalListHeader++;
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CSPOSKeyboardSQLImporter::ImportListItems( CSSFile& fileSQL )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[ListNo]" );
	csvHeader.Add( "[PageNo]" );
	csvHeader.Add( "[ItemNo]" );
	csvHeader.Add( "[FuncType]" );
	csvHeader.Add( "[FuncValue]" );
	csvHeader.Add( "[ItemText]" );
	csvHeader.Add( "[Foreground]" );
	csvHeader.Add( "[Background]" );
	csvHeader.Add( "[Size]" );
	csvHeader.Add( "[mx]" );
	csvHeader.Add( "[my]" );
	csvHeader.Add( "[FontSpecifier]" );
	csvHeader.Add( "[AlignmentH]" );
	csvHeader.Add( "[AlignmentV]" );
	csvHeader.Add( "[Image]" );
	csvHeader.Add( "[ImageScale]" );
	csvHeader.Add( "[HideText]" );
	csvHeader.Add( "[TextDefaultOnly]" );
	csvHeader.Add( "[GJA$NoSort]" );
	
	CSQLIn SQLIn ( fileSQL, "ListItems", csvHeader.GetLine() );

	bool bValidListPage = FALSE;
	int nLastListNo = 0;
	int nLastPageNo = 0;
	int nMaxItemNo = 0;

	StatusProgress.Lock( TRUE, "Processing List Items" );

	CString strBuffer;
	while ( SQLIn.ReadLine( strBuffer, TRUE ) )
	{
		CCSV csvIn( strBuffer );
		
		__int64 nListNo = csvIn.GetInt64(0);
		int nPageNo = csvIn.GetInt(1);
		int nItemNo = csvIn.GetInt(2);
		
		if ( nListNo != nLastListNo || nPageNo != nLastPageNo )
		{
			bValidListPage = TRUE;
			__int64 nLastListNo = nListNo;
			nLastPageNo = nPageNo;

			int nListIdx;
			if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == FALSE )
				bValidListPage = FALSE;
			else
			{
				CSPOSKeyboardListRecord ListRecord;
				SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );
				
				if ( nPageNo < 1 || nPageNo > ListRecord.GetPageCount() )
					bValidListPage = FALSE;
				else
					nMaxItemNo = ListRecord.GetKeysPerPage();
			}
		}

		if ( bValidListPage == FALSE )
			continue;
	
		if ( nItemNo < 1 || nItemNo > nMaxItemNo )
			continue;

		int nKeyIdx;
		if ( SPOSKeyboardManager.GetListManager() -> FindOrInsertKey( nListNo, nPageNo, nItemNo, nKeyIdx ) == FALSE )
			continue;

		int nKeyType = csvIn.GetInt(3);
		__int64 nKeyValue = csvIn.GetInt64(4);

		if ( ( KEY_TYPE_MODIFIER == nKeyType ) && ( -1 == nKeyValue ) )
			nKeyValue = 0;

		CSPOSKeyboardKeyRecord KeyRecord;
		KeyRecord.SetListNo( nListNo );
		KeyRecord.SetPageNo( nPageNo );
		KeyRecord.SetKeyNo( nItemNo );
		KeyRecord.SetType( nKeyType );
		KeyRecord.SetValue( nKeyValue );
		KeyRecord.SetForeground( csvIn.GetInt(6) + 1 );
		KeyRecord.SetBackground( csvIn.GetInt(7) + 1 );
		KeyRecord.SetLegacyTextSize( csvIn.GetInt(8) );
		KeyRecord.SetColSize( csvIn.GetInt(9) );
		KeyRecord.SetRowSize( csvIn.GetInt(10) );
		
		//V3 FIELDS WILL NOT BE PRESENT IN LEGACY DATA
		if ( !csvIn.IsEmpty(11) )
		{
			CString strFont = csvIn.GetString(11);
			strFont.MakeUpper();

			int nLen = strFont.GetLength();
			if ( nLen > 1 )
			{
				KeyRecord.SetBoldFlag( strFont.Right(1) == "B" );
				KeyRecord.SetFontSize( atoi( strFont.Left( nLen - 1 ) ) );
			}
		}

		if ( !csvIn.IsEmpty(12) ) KeyRecord.SetAlignmentH( csvIn.GetInt(12) );
		if ( !csvIn.IsEmpty(13) ) KeyRecord.SetAlignmentV( csvIn.GetInt(13) );
		if ( !csvIn.IsEmpty(14) ) KeyRecord.SetImageFilename( csvIn.GetString(14) );
		if ( !csvIn.IsEmpty(15) ) KeyRecord.SetImageScale( csvIn.GetInt(15) );
		if ( !csvIn.IsEmpty(16) ) KeyRecord.SetHideTextFlag( csvIn.GetBool(16) );
		if ( !csvIn.IsEmpty(17) ) KeyRecord.SetDefaultTextAlwaysFlag( csvIn.GetBool(17) );
		if ( !csvIn.IsEmpty(18) ) KeyRecord.SetGJANoSortFlag( csvIn.GetBool(18) );
		
		KeyRecord.SetKeyText( csvIn.GetString(5) );
		
		SPOSKeyboardManager.GetListManager() -> SetKeyRecord( nKeyIdx, KeyRecord );
		m_nTotalListItem++;
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CSPOSKeyboardSQLImporter::ImportScreenModes( CSSFile& fileSQL )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[ScrnNo]" );
	csvHeader.Add( "[Description]" );
	csvHeader.Add( "[ListTop]" );
	csvHeader.Add( "[FListLoggedOff]" );
	csvHeader.Add( "[FListTrading]" );
	csvHeader.Add( "[FListPayment]" );
	csvHeader.Add( "[ListFastBar]" );
	csvHeader.Add( "[ScreenStyle]" );
	csvHeader.Add( "[ListTopPayment]" );
	csvHeader.Add( "[ListLoggedOff]" );
	csvHeader.Add( "[ScreenStyleLoggedOff]" );
	
	CSQLIn SQLIn ( fileSQL, "ScreenModes", csvHeader.GetLine() );

	StatusProgress.Lock( TRUE, "Processing Screen Modes" );

	CString strBuffer;
	while ( SQLIn.ReadLine( strBuffer, TRUE ) )
	{
		CCSV csvIn( strBuffer );

		int nScreenNo = csvIn.GetInt(0);
		if ( nScreenNo < 1 || nScreenNo > MAX_SCREEN_NO )
			continue;

		CSPOSKeyboardScreenRecord ScreenRecord;
		ScreenRecord.SetScreenNo( nScreenNo );

		int nScreenIdx;
		if ( SPOSKeyboardManager.GetListManager() -> FindOrInsertScreen( nScreenNo, nScreenIdx ) == FALSE )
			continue;
	
		ScreenRecord.SetName( csvIn.GetString(1) );

		//V3 FIELDS WILL NOT BE PRESENT IN LEGACY DATA
		if ( !csvIn.IsEmpty(7) )	ScreenRecord.SetTradingScreenStyle( csvIn.GetInt(7) );
		if ( !csvIn.IsEmpty(10) )	ScreenRecord.SetLoggedOffScreenStyle( csvIn.GetInt(10) );

		//LISTTOP
		int nListIdx;
		__int64 nListNo = csvIn.GetInt64(2);
		if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == TRUE )
		{
			CSPOSKeyboardListRecord ListRecord;
			SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );

			if ( ScreenRecord.GetTradingListTypeTopLevel() == ListRecord.GetListType() )
				ScreenRecord.SetTradingListTopLevel( nListNo );
		}

		//LOGGED OFF LIST FUNCTION STANDARD
		nListNo = csvIn.GetInt64(3);
		if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == TRUE )
		{
			CSPOSKeyboardListRecord ListRecord;
			SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );

			if ( ListRecord.GetListType() == LISTTYPE_FUNCTION_STANDARD )
				ScreenRecord.SetLoggedOffListFunction( nListNo );
		}

		//TRADING LIST FUNCTION
		nListNo = csvIn.GetInt64(4);
		if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == TRUE )
		{
			CSPOSKeyboardListRecord ListRecord;
			SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );

			if ( ScreenRecord.GetTradingListTypeFunction() == ListRecord.GetListType() )
				ScreenRecord.SetTradingListFunction( nListNo );
		}

		//PAYMENT LIST FUNCTION
		nListNo = csvIn.GetInt64(5);
		if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == TRUE )
		{
			CSPOSKeyboardListRecord ListRecord;
			SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );

			if ( LISTTYPE_FUNCTION_STANDARD == ListRecord.GetListType() )
				ScreenRecord.SetPaymentListFunction( nListNo );
		}

		//PAYMENT LIST TOP LEVEL
		nListNo = csvIn.GetInt64(8);
		if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == TRUE )
		{
			CSPOSKeyboardListRecord ListRecord;
			SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );

			if ( LISTTYPE_ITEM_STANDARD == ListRecord.GetListType() )
				ScreenRecord.SetPaymentListTopLevel( nListNo );
		}

		//LOGGED OFF LIST FUNCTION LARGE
		nListNo = csvIn.GetInt64(9);
		if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == TRUE )
		{
			CSPOSKeyboardListRecord ListRecord;
			SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );

			if ( ListRecord.GetListType() == LISTTYPE_ITEM_LARGE )
				ScreenRecord.SetLoggedOffListFunctionLarge( nListNo );
		}

		SPOSKeyboardManager.GetListManager() -> SetScreenRecord( nScreenIdx, ScreenRecord );
		m_nTotalScreenMode++;
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CSPOSKeyboardSQLImporter::ImportSysValues( CSSFile& fileSQL )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[FieldName]" );
	csvHeader.Add( "[FieldIndex]" );
	csvHeader.Add( "[iValue]" );
	
	CSQLIn SQLIn ( fileSQL, "SysValues", csvHeader.GetLine() );

	StatusProgress.Lock( TRUE, "Processing Colours" );

	CString strBuffer;
	while ( SQLIn.ReadLine( strBuffer, TRUE ) )
	{
		CCSV csvIn( strBuffer );
		
		CString strType = csvIn.GetString(0);
		
		if ( strType == "Colour" )
		{
			SPOSKeyboardManager.GetColourTable() -> SetCurrentRGB( csvIn.GetInt(1), csvIn.GetInt(2) );
			m_nTotalSysValues++;
		}
		else if ( strType == "ColourInitial" )
			SPOSKeyboardManager.GetColourTable() -> SetDefaultRGB( csvIn.GetInt(1), csvIn.GetInt(2) );
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CSPOSKeyboardSQLImporter::ImportMacros( CSSFile& fileSQL )
{
	fileSQL.Seek(0, CFile::begin);

	CCSV csvHeader;
	csvHeader.Add( "[MacNo]" );
	csvHeader.Add( "[MacText]" );
	csvHeader.Add( "[Description]" );
	
	CSQLIn SQLIn ( fileSQL, "Macros", csvHeader.GetLine() );

	StatusProgress.Lock( TRUE, "Processing Macros" );

	CString strBuffer;
	while ( SQLIn.ReadLine( strBuffer, TRUE ) )
	{
		CCSV csvIn( strBuffer );

		int nMacroNo = csvIn.GetInt(0);
		if ( ( nMacroNo < 1 ) || ( nMacroNo > CascadingMacro::MacroNo.Max ) )
			continue;

		CSortedStringByInt item;
		item.m_nItem = nMacroNo;
		item.m_strItem = strBuffer;
		m_MacroBuffer.Consolidate( item );

		m_nTotalMacros++;
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CSPOSKeyboardSQLImporter::MergeMacroBufferKeyboard( bool bNewMacrosOnly )
{
	for ( int n = 0; n < m_MacroBuffer.GetSize(); n++ )
	{
		CSortedStringByInt item; 
		m_MacroBuffer.GetAt(n, item);

		int nMacroIdx;
		int nMacroNo = item.m_nItem;
		if ( DataManagerNonDb.CCMacroKeyboard.FindMacroByNumber( nMacroNo, nMacroIdx ) == FALSE )
		{
			CCascadingMacroCSVRecord Macro;
			CreateKeyboardMacro( Macro, nMacroNo, item.m_strItem );
			DataManagerNonDb.CCMacroKeyboard.InsertAt( nMacroIdx, Macro );
		}
		else if ( FALSE == bNewMacrosOnly )
		{
			CCascadingMacroCSVRecord Macro;
			CreateKeyboardMacro( Macro, nMacroNo, item.m_strItem );
			DataManagerNonDb.CCMacroKeyboard.SetAt( nMacroIdx, Macro );
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardSQLImporter::CreateKeyboardMacro( CCascadingMacroCSVRecord& Macro, int nMacroNo, CString& strDetail )
{
	CCSV csvDetail( strDetail );

	Macro.SetMacroNo( nMacroNo );
	Macro.SetParentMacroTypeExternal( NODE_PLACEHOLDER );
	Macro.SetParentMacroName( "" );
	Macro.SetParentMacroData( "" );
	Macro.SetLocalMacroFlag( TRUE );
	Macro.SetLocalMacroName( csvDetail.GetString(2) );
	Macro.SetLocalMacroData( csvDetail.GetString(1) );
}

/**********************************************************************/

void CSPOSKeyboardSQLImporter::MergeMacroBufferActive()
{
	for ( int n = 0; n < m_MacroBuffer.GetSize(); n++ )
	{
		CSortedStringByInt item; 
		m_MacroBuffer.GetAt(n, item);

		int nMacroIdx;
		int nMacroNo = item.m_nItem;
		if ( DataManagerNonDb.CCMacroActive.FindMacroByNumber( nMacroNo, nMacroIdx ) == FALSE )
		{
			CCSV csvDetail( item.m_strItem );

			CCascadingMacroCSVRecord Macro;

			Macro.SetMacroNo( nMacroNo );
			Macro.SetParentMacroTypeExternal( NODE_DBKEYBOARDSET );
			Macro.SetParentMacroName( csvDetail.GetString(2) );
			Macro.SetParentMacroData( csvDetail.GetString(1) );
			Macro.SetLocalMacroFlag( FALSE );
			Macro.SetLocalMacroName( "" );
			Macro.SetLocalMacroData( "" );
			
			DataManagerNonDb.CCMacroActive.InsertAt( nMacroIdx, Macro );
		}
	}
}

/**********************************************************************/

