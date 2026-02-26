/**********************************************************************/
#include "BarcodeCSVArray.h"
#include "BarcodePluNoTable.h"
#include "BarcodeNumberDlg.h"
#include "..\CommonEcrStock\ListDataUnitDlg.h"
#include "MaxLengths.h"
#include "PriceHelpers.h"
#include "PriceSetItemManager.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPagePluUnitPrice.h"
/**********************************************************************/

CPropPagePluUnitPrice::CPropPagePluUnitPrice() : CSSPropertyPage(CPropPagePluUnitPrice::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePluUnitPrice)
	m_dPrice = 0.0;
	m_dPluSize = 0.0;
	//}}AFX_DATA_INIT
	m_pPluRecord = NULL;
	m_strPriceType = "";
	m_bReadOnly = FALSE;
	
	CDataManagerInfo info;
	DataManagerNonDb.OpenPluUnit( DB_READONLY, info );
}

/**********************************************************************/

CPropPagePluUnitPrice::~CPropPagePluUnitPrice()
{
	CDataManagerInfo info;
	DataManagerNonDb.ClosePluUnit( info );
}

/**********************************************************************/

void CPropPagePluUnitPrice::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePluUnitPrice)
	DDX_Control(pDX, IDC_STATIC_PRICE1, m_staticPriceType);
	DDX_Control(pDX, IDC_EDIT_LABEL, m_editLabel);
	DDX_Control(pDX, IDC_COMBO_DESCRIPTION, m_comboPluUnit);
	DDX_Text(pDX, IDC_EDIT_PLUSIZE, m_dPluSize);
	//}}AFX_DATA_MAP
	DDX_CBString(pDX, IDC_COMBO_DESCRIPTION, m_strPluUnit);
	if ( m_bReadOnly == FALSE ) DDV_MaxChars(pDX, m_strPluUnit, MAX_LENGTH_UNIT_NAME);
	DDX_Text(pDX, IDC_EDIT_TOTALPRICE, m_dPrice );
	DDX_Text(pDX, IDC_EDIT_PLUSIZE, m_dPluSize); 
	DDX_Control(pDX, IDC_BUTTON_SETUNIT, m_buttonSetUnit);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_buttonBarcodeAdd);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_buttonBarcodeDelete);
	DDX_Control(pDX, IDC_BUTTON_SETTINGS, m_buttonBarcodeSettings);
	DDX_Control(pDX, IDC_CHECK_SHELF, m_checkShelf);
	DDX_Control(pDX, IDC_CHECK_PRODUCT, m_checkProduct);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePluUnitPrice, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePluUnitPrice)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_SETUNIT, OnButtonSetUnit)
	ON_CBN_SELCHANGE(IDC_COMBO_DESCRIPTION, OnChangeLabel)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUSIZE, OnChangePluSize)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_SETTINGS, OnButtonSettings)
	ON_CBN_KILLFOCUS(IDC_COMBO_DESCRIPTION, OnChangeLabel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPropPagePluUnitPrice::GetEditPluSize() { return GetEdit( IDC_EDIT_PLUSIZE ); }
/**********************************************************************/

BOOL CPropPagePluUnitPrice::OnInitDialog() 
{
	//MUST DO THIS BEFORE CALLING GET RECORD DATA 
	//FOR THE FIRST TIME IN ANY SESSION
	DataManager.Barcode.CreatePluBarcodeList();
	
	GetRecordData();
	CPropertyPage::OnInitDialog();
	
	m_listBarcodes.SubclassDlgItem ( IDC_LIST, this );
	m_listBarcodes.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listBarcodes.InsertColumn ( 0, "No", LVCFMT_LEFT, 50 );
	m_listBarcodes.InsertColumn ( 1, "Barcode", LVCFMT_LEFT, 150 );

	if ( EcrmanOptions.AllowBarcodeModifierLinks() == TRUE )
	{
		m_listBarcodes.InsertColumn ( 2, "Modifier", LVCFMT_LEFT, 220 );
		m_listBarcodes.InsertColumn ( 3, "Printable", LVCFMT_LEFT, 160 );
		ResizeControl( &m_listBarcodes, 420, 210 );
		MoveControl( &m_buttonBarcodeAdd, 450, 147 );
		MoveControl( &m_buttonBarcodeDelete, 450, 184 );
		MoveControl( &m_buttonBarcodeSettings, 450, 221 );
		MoveControl( &m_checkShelf, 450, 280 );
		MoveControl( &m_checkProduct, 450, 298 );
		m_buttonBarcodeSettings.SetWindowText( "Change\nSettings" );
	}
	else
	{
		m_listBarcodes.InsertColumn ( 2, "Printable", LVCFMT_LEFT, 160 );
	}

	SubclassEdit ( IDC_EDIT_PLUSIZE, SS_NUM_DP, 8, "%.3f" );
	SubclassEdit ( IDC_EDIT_TOTALPRICE, SS_NUM_DP, 8, "%.*f", SysInfo.GetDPValue() );

	FillPluUnitCombo();
	SetRecordControls();
	m_staticPriceType.SetWindowText ( m_strPriceType );	
	return TRUE;  
}

/**********************************************************************/

void CPropPagePluUnitPrice::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
   {
	   m_listBarcodes.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

void CPropPagePluUnitPrice::OnButtonSetUnit() 
{
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenPluUnitBuffer( info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	UpdateData( TRUE, FALSE );

	CString strOldPluUnit = m_strPluUnit;

	CListDataUnitDlg dlg ( DataManagerNonDb.PluUnit, strOldPluUnit, this );	
	
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManagerNonDb.WritePluUnitBuffer( info ) == FALSE )
			Prompter.WriteError( info );
		else
		{
			if ( dlg.GetSelectedText() != "" )
				m_strPluUnit = dlg.GetSelectedText();
		}
	}
	
	DataManagerNonDb.ClosePluUnitBuffer( info );

	FillPluUnitCombo();
		
	if ( m_strPluUnit != strOldPluUnit )
		SetRecordControls();
	
	UpdateData ( FALSE );
}

/**********************************************************************/

BOOL CPropPagePluUnitPrice::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePluUnitPrice::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPagePluUnitPrice::GetRecordData()
{
	int nPriceLevel = EcrmanOptions.GetLabelsShelfPriceLevel1();
	
	if ( nPriceLevel >= 1 && nPriceLevel <= MAX_PLU_PRICES )
	{
		m_dPrice = m_pPluRecord -> GetPrice ( nPriceLevel - 1 );
		m_strPriceType.Format ( "Price %1.1d", nPriceLevel );
	}
	else
		m_dPrice = 0.0;

	m_dPluSize = m_pPluRecord -> GetUnitPriceSize();
	m_strPluUnit = m_pPluRecord -> GetUnitPriceText();

	DataManager.Barcode.GetBarcodeIdxArray( m_pPluRecord -> GetPluNoInt(), m_BarcodeIdxArray );
}

/**********************************************************************/

void CPropPagePluUnitPrice::SaveRecord()
{
	if ( m_bReadOnly == TRUE )
		return;

	m_pPluRecord -> SetUnitPriceSize ( m_dPluSize );
	::TrimSpaces ( m_strPluUnit, FALSE );
	m_pPluRecord -> SetUnitPriceText ( m_strPluUnit );
	m_pPluRecord -> SetPrintShelfFlag( m_checkShelf.GetCheck() != 0 );
	m_pPluRecord -> SetPrintProductFlag( m_checkProduct.GetCheck() != 0 );
}

/**********************************************************************/

bool CPropPagePluUnitPrice::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPagePluUnitPrice::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/

void CPropPagePluUnitPrice::SetRecordControls()
{
	CString strLabel = "";
	double dRatio = CPriceHelpers::GetUnitPriceRatio ( m_strPluUnit, m_dPluSize );
	double dUnitPrice = ( dRatio == 0.0 ) ? 0.0 : m_dPrice / dRatio;

	if ( dUnitPrice >= 1.00 )
	{
		if ( dRatio != 0.0 )
			strLabel.Format ( "£%.2f %s", dUnitPrice, ( const char* ) m_strPluUnit );
		else
			strLabel = m_strPluUnit;
	}
	else
	{
		if ( dRatio != 0.0 )
			strLabel.Format ( "%.1fp %s", dUnitPrice * 100.0, ( const char* ) m_strPluUnit );
		else
			strLabel = m_strPluUnit;
	}

	m_editLabel.SetWindowText ( strLabel );

	m_checkShelf.SetCheck( m_pPluRecord -> GetPrintShelfFlag() );
	m_checkProduct.SetCheck( m_pPluRecord -> GetPrintProductFlag() );

	UpdateData ( FALSE );

	if ( PriceSetItemManager.IsPluMaster() == FALSE )
	{
		if ( PriceSetItemManager.GetPluStatus() != LOCALPLU_LOCAL )
		{
			m_comboPluUnit.EnableWindow ( FALSE );
			GetEditPluSize() -> EnableWindow ( FALSE );
			m_checkShelf.EnableWindow( FALSE );
			m_checkProduct.EnableWindow( FALSE );
		}
		else
		{
			m_comboPluUnit.EnableWindow ( PriceSetItemManager.CheckField ( Plu::UnitPriceText.Label ) );
			GetEditPluSize() -> EnableWindow ( PriceSetItemManager.CheckField ( Plu::UnitPriceSize.Label ) );
			m_checkShelf.EnableWindow ( PriceSetItemManager.CheckField ( Plu::PrintShelf.Label ) );
			m_checkProduct.EnableWindow ( PriceSetItemManager.CheckField ( Plu::PrintProduct.Label ) );
		}

		m_buttonSetUnit.ShowWindow( SW_HIDE );
		m_buttonBarcodeAdd.ShowWindow( SW_HIDE );
		m_buttonBarcodeDelete.ShowWindow( SW_HIDE );
		m_buttonBarcodeSettings.ShowWindow( SW_HIDE );
	}

	AddList( -1 );

	if ( m_bReadOnly == TRUE )
	{
		m_comboPluUnit.EnableWindow( FALSE );
		GetEditPluSize() -> EnableWindow ( FALSE );
		m_buttonSetUnit.ShowWindow( SW_HIDE );
		m_buttonBarcodeAdd.ShowWindow( SW_HIDE );
		m_buttonBarcodeDelete.ShowWindow( SW_HIDE );
		m_buttonBarcodeSettings.ShowWindow( SW_HIDE );
		m_checkShelf.EnableWindow( FALSE );
		m_checkProduct.EnableWindow( FALSE );
	}

	if ( BarcodePluNoTable.IsValidEan( m_pPluRecord -> GetPluNoInt() ) == FALSE )
		m_checkProduct.ShowWindow( SW_HIDE );

	if ( SysInfo.IsModifiableSSPluNo( m_pPluRecord -> GetPluNoInt() ) == TRUE )
	{
		m_checkProduct.ShowWindow( SW_HIDE );
		m_checkShelf.ShowWindow( SW_HIDE );
	}
}

/**********************************************************************/

void CPropPagePluUnitPrice::OnChangeLabel() 
{
	CString strPluUnitOld = m_strPluUnit;

	UpdateData( TRUE, FALSE );

	int nSelection = m_comboPluUnit.GetCurSel();
	if ( nSelection >= 0 && nSelection < m_comboPluUnit.GetCount() )
		m_comboPluUnit.GetLBText( nSelection, m_strPluUnit );
		
	if ( m_strPluUnit != strPluUnitOld )
		SetRecordControls();
}

/**********************************************************************/

void CPropPagePluUnitPrice::FillPluUnitCombo()
{
	m_comboPluUnit.ResetContent();	
	for ( int nIndex = 0; nIndex < DataManagerNonDb.PluUnit.GetSize(); nIndex++ )
		m_comboPluUnit.AddString ( DataManagerNonDb.PluUnit.GetText ( nIndex ) );
}

/**********************************************************************/

void CPropPagePluUnitPrice::OnChangePluSize() 
{
	UpdateData( TRUE, FALSE );
	SetRecordControls();
}

/**********************************************************************/

void CPropPagePluUnitPrice::AddList( int nTargetBarcodeIdx )
{
	m_listBarcodes.DeleteAllItems();
	for ( int nIndex = 0; nIndex < m_BarcodeIdxArray.GetSize(); nIndex++ )
	{
		CBarcodeCSVRecord BarcodeRecord;
		int nBarcodeIdx = m_BarcodeIdxArray.GetAt( nIndex );
		DataManager.Barcode.GetAt ( nBarcodeIdx, BarcodeRecord );

		CString strNum;
		strNum.Format ( "%3.3d", nIndex + 1 );

		CString strMod = "";
		{
			int nMod = BarcodeRecord.GetModifier();
			if ( nMod != 0 )
				strMod = DataManager.Modifier.GetDisplayName( nMod );
		}
		
		CCSV csvOut ( '\t' );
		csvOut.Add ( strNum );
		csvOut.Add ( BarcodeRecord.GetBarcodeNoString() );

		if ( EcrmanOptions.AllowBarcodeModifierLinks() == TRUE )
			csvOut.Add ( strMod );

		csvOut.Add ( BarcodeRecord.GetPrintFlagText() );
		
		m_listBarcodes.AddString ( csvOut.GetLine() );

		if ( nBarcodeIdx == nTargetBarcodeIdx )
			m_listBarcodes.SetCurSel( nIndex );
	}
}

/**********************************************************************/

void CPropPagePluUnitPrice::OnButtonAdd() 
{
	CBarcodeCSVRecord BarcodeRecord;
	BarcodeRecord.SetPluNo ( m_pPluRecord -> GetPluNoInt() );
	BarcodeRecord.SetBarcodeNo ( 0 );
	
	CBarcodeNumberDlg dlg ( BARCODE_PLUDB_ADD, BarcodeRecord, this );
	if ( dlg.DoModal() != IDOK )
		return;

	__int64 nBarcodeNo = BarcodeRecord.GetBarcodeNoInt();

	int nBarcodeIdx;
	if ( DataManager.Barcode.FindBarcodeByNumber( nBarcodeNo, nBarcodeIdx ) == FALSE )
	{
		m_pPluChangesBuffer -> LogBarcodeChange( BarcodeRecord.GetBarcodeNoString() );
		DataManager.Barcode.InsertRecord( nBarcodeIdx, BarcodeRecord );
		DataManager.Barcode.GetBarcodeIdxArray( m_pPluRecord -> GetPluNoInt(), m_BarcodeIdxArray );
		AddList( nBarcodeIdx );
	}
	else
	{
		CBarcodeCSVRecord BarcodeRecord;
		DataManager.Barcode.GetAt( nBarcodeIdx, BarcodeRecord );
		CString strPluNo1 = BarcodeRecord.GetPluNoString();
		CString strPluNo2 = m_pPluRecord -> GetPluNoString();

		if ( strPluNo1 == strPluNo2 )
		{
			CString strMsg;
			strMsg.Format ( "Barcode %s is already linked to this Plu",
				BarcodeRecord.GetBarcodeNoString() );

			Prompter.Info ( strMsg );
			return;
		}
		else
		{
			int nPluIdx;
			__int64 nPluNo = BarcodeRecord.GetPluNoInt();
			if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) )
			{
				CString strMsg;
				strMsg.Format ( "Barcode %s is already linked to Plu %s\n\nWould you like to link it to this Plu instead",
					BarcodeRecord.GetBarcodeNoString(),
					BarcodeRecord.GetPluNoString() );

				if ( Prompter.YesNo ( strMsg ) != IDYES )
					return;
			}
			
			//REMOVE AND REINSERT THE RECORD TO UPDATE THE
			//INTERNAL PLU TO BARCODE TABLE 
			m_pPluChangesBuffer -> LogBarcodeChange( BarcodeRecord.GetBarcodeNoString() );
			DataManager.Barcode.RemoveRecord( nBarcodeIdx );
			BarcodeRecord.SetPluNo( _atoi64( strPluNo2 ) );
			BarcodeRecord.SetPrintShelfFlag( TRUE );
			BarcodeRecord.SetPrintProductFlag( TRUE );
			DataManager.Barcode.InsertRecord( nBarcodeIdx, BarcodeRecord );
			DataManager.Barcode.GetBarcodeIdxArray( m_pPluRecord -> GetPluNoInt(), m_BarcodeIdxArray );
			AddList( nBarcodeIdx );
		}
	}
}

/**********************************************************************/

void CPropPagePluUnitPrice::OnButtonRemove() 
{
	int nSel = m_listBarcodes.GetCurSel();
	int nListSize = m_BarcodeIdxArray.GetSize();
	if ( nSel >= 0 && nSel < nListSize )
	{
		int nBarcodeIdx = m_BarcodeIdxArray.GetAt( nSel );
		CBarcodeCSVRecord BarcodeRecord;
		DataManager.Barcode.GetAt( nBarcodeIdx, BarcodeRecord );

		CString strMsg;
		strMsg.Format ( "Are you sure you wish to delete barcode no %s", BarcodeRecord.GetBarcodeNoString() );
	
		if ( Prompter.YesNo( strMsg ) != IDYES )
			return;

		m_pPluChangesBuffer -> LogBarcodeChange ( BarcodeRecord.GetBarcodeNoString() );
		DataManager.Barcode.RemoveRecord( nBarcodeIdx );
		DataManager.Barcode.GetBarcodeIdxArray( m_pPluRecord -> GetPluNoInt(), m_BarcodeIdxArray );
		
		AddList( -1 );

		if ( nSel < nListSize )
			m_listBarcodes.SetCurSel( nSel );
		else if ( nListSize > 0 )
			m_listBarcodes.SetCurSel ( nListSize - 1 );
	}
}

/**********************************************************************/

void CPropPagePluUnitPrice::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonSettings();	
	*pResult = 0;
}

/**********************************************************************/

void CPropPagePluUnitPrice::OnButtonSettings() 
{
	int nSel = m_listBarcodes.GetCurSel();
	int nListSize = m_BarcodeIdxArray.GetSize();
	if ( nSel >= 0 && nSel < nListSize )
	{
		int nBarcodeIdx = m_BarcodeIdxArray.GetAt( nSel );
		CBarcodeCSVRecord BarcodeRecord;
		DataManager.Barcode.GetAt( nBarcodeIdx, BarcodeRecord );
		m_pPluChangesBuffer -> LogBarcodeChange ( BarcodeRecord.GetBarcodeNoString() );
			
		if ( EcrmanOptions.AllowBarcodeModifierLinks() == TRUE )
		{
			CBarcodeNumberDlg dlg ( BARCODE_PLUDB_EDIT, BarcodeRecord, this );
			if ( dlg.DoModal() == IDOK )
			{
				DataManager.Barcode.SetAt( nBarcodeIdx, BarcodeRecord );
				AddList( nBarcodeIdx );
			}
		}
		else
		{
			bool bShelf = BarcodeRecord.GetPrintShelfFlag();
			bool bProduct = BarcodeRecord.GetPrintProductFlag();

			if ( FALSE == bShelf )
				BarcodeRecord.SetPrintShelfFlag( TRUE );
			else if ( FALSE == bProduct )
			{
				BarcodeRecord.SetPrintShelfFlag( FALSE );
				BarcodeRecord.SetPrintProductFlag( TRUE );
			}
			else
			{
				BarcodeRecord.SetPrintShelfFlag( FALSE );
				BarcodeRecord.SetPrintProductFlag( FALSE );
			}

			DataManager.Barcode.SetAt( nBarcodeIdx, BarcodeRecord );
			AddList( nBarcodeIdx );
		}
	}
}

/**********************************************************************/
