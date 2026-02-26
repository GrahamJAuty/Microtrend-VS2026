/**********************************************************************/
#include "ExplodePlu.h"
#include "FilterTypes.h"
#include "..\CommonEcrStock\ListDataUnitDlg.h"
#include "PluFilterDlgStockman.h"
#include "BarcodePluNoTable.h"
#include "RepCSV.h"
#include "ReportHelpers.h"
#include "StockAuditBaseReport.h"
#include "StockFilterDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "AdjustmentsEditPluDlg.h"
/**********************************************************************/

CAdjustmentsEditPluDlg::CAdjustmentsEditPluDlg( int nSpIdx, const char* szSpOrLoc, CString& strLastAdjustment, CAdjustmentsEditPluDlg** pParentToThis, int nStkTakeNo, CWnd* pParent)
	: CSSDialog("IDD_ADJUSTMENT_EDIT_PLU", pParent),
	m_FilterArray ( FILTERTYPE_BROWSE_PLU ), 
	m_StockFilterArray ( FILTERTYPE_BROWSE_STOCK ),
	m_strLastAdjustment( strLastAdjustment )
{
	m_nSpIdx = nSpIdx;
	m_nStkTakeNo = nStkTakeNo;

	DataInit ( szSpOrLoc );

	m_bAdd = TRUE;
	m_nEntryMode = ENTRY_MODE_PLUNO;
	
	m_pParentToThis = pParentToThis;
	*m_pParentToThis = this;

	m_dQuantity = 0.0;
}

/**********************************************************************/

CAdjustmentsEditPluDlg::CAdjustmentsEditPluDlg( int nSpIdx, const char* szSpOrLoc, CString& strDummy, int nStkTakeNo, CWnd* pParent)
	: CSSDialog("IDD_ADJUSTMENT_EDIT_PLU", pParent),
	m_FilterArray ( FILTERTYPE_BROWSE_PLU ), 
	m_StockFilterArray ( FILTERTYPE_BROWSE_STOCK ),
	m_strLastAdjustment ( strDummy )
{
	m_nSpIdx = nSpIdx;
	m_nStkTakeNo = nStkTakeNo;

	m_strLastAdjustment = strDummy;
	DataInit ( szSpOrLoc );
	m_bAdd = FALSE;
}

/**********************************************************************/

void CAdjustmentsEditPluDlg::DataInit( const char* szSpOrLoc )
{
	//{{AFX_DATA_INIT(CAdjustmentsEditPluDlg)
	//}}AFX_DATA_INIT
	m_bModified = FALSE;
	m_dQuantity = 1.0;
	m_dValue = 0.0;
	m_strPluNo = "";
	m_nPluIdx = -1;

	m_strSpOrLoc = szSpOrLoc;

	m_strInputLine = "";
	m_strOutputLine = "";

	m_nModifier = 0;
	m_dMultiplier = 1.0;

	if ( m_nStkTakeNo >= 1 )
	{
		m_StockFilterArray.SetSpIdx( m_nSpIdx, -1 );
		m_StockFilterArray.SetStocktakeFilter( m_nStkTakeNo );
	}
}

/**********************************************************************/

void CAdjustmentsEditPluDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdjustmentsEditPluDlg)
	DDX_Control(pDX, IDC2_COMBO_REASON, m_comboReason);
	DDX_Control(pDX, IDC2_BUTTON_BROWSE_PLU, m_buttonBrowsePlu);
	DDX_Control(pDX, IDC2_BUTTON_BROWSE, m_buttonBrowseStock);
	DDX_Control(pDX, IDC2_RADIO_STOCKCODE, m_radioStockCode);
	DDX_Control(pDX, IDC2_RADIO_PLUNUMBER, m_radioPluNumber);
	DDX_Control(pDX, IDC2_STATIC_VALUE, m_staticValue);
	DDX_Control(pDX, IDC2_EDIT_STOCKCODE, m_editStockCode);
	DDX_Control(pDX, IDC2_COMBO_LEVEL, m_comboLevel);
	DDX_Text(pDX, IDC2_EDIT_DESCRIPTION, m_strDescription );
	DDX_Control(pDX, IDC2_COMBO_MODIFIER, m_comboModifier);
	DDX_Control(pDX, IDC2_EDIT_MULTIPLIER, m_editMultiplier);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC2_STATIC_MODIFIER1, m_staticModifier1);
	DDX_Control(pDX, IDC2_STATIC_MODIFIER2, m_staticModifier2);
	DDX_Text ( pDX, IDC2_EDIT_QTY, m_dQuantity );	
	DDX_Text(pDX, IDC2_EDIT_VALUE, m_dValue );
	DDX_Text(pDX, IDC2_EDIT_STOCKCODE, m_strStockCode);
	DDV_MaxChars(pDX, m_strStockCode, Stock::StockCode.Max); 
	DDX_IntegerString(pDX, IDC2_EDIT_PLUNO, m_strPluNo);
	DDX_Text(pDX, IDC2_COMBO_REASON, m_strLastAdjustment);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDOK, m_buttonOK);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CAdjustmentsEditPluDlg, CDialog)
	//{{AFX_MSG_MAP(CAdjustmentsEditPluDlg)
	ON_EN_KILLFOCUS(IDC2_EDIT_STOCKCODE, OnKillFocusStockCode)
	ON_BN_CLICKED(IDC2_BUTTON_BROWSE, OnButtonBrowseStock)
	ON_BN_CLICKED(IDC2_BUTTON_SET, OnButtonSet)
	ON_EN_KILLFOCUS(IDC2_EDIT_PLUNO, OnKillFocusPluNo)
	ON_EN_KILLFOCUS(IDC2_EDIT_QTY, OnKillFocusQty)
	ON_BN_CLICKED(IDC2_BUTTON_BROWSE_PLU, OnButtonBrowsePlu)
	ON_BN_CLICKED(IDC2_RADIO_PLUNUMBER, OnRadioPluNumber)
	ON_BN_CLICKED(IDC2_RADIO_STOCKCODE, OnRadioStockCode)
	ON_CBN_SELCHANGE(IDC2_COMBO_LEVEL, OnSelectLevel)
	ON_CBN_SELCHANGE(IDC2_COMBO_MODIFIER, OnSelectModifier)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CAdjustmentsEditPluDlg::GetEditQty() { return GetEdit( IDC2_EDIT_QTY ); }
CEdit* CAdjustmentsEditPluDlg::GetEditValue() { return GetEdit( IDC2_EDIT_VALUE ); }
CEdit* CAdjustmentsEditPluDlg::GetEditPluNo() { return GetEdit( IDC2_EDIT_PLUNO ); }
/**********************************************************************/

BOOL CAdjustmentsEditPluDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SubclassEdit ( IDC2_EDIT_QTY, SS_NUM_SDP, 8, "%.5f" );
	SubclassEdit ( IDC2_EDIT_VALUE, SS_NUM_DP, 8, "%.*f", SysInfo.GetDPValue() );
	SubclassEdit ( IDC2_EDIT_PLUNO, SS_NUM, Super.MaxPluLen(), "%s" );

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(40);
	m_comboLevel.SetTabStops( arrayStops );

	for ( int nLevel = 0; nLevel < 10; nLevel++ )
	{
		m_comboLevel.AddItem( DataManager.PriceText.GetTabbedComboText(nLevel) );
	}

	m_comboLevel.SetCurSel(0);

	for ( int nIndex = 0; nIndex < DataManagerNonDb.AdjustText.GetSize(); nIndex++ )
		m_comboReason.AddString ( DataManagerNonDb.AdjustText.GetText( nIndex ) );

	CString strTitle;

	if ( m_bAdd == TRUE )
	{
		strTitle.Format ( "Add Adjustment Line (%s)", 
			(const char*) m_strSpOrLoc );

		m_comboReason.SetWindowText( m_strLastAdjustment );
		
		OnRadioPluNumber();
		UpdateModifierCombo();
	}
	else
	{
		strTitle.Format ( "Edit Adjustment Line (%s)", 
			(const char*) m_strSpOrLoc );
		
		CCSVAdjustmentPlu csvIn ( m_strInputLine );			
		m_nEntryMode = csvIn.GetItemLookupMethod();
		m_radioPluNumber.SetCheck( m_nEntryMode == ENTRY_MODE_PLUNO );
		m_radioStockCode.SetCheck( m_nEntryMode == ENTRY_MODE_STOCKCODE );

		GetEditPluNo() -> EnableWindow( m_nEntryMode == ENTRY_MODE_PLUNO );
		m_editStockCode.EnableWindow( m_nEntryMode == ENTRY_MODE_STOCKCODE );
	
		::FormatPluNo ( csvIn.GetPluNo(), m_strPluNo );
		m_strStockCode = csvIn.GetStockCode();		
		m_strDescription = csvIn.GetDescription();

		m_dQuantity = csvIn.GetQty();
		m_dValue = csvIn.GetValue();
		
		int nLevel = csvIn.GetPriceLevel();
		if ( nLevel < 0 || nLevel >= m_comboLevel.GetCount() )
			nLevel = 0;

		m_comboLevel.SetCurSel( nLevel );

		if ( DataManager.Plu.FindPluByNumber( _atoi64(m_strPluNo), m_nPluIdx ) == FALSE )
			m_nPluIdx = -1;

		UpdateModifierCombo();
		
		m_nModifier = 0;
		m_dMultiplier = 1.0;
		
		int nMod = csvIn.GetModifier();
		for ( int n = 0; n < m_comboModifier.GetCount(); n++ )
		{
			if ( m_comboModifier.GetItemData(n) == (WORD) nMod )
			{
				m_nModifier = nMod;
				m_dMultiplier = DataManager.Modifier.GetMultiplierForQty(nMod);
				m_comboModifier.SetCurSel(n);
				break;
			}
		}

		UpdateData( FALSE );
		
		m_buttonOK.SetWindowText ( "OK" );
		m_buttonCancel.SetWindowText ( "Cancel" );

		CString strMultiplier;
		strMultiplier.Format ( "%.2f", m_dMultiplier );
		m_editMultiplier.SetWindowText ( strMultiplier );

		GetEditQty() -> SetFocus();
		GetEditQty() -> SetSel( 0, -1 );

		m_comboReason.SetWindowText( csvIn.GetReason() );
	}

	if ( SysInfo.GetMaxBasePluLen() == 0 )
	{
		m_staticModifier1.ShowWindow( SW_HIDE );
		m_staticModifier2.ShowWindow( SW_HIDE );
		m_comboModifier.SetCurSel(0);
		m_comboModifier.EnableWindow( FALSE );
		m_comboModifier.ShowWindow( FALSE );
		m_editMultiplier.ShowWindow( SW_HIDE );
	}

	SetWindowText ( strTitle );
	CenterWindow();
	
	//WE HAVE SET FOCUS TO A CONTROL
	return FALSE;  
}

/**********************************************************************/

void CAdjustmentsEditPluDlg::UpdateModifierCombo()
{
	if ( SysInfo.GetMaxBasePluLen() != 0 )
	{
		m_comboModifier.ResetContent();
		m_comboModifier.AddString ( "None" );
		m_comboModifier.SetItemData ( 0, 0 );

		if ( m_nPluIdx >= 0 && m_nPluIdx < DataManager.Plu.GetSize() )
		{
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt( m_nPluIdx, PluRecord );

			for ( int nMod = 1; nMod <= PLU_MODIFIERS; nMod++ )
			{
				if ( PluRecord.GetModifierEnable( nMod ) )
				{
					int nPos = m_comboModifier.AddString ( DataManager.Modifier.GetDisplayName( nMod ) );
					m_comboModifier.SetItemData( nPos, nMod );
				}
			}
		}

		m_comboModifier.SetCurSel(0);
		m_comboModifier.EnableWindow( m_comboModifier.GetCount() >= 2 );
		
		m_dMultiplier = 1.0;
		m_nModifier = 0;
		m_editMultiplier.SetWindowText ( "1.00" );
	}
}

/**********************************************************************/

bool CAdjustmentsEditPluDlg::LookUpPluNumber( __int64& nPluNo, CString& strStockCode, int& nPluIdx )
{
	strStockCode = "";
	nPluIdx = -1;

	bool bFound = FALSE;

	if ( nPluNo != 0 )
	{
		if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == TRUE )
		{
			bFound = TRUE;
			DataManager.Plu.GetPluStockCode ( nPluNo, strStockCode, FALSE );
		
			if ( strStockCode != "" )
			{
				int nStockIdx;
				if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) == FALSE )
					strStockCode = "";
			}
		}
		else
			nPluIdx = -1;
	}

	return bFound;
}

/**********************************************************************/

bool CAdjustmentsEditPluDlg::LookUpStockCode( __int64& nPluNo, CString& strStockCode, int& nPluIdx )
{
	nPluNo = 0;
	nPluIdx = -1;

	int nStockIdx;
	if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) == FALSE )
		return FALSE;
	
	int nStockPluIdx;
	if ( DataManager.StockPlu.FindStockCode( strStockCode, nStockPluIdx ) == FALSE )
		return FALSE;
	
	CStockPluRecord StockPlu;
	DataManager.StockPlu.GetAt( nStockPluIdx, StockPlu );

	if ( StockPlu.GetPluNoCount() >= 1 )
		nPluNo = StockPlu.GetPluNo(0);
	
	if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == TRUE )
		return TRUE;
	else
	{
		nPluNo = 0;
		nPluIdx = -1;
		return FALSE;
	}
}

/**********************************************************************/

void CAdjustmentsEditPluDlg::OnKillFocusPluNo() 
{
	UpdateData ( TRUE, FALSE );
	m_strPluNo = BarcodePluNoTable.ConvertPlu( m_strPluNo );
	
	__int64 nPluNo = _atoi64( m_strPluNo );
	LookUpPluNumber( nPluNo, m_strStockCode, m_nPluIdx );
	UpdatePluNumber( m_nPluIdx );
	
	CPluCSVRecord PluRecord;
	if ( m_nPluIdx != -1 )
		DataManager.Plu.GetAt( m_nPluIdx, PluRecord );
	
	UpdateItemValue( PluRecord, 1.0 );
	UpdateModifierCombo();
	UpdateData(FALSE);
}

/***********************************************************************/

void CAdjustmentsEditPluDlg::OnKillFocusStockCode() 
{
	UpdateData ( TRUE, FALSE );
	m_strStockCode = BarcodePluNoTable.ConvertStockCode( m_strStockCode );
	
	__int64 nPluNo = 0;
	LookUpStockCode( nPluNo, m_strStockCode, m_nPluIdx );
	::FormatPluNo( nPluNo, m_strPluNo );
	UpdatePluNumber( m_nPluIdx );
	
	CPluCSVRecord PluRecord;
	if ( m_nPluIdx != -1 )
		DataManager.Plu.GetAt( m_nPluIdx, PluRecord );
	
	UpdateItemValue( PluRecord, 1.0 );
	UpdateModifierCombo();
	UpdateData(FALSE);

	//WE **MUST** DO THIS HERE BECAUSE WE ARE TABBING FROM A STANDARD (NON-CSS)
	//EDIT THAT DOES NOT IMPLEMENT PAUL'S SETSEL FIX
	GetEditQty() -> SetSel( 0, -1 );
}

/**********************************************************************/

void CAdjustmentsEditPluDlg::UpdatePluNumber( int nPluIdx )
{
	if ( nPluIdx != -1 )
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt( nPluIdx, PluRecord );
		m_strDescription = ReportHelpers.GetReportText( PluRecord );
		m_dValue = PluRecord.GetPrice(0);
		m_dQuantity = 1.0;
	}
	else
	{
		m_strDescription = "Unknown";
		m_dValue = 0.0;
		m_dQuantity = 1.0;
	}
}

/**********************************************************************/

void CAdjustmentsEditPluDlg::OnKillFocusQty() 
{
	UpdateData( TRUE, FALSE );

	if ( m_nPluIdx != -1 )
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( m_nPluIdx, PluRecord );
		UpdateItemValue ( PluRecord, m_dQuantity );
		UpdateData ( FALSE );
		GetEditValue() -> SetSel ( 0, -1 );
	}
}

/**********************************************************************/

void CAdjustmentsEditPluDlg::OnButtonBrowsePlu() 
{
	m_pParentWnd -> EnableWindow( FALSE );
	BrowsePluInternal();
	m_pParentWnd -> EnableWindow( TRUE );
}

/**********************************************************************/

void CAdjustmentsEditPluDlg::BrowsePluInternal()
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
	m_strPluNo = PluRecord.GetPluNoString();	

	DataManager.Plu.GetPluStockCode ( PluRecord.GetPluNoInt(), m_strStockCode, FALSE );
		
	if ( m_strStockCode != "" )
	{
		int nStockIdx;
		if ( DataManager.Stock.FindStockItemByCode( m_strStockCode, nStockIdx ) == FALSE )
			m_strStockCode = "";
	}

	UpdatePluNumber( nPluIdx );
	UpdateModifierCombo();
	UpdateData ( FALSE );

	switch ( m_nEntryMode )
	{
	case ENTRY_MODE_STOCKCODE:
		GetEditQty() -> SetFocus();
		GetEditQty() -> SetSel( 0, -1 );
		break;

	case ENTRY_MODE_PLUNO:
	default:
		GetEditPluNo() -> SetFocus();
		GetEditPluNo() -> SetSel ( 0, -1 );
		break;
	}
}

/**********************************************************************/

void CAdjustmentsEditPluDlg::OnButtonBrowseStock() 
{
	m_pParentWnd -> EnableWindow( FALSE );
	BrowseStockInternal();
	m_pParentWnd -> EnableWindow( TRUE );
}

/**********************************************************************/

void CAdjustmentsEditPluDlg::BrowseStockInternal()
{
	UpdateData( TRUE, FALSE );

	CStockFilterDlg dlgBrowse ( m_StockFilterArray, this );
	dlgBrowse.SetSimpleSupplierMode();
	
	if ( dlgBrowse.DoModal() != IDOK )
		return;

	int nStockIdx = dlgBrowse.GetStockIdx();
	if ( nStockIdx < 0 )
		return;

	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	m_strStockCode = StockRecord.GetStockCode();	
	
	int nPluIdx;
	__int64 nPluNo = _atoi64( m_strPluNo );
	LookUpStockCode( nPluNo, m_strStockCode, nPluIdx );
	UpdatePluNumber( nPluIdx );
	UpdateModifierCombo();

	UpdateData ( FALSE );

	switch ( m_nEntryMode )
	{
	case ENTRY_MODE_PLUNO:
		GetEditQty() -> SetFocus();
		GetEditQty() -> SetSel( 0, -1 );
		break;

	case ENTRY_MODE_STOCKCODE:
	default:
		m_editStockCode.SetFocus();
		m_editStockCode.SetSel ( 0, -1 );
		break;
	}
}

/**********************************************************************/

void CAdjustmentsEditPluDlg::OnRadioPluNumber() 
{
	m_radioPluNumber.SetCheck( TRUE );
	m_radioStockCode.SetCheck( FALSE );

	GetEditPluNo() -> EnableWindow( TRUE );
	m_editStockCode.EnableWindow( FALSE );

	m_nEntryMode = ENTRY_MODE_PLUNO;

	GetEditPluNo() -> SetFocus();
	GetEditPluNo() -> SetSel( 0, -1 );
}

/**********************************************************************/

void CAdjustmentsEditPluDlg::OnRadioStockCode() 
{
	m_radioPluNumber.SetCheck( FALSE );
	m_radioStockCode.SetCheck( TRUE );

	GetEditPluNo() -> EnableWindow( FALSE );
	m_editStockCode.EnableWindow( TRUE );

	m_nEntryMode = ENTRY_MODE_STOCKCODE;

	m_editStockCode.SetFocus();
	m_editStockCode.SetSel( 0, -1 );
}

/**********************************************************************/

void CAdjustmentsEditPluDlg::OnSelectLevel()
{
	UpdateData( TRUE, FALSE );
	
	if ( m_nPluIdx != -1 )
	{
		int nLevel = m_comboLevel.GetCurSel();

		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt( m_nPluIdx, PluRecord );
		m_strDescription = ReportHelpers.GetReportText( PluRecord );
		m_dValue = m_dQuantity * PluRecord.GetPrice( nLevel );
	}

	UpdateData( FALSE );
}

/**********************************************************************/

void CAdjustmentsEditPluDlg::OnSelectModifier() 
{
	if ( SysInfo.GetMaxBasePluLen() != 0 )
	{
		UpdateData( TRUE, FALSE );

		int nSel = m_comboModifier.GetCurSel();
		if ( nSel < 0 || nSel >= m_comboModifier.GetCount() )
			return;

		m_dMultiplier = 1.0;
		m_nModifier = m_comboModifier.GetItemData( nSel );
		if ( m_nModifier != 0 ) m_dMultiplier = DataManager.Modifier.GetMultiplierForQty( m_nModifier ); 

		CString strMultiplier;
		strMultiplier.Format ( "%.6f", m_dMultiplier );
		m_editMultiplier.SetWindowText( strMultiplier );
		
		if ( m_nPluIdx >= 0 && m_nPluIdx < DataManager.Plu.GetSize() )
		{
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt( m_nPluIdx, PluRecord );
			UpdateItemValue ( PluRecord, m_dQuantity );
			UpdateData( FALSE );
		}
	}
}

/**********************************************************************/

void CAdjustmentsEditPluDlg::UpdateItemValue ( CPluCSVRecord& PluRecord, double dQty )
{
	__int64 nPluNo = PluRecord.GetPluNoInt();
	if ( SysInfo.IsModifiableSSPluNo( nPluNo ) == TRUE )
	{
		m_dQuantity = dQty;
		m_dValue = m_dQuantity * PluRecord.GetModifierPrice( m_nModifier, m_comboLevel.GetCurSel() );
	}
	else
	{
		m_dQuantity = dQty;
		m_dValue = m_dQuantity * PluRecord.GetPrice( m_comboLevel.GetCurSel() );
	}
}

/**********************************************************************/

void CAdjustmentsEditPluDlg::OnButtonSet() 
{
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenAdjustTextBuffer( info ) == FALSE )
		Prompter.ShareError( info );
	else
	{
		CString strText;
		m_comboReason.GetWindowText( strText );

		CListDataUnitDlg dlg ( DataManagerNonDb.AdjustText, strText, this );
		if ( dlg.DoModal() == IDOK )
		{
			if ( DataManagerNonDb.WriteAdjustTextBuffer( info ) == FALSE )
				Prompter.WriteError( info );
			else
			{
				m_comboReason.ResetContent();
				for ( int nIndex = 0; nIndex < DataManagerNonDb.AdjustText.GetSize(); nIndex++ )
					m_comboReason.AddString ( DataManagerNonDb.AdjustText.GetText( nIndex ) );
		
				m_comboReason.SetWindowText( dlg.GetSelectedText() );
			}
		}

		DataManagerNonDb.CloseAdjustTextBuffer( info );
	}
}

/**********************************************************************/

void CAdjustmentsEditPluDlg::OnOK() 
{
	m_buttonOK.SetFocus();

	if ( UpdateData ( TRUE ) == FALSE )
		return;

	CEdit* pNewFocus;

	switch ( m_nEntryMode )
	{
	case ENTRY_MODE_PLUNO:		pNewFocus = GetEditPluNo();	break;
	case ENTRY_MODE_STOCKCODE:	
	default:					pNewFocus = &m_editStockCode;	break;
	}
	
	int nResult = HandleAdd();

	switch ( nResult )
	{
	case 1:
		m_pParentWnd -> EnableWindow( FALSE );
		Prompter.Error ( "The selected Plu number does not exist in your database" );
		m_pParentWnd -> EnableWindow( TRUE );
		break;

	case 2:
		m_pParentWnd -> EnableWindow( FALSE );
		Prompter.Error ( "The selected Plu number is linked to invalid stock items" );
		m_pParentWnd -> EnableWindow( TRUE );
		break;

	case 3:
		m_pParentWnd -> EnableWindow( FALSE );
		Prompter.Error ( "The selected Plu number is not linked to any stock items" );
		m_pParentWnd -> EnableWindow( TRUE );
		break;

	case 4:
		m_pParentWnd -> EnableWindow( FALSE );
		Prompter.Error ( "Please enter a non-zero quantity" );
		m_pParentWnd -> EnableWindow( TRUE );
		pNewFocus = GetEditQty();
		break;

	case 5:
		m_pParentWnd -> EnableWindow( FALSE );
		Prompter.Error ( "You must enter a reason for each adjustment" );
		m_pParentWnd -> EnableWindow( TRUE );
		m_comboReason.SetFocus();
		break;

	case 6:
		m_pParentWnd -> EnableWindow( FALSE );
		Prompter.Error ( "The selected stock code is not in this stocktake" );
		m_pParentWnd -> EnableWindow( TRUE );
		break;

	case 7:
		m_pParentWnd -> EnableWindow( FALSE );
		Prompter.Error ( "The selected stock item is not active at this stockpoint" );
		m_pParentWnd -> EnableWindow( TRUE );
		break;

	default:
		if ( m_bAdd == TRUE )
			GetParent() -> PostMessage ( WM_APP, 0, (long) this );
		else
		{
			EndDialog ( IDOK );
			return;
		}
		break;
	}
	
	pNewFocus -> SetFocus();
	pNewFocus -> SetSel( 0, -1 );
}

/**********************************************************************/

int CAdjustmentsEditPluDlg::HandleAdd() 
{
	m_buttonOK.SetFocus();

	if ( m_nPluIdx < 0 || m_nPluIdx >= DataManager.Plu.GetSize() )
		return 1;

	if ( m_strLastAdjustment == "" )
		return 5;
	
	if ( m_dQuantity == 0.0 )
		return 4;
	
	CString strPluNo = DataManager.Plu.GetPluNoString ( m_nPluIdx );
	__int64 nPluNo = DataManager.Plu.GetPluNoInt ( m_nPluIdx );

	CExplodePluStockTake ExplodePlu( dbStockpoint.GetSpNo( m_nSpIdx ), m_nStkTakeNo );
	
	CStringArray StockDetails;
	int nResult = ExplodePlu.ExplodePlu ( nPluNo, StockDetails, AUDIT_ADJUSTMENT_PLU, 0, 0 );
	CCSVExplodedPlu csvItem ( StockDetails.GetAt ( 0 ) );

	switch( nResult )
	{
	case PLU_EXCEPTION_NONE:
	case PLU_EXCEPTION_PLUISRECIPE:
		break;

	case PLU_EXCEPTION_PLUNOTEXIST:
		return 1;

	case PLU_EXCEPTION_PLUNONSTOCK:
	case PLU_EXCEPTION_PLUNOTLINKED:
		return 3;

	case PLU_EXCEPTION_STOCKTAKE:
		return 6;

	case PLU_EXCEPTION_STOCKINACTIVE:
		return 7;

	case PLU_EXCEPTION_STOCKNOTEXIST:
	default:
		return 2;
	}
	
	CReportPluInfoFinder PluInfoFinder( nPluNo );
	
	CCSVStockAudit csvOut;
	csvOut.Add ( strPluNo );
	csvOut.Add ( PluInfoFinder.GetPluInfoRepText(FALSE) );
	csvOut.Add ( m_comboLevel.GetCurSel() );
	csvOut.AddStockAuditQty ( m_dQuantity );
	csvOut.AddStockAuditVal ( m_dValue );
	csvOut.Add ( PluInfoFinder.GetModifierDeptNo( m_nModifier ) );
	csvOut.Add ( m_nEntryMode );
	csvOut.Add ( m_strStockCode );

	if ( SysInfo.IsModifiableSSPluNo( nPluNo ) == TRUE )
	{
		csvOut.Add ( m_nModifier );
		csvOut.AddStockAuditQty ( m_dMultiplier );
		csvOut.AddStockAuditQty ( m_dQuantity * m_dMultiplier );
	}
	else
	{
		csvOut.Add ( 0 );
		csvOut.AddStockAuditQty ( 1.0 );
		csvOut.AddStockAuditQty ( m_dQuantity );
	}
	
	csvOut.Add ( m_strLastAdjustment );
	m_strOutputLine = csvOut.GetLine();
	
	return 0;
}

/**********************************************************************/

void CAdjustmentsEditPluDlg::OnCancel() 
{
	if ( m_bAdd == TRUE )
		DestroyWindow();
	else
		EndDialog( IDCANCEL );
}

/**********************************************************************/

void CAdjustmentsEditPluDlg::PostNcDestroy() 
{
	if ( m_bAdd == TRUE )
	{
		*m_pParentToThis = NULL;
		delete this;
	}
}

/**********************************************************************/
