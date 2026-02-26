/**********************************************************************/

CMenuChangerCSVRecord::CMenuChangerCSVRecord()
{
	m_nItemDepth = 0;
	m_nItemType = MENU_ITEM_OPTION;
	m_nItemId = 0;
	m_strItemText = "";
	m_bEnable = FALSE;
	m_bDisplay = FALSE;
	m_bKill = FALSE;
}

/**********************************************************************/

void CMenuChangerCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	m_nItemDepth = csv.GetInt(0);
	m_nItemType = csv.GetInt(1);
	m_nItemId = csv.GetInt(2);
	m_strItemText = csv.GetString(3);
	m_bEnable = csv.GetBool(4);
	m_bDisplay = csv.GetBool(5);
	m_bKill = csv.GetBool(6);
}

/**********************************************************************/

void CMenuChangerCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nItemDepth );
	csv.Add ( m_nItemType );
	csv.Add ( m_nItemId );
	csv.Add ( m_strItemText );
	csv.Add ( m_bEnable );
	csv.Add ( m_bDisplay );
	csv.Add ( m_bKill );
}

/**********************************************************************/
				
void CMenuChangerCSVArray::AddOption ( int nId, const char* szText, int nItemDepth )
{
	CMenuChangerCSVRecord NewRecord;
	NewRecord.SetItemDepth ( nItemDepth );
	NewRecord.SetItemType ( MENU_ITEM_OPTION );
	NewRecord.SetItemId ( nId );
	NewRecord.SetItemText ( szText );
	NewRecord.SetDisplayFlag ( TRUE );
	NewRecord.SetEnableFlag ( TRUE );
	NewRecord.SetKillFlag ( FALSE );
	Add ( NewRecord );
}

/**********************************************************************/

void CMenuChangerCSVArray::AddSeparator( int nItemDepth)
{
	CMenuChangerCSVRecord NewRecord;
	NewRecord.SetItemDepth ( nItemDepth );
	NewRecord.SetItemType ( MENU_ITEM_SEPARATOR );
	NewRecord.SetItemId ( 0 );
	NewRecord.SetItemText ( "" );
	NewRecord.SetDisplayFlag ( TRUE );
	NewRecord.SetEnableFlag ( FALSE );
	NewRecord.SetKillFlag ( FALSE );
	Add ( NewRecord );
}

/**********************************************************************/

void CMenuChangerCSVArray::AddSubMenu ( const char* szText, int nItemDepth )
{
	CMenuChangerCSVRecord NewRecord;
	NewRecord.SetItemDepth ( nItemDepth );
	NewRecord.SetItemType ( MENU_ITEM_SUBMENU );
	NewRecord.SetItemId ( 0 );
	NewRecord.SetItemText ( szText );
	NewRecord.SetDisplayFlag ( TRUE );
	NewRecord.SetEnableFlag ( TRUE );
	NewRecord.SetKillFlag ( FALSE );
	Add ( NewRecord );
}

/**********************************************************************/

void CMenuChangerCSVArray::AddEndMenu( int nItemDepth )
{
	CMenuChangerCSVRecord NewRecord;
	NewRecord.SetItemDepth ( nItemDepth );
	NewRecord.SetItemType ( MENU_ITEM_ENDMENU );
	NewRecord.SetItemId ( 0 );
	NewRecord.SetItemText ( "" );
	NewRecord.SetDisplayFlag ( FALSE );
	NewRecord.SetEnableFlag ( FALSE );
	NewRecord.SetKillFlag ( FALSE );
	Add ( NewRecord );
}

/**********************************************************************/

void CMenuChangerCSVArray::EnableItem ( int nId, bool bEnable )
{
	for ( int nIndex = 0; nIndex < GetSize(); nIndex++ )
	{
		CMenuChangerCSVRecord Record;
		GetAt ( nIndex, Record );

		if ( Record.GetItemId() == nId )
		{
			Record.SetEnableFlag ( bEnable );
			SetAt ( nIndex, Record );
			break;
		}
	}
}

/**********************************************************************/

void CMenuChangerCSVArray::EnableAll( bool bEnable )
{
	for ( int nIndex = 0; nIndex < GetSize(); nIndex++ )
	{
		CMenuChangerCSVRecord Record;
		GetAt ( nIndex, Record );
		
		switch( Record.GetItemType() )
		{
		case MENU_ITEM_OPTION:
		case MENU_ITEM_SUBMENU:
			Record.SetEnableFlag ( bEnable );
			SetAt ( nIndex, Record );
			break;
		}
	}
}

/**********************************************************************/

void CMenuChangerCSVArray::KillItem ( int nId )
{
	for ( int nIndex = 0; nIndex < GetSize(); nIndex++ )
	{
		CMenuChangerCSVRecord Record;
		GetAt ( nIndex, Record );

		if ( Record.GetItemId() == nId )
		{
			Record.SetKillFlag ( TRUE );
			SetAt ( nIndex, Record );
			break;
		}
	}
}

/**********************************************************************/

void CMenuChangerCSVArray::ShowItem ( int nId, bool bShow )
{
	for ( int nIndex = 0; nIndex < GetSize(); nIndex++ )
	{
		CMenuChangerCSVRecord Record;
		GetAt ( nIndex, Record );

		if ( Record.GetItemId() == nId )
		{
			Record.SetDisplayFlag ( bShow );
			SetAt ( nIndex, Record );
			break;
		}
	}
}

/**********************************************************************/

void CMenuChangerCSVArray::SetItemText ( int nId, const char* szText )
{
	for ( int nIndex = 0; nIndex < GetSize(); nIndex++ )
	{
		CMenuChangerCSVRecord Record;
		GetAt ( nIndex, Record );

		if ( Record.GetItemId() == nId )
		{
			Record.SetItemText ( szText );
			SetAt ( nIndex, Record );
			break;
		}
	}
}

/**********************************************************************/

int CMenuChangerCSVArray::FindParentItem(int nId, int nDepth)
{
	int nResult = -1;

	CArray<int,int> ParentArray;
	ParentArray.Add(0);
	int nIndent = 0;

	for (int nIndex = 0; nIndex < GetSize(); nIndex++)
	{
		CMenuChangerCSVRecord Record;
		GetAt(nIndex, Record);

		if (Record.GetItemType() == MENU_ITEM_SUBMENU)
		{
			ParentArray.Add(nIndex);
			nIndent++;
			continue;
		}

		if (Record.GetItemType() == MENU_ITEM_ENDMENU)
		{
			ParentArray.RemoveAt(ParentArray.GetSize() - 1);
			nIndent--;
			continue;
		}

		if (Record.GetItemId() == nId)
		{
			if (nIndent >= nDepth)
			{
				nResult = ParentArray.GetAt((nIndent + 1) - nDepth);
				break;
			}
		}
	}

	return nResult;
}

/**********************************************************************/

void CMenuChangerCSVArray::SetParentStatus(int nId, int nType, bool bFlag, int nDepth)
{
	if (nDepth < 1)
	{
		return;
	}

	int nParentIdx = FindParentItem(nId, nDepth);

	if (nParentIdx != -1)
	{
		CMenuChangerCSVRecord Record;
		GetAt(nParentIdx, Record);

		switch (nType)
		{
		case MENU_FLAG_SHOW:
			Record.SetDisplayFlag(bFlag);
			break;

		case MENU_FLAG_ENABLE:
			Record.SetEnableFlag(bFlag);
			break;

		case MENU_FLAG_KILL:
			Record.SetKillFlag(bFlag);
			break;
		}

		SetAt(nParentIdx, Record);
	}
}

/**********************************************************************/

void CMenuChangerCSVArray::SetParentText(int nId, CString strText, int nDepth)
{
	if (nDepth < 1)
	{
		return;
	}

	int nParentIdx = FindParentItem(nId, nDepth);

	if (nParentIdx != -1)
	{
		CMenuChangerCSVRecord Record;
		GetAt(nParentIdx, Record);
		Record.SetItemText(strText);
		SetAt(nParentIdx, Record);
	}
}

/**********************************************************************/

bool CMenuChangerCSVArray::InsertExtraMenu ( UINT nFirstNewID, const char* szMenuStructure, UINT& nNextFreeID )
{
	nNextFreeID = nFirstNewID;

	CString strMenuStructure = szMenuStructure;
	CCSV csvMenuStructure ( strMenuStructure );
	
	//FIRST ITEM IN CSV LINE IS INTERNAL REF FOR EXTRA MENU
	int nItemCount = csvMenuStructure.GetInt(1);
	int nOffset = 2;
	
	//VALIDATION PASS FOR MENU STRUCTURE
	if ( nItemCount == 0 )
		return FALSE;

	int nSubMenuDepth = 0;

	for ( int nExtraItem = 0; nExtraItem < nItemCount; nExtraItem++ )	
	{
		CString strType = csvMenuStructure.GetString ( nOffset++ );

		if ( strType == "SUBMENU" )
		{
			nSubMenuDepth++;
			nOffset++;
		}
		else if ( strType == "ENDMENU" )
		{
			nSubMenuDepth--;

			if ( nSubMenuDepth < 0 )
				return FALSE;
		}
		else if ( strType == "OPTION" ) 
			nOffset++;
		else if ( strType != "SEPARATOR" )
			return FALSE;
	}

	if ( nSubMenuDepth != 0 )
		return FALSE;

	CMenuChangerCSVRecord MenuRecord;
	
	//FIND INSERTION POSITION BEFORE LAST TOP MENU
	int nInsertionIndex = GetSize();

	for ( int nTestIndex = GetSize() - 1; nTestIndex >= 0; nTestIndex-- )
	{
		GetAt ( nTestIndex, MenuRecord );
		
		if ( ( MenuRecord.GetItemType() == MENU_ITEM_SUBMENU ) && ( MenuRecord.GetItemDepth() == 0 ) )
		{
			nInsertionIndex = nTestIndex;
			break;
		}
	}

	nOffset = 2;
	nSubMenuDepth = 0;

	for ( int nExtraItem = 0; nExtraItem < nItemCount; nExtraItem++ )	
	{
		CString strType = csvMenuStructure.GetString ( nOffset++ );

		if ( strType == "OPTION" )
		{
			CMenuChangerCSVRecord NewRecord;
			NewRecord.SetItemDepth ( nSubMenuDepth );
			NewRecord.SetItemType ( MENU_ITEM_OPTION );
			NewRecord.SetItemId ( nNextFreeID++ );
			NewRecord.SetItemText ( csvMenuStructure.GetString ( nOffset++ ) );
			NewRecord.SetDisplayFlag ( TRUE );
			NewRecord.SetEnableFlag ( TRUE );
			NewRecord.SetKillFlag ( FALSE );
			InsertAt ( nInsertionIndex++, NewRecord );
		}
		else if ( strType == "SEPARATOR" )
		{
			CMenuChangerCSVRecord NewRecord;
			NewRecord.SetItemDepth ( nSubMenuDepth );
			NewRecord.SetItemType ( MENU_ITEM_SEPARATOR );
			NewRecord.SetItemId ( 0 );
			NewRecord.SetItemText ( "" );
			NewRecord.SetDisplayFlag ( TRUE );
			NewRecord.SetEnableFlag ( FALSE );
			NewRecord.SetKillFlag ( FALSE );
			InsertAt ( nInsertionIndex++, NewRecord );
		}
		else if ( strType == "SUBMENU" )
		{
			CMenuChangerCSVRecord NewRecord;
			NewRecord.SetItemDepth ( nSubMenuDepth++ );
			NewRecord.SetItemType ( MENU_ITEM_SUBMENU );
			NewRecord.SetItemId ( 0 );
			NewRecord.SetItemText ( csvMenuStructure.GetString ( nOffset++ ) );
			NewRecord.SetDisplayFlag ( TRUE );
			NewRecord.SetEnableFlag ( TRUE );
			NewRecord.SetKillFlag ( FALSE );
			InsertAt ( nInsertionIndex++, NewRecord );
		}
		else if ( strType == "ENDMENU" )
		{
			CMenuChangerCSVRecord NewRecord;
			NewRecord.SetItemDepth ( nSubMenuDepth-- );
			NewRecord.SetItemType ( MENU_ITEM_ENDMENU );
			NewRecord.SetItemId ( 0 );
			NewRecord.SetItemText ( "" );
			NewRecord.SetDisplayFlag ( FALSE );
			NewRecord.SetEnableFlag ( FALSE );
			NewRecord.SetKillFlag ( FALSE );
			InsertAt ( nInsertionIndex++, NewRecord );
		}
		else
			break;
	}
	return TRUE;
}

/**********************************************************************/

