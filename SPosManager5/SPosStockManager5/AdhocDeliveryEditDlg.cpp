/**********************************************************************/
#include "AdhocDeliveryUnitDlg.h"
#include "AdhocPreferredSupplierDlg.h"
#include "BarcodePluNoTable.h"
#include "FilterTypes.h"
#include "MessageLogger.h"
#include "PluCreateLinkDlg.h"
#include "PluFilterDlgStockman.h"
#include "BarcodePluNoTable.h"
#include "PluNumberFullStockDlg.h"
#include "PriceHelpers.h"
#include "PropertySheetPluStock.h"
#include "PropertySheetPluStock.h"
#include "PropertySheetStock.h"
#include "ReportHelpers.h"
#include "StockFilterArray.h"
#include "StockFilterDlg.h"
#include "StockOptionsIni.h"
#include "StockPluArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "AdhocDeliveryEditDlg.h"
/**********************************************************************/

CAdhocDeliveryEditDlg::CAdhocDeliveryEditDlg( int nSuppIdx, int nSpIdx, CSuppRefFinder& SuppRefFinder, int& nNewPluDeptNo, CAdhocDeliveryEditDlg** pParentToThis, int nStkTakeNo, CWnd* pParent)
	: CSSDialog("IDD_ADHOC_DELIVERY_EDIT", pParent), m_FilterArray(FILTERTYPE_BROWSE_PLU), m_StockFilterArray(FILTERTYPE_BROWSE_STOCK), m_SuppRefFinder(SuppRefFinder), m_nNewPluDeptNo(nNewPluDeptNo)
{
	m_nStkTakeNo = nStkTakeNo;

	DataInit( nSuppIdx, nSpIdx );

	m_bAdd = TRUE;
	m_strAction = "Add";

	m_pParentToThis = pParentToThis;
	*m_pParentToThis = this;
}

/**********************************************************************/

CAdhocDeliveryEditDlg::CAdhocDeliveryEditDlg( int nSuppIdx, int nSpIdx, CSuppRefFinder& SuppRefFinder, int& nNewPluDeptNo, int nStkTakeNo, CWnd* pParent)
	: CSSDialog("IDD_ADHOC_DELIVERY_EDIT", pParent), m_FilterArray ( FILTERTYPE_BROWSE_PLU ), m_StockFilterArray ( FILTERTYPE_BROWSE_STOCK ), m_SuppRefFinder( SuppRefFinder ), m_nNewPluDeptNo( nNewPluDeptNo )
{	
	m_nStkTakeNo = nStkTakeNo;

	DataInit( nSuppIdx, nSpIdx );
	m_bAdd = FALSE;
	m_strAction = "Edit";
}

/**********************************************************************/

void CAdhocDeliveryEditDlg::DataInit( int nSuppIdx, int nSpIdx )
{
	//{{AFX_DATA_INIT(CAdhocDeliveryEditDlg)
	m_dQty = 0.0;
	m_bFreeStock = FALSE;
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

void CAdhocDeliveryEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdhocDeliveryEditDlg)
	DDX_Control(pDX, IDC2_BUTTON_NEW, m_buttonNew);
	DDX_Control(pDX, IDC2_EDIT_DESCRIPTION_PLU, m_editDescriptionPlu);
	DDX_Control(pDX, IDC2_BUTTON_EDIT, m_buttonEdit);
	DDX_Control(pDX, IDC2_STATIC_GPTYPE, m_staticGPType);
	DDX_Control(pDX, IDC2_EDIT_PRICE1, m_editPrice1);
	DDX_Control(pDX, IDC2_EDIT_GP, m_editGP);
	DDX_Control(pDX, IDC2_COMBO_RETAIL, m_comboRetail);
	DDX_Control(pDX, IDC2_BUTTON_BROWSE_PLU, m_buttonBrowsePlu);
	DDX_Control(pDX, IDC2_BUTTON_BROWSE, m_buttonBrowseStock);
	DDX_Control(pDX, IDC2_RADIO_SUPPREF, m_radioSuppRef);
	DDX_Control(pDX, IDC2_RADIO_STOCKCODE, m_radioStockCode);
	DDX_Control(pDX, IDC2_RADIO_PLUNUMBER, m_radioPluNumber);
	DDX_Control(pDX, IDC2_EDIT_SUPPREF, m_editSuppRef);
	DDX_Control(pDX, IDC2_COMBO_DELUNIT, m_comboDeliveryUnit);
	DDX_Control(pDX, IDC2_EDIT_STOCKCODE, m_editStockCode);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Check(pDX, IDC2_CHECK_FREESTOCK, m_bFreeStock);
	DDX_Text(pDX, IDC2_EDIT_DESCRIPTION, m_strDescription );
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC2_EDIT_QTY_DOUBLE, m_dQty );
	DDX_Text(pDX, IDC2_EDIT_COST, m_dCost );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CAdhocDeliveryEditDlg, CDialog)
	//{{AFX_MSG_MAP(CAdhocDeliveryEditDlg)
	ON_EN_KILLFOCUS(IDC2_EDIT_STOCKCODE, OnKillFocusStockCode)
	ON_BN_CLICKED(IDC2_BUTTON_BROWSE, OnButtonBrowseStock)
	ON_EN_KILLFOCUS(IDC2_EDIT_PLUNO, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC2_BUTTON_BROWSE_PLU, OnButtonBrowsePlu)
	ON_BN_CLICKED(IDC2_CHECK_FREESTOCK, OnButtonFreeStock)
	ON_EN_KILLFOCUS(IDC2_EDIT_QTY_DOUBLE, OnKillFocusQty)
	ON_EN_KILLFOCUS(IDC2_EDIT_SUPPREF, OnKillFocusSuppRef)
	ON_BN_CLICKED(IDC2_RADIO_PLUNUMBER, OnRadioPluNumber)
	ON_BN_CLICKED(IDC2_RADIO_STOCKCODE, OnRadioStockCode)
	ON_BN_CLICKED(IDC2_RADIO_SUPPREF, OnRadioSuppRef)
	ON_CBN_SELCHANGE(IDC2_COMBO_RETAIL, OnSelectRetailPlu)
	ON_BN_CLICKED(IDC2_BUTTON_EDIT, OnButtonEdit)
	ON_CBN_SELCHANGE(IDC2_COMBO_DELUNIT, OnSelectDelUnit)
	ON_BN_CLICKED(IDC2_BUTTON_NEW, OnButtonEditDeliveryUnit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CAdhocDeliveryEditDlg::GetEditQty() { return GetEdit( IDC2_EDIT_QTY_DOUBLE ); }
CEdit* CAdhocDeliveryEditDlg::GetEditCost() { return GetEdit( IDC2_EDIT_COST ); }
CEdit* CAdhocDeliveryEditDlg::GetEditPluNo() { return GetEdit( IDC2_EDIT_PLUNO ); }
/**********************************************************************/

BOOL CAdhocDeliveryEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit ( IDC2_EDIT_QTY_DOUBLE, SS_NUM_DP, 8, "%.5f" );
	SubclassEdit ( IDC2_EDIT_COST, SS_NUM_DP, 8, "%.*f", SysInfo.GetDPValue() );
	SubclassEdit ( IDC2_EDIT_PLUNO, SS_NUM, Super.MaxPluLen(), "%s" );

	m_editStockCode.LimitText( Stock::StockCode.Max );
	m_editSuppRef.LimitText( Stock::SupplierRef.Max );
	
	CString strTitle;
	strTitle.Format ( "%s Delivery Line (%s, %s)", 
		(const char*) m_strAction,
		dbStockpoint.GetName( m_nSpIdx ), 
		(const char*) m_strSupplier );
	
	SetWindowText ( strTitle );

	if ( m_bAdd == TRUE )
	{
		OnRadioSuppRef();
		FillDeliveryUnitCombo( -1, -1 );
		FillRetailCombo( -1 );
		m_buttonNew.EnableWindow( FALSE );
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

		if (m_nEntryMode != ENTRY_MODE_WRAPPER)
		{
			m_StockItemKey.m_strSuppRef = csvIn.GetSuppRef();
		}
		else
		{
			m_StockItemKey.m_strSuppRef = csvIn.GetWrapperCode();
		}

		if (csvIn.GetUseDUFlag() == TRUE)
		{
			m_dQty = csvIn.GetDUQty();
		}
		else
		{
			m_dQty = csvIn.GetSUQty();
		}

		m_dCost = csvIn.GetTotalCost();
		UpdateData( FALSE );
		UpdateItemKey( FALSE, TRUE, TRUE, TRUE );
		
		int nStockIdx;
		if ( DataManager.Stock.FindStockItemByCode( m_StockItemKey.m_strStockCode, nStockIdx ) == TRUE )
		{
			FillDeliveryUnitCombo( nStockIdx, -1 );
			FillRetailCombo( nStockIdx );
			m_buttonNew.EnableWindow( TRUE );
		}
		else
		{
			FillDeliveryUnitCombo( -1, -1 );
			FillRetailCombo( -1 );
			m_buttonNew.EnableWindow( FALSE );
		}
		
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

void CAdhocDeliveryEditDlg::UpdateItemKey( bool bFromControl, bool bPluNo, bool bStockCode, bool bSuppRef )
{
	if ( FALSE == bFromControl )
	{
		CString strPluNo;
		::FormatPluNo( m_StockItemKey.m_nPluNo, strPluNo );

		if ( TRUE == bPluNo )
			GetEditPluNo() -> SetWindowText( strPluNo );

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

bool CAdhocDeliveryEditDlg::LookUpSuppRef( int& nStockIdx, int& nStockSuppIdx )
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
		DataManager.StockPlu.Rebuild();
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

int CAdhocDeliveryEditDlg::LookUpPluNo( int& nStockIdx, int& nStockSuppIdx )
{
	m_StockItemKey.m_strSuppRef = "";
	m_StockItemKey.m_strStockCode = "";
	
	nStockIdx = -1;
	nStockSuppIdx = -1;

	if ( 0 == m_StockItemKey.m_nPluNo )
		return PLULOOKUP_NOPLU;

	int nPluIdx;
	if ( DataManager.Plu.FindPluByNumber( m_StockItemKey.m_nPluNo, nPluIdx ) == FALSE )
		return PLULOOKUP_NOPLU;

	DataManager.Plu.GetPluStockCode ( m_StockItemKey.m_nPluNo, m_StockItemKey.m_strStockCode, FALSE );
	if ( "" == m_StockItemKey.m_strStockCode )
		return PLULOOKUP_NOLINK;

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

		if ( ( nStockSuppIdx == -1 ) && ( StockRecord.GetSupplierCount() > 0 ) )
			nStockSuppIdx = 0;
	}
	
	return PLULOOKUP_GOTLINK;
}

/**********************************************************************/

bool CAdhocDeliveryEditDlg::LookUpStockCode( int& nStockIdx, int& nStockSuppIdx )
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

void CAdhocDeliveryEditDlg::FillDeliveryUnitCombo( int nStockIdx, int nStockSuppIdx )
{
	m_DelUnitSelector.FillComboBox ( &m_comboDeliveryUnit, nStockIdx, m_nSuppNo );
	
	if ( nStockSuppIdx >= 0 )
		m_DelUnitSelector.SelectStockSuppIdx( nStockSuppIdx );
	else
		m_comboDeliveryUnit.SetCurSel(0);
}

/**********************************************************************/

void CAdhocDeliveryEditDlg::FillRetailCombo( int nStockIdx )
{
	m_comboRetail.ResetContent();

	if ( nStockIdx >= 0 && nStockIdx < DataManager.Stock.GetSize() )
	{
		int nStockPluIdx;
		CString strStockCode = DataManager.Stock.GetStockCode( nStockIdx );
		
		if ( DataManager.StockPlu.FindStockCode( strStockCode, nStockPluIdx ) == TRUE )
		{
			for ( int n = 0; n < DataManager.StockPlu.GetPluNoCount( nStockPluIdx ); n++ )
			{
				__int64 nPluNo = DataManager.StockPlu.GetPluNo( nStockPluIdx, n );

				int nPluIdx;
				if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == TRUE )
				{
					CString strPluNo;
					::FormatPluNo( nPluNo, strPluNo );
					int nPos = m_comboRetail.AddString( strPluNo );
					m_comboRetail.SetItemData( nPos, nPluIdx );
				}
			}
		}
	}

	if ( m_comboRetail.GetCount() >= 1 )
	{
		m_comboRetail.SetCurSel(0);
	
		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber( m_StockItemKey.m_nPluNo, nPluIdx ) == TRUE )
		{
			for ( int nSel = 0; nSel < m_comboRetail.GetCount(); nSel++ )
			{
				if ( m_comboRetail.GetItemData( nSel ) == (DWORD) nPluIdx )
				{
					m_comboRetail.SetCurSel( nSel );
					break;
				}
			}
		}
	}

	UpdateRetailPlu();
	
	m_comboRetail.EnableWindow( m_comboRetail.GetCount() >= 2 );
}

/**********************************************************************/

void CAdhocDeliveryEditDlg::UpdateRetailPlu()
{
	bool bPluOK = FALSE;

	int nSel = m_comboRetail.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboRetail.GetCount() )
	{
		int nPluIdx = m_comboRetail.GetItemData( nSel );
		if ( nPluIdx >= 0 && nPluIdx < DataManager.Plu.GetSize() )
		{
			bPluOK = TRUE;
			
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt( nPluIdx, PluRecord );
			
			CString strPrice1 = "";
			CString strProfitMargin = "";
			CString strProfitType = "";

			CTaxRateInfo TaxRateInfo;
			DataManager.TaxRates.GetTaxRateInfo( PluRecord.GetBaseTaxBandInt(), TaxRateInfo );

			double dCost = DataManager.Plu.GetPluPurchaseCost( PluRecord );

			switch ( PluRecord.GetGPType() )
			{
			case PLU_GPTYPE_FIXED:
				{
					double dProfitMargin = PluRecord.GetProfitMargin();
					double dPrice1 = CPriceHelpers::GetPrice1GPFixed ( TaxRateInfo.m_dTaxRate, dCost, dProfitMargin );
					strPrice1.Format ( "%.*f", SysInfo.GetDPValue(), dPrice1 );
					strProfitMargin.Format ( "%.*f", SysInfo.GetDPValue(), dProfitMargin );
					strProfitType = "Fixed GP";
				}
				break;

			case PLU_GPTYPE_PERCENT:
				{
					double dProfitMargin = PluRecord.GetProfitMargin();
					double dPrice1 = CPriceHelpers::GetPrice1GPPercent ( TaxRateInfo.m_dTaxRate, dCost, dProfitMargin );
					strPrice1.Format ( "%.*f", SysInfo.GetDPValue(), dPrice1 );
					strProfitMargin.Format ( "%.2f", dProfitMargin );
					strProfitType = "Fixed GP%";
				}
				break;

			case PLU_GPTYPE_VARIABLE:
			default:
				{
					double dPrice1 = PluRecord.GetPrice(0);
					double dProfitMargin = ReportHelpers.CalcGP ( dPrice1, TaxRateInfo.m_dTaxRate, dCost );
					strPrice1.Format ( "%.*f", SysInfo.GetDPValue(), dPrice1 );
					strProfitMargin.Format ( "%.*f", SysInfo.GetDPValue(), dProfitMargin );
					strProfitType = "Variable GP";
				}
				break;
			}

			m_staticGPType.SetWindowText( strProfitType );
			m_editGP.SetWindowText( strProfitMargin );
			m_editPrice1.SetWindowText( strPrice1 );
			m_editDescriptionPlu.SetWindowText( ReportHelpers.GetReportText ( PluRecord ) );
			m_buttonEdit.EnableWindow( TRUE );
		}
	}

	if ( bPluOK == FALSE )
	{
		m_editDescriptionPlu.SetWindowText ( "" );
		m_buttonEdit.EnableWindow( FALSE );
		m_staticGPType.SetWindowText ( "Variable GP" );
		m_editGP.SetWindowText ( "" );
		m_editPrice1.SetWindowText( "" );
	}
}

/**********************************************************************/

void CAdhocDeliveryEditDlg::OnSelectRetailPlu() 
{
	UpdateRetailPlu();
}

/**********************************************************************/

void CAdhocDeliveryEditDlg::OnKillFocusSuppRef() 
{
	UpdateData ( TRUE, FALSE );
	
	int nStockIdx, nStockSuppIdx;
	
	UpdateItemKey( TRUE, FALSE, FALSE, TRUE );
	LookUpSuppRef( nStockIdx, nStockSuppIdx );
	UpdateItemKey( FALSE, TRUE, TRUE, FALSE );

	if ( ( nStockIdx != -1 ) && ( nStockSuppIdx > 0 ) && ( m_nSuppNo > 0 ) )
	{
		bool bMakePreferred = FALSE;
		
		switch( StockOptions.GetStockAdhocSupplierChangeType() )
		{
		case 1:
			bMakePreferred = TRUE;
			break;

		case 2:
			{
				m_pParentWnd -> EnableWindow( FALSE );
				CAdhocPreferredSupplierDlg dlg ( nStockIdx, nStockSuppIdx, this );
				bMakePreferred = ( dlg.DoModal() == IDOK );
				m_pParentWnd -> EnableWindow( TRUE );
			}
			break;
		}

		if ( bMakePreferred == TRUE )
		{
			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt( nStockIdx, StockRecord );
			StockRecord.MakePreferred( nStockSuppIdx );
			DataManager.Stock.SetAt( nStockIdx, StockRecord );
			m_SuppRefFinder.HandlePreferredSupplierChange( nStockIdx );
			nStockSuppIdx = 0;
		}
	}

	UpdateStockCode( nStockIdx, nStockSuppIdx );
	m_bFreeStock = FALSE;
	UpdateData(FALSE);
	
	//WE **MUST** DO THIS HERE BECAUSE WE ARE TABBING FROM A STANDARD (NON-CSS)
	//EDIT THAT DOES NOT IMPLEMENT PAUL'S SETSEL FIX
	GetEditQty() -> SetSel( 0, -1 );
}

/**********************************************************************/

void CAdhocDeliveryEditDlg::OnKillFocusPluNo() 
{
	UpdateData ( TRUE, FALSE );

	UpdateItemKey( TRUE, TRUE, FALSE, FALSE );
	UpdateItemKey( FALSE, TRUE, FALSE, FALSE );
	
	int nStockIdx, nStockSuppIdx;
	switch ( LookUpPluNo( nStockIdx, nStockSuppIdx ) )
	{
	case PLULOOKUP_NOPLU:
		UpdateItemKey( FALSE, TRUE, TRUE, TRUE );

		if ( m_nStkTakeNo <= 0 )
		{
			if ( CreatePluRecord ( m_StockItemKey.m_nPluNo ) == TRUE )
			{
				m_SuppRefFinder.Invalidate();
				LookUpPluNo( nStockIdx, nStockSuppIdx );
			}
		}
		break;

	case PLULOOKUP_NOLINK:
		UpdateItemKey( FALSE, TRUE, TRUE, TRUE );

		if ( m_nStkTakeNo <= 0 )
		{
			if ( LinkPluRecord ( m_StockItemKey.m_nPluNo ) == TRUE )
			{
				m_SuppRefFinder.Invalidate();
				LookUpPluNo( nStockIdx, nStockSuppIdx );
			}
		}
		break;

	case PLULOOKUP_GOTLINK:
	default:
		break;
	}

	UpdateItemKey( FALSE, TRUE, TRUE, TRUE );

	UpdateStockCode( nStockIdx, nStockSuppIdx );
	m_bFreeStock = FALSE;
	UpdateData(FALSE);
}

/**********************************************************************/

void CAdhocDeliveryEditDlg::OnKillFocusStockCode() 
{
	UpdateData ( TRUE, FALSE );

	UpdateItemKey( TRUE, FALSE, TRUE, FALSE );
	m_StockItemKey.m_strStockCode = BarcodePluNoTable.ConvertStockCode( m_StockItemKey.m_strStockCode );
	UpdateItemKey( FALSE, FALSE, TRUE, FALSE );

	int nStockIdx, nStockSuppIdx;
	LookUpStockCode( nStockIdx, nStockSuppIdx );
	UpdateStockCode( nStockIdx, nStockSuppIdx );
	m_bFreeStock = FALSE;
	UpdateData(FALSE);

	UpdateItemKey( FALSE, TRUE, TRUE, TRUE );
	
	//WE **MUST** DO THIS HERE BECAUSE WE ARE TABBING FROM A STANDARD (NON-CSS)
	//EDIT THAT DOES NOT IMPLEMENT PAUL'S SETSEL FIX
	GetEditQty() -> SetSel( 0, -1 );
}

/**********************************************************************/

void CAdhocDeliveryEditDlg::UpdateStockCode( int nStockIdx, int nStockSuppIdx )
{		
	if ( nStockIdx != -1 )
	{
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( nStockIdx, StockRecord );
		m_strDescription = StockRecord.GetDescription();
		m_buttonNew.EnableWindow( TRUE );
	}
	else
	{
		m_strDescription = "Unknown";
		m_buttonNew.EnableWindow( FALSE );
	}

	FillDeliveryUnitCombo( nStockIdx, nStockSuppIdx );
	FillRetailCombo( nStockIdx );

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

bool CAdhocDeliveryEditDlg::CreatePluRecord( __int64 nPluNo )
{
	m_pParentWnd -> EnableWindow( FALSE );
	bool bResult = CreatePluRecordInternal( nPluNo );
	m_pParentWnd -> EnableWindow( TRUE );
	return bResult;
}

/**********************************************************************/

bool CAdhocDeliveryEditDlg::CreatePluRecordInternal( __int64 nPluNo )
{
	if ( SysInfo.IsValidSSPluNo( nPluNo ) == FALSE )
	{
		int nMaxBaseLen = SysInfo.GetMaxBasePluLen();

		CString strPluNo;
		strPluNo.Format( "%I64d", nPluNo );

		if ( ( strPluNo.GetLength() == nMaxBaseLen + 1 ) && ( nMaxBaseLen != 0 ) )
		{
			CString strMessage;
			strMessage.Format( "%s is not configured to allow %d digit PLU items.", 
				AfxGetAppName(),
				nMaxBaseLen + 1 );
					
			Prompter.ContactDealer( strMessage );
		}
		
		return FALSE;
	}

	CString strPluNo;
	::FormatPluNo( nPluNo, strPluNo );

	CString strMsg;
	strMsg.Format ( "Plu number %s does not exist in your database\n\nWould you like to create this Plu number now",
		(const char*) strPluNo );

	if ( Prompter.YesNo ( strMsg, TRUE ) == IDNO )
		return FALSE;

	CPluAddOrCopyInfo PluInfo;
	PluInfo.m_nNewPluSuppNo = m_nSuppNo;
	PluInfo.m_nNewPluDeptNo = m_nNewPluDeptNo;
	PluInfo.m_strNewPluNo = strPluNo;
	
	CPluNumberFullStockDlg StockCodeDlg( FALSE, PluInfo, this );
	
	if ( StockCodeDlg.DoModal() == IDCANCEL )
		return FALSE;

	if ( PluInfo.m_strNewPluStockCode == "" )
		return FALSE;

	int nStockIdx;
	bool bNewStock = FALSE;
	if ( DataManager.Stock.FindStockItemByCode ( PluInfo.m_strNewPluStockCode, nStockIdx ) == FALSE )
	{
		CStockCSVRecord StockRecord;
		StockRecord.SetStockCode ( PluInfo.m_strNewPluStockCode );
		StockRecord.SetDescription ( "Plu" );
		StockRecord.SetCategory ( 1 );
		DataManager.Stock.InsertRecord ( StockRecord, nStockIdx );		
		bNewStock = TRUE;
	}

	//LINK THE STOCK RECORD TO THIS SUPPLIER IF IT DOES NOT
	//HAVE A SUPPLIER ALREADY
	if ( m_nSuppNo >= 1 )
	{
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( nStockIdx, StockRecord );
		
		if ( StockRecord.GetSupplierCount() == 0 )
		{
			StockRecord.SetSuppNo( 0, m_nSuppNo );
			StockRecord.TidySuppliers( StockOptions.GetStockMaxSuppliersPerItem() );
			DataManager.Stock.SetAt( nStockIdx, StockRecord );
		}
	}

	m_nNewPluDeptNo = PluInfo.m_nNewPluDeptNo;

	CPluCSVRecord PluRecord;
	PluRecord.SetPluNo ( nPluNo );
	
	if ( DataManager.Plu.CopyDefaultPlu( m_nNewPluDeptNo, PluRecord ) == FALSE )
		PluRecord.SetDefaultText();

	PluRecord.SetBaseDeptNo( m_nNewPluDeptNo );
	PluRecord.SetModifiedDateTimePlu();

	int nInsertionPoint = DataManager.Plu.InsertRecord ( PluRecord, PluInfo.m_strNewPluStockCode, TRUE );
	DataManager.StockPlu.AddLink( PluInfo.m_strNewPluStockCode, PluRecord.GetPluNoInt() );

	CString strPluTemp = PluRecord.GetPluNoString();
	::TrimSpaces( strPluTemp, TRUE );

	CStringArray strArray;
	strArray.Add( strPluTemp );
	strArray.Add( PluInfo.m_strNewPluStockCode );
	MessageLogger.LogSystem ( 1, 5, FALSE, strArray );
	
	CPluFilterArray TempFilter ( FILTERTYPE_DATABASE );

	TempFilter.SetArrayIdx ( nInsertionPoint );

	CPropertySheetPluStockInfo info;
	info.m_bEnableSpin = FALSE;
	info.m_wSpinID = IDC_SPIN;
	info.m_pPluFilter = &TempFilter;
	info.m_nStockIdx = nStockIdx;
	info.m_bNewStock = bNewStock;
	info.m_bStockLevelTab = TRUE;
	info.m_nDefaultSuppNo = m_nSuppNo;
	info.m_bReadOnly = FALSE;
	info.m_nSpIdxToEdit = m_nSpIdx;
	info.m_bSingleSpIdx = TRUE;

	CPropertySheetPluStock propSheet ( info, this );	
	
	propSheet.EnableStackedTabs ( FALSE );

	if ( propSheet.DoModal() == IDOK )
		propSheet.SavePluRecord();
	else
		propSheet.UndoChanges();
	
	return TRUE;
}

/**********************************************************************/

bool CAdhocDeliveryEditDlg::LinkPluRecord( __int64 nPluNo )
{
	m_pParentWnd -> EnableWindow( FALSE );
	bool bResult = LinkPluRecordInternal( nPluNo );
	m_pParentWnd -> EnableWindow( TRUE );
	return bResult;
}

/**********************************************************************/

bool CAdhocDeliveryEditDlg::LinkPluRecordInternal( __int64 nPluNo )
{
	int nPluIdx;
	if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == FALSE )
		return FALSE;

	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt( nPluIdx, PluRecord );

	CString strMsg;
	strMsg.Format ( "Plu number %s (%s) is not linked to a stock item\n\nWould you like to link this Plu number to a stock item now",
		PluRecord.GetPluNoString(),
		ReportHelpers.GetReportText( PluRecord ) );

	if ( Prompter.YesNo ( strMsg, TRUE ) == IDNO )
		return FALSE;

	CPluCreateLinkDlg dlg( PluRecord );
	if ( dlg.DoModal() == IDCANCEL )
		return FALSE;

	CString strStockCode = dlg.m_strStockCode;
	if ( strStockCode == "" )
		return FALSE;

	int nStockIdx;
	bool bNewStock = FALSE;
	if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == FALSE )
	{
		CStockCSVRecord StockRecord;
		StockRecord.SetStockCode ( strStockCode );
		StockRecord.SetDescription ( "Plu" );
		StockRecord.SetCategory ( 1 );
		DataManager.Stock.InsertRecord ( StockRecord, nStockIdx );		
		bNewStock = TRUE;
	}

	//LINK THE STOCK RECORD TO THIS SUPPLIER IF IT DOES NOT
	//HAVE A SUPPLIER ALREADY
	if ( m_nSuppNo >= 1 )
	{
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( nStockIdx, StockRecord );
		
		if ( StockRecord.GetSupplierCount() == 0 )
		{
			StockRecord.SetSuppNo( 0, m_nSuppNo );
			StockRecord.TidySuppliers( StockOptions.GetStockMaxSuppliersPerItem() );
			DataManager.Stock.SetAt( nStockIdx, StockRecord );
		}
	}

	CRecipeCSVRecord Recipe;
	Recipe.SetRecipePluNo( PluRecord.GetPluNoString() );
	Recipe.AddStockCode( strStockCode, 1.0 );
	DataManager.Plu.SetRecipe( nPluIdx, Recipe );

	DataManager.StockPlu.AddLink( strStockCode, PluRecord.GetPluNoInt() );

	CString strPluTemp = PluRecord.GetPluNoString();
	::TrimSpaces( strPluTemp, TRUE );

	CStringArray strArray;
	strArray.Add( strPluTemp );
	strArray.Add( strStockCode );
	MessageLogger.LogSystem ( 1, 6, FALSE, strArray );

	CPluFilterArray TempFilter ( FILTERTYPE_DATABASE );

	TempFilter.SetArrayIdx ( nPluIdx );

	CPropertySheetPluStockInfo info;
	info.m_bEnableSpin = FALSE;
	info.m_wSpinID = IDC_SPIN;
	info.m_pPluFilter = &TempFilter;
	info.m_nStockIdx = nStockIdx;
	info.m_bNewStock = bNewStock;
	info.m_bStockLevelTab = TRUE;
	info.m_nDefaultSuppNo = m_nSuppNo;
	info.m_bReadOnly = FALSE;
	info.m_nSpIdxToEdit = m_nSpIdx;

	CPropertySheetPluStock propSheet ( info, this );	

	propSheet.EnableStackedTabs ( FALSE );

	if ( propSheet.DoModal() == IDOK )
		propSheet.SavePluRecord();
	else
		propSheet.UndoChanges();
	
	m_StockItemKey.m_nPluNo = PluRecord.GetPluNoInt();
	return TRUE;
}

/**********************************************************************/

void CAdhocDeliveryEditDlg::OnSelectDelUnit() 
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

void CAdhocDeliveryEditDlg::OnKillFocusQty() 
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

void CAdhocDeliveryEditDlg::OnButtonBrowsePlu() 
{
	m_pParentWnd -> EnableWindow( FALSE );
	BrowsePluInternal();
	m_pParentWnd -> EnableWindow( TRUE );
}

/**********************************************************************/

void CAdhocDeliveryEditDlg::BrowsePluInternal() 
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
	m_bFreeStock = FALSE;
	
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

void CAdhocDeliveryEditDlg::OnButtonBrowseStock() 
{
	m_pParentWnd -> EnableWindow( FALSE );
	BrowseStockInternal();
	m_pParentWnd -> EnableWindow( TRUE );
}

/**********************************************************************/

void CAdhocDeliveryEditDlg::BrowseStockInternal()
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
	m_bFreeStock = FALSE;

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

void CAdhocDeliveryEditDlg::OnButtonFreeStock() 
{
	UpdateData( TRUE, FALSE );
	GetEditCost() -> EnableWindow ( !m_bFreeStock );
}

/**********************************************************************/

void CAdhocDeliveryEditDlg::OnRadioSuppRef() 
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

void CAdhocDeliveryEditDlg::OnRadioPluNumber() 
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

void CAdhocDeliveryEditDlg::OnRadioStockCode() 
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

void CAdhocDeliveryEditDlg::OnOK() 
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
			EndDialog (IDOK );
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

int CAdhocDeliveryEditDlg::HandleAdd() 
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
	switch ( DataManager.Stock.CheckStockLevelsForImport( nStockIdx, StockRecord, StockLevels, dbStockpoint.GetSpNo( m_nSpIdx ), m_nStkTakeNo ) )
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

		if ( ( dDUQty != 0.0 ) && ( m_bFreeStock == FALSE ) )
		{
			double dExpectedCost = DelUnitInfo.m_dDUCost * dDUQty;

			if ( CPriceHelpers::CompareDoubles( m_dCost, dExpectedCost, SysInfo.GetDPValue() ) != 0 )
			{
				bool bUpdateCost = FALSE;
					
				switch ( StockOptions.GetStockAdhocPriceChangeType() )
				{
				case 1:	
					bUpdateCost = TRUE;	
					break;

				case 2:
					{
						CString strMsg = "You have changed the cost price for this item from\n";
						strMsg += "the value that is stored in the stock database.\n\n";
						strMsg += "Do you wish to save the new cost price";

						m_pParentWnd -> EnableWindow( FALSE );
						int nResult = Prompter.YesNo ( strMsg, TRUE ); 
						m_pParentWnd -> EnableWindow( TRUE );
						
						if ( nResult == IDYES )
							bUpdateCost = TRUE;
					}
					break;

				case 0:
				default:
					break;
				}

				if ( bUpdateCost == TRUE )
				{
					StockRecord.SetCost ( DelUnitInfo.m_nStockSuppIdx, ( m_dCost / dDUQty ) );
					DataManager.Stock.SetAt ( nStockIdx, StockRecord );

					int nSel = DelUnitInfo.m_nComboPos;
					m_DelUnitSelector.FillComboBox( &m_comboDeliveryUnit, nStockIdx, m_nSuppNo );
					m_comboDeliveryUnit.SetCurSel( nSel );

					FillRetailCombo( nStockIdx );		
				}
			}
		}
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
		
	if ( m_bFreeStock == FALSE )
	{
		dTotalCost = m_dCost;
		if ( dDUQty != 0.0 ) dDUCost = ( m_dCost / dDUQty );
	}

	CString strPluNo;
	::FormatPluNo( m_StockItemKey.m_nPluNo, strPluNo );

	CCSVStockAudit csvOut;
	csvOut.Add ( strPluNo );
	csvOut.Add ( strSuppRef );			
	csvOut.Add ( DataManager.Stock.GetStockCode( nStockIdx ) );
	csvOut.Add ( DataManager.Stock.GetDescription( nStockIdx ) );
	csvOut.Add ( m_bFreeStock );
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

void CAdhocDeliveryEditDlg::OnCancel() 
{
	if ( m_bAdd == TRUE )
		DestroyWindow();
	else
		EndDialog( IDCANCEL );
}

/**********************************************************************/

void CAdhocDeliveryEditDlg::PostNcDestroy() 
{
	if ( m_bAdd == TRUE )
	{
		*m_pParentToThis = NULL;
		delete this;
	}
}

/**********************************************************************/

void CAdhocDeliveryEditDlg::OnButtonEdit() 
{
	m_pParentWnd -> EnableWindow( FALSE );
	m_buttonEdit.SetFocus();
	HandleEditInternal();
	m_pParentWnd -> EnableWindow( TRUE );
}

/**********************************************************************/

void CAdhocDeliveryEditDlg::HandleEditInternal()
{
	int nSel = m_comboRetail.GetCurSel();
	if ( nSel < 0 || nSel >= m_comboRetail.GetCount() )
		return;
	
	int nPluIdx = m_comboRetail.GetItemData( nSel );
	if ( nPluIdx < 0 || nPluIdx >= DataManager.Plu.GetSize() )
		return;

	CString strStockCode;
	DataManager.Plu.GetPluStockCode( DataManager.Plu.GetPluNoInt(nPluIdx), strStockCode, FALSE );
	if ( strStockCode == "" )
		return;

	int nStockIdx;
	if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) == FALSE )
		return;

	CPluFilterArray TempFilter ( FILTERTYPE_DATABASE );

	TempFilter.SetArrayIdx ( nPluIdx );

	CPropertySheetPluStockInfo info;
	info.m_bEnableSpin = FALSE;
	info.m_wSpinID = IDC_SPIN;
	info.m_pPluFilter = &TempFilter;
	info.m_nStockIdx = nStockIdx;
	info.m_bNewStock = FALSE;
	info.m_bStockLevelTab = TRUE;
	info.m_nDefaultSuppNo = m_nSuppNo;
	info.m_bReadOnly = FALSE;
	info.m_nSpIdxToEdit = m_nSpIdx;
	info.m_bSingleSpIdx = TRUE;

	CPropertySheetPluStock propSheet ( info, this );

	propSheet.EnableStackedTabs ( FALSE );

	int nLastSupplierTab = -1;
	if ( propSheet.DoModal() == IDOK )
	{
		propSheet.SavePluRecord();
		m_SuppRefFinder.Invalidate();
		nLastSupplierTab = propSheet.GetLastSupplierTab();
	}
	else
		propSheet.UndoChanges();

	UpdateRetailPlu();

	CDelUnitInfo DelUnitInfo;
	m_DelUnitSelector.GetDelUnitInfo( DelUnitInfo );

	m_DelUnitSelector.FillComboBox ( &m_comboDeliveryUnit, nStockIdx, m_nSuppNo, nLastSupplierTab );
	
	if ( nLastSupplierTab == -1 )
		m_DelUnitSelector.SelectDelUnitByInfo( DelUnitInfo );
	
	OnSelectDelUnit();
}

/**********************************************************************/

void CAdhocDeliveryEditDlg::OnButtonEditDeliveryUnit() 
{
	m_buttonNew.SetFocus();

	UpdateItemKey( TRUE, TRUE, TRUE, TRUE );

	int nStockIdx;
	if ( DataManager.Stock.FindStockItemByCode( m_StockItemKey.m_strStockCode, nStockIdx ) == FALSE )
	{
		m_pParentWnd -> EnableWindow( FALSE );
		Prompter.Error ( "The selected stock code does not exist in your database" );
		m_pParentWnd -> EnableWindow( TRUE );
		return;
	}
		
	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );

	CDelUnitInfo DelUnitInfo;
	m_DelUnitSelector.GetDelUnitInfo( DelUnitInfo );
	
	int nStockSuppIdx = DelUnitInfo.m_nStockSuppIdx;

	if ( m_nSuppNo >= 1 )
	{
		if ( DelUnitInfo.m_nColour == DELUNIT_COLOUR_BLUE )
		{
			bool bGotSuppNoMatch = FALSE;
		
			for ( int n = 0; n < StockRecord.GetSupplierCount(); n++ )
			{
				if ( StockRecord.GetSuppNo(n) == m_nSuppNo )
				{
					bGotSuppNoMatch = TRUE;
					break;
				}
			}
	
			if ( bGotSuppNoMatch == FALSE ) 
			{
				CAdhocDeliveryUnitDlg dlgUnit( StockRecord, nStockSuppIdx, m_nSuppNo );
				if ( dlgUnit.DoModal() != IDOK )
					return;

				DataManager.Stock.SetAt( nStockIdx, StockRecord );

				nStockSuppIdx = dlgUnit.GetStockSuppIdx();
				m_DelUnitSelector.FillComboBox ( &m_comboDeliveryUnit, nStockIdx, m_nSuppNo, nStockSuppIdx );
				m_DelUnitSelector.GetDelUnitInfo( DelUnitInfo );
			}
		}
		else if ( DelUnitInfo.m_nColour == DELUNIT_COLOUR_WHITE )
		{
			//STOCK RECORD NOT LINKED TO ANY SUPPLIERS
			if ( StockRecord.GetSupplierCount() == 0 )
			{
				CString strMsg;
				strMsg.Format ( "Stock item %s (%s) is not linked to any delivery units\n\nWould you like to create a delivery unit for this stock item now",
					StockRecord.GetStockCode(),
					StockRecord.GetDescription() );

				if ( Prompter.YesNo ( strMsg, TRUE ) == IDYES )
				{
					StockRecord.SetSuppNo( 0, m_nSuppNo );
					StockRecord.SetSuppRef( 0, "" );
					StockRecord.SetWrapper( 0, "" );
					StockRecord.SetCase( 0, "" );
					StockRecord.SetDUItems( 0, 1 );
					StockRecord.SetDUItemSize( 0, 1.0 );
					StockRecord.SetCost( 0, 0.0 );
					StockRecord.TidySuppliers( StockOptions.GetStockMaxSuppliersPerItem() );
					DataManager.Stock.SetAt( nStockIdx, StockRecord );

					m_DelUnitSelector.FillComboBox ( &m_comboDeliveryUnit, nStockIdx, m_nSuppNo, nStockSuppIdx );
					m_DelUnitSelector.GetDelUnitInfo( DelUnitInfo );
				}
			}
			else
				nStockSuppIdx = -1;
		}
	}

	CStockFilterArray TempFilter ( FILTERTYPE_DATABASE );
	TempFilter.SetArrayIdx ( nStockIdx );

	int nActivePage = 0;
	if ( nStockSuppIdx >= 0 && nStockSuppIdx < StockRecord.GetSupplierCount() )
		nActivePage = nStockSuppIdx + 1;

	CPropertySheetStockInfo info;
	info.m_bEnableSpin = FALSE;
	info.m_wSpinID = IDC_SPIN;
	info.m_pStockFilter = &TempFilter;
	info.m_bPluLinkTab = FALSE;
	info.m_bSupplierEdit = TRUE;
	info.m_bStockLevelTab = TRUE;
	info.m_nDefaultSuppNo = m_nSuppNo;
	info.m_bReadOnly = FALSE;
	info.m_nSpIdxToEdit = m_nSpIdx;
	info.m_bSingleSpIdx = TRUE;

	CPropertySheetStock propSheet ( info, this );
	propSheet.EnableStackedTabs ( FALSE );
	propSheet.SetActivePage( nActivePage );

	int nLastSupplierTab = -1;
	if ( propSheet.DoModal() == IDOK )
	{
		m_SuppRefFinder.Invalidate();
		nLastSupplierTab = propSheet.GetLastSupplierTab();
	}
	else
		propSheet.UndoChanges();

	UpdateRetailPlu();

	m_DelUnitSelector.FillComboBox ( &m_comboDeliveryUnit, nStockIdx, m_nSuppNo, nLastSupplierTab );
	
	if ( nLastSupplierTab == -1 )
		m_DelUnitSelector.SelectDelUnitByInfo( DelUnitInfo );
	
	OnSelectDelUnit();	
}

/**********************************************************************/

