/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "FilterTypes.h"
#include "..\SPosEcrManager5\PluFilterDlgEcrman.h"
#include "..\SPosStockManager5\PluFilterDlgStockman.h"
#include "ReportHelpers.h"
#include "ReportPluInfoFinder.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PMSManualSalesEditDlg.h"
/**********************************************************************/

CPMSManualSalesEditDlg::CPMSManualSalesEditDlg( CPMSManualSalesEditDlg** pParentToThis, int nLineType, CWnd* pParent)
	: CSSDialog(CPMSManualSalesEditDlg::IDD, pParent), 
	m_FilterArray ( FILTERTYPE_BROWSE_PLU ) 
{
	DataInit();

	m_bAdd = TRUE;
	
	m_pParentToThis = pParentToThis;
	*m_pParentToThis = this;

	m_SaleInfo.m_nLineType = nLineType;
}

/**********************************************************************/

CPMSManualSalesEditDlg::CPMSManualSalesEditDlg( CWnd* pParent)
	: CSSDialog(CPMSManualSalesEditDlg::IDD, pParent), 
	m_FilterArray ( FILTERTYPE_BROWSE_PLU )
{
	DataInit();
	m_bAdd = FALSE;
}

/**********************************************************************/

void CPMSManualSalesEditDlg::DataInit()
{
	//{{AFX_DATA_INIT(CPMSManualSalesEditDlg)
	//}}AFX_DATA_INIT
	m_bModified = FALSE;
	m_dQuantity = 1.0;
	m_dValue = 0.0;
	m_dDiscount = 0.0;
	m_dNet = 0.0;
	m_strPluNo = "";
	m_nPluIdx = -1;

	m_strInputLine = "";
	m_strOutputLine = "";

	m_nModifier = 0;
	m_dMultiplier = 1.0;
}

/**********************************************************************/

void CPMSManualSalesEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPMSManualSalesEditDlg)
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_PLU, m_buttonBrowsePlu);
	DDX_Control(pDX, IDC_STATIC_VALUE, m_staticValue);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDC_COMBO_LEVEL, m_comboLevel);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_strDescription );
	DDX_Control(pDX, IDC_COMBO_MODIFIER, m_comboModifier);
	DDX_Control(pDX, IDC_EDIT_MULTIPLIER, m_editMultiplier);
	DDX_Control(pDX, IDC_STATIC_MODIFIER1, m_staticModifier1);
	DDX_Control(pDX, IDC_STATIC_MODIFIER2, m_staticModifier2);
	DDX_Control(pDX, IDC_STATIC_SALES, m_staticSales);
	DDX_Control(pDX, IDC_STATIC_DISCOUNT, m_staticDiscount);
	DDX_Control(pDX, IDC_STATIC_NET, m_staticNet);
	//}}AFX_DATA_MAP
	DDX_Text ( pDX, IDC_EDIT_QTY, m_dQuantity );	
	DDX_Text(pDX, IDC_EDIT_VALUE, m_dValue );
	DDX_Text(pDX, IDC_EDIT_DISCOUNT, m_dDiscount );
	DDX_Text(pDX, IDC_EDIT_NET, m_dNet );
	DDX_IntegerString(pDX, IDC_EDIT_PLUNO, m_strPluNo);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSManualSalesEditDlg, CDialog)
	//{{AFX_MSG_MAP(CPMSManualSalesEditDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO, OnKillFocusPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_QTY, OnKillFocusQty)
	ON_EN_KILLFOCUS(IDC_EDIT_VALUE, OnKillFocusValue)
	ON_EN_KILLFOCUS(IDC_EDIT_DISCOUNT, OnKillFocusDiscount)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU, OnButtonBrowsePlu)
	ON_CBN_SELCHANGE(IDC_COMBO_LEVEL, OnSelectLevel)
	ON_CBN_SELCHANGE(IDC_COMBO_MODIFIER, OnSelectModifier)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPMSManualSalesEditDlg::GetEditQty() { return GetEdit( IDC_EDIT_QTY ); }
CEdit* CPMSManualSalesEditDlg::GetEditValue() { return GetEdit( IDC_EDIT_VALUE ); }
CEdit* CPMSManualSalesEditDlg::GetEditDiscount() { return GetEdit( IDC_EDIT_DISCOUNT ); }
CEdit* CPMSManualSalesEditDlg::GetEditNet() { return GetEdit( IDC_EDIT_NET ); }
CEdit* CPMSManualSalesEditDlg::GetEditPluNo() { return GetEdit( IDC_EDIT_PLUNO ); }
/**********************************************************************/

BOOL CPMSManualSalesEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SubclassEdit ( IDC_EDIT_QTY, SS_NUM_SDP, 8, "%.5f" );
	SubclassEdit ( IDC_EDIT_VALUE, SS_NUM_SDP, 8, "%.*f", SysInfo.GetDPValue() );
	SubclassEdit ( IDC_EDIT_DISCOUNT, SS_NUM_SDP, 8, "%.*f", SysInfo.GetDPValue() );
	SubclassEdit ( IDC_EDIT_NET, SS_NUM_SDP, 8, "%.*f", SysInfo.GetDPValue() );
	SubclassEdit ( IDC_EDIT_PLUNO, SS_NUM, Super.MaxPluLen(), "%s" );

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(40);
	m_comboLevel.SetTabStops( arrayStops );

	for ( int nLevel = 0; nLevel < 10; nLevel++ )
	{
		CString strNum;
		strNum.Format ( "%2.2d", nLevel + 1 );
		CCSV csv;
		csv.Add( strNum );
		csv.Add( DataManager.PriceText.GetReportText( nLevel ) );
		m_comboLevel.AddItem( csv.GetLine() );
	}

	m_comboLevel.SetCurSel(0);

	switch( m_SaleInfo.m_nLineType )
	{
	case PMS_LINE_SALE:
		m_staticSales.SetWindowText( "Enter Sales" );
		break;

	case PMS_LINE_REFUND:
		m_staticSales.SetWindowText( "Enter Refunds" );
		break;

	case PMS_LINE_ALLOWANCE:
		m_staticSales.SetWindowText( "Enter Allowances" );
		break;

	default:
		break;
	}

	switch( m_SaleInfo.m_nLineType )
	{
	case PMS_LINE_REFUND:
	case PMS_LINE_ALLOWANCE:
		m_staticDiscount.ShowWindow( SW_HIDE );
		m_staticNet.ShowWindow( SW_HIDE );
		GetEditDiscount() -> ShowWindow( SW_HIDE );
		GetEditNet() -> ShowWindow( SW_HIDE );
		break;

	default:
		break;
	}

	CString strTitle;

	if ( m_bAdd == TRUE )
	{
		switch( m_SaleInfo.m_nLineType )
		{
		case PMS_LINE_SALE:
			strTitle = "Add Sale";
			break;

		case PMS_LINE_REFUND:
			strTitle = "Add Refund";
			break;

		case PMS_LINE_ALLOWANCE:
			strTitle = "Add Allowance";
			break;

		default:
			strTitle= "";
			break;
		}

		UpdateModifierCombo();
	}
	else
	{
		switch( m_SaleInfo.m_nLineType )
		{
		case PMS_LINE_SALE:
			strTitle = "Edit Sale";
			break;

		case PMS_LINE_REFUND:
			strTitle = "Edit Refund";
			break;

		case PMS_LINE_ALLOWANCE:
			strTitle = "Edit Allowance";
			break;

		default:
			strTitle= "";
			break;
		}
		
		::FormatPluNo( m_SaleInfo.m_nBasePluNo, m_strPluNo );
		m_strDescription = m_SaleInfo.m_strDescription;

		m_dQuantity = m_SaleInfo.m_dEposPluQty;
		m_dValue = m_SaleInfo.m_dValue;
		m_dDiscount = m_SaleInfo.m_dDiscount;
		m_dNet = m_SaleInfo.m_dValue - m_SaleInfo.m_dDiscount;
		
		switch( m_SaleInfo.m_nLineType )
		{
		case PMS_LINE_SALE:
			m_dValue = m_SaleInfo.m_dValue;
			m_dDiscount = m_SaleInfo.m_dDiscount;
			m_dNet = m_SaleInfo.m_dValue - m_SaleInfo.m_dDiscount;
			break;

		case PMS_LINE_REFUND:
			m_dValue = m_SaleInfo.m_dValue;
			m_dDiscount = 0.0;
			m_dNet = m_dValue;
			break;

		case PMS_LINE_ALLOWANCE:
			m_dValue = m_SaleInfo.m_dAllowance;
			m_dDiscount = 0.0;
			m_dNet = m_dValue;
			break;
		}

		int nLevel = m_SaleInfo.m_nLevel;
		if ( nLevel < 0 || nLevel >= m_comboLevel.GetCount() )
			nLevel = 0;

		m_comboLevel.SetCurSel( nLevel );

		if ( DataManager.Plu.FindPluByNumber( _atoi64(m_strPluNo), m_nPluIdx ) == FALSE )
			m_nPluIdx = -1;

		UpdateModifierCombo();
		
		m_nModifier = 0;
		m_dMultiplier = 1.0;
		
		int nMod = m_SaleInfo.m_nModifier;
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
	}

	if ( SysInfo.GetMaxBasePluLen() == 0 )
	{
		m_staticModifier1.ShowWindow( SW_HIDE );
		m_staticModifier2.ShowWindow( SW_HIDE );
		m_comboModifier.SetCurSel(0);
		m_comboModifier.EnableWindow( FALSE );
		m_comboModifier.ShowWindow( SW_HIDE );
		m_editMultiplier.ShowWindow( SW_HIDE );
	}

	SetWindowText ( strTitle );
	CenterWindow();
	
	//WE HAVE SET FOCUS TO A CONTROL
	return FALSE;  
}

/**********************************************************************/

void CPMSManualSalesEditDlg::UpdateModifierCombo()
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

bool CPMSManualSalesEditDlg::LookUpPluNumber( __int64 nPluNo, int& nPluIdx )
{
	nPluIdx = -1;

	bool bFound = FALSE;

	if ( nPluNo != 0 )
	{
		if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == TRUE )
			bFound = TRUE;
		else
			nPluIdx = -1;
	}

	return bFound;
}

/**********************************************************************/

void CPMSManualSalesEditDlg::OnKillFocusPluNo() 
{
	UpdateData ( TRUE, FALSE );
	m_strPluNo = BarcodePluNoTable.ConvertPlu( m_strPluNo );
	__int64 nPluNo = _atoi64( m_strPluNo );
	LookUpPluNumber( nPluNo, m_nPluIdx );
	UpdatePluNumber( m_nPluIdx );
	
	CPluCSVRecord PluRecord;
	if ( m_nPluIdx != -1 )
		DataManager.Plu.GetAt( m_nPluIdx, PluRecord );
	
	UpdateItemValue( PluRecord, 1.0 );
	UpdateModifierCombo();
	UpdateData(FALSE);
}

/***********************************************************************/

void CPMSManualSalesEditDlg::UpdatePluNumber( int nPluIdx )
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

void CPMSManualSalesEditDlg::OnKillFocusQty() 
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

void CPMSManualSalesEditDlg::OnKillFocusValue() 
{
	UpdateData( TRUE, FALSE );
	m_dNet = m_dValue - m_dDiscount;	
	UpdateData ( FALSE );
	GetEditDiscount() -> SetSel ( 0, -1 );
}

/**********************************************************************/

void CPMSManualSalesEditDlg::OnKillFocusDiscount() 
{
	UpdateData( TRUE, FALSE );
	m_dNet = m_dValue - m_dDiscount;	
	UpdateData ( FALSE );
	GetEditValue() -> SetSel ( 0, -1 );
}

/**********************************************************************/

void CPMSManualSalesEditDlg::OnButtonBrowsePlu() 
{
	m_pParentWnd -> EnableWindow( FALSE );
	BrowsePluInternal();
	m_pParentWnd -> EnableWindow( TRUE );
}

/**********************************************************************/

void CPMSManualSalesEditDlg::BrowsePluInternal()
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
	__int64 nPluNo = PluRecord.GetPluNoInt();

	UpdatePluNumber( nPluIdx );
	UpdateModifierCombo();
	UpdateData ( FALSE );

	GetEditPluNo() -> SetFocus();
	GetEditPluNo() -> SetSel ( 0, -1 );
}

/**********************************************************************/

void CPMSManualSalesEditDlg::OnSelectLevel()
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

void CPMSManualSalesEditDlg::OnSelectModifier() 
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

void CPMSManualSalesEditDlg::UpdateItemValue ( CPluCSVRecord& PluRecord, double dQty )
{
	__int64 nPluNo = PluRecord.GetPluNoInt();

	if ( SysInfo.IsModifiableSSPluNo( nPluNo ) == TRUE )
	{
		m_dQuantity = dQty;
		m_dValue = m_dQuantity * PluRecord.GetModifierPrice( m_nModifier, m_comboLevel.GetCurSel() );
		m_dNet = m_dValue - m_dDiscount;
	}
	else
	{
		m_dQuantity = dQty;
		m_dValue = m_dQuantity * PluRecord.GetPrice( m_comboLevel.GetCurSel() );
		m_dNet = m_dValue - m_dDiscount;
	}
}

/**********************************************************************/

void CPMSManualSalesEditDlg::OnOK() 
{
	m_buttonOK.SetFocus();

	if ( UpdateData ( TRUE ) == FALSE )
		return;

	CEdit* pNewFocus = GetEditPluNo();	
	
	int nResult = HandleAdd();

	switch ( nResult )
	{
	case 1:
		m_pParentWnd -> EnableWindow( FALSE );
		Prompter.Error ( "The selected Plu number does not exist in your database" );
		m_pParentWnd -> EnableWindow( TRUE );
		break;

	case 4:
		m_pParentWnd -> EnableWindow( FALSE );
		Prompter.Error ( "Please enter a non-zero quantity" );
		m_pParentWnd -> EnableWindow( TRUE );
		pNewFocus = GetEditQty();
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

int CPMSManualSalesEditDlg::HandleAdd() 
{
	m_buttonOK.SetFocus();

	if ( m_nPluIdx < 0 || m_nPluIdx >= DataManager.Plu.GetSize() )
		return 1;

	if ( m_dQuantity == 0.0 )
		return 4;
	
	CString strPluNo = DataManager.Plu.GetPluNoString ( m_nPluIdx );
	__int64 nPluNo = DataManager.Plu.GetPluNoInt( m_nPluIdx );
		
	int nLevel = m_comboLevel.GetCurSel();
	
	CReportPluInfoFinder PluInfoFinder( nPluNo );

	m_SaleInfo.m_nBasePluNo = nPluNo;
	m_SaleInfo.m_nModifier = m_nModifier;
	m_SaleInfo.m_strDescription = PluInfoFinder.GetPluInfoRepText(FALSE);
	m_SaleInfo.m_nLevel = m_comboLevel.GetCurSel();
	m_SaleInfo.m_dEposPluQty = m_dQuantity;
	m_SaleInfo.m_dBasePluQty = m_dQuantity * m_dMultiplier;
	m_SaleInfo.m_nDeptNo = PluInfoFinder.GetModifierDeptNo( m_nModifier );

	int nDeptIdx;
	int nGroupNo = 0;
	if ( DataManager.Department.FindDeptByNumber( m_SaleInfo.m_nDeptNo, nDeptIdx ) == TRUE )
	{
		CDepartmentCSVRecord DeptRecord;
		DataManager.Department.GetAt( nDeptIdx, DeptRecord );
		nGroupNo = DeptRecord.GetEposGroup();
	}

	m_SaleInfo.m_nGroupNo = nGroupNo;

	switch( m_SaleInfo.m_nLineType )
	{
	case PMS_LINE_SALE:
		m_SaleInfo.m_dValue = m_dValue;
		m_SaleInfo.m_dAllowance = 0.0;
		m_SaleInfo.m_dDiscount = m_dDiscount;

		if ( TRUE == m_bAdd )
			m_SaleInfo.m_nTaxBand = PluInfoFinder.GetModifierTaxBand(m_nModifier);

		break;

	case PMS_LINE_REFUND:
		m_SaleInfo.m_dValue = m_dValue;
		m_SaleInfo.m_dAllowance = 0.0;
		m_SaleInfo.m_dDiscount = 0.0;

		if ( TRUE == m_bAdd )
			m_SaleInfo.m_nTaxBand = PluInfoFinder.GetModifierTaxBand(m_nModifier);

		break;

	case PMS_LINE_ALLOWANCE:
		m_SaleInfo.m_dValue = 0.0;
		m_SaleInfo.m_dAllowance = m_dValue;
		m_SaleInfo.m_dDiscount = 0.0;
		break;
	}

	return 0;
}

/**********************************************************************/

void CPMSManualSalesEditDlg::OnCancel() 
{
	if ( m_bAdd == TRUE )
		DestroyWindow();
	else
		EndDialog( IDCANCEL );
}

/**********************************************************************/

void CPMSManualSalesEditDlg::PostNcDestroy() 
{
	if ( m_bAdd == TRUE )
	{
		*m_pParentToThis = NULL;
		delete this;
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
