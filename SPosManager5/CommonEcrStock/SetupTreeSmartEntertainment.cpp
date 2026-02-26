/**********************************************************************/
 
/**********************************************************************/
#include "..\CommonEcrStock\DefEditDlg.h"
//#include "EcrmanOptionsIni.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "SportsBookerNetworkEditDlg.h"
#include "NetworkTerminalMap.h"
 
#include "RepSpawn.h"
#include "SmartEntertainmentNetworkEditDlg.h"
#include "SmartEntertainmentWebAPISalesDate.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPageSetupSystem.h"
/**********************************************************************/

void CPropPageSetupSystem::BuildSmartEntertainmentTree()
{
	HTREEITEM hRoot = AddRoot();

	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		for ( int nNetworkIdx = 0; nNetworkIdx < dbNetwork.GetSize(); nNetworkIdx++ )
		{
			switch( dbNetwork.GetConnectionType( nNetworkIdx ) )
			{
			case CONNECTION_TYPE_SMARTENT_NONE:
			case CONNECTION_TYPE_SMARTENT_WEB:
				BuildSmartEntertainmentTreeNetwork( hRoot, nNetworkIdx );
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
			case CONNECTION_TYPE_SMARTENT_NONE:
			case CONNECTION_TYPE_SMARTENT_WEB:
				BuildSmartEntertainmentTreeNetwork( hRoot, nNetworkIdx );
				break;
			}
		}
	}

	m_buttonAction4.ShowWindow( SW_HIDE );
	CollapseSmartEntertainmentTree();
	m_treeSystem.SelectItem ( m_treeSystem.GetRootItem() );
}

/**********************************************************************/

void CPropPageSetupSystem::BuildSmartEntertainmentTreeNetwork( HTREEITEM hRoot, int nNetworkIdx )
{
	int nNetworkNo = dbNetwork.GetNetworkNo( nNetworkIdx );
	HTREEITEM hNetworkNode = AddNetworkNode ( hRoot, nNetworkIdx );
		
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		int nStartIdx, nEndIdx;
		dbLocation.GetNetworkLocIdxRange ( nNetworkIdx, nStartIdx, nEndIdx );
		for ( int nLocIdx = nStartIdx; nLocIdx <= nEndIdx; nLocIdx++ )
			BuildSmartEntertainmentTreeLocation( hNetworkNode, nLocIdx );
	}
	else
	{
		CArray<int,int> arrayLocIdx;
		dbLocation.SortByName( arrayLocIdx, -1, nNetworkIdx );

		for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
		{
			int nLocIdx = arrayLocIdx.GetAt(n);
			BuildSmartEntertainmentTreeLocation( hNetworkNode, nLocIdx );
		}
	}
}

/**********************************************************************/

void CPropPageSetupSystem::BuildSmartEntertainmentTreeLocation( HTREEITEM hNetworkNode, int nLocIdx )
{
	int nLocNo = dbLocation.GetLocNo( nLocIdx );			
	HTREEITEM hLocNode = AddLocationNode ( hNetworkNode, nLocIdx, SETUP_NODE_LOC, FALSE );
						
	for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )
		HTREEITEM hTerminalNode = AddTerminalNode ( hLocNode, SETUP_NODE_TERM, nLocIdx, nTerminalIdx );
}

/**********************************************************************/

void CPropPageSetupSystem::SelectItemSmartEntertainmentTree ( int nNodeType )
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
		SetButtonTexts ( "Add", "Edit\nLocation", "Delete\nLocation", "" );
		EnableActionButtons ( FALSE, TRUE, TRUE, FALSE );	
		break;

	case SETUP_NODE_TERM:
		SetButtonTexts ( "Add", "Edit", "Delete", "" );
		EnableActionButtons ( FALSE, FALSE, FALSE, FALSE );
		break;
	}
}

/**********************************************************************/

void CPropPageSetupSystem::Button1SmartEntertainmentTree()
{
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_SYSTEM:			
		AddSmartEntertainmentNetwork();	
		break;

	case SETUP_NODE_NETWORK:		
		AddSmartEntertainmentLoc();		
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button2SmartEntertainmentTree()
{
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_NETWORK:		
		EditSmartEntertainmentNetwork();	
		break;

	case SETUP_NODE_LOC:			
		EditSmartEntertainmentLoc();		
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button3SmartEntertainmentTree()
{
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_NETWORK:		
		DeleteSmartEntertainmentNetwork();
		break;

	case SETUP_NODE_LOC:			
		DeleteSmartEntertainmentLoc();	
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button4SmartEntertainmentTree()
{
	/*
	CFilenameUpdater FnUp ( SysFiles::NetCheckPrm );
	
	if ( dbNetwork.ValidateNetworks( FALSE, FALSE, FALSE, TRUE ) == FALSE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
	}
	else
		Prompter.Info ( "There are no duplicated terminal numbers\non any Sports Booker network." );
	*/
}

/**********************************************************************/

void CPropPageSetupSystem::AddSmartEntertainmentNetwork() 
{
	int nNetworkIdx = dbNetwork.AddNetwork( CONNECTION_TYPE_SMARTENT_NONE );
	
	if ( -1 == nNetworkIdx )
	{
		Prompter.Error ( "You have already set up the\nmaximum number of networks" );
		return;
	}
	
	::CreateSubdirectory ( dbNetwork.GetFolderPathData( nNetworkIdx ) );
	::CreateSubdirectory ( dbNetwork.GetFolderPathSysset( nNetworkIdx ) );

	CSmartEntertainmentWebAPISalesDate SmartEntertainmentDates;
	SmartEntertainmentDates.CreateFile( nNetworkIdx );
	
	m_treeSystem.SelectItem ( AddNetworkNode ( m_hItem, nNetworkIdx ) );
}

/**********************************************************************/

void CPropPageSetupSystem::EditSmartEntertainmentNetwork() 
{
	int nNetworkIdx;
	dbNetwork.FindNetworkByNumber ( m_nNetworkNo, nNetworkIdx );

	CSmartEntertainmentWebAPISalesDate dates;
	dates.Read( nNetworkIdx );
		
	CSmartEntertainmentNetworkEditDlg dlg ( nNetworkIdx, dates, this );

	if ( dlg.DoModal() == IDOK )
	{
		dbNetwork.SetName( nNetworkIdx, dlg.m_strName );
		dbNetwork.SetSportsBookerUserName ( nNetworkIdx, dlg.m_strUserName );
		dbNetwork.SetSportsBookerTimeOut( nNetworkIdx, dlg.m_nTimeOut );
		dbNetwork.SetConnectionType ( nNetworkIdx, CONNECTION_TYPE_SMARTENT_WEB );
		
		if ( dlg.m_strNewPassword != "" )
			dbNetwork.SetSportsBookerPassword ( nNetworkIdx, dlg.m_strNewPassword );

		dates.Write();

		m_treeSystem.SetItemText ( m_hItem, dbNetwork.GetTreeName( nNetworkIdx ) );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::DeleteSmartEntertainmentNetwork() 
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

void CPropPageSetupSystem::AddSmartEntertainmentLoc() 
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
		Prompter.Error( "You can only add one location\nto a Smart Entertainment network." );
		return;
	}

	int nLocIdx = dbLocation.AddLocationByUser ( nNetworkNo, CONNECTION_TYPE_SMARTENT_NONE );

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

void CPropPageSetupSystem::EditSmartEntertainmentLoc() 
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

void CPropPageSetupSystem::DeleteSmartEntertainmentLoc() 
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

void CPropPageSetupSystem::CollapseSmartEntertainmentTree()
{
	CollapseSmartEntertainmentTree ( m_treeSystem.GetRootItem() );
}

/**********************************************************************/

void CPropPageSetupSystem::CollapseSmartEntertainmentTree( HTREEITEM hItem )
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
