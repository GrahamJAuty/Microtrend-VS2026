/**********************************************************************/
#include "ListDataDelUnitDlg.h"
#include "ListDataSupplierDlg.h"
#include "MaxLengths.h"
#include "MessageLogger.h"
#include "PriceHelpers.h"
#include "StockOptionsIni.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPageStockSupplier.h"
/**********************************************************************/

CPropPageStockSupplier::CPropPageStockSupplier() : CSSPropertyPage(CPropPageStockSupplier::IDD)
{
	//{{AFX_DATA_INIT(CPropPageStockSupplier)
	//}}AFX_DATA_INIT
	m_bAllowChangeSupplier = FALSE;
	m_bAllowMakePreferred = FALSE;
	m_bMakePreferred = FALSE;
	m_pPluRecord = NULL;
	m_pLastSupplierTab = NULL;
	m_nDefaultSuppNo = 0;
	m_bReadOnly = FALSE;
	m_bInitialTabOnly = FALSE;
}

/**********************************************************************/

CPropPageStockSupplier::~CPropPageStockSupplier()
{
}

/**********************************************************************/

void CPropPageStockSupplier::SetSuppIdx ( int nIdx, int* pIdx, int nSuppNo )
{
	m_nSuppIdx = nIdx; 
	m_pLastSupplierTab = pIdx;
	m_nDefaultSuppNo = nSuppNo; 
}

/**********************************************************************/
	
void CPropPageStockSupplier::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageStockSupplier)
	DDX_Control( pDX, IDC_TAB_SUPPLIER, m_TabSupplier );
	DDX_Control(pDX, IDC_STATIC_SELECT, m_staticSelect);
	DDX_Control(pDX, IDC_STATIC_RETAIL, m_staticRetail);
	DDX_Control(pDX, IDC_BUTTON_SELECT, m_buttonSelect);
	DDX_Control(pDX, IDC_EDIT_WRAPPER, m_editWrapper);
	DDX_Control(pDX, IDC_BUTTON_SUPPLIER, m_buttonSupplier);
	DDX_Control(pDX, IDC_EDIT_STOCKCODE, m_editStockCode);
	DDX_Control(pDX, IDC_BUTTON_DELUNIT, m_buttonDelUnit);
	DDX_Control(pDX, IDC_CHECK_PREFERRED, m_checkPreferred);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_buttonDelete);
	DDX_Control(pDX, IDC_COMBO_SUPPLIER, m_comboSupplier);
	DDX_Control(pDX, IDC_COMBO_DELUNIT, m_comboDeliveryUnit);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_STOCKCODE, m_strStockCode);
	if( m_bReadOnly == FALSE ) DDV_MaxChars(pDX, m_strStockCode, Stock::SupplierRef.Max );
	DDX_Text(pDX, IDC_EDIT_WRAPPER, m_strWrapper);
	if( m_bReadOnly == FALSE ) DDV_MaxChars(pDX, m_strWrapper, Stock::Wrapper.Max );
	DDX_Text (pDX, IDC_EDIT_COST, m_dCost );
	DDX_Text (pDX, IDC_EDIT_RETAIL, m_dRetail );
	DDX_Text (pDX, IDC_EDIT_SUCOST, m_dSUCost );
	DDX_Text (pDX, IDC_EDIT_ITEMS, m_nDUItems );
	if( m_bReadOnly == FALSE ) DDV_MinMaxInt ( pDX, m_nDUItems, Stock::DUItems.Min, Stock::DUItems.Max );
	DDX_Text (pDX, IDC_EDIT_SUPERITEM, m_dDUItemSize );
	if( m_bReadOnly == FALSE ) DDV_MinMaxDouble ( pDX, m_dDUItemSize, Stock::DUItemSize.Min, Stock::DUItemSize.Max );
	DDX_Text (pDX, IDC_EDIT_SUTOTAL, m_dTotalSU );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageStockSupplier, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageStockSupplier)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SUPPLIER, OnTcnSelchangeTabs)
	ON_CBN_SELCHANGE(IDC_COMBO_DELUNIT, OnSelectDeliveryUnit)
	ON_BN_CLICKED(IDC_BUTTON_DELUNIT, OnButtonDeliveryUnit)
	ON_BN_CLICKED(IDC_BUTTON_SUPPLIER, OnButtonSupplier)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_CBN_SELCHANGE(IDC_COMBO_SUPPLIER, OnSelectSupplier)
	ON_EN_KILLFOCUS(IDC_EDIT_COST, OnKillFocusCost)
	ON_EN_KILLFOCUS(IDC_EDIT_ITEMS, OnKillFocusCost)
	ON_EN_KILLFOCUS(IDC_EDIT_SUPERITEM, OnKillFocusCost)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPropPageStockSupplier::GetEditCost() { return GetEdit( IDC_EDIT_COST ); }
CEdit* CPropPageStockSupplier::GetEditRetail() { return GetEdit( IDC_EDIT_RETAIL ); }
CEdit* CPropPageStockSupplier::GetEditDUItems() { return GetEdit( IDC_EDIT_ITEMS ); }
CEdit* CPropPageStockSupplier::GetEditDUItemSize() { return GetEdit( IDC_EDIT_SUPERITEM ); }
/**********************************************************************/

BOOL CPropPageStockSupplier::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	for ( int n = 0; n < StockOptions.GetStockMaxSuppliersPerItem(); n++ )
	{
		if ( ( FALSE == m_bInitialTabOnly ) || ( n == m_nSuppIdx ) )
		{
			CString str;
			str.Format( "Supplier%d", n + 1 );
			m_TabSupplier.InsertItem( n, str );
		}
	}	
	m_TabSupplier.SetCurSel(0);

	if ( StockOptions.GetStockSalesEntryType() == 0 )
	{
		m_staticRetail.ShowWindow( SW_HIDE );
		GetEditRetail() -> ShowWindow( SW_HIDE );
	}
	
	SubclassEdit ( IDC_EDIT_COST, SS_NUM_DP, 8, "%.*f", SysInfo.GetDPValue() );
	SubclassEdit ( IDC_EDIT_RETAIL, SS_NUM_DP, 8, "%.*f", SysInfo.GetDPValue() );
	SubclassEdit ( IDC_EDIT_SUCOST, SS_NUM_DP, 8, "%.5f" );
	SubclassEdit ( IDC_EDIT_ITEMS, SS_NUM, 6, "%d" );
	SubclassEdit ( IDC_EDIT_SUPERITEM, SS_NUM_DP, 8, "%.3f" );

	FillDeliveryUnitCombo();
	FillSupplierCombo();
	SelectSupplier();
	SetRecordControls();
	SetInitialDelUnit( m_StockRecord.GetCase ( m_nSuppIdx ), m_StockRecord.GetDUItems ( m_nSuppIdx ), m_StockRecord.GetDUItemSize( m_nSuppIdx ) );
	return TRUE;  
}

/**********************************************************************/

void CPropPageStockSupplier::OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	
	if ( FALSE == m_bInitialTabOnly )
	{
		int nSuppIdx = m_TabSupplier.GetCurSel();
		if ( ( nSuppIdx < 0 ) || ( nSuppIdx >= 5 ) )
			nSuppIdx = 0;

		if ( m_nSuppIdx != nSuppIdx )
		{
			SaveRecord();
			m_nSuppIdx = nSuppIdx;
			Refresh();
		}
	}
}

/**********************************************************************/

void CPropPageStockSupplier::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	FillSupplierCombo();
	SelectSupplier();
	SetRecordControls();
	SetInitialDelUnit( m_StockRecord.GetCase ( m_nSuppIdx ), m_StockRecord.GetDUItems ( m_nSuppIdx ), m_StockRecord.GetDUItemSize( m_nSuppIdx ) );
}

/**********************************************************************/

bool CPropPageStockSupplier::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageStockSupplier::SaveRecord()
{
	if( m_bReadOnly == TRUE )
		return;

	if ( m_nStockIdx >= 0 && m_nStockIdx < DataManager.Stock.GetSize() )
	{
		UpdateData( TRUE, FALSE );

		int nDelUnit = m_comboDeliveryUnit.GetCurSel();

		if ( nDelUnit != -1 )
			m_StockRecord.SetCase ( m_nSuppIdx, DataManager.DelUnit.GetText ( nDelUnit ) );
		else
		{
			CString strText;
			m_comboDeliveryUnit.GetWindowText ( strText );
			strText = strText.SpanExcluding ( "(" );
			::TrimSpaces ( strText, FALSE );
			strText = strText.Left ( MAX_LENGTH_UNIT_NAME);
			m_StockRecord.SetCase ( m_nSuppIdx, strText );
		}

		m_StockRecord.SetSuppNo ( m_nSuppIdx, m_nSuppNo );
		m_StockRecord.SetDUItems ( m_nSuppIdx, m_nDUItems );
		m_StockRecord.SetDUItemSize ( m_nSuppIdx, m_dDUItemSize );
		m_StockRecord.SetCost ( m_nSuppIdx, m_dCost );
		m_StockRecord.SetSuppRef ( m_nSuppIdx, m_strStockCode );
		m_StockRecord.SetWrapper ( m_nSuppIdx, m_strWrapper ); 
		m_StockRecord.SetRetail( m_nSuppIdx, m_dRetail );
		m_StockRecord.TidySuppliers( StockOptions.GetStockMaxSuppliersPerItem() );

		if ( m_nSuppIdx != 0 && m_checkPreferred.GetCheck() == TRUE )
		{
			m_StockRecord.MakePreferred ( m_nSuppIdx );
			m_bMakePreferred = TRUE;
			if ( m_pLastSupplierTab != NULL ) *m_pLastSupplierTab = 0;
		}
		else if ( m_nSuppNo >= 1 )
			if ( m_pLastSupplierTab != NULL ) *m_pLastSupplierTab = m_nSuppIdx;
		
		DataManager.Stock.SetAt ( m_nStockIdx, m_StockRecord );

		if ( m_pPluRecord != NULL )
			DataManager.Plu.UpdatePluPrice ( *m_pPluRecord, 0.0, TRUE );
	}
}

/**********************************************************************/

void CPropPageStockSupplier::GetRecordData()
{
	if ( m_nStockIdx >= 0 && m_nStockIdx < DataManager.Stock.GetSize() )
	{
		DataManager.Stock.GetAt ( m_nStockIdx, m_StockRecord );

		m_StockRecord.TidySuppliers( StockOptions.GetStockMaxSuppliersPerItem() );
		m_nSuppNo = m_StockRecord.GetSuppNo ( m_nSuppIdx );
		m_nDUItems = m_StockRecord.GetDUItems ( m_nSuppIdx );
		m_dDUItemSize = m_StockRecord.GetDUItemSize ( m_nSuppIdx );
		m_dCost = m_StockRecord.GetCost ( m_nSuppIdx );
		m_dRetail = m_StockRecord.GetRetail( m_nSuppIdx );

		if ( DealerFlags.GetSuppRefFlag() == FALSE )
			m_strStockCode = m_StockRecord.GetSuppRefInternal ( m_nSuppIdx );
		else
			m_strStockCode = m_StockRecord.GetStockCode();

		m_strWrapper = m_StockRecord.GetWrapper( m_nSuppIdx );
		
		m_bInUse = ( m_StockRecord.GetSupplierCount() > m_nSuppIdx ) ? TRUE : FALSE; 
		m_bAvailable = ( m_StockRecord.GetSupplierCount() >= m_nSuppIdx ) ? TRUE : FALSE;
	
		m_dTotalSU = m_StockRecord.GetYield( m_nSuppIdx );

		if ( m_dTotalSU != 0.0 )
			m_dSUCost = m_dCost / m_dTotalSU;
		else
			m_dSUCost = 0.0;
	}
}

/**********************************************************************/

BOOL CPropPageStockSupplier::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageStockSupplier::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	FillSupplierCombo();
	SelectSupplier();
	SetRecordControls();
	SetInitialDelUnit( m_StockRecord.GetCase ( m_nSuppIdx ), m_StockRecord.GetDUItems ( m_nSuppIdx ), m_StockRecord.GetDUItemSize( m_nSuppIdx ) );
	return bResult;
}

/**********************************************************************/

void CPropPageStockSupplier::SetRecordControls()
{
	m_buttonDelete.EnableWindow ( m_bInUse & m_bAllowChangeSupplier );
	m_buttonSupplier.EnableWindow ( m_bAvailable & m_bAllowChangeSupplier );
	
	if ( m_bAllowChangeSupplier == FALSE )
		m_buttonDelete.ShowWindow( SW_HIDE );

	if ( m_nSuppIdx == 0 )
	{
		ShowAndEnableWindow( &m_checkPreferred, FALSE );
	}
	else 
	{
		ShowAndEnableWindow( &m_checkPreferred, TRUE, m_bInUse && m_bAllowMakePreferred );
		m_checkPreferred.SetCheck ( FALSE );
	}

	if ( m_nDefaultSuppNo > 0 )
	{
		m_buttonSelect.ShowWindow( SW_SHOW );
		m_staticSelect.ShowWindow( SW_HIDE );
		m_buttonSelect.EnableWindow ( m_bAvailable & m_bAllowChangeSupplier );
	}
	else
	{
		m_buttonSelect.ShowWindow( SW_HIDE );
		m_staticSelect.ShowWindow( SW_SHOW );
		m_buttonSelect.EnableWindow( FALSE );
	}

	if ( m_bReadOnly == TRUE )
	{
		m_comboSupplier.EnableWindow( FALSE );
		m_buttonSupplier.ShowWindow( SW_HIDE );
		m_checkPreferred.EnableWindow( FALSE );
		m_comboDeliveryUnit.EnableWindow( FALSE );
		m_buttonDelUnit.ShowWindow( SW_HIDE );
		m_buttonDelete.ShowWindow( SW_HIDE );
		m_editStockCode.EnableWindow( FALSE );
		m_editWrapper.EnableWindow( FALSE );
		GetEditDUItems() -> EnableWindow( FALSE );
		GetEditDUItemSize() -> EnableWindow( FALSE );
		GetEditCost() -> EnableWindow( FALSE );
		GetEditRetail() -> EnableWindow( FALSE );
	}
}

/**********************************************************************/

void CPropPageStockSupplier::SetInitialDelUnit( const char* szCase, int nDUItems, double dDUItemSize )
{
	CString strCase = szCase;
	bool bFound = FALSE;

	for ( int nIndex = 0; nIndex < DataManager.DelUnit.GetSize(); nIndex++ )
	{
		int nArrayDUItems = DataManager.DelUnit.GetDUItems( nIndex );
		double dArrayDUItemSize = DataManager.DelUnit.GetDUItemSize( nIndex );

		if ( strCase == DataManager.DelUnit.GetText ( nIndex ) && ( nDUItems == nArrayDUItems ) && ( CPriceHelpers::CompareDoubles ( dDUItemSize, dArrayDUItemSize, 3 ) == 0 ) )
		{
			m_comboDeliveryUnit.SetCurSel ( nIndex );
			bFound = TRUE;
			break;
		}
	}

	if ( bFound == FALSE )
	{
		m_comboDeliveryUnit.SetCurSel ( -1 );
		m_comboDeliveryUnit.SetWindowText ( strCase );
	}
}

/**********************************************************************/

void CPropPageStockSupplier::FillSupplierCombo()
{
	m_comboSupplier.ResetContent();

	if ( m_bAvailable == TRUE )
	{
		for ( int nIndex = 0; nIndex < DataManager.Supplier.GetSize(); nIndex++ )
		{
			int nInsertionPoint = m_comboSupplier.AddString ( DataManager.Supplier.GetName ( nIndex ) );
			m_comboSupplier.SetItemData ( nInsertionPoint, DataManager.Supplier.GetSuppNo ( nIndex  ) );
		}
	}
	
	if ( m_bInUse == FALSE )
	{
		m_comboSupplier.InsertString ( 0, "Not Used" );
		m_comboSupplier.SetItemData ( 0, 0 );
	}

	bool bFoundSupplier = FALSE;

	for ( int nIndex = 0; nIndex < m_comboSupplier.GetCount(); nIndex++ )
	{
		if ( m_comboSupplier.GetItemData ( nIndex ) == ( WORD ) m_nSuppNo )
		{
			m_comboSupplier.SetCurSel ( nIndex );
			bFoundSupplier = TRUE;
			break;
		}
	}

	if ( bFoundSupplier == FALSE )
		m_comboSupplier.SetCurSel ( 0 );

	if ( m_comboSupplier.GetCount() < 2 )
		m_comboSupplier.EnableWindow ( FALSE );
	else
		m_comboSupplier.EnableWindow ( m_bAllowChangeSupplier );

	int nSelection = m_comboSupplier.GetCurSel();
	m_nSuppNo = m_comboSupplier.GetItemData(nSelection);
}

/**********************************************************************/

void CPropPageStockSupplier::FillDeliveryUnitCombo()
{
	m_comboDeliveryUnit.ResetContent();

	for ( int nIndex = 0; nIndex < DataManager.DelUnit.GetSize(); nIndex++ )
		m_comboDeliveryUnit.AddString ( DataManager.DelUnit.GetComboText ( nIndex ) );
}

/**********************************************************************/

void CPropPageStockSupplier::OnSelectDeliveryUnit() 
{
	UpdateData ( TRUE, FALSE );

	int nSel = m_comboDeliveryUnit.GetCurSel();
	if ( nSel >= 0 && nSel < DataManager.DelUnit.GetSize() )
	{
		m_nDUItems = DataManager.DelUnit.GetDUItems( nSel );
		m_dDUItemSize = DataManager.DelUnit.GetDUItemSize( nSel );

		m_dTotalSU = m_dDUItemSize * m_nDUItems;
			
		if ( m_dTotalSU != 0.0 )
			m_dSUCost = m_dCost / m_dTotalSU;
		else
			m_dSUCost = 0.0;
		
		UpdateData ( FALSE );
	}
}

/**********************************************************************/

void CPropPageStockSupplier::OnButtonDeliveryUnit() 
{
	CDataManagerInfo info;
	if ( DataManager.OpenDelUnitBuffer( info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	UpdateData( TRUE, FALSE );

	CString strText;
	m_comboDeliveryUnit.GetWindowText( strText );
	strText = strText.SpanExcluding ( "(" );

	//THE LIST OF DELIVERY UNITS MAY BE CHANGED BY THE READ
	FillDeliveryUnitCombo();
	SetInitialDelUnit( strText, m_nDUItems, m_dDUItemSize );

	CListDataDelUnitDlg dlg ( strText, m_nDUItems, m_dDUItemSize, this );
		
	if ( dlg.DoModal() == IDOK )	
	{
		if ( DataManager.WriteDelUnitBuffer( info ) == FALSE )
			Prompter.WriteError( info );
		else
		{
			FillDeliveryUnitCombo();

			int nYieldIdx = dlg.GetListSelection();
			if ( nYieldIdx != -1 )
			{
				SetInitialDelUnit ( DataManager.DelUnit.GetText ( nYieldIdx ), DataManager.DelUnit.GetDUItems ( nYieldIdx ), DataManager.DelUnit.GetDUItemSize( nYieldIdx ) );
				m_nDUItems = DataManager.DelUnit.GetDUItems( nYieldIdx );
				m_dDUItemSize = DataManager.DelUnit.GetDUItemSize( nYieldIdx );
			}
		}
	}
	
	DataManager.CloseDelUnitBuffer( info );

	m_dTotalSU = m_dDUItemSize * m_nDUItems;

	if ( m_dTotalSU != 0.0 )
		m_dSUCost = m_dCost / m_dTotalSU;
	else
		m_dSUCost = 0.0;

	UpdateData ( FALSE );
}

/**********************************************************************/

void CPropPageStockSupplier::OnButtonSupplier() 
{
	if ( DataManager.Supplier.IsValid() == FALSE )
	{
		Prompter.SupplierError();
		return;
	}

	CDataManagerInfo info;
	if ( DataManager.OpenSupplierBuffer( info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	//READING FROM THE FILE MAY CHANGE THE SUPPLIER LIST
	FillSupplierCombo();
	SelectSupplier();

	UpdateData( TRUE, FALSE );
	
	CListDataSupplierDlg dlg ( m_nSuppNo, this );
	
	bool bSavedSuppliers = FALSE;

	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManager.WriteSupplierBuffer( info ) == TRUE )
			bSavedSuppliers = TRUE;
		else
			Prompter.WriteError( info );
	}

	DataManager.CloseSupplierBuffer( info );

	if ( bSavedSuppliers == TRUE )		
	{
		if ( dlg.GetListSelection() >= 0 )
			m_nSuppNo = DataManager.Supplier.GetSuppNo ( dlg.GetListSelection() );

		FillSupplierCombo();
		UpdateData ( FALSE );
		
		int nSelection = m_comboSupplier.GetCurSel();
		
		if ( nSelection >= 0 )
			m_nSuppNo = m_comboSupplier.GetItemData ( nSelection );	
		else
			m_nSuppNo = 0;

		SelectSupplier();
	}
}

/**********************************************************************/

void CPropPageStockSupplier::OnButtonDelete() 
{
	if ( Prompter.YesNo ( "Are you sure you wish to remove this delivery method from this stock item" ) == IDYES )	
	{
		m_StockRecord.ClearSupplier ( m_nSuppIdx );
		m_StockRecord.TidySuppliers( StockOptions.GetStockMaxSuppliersPerItem() );
		DataManager.Stock.SetAt ( m_nStockIdx, m_StockRecord );
		Refresh();
	}
}

/**********************************************************************/

void CPropPageStockSupplier::OnSelectSupplier()
{
	UpdateData( TRUE, FALSE );
	int nSelection = m_comboSupplier.GetCurSel();
	m_nSuppNo = m_comboSupplier.GetItemData( nSelection );
	SelectSupplier();
}

/**********************************************************************/

void CPropPageStockSupplier::OnButtonSelect()
{
	UpdateData( TRUE, FALSE );

	for ( int n = 0; n < m_comboSupplier.GetCount(); n++ )
	{
		int nSuppNo = (int) m_comboSupplier.GetItemData(n);
		if ( nSuppNo == m_nDefaultSuppNo )
		{
			m_comboSupplier.SetCurSel(n);
			m_nSuppNo = nSuppNo;
			SelectSupplier();
			break;
		}
	}
}

/**********************************************************************/

void CPropPageStockSupplier::OnKillFocusCost() 
{
	UpdateData( TRUE, FALSE );

	m_dTotalSU = m_dDUItemSize * m_nDUItems;

	if ( m_dTotalSU != 0.0 )
		m_dSUCost = m_dCost / m_dTotalSU;
	else
		m_dSUCost = 0.0;

	UpdateData ( FALSE );
	UpdateFocus();
}

/**********************************************************************/

void CPropPageStockSupplier::SelectSupplier() 
{	
	if ( m_nSuppNo == 0 )
	{
		m_checkPreferred.EnableWindow ( FALSE );
		m_checkPreferred.SetCheck ( FALSE );
		m_comboDeliveryUnit.EnableWindow ( FALSE );
		m_buttonDelUnit.EnableWindow ( FALSE );
		GetEditCost() -> EnableWindow ( FALSE );
		GetEditRetail() -> EnableWindow( FALSE );
		GetEditDUItems() -> EnableWindow ( FALSE );
		GetEditDUItemSize() -> EnableWindow ( FALSE );
		m_editStockCode.EnableWindow ( FALSE );
		m_editWrapper.EnableWindow( FALSE );
	}
	else
	{
		m_checkPreferred.EnableWindow ( m_bAllowMakePreferred );
		m_comboDeliveryUnit.EnableWindow ( TRUE );
		m_buttonDelUnit.EnableWindow ( TRUE );
		GetEditCost() -> EnableWindow ( TRUE );
		GetEditRetail() -> EnableWindow( TRUE );
		GetEditDUItems() -> EnableWindow ( TRUE );
		GetEditDUItemSize() -> EnableWindow ( TRUE );
		m_editStockCode.EnableWindow ( DealerFlags.GetSuppRefFlag() == FALSE );
		m_editWrapper.EnableWindow( TRUE );
	}

	if ( m_nSuppIdx == 0 )
	{
		m_checkPreferred.EnableWindow ( FALSE );
		m_checkPreferred.SetCheck ( TRUE );
	}
}
	
/**********************************************************************/

void CPropPageStockSupplier::SetStockIdx ( int nStockIdx )
{
	if ( nStockIdx >= 0 && nStockIdx < DataManager.Stock.GetSize() )
		m_nStockIdx = nStockIdx;
	else
		m_nStockIdx = -1;
}

/**********************************************************************/

void CPropPageStockSupplier::UpdateFocus()
{
	CWnd* pWnd = GetFocus();

	if ( pWnd == GetEditCost() )
		GetEditCost() -> SetSel( 0, -1 );
	else if ( pWnd == GetEditDUItems() )
		GetEditDUItems() -> SetSel( 0, -1 );
	else if ( pWnd == GetEditDUItemSize() )
		GetEditDUItemSize() -> SetSel( 0, -1 );
}

/**********************************************************************/

