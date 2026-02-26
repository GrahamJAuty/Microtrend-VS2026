/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "EposTaskDatabase.h"
#include "KeyboardSetCSVArray.h"
#include "LocationCSVArray.h"
#include "LocationSetCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "PosTrayTokenFiles.h"
#include "PropertySheetSetup.h"
#include "ReportpointCSVArray.h"
#include "SelectMultipleLocationDlg.h"
#include "SelectMultipleTerminalDlg.h"
#include "..\SPosStockManager5\SelectMultipleStockpointDlg.h"
#include "..\SPosStockManager5\SiteCSVArray.h"
#include "..\SPosStockManager5\StockpointCSVArray.h"
#include "SystemMap.h"
#include "TreeNode.h"
/**********************************************************************/
#include "PropPageSetupSystem.h"
/**********************************************************************/

CPropPageSetupSystem::CPropPageSetupSystem()
	: CPropertyPage(CPropPageSetupSystem::IDD)
{
	//{{AFX_DATA_INIT(CPropPageSetupSystem)
	//}}AFX_DATA_INIT
	m_nNetworkNo = 0;
	m_nTerminalIdx = 0;
	m_nDbNo = 0;
	m_nRpNo = 0;
	m_nKbsNo = 0;
	m_nSiteNo = 0;
	m_nSpNo = 0;
	m_nLocNo = 0;
	m_nLsNo = 0;
}

/**********************************************************************/

CPropPageSetupSystem::~CPropPageSetupSystem()
{
}

/**********************************************************************/

void CPropPageSetupSystem::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageSetupSystem)
	DDX_Control(pDX, IDC_BUTTON_OUTLINE, m_buttonOutline);
	DDX_Control(pDX, IDC_BUTTON_ACTION4, m_buttonAction4);
	DDX_Control(pDX, IDC_BUTTON_ACTION3, m_buttonAction3);
	DDX_Control(pDX, IDC_BUTTON_ACTION2, m_buttonAction2);
	DDX_Control(pDX, IDC_BUTTON_ACTION1, m_buttonAction1);
	DDX_Control(pDX, IDC_SETUP_TREE, m_treeSystem);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageSetupSystem, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageSetupSystem)
	ON_NOTIFY(NM_DBLCLK, IDC_SETUP_TREE, OnDblclkSetupTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_SETUP_TREE, OnSelectItem)
	ON_BN_CLICKED(IDC_BUTTON_ACTION1, OnButtonAction1)
	ON_BN_CLICKED(IDC_BUTTON_ACTION2, OnButtonAction2)
	ON_BN_CLICKED(IDC_BUTTON_ACTION3, OnButtonAction3)
	ON_BN_CLICKED(IDC_BUTTON_ACTION4, OnButtonAction4)
	ON_BN_CLICKED(IDC_BUTTON_OUTLINE, OnButtonOutline)
	ON_NOTIFY(NM_RCLICK, IDC_SETUP_TREE, OnRightClickTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageSetupSystem::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	m_ImageList.Create ( IDB_ICONS_TREE, 31, 0, COLORREF(0) );
	m_treeSystem.SetImageList ( &m_ImageList, TVSIL_NORMAL );
	m_treeSystem.SetImageList ( &m_ImageList, TVSIL_STATE );
	BuildTree();
	return TRUE;  
}

/**********************************************************************/

int CPropPageSetupSystem::GetNodeType( HTREEITEM hItem )
{
	DWORD dwItem = m_treeSystem.GetItemData( hItem );
	return ( dwItem & 0xF0000000 ) >> 28;
}

/**********************************************************************/

void CPropPageSetupSystem::BuildTree()
{
	DeleteTreeNodes();

	switch ( m_nTreeType )
	{
	case SETUPTREE_TYPE_NETWORK:	
		BuildNetworkTree();		
		break;

	case SETUPTREE_TYPE_SPTBOOK:	
		BuildSportsBookerTree();		
		break;

	case SETUPTREE_TYPE_SMARTENT:	
		BuildSmartEntertainmentTree();		
		break;
		
	case SETUPTREE_TYPE_SYSTEM:		
		BuildSystemTree();		
		break;

	case SETUPTREE_TYPE_REPPOINT:	
		BuildRepPointTree();	
		break;

	case SETUPTREE_TYPE_KBSET:	
		BuildKbSetTree();	
		break;

	case SETUPTREE_TYPE_LOCSET:		
		BuildLocSetTree();		
		break;

#ifdef STOCKMAN_SYSTEM
	
	case SETUPTREE_TYPE_STOCK:		
		BuildStockTree();		
		break;
	
	case SETUPTREE_TYPE_SITE:		
		BuildSiteTree();		
		break;

#endif
	}
}

/**********************************************************************/

void CPropPageSetupSystem::ExpandTree()
{
	ExpandSubTree ( m_treeSystem.GetRootItem() );
}

/**********************************************************************/

void CPropPageSetupSystem::ExpandSubTree( HTREEITEM hRoot )
{
	m_treeSystem.Expand ( hRoot, TVE_EXPAND );

	HTREEITEM hChild = m_treeSystem.GetChildItem( hRoot );

	while ( hChild != NULL )
	{
		ExpandSubTree ( hChild );
		hChild = m_treeSystem.GetNextSiblingItem ( hChild );
	}
}
	
/**********************************************************************/

HTREEITEM CPropPageSetupSystem::AddRoot()
{
	HTREEITEM hRoot = m_treeSystem.InsertItem ( "System", TVI_ROOT, TVI_SORT );
	
	DWORD dwItem = SETUP_NODE_SYSTEM;
	dwItem <<= 28;
	
	m_treeSystem.SetItemData ( hRoot, dwItem );
	m_treeSystem.SetItemImage ( hRoot, NODE_ICON_SYSTEM, NODE_ICON_SYSTEM );
	
	return hRoot;
}

/**********************************************************************/

HTREEITEM CPropPageSetupSystem::AddNetworkNode ( HTREEITEM hParent, int nNetworkIdx )
{
	HTREEITEM hNewNode = m_treeSystem.InsertItem ( dbNetwork.GetTreeName( nNetworkIdx ), hParent );
	
	DWORD dwItem = SETUP_NODE_NETWORK << 28;
	dwItem += dbNetwork.GetNetworkNo( nNetworkIdx );
	
	m_treeSystem.SetItemData ( hNewNode, dwItem );
	m_treeSystem.SetItemImage ( hNewNode, NODE_ICON_NETWORK, NODE_ICON_NETWORK );
	
	m_treeSystem.Expand ( hParent, TVE_EXPAND );
	return hNewNode;
}

/**********************************************************************/

HTREEITEM CPropPageSetupSystem::AddDatabaseNode ( HTREEITEM hParent, int nDbIdx )
{
	HTREEITEM hNewNode = m_treeSystem.InsertItem ( dbDatabase.GetName( nDbIdx ), hParent );
	
	DWORD dwItem = SETUP_NODE_DBASE << 28;
	dwItem += dbDatabase.GetDbNo ( nDbIdx );
	
	m_treeSystem.SetItemData ( hNewNode, dwItem );
	m_treeSystem.SetItemImage ( hNewNode, NODE_ICON_DATABASE, NODE_ICON_DATABASE );
	
	m_treeSystem.Expand ( hParent, TVE_EXPAND );
	return hNewNode;
}

/**********************************************************************/

HTREEITEM CPropPageSetupSystem::AddLocationNode ( HTREEITEM hParent, int nLocIdx, int nType, bool bUseTreeName )
{		
	CString strName = "";
	if ( bUseTreeName )
		strName = dbLocation.GetTreeName( nLocIdx );
	else
		strName = dbLocation.GetName( nLocIdx );

	HTREEITEM hNewNode = m_treeSystem.InsertItem ( strName, hParent );
	
	DWORD dwItem = nType << 28;
	dwItem += ( dbLocation.GetNetworkNo( nLocIdx ) << 10 );
	dwItem += dbLocation.GetLocNo( nLocIdx );
	
	m_treeSystem.SetItemData ( hNewNode, dwItem );
	m_treeSystem.SetItemImage ( hNewNode, NODE_ICON_LOCATION, NODE_ICON_LOCATION );
	
	m_treeSystem.Expand ( hParent, TVE_EXPAND );
	return hNewNode;
}

/**********************************************************************/

HTREEITEM CPropPageSetupSystem::AddRepPointNode ( HTREEITEM hParent, int nRpIdx )
{
	HTREEITEM hNewNode = m_treeSystem.InsertItem ( dbReportpoint.GetName( nRpIdx ), hParent );
	
	DWORD dwItem = SETUP_NODE_REPPOINT << 28;
	dwItem += dbReportpoint.GetRpNo( nRpIdx );
	
	m_treeSystem.SetItemData ( hNewNode, dwItem );
	m_treeSystem.SetItemImage ( hNewNode, NODE_ICON_REPPOINT, NODE_ICON_REPPOINT );
	
	m_treeSystem.Expand ( hParent, TVE_EXPAND );
	return hNewNode;
}

/**********************************************************************/

HTREEITEM CPropPageSetupSystem::AddKbSetNode ( HTREEITEM hParent, int nKbsIdx )
{
	HTREEITEM hNewNode = m_treeSystem.InsertItem ( dbKeyboardSet.GetName( nKbsIdx ), hParent );
	
	DWORD dwItem = SETUP_NODE_KBSET << 28;
	dwItem += dbKeyboardSet.GetKbsNo( nKbsIdx );
	
	m_treeSystem.SetItemData ( hNewNode, dwItem );
	m_treeSystem.SetItemImage ( hNewNode, NODE_ICON_PRICESET, NODE_ICON_PRICESET );
	
	m_treeSystem.Expand ( hParent, TVE_EXPAND );
	return hNewNode;
}

/**********************************************************************/

HTREEITEM CPropPageSetupSystem::AddLocSetNode ( HTREEITEM hParent, int nLsIdx )
{
	HTREEITEM hNewNode = m_treeSystem.InsertItem ( dbLocationSet.GetName( nLsIdx ), hParent );
	
	DWORD dwItem = SETUP_NODE_LOCSET << 28;
	dwItem += dbLocationSet.GetLsNo ( nLsIdx );
	
	m_treeSystem.SetItemData ( hNewNode, dwItem );
	m_treeSystem.SetItemImage ( hNewNode, NODE_ICON_PRICESET, NODE_ICON_PRICESET );
	
	m_treeSystem.Expand ( hParent, TVE_EXPAND );
	return hNewNode;
}

/**********************************************************************/

HTREEITEM CPropPageSetupSystem::AddTerminalNode ( HTREEITEM hParent, int nType, int nLocIdx, int nTerminalIdx )
{
	CString strName;
	
	switch ( nType )
	{
	case SETUP_NODE_TERM_STKPOINT:
	case SETUP_NODE_TERM_REPPOINT:
		strName.Format( "%s, TNo %3.3d (%s)",
			dbLocation.GetName( nLocIdx ),
			dbLocation.GetTNo( nLocIdx, nTerminalIdx ),
			dbLocation.GetTerminalName( nLocIdx, nTerminalIdx ) );
		break;

	case SETUP_NODE_TERM:
	default:
		strName.Format( "TNo %3.3d (%s)",
			dbLocation.GetTNo( nLocIdx, nTerminalIdx ),
			dbLocation.GetTerminalName( nLocIdx, nTerminalIdx ) );
		
		nType = SETUP_NODE_TERM;
		break;
	}
		
	HTREEITEM hNewNode = m_treeSystem.InsertItem ( strName, hParent );

	DWORD dwItem = nType << 28;
	dwItem += ( dbLocation.GetNetworkNo ( nLocIdx ) << 18 );
	dwItem += ( dbLocation.GetLocNo ( nLocIdx ) << 8 );
	dwItem += nTerminalIdx;

	m_treeSystem.SetItemData ( hNewNode, dwItem );
	m_treeSystem.SetItemImage ( hNewNode, NODE_ICON_TERMINAL, NODE_ICON_TERMINAL );
	
	m_treeSystem.Expand ( hParent, TVE_EXPAND );
	return hNewNode;
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

HTREEITEM CPropPageSetupSystem::AddSiteNode ( HTREEITEM hParent, int nSiteIdx )
{		
	HTREEITEM hNewNode = m_treeSystem.InsertItem ( dbSite.GetName( nSiteIdx ), hParent );
	
	DWORD dwItem = SETUP_NODE_SITE << 28;
	dwItem += dbSite.GetSiteNo( nSiteIdx );
	
	m_treeSystem.SetItemData ( hNewNode, dwItem );
	m_treeSystem.SetItemImage ( hNewNode, NODE_ICON_SITE, NODE_ICON_SITE );
	
	m_treeSystem.Expand ( hParent, TVE_EXPAND );
	return hNewNode;
}

/**********************************************************************/

HTREEITEM CPropPageSetupSystem::AddStockpointNode ( HTREEITEM hParent, int nSpIdx, int nType )
{	
	int nIconType;
	if ( dbStockpoint.GetWarehouseFlag( nSpIdx ) == TRUE )
		nIconType = NODE_ICON_WAREHOUSE;
	else
		nIconType = NODE_ICON_STOCKPOINT;

	HTREEITEM hNewNode = m_treeSystem.InsertItem ( dbStockpoint.GetName( nSpIdx ), hParent );
	
	DWORD dwItem = nType << 28;
	dwItem += dbStockpoint.GetSpNo( nSpIdx );
	
	m_treeSystem.SetItemData ( hNewNode, dwItem );
	m_treeSystem.SetItemImage ( hNewNode, nIconType, nIconType );
	
	m_treeSystem.Expand ( hParent, TVE_EXPAND );
	return hNewNode;
}

/**********************************************************************/
#endif
/**********************************************************************/

void CPropPageSetupSystem::GetDataForTreeItem ( HTREEITEM hItem )
{	
	DWORD dwItem = m_treeSystem.GetItemData( hItem );

	switch ( GetNodeType( hItem ) )
	{
	case SETUP_NODE_SYSTEM:
		break;

	case SETUP_NODE_DBASE:
		m_nDbNo = dwItem & 0x3FF;
		GetDataForTreeItem ( m_treeSystem.GetParentItem ( hItem ) );
		break;

	case SETUP_NODE_NETWORK:
		m_nNetworkNo = dwItem & 0x3FF;
		GetDataForTreeItem ( m_treeSystem.GetParentItem ( hItem ) );
		break;

	case SETUP_NODE_LOC:
	case SETUP_NODE_LOC_DBASE:
	case SETUP_NODE_LOC_LOCSET:
	case SETUP_NODE_LOC_KBSET:
		m_nNetworkNo = ( dwItem & 0xFFC00 ) >> 10;
		m_nLocNo = ( dwItem & 0x3FF );
		GetDataForTreeItem ( m_treeSystem.GetParentItem ( hItem ) );
		break;

	case SETUP_NODE_REPPOINT:
		m_nRpNo = ( dwItem & 0x3FF );
		GetDataForTreeItem ( m_treeSystem.GetParentItem ( hItem ) );
		break;

	case SETUP_NODE_KBSET:
		m_nKbsNo = ( dwItem & 0x3FF );
		GetDataForTreeItem ( m_treeSystem.GetParentItem ( hItem ) );
		break;

	case SETUP_NODE_LOCSET:
		m_nLsNo = ( dwItem & 0x3FF );
		GetDataForTreeItem ( m_treeSystem.GetParentItem ( hItem ) );
		break;

	case SETUP_NODE_TERM:
	case SETUP_NODE_TERM_REPPOINT:
	case SETUP_NODE_TERM_STKPOINT:
		m_nNetworkNo = ( dwItem & 0xFFC0000 ) >> 18;
		m_nLocNo = ( dwItem & 0x3FF00 ) >> 8;
		m_nTerminalIdx = dwItem & 0xFF;
		GetDataForTreeItem ( m_treeSystem.GetParentItem ( hItem ) );
		break;

	case SETUP_NODE_STKPOINT:
	case SETUP_NODE_STKPOINT_SITE:
		m_nSpNo = dwItem & 0x3FF;
		GetDataForTreeItem ( m_treeSystem.GetParentItem ( hItem ) );
		break;

	case SETUP_NODE_SITE:
		m_nSiteNo = dwItem & 0x3FF;
		GetDataForTreeItem ( m_treeSystem.GetParentItem ( hItem ) );
		break;

	default:
		break;
	}
	return;
}

/**********************************************************************/

void CPropPageSetupSystem::GetLocalDataForTreeItem( HTREEITEM hItem, CSelectMultipleEntityInfo& infoNode, bool bFirstSibling )
{	
	if ( TRUE == bFirstSibling )
		infoNode.Reset();
	
	CSelectMultipleEntityFeeder Feeder;
	GetLocalDataForTreeItemInternal( hItem, Feeder, bFirstSibling );

	if ( Feeder.m_nDbNo != -1 )
		if ( dbDatabase.FindDatabaseByNumber( Feeder.m_nDbNo, infoNode.m_nDbIdx ) == FALSE )
			infoNode.m_nDbIdx = -1;

	if ( Feeder.m_nNwkNo != -1 )
		if ( dbNetwork.FindNetworkByNumber( Feeder.m_nNwkNo, infoNode.m_nNwkIdx ) == FALSE )
			infoNode.m_nNwkIdx = -1;
	
	if ( ( Feeder.m_nNwkNo != -1 ) && ( Feeder.m_nLocNo != -1 ) )
		if ( dbLocation.FindLocationByNumber( Feeder.m_nNwkNo, Feeder.m_nLocNo, infoNode.m_nLocIdx ) == FALSE )
			infoNode.m_nLocIdx = -1;

#ifdef STOCKMAN_SYSTEM
	if ( ( Feeder.m_nDbNo != -1 ) && ( Feeder.m_nSpNo != -1 ) )
		if ( dbStockpoint.FindStockpointByNumber( Feeder.m_nDbNo, Feeder.m_nSpNo, infoNode.m_nSpIdx ) == FALSE )
			infoNode.m_nSpIdx = -1;
#endif

	infoNode.m_nTermIdx = Feeder.m_nTermIdx;

}

/**********************************************************************/

void CPropPageSetupSystem::GetLocalDataForTreeItemInternal( HTREEITEM hItem, CSelectMultipleEntityFeeder& Feeder, bool bFirstSibling )
{
	DWORD dwItem = m_treeSystem.GetItemData( hItem );

	switch ( GetNodeType( hItem ) )
	{
	case SETUP_NODE_DBASE:
		Feeder.m_nDbNo = dwItem & 0x3FF;
		break;

	case SETUP_NODE_LOC_DBASE:
	case SETUP_NODE_LOC_LOCSET:
	case SETUP_NODE_LOC_KBSET:
		Feeder.m_nNwkNo =  ( dwItem & 0xFFC00 ) >> 10;
		Feeder.m_nLocNo = ( dwItem & 0x3FF );
		break;

	case SETUP_NODE_TERM_REPPOINT:
	case SETUP_NODE_TERM_STKPOINT:
		Feeder.m_nNwkNo = ( dwItem & 0xFFC0000 ) >> 18;
		Feeder.m_nLocNo = ( dwItem & 0x3FF00 ) >> 8;
		Feeder.m_nTermIdx = dwItem & 0xFF;
		break;

	case SETUP_NODE_STKPOINT_SITE:
		Feeder.m_nSpNo = dwItem & 0x3FF;
		break;
	}

	if ( TRUE == bFirstSibling )
	{
		switch ( GetNodeType( hItem ) )
		{
		case SETUP_NODE_STKPOINT_SITE:
		case SETUP_NODE_SITE:
			GetLocalDataForTreeItemInternal( m_treeSystem.GetParentItem ( hItem ), Feeder, TRUE );
			break;
		}
	}
}

/**********************************************************************/

void CPropPageSetupSystem::EnableActionButtons ( bool bAction1, bool bAction2, bool bAction3, bool bAction4 )
{
	m_buttonAction1.EnableWindow ( bAction1 );
	m_buttonAction2.EnableWindow ( bAction2 );
	m_buttonAction3.EnableWindow ( bAction3 );
	m_buttonAction4.EnableWindow ( bAction4 );
}

/**********************************************************************/

void CPropPageSetupSystem::SetButtonTexts ( const char* szButton1, const char* szButton2, const char* szButton3, const char* szButton4 )
{
	m_buttonAction1.SetWindowText ( szButton1 );
	m_buttonAction2.SetWindowText ( szButton2 );
	m_buttonAction3.SetWindowText ( szButton3 );
	m_buttonAction4.SetWindowText ( szButton4 );
}

/**********************************************************************/

void CPropPageSetupSystem::OnSelectItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SelectItem();
	*pResult = 0;
}

/**********************************************************************/

void CPropPageSetupSystem::SelectItem()
{
	HTREEITEM hItem = m_treeSystem.GetSelectedItem();

	if ( hItem != NULL )		
	{
		m_hItem = hItem;
		int nNodeType = GetNodeType( m_hItem );
		GetDataForTreeItem ( m_hItem );

		CString str;
		str.Format( " Node Type %d \n Db No %d \n Net No %d \n Loc No %d \n T Idx %d \n Rp No %d \n KbSet %d \n Ls No %d \n Sp No %d \n Site No %d",
			nNodeType,
			m_nDbNo,
			m_nNetworkNo,
			m_nLocNo,
			m_nTerminalIdx,
			m_nRpNo,
			m_nKbsNo,
			m_nLsNo,
			m_nSpNo,
			m_nSiteNo );
		//Prompter.Diagnostic( str );
		
		switch ( m_nTreeType )
		{
		case SETUPTREE_TYPE_NETWORK:	
			SelectItemNetworkTree( nNodeType );		
			break;

		case SETUPTREE_TYPE_SPTBOOK:	
			SelectItemSportsBookerTree( nNodeType );		
			break;

		case SETUPTREE_TYPE_SMARTENT:	
			SelectItemSmartEntertainmentTree( nNodeType );		
			break;

		case SETUPTREE_TYPE_SYSTEM:		
			SelectItemSystemTree( nNodeType );		
			break;

		case SETUPTREE_TYPE_REPPOINT:	
			SelectItemRepPointTree( nNodeType );	
			break;

		case SETUPTREE_TYPE_KBSET:	
			SelectItemKbSetTree( nNodeType );	
			break;

		case SETUPTREE_TYPE_LOCSET:		
			SelectItemLocSetTree( nNodeType );		
			break;

#ifdef STOCKMAN_SYSTEM
		
		case SETUPTREE_TYPE_STOCK:		
			SelectItemStockTree( nNodeType );		
			break;
		
		case SETUPTREE_TYPE_SITE:		
			SelectItemSiteTree( nNodeType);		
			break;

#endif
		}
	}
	else
	{
		m_buttonAction1.EnableWindow ( FALSE );
		m_buttonAction2.EnableWindow ( FALSE );
		m_buttonAction3.EnableWindow ( FALSE );
		m_buttonAction4.EnableWindow ( FALSE );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::OnDblclkSetupTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DWORD dwPos = ::GetMessagePos();
	CPoint point ( ( int ) LOWORD ( dwPos ), ( int ) HIWORD ( dwPos ) );

	m_treeSystem.ScreenToClient ( &point );

	UINT nFlags;
	m_treeSystem.HitTest( point, &nFlags );
	
	*pResult = 0;
	if ( nFlags == TVHT_ONITEMLABEL )
	{
		switch ( m_nTreeType )
		{
		case SETUPTREE_TYPE_NETWORK:	
			Button2NetworkTree();	
			break;

		case SETUPTREE_TYPE_SPTBOOK:	
			Button2SportsBookerTree();	
			break;

		case SETUPTREE_TYPE_SMARTENT:	
			Button2SmartEntertainmentTree();	
			break;

		case SETUPTREE_TYPE_SYSTEM:		
			Button2SystemTree();	
			break;

		case SETUPTREE_TYPE_REPPOINT:	
			Button2RepPointTree();	
			break;

		case SETUPTREE_TYPE_KBSET:	
			Button2KbSetTree();	
			break;

		case SETUPTREE_TYPE_LOCSET:		
			Button2LocSetTree();	
			break;

#ifdef STOCKMAN_SYSTEM
		
		case SETUPTREE_TYPE_STOCK:	
			Button2StockTree();		
			break;

		case SETUPTREE_TYPE_SITE:	
			Button2SiteTree();		
			break;

#endif
		}
		*pResult = 1;
	}
}

/**********************************************************************/

void CPropPageSetupSystem::DeleteTreeNodes()
{
	DeleteSubTreeNodes ( m_treeSystem.GetRootItem() );
}

/**********************************************************************/

void CPropPageSetupSystem::DeleteSubTreeNodes ( HTREEITEM hRoot )
{
	HTREEITEM hChild;
	while ( ( hChild = m_treeSystem.GetChildItem ( hRoot ) ) != NULL )
		DeleteSubTreeNodes ( hChild );
			
	m_treeSystem.DeleteItem ( hRoot );
}

/**********************************************************************/

void CPropPageSetupSystem::DeleteSubTreeNodesAndData ( HTREEITEM hRoot )
{
	int nNodeType = GetNodeType( hRoot );
	GetDataForTreeItem ( hRoot );

	int nDbNo = m_nDbNo;
	int nNetworkNo = m_nNetworkNo;
	int nLocNo = m_nLocNo;
	int nTerminalIdx = m_nTerminalIdx;
	int nRpNo = m_nRpNo;
	int nKbsNo = m_nKbsNo;
	int nLsNo = m_nLsNo;
	int nSpNo = m_nSpNo;
	int nSiteNo = m_nSiteNo;

	CString str;
	str.Format( " Node Type %d \n Db No %d \n Net No %d \n Loc No %d \n T Idx %d \n Rp No %d \n Ls No %d \n Sp No %d \n Site No %d",
		nNodeType,
		nDbNo,
		nNetworkNo,
		nLocNo,
		nTerminalIdx,
		nRpNo,
		nLsNo,
		nSpNo,
		nSiteNo );
	//Prompter.Diagnostic( str );

	HTREEITEM hParent = m_treeSystem.GetParentItem ( hRoot );
	HTREEITEM hNewSelection = m_treeSystem.GetNextSiblingItem ( hRoot );

	if ( hNewSelection == NULL )
		hNewSelection = m_treeSystem.GetPrevSiblingItem ( hRoot );

	if ( hNewSelection == NULL )
		hNewSelection = hParent;

	DeleteSubTreeNodes ( hRoot );

	switch ( nNodeType )
	{
	case SETUP_NODE_DBASE:
		dbDatabase.DeleteDatabaseByNumber ( nDbNo );
		break;

	case SETUP_NODE_NETWORK:
		{
			CEposTaskDatabase TaskDatabase;
			dbNetwork.DeleteNetworkByNumber ( nNetworkNo );
			TaskDatabase.CleanUpTaskFiles();
		}
		break;

	case SETUP_NODE_LOC:
		{
			CEposTaskDatabase TaskDatabase;
			dbLocation.DeleteLocationByNumber ( nNetworkNo, nLocNo );
			TaskDatabase.CleanUpTaskFiles();
		
			int nDbIdx;
			if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == TRUE )
			{
				int nStartIdx, nEndIdx;
				dbLocationSet.GetLsIdxRange( nDbIdx, nStartIdx, nEndIdx );

				for ( int nLsIdx = nStartIdx; nLsIdx <= nEndIdx; nLsIdx++ )
					dbLocationSet.RemoveLocation( nLsIdx, nNetworkNo, nLocNo );
			}
		}
		break;

	case SETUP_NODE_LOC_DBASE:
		{
			int nLocIdx;
			if ( dbLocation.FindLocationByNumber ( nNetworkNo, nLocNo, nLocIdx ) == TRUE )
			{
				dbLocation.SetDbNo ( nLocIdx, 0 );

				for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )
				{
					dbLocation.ClearAllRpNoLinks ( nLocIdx, nTerminalIdx );
					dbLocation.SetDbKeyboardSet( nLocIdx, 0 );
#ifdef STOCKMAN_SYSTEM
					dbLocation.SetSpNo ( nLocIdx, nTerminalIdx, 0 );
#endif
				}
			}

			int nDbIdx;
			if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == TRUE )
			{
				int nStartIdx, nEndIdx;
				dbLocationSet.GetLsIdxRange( nDbIdx, nStartIdx, nEndIdx );

				for ( int nLsIdx = nStartIdx; nLsIdx <= nEndIdx; nLsIdx++ )
					dbLocationSet.RemoveLocation( nLsIdx, nNetworkNo, nLocNo );
			}
		}
		break;

	case SETUP_NODE_REPPOINT:
		dbReportpoint.DeleteReportpointByNumber ( nDbNo, nRpNo );
		{
			int nDbIdx;
			if ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) )
			{
				CArray<int,int> Set;
				dbLocation.GetDatabaseLocIdxSet ( nDbIdx, Set );

				for ( int nIndex = 0; nIndex < Set.GetSize(); nIndex++ )
				{
					int nLocIdx = Set.GetAt ( nIndex );
					for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )
						dbLocation.ClearRpNoLink( nLocIdx, nTerminalIdx, nRpNo );
				}
			}
		}	
		break;

	case SETUP_NODE_KBSET:
		dbKeyboardSet.DeleteKeyboardSetByNumber ( nDbNo, nKbsNo );
		{
			int nDbIdx;
			if ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) )
			{
				CArray<int,int> Set;
				dbLocation.GetDatabaseLocIdxSet ( nDbIdx, Set );

				for ( int nIndex = 0; nIndex < Set.GetSize(); nIndex++ )
				{
					int nLocIdx = Set.GetAt ( nIndex );
					if ( dbLocation.GetDbKeyboardSet( nLocIdx ) == nKbsNo )
						dbLocation.SetDbKeyboardSet( nLocIdx, 0 );
				}
			}
		}	
		break;

	case SETUP_NODE_LOCSET:
		dbLocationSet.DeleteLocationSetByNumber ( nDbNo, nLsNo );
		break;

	case SETUP_NODE_LOC_LOCSET:
		{
			int nLsIdx;
			if ( dbLocationSet.FindLocationSetByNumber( nDbNo, nLsNo, nLsIdx ) == TRUE )
				dbLocationSet.RemoveLocation( nLsIdx, nNetworkNo, nLocNo );
		}
		break;
	
	case SETUP_NODE_TERM_REPPOINT:
		{
			int nLocIdx;
			if ( dbLocation.FindLocationByNumber ( nNetworkNo, nLocNo, nLocIdx ) == TRUE )
				dbLocation.ClearRpNoLink( nLocIdx, nTerminalIdx, nRpNo );
		}
		break;

	case SETUP_NODE_LOC_KBSET:
		{
			int nLocIdx;
			if ( dbLocation.FindLocationByNumber ( nNetworkNo, nLocNo, nLocIdx ) == TRUE )
				dbLocation.SetDbKeyboardSet ( nLocIdx, 0 );
		}
		break;
	
#ifdef STOCKMAN_SYSTEM
	case SETUP_NODE_STKPOINT:
		dbStockpoint.DeleteStockpointByNumber ( nDbNo, nSpNo );
		{
			int nDbIdx;
			if ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) )
			{
				CArray<int,int> Set;
				dbLocation.GetDatabaseLocIdxSet ( nDbIdx, Set );

				for ( int nIndex = 0; nIndex < Set.GetSize(); nIndex++ )
				{
					int nLocIdx = Set.GetAt ( nIndex );

					for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )
						if ( dbLocation.GetSpNo ( nLocIdx, nTerminalIdx ) == nSpNo )
							dbLocation.SetSpNo ( nLocIdx, nTerminalIdx, 0 );
				}
			}
		}	
		break;
	
	case SETUP_NODE_SITE:
		dbSite.DeleteSiteByNumber ( nDbNo, nSiteNo );
		{
			int nDbIdx;
			if ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) )
			{
				int nStartIdx, nEndIdx;
				dbStockpoint.GetSpIdxRange ( nDbIdx, nStartIdx, nEndIdx );

				for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
					if ( dbStockpoint.GetSiteNo( nSpIdx ) == nSiteNo )
						dbStockpoint.SetSiteNo( nSpIdx, 0 );
			}
		}
		break;

	case SETUP_NODE_TERM_STKPOINT:
		{
			int nLocIdx;
			if ( dbLocation.FindLocationByNumber ( nNetworkNo, nLocNo, nLocIdx ) == TRUE )
				dbLocation.SetSpNo ( nLocIdx, nTerminalIdx, 0 );
		}
		break;

	case SETUP_NODE_STKPOINT_SITE:
		{
			int nSpIdx;
			if ( dbStockpoint.FindStockpointByNumber ( nDbNo, nSpNo, nSpIdx ) == TRUE )
				dbStockpoint.SetSiteNo( nSpIdx, 0);
		}
		break;
#endif	
	}
	m_treeSystem.SelectItem ( hNewSelection );
}

/**********************************************************************/

void CPropPageSetupSystem::OnButtonAction1() 
{
	switch ( m_nTreeType )
	{
	case SETUPTREE_TYPE_NETWORK:	
		Button1NetworkTree();	
		break;

	case SETUPTREE_TYPE_SPTBOOK:	
		Button1SportsBookerTree();	
		break;

	case SETUPTREE_TYPE_SMARTENT:	
		Button1SmartEntertainmentTree();	
		break;

	case SETUPTREE_TYPE_SYSTEM:		
		Button1SystemTree();	
		break;

	case SETUPTREE_TYPE_REPPOINT:	
		Button1RepPointTree();	
		break;

	case SETUPTREE_TYPE_KBSET:	
		Button1KbSetTree();	
		break;

	case SETUPTREE_TYPE_LOCSET:		
		Button1LocSetTree();	
		break;

#ifdef STOCKMAN_SYSTEM
	
	case SETUPTREE_TYPE_STOCK:		
		Button1StockTree();		
		break;

	case SETUPTREE_TYPE_SITE:		
		Button1SiteTree();		
		break;

#endif
	}
}

/**********************************************************************/

void CPropPageSetupSystem::OnButtonAction2() 
{
	switch ( m_nTreeType )
	{
	case SETUPTREE_TYPE_NETWORK:	
		Button2NetworkTree();	
		break;

	case SETUPTREE_TYPE_SPTBOOK:	
		Button2SportsBookerTree();	
		break;

	case SETUPTREE_TYPE_SMARTENT:	
		Button2SmartEntertainmentTree();	
		break;

	case SETUPTREE_TYPE_SYSTEM:		
		Button2SystemTree();	
		break;

	case SETUPTREE_TYPE_REPPOINT:	
		Button2RepPointTree();	
		break;

	case SETUPTREE_TYPE_KBSET:	
		Button2KbSetTree();	
		break;

	case SETUPTREE_TYPE_LOCSET:		
		Button2LocSetTree();	
		break;

#ifdef STOCKMAN_SYSTEM
	
	case SETUPTREE_TYPE_STOCK:		
		Button2StockTree();		
		break;

	case SETUPTREE_TYPE_SITE:		
		Button2SiteTree();		
		break;

#endif
	}
}

/**********************************************************************/

void CPropPageSetupSystem::OnButtonAction3() 
{
	switch ( m_nTreeType )
	{
	case SETUPTREE_TYPE_NETWORK:	
		Button3NetworkTree();	
		break;

	case SETUPTREE_TYPE_SPTBOOK:	
		Button3SportsBookerTree();	
		break;

	case SETUPTREE_TYPE_SMARTENT:	
		Button3SmartEntertainmentTree();	
		break;

	case SETUPTREE_TYPE_SYSTEM:		
		Button3SystemTree();	
		break;

	case SETUPTREE_TYPE_REPPOINT:	
		Button3RepPointTree();	
		break;

	case SETUPTREE_TYPE_KBSET:	
		Button3KbSetTree();	
		break;

	case SETUPTREE_TYPE_LOCSET:		
		Button3LocSetTree();	
		break;

#ifdef STOCKMAN_SYSTEM
	
	case SETUPTREE_TYPE_STOCK:		
		Button3StockTree();		
		break;

	case SETUPTREE_TYPE_SITE:		
		Button3SiteTree();		
		break;

#endif
	}
}

/**********************************************************************/

void CPropPageSetupSystem::OnButtonAction4() 
{
	switch ( m_nTreeType )
	{
	case SETUPTREE_TYPE_NETWORK:	
		Button4NetworkTree();	
		break;

	case SETUPTREE_TYPE_SPTBOOK:	
		Button4SportsBookerTree();	
		break;

	case SETUPTREE_TYPE_SMARTENT:	
		Button4SmartEntertainmentTree();	
		break;
	}
}

/**********************************************************************/

BOOL CPropPageSetupSystem::OnSetActive() 
{
	switch ( m_nTreeType )
	{
	case SETUPTREE_TYPE_NETWORK:
	case SETUPTREE_TYPE_SPTBOOK:
	case SETUPTREE_TYPE_SMARTENT:
	case SETUPTREE_TYPE_SYSTEM:
	case SETUPTREE_TYPE_STOCK:
	case SETUPTREE_TYPE_REPPOINT:
	case SETUPTREE_TYPE_KBSET:
	case SETUPTREE_TYPE_LOCSET:
	case SETUPTREE_TYPE_SITE:
		BuildTree();
		break;
	}

	return CPropertyPage::OnSetActive();
}

/**********************************************************************/

BOOL CPropPageSetupSystem::OnKillActive() 
{
	SystemMapFull.WriteSystem( FALSE );
	CPosTrayTokenFiles::CreateSystemUpdateFile();
	return CPropertyPage::OnKillActive();
}

/**********************************************************************/

void CPropPageSetupSystem::OnButtonOutline() 
{
	switch ( m_nTreeType )
	{
	case SETUPTREE_TYPE_NETWORK:	
		CollapseNetworkTree();			
		break;

	case SETUPTREE_TYPE_SPTBOOK:	
		CollapseSportsBookerTree();			
		break;

	case SETUPTREE_TYPE_SMARTENT:	
		CollapseSmartEntertainmentTree();			
		break;

	case SETUPTREE_TYPE_SYSTEM:		
		CollapseSystemTree();			
		break;
	
	case SETUPTREE_TYPE_REPPOINT:	
		CollapseRepPointTree();			
		break;

	case SETUPTREE_TYPE_KBSET:	
		CollapseKbSetTree();			
		break;

	case SETUPTREE_TYPE_LOCSET:		
		CollapseLocSetTree();			
		break;

#ifdef STOCKMAN_SYSTEM
	
	case SETUPTREE_TYPE_STOCK:		
		CollapseStockTree();			
		break;

	case SETUPTREE_TYPE_SITE:		
		CollapseSiteTree();				
		break;

#endif
	}

	m_treeSystem.Invalidate();
	SelectItem();	
}
	
/**********************************************************************/

void CPropPageSetupSystem::OnRightClickTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	CPoint ptScreen, ptClient;
	GetCursorPos ( &ptScreen );
	
	UINT nFlags;
	ptClient = ptScreen;
	m_treeSystem.ScreenToClient ( &ptClient );
	HTREEITEM hItem = m_treeSystem.HitTest ( ptClient, &nFlags );
	
	if ( hItem == NULL )
		return;

	m_treeSystem.SelectItem ( hItem );
	m_hItem = hItem;
}

/**********************************************************************/

void CPropPageSetupSystem::RemoveLinks( HTREEITEM hItem, int nNodeType )
{
	if ( GetNodeType( hItem ) == nNodeType )
	{
		CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo> arrayNodeInfo;
		CArray<HTREEITEM,HTREEITEM> arrayHandles;
	
		HTREEITEM hParent = m_treeSystem.GetParentItem( hItem );
		HTREEITEM hChild = m_treeSystem.GetChildItem( hParent );

		CSelectMultipleEntityInfo infoNode;

		int nInitialSel = 0;
		bool bFirstSibling = TRUE;

		while ( hChild != NULL )
		{
			GetLocalDataForTreeItem( hChild, infoNode, bFirstSibling );
			bFirstSibling = FALSE;
			
			if ( hChild == hItem )
			{
				nInitialSel = arrayNodeInfo.GetSize();
				infoNode.m_bSelected = TRUE;
			}
			else
				infoNode.m_bSelected = FALSE;
			
			arrayNodeInfo.Add( infoNode );
			arrayHandles.Add( hChild );

			hChild = m_treeSystem.GetNextSiblingItem( hChild );
		}

		bool bContinue = FALSE;
		switch( nNodeType )
		{
		case SETUP_NODE_LOC_LOCSET:
		case SETUP_NODE_LOC_KBSET:
			{
				CSelectMultipleLocationDlg dlg ( "Remove Locations From Set", arrayNodeInfo, TRUE, this );
				bContinue = ( dlg.DoModal() == IDOK );
			}
			break;

		case SETUP_NODE_LOC_DBASE:
			{
				CSelectMultipleLocationDlg dlg ( "Remove Locations From Database", arrayNodeInfo, TRUE, this );
				bContinue = ( dlg.DoModal() == IDOK );
			}
			break;

		case SETUP_NODE_TERM_REPPOINT:
			{
				CSelectMultipleTerminalDlg dlg ( "Remove Terminals from Report Point", arrayNodeInfo, this );
				bContinue = ( dlg.DoModal() == IDOK );
			}
			break;

		case SETUP_NODE_TERM_STKPOINT:
			{
				CSelectMultipleTerminalDlg dlg ( "Remove Terminals From Stockpoint", arrayNodeInfo, this );
				bContinue = ( dlg.DoModal() == IDOK );
			}
			break;

#ifdef STOCKMAN_SYSTEM
		case SETUP_NODE_STKPOINT_SITE:
			{
				CSelectMultipleStockpointDlg dlg ( "Remove Stockpoints From Site", arrayNodeInfo, this );
				bContinue = ( dlg.DoModal() == IDOK );
			}
#endif
			break;
		}

		if ( TRUE == bContinue )
		{
			for ( int n = 0; n < arrayHandles.GetSize(); n++ )
				if ( TRUE == arrayNodeInfo[n].m_bSelected )
					DeleteSubTreeNodesAndData( arrayHandles[n] );

			HTREEITEM hNewSel = hParent;

			for ( int n = 0; n < arrayNodeInfo.GetSize(); n++ )
			{
				if ( FALSE == arrayNodeInfo[n].m_bSelected )
				{
					hNewSel = arrayHandles[n];

					if ( n >= nInitialSel )
						break;
				}
			}	

			m_treeSystem.SelectItem( hNewSel );
		}
	}
}

/**********************************************************************/
