/**********************************************************************/
#include "EditStockTypes.h"
#include "FilterTypes.h"
#include "PluStockActionDlg.h"
#include "PluStockItemDlg.h"
#include "PropertySheetStock.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "PropPagePluRecipe.h"
/**********************************************************************/

CPropPagePluRecipe::CPropPagePluRecipe() : CPropertyPage(CPropPagePluRecipe::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePluRecipe)
	//}}AFX_DATA_INIT
	m_nEditMode = EDIT_STOCK_SALEQTY;
	m_bReadOnly = FALSE;
	m_bApparentTab = TRUE;
	m_bStockLevelTab = TRUE;
}

/**********************************************************************/

CPropPagePluRecipe::~CPropPagePluRecipe()
{
}

/**********************************************************************/

void CPropPagePluRecipe::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePluRecipe)
	DDX_Control(pDX, IDC_BUTTON_ADD, m_buttonAdd);
	DDX_Control(pDX, IDC_BUTTON_EDIT_QUANTITY, m_buttonEditQuantity);
	DDX_Control(pDX, IDC_BUTTON_EDIT_SETTINGS, m_buttonEditSettings);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_buttonRemove);
	DDX_Control(pDX, IDC_BUTTON_STOCK, m_buttonStock);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePluRecipe, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePluRecipe)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_QUANTITY, OnButtonEditQuantity)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_SETTINGS, OnButtonEditSettings)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_STOCK, OnButtonStock)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePluRecipe::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();
	m_listItems.SubclassDlgItem ( IDC_LIST, this );
	m_listItems.InsertColumn ( 0, "Stock Code", LVCFMT_LEFT, 120 );
	m_listItems.InsertColumn ( 1, "Description", LVCFMT_LEFT, 240 );
	m_listItems.InsertColumn ( 2, "Category", LVCFMT_LEFT, 200 );
	m_listItems.InsertColumn ( 3, "Unit", LVCFMT_LEFT, 160 );
	m_listItems.InsertColumn ( 4, "Qty", LVCFMT_RIGHT, 100 );
	m_listItems.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	AddList();
	SetButtons();
	return TRUE;  
}

/**********************************************************************/

void CPropPagePluRecipe::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
   {
	   m_listItems.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

BOOL CPropPagePluRecipe::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePluRecipe::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	AddList();
	SetButtons();
	return bResult;
}

/**********************************************************************/

void CPropPagePluRecipe::OnButtonAdd() 
{
	CRecipeCSVRecord Recipe;
	CPluStockItemDlg dlg( Recipe, "Add Stock Item", TRUE, FALSE, m_bApparentTab, this );

	if ( dlg.DoModal() != IDOK )
		return;

	if ( Recipe.GetSize() == 0 )
		return;

	CString strStockCode = Recipe.GetStockCode(0);
	if ( strStockCode == "" )
		return;

	int nIndex;
	if ( m_pRecipeRecord -> FindStockCode ( strStockCode, nIndex ) == TRUE )
	{
		m_pRecipeRecord -> SetStockQty( nIndex, Recipe.GetStockQty(0) );
		AddList();
		SelectRecipeItem( nIndex );
	}
	else
	{
		int nStockIdx = 0;
		if ( ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == TRUE ) || ( CreateStockItem ( strStockCode ) == TRUE ) )
		{
			m_pRecipeRecord -> AddStockCode ( Recipe.GetStockCode(0), Recipe.GetStockQty(0) );
			AddList();
			SelectRecipeItem ( m_pRecipeRecord -> GetSize() - 1 );
		}	
	}
}

/**********************************************************************/

void CPropPagePluRecipe::OnButtonEditQuantity() 
{
	CRecipeListItem ListItem;
	GetSelectedItem( ListItem );

	if ( RECIPE_LISTITEM_INDEX != ListItem.m_nType )
		return;

	if ( ( ListItem.m_nIndex < 0 ) || ( ListItem.m_nIndex >= m_pRecipeRecord -> GetSize() ) )
		return;

	CRecipeCSVRecord Recipe;
	Recipe.AddStockCode( m_pRecipeRecord -> GetStockCode( ListItem.m_nIndex ), m_pRecipeRecord -> GetStockQty( ListItem.m_nIndex ) );

	CPluStockItemDlg dlg( Recipe, "Edit Stock Details", FALSE, FALSE, m_bApparentTab, this );

	if ( ( dlg.DoModal() == IDOK ) && ( Recipe.GetSize() != 0 ) )
	{
		m_pRecipeRecord -> SetStockCode ( ListItem.m_nIndex, Recipe.GetStockCode(0) ); 
		m_pRecipeRecord -> SetStockQty ( ListItem.m_nIndex, Recipe.GetStockQty(0) );
		AddList();
		SelectRecipeItem( ListItem.m_nIndex );
	}
}

/**********************************************************************/

void CPropPagePluRecipe::OnButtonEditSettings() 
{
	if ( m_pRecipeRecord -> GetSize() != 0 )
	{
		CRecipeListItem ListItem;
		GetSelectedItem( ListItem );

		CRecipeCSVRecord Recipe;
		Recipe.SetSpNo( m_pRecipeRecord -> GetSpNo() );
		Recipe.SetReverseSaleFlag( m_pRecipeRecord -> GetReverseSaleFlag() );

		CPluStockActionDlg dlg( Recipe, this );

		if ( dlg.DoModal() == IDOK )
		{
			m_pRecipeRecord -> SetSpNo( Recipe.GetSpNo() ); 
			m_pRecipeRecord -> SetReverseSaleFlag( Recipe.GetReverseSaleFlag() );
			AddList();
			SelectListItem( ListItem );
		}
	}
}

/**********************************************************************/

void CPropPagePluRecipe::OnButtonRemove() 
{
	CRecipeListItem ListItem;
	GetSelectedItem( ListItem );

	if ( RECIPE_LISTITEM_INDEX != ListItem.m_nType )
		return;

	if ( ( ListItem.m_nIndex < 0 ) || ( ListItem.m_nIndex >= m_pRecipeRecord -> GetSize() ) )
		return;

	if ( Prompter.YesNo ( "Are you sure you wish to remove the link\nbetween this stock item and this plu." ) == IDYES )
	{
		m_pRecipeRecord -> RemoveItem ( ListItem.m_nIndex );
		AddList();

		if ( ListItem.m_nIndex <= m_pRecipeRecord -> GetSize() - 1 )
			SelectRecipeItem ( ListItem.m_nIndex );
		else
			SelectRecipeItem ( m_pRecipeRecord -> GetSize() - 1 );
	}
}

/**********************************************************************/

void CPropPagePluRecipe::Refresh()
{
	GetRecordData();
	AddList();
	SetButtons();
	UpdateData ( FALSE );
}

/**********************************************************************/

bool CPropPagePluRecipe::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPagePluRecipe::SaveRecord()
{
	if ( m_bReadOnly == TRUE )
		return;
	
	if ( m_pRecipeRecord -> GetPluStockLinkFlag() == TRUE )
		DataManager.Plu.UpdatePluPrice ( *m_pPluRecord, DataManager.Plu.GetRecipeCost ( *m_pRecipeRecord ), FALSE );
	else
		DataManager.Plu.UpdatePluPrice ( *m_pPluRecord, m_pPluRecord -> GetPurchaseCost(), FALSE );
}

/**********************************************************************/

void CPropPagePluRecipe::GetRecordData()
{
}

/**********************************************************************/

void CPropPagePluRecipe::SelectRecipeItem( int nIndex )
{
	CRecipeListItem Target;
	Target.m_nType = RECIPE_LISTITEM_INDEX;
	Target.m_nIndex = nIndex;
	SelectListItem( Target );
}

/**********************************************************************/

void CPropPagePluRecipe::SelectListItem( CRecipeListItem& Target )
{
	if ( m_arrayListItems.GetSize() > 0 )
	{
		int nSel = 0;
		for ( int nPos = 0; nPos < m_arrayListItems.GetSize(); nPos++ )
		{
			CRecipeListItem ListItem = m_arrayListItems[nPos];
			if ( ( Target.m_nType == ListItem.m_nType ) && ( Target.m_nIndex == ListItem.m_nIndex ) )
			{
				nSel = nPos;
				break;
			}
		}

		m_listItems.SetCurSel(nSel);
	}
}

/**********************************************************************/

void CPropPagePluRecipe::GetSelectedItem( CRecipeListItem& Item )
{
	Item.m_nType = -1;
	Item.m_nIndex = -1;
	
	int nSel = m_listItems.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_arrayListItems.GetSize() ) )
		Item = m_arrayListItems.GetAt( nSel );
}

/**********************************************************************/

void CPropPagePluRecipe::AddList()
{
	m_arrayListItems.RemoveAll();
	m_listItems.DeleteAllItems();

	CRecipeListItem ListItem;
	ListItem.m_nType = RECIPE_LISTITEM_INFO;
	ListItem.m_nIndex = 0;

	if ( m_pRecipeRecord -> GetSize() != 0 )
	{
		if ( StockOptions.GetPluRedirectToStockpointFlag() == TRUE )
		{
			int nSpNo = m_pRecipeRecord -> GetSpNo();
			if ( nSpNo  != 0 )
			{
				int nSpIdx;
				int nDbNo = dbDatabase.GetDbNo( DataManager.GetActiveDbIdx() );
				if ( dbStockpoint.FindStockpointByNumber( nDbNo, nSpNo, nSpIdx ) == TRUE )
				{
					CString strInfo;
					strInfo.Format( "Assign sales to %s", dbStockpoint.GetName( nSpIdx ) );
				
					CCSV csv ( '\t' );
					csv.Add ( "" );
					csv.Add ( strInfo );
					m_listItems.AddString ( csv.GetLine() );
					m_arrayListItems.Add( ListItem );
				}
			}
		}

		if ( m_pRecipeRecord -> GetReverseSaleFlag() == TRUE )
		{
			CCSV csv ( '\t' );
			csv.Add ( "" );
			csv.Add ( "Sales increase apparent stock" );
			m_listItems.AddString ( csv.GetLine() );
			m_arrayListItems.Add( ListItem );
		}

		if ( m_arrayListItems.GetSize() != 0 )
		{
			ListItem.m_nType = RECIPE_LISTITEM_BLANK;
			m_listItems.AddString ( "" );
			m_arrayListItems.Add( ListItem );
		}
	}

	ListItem.m_nType = RECIPE_LISTITEM_INDEX;
	for ( int nIndex = 0; nIndex < m_pRecipeRecord -> GetSize(); nIndex++ )
	{
		CString strStockCode = m_pRecipeRecord -> GetStockCode ( nIndex );
		CString strDescription = "Unknown";
		CString strStockUnit = "Unknown";
		CString strCategory = "Unknown";
		CString strQty = "";

		double dQty = m_pRecipeRecord -> GetStockQty ( nIndex );

		int nStockIdx = 0;
		if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == TRUE )
		{
			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt ( nStockIdx, StockRecord );
			strDescription = StockRecord.GetDescription();
			strStockUnit = DataManager.Stock.GetStockSubUnitNameForPluEdit( StockRecord );
			strCategory = DataManager.Category.GetText ( StockRecord.GetCategory() );
			strQty = DataManager.Stock.GetStockLevelStringForPluEdit ( StockRecord, dQty, 5 );
		}
		else
			strQty.Format ( "%.5f", dQty );

		CCSV csv ( '\t' );
		csv.Add ( strStockCode );
		csv.Add ( strDescription );
		csv.Add ( strCategory );
		csv.Add ( strStockUnit );
		csv.Add ( strQty );
		m_listItems.AddString ( csv.GetLine() );

		ListItem.m_nIndex = nIndex;
		m_arrayListItems.Add( ListItem );
	}

	SelectRecipeItem(0);
	
	SetButtons();
}

/**********************************************************************/

void CPropPagePluRecipe::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if ( m_bReadOnly == FALSE )
	{
		CRecipeListItem ListItem;
		GetSelectedItem( ListItem );

		switch( ListItem.m_nType )
		{
		case RECIPE_LISTITEM_INDEX:
			OnButtonEditQuantity();
			break;

		case RECIPE_LISTITEM_INFO:
			OnButtonEditSettings();
			break;
		}
	}

	*pResult = 0;
}

/**********************************************************************/

void CPropPagePluRecipe::OnButtonStock() 
{
	if ( m_pRecipeRecord -> GetSize() == 0 )
	{
		if ( m_pRecipeRecord -> GetPluStockLinkFlag() == TRUE )
			m_pRecipeRecord -> DisablePluStockLink();
		else
			m_pRecipeRecord -> EnablePluStockLink();

		SetButtons();
	}
	else
	{
		CRecipeListItem ListItem;
		GetSelectedItem( ListItem );

		if ( RECIPE_LISTITEM_INDEX != ListItem.m_nType )
			return;

		if ( ( ListItem.m_nIndex < 0 ) || ( ListItem.m_nIndex >= m_pRecipeRecord -> GetSize() ) )
			return;

		CString strStockCode = m_pRecipeRecord -> GetStockCode ( ListItem.m_nIndex );

		int nStockIdx = 0;
		if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == TRUE )
		{
			m_pPluChangesBuffer -> LogStockChange ( strStockCode );

			CStockFilterArray FilterArray ( FILTERTYPE_DATABASE );
			FilterArray.SetArrayIdx ( nStockIdx );
			
			CPropertySheetStockInfo info;
			info.m_bEnableSpin = FALSE;
			info.m_wSpinID = IDC_SPIN;
			info.m_pStockFilter = &FilterArray;
			info.m_bPluLinkTab = FALSE;
			info.m_bSupplierEdit = TRUE;
			info.m_nDefaultSuppNo = 0;
			info.m_bReadOnly = m_bReadOnly;
			info.m_bStockLevelTab = m_bStockLevelTab;
			info.m_bApparentTab = m_bApparentTab;
			
			CPropertySheetStock propSheet ( info, this );
			
			if ( propSheet.DoModal() != IDOK )
				propSheet.UndoChanges();
		}
		else
			CreateStockItem ( strStockCode );
		
		AddList();
		SelectListItem ( ListItem );
	}
}

/**********************************************************************/

bool CPropPagePluRecipe::CreateStockItem ( const char* szCode )
{
	bool bResult= FALSE;

	CString strMsg = "There is no item with the stock code\n";
	strMsg += szCode;
	strMsg += " in the stock database.\n\n";
	strMsg += "Would you like to create this item now?";
			
	if ( Prompter.YesNo ( strMsg ) == IDYES )
	{
		bResult = TRUE;

		CStockCSVRecord StockRecord;
		StockRecord.SetStockCode ( szCode );
		StockRecord.SetDescription ( m_pPluRecord -> GetRepText() );
		StockRecord.SetCategory ( m_pPluRecord -> GetBaseDeptNo() );
				
		m_pPluChangesBuffer -> LogStockChange ( StockRecord.GetStockCode() );

		int nStockIdx;
		DataManager.Stock.InsertRecord ( StockRecord, nStockIdx );
			
		CStockFilterArray FilterArray ( FILTERTYPE_DATABASE );
		FilterArray.SetArrayIdx ( nStockIdx );
			
		CPropertySheetStockInfo info;
		info.m_bEnableSpin = FALSE;
		info.m_wSpinID = IDC_SPIN;
		info.m_pStockFilter = &FilterArray;
		info.m_bPluLinkTab = FALSE;
		info.m_bSupplierEdit = TRUE;
		info.m_nDefaultSuppNo = 0;
		info.m_bReadOnly = m_bReadOnly;
		info.m_bStockLevelTab = m_bStockLevelTab;
		info.m_bApparentTab = m_bApparentTab;
		
		CPropertySheetStock propSheet ( info, this );
		
		if ( propSheet.DoModal() != IDOK )
			propSheet.UndoChanges();
	}

	return bResult;
}

/**********************************************************************/

void CPropPagePluRecipe::SetButtons()
{
	bool bShowAdd = FALSE;
	bool bShowEdit = FALSE;
	bool bShowRemove = FALSE;
	bool bShowStock = ( EDIT_STOCK_FULL == m_nEditMode );
	CString strStockText = "View\nStock";

	if ( FALSE == m_bReadOnly )
	{
		switch( m_nEditMode )
		{
		case EDIT_STOCK_FULL:
			bShowAdd = TRUE;
			bShowEdit = TRUE;
			bShowRemove = TRUE;
			bShowStock = TRUE;
			break;

		case EDIT_STOCK_SALEQTY:
		default:
			bShowAdd = FALSE;
			bShowEdit = TRUE;
			bShowRemove = FALSE;
			bShowStock = FALSE;
			break;
		}

		strStockText = "Edit\nStock";

		if ( EDIT_STOCK_FULL == m_nEditMode )
		{
			if ( m_pRecipeRecord -> GetSize() == 0 )
			{
				if ( m_pRecipeRecord -> GetPluStockLinkFlag() == TRUE )
				{
					strStockText = "Make Non\nStock Item";
				}
				else
				{
					bShowAdd = FALSE;
					bShowEdit = FALSE;
					bShowRemove = FALSE;
					strStockText = "Allow Stock\nControl";
				}
			}
		}
	}

	m_buttonAdd.ShowWindow ( ( bShowAdd ) ? SW_SHOW : SW_HIDE );
	m_buttonEditQuantity.ShowWindow ( ( bShowEdit ) ? SW_SHOW : SW_HIDE );
	m_buttonEditSettings.ShowWindow ( ( bShowEdit ) ? SW_SHOW : SW_HIDE );
	m_buttonRemove.ShowWindow ( ( bShowRemove ) ? SW_SHOW : SW_HIDE );
	m_buttonStock.ShowWindow ( ( bShowStock ) ? SW_SHOW : SW_HIDE );
	
	m_buttonAdd.EnableWindow ( bShowAdd );
	m_buttonEditQuantity.EnableWindow ( bShowEdit );
	m_buttonEditSettings.EnableWindow ( bShowEdit );
	m_buttonRemove.EnableWindow ( bShowRemove );
	m_buttonStock.EnableWindow ( bShowStock );

	m_buttonStock.SetWindowText( strStockText );
}

/**********************************************************************/
