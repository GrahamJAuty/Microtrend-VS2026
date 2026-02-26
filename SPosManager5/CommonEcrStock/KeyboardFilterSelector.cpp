/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//#include "DataManager.h"
//#include "EcrmanOptionsIni.h"
#include "KeyboardSetCSVArray.h"
#include "..\CommonEcrStock\LocationSelector.h"
#include "NodeTypes.h"
/**********************************************************************/
#include "KeyboardFilterSelector.h"
/**********************************************************************/

CKeyboardFilterSelector::CKeyboardFilterSelector(void)
{
	m_pComboLocation = NULL;
	m_pComboKeyboard = NULL;
	m_ImageList.Create ( IDB_ICONS_COMBO, 31, 0, COLORREF(0xFFFFFF) );
	m_nLocComboCurrentSelection = 0;
}

/**********************************************************************/

void CKeyboardFilterSelector::FillCombos ( CSSComboBoxEx* pComboLocation, CComboBox* pComboKeyboard )
{
	m_pComboLocation = pComboLocation;
	m_pComboKeyboard = pComboKeyboard;

	m_pComboLocation -> ResetContent();
	m_pComboLocation -> SetImageList ( &m_ImageList );

	m_arrayKbdLocs.RemoveAll();

	AddKeyboardLocation( NODE_SYSTEM, 0 );

	if ( EcrmanOptions.GetFeaturesKeyboardFlag() == TRUE )
	{
		CArray<int,int> arrayLocIdx;
		dbLocation.GetDatabaseLocIdxSet( DataManager.GetActiveDbIdx(), arrayLocIdx );

		if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
			dbLocation.SortLocListByName( arrayLocIdx );

		for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
		{
			int nLocIdx = arrayLocIdx.GetAt(n);
			if ( dbLocation.GetDbKeyboardSet(nLocIdx) == 0 )
				AddKeyboardLocation( NODE_LOCATION, nLocIdx ); 
		}

		if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
		{
			CWordArray arrayKbsIdx;
			dbKeyboardSet.SortByName( arrayKbsIdx, DataManager.GetActiveDbIdx() );

			for ( int n = 0; n < arrayKbsIdx.GetSize(); n++ )
			{
				int nKbsIdx = arrayKbsIdx.GetAt(n);
				AddKeyboardLocation( NODE_DBKEYBOARDSET, nKbsIdx );
			}
		}
		else
		{
			int nStartIdx, nEndIdx;
			dbKeyboardSet.GetKbsIdxRange( DataManager.GetActiveDbIdx(), nStartIdx, nEndIdx );

			for ( int nKbsIdx = nStartIdx; nKbsIdx <= nEndIdx; nKbsIdx++ )
				AddKeyboardLocation( NODE_DBKEYBOARDSET, nKbsIdx );
		}
	}

	m_pComboLocation -> SetCurSel(0);
	m_pComboLocation -> EnableWindow( m_pComboLocation -> GetCount() > 1 );
	m_pComboKeyboard -> EnableWindow( FALSE );
	m_nLocComboCurrentSelection = 0;
}

/**********************************************************************/

void CKeyboardFilterSelector::AddKeyboardLocation( int nNodeType, int nEntityIdx )
{
	CKeyboardFilterSelectorLoc info;
	info.m_nNodeType = nNodeType;
	info.m_nEntityIdx = nEntityIdx;
	info.m_strKeyboards = "";
	
	switch( info.m_nNodeType )
	{
	case NODE_LOCATION:
	case NODE_DBKEYBOARDSET:
		break;

	case NODE_SYSTEM:
		info.m_nEntityIdx = 0;
		break;

	default:
		return;
	}

	CCSV csv;
	if ( info.m_nNodeType != NODE_SYSTEM )
	{
		for ( int k = 1; k <= EcrmanOptions.GetFeaturesKeyboardsPerLocation(); k++ )
		{
			CString strPath = "";

			if ( NODE_LOCATION == info.m_nNodeType )
				strPath += dbLocation.GetFolderPathDataKeyboard( info.m_nEntityIdx, k );
			else
				strPath += dbKeyboardSet.GetFolderPathKeyboard( info.m_nEntityIdx, k );
			
			strPath += "\\PluNum.dat";
			
			if ( ::FileExists( strPath ) == TRUE )
				csv.Add(k);
		}
	}

	if ( ( csv.GetSize() != 0 ) || ( NODE_SYSTEM == info.m_nNodeType ) )
	{
		info.m_strKeyboards = csv.GetLine();
		m_arrayKbdLocs.Add( info );

		COMBOBOXEXITEM item;
		item.mask = CBEIF_DI_SETITEM | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;	
		item.iItem = m_pComboLocation -> GetCount();
		
		switch( info.m_nNodeType )
		{
		case NODE_LOCATION:
			item.pszText = (char*) dbLocation.GetName( info.m_nEntityIdx );
			item.iImage = NODE_COMBOICON_LOCATION;
			break;

		case NODE_DBKEYBOARDSET:
			item.pszText = (char*) dbKeyboardSet.GetName( info.m_nEntityIdx );
			item.iImage = NODE_COMBOICON_LOCSET;
			break;
		
		case NODE_SYSTEM:
		default:
			item.pszText = (char*) "System";
			item.iImage = NODE_COMBOICON_NONE;
			break;
		}

		item.iSelectedImage = item.iImage;
		m_pComboLocation -> InsertItem ( &item );
	}
}

/**********************************************************************/

void CKeyboardFilterSelector::SelectLocation()
{
	int nSel = m_pComboLocation -> GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_arrayKbdLocs.GetSize() ) && ( nSel != m_nLocComboCurrentSelection ) )
	{
		int nOldKbNum = 0;
		{
			int nKbNumSel = m_pComboKeyboard -> GetCurSel();
			if ( ( nKbNumSel >= 0 ) && ( nKbNumSel <= m_pComboKeyboard -> GetCount() ) )
				nOldKbNum = m_pComboKeyboard -> GetItemData( nKbNumSel );
		}

		switch( m_arrayKbdLocs[nSel].m_nNodeType )
		{
		case NODE_LOCATION:
		case NODE_DBKEYBOARDSET:
			{
				m_pComboKeyboard -> ResetContent();

				CCSV csv( m_arrayKbdLocs[nSel].m_strKeyboards );
				for ( int n = 0; n < csv.GetSize(); n++ )
				{
					CString str;
					int nKbNo = csv.GetInt(n);
					str.Format( "Keyboard %d", nKbNo );
					m_pComboKeyboard -> AddString( str );
					m_pComboKeyboard -> SetItemData( n, nKbNo );
				}

				m_pComboKeyboard -> SetCurSel(0);
				for ( int n = 0; n < m_pComboKeyboard -> GetCount(); n++ )
				{
					if ( m_pComboKeyboard -> GetItemData(n) == nOldKbNum )
					{
						m_pComboKeyboard -> SetCurSel(n);
						break;
					}
				}

				m_pComboKeyboard -> EnableWindow( TRUE );
			}
			break;
		
		case NODE_SYSTEM:
		default:
			m_pComboKeyboard -> ResetContent();
			m_pComboKeyboard -> EnableWindow( FALSE );
			break;
		}

		m_nLocComboCurrentSelection = nSel;
	}
}

/**********************************************************************/

void CKeyboardFilterSelector::GetSelection( CKeyboardFilterSelectorLoc& Loc, int& nKbNo )
{
	Loc.m_nNodeType = NODE_SYSTEM;
	Loc.m_nEntityIdx = 0;
	Loc.m_strKeyboards = "";
	nKbNo = 1;

	if ( ( m_nLocComboCurrentSelection > 0 ) && ( m_nLocComboCurrentSelection < m_arrayKbdLocs.GetSize() ) )
	{
		Loc.m_nNodeType = m_arrayKbdLocs[m_nLocComboCurrentSelection].m_nNodeType;
		Loc.m_nEntityIdx = m_arrayKbdLocs[m_nLocComboCurrentSelection].m_nEntityIdx;

		int nSel = m_pComboKeyboard -> GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_pComboKeyboard -> GetCount() ) )
			nKbNo = m_pComboKeyboard -> GetItemData( nSel );
	}
}

/**********************************************************************/

void CKeyboardFilterSelector::SetSelection( CKeyboardFilterSelectorLoc& Loc, int nKbNo )
{
	if ( m_arrayKbdLocs.GetSize() == m_pComboLocation -> GetCount() )
	{
		for ( int n = 0; n < m_arrayKbdLocs.GetSize(); n++ )
		{
			if ( ( m_arrayKbdLocs[n].m_nNodeType == Loc.m_nNodeType ) && ( m_arrayKbdLocs[n].m_nEntityIdx == Loc.m_nEntityIdx ) )
			{
				m_pComboLocation -> SetCurSel(n);
				SelectLocation();

				m_pComboKeyboard -> SetCurSel(0);
				for ( int x = 0; x < m_pComboKeyboard -> GetCount(); x++ )
				{
					if ( nKbNo == m_pComboKeyboard -> GetItemData(x) )
					{
						m_pComboKeyboard -> SetCurSel(x);
						break;
					}
				}

				break;
			}
		}
	}
}

/**********************************************************************/
