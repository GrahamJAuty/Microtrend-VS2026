/**********************************************************************/
 
/**********************************************************************/
#include "..\CommonEcrStock\DefEditDlg.h"
//#include "EcrmanOptionsIni.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "SportsBookerNetworkEditDlg.h"
#include "NetworkTerminalMap.h"
 
#include "RepSpawn.h"
#include "SportsBookerWebAPISalesDateTime.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPageSetupSystem.h"
/**********************************************************************/

void CPropPageSetupSystem::BuildSportsBookerTree()
{
	HTREEITEM hRoot = AddRoot();

	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		for ( int nNetworkIdx = 0; nNetworkIdx < dbNetwork.GetSize(); nNetworkIdx++ )
		{
			switch( dbNetwork.GetConnectionType( nNetworkIdx ) )
			{
			case CONNECTION_TYPE_SPTBOOK_NONE:
			case CONNECTION_TYPE_SPTBOOK_WEB:
				BuildSportsBookerTreeNetwork( hRoot, nNetworkIdx );
				break;
			}
		}
	}
	else
	{
		CWordArray arrayNetworkIdx;
		dbNetwork.SortByName( arrayNetworkIdx );

		for ( int n = 0; n < arrayNetworkIdx.GetSize(); n++ )
		{
			int nNetworkIdx = arrayNetworkIdx.GetAt(n);
			
			switch( dbNetwork.GetConnectionType( nNetworkIdx ) )
			{
			case CONNECTION_TYPE_SPTBOOK_NONE:
			case CONNECTION_TYPE_SPTBOOK_WEB:
				BuildSportsBookerTreeNetwork( hRoot, nNetworkIdx );
				break;
			}
		}
	}

	m_buttonAction4.ShowWindow( SW_HIDE );
	CollapseSportsBookerTree();
	m_treeSystem.SelectItem ( m_treeSystem.GetRootItem() );
}

/**********************************************************************/

void CPropPageSetupSystem::BuildSportsBookerTreeNetwork( HTREEITEM hRoot, int nNetworkIdx )
{
	int nNetworkNo = dbNetwork.GetNetworkNo( nNetworkIdx );
	HTREEITEM hNetworkNode = AddNetworkNode ( hRoot, nNetworkIdx );
		
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		int nStartIdx, nEndIdx;
		dbLocation.GetNetworkLocIdxRange ( nNetworkIdx, nStartIdx, nEndIdx );
		for ( int nLocIdx = nStartIdx; nLocIdx <= nEndIdx; nLocIdx++ )
			BuildSportsBookerTreeLocation( hNetworkNode, nLocIdx );
	}
	else
	{
		CArray<int,int> arrayLocIdx;
		dbLocation.SortByName( arrayLocIdx, -1, nNetworkIdx );

		for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
		{
			int nLocIdx = arrayLocIdx.GetAt(n);
			BuildSportsBookerTreeLocation( hNetworkNode, nLocIdx );
		}
	}
}

/**********************************************************************/

void CPropPageSetupSystem::BuildSportsBookerTreeLocation( HTREEITEM hNetworkNode, int nLocIdx )
{
	int nLocNo = dbLocation.GetLocNo( nLocIdx );			
	HTREEITEM hLocNode = AddLocationNode ( hNetworkNode, nLocIdx, SETUP_NODE_LOC, FALSE );
						
	for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )
		HTREEITEM hTerminalNode = AddTerminalNode ( hLocNode, SETUP_NODE_TERM, nLocIdx, nTerminalIdx );
}

/**********************************************************************/

void CPropPageSetupSystem::SelectItemSportsBookerTree ( int nNodeType )
{
	switch ( nNodeType )
	{
	case SETUP_NODE_SYSTEM:
		SetButtonTexts ( "Add\nNetwork", "Edit", "Delete", "" );
		EnableActionButtons ( TRUE, FALSE, FALSE, FALSE );
		break;

	case SETUP_NODE_NETWORK:
		SetButtonTexts ( "Add\nLocation", "Edit\nNetwork", "Delete\nNetwork", "" );
		EnableActionButtons ( TRUE, TRUE, TRUE, FALSE );
		break;

	case SETUP_NODE_LOC:
		SetButtonTexts ( "Add", "Edit\nLocation", "Delete", "" );
		EnableActionButtons ( FALSE, TRUE, TRUE, FALSE );	
		break;

	case SETUP_NODE_TERM:
		SetButtonTexts ( "Add", "Edit", "Delete", "" );
		EnableActionButtons ( FALSE, FALSE, FALSE, FALSE );
		break;
	}
}

/**********************************************************************/

void CPropPageSetupSystem::Button1SportsBookerTree()
{
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_SYSTEM:			
		AddSportsBookerNetwork();	
		break;

	case SETUP_NODE_NETWORK:		
		AddSportsBookerLoc();		
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button2SportsBookerTree()
{
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_NETWORK:		
		EditSportsBookerNetwork();	
		break;

	case SETUP_NODE_LOC:			
		EditSportsBookerLoc();		
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button3SportsBookerTree()
{
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_NETWORK:		
		DeleteSportsBookerNetwork();
		break;

	case SETUP_NODE_LOC:			
		DeleteSportsBookerLoc();	
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button4SportsBookerTree()
{
	/*
	CFilenameUpdater FnUp ( SysFiles::NetCheckPrm );
	
	if ( dbNetwork.ValidateNetworks( FALSE, FALSE, TRUE, FALSE ) == FALSE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
	}
	else
		Prompter.Info ( "There are no duplicated terminal numbers\non any Sports Booker network." );
	*/
}

/**********************************************************************/

void CPropPageSetupSystem::AddSportsBookerNetwork() 
{
	int nNetworkIdx = dbNetwork.AddNetwork( CONNECTION_TYPE_SPTBOOK_NONE );
	
	if ( -1 == nNetworkIdx )
	{
		Prompter.Error ( "You have already set up the\nmaximum number of networks" );
		return;
	}
	
	::CreateSubdirectory ( dbNetwork.GetFolderPathData( nNetworkIdx ) );
	::CreateSubdirectory ( dbNetwork.GetFolderPathSysset( nNetworkIdx ) );

	CSportsBookerWebAPISalesDateTime SportsBookerDates;
	SportsBookerDates.CreateFile( nNetworkIdx );
	
	m_treeSystem.SelectItem ( AddNetworkNode ( m_hItem, nNetworkIdx ) );
}

/**********************************************************************/

void CPropPageSetupSystem::EditSportsBookerNetwork() 
{
	int nNetworkIdx;
	dbNetwork.FindNetworkByNumber ( m_nNetworkNo, nNetworkIdx );

	CSportsBookerWebAPISalesDateTime dates;
	dates.Read( nNetworkIdx );
		
	CSportsBookerNetworkEditDlg dlg ( nNetworkIdx, dates, this );

	if ( dlg.DoModal() == IDOK )
	{
		dbNetwork.SetName( nNetworkIdx, dlg.m_strName );
		dbNetwork.SetSportsBookerSite ( nNetworkIdx, dlg.m_strSite );
		dbNetwork.SetSportsBookerUserName ( nNetworkIdx, dlg.m_strUserName );
		dbNetwork.SetSportsBookerTimeOut( nNetworkIdx, dlg.m_nTimeOut );
		dbNetwork.SetConnectionType ( nNetworkIdx, CONNECTION_TYPE_SPTBOOK_WEB );

		if ( dlg.m_strNewPassword != "" )
			dbNetwork.SetSportsBookerPassword ( nNetworkIdx, dlg.m_strNewPassword );

		dates.Write();

		m_treeSystem.SetItemText ( m_hItem, dbNetwork.GetTreeName( nNetworkIdx ) );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::DeleteSportsBookerNetwork() 
{
	int nNetworkNo = m_nNetworkNo;

	int nNetworkIdx;
	dbNetwork.FindNetworkByNumber ( nNetworkNo, nNetworkIdx );
	
	CString strTitle;
	strTitle.Format ( "Delete %s", dbNetwork.GetExtendedSystemName( nNetworkIdx ) );

	CStringArray Warning;
	Warning.Add ( strTitle );
	Warning.Add ( "The network is removed from the system." );
	Warning.Add ( "" );
	Warning.Add ( "All locations linked to the network are lost." );
	
	if ( Prompter.Warning( Warning ) == IDYES )
	{
		NetworkTerminalMap.RemoveNetwork( nNetworkNo );
		DeleteSubTreeNodesAndData ( m_hItem );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::AddSportsBookerLoc() 
{
	int nNetworkNo = m_nNetworkNo;

	int nNetworkIdx;
	bool bGotLocation = TRUE;
	if ( dbNetwork.FindNetworkByNumber( nNetworkNo, nNetworkIdx ) == TRUE )
	{
		int nStart, nEnd;
		dbLocation.GetNetworkLocIdxRange( nNetworkIdx, nStart, nEnd );
		bGotLocation = ( nEnd >= nStart );
	}

	if ( TRUE == bGotLocation )
	{
		Prompter.Error( "You can only add one location\nto a Sports Booker network." );
		return;
	}

	int nLocIdx = dbLocation.AddLocationByUser ( nNetworkNo, CONNECTION_TYPE_SPTBOOK_NONE );

	if ( nLocIdx == -1 )
	{
		Prompter.Error ( "You have already set up the\nmaximum number of locations" );
		return;
	}

	int nLocNo = dbLocation.GetLocNo( nLocIdx );
	dbLocation.SetTerminalExportPath( nLocIdx, "" );
	dbLocation.SetDownloadPath( nLocIdx, "" );
	dbLocation.SetDownloadNowPath( nLocIdx, "" );

	CChangeSBLocationNameDlg dlgName( nLocIdx, this );

	if ( dlgName.DoModal() != IDOK )
	{
		for ( int nTermIdx = 0; nTermIdx < dbLocation.GetTerminalCount( nLocIdx ); nTermIdx++ )
			NetworkTerminalMap.RemoveTerminal( nNetworkNo, dbLocation.GetTNo( nLocIdx, nTermIdx ) );

		dbLocation.DeleteLocationByIndex ( nLocIdx, FALSE );
	}
	else
	{
		dbLocation.SetName( nLocIdx, dlgName.m_strName );

		HTREEITEM hLoc = AddLocationNode ( m_hItem, nLocIdx, SETUP_NODE_LOC, FALSE );	
		for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )			
			AddTerminalNode ( hLoc, SETUP_NODE_TERM, nLocIdx, nTerminalIdx );
			
		m_treeSystem.SelectItem ( hLoc );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::EditSportsBookerLoc() 
{
	int nLocIdx; 
	dbLocation.FindLocationByNumber ( m_nNetworkNo, m_nLocNo, nLocIdx );
	
	CChangeSBLocationNameDlg dlgName( nLocIdx, this );
	if ( dlgName.DoModal() != IDOK )
		return;

	dbLocation.SetName( nLocIdx, dlgName.m_strName );

	HTREEITEM hTerminal;

	while ( ( hTerminal = m_treeSystem.GetChildItem ( m_hItem ) ) != NULL )
		m_treeSystem.DeleteItem ( hTerminal );
	
	for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )			
		AddTerminalNode ( m_hItem, SETUP_NODE_TERM, nLocIdx, nTerminalIdx );
	
	m_treeSystem.SetItemText ( m_hItem, dbLocation.GetName ( nLocIdx ) );
	m_treeSystem.SelectItem ( m_hItem );
}

/**********************************************************************/

void CPropPageSetupSystem::DeleteSportsBookerLoc() 
{
	int nNetworkNo = m_nNetworkNo;
	int nLocNo = m_nLocNo;

	int nLocIdx; 
	if ( dbLocation.FindLocationByNumber ( nNetworkNo, nLocNo, nLocIdx ) == FALSE )
		return;	
	
	CString strTitle;
	strTitle.Format ( "Delete %s", dbLocation.GetExtendedSystemName( nLocIdx ) );
	CStringArray Warning;
	Warning.Add ( strTitle );
	Warning.Add ( "The location is removed from the system." );
	
	if ( Prompter.Warning( Warning ) == IDYES )
	{
		for ( int nTermIdx = 0; nTermIdx < dbLocation.GetTerminalCount( nLocIdx ); nTermIdx++ )
			NetworkTerminalMap.RemoveTerminal( nNetworkNo, dbLocation.GetTNo( nLocIdx, nTermIdx ) );

		DeleteSubTreeNodesAndData ( m_hItem );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::CollapseSportsBookerTree()
{
	CollapseSportsBookerTree ( m_treeSystem.GetRootItem() );
}

/**********************************************************************/

void CPropPageSetupSystem::CollapseSportsBookerTree( HTREEITEM hItem )
{
	if ( hItem == NULL )
		return;

	int nActionType;
	switch ( GetNodeType( hItem ) )
	{
	case SETUP_NODE_SYSTEM:
	case SETUP_NODE_NETWORK:		
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
		CollapseSportsBookerTree ( hChild );
		hChild = m_treeSystem.GetNextSiblingItem ( hChild );
	}
}

/**********************************************************************/
