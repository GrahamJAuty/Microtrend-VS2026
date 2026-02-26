/**********************************************************************/
 
/**********************************************************************/
#include "..\CommonEcrStock\DefEditDlg.h"
//#include "EcrmanOptionsIni.h"
#include "SPOSVersions.h"
#include "KeyboardSetCSVArray.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
 
#include "SelectMultipleLocationDlg.h"
/**********************************************************************/
#include "PropPageSetupSystem.h"
/**********************************************************************/

void CPropPageSetupSystem::BuildKbSetTree()
{
	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
		if ( dbLocation.IsWebSalesLocation( nLocIdx ) == TRUE )
			dbLocation.SetDbKeyboardSet( nLocIdx, 0 );

	HTREEITEM hRoot = AddRoot();

	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
			BuildKbSetTreeDatabase( hRoot, nDbIdx );
	}
	else
	{
		CWordArray arrayDbIdx;
		dbDatabase.SortByName( arrayDbIdx );

		for ( int n = 0; n < arrayDbIdx.GetSize(); n++ )
		{
			int nDbIdx = arrayDbIdx.GetAt(n);
			BuildKbSetTreeDatabase( hRoot, nDbIdx );
		}
	}

	m_buttonAction4.ShowWindow( SW_HIDE );
	CollapseKbSetTree();
	m_treeSystem.SelectItem ( m_treeSystem.GetRootItem() );
}

/**********************************************************************/

void CPropPageSetupSystem::BuildKbSetTreeDatabase( HTREEITEM hRoot, int nDbIdx )
{
	CString strName = dbDatabase.GetName( nDbIdx );

	HTREEITEM hDbNode = AddDatabaseNode ( hRoot, nDbIdx );

	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		int nStartIdx, nEndIdx;
		dbKeyboardSet.GetKbsIdxRange( nDbIdx, nStartIdx, nEndIdx );

		for ( int nKbsIdx = nStartIdx; nKbsIdx <= nEndIdx; nKbsIdx++ )
			BuildKbSetTreeKbSet( hDbNode, nDbIdx, nKbsIdx );
	}
	else
	{
		CWordArray arrayKbsIdx;
		dbKeyboardSet.SortByName( arrayKbsIdx, nDbIdx );

		for ( int n = 0; n < arrayKbsIdx.GetSize(); n++ )
		{
			int nKbsIdx = arrayKbsIdx.GetAt(n);
			BuildKbSetTreeKbSet( hDbNode, nDbIdx, nKbsIdx );
		}
	}	
}

/**********************************************************************/

void CPropPageSetupSystem::BuildKbSetTreeKbSet( HTREEITEM hDbNode, int nDbIdx, int nKbsIdx )
{		
	int nDbNo = dbDatabase.GetDbNo( nDbIdx );
	int nKbsNo = dbKeyboardSet.GetKbsNo( nKbsIdx );
	CString strName = dbKeyboardSet.GetName( nKbsIdx );
	HTREEITEM hKbsNode = AddKbSetNode ( hDbNode, nKbsIdx );
	AddKbSetLocationNodes ( hKbsNode, nKbsNo, nDbNo );	
}

/**********************************************************************/

void CPropPageSetupSystem::AddKbSetLocationNodes ( HTREEITEM hKbsNode, int nKbsNo, int nDbNo )
{
	HTREEITEM hChild;
	while ( ( hChild = m_treeSystem.GetChildItem ( hKbsNode ) ) != NULL )
		DeleteSubTreeNodes ( hChild );
	
	int nDbIdx;
	if ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) == FALSE )
		return;

	CArray<int,int> Set;
	dbLocation.GetDatabaseLocIdxSet ( nDbIdx, Set );

	if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
		dbLocation.SortLocListByName( Set );

	int nBaseVersionSet = -1;
	for ( int nIndex = 0; nIndex < Set.GetSize(); nIndex++ )
	{
		int nLocIdx = Set.GetAt ( nIndex );
		if ( dbLocation.GetDbKeyboardSet (nLocIdx) == nKbsNo )
		{
			int nNetWkIdx;
			if ( dbNetwork.FindNetworkByNumber( dbLocation.GetNetworkNo(nLocIdx), nNetWkIdx ) == TRUE )
			{
				int nBaseVersionLoc = CSPOSVersions::GetSPOSBaseVersion( dbNetwork.GetSPOSVersion( nNetWkIdx ) );
				
				if ( -1 == nBaseVersionSet )
					nBaseVersionSet = nBaseVersionLoc;

				if ( ( nBaseVersionSet == nBaseVersionLoc ) && ( nBaseVersionSet != -1 ) )
					AddLocationNode( hKbsNode, nLocIdx, SETUP_NODE_LOC_KBSET, TRUE );
				else
					dbLocation.SetDbKeyboardSet( nLocIdx, 0 );
			}
		}
	}
}

/**********************************************************************/

void CPropPageSetupSystem::SelectItemKbSetTree ( int nNodeType )
{
	switch ( nNodeType )
	{
	case SETUP_NODE_SYSTEM:
		SetButtonTexts ( "Add", "Edit", "Delete", "" );
		EnableActionButtons ( FALSE, FALSE, FALSE, FALSE );
		break;

	case SETUP_NODE_DBASE:
		SetButtonTexts ( "Add\nKb.Set", "Edit", "Delete", "" );
		EnableActionButtons ( TRUE, FALSE, FALSE, FALSE );
		break;

	case SETUP_NODE_KBSET:
		SetButtonTexts ( "Link\nLocations", "Edit\nKb.Set", "Delete\nKb.Set", "" );
		EnableActionButtons ( TRUE, TRUE, TRUE, FALSE );	
		break;

	case SETUP_NODE_LOC_KBSET:
		SetButtonTexts ( "Add", "Edit", "Remove\nLink", "" );
		EnableActionButtons ( FALSE, FALSE, TRUE, FALSE );	
		break;
	}
}

/**********************************************************************/

void CPropPageSetupSystem::Button1KbSetTree()
{
	switch ( GetNodeType( m_hItem) )
	{
	case SETUP_NODE_DBASE:			
		AddKbSet();				
		break;

	case SETUP_NODE_KBSET:
		AddLocationsToKbSet();	
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button2KbSetTree()
{
	switch ( GetNodeType( m_hItem) )
	{
	case SETUP_NODE_KBSET:		
		EditKbSet();		
		break;
	}
	
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button3KbSetTree()
{
	switch ( GetNodeType( m_hItem) )
	{
	case SETUP_NODE_KBSET:			
		DeleteKbSet();				
		break;

	case SETUP_NODE_LOC_KBSET:		
		RemoveLocationsFromKbSet();	
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::AddKbSet()
{
	int nKbsIdx = dbKeyboardSet.AddKeyboardSet ( m_nDbNo );
	if ( nKbsIdx == -1 )
	{
		Prompter.Error ( "You have already set up the\nmaximum number of keyboard sets" ); 
		return;
	}

	m_treeSystem.SelectItem ( AddKbSetNode ( m_hItem, nKbsIdx ) );
}

/**********************************************************************/

void CPropPageSetupSystem::EditKbSet()
{
	int nKbsIdx;
	if ( dbKeyboardSet.FindKeyboardSetByNumber ( m_nDbNo, m_nKbsNo, nKbsIdx ) == FALSE )
		return;
	
	CChangeKeyboardSetNameDlg dlg ( nKbsIdx, this );

	if ( dlg.DoModal() == IDOK )
	{
		dbKeyboardSet.SetName ( nKbsIdx, dlg.m_strName );
		CString strName = dbKeyboardSet.GetName( nKbsIdx );
		m_treeSystem.SetItemText ( m_hItem, strName );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::DeleteKbSet()
{
	int nKbsIdx; 
	if ( dbKeyboardSet.FindKeyboardSetByNumber ( m_nDbNo, m_nKbsNo, nKbsIdx ) == FALSE )
		return;

	if ( Prompter.YesNo ( "Are you sure you wish to delete this keyboard set" ) == IDNO )
		return;

	if ( dbKeyboardSet.RenameForDeletion( nKbsIdx ) == FALSE )
		Prompter.Error( "Unable to delete keyboard set" );
	else
		DeleteSubTreeNodesAndData ( m_hItem );
}

/**********************************************************************/

void CPropPageSetupSystem::AddLocationsToKbSet()
{
	CArray<int,int> arrayLocIdx;

	int nDbIdx;
	if ( dbDatabase.FindDatabaseByNumber ( m_nDbNo, nDbIdx ) )
	{
		CArray<int,int> Set;
		dbLocation.GetDatabaseLocIdxSet ( nDbIdx, Set );

		int nBaseSPOSVersion = -1;
		for ( int nIndex = 0; ( nIndex < Set.GetSize() ) && ( -1 == nBaseSPOSVersion ); nIndex++ )
		{
			int nLocIdx = Set.GetAt ( nIndex );
			if ( dbLocation.GetDbKeyboardSet( nLocIdx ) == m_nKbsNo )
			{
				int nNetWkIdx;
				if ( dbNetwork.FindNetworkByNumber( dbLocation.GetNetworkNo(nLocIdx), nNetWkIdx ) == TRUE )
					nBaseSPOSVersion = CSPOSVersions::GetSPOSBaseVersion( dbNetwork.GetSPOSVersion( nNetWkIdx ) );
			}
		}

		for ( int nIndex = 0; nIndex < Set.GetSize(); nIndex++ )
		{
			int nLocIdx = Set.GetAt ( nIndex );
			if ( ( dbLocation.GetDbKeyboardSet( nLocIdx ) == 0 ) && ( dbLocation.IsWebSalesLocation( nLocIdx ) == FALSE ) )
			{
				int nNetWkIdx;
				if ( dbNetwork.FindNetworkByNumber( dbLocation.GetNetworkNo(nLocIdx), nNetWkIdx ) == TRUE )
				{
					int nLocBaseSPOSVersion = CSPOSVersions::GetSPOSBaseVersion( dbNetwork.GetSPOSVersion( nNetWkIdx ) );

					if ( ( nLocBaseSPOSVersion == nBaseSPOSVersion ) || ( -1 == nBaseSPOSVersion ) )
						arrayLocIdx.Add( nLocIdx );
				}
			}
		}
	}

	if ( arrayLocIdx.GetSize() == 0 )
	{
		Prompter.Error ( "There are no more locations available to link." );
		return;
	}

	if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
		dbLocation.SortLocListByName( arrayLocIdx );

	CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo> arrayLocInfo;
	CSelectMultipleLocationDlg::CreateLocInfoArray( arrayLocIdx, arrayLocInfo );
	CSelectMultipleLocationDlg dlg( "Add Locations To Keyboard Set", arrayLocInfo, TRUE, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		for ( int nIndex = 0; nIndex < arrayLocInfo.GetSize(); nIndex++ )
			if ( TRUE == arrayLocInfo[nIndex].m_bSelected )
				dbLocation.SetDbKeyboardSet ( arrayLocInfo[nIndex].m_nLocIdx, m_nKbsNo );
		
		AddKbSetLocationNodes ( m_hItem, m_nKbsNo, m_nDbNo );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::RemoveLocationsFromKbSet()
{
	HTREEITEM hItem = m_treeSystem.GetSelectedItem();
	RemoveLinks( hItem, SETUP_NODE_LOC_KBSET ); 
}

/**********************************************************************/

void CPropPageSetupSystem::CollapseKbSetTree()
{
	CollapseKbSetTree ( m_treeSystem.GetRootItem() );
}

/**********************************************************************/

void CPropPageSetupSystem::CollapseKbSetTree( HTREEITEM hItem )
{
	if ( hItem == NULL )
		return;

	int nActionType;
	switch ( GetNodeType( hItem ) )
	{
	case SETUP_NODE_SYSTEM:
	case SETUP_NODE_DBASE:			
		nActionType = TVE_EXPAND;	
		break;

	default:						
		nActionType = TVE_COLLAPSE;	
		break;
	}

	m_treeSystem.Expand ( hItem, nActionType );

	HTREEITEM hChild = m_treeSystem.GetChildItem( hItem );
	while ( hChild != NULL )
	{
		CollapseKbSetTree ( hChild );
		hChild = m_treeSystem.GetNextSiblingItem ( hChild );
	}
}

/**********************************************************************/

