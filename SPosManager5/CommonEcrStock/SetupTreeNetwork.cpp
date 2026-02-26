/**********************************************************************/
//#include "EcrmanOptionsIni.h"
//#include "globalfunctions.h"
#include "LocationCSVArray.h"
#include "LocationMasterDlg.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "NetworkEditDlg.h"
#include "NetworkTerminalMap.h"
#include "NodeTypes.h"
#include "PriceSetItemManager.h"
#include "PropertySheetLocation.h"
#include "RepSpawn.h"
#include "SysInfo.h"
#include "TerminalRangeDlg.h"
#include "TreeNode.h"
/**********************************************************************/
#include "PropPageSetupSystem.h"
/**********************************************************************/

void CPropPageSetupSystem::BuildNetworkTree()
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
			case CONNECTION_TYPE_SMARTENT_NONE:
			case CONNECTION_TYPE_SMARTENT_WEB:
				break;

			default:
				BuildNetworkTreeNetwork( hRoot, nNetworkIdx );
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
			case CONNECTION_TYPE_SMARTENT_NONE:
			case CONNECTION_TYPE_SMARTENT_WEB:
				break;

			default:
				BuildNetworkTreeNetwork( hRoot, nNetworkIdx );
				break;
			}
		}
	}

	m_buttonAction4.SetWindowText ( "Check" );
	CollapseNetworkTree();
	m_treeSystem.SelectItem ( m_treeSystem.GetRootItem() );
}

/**********************************************************************/

void CPropPageSetupSystem::BuildNetworkTreeNetwork( HTREEITEM hRoot, int nNetworkIdx )
{
	int nNetworkNo = dbNetwork.GetNetworkNo( nNetworkIdx );
	HTREEITEM hNetworkNode = AddNetworkNode ( hRoot, nNetworkIdx );
		
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		int nStartIdx, nEndIdx;
		dbLocation.GetNetworkLocIdxRange ( nNetworkIdx, nStartIdx, nEndIdx );
		for ( int nLocIdx = nStartIdx; nLocIdx <= nEndIdx; nLocIdx++ )
			BuildNetworkTreeLocation( hNetworkNode, nLocIdx );
	}
	else
	{
		CArray<int,int> arrayLocIdx;
		dbLocation.SortByName( arrayLocIdx, -1, nNetworkIdx );

		for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
		{
			int nLocIdx = arrayLocIdx.GetAt(n);
			BuildNetworkTreeLocation( hNetworkNode, nLocIdx );
		}
	}
}

/**********************************************************************/

void CPropPageSetupSystem::BuildNetworkTreeLocation( HTREEITEM hNetworkNode, int nLocIdx )
{
	int nLocNo = dbLocation.GetLocNo( nLocIdx );			
	HTREEITEM hLocNode = AddLocationNode ( hNetworkNode, nLocIdx, SETUP_NODE_LOC, FALSE );
						
	for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )
		HTREEITEM hTerminalNode = AddTerminalNode ( hLocNode, SETUP_NODE_TERM, nLocIdx, nTerminalIdx );
}

/**********************************************************************/

void CPropPageSetupSystem::SelectItemNetworkTree(int nNodeType)
{
	CString strCheck = "Check\nTerminals";

	switch (nNodeType)
	{
	case SETUP_NODE_SYSTEM:
		SetButtonTexts("Add\nNetwork", "Edit", "Delete", strCheck);
		EnableActionButtons(TRUE, FALSE, FALSE, TRUE);
		break;

	case SETUP_NODE_NETWORK:
		SetButtonTexts("Add\nLocation", "Edit\nNetwork", "Delete\nNetwork", strCheck);
		EnableActionButtons(TRUE, TRUE, TRUE, TRUE);
		break;

	case SETUP_NODE_LOC:
		SetButtonTexts("Add\nTerminals", "Edit\nLocation", "Delete\nLocation", strCheck);
		EnableActionButtons(TRUE, TRUE, TRUE, TRUE);
		break;

	case SETUP_NODE_TERM:
		SetButtonTexts("Add", "Edit\nTerminal", "Delete\nTerminals", strCheck);
		EnableActionButtons(FALSE, TRUE, TRUE, TRUE);
		break;
	}
}

/**********************************************************************/

void CPropPageSetupSystem::Button1NetworkTree()
{
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_SYSTEM:			
		AddNetwork();	
		break;

	case SETUP_NODE_NETWORK:		
		AddLoc();		
		break;

	case SETUP_NODE_LOC:		
		AddTerminals();		
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button2NetworkTree()
{
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_NETWORK:		
		EditNetwork();	
		break;

	case SETUP_NODE_LOC:			
		EditLoc( TRUE );		
		break;

	case SETUP_NODE_TERM:			
		EditTerminal( FALSE );		
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button3NetworkTree()
{
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_NETWORK:		
		DeleteNetwork();
		break;

	case SETUP_NODE_LOC:			
		DeleteLoc();	
		break;

	case SETUP_NODE_TERM:			
		DeleteTerminal();	
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button4NetworkTree()
{
	CFilenameUpdater FnUp ( SysFiles::NetCheckPrm );
	
	if ( dbNetwork.ValidateNetworks( FALSE, TRUE, FALSE, FALSE ) == FALSE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
	}
	else
		Prompter.Info ( "There are no duplicated terminal numbers on any network." );
}

/**********************************************************************/

void CPropPageSetupSystem::AddNetwork() 
{
	int nNetworkIdx = dbNetwork.AddNetwork( CONNECTION_TYPE_STANDARD_NONE );
		
	if ( -1 == nNetworkIdx )
	{
		Prompter.Error ( "You have already set up the\nmaximum number of networks" );
		return;
	}
	
	::CreateSubdirectory ( dbNetwork.GetFolderPathData( nNetworkIdx ) );
	::CreateSubdirectory ( dbNetwork.GetFolderPathSysset( nNetworkIdx ) );
	
	m_treeSystem.SelectItem ( AddNetworkNode ( m_hItem, nNetworkIdx ) );
}

/**********************************************************************/

void CPropPageSetupSystem::EditNetwork() 
{
	int nNetworkIdx;
	dbNetwork.FindNetworkByNumber ( m_nNetworkNo, nNetworkIdx );
		
	CNetworkEditDlg dlg ( nNetworkIdx, this );

	if ( dlg.DoModal() == IDOK )
	{
		dbNetwork.SetSPOSVersion( nNetworkIdx, dlg.m_nSPOSVersion );
		dbNetwork.SetConnectionType ( nNetworkIdx, dlg.m_nConnectionType );
		dbNetwork.SetName ( nNetworkIdx, dlg.m_strName );
		dbNetwork.SetFileNetworkPath ( nNetworkIdx, dlg.m_strFilePath );
		m_treeSystem.SetItemText ( m_hItem, dbNetwork.GetTreeName( nNetworkIdx ) );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::DeleteNetwork() 
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

void CPropPageSetupSystem::AddLoc() 
{
	int nNetworkNo = m_nNetworkNo;

	int nLocIdx = dbLocation.AddLocationByUser ( nNetworkNo, CONNECTION_TYPE_STANDARD_NONE );

	if ( nLocIdx == -1 )
	{
		Prompter.Error ( "You have already set up the\nmaximum number of locations" );
		return;
	}

	int nLocNo = dbLocation.GetLocNo( nLocIdx );

	int nNetworkIdx;
	if ( dbNetwork.FindNetworkByNumber( nNetworkNo, nNetworkIdx ) )
	{
		switch( dbNetwork.GetConnectionType( nNetworkIdx ) )
		{
		case CONNECTION_TYPE_STANDARD_REALTIME:
		case CONNECTION_TYPE_STANDARD_BATCH:
		case CONNECTION_TYPE_STANDARD_NONE:
		default:
			{
				CString strNetworkPath;
				dbNetwork.GetSafeFileNetworkPath( nNetworkIdx, strNetworkPath );
		
				CString strTest = strNetworkPath;
				strTest.MakeUpper();
		
				if ( ( strTest.GetLength() > 9 ) && ( strTest.Right(9) == "\\UPAUDIT\\" ) )	
				{
					CString strNetworkParent = strNetworkPath.Left( strNetworkPath.GetLength() - 8 );
					dbLocation.SetTerminalExportPath( nLocIdx, "commsout" );
					dbLocation.SetDownloadPath( nLocIdx, strNetworkParent + "commsin" );
					dbLocation.SetDownloadNowPath( nLocIdx, strNetworkParent + "commsin\\now" );
				}
			}
			break;
		}
	}

	CPropertySheetLocation propSheet ( nLocIdx, TRUE, this );
	
	if ( propSheet.DoModal() != IDOK )
	{
		for ( int nTermIdx = 0; nTermIdx < dbLocation.GetTerminalCount( nLocIdx ); nTermIdx++ )
			NetworkTerminalMap.RemoveTerminal( nNetworkNo, dbLocation.GetTNo( nLocIdx, nTermIdx ) );

		dbLocation.DeleteLocationByIndex ( nLocIdx, FALSE );
	}
	else
	{
		propSheet.SaveRecord( TRUE );

		dbLocation.ResetPluPresetDate( nLocIdx, 0 );
		dbLocation.ResetPluInfoDate( nLocIdx, 0 );

		HTREEITEM hLoc = AddLocationNode ( m_hItem, nLocIdx, SETUP_NODE_LOC, FALSE );	
		for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )			
			AddTerminalNode ( hLoc, SETUP_NODE_TERM, nLocIdx, nTerminalIdx );
			
		LocationMasterMap.WriteLocationMasterFile( nLocIdx );

		m_treeSystem.SelectItem ( hLoc );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::EditLoc( bool bShowTerminals ) 
{
	int nLocIdx; 
	dbLocation.FindLocationByNumber ( m_nNetworkNo, m_nLocNo, nLocIdx );
	
	CArray<CLocationMasterInfo,CLocationMasterInfo> arrayMasters;
	LocationMasterMap.GetLocationMasterList( nLocIdx, arrayMasters, FALSE, FALSE );
	
	CPropertySheetLocation propSheet ( nLocIdx, FALSE, this );
	if ( propSheet.DoModal() != IDOK )
	{
		LocationMasterMap.SetLocationMasterList( nLocIdx, arrayMasters );
		return;
	}

	propSheet.SaveRecord( FALSE );

	if ( TRUE == bShowTerminals )
	{
		HTREEITEM hTerminal;
		while ( ( hTerminal = m_treeSystem.GetChildItem ( m_hItem ) ) != NULL )
			m_treeSystem.DeleteItem ( hTerminal );
		
		for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )			
			AddTerminalNode ( m_hItem, SETUP_NODE_TERM, nLocIdx, nTerminalIdx );
	}

	LocationMasterMap.WriteLocationMasterFile( nLocIdx );
	
	m_treeSystem.SetItemText ( m_hItem, dbLocation.GetName ( nLocIdx ) );
	m_treeSystem.SelectItem ( m_hItem );
}

/**********************************************************************/

void CPropPageSetupSystem::DeleteLoc() 
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

void CPropPageSetupSystem::AddTerminals()
{
	int nLocIdx = 0;
	if (dbLocation.FindLocationByNumber(m_nNetworkNo, m_nLocNo, nLocIdx) == TRUE)
	{
		if (dbLocation.IsWebSalesLocation(nLocIdx) == TRUE)
		{
			CString strError = "Terminals cannot be added or removed\n";

			if (SysInfo.IsSmartEntertainmentSystem() == TRUE)
			{
				strError += "from Smart Entertainment locations";
			}
			else
			{
				strError += "from Sports Booker locations";
			}

			Prompter.Error(strError);
		}
		else
		{
			CTerminalRangeDlg dlg(FALSE, this);
			dlg.m_nFrom = 1;
			dlg.m_nTo = 1;

			if (dlg.DoModal() != IDOK)
			{
				return;
			}

			if (dlg.m_nFrom > dlg.m_nTo)
			{
				return;
			}

			if ((dlg.m_nFrom < 1) || (dlg.m_nTo > 999))
			{
				return;
			}

			int nPos1 = 0;
			dbLocation.FindTerminalByNumber(nLocIdx, dlg.m_nFrom, nPos1);

			int nPos2 = 0;
			if (dbLocation.FindTerminalByNumber(nLocIdx, dlg.m_nTo, nPos2) == TRUE)
			{
				nPos2++;
			}

			int nNewCount = dbLocation.GetTerminalCount(nLocIdx) + (dlg.m_nTo - dlg.m_nFrom + 1) - (nPos2 - nPos1);

			if (nNewCount > MAX_TERMINALS)
			{
				CString str = "";
				str.Format("You can only add a maximum of %d terminals to each location.",
					MAX_TERMINALS);

				Prompter.Error(str);
				return;
			}

			for (int nTNo = dlg.m_nFrom; nTNo <= dlg.m_nTo; nTNo++)
			{
				int nTermIdx = 0;
				dbLocation.AddTerminal(nLocIdx, nTNo, 0, 0, 0, nTermIdx);
				NetworkTerminalMap.AddTerminal(m_nNetworkNo, nTNo, nLocIdx);
			}

			HTREEITEM hTerminal;

			while ((hTerminal = m_treeSystem.GetChildItem(m_hItem)) != NULL)
			{
				m_treeSystem.DeleteItem(hTerminal);
			}

			for (int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount(nLocIdx); nTerminalIdx++)
			{
				AddTerminalNode(m_hItem, SETUP_NODE_TERM, nLocIdx, nTerminalIdx);
			}
		}
	}
}

/**********************************************************************/

void CPropPageSetupSystem::EditTerminal( bool bShowLocName ) 
{
	int nLocIdx; 
	if ( dbLocation.FindLocationByNumber ( m_nNetworkNo, m_nLocNo, nLocIdx ) == TRUE )
	{
		if ( dbLocation.IsWebSalesLocation( nLocIdx ) == TRUE )
		{
			CString strError = "There are no editable settings for terminals\n";

			if ( SysInfo.IsSmartEntertainmentSystem() == TRUE )
				strError += "at Smart Entertainment locations";
			else
				strError += "at Sports Booker locations";

			Prompter.Error( strError );
		}
		else
		{
			CLocationMasterDlg dlgMaster( nLocIdx, dbLocation.GetTNo( nLocIdx, m_nTerminalIdx ), bShowLocName, this );
			
			if ( dlgMaster.DoModal() == IDOK )
				LocationMasterMap.WriteLocationMasterFile( nLocIdx );

			CString strName;
			if ( FALSE == bShowLocName )
			{
				strName.Format( "TNo %3.3d (%s)",
					dbLocation.GetTNo( nLocIdx, m_nTerminalIdx ),
					dbLocation.GetTerminalName( nLocIdx,m_nTerminalIdx ) );
			}
			else
			{
				strName.Format( "%s, TNo %3.3d (%s)",
					dbLocation.GetName( nLocIdx ),
					dbLocation.GetTNo( nLocIdx, m_nTerminalIdx ),
					dbLocation.GetTerminalName( nLocIdx,m_nTerminalIdx ) );
			}
			
			m_treeSystem.SetItemText ( m_hItem, strName );
			m_treeSystem.SelectItem ( m_hItem );
		}
	}
}

/**********************************************************************/

void CPropPageSetupSystem::DeleteTerminal()
{
	int nLocIdx = 0;
	if (dbLocation.FindLocationByNumber(m_nNetworkNo, m_nLocNo, nLocIdx) == TRUE)
	{
		if (dbLocation.IsWebSalesLocation(nLocIdx) == TRUE)
		{
			CString strError = "Terminals cannot be added or removed\n";

			if (SysInfo.IsSmartEntertainmentSystem() == TRUE)
			{
				strError += "from Smart Entertainment locations";
			}
			else
			{
				strError += "from Sports Booker locations";
			}

			Prompter.Error(strError);
		}
		else
		{
			CTerminalRangeDlg dlg(TRUE, this);
			dlg.m_nFrom = dbLocation.GetTNo(nLocIdx, m_nTerminalIdx);
			dlg.m_nTo = dlg.m_nFrom;

			if (dlg.DoModal() != IDOK)
			{
				return;
			}

			if (dlg.m_nFrom > dlg.m_nTo)
			{
				return;
			}

			if ((dlg.m_nFrom < 1) || (dlg.m_nTo > 999))
			{
				return;
			}

			int nTermIdxStart = 0;
			dbLocation.FindTerminalByNumber(nLocIdx, dlg.m_nFrom, nTermIdxStart);

			int nTermIdxEnd;
			if (dbLocation.FindTerminalByNumber(nLocIdx, dlg.m_nTo, nTermIdxEnd) == FALSE)
			{
				nTermIdxEnd--;
			}

			if (nTermIdxEnd >= nTermIdxStart)
			{
				for (int nTermIdx = nTermIdxEnd; nTermIdx >= nTermIdxStart; nTermIdx--)
				{
					int nTNo = dbLocation.GetTNo(nLocIdx, nTermIdx);

					CLocationMasterInfo infoMaster;
					LocationMasterMap.GetMasterInfo(nLocIdx, nTNo, infoMaster);
					LocationMasterMap.RemoveMaster(infoMaster);

					NetworkTerminalMap.RemoveTerminal(m_nNetworkNo, nTNo);
					dbLocation.RemoveTerminal(nLocIdx, nTNo);
				}

				LocationMasterMap.WriteLocationMasterFile(nLocIdx);

				HTREEITEM hLocation = m_treeSystem.GetParentItem(m_hItem);

				HTREEITEM hTerminal;
				while ((hTerminal = m_treeSystem.GetChildItem(hLocation)) != NULL)
				{
					m_treeSystem.DeleteItem(hTerminal);
				}

				bool bGotFinalSelection = FALSE;
				m_treeSystem.SelectItem(hLocation);
				for (int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount(nLocIdx); nTerminalIdx++)
				{
					hTerminal = AddTerminalNode(hLocation, SETUP_NODE_TERM, nLocIdx, nTerminalIdx);

					if (FALSE == bGotFinalSelection)
					{
						m_treeSystem.SelectItem(hTerminal);

						if (dbLocation.GetTNo(nLocIdx, nTerminalIdx) >= dlg.m_nFrom)
						{
							bGotFinalSelection = TRUE;
						}
					}
				}
			}
		}
	}
}

/**********************************************************************/

void CPropPageSetupSystem::CollapseNetworkTree()
{
	CollapseNetworkTree ( m_treeSystem.GetRootItem() );
}

/**********************************************************************/

void CPropPageSetupSystem::CollapseNetworkTree( HTREEITEM hItem )
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
		CollapseNetworkTree ( hChild );
		hChild = m_treeSystem.GetNextSiblingItem ( hChild );
	}
}

/**********************************************************************/
