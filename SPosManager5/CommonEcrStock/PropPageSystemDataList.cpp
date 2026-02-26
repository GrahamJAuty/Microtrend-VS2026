/**********************************************************************/
#include "CloneDatabaseDlg.h"
#include "DatabaseCSVArray.h"
#include "KeyboardSetCSVArray.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "Password.h"
#include "PropertySheetSystemDataList.h"
#include "..\SPosStockManager5\StockpointCSVArray.h"
#include "TreeNode.h"
#include "..\SPosStockManager5\UnitechBatchJobDlg.h"
/**********************************************************************/
#include "PropPageSystemDataList.h"
/**********************************************************************/

CPropPageSystemDataList::CPropPageSystemDataList() : CSSPropertyPage(CPropPageSystemDataList::IDD)
{
	//{{AFX_DATA_INIT(CPropPageSystemDataNew)
	//}}AFX_DATA_INIT
	m_nDataPageType = DATAPAGE_TYPE_ECRDATA;
	m_pPropSheet = NULL;
	m_pHelpers = NULL;
	m_bCanCloneDatabase = FALSE;
}

/**********************************************************************/

void CPropPageSystemDataList::SetDataPageType( int nType )
{
	m_nDataPageType = nType;
	m_Filter.SetAllToHide();
	
	switch ( m_nDataPageType )
	{
	case DATAPAGE_TYPE_ECRDATA:			PrepareEcrDataPage();			break;
	case DATAPAGE_TYPE_PRICESET:		PreparePriceSetPage();			break;
	case DATAPAGE_TYPE_MIXMATCH:		PrepareMixMatchPage();			break;
	case DATAPAGE_TYPE_KEYBOARD:		PrepareKeyboardPage();			break;
	case DATAPAGE_TYPE_ECRTEXT:			PrepareEcrTextPage();			break;
	case DATAPAGE_TYPE_BILLTEXT:		PrepareBillTextPage();			break;
	case DATAPAGE_TYPE_FUNCTION:		PrepareFunctionPage();			break;
	case DATAPAGE_TYPE_OPGROUP:			PrepareOPGroupPage();			break;
	case DATAPAGE_TYPE_TABLES:			PrepareTablesPage();			break;
	case DATAPAGE_TYPE_MACRO:			PrepareMacroPage();				break;
	case DATAPAGE_TYPE_REASON:			PrepareReasonPage();				break;
	case DATAPAGE_TYPE_REPORTSET:		PrepareReportSetPage();			break;
	case DATAPAGE_TYPE_SERVER:			PrepareServerPage();			break;
	case DATAPAGE_TYPE_SVRPRESET:		PrepareServerPresetPage();		break;
	case DATAPAGE_TYPE_CUSTOMER:		PrepareCustomerPage();			break;
#ifdef STOCKMAN_SYSTEM
	case DATAPAGE_TYPE_STOCKDATA:		PrepareStockDataPage();			break;
	case DATAPAGE_TYPE_STOCKTEXT:		PrepareStockTextPage();			break;
#endif
	}
}
	
/**********************************************************************/

bool CPropPageSystemDataList::IsRelevant( int nAccessType )
{
	bool bRelevant = FALSE;

	switch( m_nDataPageType )
	{
	case DATAPAGE_TYPE_KEYBOARD:
		if ( PasswordArray.GetEnable( PasswordTicks::EditKeyboards ) )
		{
			if ( EcrmanOptions.GetFeaturesKeyboardFlag() )
			{
				for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
				{
					if ( PasswordArray.CheckLocationAccessLocIdx( nLocIdx ) == TRUE )
					{
						bRelevant = TRUE;
						break;
					}
				}
			}
		}
		break;

	case DATAPAGE_TYPE_SERVER:
		if ( PasswordArray.GetEnable( PasswordTicks::EditServerDetails ) || PasswordArray.GetEnable ( PasswordTicks::EditServerNames ) )
		{
			if ( EcrmanOptions.GetFeaturesServerSettingsType() != SERVER_NONE )
			{
				switch( PasswordArray.GetLocationAccessType() )
				{
				case LOCATION_ACCESS_SYSTEM:
					bRelevant = ( EcrmanOptions.GetFeaturesServerSettingsType() != SERVER_NONE );
					break;

				case LOCATION_ACCESS_DB:
					bRelevant = ( EcrmanOptions.GetFeaturesServerSettingsType() != SERVER_SYS );
					break;

				case LOCATION_ACCESS_LOC:
					bRelevant = ( EcrmanOptions.GetFeaturesServerSettingsType() != SERVER_SYS );
					break;
				}
			}
		}
		break;

	case DATAPAGE_TYPE_SVRPRESET:
		if ( PasswordArray.GetEnable( PasswordTicks::EditServerDetails ) || PasswordArray.GetEnable ( PasswordTicks::EditServerNames ) )
			if ( EcrmanOptions.GetFeaturesServerSettingsType() != SERVER_NONE )
				if ( EcrmanOptions.GetFeaturesServerPresetFlag() == TRUE )
					bRelevant = PasswordArray.GetEnable( PasswordTicks::EditServerPresets );
		break;

	case DATAPAGE_TYPE_BILLTEXT:
		bRelevant = ( LOCATION_ACCESS_SYSTEM == nAccessType );
		bRelevant &= PasswordArray.GetEnable( PasswordTicks::SetupMenuBillTexts );
		break;

	case DATAPAGE_TYPE_FUNCTION:
		bRelevant = ( LOCATION_ACCESS_SYSTEM == nAccessType );
		bRelevant &= PasswordArray.GetEnable( PasswordTicks::SetupMenuFunction );
		break;

	case DATAPAGE_TYPE_OPGROUP:
		bRelevant = ( LOCATION_ACCESS_SYSTEM == nAccessType );
		bRelevant &= PasswordArray.GetEnable( PasswordTicks::SetupMenuOPGroup );
		break;

	case DATAPAGE_TYPE_TABLES:
		bRelevant = PasswordArray.GetEnable( PasswordTicks::SetupMenuTableNames );
		break;

	case DATAPAGE_TYPE_MACRO:
		bRelevant = PasswordArray.GetEnable( PasswordTicks::SetupMenuMacros );
		break;

	case DATAPAGE_TYPE_REASON:
		bRelevant = PasswordArray.GetEnable(PasswordTicks::SetupMenuReasons);
		break;

	default:
		switch( nAccessType )
		{
		case LOCATION_ACCESS_SYSTEM:
			bRelevant = ( GotLocalData() || GotGlobalData() || GotStockpointData() );
			break;

		case LOCATION_ACCESS_DB:
		case LOCATION_ACCESS_LOC:
			bRelevant = ( GotLocalData() || GotStockpointData() );
			break;
		}
		break;
	}

	return bRelevant;
}

/**********************************************************************/

void CPropPageSystemDataList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageSystemDataNew)
	DDX_Control(pDX, IDC_BUTTON_ACTION1, m_buttonAction1);
	DDX_Control(pDX, IDC_BUTTON_ACTION2, m_buttonAction2);
	DDX_Control(pDX, IDC_BUTTON_ACTION3, m_buttonAction3);
	DDX_Control(pDX, IDC_BUTTON_ACTION4, m_buttonAction4);
	DDX_Control(pDX, IDC_BUTTON_ACTION5, m_buttonAction5);
	DDX_Control(pDX, IDC_BUTTON_ACTION6, m_buttonAction6);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageSystemDataList, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageSystemDataList)
	ON_BN_CLICKED(IDC_BUTTON_ACTION1, OnButtonAction1)
	ON_BN_CLICKED(IDC_BUTTON_ACTION2, OnButtonAction2)
	ON_BN_CLICKED(IDC_BUTTON_ACTION3, OnButtonAction3)
	ON_BN_CLICKED(IDC_BUTTON_ACTION4, OnButtonAction4)
	ON_BN_CLICKED(IDC_BUTTON_ACTION5, OnButtonAction5)
	ON_BN_CLICKED(IDC_BUTTON_ACTION6, OnButtonAction6)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LOCATION, OnSelectLocation)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ACTION, OnSelectAction)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ACTION, OnDoubleClickAction)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageSystemDataList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_listLocation.SubclassDlgItem( IDC_LIST_LOCATION, this );
	m_listLocation.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listLocation.InsertColumn ( 0, "", LVCFMT_LEFT, 220 );

	m_listAction.SubclassDlgItem( IDC_LIST_ACTION, this );
	m_listAction.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listAction.InsertColumn ( 0, "", LVCFMT_LEFT, 220 );

	BuildList();
	SelectLine( m_listLocation, 0 );

	return TRUE;  
}

/**********************************************************************/

void CPropPageSystemDataList::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	switch( nIDCtl )
	{
	case IDC_LIST_ACTION:
		m_listAction.AdjustRowHeight( lpMeasureItemStruct, 10 );
		break;

	case IDC_LIST_LOCATION:
		m_listLocation.AdjustRowHeight( lpMeasureItemStruct, 10 );
		break;
	}
}

/**********************************************************************/

void CPropPageSystemDataList::OnDoubleClickAction(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnButtonAction1();
	*pResult = 0;
}

/**********************************************************************/

void CPropPageSystemDataList::SelectLine ( CSSListCtrl& list, int nIndex )
{
	list.Invalidate();

	for ( int n = 0; n < list.GetItemCount(); n++ )
	{
		if ( n != nIndex )
			list.SetItemState(n, 0, LVIS_SELECTED | LVIS_FOCUSED );
		else
		{
			list.SetItemState(n, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED );
			list.SetSelectionMark(n);
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::AddGlobal()
{
	m_listLocation.InsertItem ( 0, "" );
	m_listLocation.SetItem ( 0, 0, LVIF_TEXT, "Global Data", 0, 0, 0, NULL );
		
	CLocationSelectorEntity LocSelEntity;
	LocSelEntity.SetSystemNode();
	m_arrayLocation.InsertAt( 0, LocSelEntity );	
}

/**********************************************************************/

void CPropPageSystemDataList::AddSystemFilter()
{
	m_listLocation.InsertItem ( 0, "" );
	m_listLocation.SetItem ( 0, 0, LVIF_TEXT, "System Reports", 0, 0, 0, NULL );
		
	CLocationSelectorEntity LocSelEntity;
	LocSelEntity.SetSystemFilterNode();
	m_arrayLocation.InsertAt( 0, LocSelEntity );	
}

/**********************************************************************/

void CPropPageSystemDataList::AddTimeSetting()
{
	m_listLocation.InsertItem ( 0, "" );
	m_listLocation.SetItem ( 0, 0, LVIF_TEXT, "Time Settings", 0, 0, 0, NULL );
		
	CLocationSelectorEntity LocSelEntity;
	LocSelEntity.SetTimeSettingNode();
	m_arrayLocation.InsertAt( 0, LocSelEntity );	
}

/**********************************************************************/

void CPropPageSystemDataList::AddReportList()
{
	m_listLocation.InsertItem ( 0, "" );
	m_listLocation.SetItem ( 0, 0, LVIF_TEXT, "Report Selection", 0, 0, 0, NULL );
		
	CLocationSelectorEntity LocSelEntity;
	LocSelEntity.SetReportListNode();
	m_arrayLocation.InsertAt( 0, LocSelEntity );	
}

/**********************************************************************/

void CPropPageSystemDataList::AddDatabase( int nDbIdx )
{
	int nListPos = m_arrayLocation.GetSize();
	m_listLocation.InsertItem ( nListPos, "" );
	m_listLocation.SetItem ( nListPos, 0, LVIF_TEXT, dbDatabase.GetName( nDbIdx ), 0, 0, 0, NULL );
		
	CLocationSelectorEntity LocSelEntity;
	LocSelEntity.SetDatabaseNode( nDbIdx );
	m_arrayLocation.Add( LocSelEntity );	
}

/**********************************************************************/

void CPropPageSystemDataList::AddLocation( int nLocIdx, bool bIndent )
{
	int nListPos = m_arrayLocation.GetSize();
	m_listLocation.InsertItem ( nListPos, "" );

	CString strName = ( bIndent ) ? "--> " : "";
	strName += dbLocation.GetName( nLocIdx );
	m_listLocation.SetItem ( nListPos, 0, LVIF_TEXT, strName, 0, 0, 0, NULL );
		
	CLocationSelectorEntity LocSelEntity;
	LocSelEntity.SetLocationNode( nLocIdx );
	m_arrayLocation.Add( LocSelEntity );	
}

/**********************************************************************/

void CPropPageSystemDataList::AddKeyboardMapLoc( int nMapIdx, int nLocIdx, bool bIndent )
{
	int nListPos = m_arrayLocation.GetSize();
	m_listLocation.InsertItem ( nListPos, "" );

	CString strName = ( TRUE == bIndent ) ? "--> " : "";
	strName += dbLocation.GetName( nLocIdx );
	m_listLocation.SetItem ( nListPos, 0, LVIF_TEXT, strName, 0, 0, 0, NULL );
		
	CLocationSelectorEntity LocSelEntity;
	LocSelEntity.SetKeyboardMapNode( nMapIdx );
	m_arrayLocation.Add( LocSelEntity );	
}

/**********************************************************************/

void CPropPageSystemDataList::AddKeyboardMapSet( int nMapIdx, int nSetIdx, bool bIndent )
{
	int nListPos = m_arrayLocation.GetSize();
	m_listLocation.InsertItem ( nListPos, "" );

	CString strName = ( TRUE == bIndent ) ? "--> " : "";
	strName += dbKeyboardSet.GetName( nSetIdx );
	m_listLocation.SetItem ( nListPos, 0, LVIF_TEXT, strName, 0, 0, 0, NULL );
		
	CLocationSelectorEntity LocSelEntity;
	LocSelEntity.SetKeyboardMapNode( nMapIdx );
	m_arrayLocation.Add( LocSelEntity );	
}

/**********************************************************************/

void CPropPageSystemDataList::AddStockpoint( int nSpIdx )
{
#ifdef STOCKMAN_SYSTEM
	int nListPos = m_arrayLocation.GetSize();
	m_listLocation.InsertItem ( nListPos, "" );

	CString strName = "--> ";
	strName += dbStockpoint.GetName( nSpIdx );
	m_listLocation.SetItem ( nListPos, 0, LVIF_TEXT, strName, 0, 0, 0, NULL );
		
	CLocationSelectorEntity LocSelEntity;
	LocSelEntity.SetStockpointNode( nSpIdx );
	m_arrayLocation.Add( LocSelEntity );	
#endif
}

/**********************************************************************/

void CPropPageSystemDataList::AddAction( int nNodeType, const char* szText )
{
	int nListPos = m_arrayAction.GetSize();
	m_listAction.InsertItem ( nListPos, "" );
	m_listAction.SetItem ( nListPos, 0, LVIF_TEXT, szText, 0, 0, 0, NULL );
	m_arrayAction.Add( nNodeType );
}

/**********************************************************************/

bool CPropPageSystemDataList::CheckCloneDatabase()
{
	CCloneDatabaseDlg dlg ( 0, m_Filter, this );
	m_bCanCloneDatabase = dlg.IsRelevant();
	m_bCanCloneDatabase &= ( PasswordArray.GetLocationAccessType() == LOCATION_ACCESS_SYSTEM );
	return m_bCanCloneDatabase;
}

/**********************************************************************/

void CPropPageSystemDataList::BuildList()
{
	switch ( m_nDataPageType )
	{
	case DATAPAGE_TYPE_ECRDATA:			BuildEcrDataList();			break;
	case DATAPAGE_TYPE_PRICESET:		BuildPriceSetList();		break;
	case DATAPAGE_TYPE_MIXMATCH:		BuildMixMatchList();		break;
	case DATAPAGE_TYPE_KEYBOARD:		BuildKeyboardList();		break;
	case DATAPAGE_TYPE_ECRTEXT:			BuildEcrTextList();			break;
	case DATAPAGE_TYPE_BILLTEXT:		BuildBillTextList();		break;
	case DATAPAGE_TYPE_FUNCTION:		BuildFunctionList();		break;
	case DATAPAGE_TYPE_OPGROUP:			BuildOPGroupList();			break;
	case DATAPAGE_TYPE_TABLES:			BuildTablesList();			break;
	case DATAPAGE_TYPE_MACRO:			BuildMacroList();			break;
	case DATAPAGE_TYPE_REASON:			BuildReasonList();			break;
	case DATAPAGE_TYPE_REPORTSET:		BuildReportSetList();		break;
	case DATAPAGE_TYPE_SERVER:			BuildServerList();			break;
	case DATAPAGE_TYPE_SVRPRESET:		BuildServerPresetList();	break;
	case DATAPAGE_TYPE_CUSTOMER:		BuildCustomerList();		break;
#ifdef STOCKMAN_SYSTEM
	case DATAPAGE_TYPE_STOCKDATA:		BuildStockDataList();		break;
	case DATAPAGE_TYPE_STOCKTEXT:		BuildStockTextList();		break;
#endif
	}
}

/**********************************************************************/

void CPropPageSystemDataList::OnSelectLocation(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if ((pNMListView->uChanged & LVIF_STATE) 
        && (pNMListView->uNewState & LVNI_SELECTED))
    {
		SelectLocation();
    }
}

/**********************************************************************/

void CPropPageSystemDataList::SelectLocation()
{
	switch ( m_nDataPageType )
	{
	case DATAPAGE_TYPE_ECRDATA:			SelectLocationEcrData();	break;
	case DATAPAGE_TYPE_PRICESET:		SelectLocationPriceSet();	break;
	case DATAPAGE_TYPE_MIXMATCH:		SelectLocationMixMatch();	break;
	case DATAPAGE_TYPE_KEYBOARD:		SelectLocationKeyboard();	break;
	case DATAPAGE_TYPE_ECRTEXT:			SelectLocationEcrText();	break;
	case DATAPAGE_TYPE_BILLTEXT:		SelectLocationBillText();	break;
	case DATAPAGE_TYPE_FUNCTION:		SelectLocationFunction();	break;
	case DATAPAGE_TYPE_OPGROUP:			SelectLocationOPGroup();	break;
	case DATAPAGE_TYPE_TABLES:			SelectLocationTables();		break;
	case DATAPAGE_TYPE_MACRO:			SelectLocationMacro();		break;
	case DATAPAGE_TYPE_REASON:			SelectLocationReason();		break;
	case DATAPAGE_TYPE_REPORTSET:		SelectLocationReportSet();	break;
	case DATAPAGE_TYPE_SERVER:			SelectLocationServer();		break;
	case DATAPAGE_TYPE_SVRPRESET:		SelectLocationServerPreset();break;
	case DATAPAGE_TYPE_CUSTOMER:		SelectLocationCustomer();	break;
#ifdef STOCKMAN_SYSTEM
	case DATAPAGE_TYPE_STOCKDATA:		SelectLocationStockData();	break;
	case DATAPAGE_TYPE_STOCKTEXT:		SelectLocationStockText();	break;
#endif
	}
}

/**********************************************************************/

void CPropPageSystemDataList::OnSelectAction(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if ((pNMListView->uChanged & LVIF_STATE) 
        && (pNMListView->uNewState & LVNI_SELECTED))
    {
		SelectAction();
    }
}

/**********************************************************************/

void CPropPageSystemDataList::SelectAction()
{
	switch ( m_nDataPageType )
	{
	case DATAPAGE_TYPE_ECRDATA:		SelectActionEcrData();	break;
	case DATAPAGE_TYPE_KEYBOARD:	SelectActionKeyboard();	break;
	}
}

/**********************************************************************/

void CPropPageSystemDataList::ForceSelectLocation( CLocationSelectorEntity& LocSelEntity )
{
	int nSel = 0;
	for ( int n = 0; n < m_arrayLocation.GetSize(); n++ )
	{
		CLocationSelectorEntity LocSelArray = m_arrayLocation.GetAt( n );

		if ( LocSelArray.SimpleCompare( LocSelEntity ) == TRUE )
		{
			nSel = n;
			break;
		}
	}

	SelectLine( m_listLocation, nSel );
	SelectLocation();
}

/**********************************************************************/

void CPropPageSystemDataList::ForceSelectAction( int nAction )
{
	int nSel = 0;
	for ( int n = 0; n < m_arrayAction.GetSize(); n++ )
	{
		if ( m_arrayAction.GetAt(n) == nAction )
		{
			nSel = n;
			break;
		}
	}

	SelectLine( m_listAction, nSel );
	SelectAction();
}

/**********************************************************************/

int CPropPageSystemDataList::GetDbIdx ( int nDbNo )
{
	int nDbIdx = 0;
	if ( ( nDbNo != 0 ) && ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) == FALSE ) )
		nDbIdx = -1;

	return nDbIdx;
}

/**********************************************************************/

void CPropPageSystemDataList::CloneDatabase ( int nDbNo )
{
	int nDbIdx = GetDbIdx ( nDbNo );
	if ( nDbIdx == -1 ) return;

	if ( DataManager.LockAllUsers() == FALSE )
		Prompter.LockError( "clone database files" );
	else
	{
		CCloneDatabaseDlg dlg ( nDbIdx, m_Filter, this );
		dlg.DoModal();

		DataManager.UnlockAllUsers();
	}
}

/**********************************************************************/

void CPropPageSystemDataList::OnButtonAction1() 
{
	if ( CheckDatabaseValidationToken() == TRUE )
	{
		m_pHelpers -> SetParentWnd( this );

		switch ( m_nDataPageType )
		{
		case DATAPAGE_TYPE_ECRDATA:		Button1EcrDataPage();				break;
		case DATAPAGE_TYPE_PRICESET:	Button1PriceSetPage();				break;
		case DATAPAGE_TYPE_MIXMATCH:	Button1MixMatchPage();				break;
		case DATAPAGE_TYPE_KEYBOARD:	Button1KeyboardPage();				break;
		case DATAPAGE_TYPE_ECRTEXT:		Button1EcrTextPage();				break;
		case DATAPAGE_TYPE_BILLTEXT:	Button1BillTextPage();				break;
		case DATAPAGE_TYPE_FUNCTION:	Button1FunctionPage();				break;
		case DATAPAGE_TYPE_REPORTSET:	Button1ReportSetPage();				break;
		case DATAPAGE_TYPE_SERVER:		Button1ServerPage();				break;
		case DATAPAGE_TYPE_SVRPRESET:	Button1ServerPresetPage();			break;
		case DATAPAGE_TYPE_OPGROUP:		Button1OPGroupPage();				break;
		case DATAPAGE_TYPE_TABLES:		Button1TablesPage();				break;
		case DATAPAGE_TYPE_MACRO:		Button1MacroPage();					break;
		case DATAPAGE_TYPE_REASON:		Button1ReasonPage();				break;
		case DATAPAGE_TYPE_CUSTOMER:	Button1CustomerPage();				break;
#ifdef STOCKMAN_SYSTEM
		case DATAPAGE_TYPE_STOCKDATA:	Button1StockDataPage();				break;
		case DATAPAGE_TYPE_STOCKTEXT:	Button1StockTextPage();				break;
#endif
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::OnButtonAction2() 
{
	if ( CheckDatabaseValidationToken() == TRUE )
	{
		m_pHelpers -> SetParentWnd( this );

		switch ( m_nDataPageType )
		{
		case DATAPAGE_TYPE_ECRDATA:		Button2EcrDataPage();		break;
		case DATAPAGE_TYPE_PRICESET:	Button2PriceSetPage();		break;
		case DATAPAGE_TYPE_MIXMATCH:	Button2MixMatchPage();		break;
		case DATAPAGE_TYPE_KEYBOARD:	Button2KeyboardPage();		break;
		case DATAPAGE_TYPE_ECRTEXT:		Button2EcrTextPage();		break;
		case DATAPAGE_TYPE_REPORTSET:	Button2ReportSetPage();		break;
		case DATAPAGE_TYPE_BILLTEXT:	Button2BillTextPage();		break;
		case DATAPAGE_TYPE_SVRPRESET:	Button2ServerPresetPage();	break;
		case DATAPAGE_TYPE_OPGROUP:		Button2OPGroupPage();		break;
#ifdef STOCKMAN_SYSTEM
		case DATAPAGE_TYPE_STOCKDATA:	Button2StockDataPage();		break;
		case DATAPAGE_TYPE_STOCKTEXT:	Button2StockTextPage();		break;
#endif
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::OnButtonAction3() 
{
	if ( CheckDatabaseValidationToken() == TRUE )
	{
		m_pHelpers -> SetParentWnd( this );

		switch ( m_nDataPageType )
		{
		case DATAPAGE_TYPE_PRICESET:	Button3PriceSetPage();		break;
		case DATAPAGE_TYPE_KEYBOARD:	Button3KeyboardPage();		break;
		case DATAPAGE_TYPE_BILLTEXT:	Button3BillTextPage();		break;
		case DATAPAGE_TYPE_FUNCTION:	Button3FunctionPage();		break;
		case DATAPAGE_TYPE_MACRO:		Button3MacroPage();			break;
		case DATAPAGE_TYPE_REASON:		Button3ReasonPage();		break;
		case DATAPAGE_TYPE_SVRPRESET:	Button3ServerPresetPage();	break;
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::OnButtonAction4() 
{
	if ( CheckDatabaseValidationToken() == TRUE )
	{
		m_pHelpers -> SetParentWnd( this );

		switch ( m_nDataPageType )
		{
		case DATAPAGE_TYPE_ECRDATA:		Button4EcrDataPage();		break;
		case DATAPAGE_TYPE_PRICESET:	Button4PriceSetPage();		break;
#ifdef STOCKMAN_SYSTEM
		case DATAPAGE_TYPE_STOCKDATA:	Button4StockDataPage();		break;
#endif
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::OnButtonAction5() 
{
	if ( CheckDatabaseValidationToken() == TRUE )
	{
		m_pHelpers -> SetParentWnd( this );

		switch ( m_nDataPageType )
		{
		case DATAPAGE_TYPE_ECRDATA:		Button5EcrDataPage();		break;
		case DATAPAGE_TYPE_PRICESET:	Button5PriceSetPage();		break;
		case DATAPAGE_TYPE_KEYBOARD:	Button5KeyboardPage();		break;
#ifdef STOCKMAN_SYSTEM
		case DATAPAGE_TYPE_STOCKDATA:	Button5StockDataPage();		break;
#endif
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::OnButtonAction6() 
{
	if ( CheckDatabaseValidationToken() == TRUE )
	{
		m_pHelpers -> SetParentWnd( this );

		switch ( m_nDataPageType )
		{
		case DATAPAGE_TYPE_KEYBOARD:	Button6KeyboardPage();		break;
		}
	}
}

/**********************************************************************/

bool CPropPageSystemDataList::CheckDatabaseValidationToken()
{
	if ( m_pPropSheet != NULL )
	{
		CFilenameUpdater FnUp( SysFiles::DataValidationQuit );
		if ( ::FileExists( FnUp.GetFilenameToUse() ) == TRUE )
		{
			CPropertySheetSystemDataList* pPropSheet = ( (CPropertySheetSystemDataList*) m_pPropSheet );
			pPropSheet -> SetDataValidationErrorFlag();
			pPropSheet -> EndDialog( IDCANCEL );
			return FALSE;
		}
	}

	return TRUE;
}

/**********************************************************************/