/**********************************************************************/
#include "ButtonDefines.h"
#include "ButtonManager.h"
#include "CommsHandshaker.h"
#include "DatabaseCSVArray.h"
#include "DatabaseValidator.h"
#include "KeyboardSetCSVArray.h"
#include "LocationCSVArray.h"
#include "Password.h"
#include "PresetReportManager.h"
#include "SharedResources.h"
#include "SysInfo.h"
/**********************************************************************/
#include "..\SPosStockManager5\StockOptionsIni.h"
/**********************************************************************/
#include "ButtonActionHandler.h"
/**********************************************************************/

CButtonActionHandler::CButtonActionHandler(void)
{
	m_arrayContextIDs.Add( IDM_BUTTONBAR_OPTION1 );
	m_arrayContextIDs.Add( IDM_BUTTONBAR_OPTION2 );
	m_arrayContextIDs.Add( IDM_BUTTONBAR_OPTION3 );
	m_arrayContextIDs.Add( IDM_BUTTONBAR_OPTION4 );
	m_arrayContextIDs.Add( IDM_BUTTONBAR_OPTION5 );
	m_arrayContextIDs.Add( IDM_BUTTONBAR_OPTION6 );
	m_arrayContextIDs.Add( IDM_BUTTONBAR_OPTION7 );
	m_arrayContextIDs.Add( IDM_BUTTONBAR_OPTION8 );
	m_arrayContextIDs.Add( IDM_BUTTONBAR_OPTION9 );
	m_arrayContextIDs.Add( IDM_BUTTONBAR_OPTION10 );
	m_arrayContextIDs.Add( IDM_BUTTONBAR_OPTION11 );
	m_arrayContextIDs.Add( IDM_BUTTONBAR_OPTION12 );
	m_arrayContextIDs.Add( IDM_BUTTONBAR_OPTION13 );
	m_arrayContextIDs.Add( IDM_BUTTONBAR_OPTION14 );
	m_arrayContextIDs.Add( IDM_BUTTONBAR_OPTION15 );
	m_arrayContextIDs.Add( IDM_BUTTONBAR_OPTION16 );
	m_arrayContextIDs.Add( IDM_BUTTONBAR_OPTION17 );
	m_arrayContextIDs.Add( IDM_BUTTONBAR_OPTION18 );
	m_arrayContextIDs.Add( IDM_BUTTONBAR_OPTION19 );
	m_arrayContextIDs.Add( IDM_BUTTONBAR_OPTION20 );
	m_nContextMenuMode = 0;
	m_nEditButtonIdx = 0;
	m_pMenuParent = NULL;
}

/**********************************************************************/

bool CButtonActionHandler::PrepareCommand()
{
	if ((DatabaseValidator.GetDoingUpdateFlag() == TRUE) || (DatabaseValidator.GetBusyFlag() == TRUE))
	{
		return FALSE;
	}

	if ( DatabaseValidator.ValidAtStartup() == TRUE )
	{
		CFilenameUpdater FnUp( SysFiles::DataValidationQuit );
		if ( ::FileExists( FnUp.GetFilenameToUse() ) == TRUE )
		{
			DatabaseValidator.SetErrorFlag( TRUE );
			m_AccessHelpers.GetParentWnd() -> PostMessage( WM_CLOSE );
			return FALSE;
		}
	}
	else
	{
		Prompter.Error("This option is unavailable because the product database\nwas invalid at program startup.");
		return FALSE;
	}

	CommsHandshaker.ProcessSales();
	return TRUE;
}

/**********************************************************************/

void CButtonActionHandler::ShowContextMenu( CButtonInfo& infoDisplay, CArray<CSortedIntAndStringItem,CSortedIntAndStringItem>& arrayItems )
{
	m_ButtonContext = infoDisplay;

	CPoint pt;
	GetCursorPos(&pt);

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP_BUTTONBAR));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = m_pMenuParent;

	CMenuChanger MenuChanger;
	MenuChanger.EatMenuPopup( pPopup );

	m_arrayContextOptions.RemoveAll();

	for ( int nIdx = 0; nIdx < arrayItems.GetSize(); nIdx++ )
	{
		CSortedIntAndStringItem item = arrayItems.GetAt(nIdx);
		MenuChanger.SetItemText( m_arrayContextIDs.GetAt(nIdx), item.m_strItem );
		m_arrayContextOptions.Add( item.m_nItem );
	}
		
	for (int nIdx = arrayItems.GetSize(); nIdx < 20; nIdx++)
	{
		MenuChanger.KillItem(m_arrayContextIDs.GetAt(nIdx));
	}

	MenuChanger.BuildMenu( pPopup );

	//while (pWndPopupOwner->GetStyle() & WS_CHILD)
	//	pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,
		pWndPopupOwner);
}

/**********************************************************************/

void CButtonActionHandler::ShowContextMenuEditKey( CWnd* pParent, int nButtonIdx, CButtonInfo& infoDisplay )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		m_pMenuParent = pParent;
		m_nEditButtonIdx = nButtonIdx;
		m_nContextMenuMode = 5;

		m_ButtonContext = infoDisplay;

		CPoint pt;
		GetCursorPos(&pt);

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_POPUP_BUTTONEDIT));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = m_pMenuParent;

		CMenuChanger MenuChanger;
		MenuChanger.EatMenuPopup( pPopup );

		m_arrayContextOptions.RemoveAll();
		m_arrayContextOptions.Add(1);
		m_arrayContextOptions.Add(2);
		m_arrayContextOptions.Add(3);
	
		switch ( m_ButtonContext.GetButtonType() )
		{
		case BUTTON_TYPE_ACTION:
		case BUTTON_TYPE_LIST:
			m_arrayContextOptions.Add(4);
			break;

		default:
			MenuChanger.KillItem( IDM_BUTTONBAR_OPTION4 );
			break;
		}

		MenuChanger.BuildMenu( pPopup );

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,
			pWndPopupOwner);
	}
}

/**********************************************************************/

void CButtonActionHandler::ShowContextMenuDatabase( CButtonInfo& infoDisplay )
{
	m_nContextMenuMode = 0;
	CArray<CSortedIntAndStringItem,CSortedIntAndStringItem> arrayItems;

	for ( int nDbIdx = 0; ( nDbIdx < dbDatabase.GetSize() ) && ( arrayItems.GetSize() < 20 ); nDbIdx++ )
	{
		if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
		{
			CSortedIntAndStringItem item;
			item.m_nItem = dbDatabase.GetDbNo( nDbIdx );
			item.m_strItem = dbDatabase.GetName( nDbIdx );
			arrayItems.Add( item );
		}
	}

	switch( arrayItems.GetSize() )
	{
	case 0:
		ShowAuthorisationError(2);
		break;

	case 1:
		{
			infoDisplay.SetDetail3( arrayItems[0].m_nItem );
			ActionInternal( infoDisplay );
		}
		break;

	default:
		ShowContextMenu( infoDisplay, arrayItems );
		break;
	}
}

/**********************************************************************/

void CButtonActionHandler::ShowContextMenuDatabaseServer( CButtonInfo& infoDisplay )
{
	m_nContextMenuMode = 2;
	CArray<CSortedIntAndStringItem,CSortedIntAndStringItem> arrayItems;

	for ( int nDbIdx = 0; ( nDbIdx < dbDatabase.GetSize() ) && ( arrayItems.GetSize() < 20 ); nDbIdx++ )
	{
		if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
		{
			CSortedIntAndStringItem item;
			item.m_nItem = nDbIdx;
			item.m_strItem = dbDatabase.GetName( nDbIdx );
			arrayItems.Add( item );
		}
	}

	switch( arrayItems.GetSize() )
	{
	case 0:
		ShowAuthorisationError(2);
		break;

	case 1:
		EditServerDatabase( infoDisplay, arrayItems[0].m_nItem );
		break;

	default:
		ShowContextMenu( infoDisplay, arrayItems );
		break;
	}
}

/**********************************************************************/

void CButtonActionHandler::ShowContextMenuLocationServer( CButtonInfo& infoDisplay )
{
	m_nContextMenuMode = 3;
	CArray<CSortedIntAndStringItem,CSortedIntAndStringItem> arrayItems;

	for ( int nLocIdx = 0; ( nLocIdx < dbLocation.GetSize() ) && ( arrayItems.GetSize() < 20 ); nLocIdx++ )
	{
		if ( PasswordArray.CheckLocationAccessLocIdx( nLocIdx ) == TRUE )
		{
			int nDbIdx;
			if ( dbDatabase.FindDatabaseByNumber( dbLocation.GetDbNo( nLocIdx ), nDbIdx ) == TRUE )
			{
				CSortedIntAndStringItem item;
				item.m_nItem = nLocIdx;
				item.m_strItem = dbLocation.GetName( nLocIdx );
				arrayItems.Add( item );
			}
		}
	}

	switch( arrayItems.GetSize() )
	{
	case 0:
		ShowAuthorisationError(2);
		break;

	case 1:
		EditServerLocation( infoDisplay, arrayItems[0].m_nItem );
		break;

	default:
		ShowContextMenu( infoDisplay, arrayItems );
		break;
	}
}

/**********************************************************************/

void CButtonActionHandler::ShowContextMenuKeyboardLocation( CButtonInfo& infoDisplay )
{
	m_nContextMenuMode = 1;
	CArray<CSortedIntAndStringItem,CSortedIntAndStringItem> arrayItems;

	for ( int nIdx = 0; ( nIdx < m_AccessHelpers. m_Keyboard.GetKeyboardMapSize() ) && ( arrayItems.GetSize() < 20 ); nIdx++ )
	{
		CKeyboardMapInfo infoMap;
		m_AccessHelpers. m_Keyboard.GetKeyboardMapInfo( nIdx, infoMap );

		CSortedIntAndStringItem item;
		
		switch( infoMap.m_nEntryType )
		{
		case KBD_MAP_SET:
			{
				if ( ( infoMap.m_nKbsIdx >= 0 ) && ( infoMap.m_nKbsIdx < dbKeyboardSet.GetSize() ) )
				{
					item.m_nItem = 1000000;
					item.m_nItem += ( dbKeyboardSet.GetDbNo( infoMap.m_nKbsIdx ) * 1000 );
					item.m_nItem += ( dbKeyboardSet.GetKbsNo( infoMap.m_nKbsIdx ) );
					item.m_strItem = dbKeyboardSet.GetName( infoMap.m_nKbsIdx );
					arrayItems.Add( item );
				}
			}
			break;

		case KBD_MAP_LOC:
			{
				if ( ( infoMap.m_nLocIdx >= 0 ) && ( infoMap.m_nLocIdx < dbLocation.GetSize() ) )
				{
					item.m_nItem = dbLocation.GetNetworkLocNo( infoMap.m_nLocIdx );
					item.m_strItem = dbLocation.GetName( infoMap.m_nLocIdx );
					arrayItems.Add( item );
				}
			}	
			break;
		}
	}

	switch( arrayItems.GetSize() )
	{
	case 0:
		ShowAuthorisationError(2);
		break;

	case 1:
		{
			infoDisplay.SetDetail2( arrayItems[0].m_nItem );
			KeyboardAction( infoDisplay );
		}
		break;

	default:
		ShowContextMenu( infoDisplay, arrayItems );
		break;
	}
}

/**********************************************************************/

void CButtonActionHandler::ShowContextMenuKeyboardNumber( CButtonInfo& infoDisplay )
{
	m_nContextMenuMode = 0;
	CArray<CSortedIntAndStringItem,CSortedIntAndStringItem> arrayItems;

	for ( int nKbNo = 1; ( nKbNo <= EcrmanOptions.GetFeaturesKeyboardsPerLocation() ) && ( nKbNo <= 20 ); nKbNo++ )
	{
		CSortedIntAndStringItem item;
		item.m_nItem = nKbNo;
		item.m_strItem.Format( "Keyboard %d", nKbNo );
		arrayItems.Add( item );
	}

	switch( arrayItems.GetSize() )
	{
	case 0:
		ShowAuthorisationError(2);
		break;

	case 1:
		{
			infoDisplay.SetDetail3( arrayItems[0].m_nItem );
			KeyboardAction( infoDisplay );
		}
		break;

	default:
		ShowContextMenu( infoDisplay, arrayItems );
		break;
	}
}

/**********************************************************************/

void CButtonActionHandler::ShowContextMenuPriceSetLocation( CButtonInfo& infoDisplay )
{
	m_nContextMenuMode = 4;
	CArray<CSortedIntAndStringItem,CSortedIntAndStringItem> arrayItems;

	for ( int nLocIdx = 0; ( nLocIdx < dbLocation.GetSize() ) && ( arrayItems.GetSize() < 20 ); nLocIdx++ )
	{
		if ( PasswordArray.CheckLocationAccessLocIdx( nLocIdx ) == TRUE )
		{
			CSortedIntAndStringItem item;
			item.m_nItem = dbLocation.GetNetworkLocNo( nLocIdx );
			item.m_strItem = dbLocation.GetName( nLocIdx );
			arrayItems.Add( item );
		}
	}

	switch( arrayItems.GetSize() )
	{
	case 0:
		ShowAuthorisationError(2);
		break;

	case 1:
		{
			infoDisplay.SetDetail2( arrayItems[0].m_nItem );
			PriceSetAction( infoDisplay );
		}
		break;

	default:
		ShowContextMenu( infoDisplay, arrayItems );
		break;
	}
}

/**********************************************************************/

void CButtonActionHandler::ShowContextMenuPriceSetDatabase( CButtonInfo& infoDisplay )
{
	m_nContextMenuMode = 4;
	CArray<CSortedIntAndStringItem,CSortedIntAndStringItem> arrayItems;

	bool bGotDbPriceSet = FALSE;

	for ( int nDbIdx = 0; ( nDbIdx < dbDatabase.GetSize() ) && ( arrayItems.GetSize() < 20 ); nDbIdx++ )
	{
		if ( TRUE == bGotDbPriceSet )
			if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == FALSE )
				continue;
		
		CArray<int,int> Set;
		dbLocation.GetDatabaseLocIdxSet( nDbIdx, Set );
		
		bool bGotThisDbPriceSet = FALSE;
		for ( int n = 0; n < Set.GetSize(); n++ )
		{
			int nLocIdx = Set.GetAt(n);
			if ( ( dbLocation.GetDbPriceSet1( nLocIdx ) != 0 ) || ( dbLocation.GetDbPriceSet2( nLocIdx ) != 0 ) || ( dbLocation.GetDbPriceSet3( nLocIdx ) != 0 ) ) 
			{
				bGotDbPriceSet = TRUE;
				bGotThisDbPriceSet = TRUE;
				break;
			}
		}

		if ( TRUE == bGotThisDbPriceSet )
		{
			if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
			{
				CSortedIntAndStringItem item;
				item.m_nItem = dbDatabase.GetDbNo( nDbIdx );
				item.m_strItem = dbDatabase.GetName( nDbIdx );
				arrayItems.Add( item );
			}
		}
	}

	switch( arrayItems.GetSize() )
	{
	case 0:
		if ( TRUE == bGotDbPriceSet )
			ShowAuthorisationError(2);
		else
			Prompter.Info( "No database price sets are\ncurrently in use." );
		break;

	case 1:
		{
			infoDisplay.SetDetail2( arrayItems[0].m_nItem );
			PriceSetAction( infoDisplay );
		}
		break;

	default:
		ShowContextMenu( infoDisplay, arrayItems );
		break;
	}
}

/**********************************************************************/

void CButtonActionHandler::ShowContextMenuPriceSetNumber( CButtonInfo& infoDisplay, int nEntityIdx )
{
	m_nContextMenuMode = 0;
	CArray<CSortedIntAndStringItem,CSortedIntAndStringItem> arrayItems;

	int nMaxPSet = 0;
	switch( infoDisplay.GetDetail1() )
	{
	case BUTTON_DETAIL1_PSET_DB:
		{
			CArray<int,int> Set;
			dbLocation.GetDatabaseLocIdxSet( nEntityIdx, Set );

			CReportConsolidationArray<CSortedIntByInt> PriceSetMap; 

			for ( int n = 0; n < Set.GetSize(); n++ )
			{
				int nLocIdx = Set.GetAt(n);
				
				CArray<int,int> arrayDbSets;
				arrayDbSets.Add( dbLocation.GetDbPriceSet1( nLocIdx ) );
				arrayDbSets.Add( dbLocation.GetDbPriceSet2( nLocIdx ) );
				arrayDbSets.Add( dbLocation.GetDbPriceSet3( nLocIdx ) );

				for ( int x = 0; x < arrayDbSets.GetSize(); x++ )
				{
					CSortedIntByInt item;
					item.m_nKey = arrayDbSets.GetAt(x);

					if ( ( item.m_nKey >= 1 ) && ( item.m_nKey <= 20 ) )
					{
						int nPos;
						if ( PriceSetMap.Find( item, nPos ) == FALSE )
						{
							item.m_nVal = ( nLocIdx * 4 ) + x + 1;
							PriceSetMap.InsertAt( nPos, item );
						}
					}
				}
			}

			for ( int x = 0; x < PriceSetMap.GetSize(); x++ )
			{
				CSortedIntByInt itemMap;
				PriceSetMap.GetAt( x, itemMap );

				CSortedIntAndStringItem itemMenu;
				itemMenu.m_nItem = itemMap.m_nVal;
				itemMenu.m_strItem.Format( "Price Set %d", itemMap.m_nKey );
				arrayItems.Add( itemMenu );
			}
						
		}
		break;

	case BUTTON_DETAIL1_PSET_LOC:
		{
			for ( int nPSet = 1; nPSet <= dbLocation.GetPluPriceSetCount( nEntityIdx ); nPSet++ )
			{
				CSortedIntAndStringItem item;
				item.m_nItem = nPSet;
				item.m_strItem.Format( "Price Set %d", nPSet );
				arrayItems.Add( item );
			}
		}
		break;
	}

	switch( arrayItems.GetSize() )
	{
	case 0:
		ShowAuthorisationError(2);
		break;

	case 1:
		{
			infoDisplay.SetDetail3( arrayItems[0].m_nItem );
			ActionInternal( infoDisplay );
		}
		break;

	default:
		ShowContextMenu( infoDisplay, arrayItems );
		break;
	}
}

/**********************************************************************/

void CButtonActionHandler::ShowContextMenuPresets( CPresetReportArray& PresetArray, CButtonInfo& infoDisplay, bool& bSingleReport )
{
	m_nContextMenuMode = 0;
	bSingleReport = FALSE;

	CArray<CSortedIntAndStringItem,CSortedIntAndStringItem> arrayItems;

	for ( int nPresetIdx = 0; ( nPresetIdx < PresetArray.GetSize() ) && ( arrayItems.GetSize() < 20 ); nPresetIdx++ )
	{
		int nEntityNo;
		CString strListName;
		PresetArray.GetEntityNoAndListName( nPresetIdx, nEntityNo, strListName );
		
		if ( nEntityNo != 0 )
		{
			CSortedIntAndStringItem item;
			item.m_nItem = nEntityNo;
			item.m_strItem = strListName;
			arrayItems.Add( item );
		}
	}

	switch( arrayItems.GetSize() )
	{
	case 0:
		ShowAuthorisationError(2);
		break;

	case 1:
		{
			infoDisplay.SetDetail3( arrayItems[0].m_nItem );
			bSingleReport = TRUE;
		}
		break;

	default:
		ShowContextMenu( infoDisplay, arrayItems );
		break;
	}
}

/**********************************************************************/

bool CButtonActionHandler::ShowContextMenuTask( CButtonInfo& infoDisplay )
{
	m_nContextMenuMode = 0;
	CArray<CSortedIntAndStringItem,CSortedIntAndStringItem> arrayItems;

	for ( int nTaskNo = 0x81; ( nTaskNo <= 0x94 ) && ( arrayItems.GetSize() < 20 ); nTaskNo++ )
	{
		bool bAllow = TRUE;
		bAllow &= ( m_TaskDatabase.GetLibraryTaskJobCount( nTaskNo ) != 0 );
		bAllow &= m_AccessHelpers. m_Task.CanRunTask( nTaskNo );

		if ( TRUE == bAllow )
		{
			CSortedIntAndStringItem item;
			item.m_nItem = nTaskNo;
			item.m_strItem = m_TaskDatabase.GetLibraryTaskName( nTaskNo );
			arrayItems.Add( item );
		}
	}

	bool bResult = FALSE;

	switch( arrayItems.GetSize() )
	{
	case 0:
		ShowAuthorisationError(2);
		break;

	case 1:
		{
			infoDisplay.SetDetail3( arrayItems[0].m_nItem );
			ActionInternal( infoDisplay );
			bResult = TRUE;
		}
		break;

	default:
		ShowContextMenu( infoDisplay, arrayItems );
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CButtonActionHandler::ShowContextMenuPluSalesHistory( CButtonInfo& infoDisplay )
{
	bool bSingleReport = FALSE;
	m_nContextMenuMode = 0;

	CDataManagerInfo info;
	PresetReportManager.Open( NODE_PRESET_PLU_HIST, DB_READONLY, info );
	ShowContextMenuPresets( PresetReportManager.PluHist, infoDisplay, bSingleReport );
	PresetReportManager.Close( NODE_PRESET_PLU_HIST, info );

	if ( TRUE == bSingleReport )
		ActionInternal( infoDisplay );
}

/**********************************************************************/

void CButtonActionHandler::ShowContextMenuEposReports( CButtonInfo& infoDisplay )
{
	bool bSingleReport = FALSE;
	m_nContextMenuMode = 0;

	CDataManagerInfo info;
	PresetReportManager.Open( NODE_PRESET_EPOS, DB_READONLY, info );
	ShowContextMenuPresets( PresetReportManager.Epos, infoDisplay, bSingleReport );
	PresetReportManager.Close( NODE_PRESET_EPOS, info );

	if ( TRUE == bSingleReport )
		ActionInternal( infoDisplay );
}

/**********************************************************************/

void CButtonActionHandler::ShowContextMenuCustomerReports( CButtonInfo& infoDisplay )
{
	bool bSingleReport = FALSE;
	m_nContextMenuMode = 0;

	CDataManagerInfo info;
	PresetReportManager.Open( NODE_PRESET_CUST_HIST, DB_READONLY, info );
	ShowContextMenuPresets( PresetReportManager.CustHist, infoDisplay, bSingleReport );
	PresetReportManager.Close( NODE_PRESET_CUST_HIST, info );

	if ( TRUE == bSingleReport )
		ActionInternal( infoDisplay );
}

/**********************************************************************/

void CButtonActionHandler::ShowContextMenuTimeAttend( CButtonInfo& infoDisplay )
{
	bool bSingleReport = FALSE;
	m_nContextMenuMode = 0;

	CDataManagerInfo info;
	PresetReportManager.Open( NODE_PRESET_TIMEATTEND, DB_READONLY, info );
	ShowContextMenuPresets( PresetReportManager.TimeAttend, infoDisplay, bSingleReport );
	PresetReportManager.Close( NODE_PRESET_TIMEATTEND, info );

	if ( TRUE == bSingleReport )
		ActionInternal( infoDisplay );
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

void CButtonActionHandler::ShowContextMenuStockSalesHistory( CButtonInfo& infoDisplay )
{
	bool bSingleReport = FALSE;
	m_nContextMenuMode = 0;

	CDataManagerInfo info;
	PresetReportManager.Open( NODE_PRESET_STOCK_HIST, DB_READONLY, info );
	ShowContextMenuPresets( PresetReportManager.StockHist, infoDisplay, bSingleReport );
	PresetReportManager.Close( NODE_PRESET_STOCK_HIST, info );

	if ( TRUE == bSingleReport )
		ActionInternal( infoDisplay );
}

/**********************************************************************/

void CButtonActionHandler::ShowContextMenuStockLevel( CButtonInfo& infoDisplay )
{
	bool bSingleReport = FALSE;
	m_nContextMenuMode = 0;

	CDataManagerInfo info;
	PresetReportManager.Open( NODE_PRESET_STOCK_LEVEL, DB_READONLY, info );
	ShowContextMenuPresets( PresetReportManager.StockLevel, infoDisplay, bSingleReport );
	PresetReportManager.Close( NODE_PRESET_STOCK_LEVEL, info );

	if ( TRUE == bSingleReport )
		ActionInternal( infoDisplay );
}

/**********************************************************************/

void CButtonActionHandler::ShowContextMenuStockAudit( CButtonInfo& infoDisplay )
{
	bool bSingleReport = FALSE;
	m_nContextMenuMode = 0;

	CDataManagerInfo info;
	PresetReportManager.Open( NODE_PRESET_STOCK_AUDIT, DB_READONLY, info );
	ShowContextMenuPresets( PresetReportManager.StockAudit, infoDisplay, bSingleReport );
	PresetReportManager.Close( NODE_PRESET_STOCK_AUDIT, info );

	if ( TRUE == bSingleReport )
		ActionInternal( infoDisplay );
}

/**********************************************************************/

void CButtonActionHandler::ShowContextMenuStockpoint( CButtonInfo& infoDisplay )
{
	m_nContextMenuMode = 0;
	CArray<CSortedIntAndStringItem,CSortedIntAndStringItem> arrayItems;
	
	bool bTransfer = FALSE;
	if ( infoDisplay.GetDetail1() == BUTTON_DETAIL1_MOVESTK )
		if ( infoDisplay.GetDetail2() == BUTTON_DETAIL2_MOVESTK_TRANSFER )
			bTransfer = TRUE;
			
	for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
	{
		int nStartIdx, nEndIdx;
		dbStockpoint.GetSpIdxRange( nDbIdx, nStartIdx, nEndIdx );
	
		for ( int nSpIdx = nStartIdx; ( nSpIdx <= nEndIdx ) && ( arrayItems.GetSize() < 20 ); nSpIdx++ )
		{
			bool bShow = TRUE;
			
			if ( PasswordArray.CheckLocationAccessSpIdx( nSpIdx ) == FALSE )
				bShow = FALSE;

			if ( ( TRUE == bTransfer ) && ( TRUE == bShow ) )
			{
				if ( dbStockpoint.GetTransferOutFlag( nSpIdx ) == FALSE )
					bShow = FALSE;
				else if ( dbStockpoint.GetTransferInFlag( nSpIdx ) == FALSE )
					bShow = ( m_AccessHelpers.m_StockData.GetDbTransferTargets( nDbIdx ) >= 1 );
				else
					bShow = ( m_AccessHelpers.m_StockData.GetDbTransferTargets( nDbIdx ) >= 2 );
			}

			if ( TRUE == bShow )
			{
				CSortedIntAndStringItem item;
				item.m_nItem = ( dbStockpoint.GetDbNo( nSpIdx ) * 1000 ) + dbStockpoint.GetSpNo( nSpIdx );
				item.m_strItem = dbStockpoint.GetName( nSpIdx );
				arrayItems.Add( item );
			}
		}

		if ( arrayItems.GetSize() >= 20 )
			break;
	}

	switch( arrayItems.GetSize() )
	{
	case 0:
		ShowAuthorisationError(2);
		break;

	case 1:
		{
			infoDisplay.SetDetail3( arrayItems[0].m_nItem );
			ActionInternal( infoDisplay );
		}
		break;

	default:
		ShowContextMenu( infoDisplay, arrayItems );
		break;
	}
}

/**********************************************************************/
#else
/**********************************************************************/

void CButtonActionHandler::ShowContextMenuEcrmanStock( CButtonInfo& infoDisplay )
{
	bool bSingleReport = FALSE;
	m_nContextMenuMode = 0;

	CDataManagerInfo info;
	PresetReportManager.Open( NODE_PRESET_ECR_STOCK, DB_READONLY, info );
	ShowContextMenuPresets( PresetReportManager.EcrStock, infoDisplay, bSingleReport );
	PresetReportManager.Close( NODE_PRESET_ECR_STOCK, info );

	if (TRUE == bSingleReport)
	{
		ActionInternal(infoDisplay);
	}
}

/**********************************************************************/
#endif
/**********************************************************************/

bool CButtonActionHandler::CheckAuthorisation( CButtonInfo& infoDisplay )
{
	bool bResult = TRUE;

	if ( PasswordArray.IsMasterPassword() == FALSE )
	{
		switch( infoDisplay.GetDetail1() )
		{
		case BUTTON_DETAIL1_SETUP:		

			switch( infoDisplay.GetDetail2() )
			{
			case BUTTON_DETAIL2_SETUP_SYSTEM:
				bResult &= PasswordArray.GetEnable( PasswordTicks::SetupMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::SetupMenuSystem );
				break;

			case BUTTON_DETAIL2_SETUP_OPTIONS:
				bResult &= PasswordArray.GetEnable( PasswordTicks::SetupMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::SetupMenuOptions );
				break;
			}
			break;

		case BUTTON_DETAIL1_EDITEPOS:	

			switch( infoDisplay.GetDetail2() )
			{
			case BUTTON_DETAIL2_EDITEPOS_PLU:		
				bResult &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditEposData );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditPluRecords );
				break;

			case BUTTON_DETAIL2_EDITEPOS_DEPT:
				bResult &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditEposData );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditDepartment );
				break;

			case BUTTON_DETAIL2_EDITEPOS_GROUP:
				bResult &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditEposData );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditGroup );
				break;

			case BUTTON_DETAIL2_EDITEPOS_TAX:
				bResult &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditEposData );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditTax );
				break;

			case BUTTON_DETAIL2_EDITEPOS_ACAT:
				bResult &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditEposData );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditAnalysisCategory );
				break;

			case BUTTON_DETAIL2_EDITEPOS_PAYMENT:
				bResult &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditEposData );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditPayment );
				break;

			case BUTTON_DETAIL2_EDITEPOS_SERVER:
				bResult &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditEposData );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditServerNames );
				break;

			case BUTTON_DETAIL2_EDITEPOS_MIXMATCH:	
			case BUTTON_DETAIL2_EDITEPOS_OFFERGROUP:		
			case BUTTON_DETAIL2_EDITEPOS_PROMOTION:		
				bResult &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditEposData );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditMixMatch );
				break;

			case BUTTON_DETAIL2_EDITEPOS_WALLETSTAMPRULE:
				bResult &= PasswordArray.GetEnable(PasswordTicks::DatabaseMenu);
				bResult &= PasswordArray.GetEnable(PasswordTicks::EditEposData);
				bResult &= PasswordArray.GetEnable(PasswordTicks::EditWalletStampRule);
				break;

			case BUTTON_DETAIL2_EDITEPOS_LOYALTYSCHEME:
				bResult &= PasswordArray.GetEnable(PasswordTicks::DatabaseMenu);
				bResult &= PasswordArray.GetEnable(PasswordTicks::EditEposData);
				break;

			case BUTTON_DETAIL2_EDITEPOS_TIMEPLAN:		
				bResult &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditEposData );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditTimePlan );
				break;
			}
			break;

#ifdef STOCKMAN_SYSTEM

		case BUTTON_DETAIL1_EDITSTK:	

			switch( infoDisplay.GetDetail2() )
			{
			case BUTTON_DETAIL2_EDITSTK_STOCK:	
				bResult &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditStockData );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditStockRecords );
				break;

			case BUTTON_DETAIL2_EDITSTK_SUPPLIER:
			case BUTTON_DETAIL2_EDITSTK_CATEGORY:
			case BUTTON_DETAIL2_EDITSTK_CUSTFLD:
			case BUTTON_DETAIL2_EDITSTK_ALLOWANCE:
			case BUTTON_DETAIL2_EDITSTK_DELUNIT:
			case BUTTON_DETAIL2_EDITSTK_STKUNIT:
			case BUTTON_DETAIL2_EDITSTK_SUBUNIT:
			case BUTTON_DETAIL2_EDITSTK_ADJTEXT:
				bResult &= PasswordArray.GetEnable( PasswordTicks::SetupMenuTexts );
				break;
			}
			break;

		case BUTTON_DETAIL1_MOVESTK:
			
			switch( infoDisplay.GetDetail2() )
			{
			case BUTTON_DETAIL2_MOVESTK_OPENING:	
				bResult &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditStockData );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditOpening );
				break;

			case BUTTON_DETAIL2_MOVESTK_DELIVERY:	
				bResult &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditStockData );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditDeliveries );
				break;

			case BUTTON_DETAIL2_MOVESTK_RETURN:	
				bResult &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditStockData );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditReturns );
				break;

			case BUTTON_DETAIL2_MOVESTK_SALEPLU:	
				bResult &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditStockData );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditManualSales );
				break;

			case BUTTON_DETAIL2_MOVESTK_SALESTK:	
				bResult &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditStockData );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditManualSales );
				break;

			case BUTTON_DETAIL2_MOVESTK_ADJUSTPLU:	
				bResult &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditStockData );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditAdjustments );
				break;

			case BUTTON_DETAIL2_MOVESTK_ADJUSTSTK:	
				bResult &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditStockData );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditAdjustments );
				break;

			case BUTTON_DETAIL2_MOVESTK_TRANSFER:	
				bResult &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditStockData );
				bResult &= PasswordArray.GetEnable( PasswordTicks::EditTransfers );
				break;
			}
			break;

#endif

		case BUTTON_DETAIL1_KEYBOARD:
			bResult &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
			bResult &= PasswordArray.GetEnable( PasswordTicks::EditKeyboards );
			break;

		case BUTTON_DETAIL1_PSET_LOC:
		case BUTTON_DETAIL1_PSET_DB:
			bResult &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
			bResult &= PasswordArray.GetEnable( PasswordTicks::EditPluRecords );
			break;

		case BUTTON_DETAIL1_REPORTS:	

			switch( infoDisplay.GetDetail2() )
			{
			case BUTTON_DETAIL2_REPORTS_PLUHISTORY:	
				bResult &= PasswordArray.GetEnable( PasswordTicks::ReportsMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::ReportsPluHistory );
				bResult &= PasswordArray.CheckLocationAccessLoc();
				break;

			case BUTTON_DETAIL2_REPORTS_EPOS_VIEW:
			case BUTTON_DETAIL2_REPORTS_EPOS_PRINT:
				bResult &= PasswordArray.GetEnable( PasswordTicks::ReportsMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::ReportsEpos );
				bResult &= PasswordArray.CheckLocationAccessLoc();
				break;

			case BUTTON_DETAIL2_REPORTS_CUSTOMER:
				bResult &= PasswordArray.GetEnable( PasswordTicks::ReportsMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::ReportsCustomer );
				bResult &= PasswordArray.CheckLocationAccessLoc();
				break;

			case BUTTON_DETAIL2_REPORTS_TIMEATTEND:
				bResult &= PasswordArray.GetEnable( PasswordTicks::ReportsMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::ReportsWorking );
				bResult &= PasswordArray.CheckLocationAccessLoc();
				break;
			
	#ifdef STOCKMAN_SYSTEM

			case BUTTON_DETAIL2_REPORTS_STKHISTORY:
				bResult &= PasswordArray.GetEnable( PasswordTicks::ReportsMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::ReportsStock );
				bResult &= PasswordArray.CheckLocationAccessLoc();
				break;

			case BUTTON_DETAIL2_REPORTS_STKLEVEL:	
				bResult &= PasswordArray.GetEnable( PasswordTicks::ReportsMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::ReportsStock );
				bResult &= PasswordArray.CheckLocationAccessLoc();
				break;

			case BUTTON_DETAIL2_REPORTS_STKAUDIT:	
				bResult &= PasswordArray.GetEnable( PasswordTicks::ReportsMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::ReportsAudit );
				bResult &= PasswordArray.CheckLocationAccessLoc();
				break;

	#else

			case BUTTON_DETAIL2_REPORTS_ECRMANSTK:	
				bResult &= PasswordArray.GetEnable( PasswordTicks::ReportsMenu );
				bResult &= ( PasswordArray.GetEnable( PasswordTicks::ReportsPluApparent ) || PasswordArray.GetEnable( PasswordTicks::ReportsPluDelivery ) );
				bResult &= PasswordArray.CheckLocationAccessLoc();
				break;

	#endif

			}

			break;

		case BUTTON_DETAIL1_TASKS:

			switch( infoDisplay.GetDetail2() )
			{
			case BUTTON_DETAIL2_TASKS_ADHOC:	
				bResult &= PasswordArray.GetEnable( PasswordTicks::TasksMenu );
				bResult &= PasswordArray.GetEnable( PasswordTicks::TasksMenuAdhoc );
				break;

			case BUTTON_DETAIL2_TASKS_EDITPRESET:
				bResult &= PasswordArray.GetEnable( PasswordTicks::SetupMenu );
				break;

			case BUTTON_DETAIL2_TASKS_RUNPRESET:
				bResult &= PasswordArray.GetEnable( PasswordTicks::TasksMenu );
				break;
			}
			break;

		}
	}

	return bResult;
}

/**********************************************************************/

void CButtonActionHandler::ShowAuthorisationError( int n )
{
	CString strMsg = "";
	strMsg.Format( "You are not authorised to perform this action.\n(Reason code %d)", n );
	Prompter.Error ( strMsg );
}

/**********************************************************************/

void CButtonActionHandler::ShowUnknownDatabaseError( int nDbNo )
{
	CString strMsg = "";
	strMsg.Format( "Unknown database number %d", nDbNo );
	Prompter.Error ( strMsg );
}

/**********************************************************************/

void CButtonActionHandler::ShowUnknownLocationError( int nNwkNo, int nLocNo )
{
	CString strMsg = "";
	strMsg.Format( "Unknown location number %d.%d", nNwkNo, nLocNo );
	Prompter.Error ( strMsg );
}

/**********************************************************************/

void CButtonActionHandler::ShowUnknownPriceSetError( int nNwkNo, int nLocNo, int nPSetNo )
{
	CString strMsg;
	strMsg.Format( "Unknown price set number %d.%d.%d", nNwkNo, nLocNo, nPSetNo );
	Prompter.Error ( strMsg );
}

/**********************************************************************/

void CButtonActionHandler::ShowUnknownStockpointError( int nDbNo, int nSpNo )
{
	CString strMsg = "";
	strMsg.Format( "Unknown stockpoint number %d.%d", nDbNo, nSpNo );
	Prompter.Error ( strMsg );
}

/**********************************************************************/

void CButtonActionHandler::ShowUnknownKeyboardLocError( int nDbNo, int nLocNo )
{
	CString strMsg = "";
	strMsg.Format( "Unknown keyboard location %d.%d", nDbNo, nLocNo );
	Prompter.Error ( strMsg );
}

/**********************************************************************/

void CButtonActionHandler::ShowUnknownKeyboardSetError( int nDbNo, int nKbsNo )
{
	CString strMsg = "";
	strMsg.Format( "Unknown keyboard set %d.%d", nDbNo, nKbsNo );
	Prompter.Error ( strMsg );
}

/**********************************************************************/

void CButtonActionHandler::ContextOption( int n )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		if ( ( n >= 1 ) && ( n <= m_arrayContextOptions.GetSize() ) )
		{
			switch( m_nContextMenuMode )
			{
			case 0:
				m_ButtonContext.SetDetail3( m_arrayContextOptions.GetAt(n - 1) );
				ActionInternal( m_ButtonContext );
				break;

			case 1:
				m_ButtonContext.SetDetail2( m_arrayContextOptions.GetAt(n - 1) );
				KeyboardAction( m_ButtonContext );
				break;

			case 2:
				EditServerDatabase( m_ButtonContext, m_arrayContextOptions.GetAt(n - 1) );
				break;

			case 3:
				EditServerLocation( m_ButtonContext, m_arrayContextOptions.GetAt(n - 1) );
				break;

			case 4:
				m_ButtonContext.SetDetail2( m_arrayContextOptions.GetAt(n - 1) );
				PriceSetAction( m_ButtonContext );
				break;

			case 5:
				switch( m_arrayContextOptions.GetAt( n - 1 ) )
				{
				case 1:
					{
						ButtonManager.SetEditingButton( m_ButtonContext );
						m_pMenuParent -> Invalidate();

						if ( ButtonManager.GetButtonEditDlg() != NULL )
						{
							delete( ButtonManager.GetButtonEditDlg() );
							ButtonManager.SetButtonEditDlg( NULL );
						}
						
						CButtonEditDlg dlgEdit( m_ButtonContext, BUTTON_EDIT_NORMAL, m_pMenuParent );
						if ( dlgEdit.DoModal() == IDOK )
						{
							ButtonManager.InvalidateImage( m_ButtonContext );
							ButtonManager.SetDisplayButton( m_nEditButtonIdx, m_ButtonContext );
							ButtonManager.SetLibraryButton( m_ButtonContext );
						}
				
						ButtonManager.ClearEditingButton();
						m_pMenuParent -> Invalidate();
					}
					break;
				
				case 2:
					if ( ButtonManager.GetButtonEditDlg() == NULL )
					{
						ButtonManager.SetButtonEditDlg ( new CButtonEditDlg( m_ButtonContext, BUTTON_EDIT_COPYALL, m_pMenuParent ) );
						ButtonManager.GetButtonEditDlg() -> Create( IDD_BUTTON_EDIT, m_pMenuParent );
					}
					else
					{
						ButtonManager.GetButtonEditDlg() -> SetCopyAllControls();
						ButtonManager.GetButtonEditDlg() -> SetStyleControlsFromButton( m_ButtonContext );
						ButtonManager.GetButtonEditDlg() -> SetActionControlsFromButton( m_ButtonContext );
					}
					ButtonManager.GetButtonEditDlg() -> SetFocus();
					break;

				case 3:
					if ( ButtonManager.GetButtonEditDlg() == NULL )
					{
						ButtonManager.SetButtonEditDlg ( new CButtonEditDlg( m_ButtonContext, BUTTON_EDIT_COPYSTYLE, m_pMenuParent ) );
						ButtonManager.GetButtonEditDlg() -> Create( IDD_BUTTON_EDIT, m_pMenuParent );
					}
					else
					{
						ButtonManager.GetButtonEditDlg() -> SetStyleControlsFromButton( m_ButtonContext );
					}
					ButtonManager.GetButtonEditDlg() -> SetFocus();
					break;

				case 4:
					switch( m_ButtonContext.GetButtonType() )
					{
					case BUTTON_TYPE_ACTION:
						Action( m_ButtonContext, m_pMenuParent, m_pMenuParent -> GetParentFrame()  );
						break;

					case BUTTON_TYPE_LIST:
						if ( ButtonManager.GotoList( m_ButtonContext ) == TRUE )
						{
							m_pMenuParent -> Invalidate();
						}
						break;
					}
					break;
				}
			}
		}
	}
}

/**********************************************************************/

void CButtonActionHandler::Action( CButtonInfo& infoDisplay, CWnd* pMenuParent, CWnd* pParent )
{
	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		m_pMenuParent = pMenuParent;
		m_AccessHelpers.SetParentWnd( pParent );

		if ( CheckAuthorisation( infoDisplay ) == FALSE )
		{
			ShowAuthorisationError(1);
			return;
		}

		switch( infoDisplay.GetDetail1() )
		{
		case BUTTON_DETAIL1_SETUP:		

			switch( infoDisplay.GetDetail2() )
			{
			case BUTTON_DETAIL2_SETUP_SYSTEM:	
			case BUTTON_DETAIL2_SETUP_OPTIONS:	
				ActionInternal( infoDisplay );
				break;
			}
			break;

		case BUTTON_DETAIL1_EXTERNAL:		

			switch( infoDisplay.GetDetail2() )
			{
			case BUTTON_DETAIL2_EXTERNAL_LOYALTY:	
				ActionInternal( infoDisplay );
				break;
			}
			break;

		case BUTTON_DETAIL1_EDITEPOS:	
			{
				switch( infoDisplay.GetDetail2() )
				{
				case BUTTON_DETAIL2_EDITEPOS_PLU:
					DatabaseAction( infoDisplay, FALSE );	
					break;

				case BUTTON_DETAIL2_EDITEPOS_MIXMATCH:
				case BUTTON_DETAIL2_EDITEPOS_OFFERGROUP:
				case BUTTON_DETAIL2_EDITEPOS_PROMOTION:
				case BUTTON_DETAIL2_EDITEPOS_TIMEPLAN:
				case BUTTON_DETAIL2_EDITEPOS_WALLETSTAMPRULE:
					DatabaseAction( infoDisplay, DealerFlags.GetGlobalPromotionFlag() );	
					break;

				case BUTTON_DETAIL2_EDITEPOS_LOYALTYSCHEME:
					DatabaseAction(infoDisplay, FALSE);
					break;

				case BUTTON_DETAIL2_EDITEPOS_DEPT:		
					DatabaseAction( infoDisplay, EcrmanOptions.GetGlobalDepartmentFlag() );
					break;

				case BUTTON_DETAIL2_EDITEPOS_GROUP:		
					DatabaseAction( infoDisplay, EcrmanOptions.GetGlobalDeptGroupEposFlag() );
					break;

				case BUTTON_DETAIL2_EDITEPOS_TAX:		
					DatabaseAction( infoDisplay, EcrmanOptions.GetGlobalTaxFlag() );
					break;

				case BUTTON_DETAIL2_EDITEPOS_ACAT:		
					DatabaseAction( infoDisplay, EcrmanOptions.GetGlobalAnalysisCategoryFlag() );
					break;

				case BUTTON_DETAIL2_EDITEPOS_PAYMENT:		
					DatabaseAction( infoDisplay, EcrmanOptions.GetGlobalPaymentFlag() );
					break;

				case BUTTON_DETAIL2_EDITEPOS_SERVER:
					ServerAction( infoDisplay );
					break;
				}
			}
			break;

	#ifdef STOCKMAN_SYSTEM

		case BUTTON_DETAIL1_EDITSTK:	
			{
				switch( infoDisplay.GetDetail2() )
				{
				case BUTTON_DETAIL2_EDITSTK_STOCK:	
					DatabaseAction( infoDisplay, FALSE );	
					break;

				case BUTTON_DETAIL2_EDITSTK_SUPPLIER:	
				case BUTTON_DETAIL2_EDITSTK_DELUNIT:
				case BUTTON_DETAIL2_EDITSTK_STKUNIT:
				case BUTTON_DETAIL2_EDITSTK_SUBUNIT:
				case BUTTON_DETAIL2_EDITSTK_ADJTEXT:
					DatabaseAction( infoDisplay, TRUE );	
					break;

				case BUTTON_DETAIL2_EDITSTK_CATEGORY:	
					DatabaseAction( infoDisplay, StockOptions.GetGlobalCategoryFlag() );	
					break;

				case BUTTON_DETAIL2_EDITSTK_CUSTFLD:	
					DatabaseAction( infoDisplay, StockOptions.GetGlobalCustomFieldFlag() );	
					break;

				case BUTTON_DETAIL2_EDITSTK_ALLOWANCE:	
					DatabaseAction( infoDisplay, StockOptions.GetGlobalAllowanceFlag() );	
					break;
				}
			}
			break;

		case BUTTON_DETAIL1_MOVESTK:	
			{
				switch( infoDisplay.GetDetail2() )
				{
				case BUTTON_DETAIL2_MOVESTK_OPENING:
				case BUTTON_DETAIL2_MOVESTK_DELIVERY:
				case BUTTON_DETAIL2_MOVESTK_RETURN:
					StockpointAction( infoDisplay );
					break;

				case BUTTON_DETAIL2_MOVESTK_SALEPLU:
					if ( StockOptions.GetStockSalesEntryType() == 1 )
						Prompter.Info( "You must enable Plu Manual Sales\nbefore you can use this button." );
					else
						StockpointAction( infoDisplay );
					break;

				case BUTTON_DETAIL2_MOVESTK_SALESTK:
					if ( StockOptions.GetStockSalesEntryType() == 0 )
						Prompter.Info( "You must enable Stock Manual Sales\nbefore you can use this button." );
					else
						StockpointAction( infoDisplay );
					break;

				case BUTTON_DETAIL2_MOVESTK_ADJUSTPLU:
					if ( StockOptions.GetStockAdjustmentsEntryType() == 0 )
						Prompter.Info( "You must enable Plu Adjustments\nbefore you can use this button." );
					else
						StockpointAction( infoDisplay );
					break;

				case BUTTON_DETAIL2_MOVESTK_ADJUSTSTK:
					if ( StockOptions.GetStockAdjustmentsEntryType() == 1 )
						Prompter.Info( "You must enable Stock Adjustments\nbefore you can use this button." );
					else
						StockpointAction( infoDisplay );
					break;

				case BUTTON_DETAIL2_MOVESTK_TRANSFER:
					m_AccessHelpers.m_StockData.BuildDbTransferCounts();
					StockpointAction( infoDisplay );
					break;
				}
			}
			break;

	#endif

		case BUTTON_DETAIL1_KEYBOARD:
			{
				if ( EcrmanOptions.GetFeaturesKeyboardFlag() == FALSE )
					Prompter.Info( "You must enable Keyboard editing\nbefore you can use this button." );
				else
				{
					m_AccessHelpers.m_Keyboard.BuildKeyboardMap();
					if ( BUTTON_DETAIL2_KEYBOARD_USERSELECT == infoDisplay.GetDetail2() )
						ShowContextMenuKeyboardLocation( infoDisplay );
					else
						KeyboardAction( infoDisplay );
				}
			}
			break;

		case BUTTON_DETAIL1_PSET_LOC:
			{
				if ( EcrmanOptions.GetPluPriceSetsFlag() == FALSE )
					Prompter.Info( "You must enable Plu Price Sets\nbefore you can use this button." );
				else
				{
					if (BUTTON_DETAIL2_PSET_USERSELECT == infoDisplay.GetDetail2())
					{
						ShowContextMenuPriceSetLocation(infoDisplay);
					}
					else
					{
						PriceSetAction(infoDisplay);
					}
				}
			}
			break;

		case BUTTON_DETAIL1_PSET_DB:
			{
				if ( EcrmanOptions.GetPluPriceSetsFlag() == FALSE )
					Prompter.Info( "You must enable Plu Price Sets\nbefore you can use this button." );
				else
				{
					if (BUTTON_DETAIL2_PSET_USERSELECT == infoDisplay.GetDetail2())
					{
						ShowContextMenuPriceSetDatabase(infoDisplay);
					}
					else
					{
						PriceSetAction(infoDisplay);
					}
				}
			}
			break;

		case BUTTON_DETAIL1_REPORTS:	

			switch( infoDisplay.GetDetail2() )
			{
			case BUTTON_DETAIL2_REPORTS_PLUHISTORY:	
				if (BUTTON_DETAIL3_REPORTS_USERSELECT == infoDisplay.GetDetail3())
				{
					ShowContextMenuPluSalesHistory(infoDisplay);
				}
				else
				{
					ActionInternal(infoDisplay);
				}
				break;

			case BUTTON_DETAIL2_REPORTS_EPOS_VIEW:
			case BUTTON_DETAIL2_REPORTS_EPOS_PRINT:
				if (BUTTON_DETAIL3_REPORTS_USERSELECT == infoDisplay.GetDetail3())
				{
					ShowContextMenuEposReports(infoDisplay);
				}
				else
				{
					ActionInternal(infoDisplay);
				}
				break;

			case BUTTON_DETAIL2_REPORTS_CUSTOMER:	

				if ( ( EcrmanOptions.GetFeaturesCustomerBalanceFlag() == TRUE ) && ( EcrmanOptions.GetReportsCustomerTransactionFlag() == TRUE ) )
				{
					if (BUTTON_DETAIL3_REPORTS_USERSELECT == infoDisplay.GetDetail3())
					{
						ShowContextMenuCustomerReports(infoDisplay);
					}
					else
					{
						ActionInternal(infoDisplay);
					}
				}
				else
				{
					Prompter.Error( "You must enable Customer Transaction Reports\nbefore you can use this button." );
				}

				break;

			case BUTTON_DETAIL2_REPORTS_TIMEATTEND:	

				if ( EcrmanOptions.GetFeaturesServerSettingsType() != SERVER_NONE )
				{
					if (BUTTON_DETAIL3_REPORTS_USERSELECT == infoDisplay.GetDetail3())
					{
						ShowContextMenuTimeAttend(infoDisplay);
					}
					else
					{
						ActionInternal(infoDisplay);
					}
				}
				else
				{
					Prompter.Error( "You must enable server (clerk) editing\nbefore you can use this button." );
				}

				break;
			
	#ifdef STOCKMAN_SYSTEM

			case BUTTON_DETAIL2_REPORTS_STKHISTORY:	
				if ( BUTTON_DETAIL3_REPORTS_USERSELECT == infoDisplay.GetDetail3() )
					ShowContextMenuStockSalesHistory( infoDisplay );
				else
					ActionInternal( infoDisplay );
				break;

			case BUTTON_DETAIL2_REPORTS_STKLEVEL:	
				if ( BUTTON_DETAIL3_REPORTS_USERSELECT == infoDisplay.GetDetail3() )
					ShowContextMenuStockLevel( infoDisplay );
				else
					ActionInternal( infoDisplay );
				break;

			case BUTTON_DETAIL2_REPORTS_STKAUDIT:	
				if ( BUTTON_DETAIL3_REPORTS_USERSELECT == infoDisplay.GetDetail3() )
					ShowContextMenuStockAudit( infoDisplay );
				else
					ActionInternal( infoDisplay );
				break;
	#else

			case BUTTON_DETAIL2_REPORTS_ECRMANSTK:	
				if (BUTTON_DETAIL3_REPORTS_USERSELECT == infoDisplay.GetDetail3())
				{
					ShowContextMenuEcrmanStock(infoDisplay);
				}
				else
				{
					ActionInternal(infoDisplay);
				}
				break;

	#endif

			}

			break;

		case BUTTON_DETAIL1_TASKS:
			{
				m_TaskDatabase.Read();

				switch( infoDisplay.GetDetail2() )
				{
				case BUTTON_DETAIL2_TASKS_ADHOC:
				case BUTTON_DETAIL2_TASKS_EDITPRESET:
					ActionInternal( infoDisplay );
					break;

				case BUTTON_DETAIL2_TASKS_RUNPRESET:
					{
						int nTaskNo = infoDisplay.GetDetail3();
					
						if (BUTTON_DETAIL3_TASKS_USERSELECT == nTaskNo)
						{
							ShowContextMenuTask(infoDisplay);
						}
						else
						{
							if (m_AccessHelpers.m_Task.CanRunTask(nTaskNo) == FALSE)
							{
								ShowAuthorisationError(3);
							}
							else if (m_TaskDatabase.GetLibraryTaskJobCount(nTaskNo) == 0)
							{
								Prompter.Error("The selected task does not contain any jobs.");
							}
							else
							{
								ActionInternal(infoDisplay);
							}
						}
					}
					break;
				}
			}
			break;
		}
	}
}

/**********************************************************************/

void CButtonActionHandler::KeyboardAction(CButtonInfo& infoDisplay)
{
	CString strToken;
	strToken.Format("%7.7d", infoDisplay.GetDetail2());

	bool bFound = FALSE;
	for (int n = 0; n < m_AccessHelpers.m_Keyboard.GetKeyboardMapSize(); n++)
	{
		m_AccessHelpers.m_Keyboard.GetKeyboardMapInfo(n, m_KeyboardMapInfo);

		CString strMapToken = "";
		switch (m_KeyboardMapInfo.m_nEntryType)
		{
		case KBD_MAP_SET:
			strMapToken.Format("1%3.3d%3.3d",
				dbDatabase.GetDbNo(m_KeyboardMapInfo.m_nDbIdx),
				dbKeyboardSet.GetKbsNo(m_KeyboardMapInfo.m_nKbsIdx));
			break;

		case KBD_MAP_LOC:
			strMapToken.Format("0%6.6d",
				dbLocation.GetNetworkLocNo(m_KeyboardMapInfo.m_nLocIdx));
			break;
		}

		if (strToken == strMapToken)
		{
			bFound = TRUE;
			break;
		}
	}

	if (TRUE == bFound)
	{
		if (BUTTON_DETAIL3_KEYBOARD_USERSELECT == infoDisplay.GetDetail3())
		{
			ShowContextMenuKeyboardNumber(infoDisplay);
		}
		else
		{
			ActionInternal(infoDisplay);
		}
	}
	else
	{
		bool bExists = FALSE;
		int nEntityNo1 = atoi(strToken.Mid(1, 3));
		int nEntityNo2 = atoi(strToken.Right(3));

		if (strToken.Left(1) == "1")
		{
			int nIndex;
			if (dbKeyboardSet.FindKeyboardSetByNumber(nEntityNo1, nEntityNo2, nIndex) == TRUE)
			{
				ShowAuthorisationError(7);
			}
			else
			{
				ShowUnknownKeyboardSetError(nEntityNo1, nEntityNo2);
			}
		}
		else
		{
			int nIndex;
			if (dbLocation.FindLocationByNumber(nEntityNo1, nEntityNo2, nIndex) == TRUE)
			{
				ShowAuthorisationError(7);
			}
			else
			{
				ShowUnknownKeyboardLocError(nEntityNo1, nEntityNo2);
			}
		}
	}
}

/**********************************************************************/

void CButtonActionHandler::PriceSetAction(CButtonInfo& infoDisplay)
{
	switch (infoDisplay.GetDetail1())
	{
	case BUTTON_DETAIL1_PSET_LOC:
	{
		int nNwkLocNo = infoDisplay.GetDetail2();
		int nNwkNo = nNwkLocNo / 1000;
		int nLocNo = nNwkLocNo % 1000;

		int nLocIdx;
		if (dbLocation.FindLocationByNumber(nNwkNo, nLocNo, nLocIdx) == FALSE)
		{
			ShowUnknownLocationError(nNwkNo, nLocNo);
			return;
		}

		if (BUTTON_DETAIL3_PSET_USERSELECT == infoDisplay.GetDetail3())
		{
			ShowContextMenuPriceSetNumber(infoDisplay, nLocIdx);
		}
		else
		{
			ActionInternal(infoDisplay);
		}
	}
	break;

	case BUTTON_DETAIL1_PSET_DB:
	{
		int nDbIdx;
		int nDbNo = infoDisplay.GetDetail2();
		if (dbDatabase.FindDatabaseByNumber(nDbNo, nDbIdx) == FALSE)
		{
			ShowUnknownDatabaseError(nDbNo);
			return;
		}

		if (BUTTON_DETAIL3_PSET_USERSELECT == infoDisplay.GetDetail3())
		{
			ShowContextMenuPriceSetNumber(infoDisplay, nDbIdx);
		}
		else
		{
			CArray<int, int> LocSet;
			dbLocation.GetDatabaseLocIdxSet(nDbIdx, LocSet);

			int nDbPSetNo = infoDisplay.GetDetail3();
			infoDisplay.SetDetail3(0);

			for (int x = 0; x < LocSet.GetSize(); x++)
			{
				int nLocIdx = LocSet.GetAt(x);

				if (dbLocation.GetDbPriceSet1(nLocIdx) == nDbPSetNo)
				{
					infoDisplay.SetDetail3((nLocIdx * 4) + 1);
				}
				else if (dbLocation.GetDbPriceSet2(nLocIdx) == nDbPSetNo)
				{
					infoDisplay.SetDetail3((nLocIdx * 4) + 2);
				}
				else if (dbLocation.GetDbPriceSet3(nLocIdx) == nDbPSetNo)
				{
					infoDisplay.SetDetail3((nLocIdx * 4) + 3);
				}

				if (infoDisplay.GetDetail3() != 0)
				{
					break;
				}
			}

			if (infoDisplay.GetDetail3() != 0)
			{
				ActionInternal(infoDisplay);
			}
			else
			{
				Prompter.Info("The selected database price set\nis not currently in use.");
			}
		}
	}
	}
}

/**********************************************************************/

void CButtonActionHandler::ServerAction(CButtonInfo& infoDisplay)
{
	int nEntityType = SERVER_LOC;

	if (EcrmanOptions.GetFeaturesServerRestrictionFlag() == FALSE)
	{
		int nServerType = EcrmanOptions.GetFeaturesServerSettingsType();
		switch (nServerType)
		{
		case SERVER_DB:
		case SERVER_SYS:
			nEntityType = nServerType;
			break;
		}
	}

	switch (nEntityType)
	{
	case SERVER_DB:
	{
		int nDbIdx = -1;
		int nDbNo = infoDisplay.GetDetail3();

		if ((nDbNo >= 1) && (nDbNo <= 999))
		{
			if (dbDatabase.FindDatabaseByNumber(nDbNo, nDbIdx) == FALSE)
			{
				ShowUnknownDatabaseError(nDbNo);
				return;
			}
		}

		if (nDbIdx == -1)
		{
			ShowContextMenuDatabaseServer(infoDisplay);
		}
		else
		{
			EditServerDatabase(infoDisplay, nDbIdx);
		}
	}
	break;

	case SERVER_LOC:
	{
		int nLocIdx = -1;
		int nNwkLocNo = infoDisplay.GetDetail3();

		if (nNwkLocNo >= 1001)
		{
			int nNwkNo = nNwkLocNo / 1000;
			int nLocNo = nNwkLocNo % 1000;

			if (dbLocation.FindLocationByNumber(nNwkNo, nLocNo, nLocIdx) == FALSE)
			{
				ShowUnknownLocationError(nNwkNo, nLocNo);
				return;
			}
		}

		if (nLocIdx == -1)
		{
			ShowContextMenuLocationServer(infoDisplay);
		}
		else
		{
			EditServerLocation(infoDisplay, nLocIdx);
		}
	}
	break;

	case SERVER_SYS:
		EditServerSystem(infoDisplay);
		break;
	}
}

/**********************************************************************/

void CButtonActionHandler::EditServerSystem( CButtonInfo& infoDisplay )
{
	if (PasswordArray.CheckLocationAccessSystem() == FALSE)
	{
		ShowAuthorisationError(3);
	}
	else
	{
		infoDisplay.SetInternalParams( "-1,-1,-1" );
		ActionInternal( infoDisplay );
	}
}

/**********************************************************************/

void CButtonActionHandler::EditServerDatabase( CButtonInfo& infoDisplay, int nServerDbIdx )
{
	if (PasswordArray.CheckLocationAccessDbIdx(nServerDbIdx) == FALSE)
	{
		ShowAuthorisationError(4);
	}
	else
	{
		CString strParams = "";
		strParams.Format( "%d,-1,-1", nServerDbIdx );
		infoDisplay.SetInternalParams( strParams );
		ActionInternal( infoDisplay );
	}
}

/**********************************************************************/

void CButtonActionHandler::EditServerLocation( CButtonInfo& infoDisplay, int nServerLocIdx )
{
	if (PasswordArray.CheckLocationAccessLocIdx(nServerLocIdx) == FALSE)
	{
		ShowAuthorisationError(4);
	}
	else
	{
		int nServerDbIdx = 0;
		int nDbNo = dbLocation.GetDbNo( nServerLocIdx );
		
		if (dbDatabase.FindDatabaseByNumber(nDbNo, nServerDbIdx) == FALSE)
		{
			ShowUnknownDatabaseError(nDbNo);
		}
		else
		{
			int nRestrictLocIdx = EcrmanOptions.GetFeaturesServerRestrictionFlag() ? nServerLocIdx : -1;

			switch( EcrmanOptions.GetFeaturesServerSettingsType() )
			{
			case SERVER_SYS:
				nServerDbIdx = -1;
				nServerLocIdx = -1;
				break;

			case SERVER_DB:
				nServerLocIdx = -1;
				break;
			}

			CString strParams = "";
			strParams.Format( "%d,%d,%d", nServerDbIdx, nServerLocIdx, nRestrictLocIdx );
			infoDisplay.SetInternalParams( strParams );

			ActionInternal( infoDisplay );
		}
	}
}

/**********************************************************************/

void CButtonActionHandler::DatabaseAction( CButtonInfo& infoDisplay, bool bGlobal ) 
{
	if ( TRUE == bGlobal )
	{
		if (PasswordArray.CheckLocationAccessSystem() == FALSE)
		{
			ShowAuthorisationError(3);
		}
		else
		{
			infoDisplay.SetDetail3( dbDatabase.GetDbNo(0) );
			ActionInternal( infoDisplay );
		}
	}
	else if (infoDisplay.GetDetail3() != BUTTON_DETAIL3_EDIT_USERSELECT)
	{
		int nDbIdx = 0;
		int nDbNo = infoDisplay.GetDetail3();
		if (dbDatabase.FindDatabaseByNumber(nDbNo, nDbIdx) == FALSE)
		{
			ShowUnknownDatabaseError(nDbNo);
		}
		else if (PasswordArray.CheckLocationAccessDbIdx(nDbIdx) == FALSE)
		{
			ShowAuthorisationError(4);
		}
		else
		{
			ActionInternal(infoDisplay);
		}
	}
	else if (dbDatabase.GetSize() == 1)
	{
		infoDisplay.SetDetail3(dbDatabase.GetDbNo(0));

		if (PasswordArray.CheckLocationAccessDbIdx(0) == FALSE)
		{
			ShowAuthorisationError(5);
		}
		else
		{
			ActionInternal(infoDisplay);
		}
	}
	else
	{
		ShowContextMenuDatabase( infoDisplay );
	}
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

void CButtonActionHandler::StockpointAction( CButtonInfo& infoDisplay ) 
{
	if ( infoDisplay.GetDetail3() != BUTTON_DETAIL3_EDIT_USERSELECT )
	{
		int nSpIdx = 0;
		int nDbNo = infoDisplay.GetDetail3() / 1000;
		int nSpNo = infoDisplay.GetDetail3() % 1000;
		if (dbStockpoint.FindStockpointByNumber(nDbNo, nSpNo, nSpIdx) == FALSE)
		{
			ShowUnknownStockpointError(nDbNo, nSpNo);
		}
		else if (PasswordArray.CheckLocationAccessSpIdx(nSpIdx) == FALSE)
		{
			ShowAuthorisationError(4);
		}
		else
		{
			ActionInternal(infoDisplay);
		}
	}
	else if (dbStockpoint.GetSize() == 1)
	{
		infoDisplay.SetDetail3((dbStockpoint.GetDbNo(0) * 1000) + dbStockpoint.GetSpNo(0));

		if (PasswordArray.CheckLocationAccessSpIdx(0) == FALSE)
		{
			ShowAuthorisationError(5);
		}
		else
		{
			ActionInternal(infoDisplay);
		}
	}
	else
	{
		ShowContextMenuStockpoint( infoDisplay );
	}
}

/**********************************************************************/
#endif
/**********************************************************************/

void CButtonActionHandler::ActionInternal( CButtonInfo& infoDisplay )
{
#ifdef STOCKMAN_SYSTEM
	CMainFrameStockman* pFrame = (CMainFrameStockman*) m_AccessHelpers. GetParentWnd();
#else
	CMainFrameEcrman* pFrame = (CMainFrameEcrman*) m_AccessHelpers. GetParentWnd();
#endif

	switch( infoDisplay.GetDetail1() )
	{
	case BUTTON_DETAIL1_SETUP:	

		if ( PrepareCommand() == TRUE )
		{
			switch( infoDisplay.GetDetail2() )
			{
			case BUTTON_DETAIL2_SETUP_SYSTEM:
				m_AccessHelpers. m_Setup.SetupSystem();
				break;

			case BUTTON_DETAIL2_SETUP_OPTIONS:
				m_AccessHelpers. m_Setup.EditOptions();
				break;
			}
		}

		break;

	case BUTTON_DETAIL1_EXTERNAL:	

		if ( PrepareCommand() == TRUE )
		{
			switch( infoDisplay.GetDetail2() )
			{
			case BUTTON_DETAIL2_EXTERNAL_LOYALTY:
				switch( SysInfo.GetLoyaltyOrSmartPayType() )
				{
				case LOYALTY_TYPE_LOY_V2:
					m_AccessHelpers.m_Loyalty.RunLoyaltyV2();
					break;

				case LOYALTY_TYPE_LOY_V3:
					m_AccessHelpers.m_Loyalty.RunLoyaltyV3();
					break;

				case LOYALTY_TYPE_LOY_V4:
					m_AccessHelpers.m_Loyalty.RunLoyaltyV4();
					break;

				case LOYALTY_TYPE_SMP_V1:
					m_AccessHelpers.m_Loyalty.RunSmartPayV1();
					break;

				case LOYALTY_TYPE_SMP_V2:
					m_AccessHelpers.m_Loyalty.RunSmartPayV2();
					break;

				case LOYALTY_TYPE_SMP_V4:
					m_AccessHelpers.m_Loyalty.RunSmartPayV4();
					break;
				}
				break;
			}
		}
		break;

	case BUTTON_DETAIL1_EDITEPOS:	

		switch( infoDisplay.GetDetail2() )
		{
		case BUTTON_DETAIL2_EDITEPOS_PLU:
		case BUTTON_DETAIL2_EDITEPOS_DEPT:
		case BUTTON_DETAIL2_EDITEPOS_GROUP:
		case BUTTON_DETAIL2_EDITEPOS_TAX:
		case BUTTON_DETAIL2_EDITEPOS_ACAT:
		case BUTTON_DETAIL2_EDITEPOS_PAYMENT:
		case BUTTON_DETAIL2_EDITEPOS_MIXMATCH:
		case BUTTON_DETAIL2_EDITEPOS_WALLETSTAMPRULE:
		case BUTTON_DETAIL2_EDITEPOS_OFFERGROUP:
		case BUTTON_DETAIL2_EDITEPOS_PROMOTION:
		case BUTTON_DETAIL2_EDITEPOS_TIMEPLAN:
		case BUTTON_DETAIL2_EDITEPOS_LOYALTYSCHEME:
			{
				int nDbIdx;
				int nDbNo = infoDisplay.GetDetail3();
				if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == TRUE )
				{
					if ( PrepareCommand() == TRUE )
					{
						switch( infoDisplay.GetDetail2() )
						{
						case BUTTON_DETAIL2_EDITEPOS_PLU:
							m_AccessHelpers.m_Plu.EditPlu( nDbNo, TRUE );
							break;

						case BUTTON_DETAIL2_EDITEPOS_DEPT:
							m_AccessHelpers.m_EcrData.EditDept( nDbNo );
							break;

						case BUTTON_DETAIL2_EDITEPOS_GROUP:
							m_AccessHelpers.m_EcrData.EditGroup( nDbNo );
							break;

						case BUTTON_DETAIL2_EDITEPOS_TAX:
							m_AccessHelpers.m_EcrData.EditTax( nDbNo );
							break;

						case BUTTON_DETAIL2_EDITEPOS_ACAT:
							m_AccessHelpers.m_EcrText.EditAnalysisCategory( nDbNo );
							break;

						case BUTTON_DETAIL2_EDITEPOS_PAYMENT:
							m_AccessHelpers.m_EcrText.EditPayment( nDbNo );
							break;

						case BUTTON_DETAIL2_EDITEPOS_MIXMATCH:
							m_AccessHelpers.m_MixMatch.EditMixMatch( nDbNo );
							break;

						case BUTTON_DETAIL2_EDITEPOS_WALLETSTAMPRULE:
							switch( SysInfo.GetLoyaltyOrSmartPayType())
							{ 
							case LOYALTY_TYPE_LOY_V4:
							case LOYALTY_TYPE_SMP_V4:
								if (EcrmanOptions.GetFeaturesWalletStampRuleFlag() == FALSE)
								{
									Prompter.Error("This feature is not enabled in system options.");
								}
								else
								{
									m_AccessHelpers.m_MixMatch.EditWalletStampRule(nDbNo);
								}
								break;

							default:
								Prompter.Error( "This feature requires Loyalty or SmartPay v4" );
								break;
							}
							break;

						case BUTTON_DETAIL2_EDITEPOS_LOYALTYSCHEME:
							if ((SysInfo.GotLoyaltyLink() == FALSE) && (SysInfo.GetLoyaltyOrSmartPayType() != LOYALTY_TYPE_UNKNOWN))
							{
								Prompter.Error("This feature requires Loyalty or SmartPay.");
							}
							else
							{
								m_AccessHelpers.m_EcrData.EditLoyaltyScheme(nDbNo);
							}	
							break;

						case BUTTON_DETAIL2_EDITEPOS_OFFERGROUP:
							m_AccessHelpers.m_MixMatch.EditOfferGroup( nDbNo );
							break;

						case BUTTON_DETAIL2_EDITEPOS_PROMOTION:
							m_AccessHelpers.m_MixMatch.EditPromotion( nDbNo );
							break;

						case BUTTON_DETAIL2_EDITEPOS_TIMEPLAN:
							m_AccessHelpers.m_MixMatch.EditTimePlan( nDbNo );
							break;
						}
					}
				}
			}
			break;

		case BUTTON_DETAIL2_EDITEPOS_SERVER:
			if ( PrepareCommand() == TRUE )
			{
				CCSV csv( infoDisplay.GetInternalParams() );
				int nServerDbIdx = csv.GetInt(0);
				int nServerLocIdx = csv.GetInt(1);
				int nTerminalLocIdx = csv.GetInt(2);
				m_AccessHelpers. m_Server.EditServer( nServerDbIdx, nServerLocIdx, nTerminalLocIdx );
			}
			break;
		}
		break;

#ifdef STOCKMAN_SYSTEM

	case BUTTON_DETAIL1_EDITSTK:	

		switch( infoDisplay.GetDetail2() )
		{
		case BUTTON_DETAIL2_EDITSTK_STOCK:
		case BUTTON_DETAIL2_EDITSTK_CATEGORY:
		case BUTTON_DETAIL2_EDITSTK_CUSTFLD:
		case BUTTON_DETAIL2_EDITSTK_ALLOWANCE:
			{
				int nDbIdx;
				int nDbNo = infoDisplay.GetDetail3();
				if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == TRUE )
				{
					if ( PrepareCommand() == TRUE )
					{
						switch( infoDisplay.GetDetail2() )
						{
						case BUTTON_DETAIL2_EDITSTK_STOCK:
							m_AccessHelpers. m_StockData.EditStock( nDbNo, TRUE );
							break;

						case BUTTON_DETAIL2_EDITSTK_CATEGORY:
							m_AccessHelpers. m_StockText.EditCategory( nDbNo );
							break;

						case BUTTON_DETAIL2_EDITSTK_CUSTFLD:
							m_AccessHelpers. m_StockText.EditCustomField( nDbNo );
							break;

						case BUTTON_DETAIL2_EDITSTK_ALLOWANCE:
							m_AccessHelpers. m_StockText.EditAllowance( nDbNo );
							break;
						}
					}
				}
			}
			break;

		case BUTTON_DETAIL2_EDITSTK_SUPPLIER:
		case BUTTON_DETAIL2_EDITSTK_DELUNIT:
		case BUTTON_DETAIL2_EDITSTK_STKUNIT:
		case BUTTON_DETAIL2_EDITSTK_SUBUNIT:
		case BUTTON_DETAIL2_EDITSTK_ADJTEXT:
			if ( PrepareCommand() == TRUE )
			{
				switch( infoDisplay.GetDetail2() )
				{
				case BUTTON_DETAIL2_EDITSTK_SUPPLIER:
					m_AccessHelpers. m_StockText.EditSupplier();
					break;

				case BUTTON_DETAIL2_EDITSTK_DELUNIT:
					m_AccessHelpers. m_StockText.EditDeliveryUnit();
					break;

				case BUTTON_DETAIL2_EDITSTK_STKUNIT:
					m_AccessHelpers. m_StockText.EditStockUnit();
					break;

				case BUTTON_DETAIL2_EDITSTK_SUBUNIT:
					m_AccessHelpers. m_StockText.EditSubUnit();
					break;

				case BUTTON_DETAIL2_EDITSTK_ADJTEXT:
					m_AccessHelpers. m_StockText.EditAdjustmentTexts();
					break;
				}
			}
			break;
		}
		break;

	case BUTTON_DETAIL1_MOVESTK:	

		{
			int nSpIdx = 0;
			int nDbNo = infoDisplay.GetDetail3() / 1000;
			int nSpNo = infoDisplay.GetDetail3() % 1000;

			int nDbIdx = 0;
			dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx );

			if ( dbStockpoint.FindStockpointByNumber( nDbNo, nSpNo, nSpIdx ) == TRUE )
			{
				switch( infoDisplay.GetDetail2() )
				{
				case BUTTON_DETAIL2_MOVESTK_OPENING:
				case BUTTON_DETAIL2_MOVESTK_DELIVERY:
				case BUTTON_DETAIL2_MOVESTK_RETURN:
				case BUTTON_DETAIL2_MOVESTK_SALEPLU:
				case BUTTON_DETAIL2_MOVESTK_SALESTK:
				case BUTTON_DETAIL2_MOVESTK_ADJUSTPLU:
				case BUTTON_DETAIL2_MOVESTK_ADJUSTSTK:
				case BUTTON_DETAIL2_MOVESTK_TRANSFER:

					if ( PrepareCommand() == TRUE )
					{
						switch( infoDisplay.GetDetail2() )
						{
						case BUTTON_DETAIL2_MOVESTK_OPENING:
							m_AccessHelpers.m_StockData.EditStockLevel( nSpIdx, NODE_OPENING, TRUE );
							break;

						case BUTTON_DETAIL2_MOVESTK_DELIVERY:
							m_AccessHelpers.m_StockData.EditMiscStockLevels( nSpIdx, NODE_DELIVERY, TRUE );
							break;

						case BUTTON_DETAIL2_MOVESTK_RETURN:
							m_AccessHelpers.m_StockData.EditMiscStockLevels( nSpIdx, NODE_RETURN, TRUE );
							break;

						case BUTTON_DETAIL2_MOVESTK_SALEPLU:
							m_AccessHelpers.m_StockData.EditManualSales( nSpIdx, NODE_MANUALSALE_PLU, TRUE );
							break;

						case BUTTON_DETAIL2_MOVESTK_SALESTK:
							m_AccessHelpers.m_StockData.EditManualSales( nSpIdx, NODE_MANUALSALE_STOCK, TRUE );
							break;

						case BUTTON_DETAIL2_MOVESTK_ADJUSTPLU:
							m_AccessHelpers.m_StockData.EditMiscStockLevels( nSpIdx, NODE_ADJUSTMENT_PLU, TRUE );
							break;

						case BUTTON_DETAIL2_MOVESTK_ADJUSTSTK:
							m_AccessHelpers.m_StockData.EditMiscStockLevels( nSpIdx, NODE_ADJUSTMENT_STOCK, TRUE );
							break;

						case BUTTON_DETAIL2_MOVESTK_TRANSFER:

							{
								bool bCanTransfer = FALSE;
								if ( dbStockpoint.GetTransferInFlag( nSpIdx ) == TRUE )
								{
									if ( m_AccessHelpers.m_StockData.GetDbTransferTargets(nDbIdx) >= 2 )
										bCanTransfer = TRUE;
								}
								else
								{
									if ( m_AccessHelpers.m_StockData.GetDbTransferTargets(nDbIdx) >= 1 )
										bCanTransfer = TRUE;
								}

								if ( dbStockpoint.GetTransferOutFlag( nSpIdx ) == FALSE )
									Prompter.Error( "The selected stockpoint does not allow transfers out." );
								else if ( FALSE == bCanTransfer )
									Prompter.Error( "There are no stockpoints available to accept\nincoming transfers." );
								else
									m_AccessHelpers.m_StockData.EditStockLevel( nSpIdx, NODE_TRANSFER, TRUE );
							}
							break;
						}
					}
					break;
				}
			}
		}
		break;

#endif

	case BUTTON_DETAIL1_KEYBOARD:
		if ( PrepareCommand() == TRUE )
		{
			CDataNodeInfoKeyboard infoNode;
			m_AccessHelpers. m_Keyboard.ConvertMapInfoToNodeInfo( m_KeyboardMapInfo, infoNode );
			infoNode.m_nKbNo = infoDisplay.GetDetail3();

			if ( ( infoNode.m_nKbNo < 1 ) || ( infoNode.m_nKbNo > EcrmanOptions.GetFeaturesKeyboardsPerLocation() ) )
			{
				CString str;
				str.Format( "Invalid Keyboard Number %d", infoNode.m_nKbNo );
				Prompter.Error( str );
			}
			else
			{
				m_AccessHelpers. m_Keyboard.EditKeyboard( infoNode );
			}
		}
		break;

	case BUTTON_DETAIL1_PSET_LOC:
		if ( PrepareCommand() == TRUE )
		{
			int nNwkLocNo = infoDisplay.GetDetail2();
			int nPSet = infoDisplay.GetDetail3();
			
			int nLocIdx;
			int nNwkNo = nNwkLocNo / 1000;
			int nLocNo = nNwkLocNo % 1000;
			if ( dbLocation.FindLocationByNumber( nNwkNo, nLocNo, nLocIdx ) == TRUE )
			{
				if ( ( nPSet < 1 ) || ( nPSet > dbLocation.GetPluPriceSetCount( nLocIdx ) ) )
				{
					ShowUnknownPriceSetError( nNwkNo, nLocNo, nPSet );
				}
				else
				{
					int nDbNo = dbLocation.GetDbNo( nLocIdx );
					m_AccessHelpers.m_Plu.EditPriceSet( nNwkNo, nDbNo, nLocNo, nPSet, TRUE );
				}
			}
			else
			{
				ShowUnknownLocationError( nNwkNo, nLocNo );
			}
		}
		break;

	case BUTTON_DETAIL1_PSET_DB:
		if ( PrepareCommand() == TRUE )
		{
			int nInfo = infoDisplay.GetDetail3();
			int nLocIdx = nInfo / 4;
			int nPSet = nInfo % 4;
			int nNwkNo = dbLocation.GetNetworkNo( nLocIdx );
			int nDbNo = dbLocation.GetDbNo( nLocIdx );
			int nLocNo = dbLocation.GetLocNo( nLocIdx );
			m_AccessHelpers.m_Plu.EditPriceSet( nNwkNo, nDbNo, nLocNo, nPSet, TRUE );
		}
		break;

	case BUTTON_DETAIL1_REPORTS:

		if ( PrepareCommand() == TRUE )
		{
			switch( infoDisplay.GetDetail2() )
			{
			case BUTTON_DETAIL2_REPORTS_PLUHISTORY:
				m_AccessHelpers. m_Reports.SalesHistoryPlu( infoDisplay.GetDetail3(), ( infoDisplay.GetActionFlag1() == FALSE ) );
				break;

			case BUTTON_DETAIL2_REPORTS_EPOS_VIEW:
				m_AccessHelpers. m_Reports.EposReports( infoDisplay.GetDetail3(), FALSE, ( infoDisplay.GetActionFlag1() == FALSE ) );
				break;

			case BUTTON_DETAIL2_REPORTS_EPOS_PRINT:
				m_AccessHelpers. m_Reports.EposReports( infoDisplay.GetDetail3(), TRUE, ( infoDisplay.GetActionFlag1() == FALSE ) );
				break;

			case BUTTON_DETAIL2_REPORTS_CUSTOMER:
				m_AccessHelpers. m_Reports.CustomerTransactions( infoDisplay.GetDetail3(), ( infoDisplay.GetActionFlag1() == FALSE ) );
				break;

			case BUTTON_DETAIL2_REPORTS_TIMEATTEND:
				m_AccessHelpers. m_Reports.TimeAttendance( infoDisplay.GetDetail3(), ( infoDisplay.GetActionFlag1() == FALSE ) );
				break;
		
#ifdef STOCKMAN_SYSTEM

			case BUTTON_DETAIL2_REPORTS_STKHISTORY:
				m_AccessHelpers. m_Reports.SalesHistoryStock( infoDisplay.GetDetail3(), ( infoDisplay.GetActionFlag1() == FALSE ) );
				break;

			case BUTTON_DETAIL2_REPORTS_STKLEVEL:
				{
					CLocationSelectorEntity LocSelEntity;
					LocSelEntity.SetSystemNode();
					m_AccessHelpers. m_StockData.HandleStockReports( infoDisplay.GetDetail3(), ( infoDisplay.GetActionFlag1() == FALSE ), LocSelEntity );
				}
				break;

			case BUTTON_DETAIL2_REPORTS_STKAUDIT:
				{
					CLocationSelectorEntity LocSelEntity;
					LocSelEntity.SetSystemNode();
					m_AccessHelpers. m_StockData.HandleStockAudits( infoDisplay.GetDetail3(), ( infoDisplay.GetActionFlag1() == FALSE ), LocSelEntity );
				}
				break;

	#else

			case BUTTON_DETAIL2_REPORTS_ECRMANSTK:
				{
					CLocationSelectorEntity LocSelEntity;
					LocSelEntity.SetSystemNode();
					m_AccessHelpers.m_Reports.EcrmanStockLevel( infoDisplay.GetDetail3(), ( infoDisplay.GetActionFlag1() == FALSE ) );
				}
				break;

	#endif
			}

			break;
		}
		break;

	case BUTTON_DETAIL1_TASKS:

		switch( infoDisplay.GetDetail2() )
		{
		case BUTTON_DETAIL2_TASKS_ADHOC:
		case BUTTON_DETAIL2_TASKS_EDITPRESET:
		case BUTTON_DETAIL2_TASKS_RUNPRESET:

			if ( PrepareCommand() == TRUE )
			{
				switch( infoDisplay.GetDetail2() )
				{
				case BUTTON_DETAIL2_TASKS_ADHOC:
					m_AccessHelpers. m_Task.AdhocTask();
					break;

				case BUTTON_DETAIL2_TASKS_EDITPRESET:
					if ( m_AccessHelpers. m_Task.SetupTasks() == TRUE )
						pFrame -> BuildJobMenu();
					break;

				case BUTTON_DETAIL2_TASKS_RUNPRESET:
					{
						int nTaskNo = infoDisplay.GetDetail3();
						m_AccessHelpers. m_Task.PresetTask( nTaskNo );
					}
					break;
				}
			}
			break;
		}
		break;
	}
}

/**********************************************************************/
