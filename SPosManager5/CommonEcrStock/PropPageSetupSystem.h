#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "TreeNode.h"
/**********************************************************************/
#define SETUP_NODE_SYSTEM 0
#define SETUP_NODE_NETWORK 1
#define SETUP_NODE_LOC 2
#define SETUP_NODE_LOC_DBASE 3
#define SETUP_NODE_LOC_LOCSET 4
#define SETUP_NODE_TERM 5
#define SETUP_NODE_TERM_REPPOINT 6
#define SETUP_NODE_TERM_STKPOINT 7
#define SETUP_NODE_DBASE 8
#define SETUP_NODE_STKPOINT 9
#define SETUP_NODE_STKPOINT_SITE 10
#define SETUP_NODE_REPPOINT 11
#define SETUP_NODE_LOCSET 12
#define SETUP_NODE_SITE 13
#define SETUP_NODE_KBSET 14
#define SETUP_NODE_LOC_KBSET 15
/**********************************************************************/
#include "SelectMultipleEntityInfo.h"
/**********************************************************************/

class CPropPageSetupSystem : public CPropertyPage
{
public:
	CPropPageSetupSystem();
	~CPropPageSetupSystem();
	void SetTreeType ( int nType ) { m_nTreeType = nType; }

	int GetNodeType( HTREEITEM hItem );

	//{{AFX_DATA(CPropPageSetupSystem)
	enum { IDD = IDD_PROPPAGE_SETUP_SYSTEM };
	CButton	m_buttonOutline;
	CButton m_buttonAction4;
	CButton	m_buttonAction3;
	CButton	m_buttonAction2;
	CButton	m_buttonAction1;
	CTreeCtrl	m_treeSystem;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageSetupSystem)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageSetupSystem)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkSetupTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAction1();
	afx_msg void OnButtonAction2();
	afx_msg void OnButtonAction3();
	afx_msg void OnButtonAction4();
	afx_msg void OnButtonOutline();
	afx_msg void OnRightClickTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
protected:
	//general operations
	void BuildTree();
	void SelectItem();
	void EnableActionButtons ( bool bAction1, bool bAction2, bool bAction3, bool bAction4 );
	void SetButtonTexts ( const char* szButton1, const char* szButton2, const char* szButton3, const char* szButton4 );
	HTREEITEM AddRoot();
	HTREEITEM AddNetworkNode ( HTREEITEM hParent, int nNetworkIdx );
	HTREEITEM AddDatabaseNode ( HTREEITEM hParent, int nDbIdx );
	HTREEITEM AddLocationNode ( HTREEITEM hParent, int nLocIdx, int nType, bool bUseTreeName );
	HTREEITEM AddRepPointNode ( HTREEITEM hParent, int nRpIdx );
	HTREEITEM AddKbSetNode ( HTREEITEM hParent, int nKbsIdx );
	HTREEITEM AddLocSetNode ( HTREEITEM hParent, int nLsIdx );
	HTREEITEM AddTerminalNode ( HTREEITEM hParent, int nType, int nLocIdx, int nTerminalIdx );
#ifdef STOCKMAN_SYSTEM
	HTREEITEM AddSiteNode ( HTREEITEM hParent, int nSiteIdx );
	HTREEITEM AddStockpointNode ( HTREEITEM hParent, int nSpIdx, int nType );
#endif

	void GetDataForTreeItem ( HTREEITEM hItem );
	void GetLocalDataForTreeItem ( HTREEITEM hItem, CSelectMultipleEntityInfo& infoNode, bool bFirstSibling );
	void DeleteTreeNodes();
	void DeleteSubTreeNodes ( HTREEITEM hItem );
	void DeleteSubTreeNodesAndData ( HTREEITEM hItem );
	void ExpandTree();
	void ExpandSubTree ( HTREEITEM hItem );
	void RemoveLinks( HTREEITEM hItem, int nNodeType );

	//network tree
	void BuildNetworkTree();
	void BuildNetworkTreeNetwork( HTREEITEM hRoot, int nNetworkIdx );
	void BuildNetworkTreeLocation( HTREEITEM hNetworkNode, int nLocIdx );

	void SelectItemNetworkTree( int nNodeType );
	void Button1NetworkTree();
	void Button2NetworkTree();
	void Button3NetworkTree();
	void Button4NetworkTree();
	void AddNetwork();
	void EditNetwork();
	void DeleteNetwork();
	void AddLoc();
	void EditLoc( bool bShowTerminals );
	void DeleteLoc();
	void AddTerminals();
	void EditTerminal( bool bShowLocName );
	void DeleteTerminal();
	void CollapseNetworkTree();
	void CollapseNetworkTree( HTREEITEM hItem );

	//sports booker tree
	void BuildSportsBookerTree();
	void BuildSportsBookerTreeNetwork( HTREEITEM hRoot, int nNetworkIdx );
	void BuildSportsBookerTreeLocation( HTREEITEM hNetworkNode, int nLocIdx );

	void SelectItemSportsBookerTree( int nNodeType );
	void Button1SportsBookerTree();
	void Button2SportsBookerTree();
	void Button3SportsBookerTree();
	void Button4SportsBookerTree();
	void AddSportsBookerNetwork();
	void EditSportsBookerNetwork();
	void DeleteSportsBookerNetwork();
	void AddSportsBookerLoc();
	void EditSportsBookerLoc();
	void DeleteSportsBookerLoc();
	void CollapseSportsBookerTree();
	void CollapseSportsBookerTree( HTREEITEM hItem );

	//smart entertainment tree
	void BuildSmartEntertainmentTree();
	void BuildSmartEntertainmentTreeNetwork( HTREEITEM hRoot, int nNetworkIdx );
	void BuildSmartEntertainmentTreeLocation( HTREEITEM hNetworkNode, int nLocIdx );

	void SelectItemSmartEntertainmentTree( int nNodeType );
	void Button1SmartEntertainmentTree();
	void Button2SmartEntertainmentTree();
	void Button3SmartEntertainmentTree();
	void Button4SmartEntertainmentTree();
	void AddSmartEntertainmentNetwork();
	void EditSmartEntertainmentNetwork();
	void DeleteSmartEntertainmentNetwork();
	void AddSmartEntertainmentLoc();
	void EditSmartEntertainmentLoc();
	void DeleteSmartEntertainmentLoc();
	void CollapseSmartEntertainmentTree();
	void CollapseSmartEntertainmentTree( HTREEITEM hItem );
	
	//system tree
	void BuildSystemTree();
	void BuildSystemTreeDatabase( HTREEITEM hRoot, int nDbIdx );
	void BuildSystemTreeLocation( HTREEITEM hDbNode, int nLocIdx );

	void SelectItemSystemTree( int nNodeType );
	void Button1SystemTree();
	void Button2SystemTree();
	void Button3SystemTree();
	void AddDatabase();
	void EditDatabase();
	void DeleteDatabase();
	void AddLocsToDatabase();
	void RemoveLocsFromDatabase();
	void AddDatabaseLocNodes ( HTREEITEM hDbNode, int nDbNo );
	void CollapseSystemTree();
	void CollapseSystemTree( HTREEITEM hItem );
	const char* GetLocationDisplayName ( int nLocIdx );
	
	//stockpoint tree
#ifdef STOCKMAN_SYSTEM
	void BuildStockTree();
	void BuildStockTreeDatabase( HTREEITEM hRoot, int nDbIdx );
	HTREEITEM BuildStockTreeStockpoint( HTREEITEM hDbNode, int nDbIdx, int nSpIdx );
	HTREEITEM RebuildStockpointTree( int nDbIdx, HTREEITEM hItem, int nSpIdx );

	void SelectItemStockTree( int nNodeType );
	void Button1StockTree();
	void Button2StockTree();
	void Button3StockTree();
	void AddStockpoint();
	void EditStockpoint();
	void DeleteStockpoint();
	void SetupWarehouse();
	void AddTerminalsToStockpoint();
	void RemoveTerminalFromStockpoint();
	void AddStockpointTerminalNodes ( HTREEITEM hSpNode, int nSpNo, int nDbNo );
	void AddStockpointTerminalNodesLocation ( HTREEITEM hRpNode, int nSpNo, int nLocIdx );
	void CollapseStockTree();
	void CollapseStockTree( HTREEITEM hItem );
#endif

	//reportpoint tree
	void BuildRepPointTree();
	void BuildRepPointTreeDatabase( HTREEITEM hRoot, int nDbIdx );
	void BuildRepPointTreeRepPoint( HTREEITEM hDbNode, int nDbIdx, int nRpIdx );

	void SelectItemRepPointTree( int nNodeType );
	void Button1RepPointTree();
	void Button2RepPointTree();
	void Button3RepPointTree();
	void AddReppoint();
	void EditReppoint();
	void DeleteReppoint();
	void AddTerminalsToReppoint();
	void RemoveTerminalsFromReppoint();
	void AddReportpointTerminalNodes ( HTREEITEM hRpNode, int nRpNo, int nDbNo );
	void AddReportpointTerminalNodesLocation ( HTREEITEM hRpNode, int nRpNo, int nLocIdx );
	void CollapseRepPointTree();
	void CollapseRepPointTree( HTREEITEM hItem );

	//keyboard sets
	void BuildKbSetTree();
	void BuildKbSetTreeDatabase( HTREEITEM hRoot, int nDbIdx );
	void BuildKbSetTreeKbSet( HTREEITEM hDbNode, int nDbIdx, int nKbsIdx );

	void SelectItemKbSetTree( int nNodeType );
	void Button1KbSetTree();
	void Button2KbSetTree();
	void Button3KbSetTree();
	void AddKbSet();
	void EditKbSet();
	void DeleteKbSet();
	void AddLocationsToKbSet();
	void RemoveLocationsFromKbSet();
	void AddKbSetLocationNodes ( HTREEITEM hRpNode, int nKbsNo, int nDbNo );
	void CollapseKbSetTree();
	void CollapseKbSetTree( HTREEITEM hItem );

	//location set tree
	void BuildLocSetTree();
	void BuildLocSetTreeDatabase( HTREEITEM hDbNode, int nDbIdx );
	void BuildLocSetTreeLocSet( HTREEITEM hLsNode, int nDbIdx, int nLsIdx );

	void SelectItemLocSetTree( int nNodeType );
	void Button1LocSetTree();
	void Button2LocSetTree();
	void Button3LocSetTree();
	void AddLocSet();
	void EditLocSet();
	void DeleteLocSet();
	void AddLocationsToLocSet();
	void RemoveLocationsFromLocSet();
	void AddLocSetLocationNodes ( HTREEITEM hLsNode, int nLsNo, int nDbNo );
	void CollapseLocSetTree();
	void CollapseLocSetTree( HTREEITEM hItem );
	
	//site tree
#ifdef STOCKMAN_SYSTEM
	void BuildSiteTree();
	void BuildSiteTreeDatabase( HTREEITEM hRoot, int nDbIdx );
	void BuildSiteTreeSite( HTREEITEM hRoot, int nDbIdx, int nSiteIdx );

	void SelectItemSiteTree( int nNodeType );
	void Button1SiteTree();
	void Button2SiteTree();
	void Button3SiteTree();
	void AddSite();
	void EditSite();
	void DeleteSite();
	void AddStockpointsToSite();
	void RemoveStockpointsFromSite();
	void AddSiteStockpointNodes ( HTREEITEM hSiteNode, int nSiteNo, int nDbNo );
	void CollapseSiteTree();
	void CollapseSiteTree( HTREEITEM hItem );
#endif

private:
	void GetLocalDataForTreeItemInternal( HTREEITEM hItem, CSelectMultipleEntityFeeder& infoNode, bool bFirstSibling );

private:
	CImageList m_ImageList;
	CString m_strLocationDisplayName;
	int m_nTreeType;
	HTREEITEM m_hItem;
	int m_nNetworkNo;
	int m_nDbNo;
	int m_nSiteNo;
	int m_nSpNo;
	int m_nRpNo;
	int m_nKbsNo;
	int m_nLocNo;
	int m_nLsNo;
	int m_nTerminalIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
