/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DealerFlags.h"
#include "..\CommonEcrStock\DefEditDlg.h"
//#include "EcrmanOptionsIni.h"
#include "EposSelectArray.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
 
#include "ReportpointCSVArray.h"
#include "SelectMultipleTerminalDlg.h"
#include "TreeNode.h"
/**********************************************************************/
#include "PropPageSetupSystem.h"
/**********************************************************************/

void CPropPageSetupSystem::BuildRepPointTree()
{
	HTREEITEM hRoot = AddRoot();

	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
			BuildRepPointTreeDatabase( hRoot, nDbIdx );
	}
	else
	{
		CWordArray arrayDbIdx;
		dbDatabase.SortByName( arrayDbIdx );

		for ( int n = 0; n < arrayDbIdx.GetSize(); n++ )
		{
			int nDbIdx = arrayDbIdx.GetAt(n);
			BuildRepPointTreeDatabase( hRoot, nDbIdx );
		}
	}

	m_buttonAction4.ShowWindow( SW_HIDE );
	CollapseRepPointTree();
	m_treeSystem.SelectItem ( m_treeSystem.GetRootItem() );
}

/**********************************************************************/

void CPropPageSetupSystem::BuildRepPointTreeDatabase( HTREEITEM hRoot, int nDbIdx )
{
	CString strName = dbDatabase.GetName( nDbIdx );

	HTREEITEM hDbNode = AddDatabaseNode ( hRoot, nDbIdx );

	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		int nStartIdx, nEndIdx;
		dbReportpoint.GetRpIdxRange( nDbIdx, nStartIdx, nEndIdx );

		for ( int nRpIdx = nStartIdx; nRpIdx <= nEndIdx; nRpIdx++ )
			BuildRepPointTreeRepPoint( hDbNode, nDbIdx, nRpIdx );
	}
	else
	{
		CWordArray arrayRpIdx;
		dbReportpoint.SortByName( arrayRpIdx, nDbIdx );

		for ( int n = 0; n < arrayRpIdx.GetSize(); n++ )
		{
			int nRpIdx = arrayRpIdx.GetAt(n);
			BuildRepPointTreeRepPoint( hDbNode, nDbIdx, nRpIdx );
		}
	}	
}

/**********************************************************************/

void CPropPageSetupSystem::BuildRepPointTreeRepPoint( HTREEITEM hDbNode, int nDbIdx, int nRpIdx )
{		
	int nDbNo = dbDatabase.GetDbNo( nDbIdx );
	int nRpNo = dbReportpoint.GetRpNo( nRpIdx );
	CString strName = dbReportpoint.GetName( nRpIdx );
	HTREEITEM hRpNode = AddRepPointNode ( hDbNode, nRpIdx );
	AddReportpointTerminalNodes ( hRpNode, nRpNo, nDbNo );	
}

/**********************************************************************/

void CPropPageSetupSystem::AddReportpointTerminalNodes ( HTREEITEM hRpNode, int nRpNo, int nDbNo )
{
	HTREEITEM hChild;
	while ( ( hChild = m_treeSystem.GetChildItem ( hRpNode ) ) != NULL )
		DeleteSubTreeNodes ( hChild );
	
	int nDbIdx;
	if ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) == FALSE )
		return;

	CArray<int,int> Set;
	dbLocation.GetDatabaseLocIdxSet ( nDbIdx, Set );

	if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
		dbLocation.SortLocListByName( Set );

	for ( int nIndex = 0; nIndex < Set.GetSize(); nIndex++ )
	{
		int nLocIdx = Set.GetAt ( nIndex );
		AddReportpointTerminalNodesLocation( hRpNode, nRpNo, nLocIdx );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::AddReportpointTerminalNodesLocation ( HTREEITEM hRpNode, int nRpNo, int nLocIdx )
{
	for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )
		if ( dbLocation.HasRpNoLink( nLocIdx, nTerminalIdx, nRpNo ) == TRUE )
			AddTerminalNode ( hRpNode, SETUP_NODE_TERM_REPPOINT, nLocIdx, nTerminalIdx );
}

/**********************************************************************/

void CPropPageSetupSystem::SelectItemRepPointTree ( int nNodeType )
{
	switch ( nNodeType )
	{
	case SETUP_NODE_SYSTEM:
		SetButtonTexts ( "Add", "Edit", "Delete", "" );
		EnableActionButtons ( FALSE, FALSE, FALSE, FALSE );
		break;

	case SETUP_NODE_DBASE:
		SetButtonTexts ( "Add\nRep.Point", "Edit", "Delete", "" );
		EnableActionButtons ( TRUE, FALSE, FALSE, FALSE );
		break;

	case SETUP_NODE_REPPOINT:
		SetButtonTexts ( "Link\nTerminals", "Edit\nRep.Point", "Delete\nRep.Point", "" );
		EnableActionButtons ( TRUE, TRUE, TRUE, FALSE );	
		break;

	case SETUP_NODE_TERM_REPPOINT:
		SetButtonTexts ( "Add", "Edit\nTerminal", "Remove\nLink", "" );
		EnableActionButtons ( FALSE, TRUE, TRUE, FALSE );	
		break;
	}
}

/**********************************************************************/

void CPropPageSetupSystem::Button1RepPointTree()
{
	switch ( GetNodeType( m_hItem) )
	{
	case SETUP_NODE_DBASE:			
		AddReppoint();				
		break;

	case SETUP_NODE_REPPOINT:
		AddTerminalsToReppoint();	
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button2RepPointTree()
{
	switch ( GetNodeType( m_hItem) )
	{
	case SETUP_NODE_REPPOINT:		
		EditReppoint();		
		break;

	case SETUP_NODE_TERM_REPPOINT:
		EditTerminal( TRUE );		
		break;
	}
	
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button3RepPointTree()
{
	switch ( GetNodeType( m_hItem) )
	{
	case SETUP_NODE_REPPOINT:			
		DeleteReppoint();				
		break;

	case SETUP_NODE_TERM_REPPOINT:		
		RemoveTerminalsFromReppoint();	
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::AddReppoint()
{
	int nRpIdx = dbReportpoint.AddReportpoint ( m_nDbNo );
	if ( nRpIdx == -1 )
	{
		Prompter.Error ( "You have already set up the\nmaximum number of reportpoints" ); 
		return;
	}

	m_treeSystem.SelectItem ( AddRepPointNode ( m_hItem, nRpIdx ) );
}

/**********************************************************************/

void CPropPageSetupSystem::EditReppoint()
{
	int nRpIdx;
	if ( dbReportpoint.FindReportpointByNumber ( m_nDbNo, m_nRpNo, nRpIdx ) == FALSE )
		return;
	
	CChangeReportPointNameDlg dlg ( nRpIdx, this );

	if ( dlg.DoModal() == IDOK )
	{
		dbReportpoint.SetName ( nRpIdx, dlg.m_strName );
		CString strName = dbReportpoint.GetName( nRpIdx );
		m_treeSystem.SetItemText ( m_hItem, strName );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::DeleteReppoint()
{
	int nRpIdx; 
	if ( dbReportpoint.FindReportpointByNumber ( m_nDbNo, m_nRpNo, nRpIdx ) == FALSE )
		return;

	if ( Prompter.YesNo ( "Are you sure you wish to delete this report point" ) == IDNO )
		return;

	if ( dbReportpoint.RenameForDeletion( nRpIdx ) == FALSE )
		Prompter.Error( "Unable to delete report point" );
	else
		DeleteSubTreeNodesAndData ( m_hItem );
}

/**********************************************************************/

void CPropPageSetupSystem::AddTerminalsToReppoint()
{
	CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo> arrayNodeInfo;

	int nDbIdx;
	if ( dbDatabase.FindDatabaseByNumber ( m_nDbNo, nDbIdx ) )
	{
		CArray<int,int> Set;
		dbLocation.GetDatabaseLocIdxSet ( nDbIdx, Set );

		if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
			dbLocation.SortLocListByName( Set );
		
		for ( int nIndex = 0; nIndex < Set.GetSize(); nIndex++ )
		{
			int nLocIdx = Set.GetAt ( nIndex );

			for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )
			{
				if ( dbLocation.CanAddRpNoLink( nLocIdx, nTerminalIdx, m_nRpNo ) )
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

	if ( arrayNodeInfo.GetSize() == 0 )
	{
		Prompter.Error ( "There are no more terminals available to link." );
		return;
	}

	CSelectMultipleTerminalDlg dlg ( "Link Terminals To Report Point", arrayNodeInfo, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		for ( int nIndex = 0; nIndex < arrayNodeInfo.GetSize(); nIndex++ )
			if ( TRUE == arrayNodeInfo[nIndex].m_bSelected )
				dbLocation.SetRpNoLink( arrayNodeInfo[nIndex].m_nLocIdx, arrayNodeInfo[nIndex].m_nTermIdx, m_nRpNo );
		
		AddReportpointTerminalNodes ( m_hItem, m_nRpNo, m_nDbNo );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::RemoveTerminalsFromReppoint()
{
	HTREEITEM hItem = m_treeSystem.GetSelectedItem();
	RemoveLinks( hItem, SETUP_NODE_TERM_REPPOINT ); 
}

/**********************************************************************/

void CPropPageSetupSystem::CollapseRepPointTree()
{
	CollapseRepPointTree ( m_treeSystem.GetRootItem() );
}

/**********************************************************************/

void CPropPageSetupSystem::CollapseRepPointTree( HTREEITEM hItem )
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
		CollapseRepPointTree ( hChild );
		hChild = m_treeSystem.GetNextSiblingItem ( hChild );
	}
}

/**********************************************************************/

