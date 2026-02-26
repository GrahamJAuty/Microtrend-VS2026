/**********************************************************************/

void CMenuChanger::EatMenuTopLevel ( CMenu* pMenu )
{
	m_bPopup = FALSE;
	EatMenuRecursive ( pMenu, 0 );
}

/**********************************************************************/

void CMenuChanger::EatMenuPopup ( CMenu* pMenu )
{
	m_bPopup = TRUE;
	EatMenuRecursive ( pMenu, 0 );
}

/**********************************************************************/

void CMenuChanger::EatMenuRecursive ( CMenu* pMenu, int nItemDepth )
{
	UINT uPos = 0;
	for ( int nPos = 0, uPos = 0; nPos < pMenu -> GetMenuItemCount(); nPos++, uPos++ )
	{
		CCSV csv;
		
		CString strItemText;
		pMenu -> GetMenuString ( uPos, strItemText, MF_BYPOSITION );
		int nItemId = pMenu -> GetMenuItemID ( nPos );

		switch ( nItemId )
		{
		case ( UINT ) -1:
			m_MenuArray.AddSubMenu ( strItemText, nItemDepth );
			EatMenuRecursive ( pMenu -> GetSubMenu ( nPos ), nItemDepth + 1 );
			break;

		case 0:
			m_MenuArray.AddSeparator( nItemDepth );
			break;

		default:
			m_MenuArray.AddOption ( nItemId, strItemText, nItemDepth );
			break;
		}	
	}
	m_MenuArray.AddEndMenu( nItemDepth );
}

/**********************************************************************/

void CMenuChanger::BuildMenu ( CMenu* pMenu )
{
	while ( pMenu -> GetMenuItemCount() > 0 )
		pMenu -> DeleteMenu ( 0, MF_BYPOSITION );

	CPtrArray MenuPtrArray;
	CStringArray MenuNameArray;
	CWordArray MenuShowArray;
	CWordArray MenuEnableArray;

	MenuPtrArray.Add ( pMenu );

	bool bAllowSeparator = FALSE;

	for ( int nIndex = 0; nIndex < m_MenuArray.GetSize(); nIndex++ )
	{
		CMenuChangerCSVRecord Record;
		m_MenuArray.GetAt ( nIndex, Record );

		bool bEnable = Record.GetEnableFlag();
		bool bDisplay = ( Record.GetDisplayFlag() && (!Record.GetKillFlag()) );
		
		int nMenuIndex = MenuPtrArray.GetSize() - 1;

		switch ( Record.GetItemType() )
		{
		case MENU_ITEM_OPTION:
			{
				if ( bDisplay == TRUE )
				{
					CMenu* pMenu = ( CMenu* ) MenuPtrArray.GetAt ( nMenuIndex );
				
					if ( bEnable == TRUE )
						pMenu -> AppendMenu ( MF_STRING | MF_ENABLED, Record.GetItemId(), Record.GetItemText() );
					else
						pMenu -> AppendMenu ( MF_STRING | MF_GRAYED, Record.GetItemId(), Record.GetItemText() );
				}
			}
			break;

		case MENU_ITEM_SEPARATOR:
			{
				CMenu* pMenu = ( CMenu* ) MenuPtrArray.GetAt ( nMenuIndex );
				pMenu -> InsertMenu ( -1, MF_BYPOSITION | MF_SEPARATOR, 0, "" );
			}
			break;

		case MENU_ITEM_SUBMENU:
			{
				CMenu* PopupMenu = new ( CMenu );
				PopupMenu -> CreatePopupMenu();
				MenuPtrArray.Add ( PopupMenu );
				MenuNameArray.Add ( Record.GetItemText() );
				MenuShowArray.Add ( bDisplay );
				MenuEnableArray.Add ( bEnable );
			}
			break;

		case MENU_ITEM_ENDMENU:
			if ( nMenuIndex != 0 )
			{
				CMenu* pMenu = ( CMenu* ) MenuPtrArray.GetAt ( nMenuIndex );
				CMenu* pParent = ( CMenu* ) MenuPtrArray.GetAt ( nMenuIndex - 1 );
				CString strText = MenuNameArray.GetAt ( nMenuIndex - 1 );
				bool bDisplay = MenuShowArray.GetAt ( nMenuIndex - 1 ) != 0;
				bool bEnable = MenuEnableArray.GetAt ( nMenuIndex - 1 ) != 0;
				
				MenuPtrArray.RemoveAt ( nMenuIndex );
				MenuNameArray.RemoveAt ( nMenuIndex - 1 );
				MenuShowArray.RemoveAt ( nMenuIndex - 1 );
				MenuEnableArray.RemoveAt ( nMenuIndex - 1 );

				if ( bDisplay == TRUE )
				{
					TidySeparators ( pMenu );

					if ( bEnable == TRUE )
						pParent -> AppendMenu ( MF_POPUP | MF_ENABLED, ( UINT ) pMenu -> Detach(), strText );
					else
						pParent -> AppendMenu ( MF_POPUP | MF_GRAYED, ( UINT ) pMenu -> Detach(), strText );
				}
				delete pMenu;
			}
			else if ( TRUE == m_bPopup )
				TidySeparators ( pMenu );

			break;
		}
	}
}

/**********************************************************************/

void CMenuChanger::TidySeparators ( CMenu* pMenu )
{
	int nCount = pMenu -> GetMenuItemCount();

	for ( int nPos = nCount - 1; nPos >= 0; nPos-- )
	{
		if ( pMenu -> GetMenuItemID ( nPos ) == 0 )
		{
			bool bDelete = FALSE;

			int nNewCount = pMenu -> GetMenuItemCount();
					
			if ( nPos == 0 || nPos == nNewCount- 1 )
				bDelete = TRUE;
			else
			{
				if ( pMenu -> GetMenuItemID ( nPos - 1 ) == 0 || pMenu -> GetMenuItemID ( nPos + 1 ) == 0 )
					bDelete = TRUE;
			}

			if ( bDelete == TRUE )
				pMenu -> DeleteMenu ( nPos, MF_BYPOSITION );
		}
	}
}

/**********************************************************************/

