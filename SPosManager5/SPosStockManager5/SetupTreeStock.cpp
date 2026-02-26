/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "..\CommonEcrStock\DefEditDlg.h"
#include "EposSelectArray.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "PropertySheetStockpoint.h"
#include "..\CommonEcrStock\SelectMultipleTerminalDlg.h"
#include "SetupWarehouseDlg.h"
#include "SiteEditDlg.h"
#include "StockpointCSVArray.h"
#include "TreeNode.h"
/**********************************************************************/
#include "..\CommonEcrStock\PropPageSetupSystem.h"
/**********************************************************************/

void CPropPageSetupSystem::BuildStockTree()
{
	HTREEITEM hRoot = AddRoot();

	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
			BuildStockTreeDatabase( hRoot, nDbIdx );
	}
	else
	{
		CWordArray arrayDbIdx;
		dbDatabase.SortByName( arrayDbIdx );

		for ( int n = 0; n < arrayDbIdx.GetSize(); n++ )
		{
			int nDbIdx = arrayDbIdx.GetAt(n);
			BuildStockTreeDatabase( hRoot, nDbIdx );
		}
	}

	m_buttonAction4.ShowWindow ( SW_HIDE );
	CollapseStockTree();
	m_treeSystem.SelectItem ( m_treeSystem.GetRootItem() );
}

/**********************************************************************/

void CPropPageSetupSystem::BuildStockTreeDatabase( HTREEITEM hRoot, int nDbIdx )
{
	int nDbNo = dbDatabase.GetDbNo( nDbIdx );
	CString strName = dbDatabase.GetName( nDbIdx );

	HTREEITEM hDbNode = AddDatabaseNode ( hRoot, nDbIdx );
	
	/*
	if ( EcrmanOptions.GetSortLocNameFlag() == FALSE )
	{
		int nStartIdx, nEndIdx;
		dbStockpoint.GetSpIdxRange( nDbIdx, nStartIdx, nEndIdx );
		for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
			BuildStockTreeStockpoint( hDbNode, nDbIdx, nSpIdx );
	}
	else
	*/
	{
		CWordArray arraySpIdx;
		dbStockpoint.SortByName( arraySpIdx, nDbIdx );

		for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
		{
			int nSpIdx = arraySpIdx.GetAt(n);
			BuildStockTreeStockpoint( hDbNode, nDbIdx, nSpIdx );
		}
	}
}

/**********************************************************************/

HTREEITEM CPropPageSetupSystem::RebuildStockpointTree( int nDbIdx, HTREEITEM hItem, int nSpIdx )
{
	HTREEITEM hResult = NULL;

	HTREEITEM hChild;
	while ( ( hChild = m_treeSystem.GetChildItem ( hItem ) ) != NULL )
		DeleteSubTreeNodes ( hChild );

	/*
	if ( EcrmanOptions.GetSortLocNameFlag() == FALSE )
	{
		int nStartIdx, nEndIdx;
		dbStockpoint.GetSpIdxRange( nDbIdx, nStartIdx, nEndIdx );
		for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
			BuildStockTreeStockpoint( hItem, nDbIdx, nSpIdx );
	}
	else
	*/
	{
		CWordArray arraySpIdx;
		dbStockpoint.SortByName( arraySpIdx, nDbIdx );

		for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
		{
			int nSpIdxTree = arraySpIdx.GetAt(n);
			HTREEITEM hStockpoint = BuildStockTreeStockpoint( hItem, nDbIdx, nSpIdxTree );

			if ( nSpIdxTree == nSpIdx )
				hResult = hStockpoint;
		}
	}

	return hResult;
}

/**********************************************************************/

HTREEITEM CPropPageSetupSystem::BuildStockTreeStockpoint( HTREEITEM hDbNode, int nDbIdx, int nSpIdx )
{		
	int nDbNo = dbDatabase.GetDbNo( nDbIdx );
	int nSpNo = dbStockpoint.GetSpNo( nSpIdx );
	HTREEITEM hSpNode = AddStockpointNode ( hDbNode, nSpIdx, SETUP_NODE_STKPOINT );
	AddStockpointTerminalNodes ( hSpNode, nSpNo, nDbNo );
	return hSpNode;
}

/**********************************************************************/

void CPropPageSetupSystem::AddStockpointTerminalNodes ( HTREEITEM hSpNode, int nSpNo, int nDbNo )
{
	HTREEITEM hChild;
	while ( ( hChild = m_treeSystem.GetChildItem ( hSpNode ) ) != NULL )
		DeleteSubTreeNodes ( hChild );
	
	int nDbIdx;
	if ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) == FALSE )
		return;

	CArray<int,int> Set;
	dbLocation.GetDatabaseLocIdxSet( nDbIdx, Set );

	if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
		dbLocation.SortLocListByName( Set );

	for ( int nIndex = 0; nIndex < Set.GetSize(); nIndex++ )
	{
		int nLocIdx = Set.GetAt ( nIndex );

		int nLocNo = dbLocation.GetLocNo( nLocIdx );
		for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )
		{
			if ( dbLocation.GetSpNo ( nLocIdx, nTerminalIdx ) != nSpNo )
				continue;

			CString strName;
			strName.Format ( "%s, %s", dbLocation.GetName( nLocIdx ), dbLocation.GetTerminalName ( nLocIdx, nTerminalIdx ) );
			AddTerminalNode ( hSpNode, SETUP_NODE_TERM_STKPOINT, nLocIdx, nTerminalIdx );
		}
	}
}

/**********************************************************************/

void CPropPageSetupSystem::AddStockpointTerminalNodesLocation ( HTREEITEM hSpNode, int nSpNo, int nLocIdx )
{
	for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nTerminalIdx ); nTerminalIdx++ )
		if ( dbLocation.GetSpNo ( nLocIdx, nTerminalIdx ) == nSpNo )
			AddTerminalNode ( hSpNode, SETUP_NODE_TERM_STKPOINT, nLocIdx, nTerminalIdx );
}

/**********************************************************************/

void CPropPageSetupSystem::SelectItemStockTree ( int nNodeType )
{
	switch ( nNodeType )
	{
	case SETUP_NODE_SYSTEM:
		SetButtonTexts ( "Add", "Edit", "Delete", "" );
		EnableActionButtons ( FALSE, FALSE, FALSE, FALSE );
		break;

	case SETUP_NODE_DBASE:
		{
			CString strEdit = "Edit";
			bool bEnable = FALSE;

			if ( Sysset.GetMaxStockPoints() >= 2 )
			{
				strEdit = "";
				strEdit += "Configure\n";
				strEdit += DealerFlags.GetWarehouseType( TRUE );
				bEnable = TRUE;
			}

			SetButtonTexts ( "Add\nStockpoint", strEdit, "Delete", "" );
			EnableActionButtons ( TRUE, bEnable, FALSE, FALSE );
		}
		break;

	case SETUP_NODE_STKPOINT:
		SetButtonTexts ( "Link\nTerminal", "Edit\nStockpoint", "Delete\nStockpoint", "" );
		EnableActionButtons ( TRUE, TRUE, TRUE, FALSE );	
		break;

	case SETUP_NODE_TERM_STKPOINT:
		SetButtonTexts ( "Add", "Edit\nTerminal", "Remove\nLink", "" );
		EnableActionButtons ( FALSE, TRUE, TRUE, FALSE );	
		break;

	default:
		SetButtonTexts ( "Add", "Edit", "Delete", "" );
		EnableActionButtons ( FALSE, FALSE, FALSE, FALSE );
		break;
	}
}

/**********************************************************************/

void CPropPageSetupSystem::Button1StockTree()
{
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_DBASE:		
		AddStockpoint();			
		break;

	case SETUP_NODE_STKPOINT:	
		AddTerminalsToStockpoint();	
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button2StockTree()
{	
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_DBASE:			
		SetupWarehouse();		
		break;

	case SETUP_NODE_STKPOINT:		
		EditStockpoint();		
		break;

	case SETUP_NODE_TERM_STKPOINT:
		EditTerminal( TRUE );		
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button3StockTree()
{
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_STKPOINT:			
		DeleteStockpoint();					
		break;

	case SETUP_NODE_TERM_STKPOINT:		
		RemoveTerminalFromStockpoint();	
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::AddStockpoint()
{
	int nSpIdx = dbStockpoint.AddStockpoint ( m_nDbNo );
	if ( nSpIdx == -1 )
	{
		Prompter.Error ( "You have already set up the\nmaximum number of stockpoints" );
		return;
	}
			
	CString strStockPath = dbStockpoint.GetFolderPath( nSpIdx );
	
	if ( ::ExistSubdirectory ( strStockPath ) == FALSE )		
		::MakeSubdirectory ( strStockPath );
	
	int nSpNo = dbStockpoint.GetSpNo( nSpIdx );
	CString strName = dbStockpoint.GetName( nSpIdx );

	m_treeSystem.SelectItem ( AddStockpointNode ( m_hItem, nSpIdx, SETUP_NODE_STKPOINT ) );
}

/**********************************************************************/

void CPropPageSetupSystem::EditStockpoint()
{
	int nSpIdx;
	if ( dbStockpoint.FindStockpointByNumber ( m_nDbNo, m_nSpNo, nSpIdx ) == FALSE )
		return;

	int nDbIdx;
	if ( dbDatabase.FindDatabaseByNumber( m_nDbNo, nDbIdx ) == FALSE )
		return;

	CPropertySheetStockpoint propSheet( nSpIdx, FALSE, this );
	if ( propSheet.DoModal() == IDOK )
	{	
		propSheet.SaveRecord();
		m_treeSystem.SetItemText ( m_hItem, dbStockpoint.GetName( nSpIdx ) );
		HTREEITEM hItem = RebuildStockpointTree( nDbIdx, m_treeSystem.GetParentItem( m_hItem ), nSpIdx );
		
		if ( hItem != NULL )
			m_treeSystem.SelectItem( hItem );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::DeleteStockpoint()
{
	int nSpIdx;
	if ( dbStockpoint.FindStockpointByNumber ( m_nDbNo, m_nSpNo, nSpIdx ) == FALSE )
		return;
			
	CString strTitle;
	strTitle.Format ( "Delete %s", dbStockpoint.GetExtendedSystemName( nSpIdx ) );

	CStringArray Warning;
	Warning.Add ( strTitle );
	Warning.Add ( "The stockpoint is removed from the system." );
	Warning.Add ( "" );
	Warning.Add ( "All stock information for the stockpoint is lost." );
	
	if ( Prompter.Warning( Warning ) != IDYES )
		return;

	if ( dbStockpoint.RenameForDeletion( nSpIdx ) == FALSE )
		Prompter.Error( "Unable to delete stockpoint" );
	else
		DeleteSubTreeNodesAndData ( m_hItem );
}

/**********************************************************************/

void CPropPageSetupSystem::AddTerminalsToStockpoint()
{
	CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo> arrayNodeInfo;

	int nLastNetWkNo = 0;
	bool bAllowSpLink = TRUE;
	
	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
	{	
		int nNetWkNo = dbLocation.GetNetworkNo( nLocIdx );
		
		if ( nNetWkNo != nLastNetWkNo )
		{
			bAllowSpLink = TRUE;
			nLastNetWkNo = nNetWkNo;
			
			int nNetWkIdx;
			if ( dbNetwork.FindNetworkByNumber( nNetWkNo, nNetWkIdx ) == TRUE )
			{
				switch( dbNetwork.GetConnectionType( nNetWkIdx ) )
				{
				case CONNECTION_TYPE_SPTBOOK_NONE:
				case CONNECTION_TYPE_SPTBOOK_WEB:
				case CONNECTION_TYPE_SMARTENT_NONE:
				case CONNECTION_TYPE_SMARTENT_WEB:
					bAllowSpLink = FALSE;
					break;
				}
			}
		}
		
		if ( TRUE == bAllowSpLink )
		{
			if  ( dbLocation.GetDbNo( nLocIdx ) == m_nDbNo )
			{		
				for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )
				{
					if ( dbLocation.GetSpNo ( nLocIdx, nTerminalIdx ) == 0 )
					{
						CSelectMultipleEntityInfo infoNode;
						dbNetwork.FindNetworkByNumber( dbLocation.GetNetworkNo( nLocIdx ), infoNode.m_nNwkIdx );
						infoNode.m_nLocIdx = nLocIdx;
						infoNode.m_nTermIdx = nTerminalIdx;
						arrayNodeInfo.Add( infoNode );
					}
				}
			}
		}
	}

	if ( arrayNodeInfo.GetSize() == 0 )
	{
		Prompter.Error ( "There are no more terminals available to link." );
		return;
	}

	CSelectMultipleTerminalDlg dlg ( "Link Terminals To Stockpoint", arrayNodeInfo, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		for ( int nIndex = 0; nIndex < arrayNodeInfo.GetSize(); nIndex++ )
			if ( TRUE == arrayNodeInfo[nIndex].m_bSelected )
				dbLocation.SetSpNo ( arrayNodeInfo[nIndex].m_nLocIdx, arrayNodeInfo[nIndex].m_nTermIdx, m_nSpNo );
		
		AddStockpointTerminalNodes ( m_hItem, m_nSpNo, m_nDbNo );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::RemoveTerminalFromStockpoint()
{
	HTREEITEM hItem = m_treeSystem.GetSelectedItem();
	RemoveLinks( hItem, SETUP_NODE_TERM_STKPOINT );
}

/**********************************************************************/

void CPropPageSetupSystem::SetupWarehouse()
{
	HTREEITEM hItem = m_hItem;

	int nDbIdx;
	if ( dbDatabase.FindDatabaseByNumber( m_nDbNo, nDbIdx ) == FALSE )
		return;

	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange( nDbIdx, nStartIdx, nEndIdx );

	if ( nStartIdx >= nEndIdx )
	{
		CString str;
		str.Format( "You must link at least two stockpoints to this database\nbefore you can select a %s stockpoint.",
			DealerFlags.GetWarehouseType( FALSE ) );
		Prompter.Error( str );
		return;
	}

	CSetupWarehouseDlg dlg( nDbIdx, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		int nStartIdx, nEndIdx;
		dbStockpoint.GetSpIdxRange( nDbIdx, nStartIdx, nEndIdx );

		if ( dlg.m_nSpIdx != -1 )
		{
			for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
			{
				if ( nSpIdx == dlg.m_nSpIdx )
					dbStockpoint.SetWarehouseFlag( nSpIdx, TRUE );
				else
					dbStockpoint.SetWarehouseFlag( nSpIdx, FALSE );
			}
		}

		if ( ( dlg.m_nTransfer1 != 0 ) || ( dlg.m_nTransfer2 != 0 ) )
		{
			for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
			{
				if ( dbStockpoint.GetWarehouseFlag( nSpIdx ) == TRUE )
				{
					switch( dlg.m_nTransfer1 )
					{
					case 1:
						dbStockpoint.SetTransferInFlag( nSpIdx, TRUE );
						break;

					case 2:
						dbStockpoint.SetTransferInFlag( nSpIdx, FALSE );
						break;
					}
				}
				else
				{
					switch( dlg.m_nTransfer2 )
					{
					case 1:
						dbStockpoint.SetTransferOutFlag( nSpIdx, TRUE );
						break;

					case 2:
						dbStockpoint.SetTransferOutFlag( nSpIdx, FALSE );
						break;
					}
				}
			}
		}

		if ( ( dlg.m_nTransfer1 != 0 ) || ( dlg.m_nTransfer2 != 0 ) || ( dlg.m_nSpIdx != -1 ) )
		{
			for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
			{
				if ( dbStockpoint.GetWarehouseFlag( nSpIdx ) == TRUE )
					dbStockpoint.SetTransferOutFlag( nSpIdx, TRUE );
				else
					dbStockpoint.SetTransferInFlag( nSpIdx, TRUE );
			}

			RebuildStockpointTree( nDbIdx, hItem, 0 );	
			m_treeSystem.SelectItem( hItem );
		}
	}
}

/**********************************************************************/

void CPropPageSetupSystem::CollapseStockTree()
{
	CollapseStockTree ( m_treeSystem.GetRootItem() );
}

/**********************************************************************/

void CPropPageSetupSystem::CollapseStockTree( HTREEITEM hItem )
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
		CollapseStockTree ( hChild );
		hChild = m_treeSystem.GetNextSiblingItem ( hChild );
	}
}

/**********************************************************************/
