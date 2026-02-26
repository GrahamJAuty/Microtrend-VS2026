/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "NodeTypes.h" 
#include "SelectMultipleStockpointDlg.h"
#include "SiteCSVArray.h"
#include "SiteEditDlg.h"
#include "StockpointCSVArray.h"
#include "TreeNode.h"
/**********************************************************************/
#include "..\CommonEcrStock\PropPageSetupSystem.h"
/**********************************************************************/

void CPropPageSetupSystem::BuildSiteTree()
{
	HTREEITEM hRoot = AddRoot();

	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
			BuildSiteTreeDatabase( hRoot, nDbIdx );
	}
	else
	{
		CWordArray arrayDbIdx;
		dbDatabase.SortByName( arrayDbIdx );

		for ( int n = 0; n < arrayDbIdx.GetSize(); n++ )
		{
			int nDbIdx = arrayDbIdx.GetAt(n);
			BuildSiteTreeDatabase( hRoot, nDbIdx );
		}
	}

	m_buttonAction4.ShowWindow( SW_HIDE );
	CollapseSiteTree();
	m_treeSystem.SelectItem ( m_treeSystem.GetRootItem() );
}

/**********************************************************************/

void CPropPageSetupSystem::BuildSiteTreeDatabase( HTREEITEM hRoot, int nDbIdx )
{
	int nDbNo = dbDatabase.GetDbNo( nDbIdx );
	CString strName = dbDatabase.GetName( nDbIdx );

	HTREEITEM hDbNode = AddDatabaseNode ( hRoot, nDbIdx );
		
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		int nStartIdx, nEndIdx;
		dbSite.GetSiteIdxRange( nDbIdx, nStartIdx, nEndIdx );
		for ( int nSiteIdx = nStartIdx; nSiteIdx <= nEndIdx; nSiteIdx++ )
			BuildSiteTreeSite( hDbNode, nDbIdx, nSiteIdx );
	}
	else
	{
		CWordArray arraySiteIdx;
		dbSite.SortByName( arraySiteIdx, nDbIdx );

		for ( int n = 0; n < arraySiteIdx.GetSize(); n++ )
		{
			int nSiteIdx = arraySiteIdx.GetAt(n);
			BuildSiteTreeSite( hDbNode, nDbIdx, nSiteIdx );
		}
	}
}

/**********************************************************************/

void CPropPageSetupSystem::BuildSiteTreeSite( HTREEITEM hDbNode, int nDbIdx, int nSiteIdx )
{
	int nDbNo = dbDatabase.GetDbNo( nDbIdx );
	int nSiteNo = dbSite.GetSiteNo ( nSiteIdx );
	CString strName = dbSite.GetName ( nSiteIdx );
	HTREEITEM hSiteNode = AddSiteNode ( hDbNode, nSiteIdx );
	AddSiteStockpointNodes ( hSiteNode, nSiteNo, nDbNo );	
}

/**********************************************************************/

void CPropPageSetupSystem::AddSiteStockpointNodes ( HTREEITEM hSiteNode, int nSiteNo, int nDbNo )
{
	HTREEITEM hChild;
	while ( ( hChild = m_treeSystem.GetChildItem ( hSiteNode ) ) != NULL )
		DeleteSubTreeNodes ( hChild );

	int nDbIdx;
	if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
		return;

	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange( nDbIdx, nStartIdx, nEndIdx );

	CArray<int,int> arraySpIdx;
	for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
		if ( dbStockpoint.GetSiteNo( nSpIdx ) == nSiteNo )
			arraySpIdx.Add( nSpIdx );

	dbStockpoint.SortSpListByName( arraySpIdx );

	for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
	{
		int nSpIdx = arraySpIdx.GetAt(n);	
		CString strName = dbStockpoint.GetName( nSpIdx );
		AddStockpointNode ( hSiteNode, nSpIdx, SETUP_NODE_STKPOINT_SITE );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::SelectItemSiteTree ( int nNodeType )
{
	switch ( nNodeType )
	{
	case SETUP_NODE_SYSTEM:
		SetButtonTexts ( "Add", "Edit", "Delete", "" );
		EnableActionButtons ( FALSE, FALSE, FALSE, FALSE );
		break;

	case SETUP_NODE_DBASE:
		SetButtonTexts ( "Add\nSite", "Edit", "Delete", "" );
		EnableActionButtons ( TRUE, FALSE, FALSE, FALSE );
		break;

	case SETUP_NODE_SITE:
		SetButtonTexts ( "Link\nStockpoint", "Edit\nSite", "Delete\nSite", "" );
		EnableActionButtons ( TRUE, TRUE, TRUE, FALSE );
		break;

	case SETUP_NODE_STKPOINT_SITE:
		SetButtonTexts ( "Add", "Edit\nStockpoint", "Remove\nLink", "" );
		EnableActionButtons ( FALSE, TRUE, TRUE, FALSE );	
		break;

	default:
		SetButtonTexts ( "Add", "Edit", "Delete", "" );
		EnableActionButtons ( FALSE, FALSE, FALSE, FALSE );
		break;
	}
}

/**********************************************************************/

void CPropPageSetupSystem::Button1SiteTree()
{
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_DBASE:		
		AddSite();				
		break;

	case SETUP_NODE_SITE:		
		AddStockpointsToSite();	
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button2SiteTree()
{
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_SITE:		
		EditSite();			
		break;

	case SETUP_NODE_STKPOINT_SITE:
		EditStockpoint();
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button3SiteTree()
{
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_SITE:			
		DeleteSite();				
		break;

	case SETUP_NODE_STKPOINT_SITE:	
		RemoveStockpointsFromSite();	
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::AddSite()
{
	int nSiteIdx = dbSite.AddSite ( m_nDbNo );
	if ( nSiteIdx == -1 )
	{
		Prompter.Error ( "You have already set up the\nmaximum number of sites." );
		return;
	}
		
	m_treeSystem.SelectItem ( AddSiteNode ( m_hItem, nSiteIdx ) );
}

/**********************************************************************/

void CPropPageSetupSystem::EditSite()
{
	int nSiteIdx;
	if ( dbSite.FindSiteByNumber ( m_nDbNo, m_nSiteNo, nSiteIdx ) == FALSE )
		return;

	CString strTitle;
	strTitle.Format ( "Edit %s", dbSite.GetSystemName( nSiteIdx ) );
	CSiteEditDlg dlg ( strTitle, this );
			
	dlg.m_strName = dbSite.GetName ( nSiteIdx );

	CAddressInfo infoAddress;
	dbSite.GetAddressInfo( nSiteIdx, infoAddress );

	dlg.m_strAddress1 = infoAddress.m_strAddress1;
	dlg.m_strAddress2 = infoAddress.m_strAddress2;
	dlg.m_strAddress3 = infoAddress.m_strAddress3;
	dlg.m_strPostcode = infoAddress.m_strPostcode;
	dlg.m_strPhone = infoAddress.m_strPhone;
	dlg.m_strFax = infoAddress.m_strFax;
	dlg.m_strEmail = infoAddress.m_strEmail;

	if ( dlg.DoModal() == IDOK )
	{
		dbSite.SetName ( nSiteIdx, dlg.m_strName );
		
		CAddressInfo infoAddress;
		infoAddress.m_strAddress1 = dlg.m_strAddress1;
		infoAddress.m_strAddress2 = dlg.m_strAddress2;
		infoAddress.m_strAddress3 = dlg.m_strAddress3;
		infoAddress.m_strPostcode = dlg.m_strPostcode;
		infoAddress.m_strPhone = dlg.m_strPhone;
		infoAddress.m_strFax = dlg.m_strFax;
		infoAddress.m_strEmail = dlg.m_strEmail;
		dbSite.SetAddressInfo( nSiteIdx, infoAddress );
		
		CString strName = dbSite.GetName( nSiteIdx );
		m_treeSystem.SetItemText ( m_hItem, strName );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::DeleteSite()
{
	int nSiteIdx;
	if ( dbSite.FindSiteByNumber ( m_nDbNo, m_nSiteNo, nSiteIdx ) == FALSE )
		return;

	if ( Prompter.YesNo ( "Are you sure you wish to delete this site" ) == IDNO )
		return;

	if ( dbSite.RenameForDeletion( nSiteIdx ) == FALSE )
		Prompter.Error( "Unable to delete site" );
	else
		DeleteSubTreeNodesAndData ( m_hItem );
}

/**********************************************************************/

void CPropPageSetupSystem::AddStockpointsToSite()
{
	int nDbIdx;
	if ( dbDatabase.FindDatabaseByNumber( m_nDbNo, nDbIdx ) == FALSE )
		return;

	int nSpIdxStart, nSpIdxEnd;
	dbStockpoint.GetSpIdxRange( nDbIdx, nSpIdxStart, nSpIdxEnd );

	CArray<int,int> arraySpIdx;
	for ( int nSpIdx = nSpIdxStart; nSpIdx <= nSpIdxEnd; nSpIdx++ )
	{		
		if ( dbStockpoint.GetSiteNo( nSpIdx ) == 0 )
			arraySpIdx.Add( nSpIdx );
	}

	if ( arraySpIdx.GetSize() == 0 )
	{
		Prompter.Error ( "There are no more stockpoints available to link." );
		return;
	}

	dbStockpoint.SortSpListByName( arraySpIdx );

	CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo> arraySpInfo;
	for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
	{
		CSelectMultipleEntityInfo infoNode;
		infoNode.m_nSpIdx = arraySpIdx.GetAt(n);
		infoNode.m_bSelected = FALSE;
		arraySpInfo.Add( infoNode );
	}

	CSelectMultipleStockpointDlg dlg ( "Add Stockpoints To Site", arraySpInfo, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		for ( int nIndex = 0; nIndex < arraySpInfo.GetSize(); nIndex++ )
			for ( int nIndex = 0; nIndex < arraySpInfo.GetSize(); nIndex++ )
				if ( TRUE == arraySpInfo[nIndex].m_bSelected )
					dbStockpoint.SetSiteNo ( arraySpInfo[nIndex].m_nSpIdx, m_nSiteNo );
		
		AddSiteStockpointNodes ( m_hItem, m_nSiteNo, m_nDbNo );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::RemoveStockpointsFromSite()
{
	HTREEITEM hItem = m_treeSystem.GetSelectedItem();
	RemoveLinks( hItem, SETUP_NODE_STKPOINT_SITE ); 
}

/**********************************************************************/

void CPropPageSetupSystem::CollapseSiteTree()
{
	CollapseSiteTree ( m_treeSystem.GetRootItem() );
}

/**********************************************************************/

void CPropPageSetupSystem::CollapseSiteTree( HTREEITEM hItem )
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
		CollapseSiteTree ( hChild );
		hChild = m_treeSystem.GetNextSiblingItem ( hChild );
	}
}

/**********************************************************************/
