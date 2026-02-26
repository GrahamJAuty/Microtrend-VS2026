/**********************************************************************/
#include "SQLOut.h"
/**********************************************************************/
#include "SPOSKeyboardSQLExporter.h"
/**********************************************************************/

CSPOSKeyboardSQLExporter::CSPOSKeyboardSQLExporter( int nSPOSVersion )
{
	m_nSPOSVersion = nSPOSVersion;
	Reset();
}

/**********************************************************************/

void CSPOSKeyboardSQLExporter::Reset()
{
	m_bGotFile = FALSE;
	m_nTotalListHeader = 0;
	m_nTotalListItem = 0;
	m_nTotalScreenMode = 0;
	m_nTotalSysValues = 0;
	m_nTotalMacros = 0;
	m_arrayPluList.RemoveAll();
}

/**********************************************************************/

void CSPOSKeyboardSQLExporter::ExportFileForDownload( const char* szFilename, bool bClearMacros )
{
	ExportFile( szFilename, TRUE, TRUE, bClearMacros );
}

/**********************************************************************/

void CSPOSKeyboardSQLExporter::ExportFileForSave( const char* szFilename, bool bIncludeMacros )
{
	ExportFile( szFilename, FALSE, bIncludeMacros, TRUE );
}

/**********************************************************************/

void CSPOSKeyboardSQLExporter::ExportFile( const char* szFilename, bool bForDownload, bool bIncludeMacros, bool bClearMacros )
{
	Reset();

	CSSFile fileSQL;
	if ( fileSQL.Open ( szFilename, "wb" ) == FALSE )
	{
		if ( DealerFlags.GetLogFileWritesFlag() == TRUE )
			SPOSKeyboardManager.GetMessageLogger() -> LogFileWrite( szFilename, 1005, GetLastError() );
		
		return;
	}

	fileSQL.WriteLine( "-->FILEBEGIN" );

	m_bGotFile = TRUE;
	ExportListHeaders( fileSQL, bForDownload );
	ExportListItems( fileSQL, bForDownload );
	ExportScreenModes( fileSQL );
	ExportSysValues( fileSQL );

	if ( TRUE == bIncludeMacros )
	{
		ExportMacros( fileSQL, bClearMacros );
	}
	
	fileSQL.WriteLine( "-->FILEEND" );
	fileSQL.Close();

	if ( DealerFlags.GetLogFileWritesFlag() == TRUE )
		SPOSKeyboardManager.GetMessageLogger() -> LogFileWrite( szFilename, 0, 0 );

	if ( FALSE == bForDownload )
	{
		CString strExportPath = SPOSKeyboardManager.GetLayoutOptions() -> GetDataPath();
		strExportPath += "\\PluNum.dat";

		CSSFile filePlu;
		if ( filePlu.Open( strExportPath, "wb" ) == TRUE )
		{
			for ( int n = 0; n < m_arrayPluList.GetSize(); n++ )
			{
				CSortedInt64Item item;
				m_arrayPluList.GetAt( n, item );
				
				CString str;
				str.Format( "%I64d", item.m_nItem );
				filePlu.WriteLine( str );
			}
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardSQLExporter::ExportListHeaders( CSSFile& fileSQL, bool bForDownload )
{
	bool bWantTableArea = FALSE;
	bool bWantPriority = FALSE;
	bool bWantStackMode = FALSE;
	
	switch( m_nSPOSVersion )
	{
	case SPOS_V4_321:
	case SPOS_V4_389:
	case SPOS_V4_423:
	case SPOS_V4_425:
	case SPOS_V4_494:
	case SPOS_V4_614:
	case SPOS_V4_634:
	case SPOS_V4_682:
	case SPOS_V4_719:
	case SPOS_V4_753:
	case SPOS_V4_780:
	case SPOS_V4_827:
	case SPOS_V4_837:
	case SPOS_V4_861:
	case SPOS_V4_958:
	case SPOS_V4_992:
	case SPOS_V4_999:
	case SPOS_V4_1071:
		bWantPriority = TRUE;
		break;

	case SPOS_V4_1092:
	case SPOS_V4_1096:
	case SPOS_V4_1124:
	case SPOS_V4_1216:
	case SPOS_V4_1300:
	case SPOS_V4_1392:
		bWantPriority = TRUE;
		bWantStackMode = TRUE;
		break;

	case SPOS_V4_1510:
	case SPOS_V4_1519:
	case SPOS_V4_1522:
	case SPOS_V4_1525:
	case SPOS_V4_1734:
	case SPOS_V4_1901:
	case SPOS_V4_1977:
	case SPOS_V4_2012:
	case SPOS_V4_2067:
	case SPOS_V4_2133:
	case SPOS_V4_2161:
	case SPOS_V4_2455:
	case SPOS_V4_2606:
	case SPOS_V4_2_369:
	case SPOS_V4_2_631:
	case SPOS_V4_2_714:
	case SPOS_V4_2_919:
	case SPOS_V4_2_959:
	case SPOS_V4_2_1081:
	case SPOS_V4_2_1087:
	case SPOS_V4_2_1384:
	case SPOS_V4_2_1456:
	case SPOS_V4_2_1530:
	case SPOS_V4_2_1579:
	case SPOS_V4_2_1605:
	case SPOS_V4_2_1618:
	case SPOS_V4_2_1622:
	case SPOS_V4_3_18:
	case SPOS_V4_3_93:
	case SPOS_V4_3_104:
		bWantTableArea = TRUE;
		bWantPriority = TRUE;
		bWantStackMode = TRUE;
		break;
	}

	CCSV csvHeader;
	csvHeader.Add( "[ListNo]" );
	csvHeader.Add( "[Description]" );
	csvHeader.Add( "[Rows]" );
	csvHeader.Add( "[Cols]" );
	csvHeader.Add( "[NPages]" );
	csvHeader.Add( "[Staydown]" );
	csvHeader.Add( "[Link]" );
	csvHeader.Add( "[SelMin]" );
	csvHeader.Add( "[SelMax]" );
	csvHeader.Add( "[Location]" );
	csvHeader.Add( "[Foreground86]" );
	csvHeader.Add( "[Background86]" );
	csvHeader.Add( "[Border86]" );
	csvHeader.Add( "[FontSpecifier86]" );
	csvHeader.Add( "[Width86]" );
	csvHeader.Add( "[Height86]" );
	csvHeader.Add( "[ListType]" );
	csvHeader.Add( "[TableNoLo]" );
	csvHeader.Add( "[TableNoHi]" );

	if (TRUE == bWantTableArea)
	{
		csvHeader.Add("[TableArea]");
	}

	if (TRUE == bWantPriority)
	{
		csvHeader.Add("[Priority]");
	}
	
	if (TRUE == bWantStackMode)
	{
		csvHeader.Add("[StackMode]");
	}

	if (FALSE == bForDownload)
	{
		csvHeader.Add("[GJA$NoOfferPluLink]");
	}
	
	csvHeader.Add( "[V$Dirty]" );

	CSQLOut SQLOut ( fileSQL, "ListHdrs", csvHeader.GetLine() );
	SQLOut.WriteDelete();

	for ( int nListIdx = 0; nListIdx < SPOSKeyboardManager.GetListManager() -> GetListCount(); nListIdx++ )
	{
		CSPOSKeyboardListRecord ListRecord;
		SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );
		SQLOut.Add( ListRecord.GetListNo() );
		SQLOut.Add( ListRecord.GetName() );
		SQLOut.Add( ListRecord.GetRowCount() );
		SQLOut.Add( ListRecord.GetColumnCount() );
		SQLOut.Add( ListRecord.GetPageCount() );
		SQLOut.Add( ListRecord.GetStaydownFlag() );
		SQLOut.Add( ListRecord.GetNextList() );
		SQLOut.Add( ListRecord.GetMinItems() );
		SQLOut.Add( ListRecord.GetMaxItems() );

		CString strListType = "L";
		switch( ListRecord.GetListType() )
		{
		case LISTTYPE_ITEM_STANDARD:		strListType = "L";		break;
		case LISTTYPE_ITEM_LARGE:			strListType = "L2";		break;
		case LISTTYPE_FUNCTION_STANDARD:	strListType = "F";		break;
		case LISTTYPE_FUNCTION_LARGE:		strListType = "F2";		break;
		case LISTTYPE_FUNCTION_BOTTOM:		strListType = "F3";		break;
		}

		SQLOut.Add( strListType );
		SQLOut.Add( ListRecord.GetForeground86() - 1 );
		SQLOut.Add( ListRecord.GetBackground86() - 1 );
		SQLOut.Add( ListRecord.GetBorder86() );
		
		CString strFont;
		strFont.Format( "%d", ListRecord.GetFontSize86() );
		strFont += ( ListRecord.GetBold86() ? "B" : "N" );
		SQLOut.Add( strFont );

		SQLOut.Add( ListRecord.GetWidth86() );
		SQLOut.Add( ListRecord.GetHeight86() );
		SQLOut.Add( ListRecord.GetListActionType() );
		SQLOut.Add( ListRecord.GetTableNoLo() );
		SQLOut.Add( ListRecord.GetTableNoHi() );

		if (TRUE == bWantTableArea)
		{
			SQLOut.Add(ListRecord.GetTableArea());
		}

		if (TRUE == bWantPriority)
		{
			SQLOut.Add(ListRecord.GetPriorityFlag());
		}

		if (TRUE == bWantStackMode)
		{
			SQLOut.Add(ListRecord.GetStackMode());
		}

		if (FALSE == bForDownload)
		{
			SQLOut.Add(ListRecord.GetOfferPluLinkFlag() ? 0 : 1);
		}

		SQLOut.Add( 1 );
		
		SQLOut.WriteInsert();
		m_nTotalListHeader++;
	}
}

/**********************************************************************/

void CSPOSKeyboardSQLExporter::ExportListItems( CSSFile& fileSQL, bool bForDownload )
{
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

	switch( m_nSPOSVersion )
	{
	case SPOS_V4_958:
	case SPOS_V4_992:
	case SPOS_V4_999:
	case SPOS_V4_1071:
	case SPOS_V4_1092:
	case SPOS_V4_1096:
	case SPOS_V4_1124:
	case SPOS_V4_1216:
	case SPOS_V4_1300:
	case SPOS_V4_1392:
	case SPOS_V4_1510:
	case SPOS_V4_1519:
	case SPOS_V4_1522:
	case SPOS_V4_1525:
	case SPOS_V4_1734:
	case SPOS_V4_1901:
	case SPOS_V4_1977:
	case SPOS_V4_2012:
	case SPOS_V4_2067:
	case SPOS_V4_2133:
	case SPOS_V4_2161:
	case SPOS_V4_2455:
	case SPOS_V4_2606:
	case SPOS_V4_2_369:
	case SPOS_V4_2_631:
	case SPOS_V4_2_714:
	case SPOS_V4_2_919:
	case SPOS_V4_2_959:
	case SPOS_V4_2_1081:
	case SPOS_V4_2_1087:
	case SPOS_V4_2_1384:
	case SPOS_V4_2_1456:
	case SPOS_V4_2_1530:
	case SPOS_V4_2_1579:
	case SPOS_V4_2_1605:
	case SPOS_V4_2_1618:
	case SPOS_V4_2_1622:
	case SPOS_V4_3_18:
	case SPOS_V4_3_93:
	case SPOS_V4_3_104:
		csvHeader.Add( "[TextDefaultOnly]" );
		break;
	}

	if ( FALSE == bForDownload )
		csvHeader.Add( "[GJA$NoSort]" );

	CSQLOut SQLOut ( fileSQL, "ListItems", csvHeader.GetLine() );
	SQLOut.WriteDelete();

	for ( int nKeyIdx = 0; nKeyIdx < SPOSKeyboardManager.GetListManager() -> GetKeyCount(); nKeyIdx++ )
	{
		CSPOSKeyboardKeyRecord KeyRecord;
		SPOSKeyboardManager.GetListManager() -> GetKeyRecord( nKeyIdx, KeyRecord );

		int nSharpPosKeyType = KeyRecord.GetType();
		__int64 nSharpPosKeyValue = KeyRecord.GetValue();

		if ( ( KEY_TYPE_MODIFIER == nSharpPosKeyType ) && ( 0 == nSharpPosKeyValue ) )
		{
			nSharpPosKeyValue = -1;

			if ( TRUE == bForDownload )
			{
				switch( m_nSPOSVersion )
				{
				case SPOS_V4_1522:
				case SPOS_V4_1525:
				case SPOS_V4_1734:
				case SPOS_V4_1901:
				case SPOS_V4_1977:
				case SPOS_V4_2012:
				case SPOS_V4_2067:
				case SPOS_V4_2133:
				case SPOS_V4_2161:
				case SPOS_V4_2455:
				case SPOS_V4_2606:
				case SPOS_V4_2_369:
				case SPOS_V4_2_631:
				case SPOS_V4_2_714:
				case SPOS_V4_2_919:
				case SPOS_V4_2_959:
				case SPOS_V4_2_1081:
				case SPOS_V4_2_1087:
				case SPOS_V4_2_1384:
				case SPOS_V4_2_1456:
				case SPOS_V4_2_1530:
				case SPOS_V4_2_1579:
				case SPOS_V4_2_1605:
				case SPOS_V4_2_1618:
				case SPOS_V4_2_1622:
				case SPOS_V4_3_18:
				case SPOS_V4_3_93:
				case SPOS_V4_3_104:
				break; 

				default:
					nSharpPosKeyType = KEY_TYPE_EMPTY;
					nSharpPosKeyValue = 0;
					break;
				}
			}
		}

		SQLOut.Add( KeyRecord.GetListNo() );
		SQLOut.Add( KeyRecord.GetPageNo() );
		SQLOut.Add( KeyRecord.GetKeyNo() );
		SQLOut.Add( nSharpPosKeyType );
		SQLOut.Add( nSharpPosKeyValue );
		SQLOut.Add( KeyRecord.GetTrimmedKeyText() );
		
		SQLOut.Add( KeyRecord.GetForeground() - 1 );
		SQLOut.Add( KeyRecord.GetBackground() - 1 );
		SQLOut.Add( KeyRecord.GetLegacyTextSize() );
		SQLOut.Add( KeyRecord.GetColSize() );
		SQLOut.Add( KeyRecord.GetRowSize() );

		CString strFont;
		strFont.Format( "%d", KeyRecord.GetFontSize() );
		strFont += ( KeyRecord.GetBoldFlag() ? "B" : "N" );
		SQLOut.Add( strFont );

		SQLOut.Add( KeyRecord.GetAlignmentH() );
		SQLOut.Add( KeyRecord.GetAlignmentV() );
		SQLOut.Add( KeyRecord.GetImageFilename() );
		SQLOut.Add( KeyRecord.GetImageScale() );
		SQLOut.Add( KeyRecord.GetHideTextFlag() ? 1 : 0 );

		switch( m_nSPOSVersion )
		{
		case SPOS_V4_958:
		case SPOS_V4_992:
		case SPOS_V4_999:
		case SPOS_V4_1071:
		case SPOS_V4_1092:
		case SPOS_V4_1096:
		case SPOS_V4_1124:
		case SPOS_V4_1216:
		case SPOS_V4_1300:
		case SPOS_V4_1392:
		case SPOS_V4_1510:
		case SPOS_V4_1519:
		case SPOS_V4_1522:
		case SPOS_V4_1525:
		case SPOS_V4_1734:
		case SPOS_V4_1901:
		case SPOS_V4_1977:
		case SPOS_V4_2012:
		case SPOS_V4_2067:
		case SPOS_V4_2133:
		case SPOS_V4_2161:
		case SPOS_V4_2455:
		case SPOS_V4_2606:
		case SPOS_V4_2_369:
		case SPOS_V4_2_631:
		case SPOS_V4_2_714:
		case SPOS_V4_2_919:
		case SPOS_V4_2_959:
		case SPOS_V4_2_1081:
		case SPOS_V4_2_1087:
		case SPOS_V4_2_1384:
		case SPOS_V4_2_1456:
		case SPOS_V4_2_1530:
		case SPOS_V4_2_1579:
		case SPOS_V4_2_1605:
		case SPOS_V4_2_1618:
		case SPOS_V4_2_1622:
		case SPOS_V4_3_18:
		case SPOS_V4_3_93:
		case SPOS_V4_3_104:
			SQLOut.Add( KeyRecord.GetDefaultTextAlwaysFlag() ? 1 : 0 );
			break;
		}

		if ( FALSE == bForDownload )
			SQLOut.Add( KeyRecord.GetGJANoSortFlag() ? 1: 0 );

		SQLOut.WriteInsert();
		m_nTotalListItem++;

		if ( KeyRecord.GetType() == KEY_TYPE_PLUNUMBER )
		{
			CSortedInt64Item item;
			item.m_nItem = KeyRecord.GetValue();
			m_arrayPluList.Consolidate( item );
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardSQLExporter::ExportScreenModes( CSSFile& fileSQL )
{
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
	
	CSQLOut SQLOut ( fileSQL, "ScreenModes", csvHeader.GetLine() );
	SQLOut.WriteDelete();

	for ( int nScreenIdx = 0; nScreenIdx < SPOSKeyboardManager.GetListManager() -> GetScreenCount(); nScreenIdx++ )
	{
		CSPOSKeyboardScreenRecord ScreenRecord;
		SPOSKeyboardManager.GetListManager() -> GetScreenRecord( nScreenIdx, ScreenRecord );

		SQLOut.Add( ScreenRecord.GetScreenNo() );
		SQLOut.Add( ScreenRecord.GetName() );
		SQLOut.Add( ScreenRecord.GetTradingListTopLevel() );
		SQLOut.Add( ScreenRecord.GetLoggedOffListFunction() );
		SQLOut.Add( ScreenRecord.GetTradingListFunction() );
		SQLOut.Add( ScreenRecord.GetPaymentListFunction() );
		SQLOut.Add( 0 );	//FIDDLE - PAUL BURTON SAYS FASTBAR NOT USED
		SQLOut.Add( ScreenRecord.GetTradingScreenStyle() );
		SQLOut.Add( ScreenRecord.GetPaymentListTopLevel() );
		SQLOut.Add( ScreenRecord.GetLoggedOffListFunctionLarge() );
		SQLOut.Add( ScreenRecord.GetLoggedOffScreenStyle() );
		
		SQLOut.WriteInsert();
		m_nTotalScreenMode++;
	}
}

/**********************************************************************/

void CSPOSKeyboardSQLExporter::ExportSysValues( CSSFile& fileSQL )
{
	CCSV csvHeader;
	csvHeader.Add( "[FieldName]" );
	csvHeader.Add( "[FieldIndex]" );
	csvHeader.Add( "[iValue]" );
	csvHeader.Add( "[fValue]" );
	csvHeader.Add( "[sValue]" );
	
	CSQLOut SQLOut ( fileSQL, "SysValues", csvHeader.GetLine() );
	
	for ( int nColourId = 33; nColourId <= 64; nColourId++ )
	{
		SQLOut.AddCondition ( "Colour" );
		SQLOut.AddCondition ( nColourId );
		SQLOut.Add( SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB( nColourId ) );
		SQLOut.Add( 0.0, 3 );
		SQLOut.Add( "" );
		SQLOut.WriteModify();
		m_nTotalSysValues++;
	}
}

/**********************************************************************/

void CSPOSKeyboardSQLExporter::ExportMacros( CSSFile& fileSQL, bool bClear )
{
	CCSV csvHeader;
	csvHeader.Add( "[MacNo]" );
	csvHeader.Add( "[MacText]" );
	csvHeader.Add( "[Description]" );
	
	CSQLOut SQLOut ( fileSQL, "Macros", csvHeader.GetLine() );
	
	if ( TRUE == bClear )
	{
		SQLOut.WriteDelete();
		
		for ( int nMacroIdx = 0; nMacroIdx < DataManagerNonDb.CCMacroActive.GetSize(); nMacroIdx++ )
		{
			CCascadingMacroCSVRecord Macro;
			DataManagerNonDb.CCMacroActive.GetAt( nMacroIdx, Macro );
			SQLOut.Add( Macro.GetMacroNo() );
			SQLOut.Add( Macro.GetParentMacroData() );
			SQLOut.Add( Macro.GetParentMacroName() );
			SQLOut.WriteInsert();
			m_nTotalMacros++;
		}
	}
	else
	{
		for ( int nMacroIdx = 0; nMacroIdx < DataManagerNonDb.CCMacroActive.GetSize(); nMacroIdx++ )
		{
			CCascadingMacroCSVRecord Macro;
			DataManagerNonDb.CCMacroActive.GetAt( nMacroIdx, Macro );

			int nMacroNo = Macro.GetMacroNo();

			{
				CString strDelete;
				strDelete.Format( "DELETE FROM Macros WHERE MacNo=%d", nMacroNo );
				fileSQL.WriteLine( strDelete );
				fileSQL.WriteLine( "--->GO" );
			}
		
			SQLOut.Add( nMacroNo );
			SQLOut.Add( Macro.GetParentMacroData() );
			SQLOut.Add( Macro.GetParentMacroName() );
			SQLOut.WriteInsert();
			m_nTotalMacros++;
		}
	}
}

/**********************************************************************/