/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "EcrmanOptionsIni.h"
//#include "globalfunctions.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
 
#include "PropertySheetDatabase.h"
#include "SelectMultipleLocationDlg.h"
#include "TreeNode.h"
/**********************************************************************/
#include "PropPageSetupSystem.h"
/**********************************************************************/

void CPropPageSetupSystem::BuildSystemTree()
{
	HTREEITEM hRoot = AddRoot();

	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
			BuildSystemTreeDatabase( hRoot, nDbIdx );
	}
	else
	{
		CWordArray arrayDbIdx;
		dbDatabase.SortByName( arrayDbIdx );

		for ( int n = 0; n < arrayDbIdx.GetSize(); n++ )
		{
			int nDbIdx = arrayDbIdx.GetAt(n);
			BuildSystemTreeDatabase( hRoot, nDbIdx );
		}
	}

	m_buttonAction4.ShowWindow ( FALSE );
	CollapseSystemTree();
	m_treeSystem.SelectItem ( m_treeSystem.GetRootItem() );
}

/**********************************************************************/

void CPropPageSetupSystem::BuildSystemTreeDatabase( HTREEITEM hRoot, int nDbIdx )
{
	int nDbNo = dbDatabase.GetDbNo( nDbIdx );
	HTREEITEM hDbNode = AddDatabaseNode ( hRoot, nDbIdx );
		
	CArray<int,int> Set;
	dbLocation.GetDatabaseLocIdxSet ( nDbIdx, Set );

	if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
		dbLocation.SortLocListByName( Set );

	for ( int nIndex = 0; nIndex < Set.GetSize(); nIndex++ )
	{
		int nLocIdx = Set.GetAt ( nIndex );
				
		HTREEITEM hLocNode = AddLocationNode ( hDbNode, nLocIdx, SETUP_NODE_LOC_DBASE, TRUE );
		
		for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )
			AddTerminalNode( hLocNode, -1, nLocIdx, nTerminalIdx );
	}	
}

/**********************************************************************/

void CPropPageSetupSystem::SelectItemSystemTree ( int nNodeType )
{
	switch ( nNodeType )
	{
	case SETUP_NODE_SYSTEM:
		SetButtonTexts ( "Add\nDatabase", "Edit", "Delete", "" );
		EnableActionButtons ( TRUE, FALSE, FALSE, FALSE );
		break;

	case SETUP_NODE_DBASE:
		SetButtonTexts ( "Link\nLocations", "Edit\nDatabase", "Delete\nDatabase", "" );
		EnableActionButtons ( TRUE, TRUE, (dbDatabase.GetSize() > 1), FALSE );
		break;

	case SETUP_NODE_LOC_DBASE:
		SetButtonTexts ( "Add\nTerminals", "Edit\nLocation", "Remove\nLink", "" );
		EnableActionButtons ( TRUE, TRUE, TRUE, FALSE );	
		break;

	case SETUP_NODE_TERM:
		SetButtonTexts ( "Add", "Edit\nTerminal", "Delete\nTerminals", "" );
		EnableActionButtons ( FALSE, TRUE, TRUE, FALSE );	
		break;
	}
}

/**********************************************************************/

void CPropPageSetupSystem::Button1SystemTree()
{
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_SYSTEM:			
		AddDatabase();			
		break;

	case SETUP_NODE_DBASE:			
		AddLocsToDatabase();	
		break;

	case SETUP_NODE_LOC_DBASE:
		AddTerminals();
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button2SystemTree()
{
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_DBASE:			
		EditDatabase();	
		break;

	case SETUP_NODE_LOC_DBASE:
		EditLoc( TRUE );		
		break;

	case SETUP_NODE_TERM:			
		EditTerminal( FALSE );	
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button3SystemTree()
{
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_DBASE:				
		DeleteDatabase();			
		break;

	case SETUP_NODE_LOC_DBASE:			
		RemoveLocsFromDatabase();	
		break;

	case SETUP_NODE_TERM:			
		DeleteTerminal();	
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::AddDatabase() 
{
	int nDbIdx = dbDatabase.AddDatabase();
	
	if ( nDbIdx == -1 )
	{
		Prompter.Error ( "You have already set up the\nmaximum number of databases" );
		return;
	}
	
	::CreateSubdirectory ( dbDatabase.GetFolderPathData( nDbIdx ) );
	::CreateSubdirectory ( dbDatabase.GetFolderPathSysset( nDbIdx ) );

	m_treeSystem.SelectItem ( AddDatabaseNode ( m_hItem, nDbIdx ) );
}

/**********************************************************************/

void CPropPageSetupSystem::EditDatabase() 
{
	int nDbIdx;
	dbDatabase.FindDatabaseByNumber ( m_nDbNo, nDbIdx );

	CPropertySheetDatabase propSheet( nDbIdx, FALSE, this );
	if ( propSheet.DoModal() == IDOK )
	{
		propSheet.SaveRecord();
		CString strName = dbDatabase.GetName( nDbIdx );
		m_treeSystem.SetItemText ( m_hItem, strName );
	}

}

/**********************************************************************/

void CPropPageSetupSystem::DeleteDatabase() 
{
	int nDbIdx;
	dbDatabase.FindDatabaseByNumber ( m_nDbNo, nDbIdx );
	
	CString strTitle;
	strTitle.Format ( "Delete %s", dbDatabase.GetExtendedSystemName( nDbIdx ) );

	CStringArray Warning;
	Warning.Add ( strTitle );
	Warning.Add ( "The database is removed from the system." );
	Warning.Add ( "" );
	
#ifdef STOCKMAN_SYSTEM
	Warning.Add ( "All plu and stock records in the database are lost." );
	Warning.Add ( "" );
	Warning.Add ( "All stockpoints linked to the database are lost." );
	Warning.Add ( "" );
	Warning.Add ( "All sites linked to the database are lost." );
	Warning.Add ( "" );
	Warning.Add ( "All reports linked to the database are lost." );
	Warning.Add ( "" );
	Warning.Add ( "All report points linked to the database are lost." );
	Warning.Add ( "" );
	Warning.Add ( "All supplier orders for the database are lost." );
#else
	Warning.Add ( "All plu records in the database are lost." );
	Warning.Add ( "" );
	Warning.Add ( "All reports linked to the database are lost." );
	Warning.Add ( "" );
	Warning.Add ( "All report points linked to the database are lost." );
#endif

	if ( Prompter.Warning( Warning ) == IDYES )
		DeleteSubTreeNodesAndData ( m_hItem );

	m_buttonAction3.EnableWindow ( dbDatabase.GetSize() > 1 ? TRUE : FALSE );		
}

/**********************************************************************/

void CPropPageSetupSystem::AddLocsToDatabase() 
{
	CArray<int,int> arrayLocIdx;
	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
		if ( dbLocation.GetDbNo ( nLocIdx ) == 0 )
			arrayLocIdx.Add( nLocIdx );

	if ( arrayLocIdx.GetSize() == 0 )
	{
		Prompter.Error ( "There are no more locations available to link." );
		return;
	}

	if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
		dbLocation.SortLocListByName( arrayLocIdx );

	CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo> arrayLocInfo;
	CSelectMultipleLocationDlg::CreateLocInfoArray( arrayLocIdx, arrayLocInfo );
	CSelectMultipleLocationDlg dlg ( "Add Location To Database", arrayLocInfo, TRUE, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		for ( int nIndex = 0; nIndex < arrayLocInfo.GetSize(); nIndex++ )
			if ( TRUE == arrayLocInfo[nIndex].m_bSelected )
				dbLocation.SetDbNo ( arrayLocInfo[nIndex].m_nLocIdx, m_nDbNo );
		
		AddDatabaseLocNodes ( m_hItem, m_nDbNo );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::AddDatabaseLocNodes ( HTREEITEM hDbNode, int nDbNo )
{
	HTREEITEM hChild;
	while ( ( hChild = m_treeSystem.GetChildItem ( hDbNode ) ) != NULL )
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
		HTREEITEM hLocNode = AddLocationNode ( hDbNode, nLocIdx, SETUP_NODE_LOC_DBASE, TRUE );
	
		for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )
			AddTerminalNode( hLocNode, -1, nLocIdx, nTerminalIdx );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::RemoveLocsFromDatabase() 
{
	HTREEITEM hItem = m_treeSystem.GetSelectedItem();
	RemoveLinks( hItem, SETUP_NODE_LOC_DBASE );
}

/**********************************************************************/

void CPropPageSetupSystem::CollapseSystemTree()
{
	CollapseSystemTree ( m_treeSystem.GetRootItem() );
}

/**********************************************************************/

void CPropPageSetupSystem::CollapseSystemTree( HTREEITEM hItem )
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
		CollapseSystemTree ( hChild );
		hChild = m_treeSystem.GetNextSiblingItem ( hChild );
	}
}

/**********************************************************************/
