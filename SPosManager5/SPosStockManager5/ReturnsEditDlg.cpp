/**********************************************************************/
#include "FilterTypes.h"
#include "PluFilterDlgStockman.h"
#include "BarcodePluNoTable.h"
#include "PriceHelpers.h"
#include "StockFilterDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ReturnsEditDlg.h"
/**********************************************************************/

CReturnsEditDlg::CReturnsEditDlg( int nSuppIdx, int nSpIdx, CSuppRefFinder& SuppRefFinder, CReturnsEditDlg** pParentToThis, int nStkTakeNo, CWnd* pParent)
	: CSSDialog(CReturnsEditDlg::IDD, pParent), m_FilterArray ( FILTERTYPE_BROWSE_PLU ), m_StockFilterArray ( FILTERTYPE_BROWSE_STOCK ), m_SuppRefFinder( SuppRefFinder )
{
	m_nStkTakeNo = nStkTakeNo;

	DataInit ( nSuppIdx, nSpIdx );
	m_bAdd = TRUE;
	m_strAction = "Add";

	m_pParentToThis = pParentToThis;
	*m_pParentToThis = this;
}

/**********************************************************************/

CReturnsEditDlg::CReturnsEditDlg( int nSuppIdx, int nSpIdx, CSuppRefFinder& SuppRefFinder, int nStkTakeNo, CWnd* pParent)
	: CSSDialog(CReturnsEditDlg::IDD, pParent), m_FilterArray ( FILTERTYPE_BROWSE_PLU ), m_StockFilterArray ( FILTERTYPE_BROWSE_STOCK ), m_SuppRefFinder( SuppRefFinder )
{
	m_nStkTakeNo = nStkTakeNo;

	DataInit ( nSuppIdx, nSpIdx );
	m_bAdd = FALSE;
	m_strAction = "Edit";
}

/**********************************************************************/

void CReturnsEditDlg::DataInit( int nSuppIdx, int nSpIdx )
{
	//{{AFX_DATA_INIT(CReturnsEditDlg)
	m_dQty = 0.0;
	m_strDescription = _T("");
	//}}AFX_DATA_INIT

	m_StockItemKey.m_nPluNo = 0;
	m_StockItemKey.m_strStockCode = "";
	m_StockItemKey.m_strSuppRef = "";

	m_nSuppIdx = nSuppIdx;

	if ( m_nSuppIdx >= 0 && m_nSuppIdx < DataManager.Supplier.GetSize() )
	{
		m_strSupplier = DataManager.Supplier.GetName ( m_nSuppIdx );
		m_nSuppNo = DataManager.Supplier.GetSuppNo ( m_nSuppIdx );
	}
	else
	{
		m_strSupplier = "Unspecified Supplier";
		m_nSuppIdx = -1;
		m_nSuppNo = -1;
	}

	m_dQty = 0.0;
	m_dCost = 0.0;
	m_nSpIdx = nSpIdx;

	if ( m_nStkTakeNo >= 1 )
	{
		m_StockFilterArray.SetSpIdx( m_nSpIdx, -1 );
		m_StockFilterArray.SetStocktakeFilter( m_nStkTakeNo );
	}
}

/**********************************************************************/

void CReturnsEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReturnsEditDlg)
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_PLU, m_buttonBrowsePlu);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowseStock);
	DDX_Control(pDX, IDC_RADIO_SUPPREF, m_radioSuppRef);
	DDX_Control(pDX, IDC_RADIO_STOCKCODE, m_radioStockCode);
	DDX_Control(pDX, IDC_RADIO_PLUNUMBER, m_radioPluNumber);
	DDX_Control(pDX, IDC_EDIT_SUPPREF, m_editSuppRef);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDC_COMBO_DELUNIT, m_comboDeliveryUnit);
	DDX_Control(pDX, IDC_EDIT_STOCKCODE, m_editStockCode);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_strDescription );
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_QTY_DOUBLE, m_dQty );
	DDX_Text(pDX, IDC_EDIT_COST, m_dCost );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CReturnsEditDlg, CDialog)
	//{{AFX_MSG_MAP(CReturnsEditDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCKCODE, OnKillFocusStockCode)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowseStock)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU, OnButtonBrowsePlu)
	ON_EN_KILLFOCUS(IDC_EDIT_QTY_DOUBLE, OnKillFocusQty)
	ON_EN_KILLFOCUS(IDC_EDIT_SUPPREF, OnKillFocusSuppRef)
	ON_BN_CLICKED(IDC_RADIO_PLUNUMBER, OnRadioPluNumber)
	ON_BN_CLICKED(IDC_RADIO_STOCKCODE, OnRadioStockCode)
	ON_BN_CLICKED(IDC_RADIO_SUPPREF, OnRadioSuppRef)
	ON_CBN_SELCHANGE(IDC_COMBO_DELUNIT, OnSelectDelUnit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CReturnsEditDlg::GetEditQty() { return GetEdit( IDC_EDIT_QTY_DOUBLE ); }
CEdit* CReturnsEditDlg::GetEditCost() { return GetEdit( IDC_EDIT_COST ); }
CEdit* CReturnsEditDlg::GetEditPluNo() { return GetEdit( IDC_EDIT_PLUNO ); }
/**********************************************************************/

BOOL CReturnsEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit ( IDC_EDIT_QTY_DOUBLE, SS_NUM_DP, 8, "%.5f" );
	SubclassEdit ( IDC_EDIT_COST, SS_NUM_DP, 8, "%.*f", SysInfo.GetDPValue() );
	SubclassEdit ( IDC_EDIT_PLUNO, SS_NUM, Super.MaxPluLen(), "%s" );
	
	m_editStockCode.LimitText( Stock::StockCode.Max );
	m_editSuppRef.LimitText( Stock::SupplierRef.Max );
	
	CString strTitle;
	strTitle.Format ( "%s Returns Line (%s, %s)", 
		(const char*) m_strAction,
		dbStockpoint.GetName( m_nSpIdx ), 
		(const char*) m_strSupplier );
	
	SetWindowText ( strTitle );

	if ( m_bAdd == TRUE )
	{
		OnRadioSuppRef();
		FillDeliveryUnitCombo( -1, -1 );
	}
	else
	{
		CCSVMiscStockLevels csvIn( m_strInputLine );
			
		m_nEntryMode = csvIn.GetItemLookupMethod();
		m_radioSuppRef.SetCheck( m_nEntryMode == ENTRY_MODE_SUPPREF || m_nEntryMode == ENTRY_MODE_WRAPPER );
		m_radioPluNumber.SetCheck( m_nEntryMode == ENTRY_MODE_PLUNO );
		m_radioStockCode.SetCheck( m_nEntryMode == ENTRY_MODE_STOCKCODE );

		m_editSuppRef.EnableWindow( m_nEntryMode == ENTRY_MODE_SUPPREF || m_nEntryMode == ENTRY_MODE_WRAPPER );
		GetEditPluNo() -> EnableWindow( m_nEntryMode == ENTRY_MODE_PLUNO );
		m_editStockCode.EnableWindow( m_nEntryMode == ENTRY_MODE_STOCKCODE );
	
		m_StockItemKey.m_strStockCode = csvIn.GetStockCode();
		m_strDescription = csvIn.GetDescription();
		m_StockItemKey.m_nPluNo = csvIn.GetPluNoInt();

		if ( m_nEntryMode != ENTRY_MODE_WRAPPER )
			m_StockItemKey.m_strSuppRef = csvIn.GetSuppRef();
		else
			m_StockItemKey.m_strSuppRef = csvIn.GetWrapperCode();

		if ( csvIn.GetUseDUFlag() == TRUE )
			m_dQty = csvIn.GetDUQty();
		else
			m_dQty = csvIn.GetSUQty();

		m_dCost = csvIn.GetTotalCost();
		UpdateData( FALSE );
		UpdateItemKey( FALSE, TRUE, TRUE, TRUE );
		
		int nStockIdx;
		if ( DataManager.Stock.FindStockItemByCode( m_StockItemKey.m_strStockCode, nStockIdx ) == TRUE )
			FillDeliveryUnitCombo( nStockIdx, -1 );
		else
			FillDeliveryUnitCombo( -1, -1 );
		
		if ( m_DelUnitSelector.SelectDelUnit( csvIn ) == FALSE )
		{
			m_dQty *= ( csvIn.GetDUItemSize() * csvIn.GetDUItems() );
			UpdateData( FALSE );
		}
		
		m_buttonOK.SetWindowText ( "OK" );
		m_buttonCancel.SetWindowText ("Cancel" );

		GetEditQty() -> SetFocus();
		GetEditQty() -> SetSel( 0, -1 );
	}

	CenterWindow();

	//WE HAVE SET FOCUS TO A CONTROL
	return FALSE;  
}

/**********************************************************************/

void CReturnsEditDlg::UpdateItemKey( bool bFromControl, bool bPluNo, bool bStockCode, bool bSuppRef )
{
	if ( FALSE == bFromControl )
	{
		if ( TRUE == bPluNo )
		{
			CString strPluNo;
			::FormatPluNo( m_StockItemKey.m_nPluNo, strPluNo );
			GetEditPluNo() -> SetWindowText( strPluNo );
		}

		if ( TRUE == bStockCode )
			m_editStockCode.SetWindowText( m_StockItemKey.m_strStockCode );

		if ( TRUE == bSuppRef )
			m_editSuppRef.SetWindowText( m_StockItemKey.m_strSuppRef );
	}
	else
	{
		if ( TRUE == bPluNo )
		{
			CString strPluNo;
			GetEditPluNo() -> GetWindowText( strPluNo );
			strPluNo = BarcodePluNoTable.ConvertPlu( strPluNo );
			m_StockItemKey.m_nPluNo = _atoi64( strPluNo );
		}

		if ( TRUE == bStockCode )
			m_editStockCode.GetWindowText( m_StockItemKey.m_strStockCode );

		if ( TRUE == bSuppRef )
			m_editSuppRef.GetWindowText( m_StockItemKey.m_strSuppRef );
	}
}

/**********************************************************************/

bool CReturnsEditDlg::LookUpSuppRef( int& nStockIdx, int& nStockSuppIdx )
{
	m_StockItemKey.m_strStockCode = "";
	m_StockItemKey.m_nPluNo = 0;
	nStockIdx = -1;
	nStockSuppIdx = -1;

	if ( m_StockItemKey.m_strSuppRef == "" )
		return FALSE;
		
	m_SuppRefFinder.Rebuild( m_nSuppNo );
		
	if ( m_SuppRefFinder.FindItemBySuppRef( m_StockItemKey.m_strSuppRef, nStockIdx, nStockSuppIdx ) == TRUE )
	{
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( nStockIdx, StockRecord );
		m_StockItemKey.m_strStockCode = StockRecord.GetStockCode();

		int nStockPluIdx;
		if ( DataManager.StockPlu.FindStockCode ( m_StockItemKey.m_strStockCode, nStockPluIdx ) == TRUE )
			if ( DataManager.StockPlu.GetPluNoCount( nStockPluIdx ) > 0 )
				m_StockItemKey.m_nPluNo = DataManager.StockPlu.GetPluNo( nStockPluIdx, 0 );
	
		m_nEntryMode = ENTRY_MODE_SUPPREF;
		return TRUE;
	}

	if ( m_SuppRefFinder.FindItemByWrapper( m_StockItemKey.m_strSuppRef, nStockIdx, nStockSuppIdx ) == TRUE )
	{
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( nStockIdx, StockRecord );
		m_StockItemKey.m_strStockCode = StockRecord.GetStockCode();

		int nStockPluIdx;
		if ( DataManager.StockPlu.FindStockCode ( m_StockItemKey.m_strStockCode, nStockPluIdx ) == TRUE )
			if ( DataManager.StockPlu.GetPluNoCount( nStockPluIdx ) > 0 )
				m_StockItemKey.m_nPluNo = DataManager.StockPlu.GetPluNo( nStockPluIdx, 0 );
	
		m_nEntryMode = ENTRY_MODE_WRAPPER;
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

bool CReturnsEditDlg::LookUpPluNo( int& nStockIdx, int& nStockSuppIdx )
{
	m_StockItemKey.m_strSuppRef = "";
	m_StockItemKey.m_strStockCode = "";
	nStockIdx = -1;
	nStockSuppIdx = -1;

	bool bFound = FALSE;

	if ( m_StockItemKey.m_nPluNo != 0 )
	{
		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber( m_StockItemKey.m_nPluNo, nPluIdx ) == TRUE )
		{
			bFound = TRUE;
			DataManager.Plu.GetPluStockCode ( m_StockItemKey.m_nPluNo, m_StockItemKey.m_strStockCode, FALSE );
		
			if ( m_StockItemKey.m_strStockCode != "" )
			{
				if ( DataManager.Stock.FindStockItemByCode( m_StockItemKey.m_strStockCode, nStockIdx ) == FALSE )
				{	
					m_StockItemKey.m_strStockCode = "";
					nStockIdx = -1;
				}
				else
				{
					CStockCSVRecord StockRecord;
					DataManager.Stock.GetAt( nStockIdx, StockRecord );

					for ( int n = 0; n < StockRecord.GetSupplierCount(); n++ )
					{
						if ( StockRecord.GetSuppNo(n) == m_nSuppNo )
						{
							nStockSuppIdx = n;
							m_StockItemKey.m_strSuppRef = StockRecord.GetSuppRefExternal( n, DealerFlags.GetSuppRefFlag() );
							break;
						}
					}
				}
			}
		}
	}

	return bFound;
}

/**********************************************************************/

bool CReturnsEditDlg::LookUpStockCode( int& nStockIdx, int& nStockSuppIdx )
{
	m_StockItemKey.m_nPluNo = 0;
	m_StockItemKey.m_strSuppRef = "";
	nStockIdx = -1;
	nStockSuppIdx = -1;

	bool bFound = FALSE;

	if ( DataManager.Stock.FindStockItemByCode( m_StockItemKey.m_strStockCode, nStockIdx ) == FALSE )
		nStockIdx = -1;
	else
	{
		bFound = TRUE;

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( nStockIdx, StockRecord );

		for ( int n = 0; n < StockRecord.GetSupplierCount(); n++ )
		{
			if ( StockRecord.GetSuppNo(n) == m_nSuppNo )
			{
				nStockSuppIdx = n;
				m_StockItemKey.m_strSuppRef = StockRecord.GetSuppRefExternal( n, DealerFlags.GetSuppRefFlag() );
				break;
			}
		}

		if ( ( nStockSuppIdx == -1 ) && ( StockRecord.GetSupplierCount() > 0 ) )
			nStockSuppIdx = 0;

		int nStockPluIdx;
		if ( DataManager.StockPlu.FindStockCode ( m_StockItemKey.m_strStockCode, nStockPluIdx ) == TRUE )
			if ( DataManager.StockPlu.GetPluNoCount( nStockPluIdx ) > 0 )
				m_StockItemKey.m_nPluNo = DataManager.StockPlu.GetPluNo( nStockPluIdx, 0 );
	}
	
	return bFound;
}

/**********************************************************************/

void CReturnsEditDlg::FillDeliveryUnitCombo( int nStockIdx, int nStockSuppIdx )
{
	m_DelUnitSelector.FillComboBox ( &m_comboDeliveryUnit, nStockIdx, m_nSuppNo );
	
	if ( nStockSuppIdx >= 0 )
		m_DelUnitSelector.SelectStockSuppIdx( nStockSuppIdx );
	else
		m_comboDeliveryUnit.SetCurSel(0);
}

/**********************************************************************/

void CReturnsEditDlg::OnKillFocusSuppRef() 
{
	UpdateData ( TRUE, FALSE );
	
	int nStockIdx, nStockSuppIdx;
	
	UpdateItemKey( TRUE, FALSE, FALSE, TRUE );
	LookUpSuppRef( nStockIdx, nStockSuppIdx );
	UpdateItemKey( FALSE, TRUE, TRUE, FALSE );
	
	UpdateStockCode( nStockIdx, nStockSuppIdx );
	UpdateData(FALSE);
	
	//WE **MUST** DO THIS HERE BECAUSE WE ARE TABBING FROM A STANDARD (NON-CSS)
	//EDIT THAT DOES NOT IMPLEMENT PAUL'S SETSEL FIX
	GetEditQty() -> SetSel( 0, -1 );
}

/**********************************************************************/

void CReturnsEditDlg::OnKillFocusPluNo() 
{
	UpdateData ( TRUE, FALSE );

	UpdateItemKey( TRUE, TRUE, FALSE, FALSE );
	UpdateItemKey( FALSE, TRUE, FALSE, FALSE );
	
	int nStockIdx, nStockSuppIdx;
	LookUpPluNo( nStockIdx, nStockSuppIdx );
	UpdateItemKey( FALSE, FALSE, TRUE, TRUE );
	
	UpdateStockCode( nStockIdx, nStockSuppIdx );
	UpdateData(FALSE);
}

/**********************************************************************/

void CReturnsEditDlg::OnKillFocusStockCode() 
{
	UpdateData ( TRUE, FALSE );

	UpdateItemKey( TRUE, FALSE, TRUE, FALSE );
	m_StockItemKey.m_strStockCode = BarcodePluNoTable.ConvertStockCode( m_StockItemKey.m_strStockCode );
	UpdateItemKey( FALSE, FALSE, TRUE, FALSE );

	int nStockIdx, nStockSuppIdx;
	LookUpStockCode( nStockIdx, nStockSuppIdx );
	UpdateStockCode( nStockIdx, nStockSuppIdx );
	UpdateData(FALSE);

	UpdateItemKey( FALSE, TRUE, TRUE, TRUE );
	
	//WE **MUST** DO THIS HERE BECAUSE WE ARE TABBING FROM A STANDARD (NON-CSS)
	//EDIT THAT DOES NOT IMPLEMENT PAUL'S SETSEL FIX
	GetEditQty() -> SetSel( 0, -1 );
}

/**********************************************************************/

void CReturnsEditDlg::UpdateStockCode( int nStockIdx, int nStockSuppIdx )
{		
	if ( nStockIdx != -1 )
	{
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( nStockIdx, StockRecord );
		m_strDescription = StockRecord.GetDescription();
	}
	else
		m_strDescription = "Unknown";

	FillDeliveryUnitCombo( nStockIdx, nStockSuppIdx );

	CDelUnitInfo DelUnitInfo;
	m_DelUnitSelector.GetDelUnitInfo( DelUnitInfo );

	if ( DelUnitInfo.m_nStockSuppIdx != -1 )
	{
		m_dQty = 1.0;
		m_dCost = DelUnitInfo.m_dDUCost;
	}
	else
	{
		m_dQty = 1.0;
		m_dCost = 0.0;
	}		
}

/**********************************************************************/

void CReturnsEditDlg::OnSelectDelUnit() 
{
	CDelUnitInfo DelUnitInfo;
	m_DelUnitSelector.GetDelUnitInfo( DelUnitInfo );

	if ( DelUnitInfo.m_nStockSuppIdx != -1 )
	{
		m_dCost = DelUnitInfo.m_dDUCost * m_dQty;
		UpdateData ( FALSE );
	}	
}

/**********************************************************************/

void CReturnsEditDlg::OnKillFocusQty() 
{
	double dOldQty = m_dQty;
	UpdateData( TRUE, FALSE );

	//WHEN EDITING AN EXISTING LINE, WE DO NOT WANT THE VALUE TO BE
	//RECALCULATED IF THE QUANTITY REMAINS THE SAME
	if ( CPriceHelpers::CompareDoubles( dOldQty, m_dQty, 5 ) != 0 )
	{
		CDelUnitInfo DelUnitInfo;
		m_DelUnitSelector.GetDelUnitInfo( DelUnitInfo );

		if ( DelUnitInfo.m_nStockSuppIdx != -1 )
		{
			m_dCost = DelUnitInfo.m_dDUCost * m_dQty;
			UpdateData ( FALSE );
		}
	}
}

/**********************************************************************/

void CReturnsEditDlg::OnButtonBrowsePlu() 
{
	m_pParentWnd -> EnableWindow( FALSE );
	BrowsePluInternal();
	m_pParentWnd -> EnableWindow( TRUE );
}

/**********************************************************************/

void CReturnsEditDlg::BrowsePluInternal()
{
	UpdateData( TRUE, FALSE );
	
	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg( m_FilterArray, this, nDlgResult, nPluIdx );
	
	if ( IDOK != nDlgResult )
		return;

	if ( nPluIdx < 0 )
		return;
		
	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt ( nPluIdx, PluRecord );
	m_StockItemKey.m_nPluNo = PluRecord.GetPluNoInt();	

	int nStockIdx, nStockSuppIdx;
	LookUpPluNo( nStockIdx, nStockSuppIdx );
	UpdateStockCode( nStockIdx, nStockSuppIdx );
	
	UpdateData ( FALSE );

	UpdateItemKey( FALSE, TRUE, TRUE, TRUE );

	switch ( m_nEntryMode )
	{
	case ENTRY_MODE_SUPPREF:
	case ENTRY_MODE_WRAPPER:
	case ENTRY_MODE_STOCKCODE:
		GetEditQty() -> SetFocus();
		GetEditQty() -> SetSel(0,-1);
		break;

	case ENTRY_MODE_PLUNO:
	default:
		GetEditPluNo() -> SetFocus();
		GetEditPluNo() -> SetSel ( 0, -1 );
		break;
	}
}

/**********************************************************************/

void CReturnsEditDlg::OnButtonBrowseStock() 
{
	m_pParentWnd -> EnableWindow( FALSE );
	BrowseStockInternal();
	m_pParentWnd -> EnableWindow( TRUE );
}

/**********************************************************************/

void CReturnsEditDlg::BrowseStockInternal()
{
	UpdateData( TRUE, FALSE );

	m_StockFilterArray.SetSupplierFilter ( m_nSuppNo );
	CStockFilterDlg dlgBrowse ( m_StockFilterArray, this );
	dlgBrowse.SetSimpleSupplierMode();

	if ( dlgBrowse.DoModal() != IDOK )
		return;

	int nStockIdx = dlgBrowse.GetStockIdx();
	if ( nStockIdx < 0 )
		return;

	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	m_StockItemKey.m_strStockCode = StockRecord.GetStockCode ();	
	
	int nStockSuppIdx;
	LookUpStockCode( nStockIdx, nStockSuppIdx );
	UpdateStockCode( nStockIdx, nStockSuppIdx );
	
	UpdateData ( FALSE );

	UpdateItemKey( FALSE, TRUE, TRUE, TRUE );

	switch ( m_nEntryMode )
	{
	case ENTRY_MODE_SUPPREF:
	case ENTRY_MODE_WRAPPER:
	case ENTRY_MODE_PLUNO:
		GetEditQty() -> SetFocus();
		GetEditQty() -> SetSel(0,-1);
		break;

	case ENTRY_MODE_STOCKCODE:
	default:
		m_editStockCode.SetFocus();
		m_editStockCode.SetSel ( 0, -1 );
		break;
	}
}

/**********************************************************************/

void CReturnsEditDlg::OnRadioSuppRef() 
{
	m_radioPluNumber.SetCheck( FALSE );
	m_radioStockCode.SetCheck( FALSE );
	m_radioSuppRef.SetCheck( TRUE );

	GetEditPluNo() -> EnableWindow( FALSE );
	m_editStockCode.EnableWindow( FALSE );
	m_editSuppRef.EnableWindow( TRUE );

	m_nEntryMode = ENTRY_MODE_SUPPREF;

	m_editSuppRef.SetFocus();
	m_editSuppRef.SetSel( 0, -1 );
}

/**********************************************************************/

void CReturnsEditDlg::OnRadioPluNumber() 
{
	m_radioPluNumber.SetCheck( TRUE );
	m_radioStockCode.SetCheck( FALSE );
	m_radioSuppRef.SetCheck( FALSE );

	GetEditPluNo() -> EnableWindow( TRUE );
	m_editStockCode.EnableWindow( FALSE );
	m_editSuppRef.EnableWindow( FALSE );

	m_nEntryMode = ENTRY_MODE_PLUNO;

	GetEditPluNo() -> SetFocus();
	GetEditPluNo() -> SetSel( 0, -1 );
}

/**********************************************************************/

void CReturnsEditDlg::OnRadioStockCode() 
{
	m_radioPluNumber.SetCheck( FALSE );
	m_radioStockCode.SetCheck( TRUE );
	m_radioSuppRef.SetCheck( FALSE );

	GetEditPluNo() -> EnableWindow( FALSE );
	m_editStockCode.EnableWindow( TRUE );
	m_editSuppRef.EnableWindow( FALSE );

	m_nEntryMode = ENTRY_MODE_STOCKCODE;

	m_editStockCode.SetFocus();
	m_editStockCode.SetSel( 0, -1 );
}

/**********************************************************************/

void CReturnsEditDlg::OnOK() 
{	
	m_buttonOK.SetFocus();

	if ( UpdateData( TRUE ) == FALSE )
		return;

	int nResult = HandleAdd();
	
	CEdit* pNewFocus = NULL;

	switch ( nResult )
	{
	case 1:
		m_pParentWnd -> EnableWindow( FALSE );
		Prompter.Error ( "The selected stock code does not exist in your database" );
		m_pParentWnd -> EnableWindow( TRUE );
		break;

	case 2:
		m_pParentWnd -> EnableWindow( FALSE );
		Prompter.Error ( "Please enter a non-zero quantity" );
		m_pParentWnd -> EnableWindow( TRUE );
		pNewFocus = GetEditQty();
		break;

	case 3:
		m_pParentWnd -> EnableWindow( FALSE );
		Prompter.Error ( "The selected stock code is not in this stocktake" );
		m_pParentWnd -> EnableWindow( TRUE );
		break;

	case 4:
		m_pParentWnd -> EnableWindow( FALSE );
		Prompter.Error ( "The selected stock item is not active at this stockpoint" );
		m_pParentWnd -> EnableWindow( TRUE );
		break;

	default:
		if ( m_bAdd == TRUE )
			GetParent() -> PostMessage ( WM_APP, 0, (long) this );
		else
		{
			EndDialog( IDOK );
			return;
		}
		break;
	}

	if ( pNewFocus == NULL )
	{
		switch ( m_nEntryMode )
		{
		case ENTRY_MODE_SUPPREF:	
		case ENTRY_MODE_WRAPPER:		pNewFocus = &m_editSuppRef;	break;
		case ENTRY_MODE_PLUNO:		pNewFocus = GetEditPluNo();	break;
		case ENTRY_MODE_STOCKCODE:	
		default:					pNewFocus = &m_editStockCode;	break;
		}
	}

	pNewFocus -> SetFocus();
	pNewFocus -> SetSel( 0, -1 );
}

/**********************************************************************/

int CReturnsEditDlg::HandleAdd() 
{
	m_buttonOK.SetFocus();

	UpdateItemKey( TRUE, TRUE, TRUE, TRUE );

	int nStockIdx;
	if ( DataManager.Stock.FindStockItemByCode( m_StockItemKey.m_strStockCode, nStockIdx ) == FALSE )
		return 1;
	
	if ( m_dQty == 0.0 )
		return 2;

	CStockCSVRecord StockRecord;
	CStockLevelsCSVRecord StockLevels;
	switch( DataManager.Stock.CheckStockLevelsForImport( nStockIdx, StockRecord, StockLevels, dbStockpoint.GetSpNo( m_nSpIdx ), m_nStkTakeNo ) )
	{
	case STOCKLEVEL_IMPORT_REJECT_STOCKTAKE:
		return 3;

	case STOCKLEVEL_IMPORT_REJECT_INACTIVE:
		return 4;
	}
			
	CDelUnitInfo DelUnitInfo;
	m_DelUnitSelector.GetDelUnitInfo( DelUnitInfo );

	bool bUseDU = FALSE;
	CString strSuppRef = "";
	CString strWrapper = "";

	int nDUItems = 0;
	double dDUItemSize = 0.0;

	double dDUQty = 0.0;
	double dTotalQty = m_dQty;
	
	double dDUCost = 0.0;
	double dTotalCost = 0.0;

	if ( DelUnitInfo.m_bEditable )
	{
		bUseDU = TRUE;
		
		dDUQty = m_dQty;
		dTotalQty *= DelUnitInfo.m_dDUYield;

		nDUItems = DelUnitInfo.m_nDUItems;
		dDUItemSize = DelUnitInfo.m_dDUItemSize;
		
		//EDITABLE UNIT MUST BELONG TO THIS SUPPLIER SO IT IS SAFE TO
		//TAKE THE SUPPLIER REFERENCE AS WELL AS THE UNIT SIZE
		strSuppRef = StockRecord.GetSuppRefExternal( DelUnitInfo.m_nStockSuppIdx, DealerFlags.GetSuppRefFlag() );		
		strWrapper = StockRecord.GetWrapper( DelUnitInfo.m_nStockSuppIdx );
	}
	else if ( DelUnitInfo.m_nComboPos > 0 )
	{
		bUseDU = TRUE;
		
		nDUItems = DelUnitInfo.m_nDUItems;
		dDUItemSize = DelUnitInfo.m_dDUItemSize;
		
		dDUQty = dTotalQty;
		dTotalQty *= DelUnitInfo.m_dDUYield;

		//UNSPECIFIED SUPPLIER CAN USE SUPPLIER REFERENCES
		if ( m_nSuppNo <= 0 )
		{
			int nStockSuppIdx = DelUnitInfo.m_nStockSuppIdx;
			if ( nStockSuppIdx >= 0 && nStockSuppIdx < StockRecord.GetSupplierCount() )
			{
				strSuppRef = StockRecord.GetSuppRefExternal( nStockSuppIdx, DealerFlags.GetSuppRefFlag() );		
				strWrapper = StockRecord.GetWrapper( nStockSuppIdx );
			}
		}
	}
		
	dTotalCost = m_dCost;
	if ( dDUQty != 0.0 ) dDUCost = ( m_dCost / dDUQty );
	
	CString strPluNo;
	::FormatPluNo( m_StockItemKey.m_nPluNo, strPluNo );

	CCSVStockAudit csvOut;
	csvOut.Add ( strPluNo );
	csvOut.Add ( strSuppRef );			
	csvOut.Add ( DataManager.Stock.GetStockCode( nStockIdx ) );
	csvOut.Add ( DataManager.Stock.GetDescription( nStockIdx ) );
	csvOut.Add ( FALSE );	//FREE STOCK FLAG
	csvOut.Add ( bUseDU );
	csvOut.AddStockAuditQty ( dDUQty );
	csvOut.Add ( nDUItems );
	csvOut.AddStockAuditQty ( dDUItemSize );
	csvOut.AddStockAuditQty ( dTotalQty );
	csvOut.AddStockAuditVal ( dDUCost );
	csvOut.AddStockAuditVal ( dTotalCost );
	csvOut.Add ( m_nEntryMode );
	csvOut.Add ( DelUnitInfo.m_nSuppNo );
	csvOut.Add ( strWrapper );

	m_strOutputLine = csvOut.GetLine();

	return 0;
}

/**********************************************************************/

void CReturnsEditDlg::OnCancel() 
{
	if ( m_bAdd == TRUE )
		DestroyWindow();
	else
		EndDialog( IDCANCEL );
}

/**********************************************************************/

void CReturnsEditDlg::PostNcDestroy() 
{
	if ( m_bAdd == TRUE )
	{
		*m_pParentToThis = NULL;
		delete this;
	}
}

/**********************************************************************/
