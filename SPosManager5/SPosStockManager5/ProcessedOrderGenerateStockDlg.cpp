/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "ProcessedOrderPreviewDatabaseDlg.h"
#include "ProcessedOrderPreviewFinaliser.h"
#include "ProcessedOrderReportDlg.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "StockOptionsIni.h"
#include "StockTreeReportNewReorder.h"
/**********************************************************************/
#include "ProcessedOrderGenerateStockDlg.h"
/**********************************************************************/

CProcessedOrderGenerateStockDlg::CProcessedOrderGenerateStockDlg( CArray<int,int>& arrayEntityIdx, CWnd* pParent)
	: CSSDialog(CProcessedOrderGenerateStockDlg::IDD, pParent), m_arrayEntityIdx( arrayEntityIdx )
{
	m_Helpers.m_pListDatabase = &m_listDatabase;
	m_Helpers.m_pComboSupplier = &m_comboSupplier;
	m_Helpers.m_pListSupplier = &m_listSupplier;
	m_Helpers.m_pComboCategory = &m_comboCategory;
	m_Helpers.m_pCategorySelector = &m_CategorySelector;
	m_Helpers.m_pSuppSelectArray = &m_wSuppSelectArray;
	m_Helpers.m_pLocationSelector = &m_LocationSelector;
	m_Helpers.m_pComboConLevel = &m_comboConLevel;
	m_Helpers.m_pArrayEntityIdx = &m_arrayEntityIdx;
	m_Helpers.m_pArrayLocation = &m_arrayLocation;

	//{{AFX_DATA_INIT(CProcessedOrderGenerateStockDlg)
	m_nConSelect = -1;
	m_bConsolidate = FALSE;
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CProcessedOrderGenerateStockDlg::~CProcessedOrderGenerateStockDlg()
{
}

/**********************************************************************/

void CProcessedOrderGenerateStockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessedOrderGenerateStockDlg)
	DDX_Control(pDX, IDC_COMBO_SUPPLIER, m_comboSupplier);
	DDX_Control(pDX, IDC_BUTTON_ALL, m_buttonAll);
	DDX_Control(pDX, IDC_BUTTON_NONE, m_buttonNone);
	DDX_Control(pDX, IDC_STATIC_PREPARE1, m_staticPrepare1);
	DDX_Control(pDX, IDC_STATIC_PREPARE2, m_staticPrepare2);
	DDX_Control(pDX, IDC_COMBO_CONLEVEL, m_comboConLevel);
	DDX_Control(pDX, IDC_STATIC_PREPARE3, m_staticPrepare3);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Control(pDX, IDC_BUTTON_DISPLAY, m_buttonDisplay);
	DDX_Control(pDX, IDC_STATIC_ORDER1, m_staticOrder1);
	DDX_Control(pDX, IDC_COMBO_METHOD, m_comboMethod);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_CBIndex(pDX, IDC_COMBO_CONLEVEL, m_nConSelect);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CProcessedOrderGenerateStockDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessedOrderGenerateStockDlg)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LOCATION, OnSelectDatabase)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY, OnButtonDisplay)
	ON_CBN_SELCHANGE(IDC_COMBO_SUPPLIER, OnSelectSupplierSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CProcessedOrderGenerateStockDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strTitle = "Generate Orders from Minimum/Order Levels";
	m_Helpers.HandleInitDialog( this, strTitle );

	m_comboMethod.AddString( "Replenish stock for items below minimum level" );
	m_comboMethod.AddString( "Replenish stock and suggest zero level orders for other items" );
	m_comboMethod.AddString( "Suggest zero level orders for all items" );
	
	int nSel = StockOptions.GetOrderSuggestionMethod();
	if ( ( nSel < 0 ) || ( nSel > 2 ) )
		nSel = 0;
	m_comboMethod.SetCurSel( nSel );

	if ( m_arrayLocation.GetSize() == 1 )
	{
		CLocationSelectorEntity LocSelEntity = m_arrayLocation.GetAt(0);
		
		CString strTitle;
		strTitle.Format( "Generate Orders from Minimum/Reorder Levels (%s)", dbDatabase.GetName( LocSelEntity.GetDbIdx() ) );
		SetWindowText( strTitle );

		m_LocationSelector.ForceSelection( LocSelEntity );
		ShowAndEnableWindow( &m_listDatabase, FALSE );
		MoveControl( &m_comboSupplier, 7, 7 );
		MoveControl( &m_listSupplier, 7, 24 );
		MoveControl( &m_buttonAll, 7, 289 );
		MoveControl( &m_buttonNone, 61, 289 );
		MoveControl( &m_staticPrepare1, 186, 24 );
		MoveControl( &m_staticPrepare2, 198, 41 );
		MoveControl( &m_comboConLevel, 239, 40 );
		MoveControl( &m_buttonDisplay, 198, 95 );
		MoveControl( &m_staticPrepare3, 198, 60 );
		MoveControl( &m_comboCategory, 239, 59 );
		MoveControl( &m_staticOrder1, 186, 153 );
		MoveControl( &m_comboMethod, 198, 174 );
		MoveControl( &m_buttonOK, 198, 228 );
		MoveControl( &m_buttonCancel, 388, 289 );
		ResizeDialog( 445, 310 );
	}
	
	return TRUE; 
}

/**********************************************************************/

void CProcessedOrderGenerateStockDlg::OnSelectDatabase(NMHDR* pNMHDR, LRESULT* pResult) 
{
    m_Helpers.HandleSelectDatabase();
}

/**********************************************************************/

void CProcessedOrderGenerateStockDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   switch( nIDCtl )	
   {	
   case IDC_LIST_LOCATION:
	   m_listDatabase.AdjustRowHeight( lpMeasureItemStruct, 10 );
	   break;
	
   case IDC_LIST_SUPPLIER:	
	   m_listSupplier.AdjustRowHeight( lpMeasureItemStruct, 10 );	
	   break;	
   }
}

/**********************************************************************/

void CProcessedOrderGenerateStockDlg::OnButtonAll() 
{
	m_Helpers.SelectSuppliersAll();
}

/**********************************************************************/

void CProcessedOrderGenerateStockDlg::OnButtonNone() 
{
	m_Helpers.SelectSuppliersNone();
}


/**********************************************************************/

void CProcessedOrderGenerateStockDlg::BuildSupplierArray()
{
	m_Helpers.BuildSupplierArray();
}

/**********************************************************************/

void CProcessedOrderGenerateStockDlg::OnSelectSupplierSet() 
{
	m_Helpers.SelectSupplierSet();
}

/**********************************************************************/

void CProcessedOrderGenerateStockDlg::OnButtonDisplay() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	CProcessedOrderReportDlg dlgReport;
	dlgReport.m_bConsolidate = m_bConsolidate;
	dlgReport.m_bSort = m_bAlphaSort;
	if ( dlgReport.DoModal() != IDOK )
		return;

	m_bConsolidate = dlgReport.m_bConsolidate;
	m_bAlphaSort = dlgReport.m_bSort;

	BuildSupplierArray();
	int nCategoryFilter = m_comboCategory.GetItemData ( m_comboCategory.GetCurSel() );

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	
	CStockLevelTreeNew tree;
	CWordArray arraySortOrder;
	arraySortOrder.Add ( SH_FIELD_SUPP + SH_FIELD_HEADER + SH_FIELD_TOTAL );
	tree.Configure( ( m_bAlphaSort ? 1 : 0 ), NULL, LocSelEntity, 0, arraySortOrder, TRUE, nCategoryFilter, "", "", FALSE, FALSE, m_wSuppSelectArray, FALSE, FALSE, FALSE );
	tree.Consolidate();

	m_Helpers.OpenStockLevels();

	CStockTreeReportNewReorder ReorderReport( tree );
	ReorderReport.SetConLevelText( LocSelEntity.GetEntityName() );
	ReorderReport.SetSeparateStockpointsFlag ( ( FALSE == m_bConsolidate ) && ( LocSelEntity.GetConType() != NODE_STOCKPOINT ) );
	ReorderReport.SetSupplierNameFlag( FALSE );
	ReorderReport.SetSupplierRefFlag( FALSE );
	ReorderReport.SetDateFrom( "" );
	ReorderReport.SetDateTo( "" );
	ReorderReport.PrepareArrays();
	ReorderReport.PrepareReport();
	ReorderReport.CreateReport();
		
	CReportManager ReportManager;			
	ReportManager.DisplayReport ( Super.ReportFilename(), ReorderReport.GetReportParamsFilename(), ReorderReport.GetReportParamsKey(), this );
}

/**********************************************************************/

void CProcessedOrderGenerateStockDlg::GenerateFromStockLevels( CProcessedOrderPreviewCSVArray& OrderArray )
{
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	CDataManagerInfo infoDM;
	if ( m_Helpers.OpenDatabaseReadWrite( infoDM ) == TRUE )
	{
		StatusProgress.Lock( TRUE, "Generating order preview" );
		m_Helpers.GenerateOrder( m_comboMethod.GetCurSel(), OrderArray );
		StatusProgress.Unlock();
		m_Helpers.OpenDatabaseReadOnly();
		m_Helpers.UpdateDatabaseControls();
		m_LocationSelector.ForceSelection( LocSelEntity );
	}
	else
	{
		m_Helpers.OpenDatabaseReadOnly();
		m_Helpers.UpdateDatabaseControls();
		m_LocationSelector.ForceSelection( LocSelEntity );
		Prompter.ShareError( infoDM );
	}
}

/**********************************************************************/

void CProcessedOrderGenerateStockDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	CDataManagerInfo infoDM;
	if ( m_Helpers.OpenDatabaseReadWrite( infoDM ) == TRUE )
	{
		HandleOKInternal();
		m_Helpers.OpenDatabaseReadOnly();
		m_Helpers.UpdateDatabaseControls();
		m_LocationSelector.ForceSelection( LocSelEntity );
	}
	else
	{
		m_Helpers.OpenDatabaseReadOnly();
		m_Helpers.UpdateDatabaseControls();
		m_LocationSelector.ForceSelection( LocSelEntity );
		Prompter.ShareError( infoDM );
	}
}

/**********************************************************************/

void CProcessedOrderGenerateStockDlg::HandleOKInternal() 
{
	BuildSupplierArray();
	
	int nSpStartIdx, nSpEndIdx;
	dbStockpoint.GetSpIdxRange( m_Helpers.GetDbIdx(), nSpStartIdx, nSpEndIdx );
	if ( nSpEndIdx < nSpStartIdx )
		return;

	int nDefSp = dbStockpoint.GetSpNo( nSpStartIdx );
	
	//THIS ARRAY WILL HOLD THE ITEMS TO BE ORDERED
	CProcessedOrderPreviewCSVArray OrderArray;

	CString strTitle;
	CString strCategory = m_CategorySelector.GetCategoryText();

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	
	strTitle.Format ( "Order Preview (%s, %s)",
		LocSelEntity.GetEntityName(),
		( const char* ) strCategory );
	
	//NOTHING TO DO IF NO SUPPLIERS WERE SPECIFIED
	if ( m_wSuppSelectArray.GetSize() != 0 )
		GenerateFromStockLevels ( OrderArray );
		
	CProcessedOrderPreviewDatabaseDlg dlgPreview ( strTitle, OrderArray, LocSelEntity, this );

	bool bGotOrder = ( dlgPreview.DoModal() == IDOK );

	if ( TRUE == bGotOrder )
	{	
		for ( int n = OrderArray.GetSize() - 1; n >= 0; n-- )
		{
			CProcessedOrderPreviewCSVRecord OrderItem;
			OrderArray.GetAt( n, OrderItem );

			if ( OrderItem.HasOrder( LocSelEntity ) == FALSE )
				OrderArray.RemoveAt( n );
		}

		bGotOrder = ( OrderArray.GetSize() > 0 );
	}

	if ( FALSE == bGotOrder )
	{
		Prompter.Info ( "No order lines were generated" );
		return;
	}

	CProcessedOrderPreviewFinaliser Finaliser( this, LocSelEntity, "", "" );

	if ( StockOptions.GetOrderSeparationType() == ORDER_DUSEPARATE_ORDER )
		Finaliser.CreateStockpointOrders( OrderArray );
	else
		Finaliser.CreateDatabaseOrders( OrderArray );
}

/**********************************************************************/

void CProcessedOrderGenerateStockDlg::OnCancel() 
{
	EndDialog ( IDCANCEL );
}

/**********************************************************************/
