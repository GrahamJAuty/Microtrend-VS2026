/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "EcrmanSupplierCSVArray.h"
#include "ListDataEcrmanSupplierDlg.h"
#include "MaxLengths.h"
#include "PriceHelpers.h"
#include "ReportHelpers.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PluDeliveryDlg.h"
/**********************************************************************/

CPluDeliveryDlg::CPluDeliveryDlg( int nPluIdx, CWnd* pParent) : CSSDialog("IDD_PLU_DELIVERY", pParent),
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName ),
	m_EcrmanSupplierSelector( m_editSupplier, m_comboSupplier, FALSE )
{
	m_bEditDepartments = TRUE;

	//{{AFX_DATA_INIT(CPluDeliveryDlg)
	m_nGPType = 0;
	//}}AFX_DATA_INIT
	m_dDelivery = 0.0;
	m_nPluIdx = nPluIdx;
	DataManager.Plu.GetAt( m_nPluIdx, m_PluRecord );

	CDataManagerInfo info;
	DataManagerNonDb.OpenEcrmanSupplier( DB_READONLY, info );
}

/**********************************************************************/

CPluDeliveryDlg::~CPluDeliveryDlg()
{
	CDataManagerInfo info;
	DataManagerNonDb.CloseEcrmanSupplier( info );
}

/**********************************************************************/

void CPluDeliveryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluDeliveryDlg)
	DDX_Control(pDX, IDC2_EDIT_SUPPLIER, m_editSupplier);
	DDX_Control(pDX, IDC2_COMBO_SUPPLIER, m_comboSupplier);
	DDX_Control(pDX, IDC2_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC2_COMBO_TAXBAND, m_comboTaxBand);
	DDX_Control(pDX, IDC2_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC2_EDIT_REPTEXT, m_editRepText);
	DDX_Control(pDX, IDC2_EDIT_EPOSTEXT, m_editEposText);
	DDX_Text(pDX, IDC2_EDIT_EPOSTEXT, m_strEposText);
	DDX_Text(pDX, IDC2_EDIT_REPTEXT, m_strRepText);
	DDX_Radio(pDX, IDC2_RADIO_GP_TYPE, m_nGPType);
	//}}AFX_DATA_MAP

	DDX_Text(pDX, IDC2_EDIT_COST, m_dCost );
	DDX_Text(pDX, IDC2_EDIT_GP_FIXED, m_dGPFixed );
	DDX_Text(pDX, IDC2_EDIT_GP_PERCENT, m_dGPPercent );
	DDX_Text(pDX, IDC2_EDIT_PRICE1, m_dBasePrice );
	DDX_Text(pDX, IDC2_EDIT_APPARENT, m_dApparent );
	DDX_Text(pDX, IDC2_EDIT_DELIVERY, m_dDelivery );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluDeliveryDlg, CDialog)
	//{{AFX_MSG_MAP(CPluDeliveryDlg)
	ON_CBN_SELCHANGE(IDC2_COMBO_DEPTNAME, OnSelectDept)
	ON_EN_KILLFOCUS(IDC2_EDIT_DEPTNO, OnKillFocusDeptNo)
	ON_CBN_SELCHANGE(IDC2_COMBO_TAXBAND, OnCalculateProfit)
	ON_BN_CLICKED(IDC2_RADIO_GP_FIXED, OnRadioGpFixed)
	ON_BN_CLICKED(IDC2_RADIO_GP_PERCENT, OnRadioGpPercent)
	ON_BN_CLICKED(IDC2_RADIO_GP_TYPE, OnRadioGpType)
	ON_EN_KILLFOCUS(IDC2_EDIT_COST, OnCalculateProfit)
	ON_EN_KILLFOCUS(IDC2_EDIT_GP_FIXED, OnCalculateProfit)
	ON_EN_KILLFOCUS(IDC2_EDIT_GP_PERCENT, OnCalculateProfit)
	ON_EN_KILLFOCUS(IDC2_EDIT_PRICE1, OnCalculateProfit)
	ON_EN_KILLFOCUS(IDC2_EDIT_SUPPLIER, OnKillFocusSupplier)
	ON_CBN_SELCHANGE(IDC2_COMBO_SUPPLIER, OnSelectSupplier)
	ON_BN_CLICKED(IDC2_BUTTON_SUPPLIER, OnButtonSupplier)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPluDeliveryDlg::FillTaxCombo()
{
	m_comboTaxBand.ResetContent();
	
	char cLetter = 'A';
	for ( int nTaxRate = 1; nTaxRate <= MAX_TAX_BANDS; nTaxRate++ )
	{
		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo( nTaxRate, TaxRateInfo );

		CString strBuf;
		strBuf.Format ( "  %.4lf", TaxRateInfo.m_dTaxRate );
		strBuf.SetAt ( 0, cLetter++ );
		int nPos = m_comboTaxBand.AddString ( strBuf );
		m_comboTaxBand.SetItemData ( nPos, nTaxRate );
	}

	int nTaxBand = m_PluRecord.GetBaseTaxBandInt();
	if ( nTaxBand > 0 && nTaxBand <= MAX_TAX_BANDS )
	{
		m_comboTaxBand.SetCurSel( nTaxBand - 1 );
		return;
	}

	m_comboTaxBand.InsertString( 0, "??" );
	m_comboTaxBand.SetItemData ( 0, 0 );
	m_comboTaxBand.SetCurSel( 0 );
}

/**********************************************************************/

int CPluDeliveryDlg::GetTaxBand()
{
	int nSelect = m_comboTaxBand.GetCurSel();
	if ( nSelect >= 0 && nSelect < m_comboTaxBand.GetCount() )
		return m_comboTaxBand.GetItemData(nSelect);
	else
		return 0;
}

/**********************************************************************/
CEdit* CPluDeliveryDlg::GetEditGPFixed() { return GetEdit( IDC2_EDIT_GP_FIXED ); }
CEdit* CPluDeliveryDlg::GetEditGPPercent() { return GetEdit( IDC2_EDIT_GP_PERCENT ); }
CEdit* CPluDeliveryDlg::GetEditCost() { return GetEdit( IDC2_EDIT_COST ); }
CEdit* CPluDeliveryDlg::GetEditBasePrice() { return GetEdit( IDC2_EDIT_PRICE1 ); }
CEdit* CPluDeliveryDlg::GetEditApparent() { return GetEdit( IDC2_EDIT_APPARENT ); }
CEdit* CPluDeliveryDlg::GetEditDelivery() { return GetEdit( IDC2_EDIT_DELIVERY ); }
/**********************************************************************/

BOOL CPluDeliveryDlg::OnInitDialog() 
{
	GetRecordData();
	CDialog::OnInitDialog();

	CString strText; 
	strText.Format ( "Delivery Plu %s, %s",
		m_PluRecord.GetPluNoString(),
		m_PluRecord.GetReportText() );
	
	SetWindowText( strText );

	m_editEposText.LimitText ( Plu::EposText.Max );
	m_editRepText.LimitText ( Plu::RepText.Max );
	
	m_editSupplier.LimitText(3);
	m_EcrmanSupplierSelector.FillEcrmanSupplierCombo( m_nSuppNo );

	SubclassEdit ( IDC2_EDIT_COST, SS_NUM_DP, Super.MaxCostLen(), "%.*f", 5 );
	SubclassEdit ( IDC2_EDIT_GP_FIXED, SS_NUM_DP, Super.MaxPriceLen(), "%.*f", SysInfo.GetDPValue() );
	SubclassEdit ( IDC2_EDIT_GP_PERCENT, SS_NUM_DP, 6, "%.2f" );
	SubclassEdit ( IDC2_EDIT_PRICE1, SS_NUM_DP, Super.MaxPriceLen(), "%.*f", SysInfo.GetDPValue() );
	SubclassEdit ( IDC2_EDIT_APPARENT, SS_NUM_SDP, 8, "%.*f", SysInfo.GetDPQty() );
	SubclassEdit ( IDC2_EDIT_DELIVERY, SS_NUM_DP, 8, "%.*f", SysInfo.GetDPQty() );
	
	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( m_nDeptNo, DEPARTMENT_SELECTOR_SINGLE );
	
	SetRecordControls();
	FillTaxCombo();
	return TRUE;  
}

/**********************************************************************/

void CPluDeliveryDlg::SaveRecord()
{
	OnKillFocusDeptNo();
	OnKillFocusSupplier();

	UpdatePrice1( GetTaxBand() );

	m_PluRecord.SetEposText ( m_strEposText );
	m_PluRecord.SetRepText ( m_strRepText );
	m_PluRecord.SetBaseDeptNo ( m_nDeptNo );
	m_PluRecord.SetSupplierNo( m_nSuppNo );
	m_PluRecord.SetBaseTaxBandInt( GetTaxBand() );
	m_PluRecord.SetGPType ( m_nGPType );
	m_PluRecord.SetPrice( 0, m_dBasePrice );
		
	m_PluRecord.SetPurchaseCost ( m_dCost );

	switch ( m_nGPType )
	{
	case PLU_GPTYPE_VARIABLE:
		m_PluRecord.SetProfitMargin ( 0.0 );
		break;

	case PLU_GPTYPE_FIXED:
		m_PluRecord.SetProfitMargin ( m_dGPFixed );
		break;

	case PLU_GPTYPE_PERCENT:
		m_PluRecord.SetProfitMargin ( m_dGPPercent );
		break;
	}

	if ( m_dDelivery != 0.0 )
	{
		m_PluRecord.SetApparentLevel( m_PluRecord.GetApparentLevel() + m_dDelivery );

		if ( EcrmanOptions.GetFeaturesPluAuditFlag() == TRUE )
		{
			CSSFile fileAudit;
			int nDbIdx = DataManager.GetActiveDbIdx();
			if ( fileAudit.Open ( dbDatabase.GetFilePathDataPluStockAuditTemp ( nDbIdx ), "ab" ) == TRUE )
			{
				CSSTime timeNow;
				CSSDate dateNow;
				timeNow.SetCurrentTime();
				dateNow.SetCurrentDate();

				CString strPluNo = m_PluRecord.GetPluNoString();
				::TrimSpaces ( strPluNo, TRUE );
	
				CCSV csv ( ',', '"', FALSE, FALSE, TRUE );
				csv.Add ( dateNow.GetCSVDate() );
				csv.Add ( timeNow.GetCSVTime() );
				csv.Add ( strPluNo );
				csv.Add ( m_PluRecord.GetReportText() );
				csv.Add ( m_PluRecord.GetBaseDeptNo() );
				csv.Add ( 0 );
				csv.Add ( m_dDelivery, SysInfo.GetDPQty() );
				csv.Add ( m_nSuppNo );

				fileAudit.WriteLine( csv.GetLine() );
			}
		}
	}
}

/**********************************************************************/

void CPluDeliveryDlg::GetRecordData()
{
	m_dCost = m_PluRecord.GetPurchaseCost();
	m_strRepText = m_PluRecord.GetRepText();
	m_strEposText = m_PluRecord.GetEposText();
	m_nDeptNo = m_PluRecord.GetBaseDeptNo();
	m_nSuppNo = m_PluRecord.GetSupplierNo();
	m_nGPType = m_PluRecord.GetGPType();
	m_dBasePrice = m_PluRecord.GetPrice(0);
	m_dApparent = m_PluRecord.GetApparentLevel();
	
	switch ( m_nGPType )
	{
	case PLU_GPTYPE_VARIABLE:
		m_dGPFixed = 0.0;
		m_dGPPercent = 0.0;
		break;

	case PLU_GPTYPE_FIXED:
		m_dGPFixed = m_PluRecord.GetProfitMargin();
		m_dGPPercent = 0.0;
		break;

	case PLU_GPTYPE_PERCENT:
		m_dGPFixed = 0.0;
		m_dGPPercent = m_PluRecord.GetProfitMargin();
		break;
	}
	UpdatePrice1( m_PluRecord.GetBaseTaxBandInt() );
}

/**********************************************************************/

void CPluDeliveryDlg::SetRecordControls()
{	
	m_EcrmanSupplierSelector.UpdateEcrmanSupplierCombo( m_nSuppNo );
	m_EcrmanSupplierSelector.UpdateEcrmanSupplierEditBox();
	m_nSuppNo = m_EcrmanSupplierSelector.GetEcrmanSupplierNo();
	
	m_DepartmentSelector.UpdateDepartmentCombo ( m_nDeptNo );
	m_DepartmentSelector.UpdateDepartmentEditBox();
	m_nDeptNo = m_DepartmentSelector.GetDepartmentFilter();
		
	UpdateData ( FALSE );
	SetGPEnables();

	if ( m_bEditDepartments == FALSE )
	{
		m_editDeptNo.EnableWindow ( FALSE );
		m_comboDeptName.EnableWindow ( FALSE );
	}
}

/**********************************************************************/

void CPluDeliveryDlg::OnKillFocusDeptNo() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
	m_nDeptNo = m_DepartmentSelector.GetDepartmentFilter();
}

/**********************************************************************/

void CPluDeliveryDlg::OnSelectDept() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
	m_nDeptNo = m_DepartmentSelector.GetDepartmentFilter();
}

/**********************************************************************/

void CPluDeliveryDlg::OnKillFocusSupplier() 
{
	m_EcrmanSupplierSelector.SelectEcrmanSupplierFromEditBox();
	m_nSuppNo = m_EcrmanSupplierSelector.GetEcrmanSupplierNo();
}

/**********************************************************************/

void CPluDeliveryDlg::OnSelectSupplier() 
{
	m_EcrmanSupplierSelector.SelectEcrmanSupplierFromCombo();
	m_nSuppNo = m_EcrmanSupplierSelector.GetEcrmanSupplierNo();	
}

/**********************************************************************/

void CPluDeliveryDlg::OnButtonSupplier() 
{
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenEcrmanSupplierBuffer( info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	CListDataEcrmanSupplierDlg dlg ( m_nSuppNo, this );
	
	int nSuppIdx;
	if ( ( m_nSuppNo >= EcrmanSupplier::SupplierNo.Min ) && ( m_nSuppNo <= EcrmanSupplier::SupplierNo.Max ) )
	{
		if ( DataManagerNonDb.EcrmanSupplier.FindSupplierByNumber( m_nSuppNo, nSuppIdx ) == FALSE )
		{
			CEcrmanSupplierCSVRecord Supplier;
			Supplier.SetSuppNo( m_nSuppNo );
			Supplier.SetSuppName( "" );
			DataManagerNonDb.EcrmanSupplier.InsertAt( nSuppIdx, Supplier );
		}
	}

	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManagerNonDb.WriteEcrmanSupplierBuffer( info ) == FALSE )
			Prompter.WriteError( info );
		else
		{
			int nSuppIdx = dlg.GetListSelection();
			if ( nSuppIdx >= 0 )
			{
				CEcrmanSupplierCSVRecord Supplier;
				DataManagerNonDb.EcrmanSupplier.GetAt( nSuppIdx, Supplier );
				m_nSuppNo = Supplier.GetSuppNo();
			}
		}
	}

	m_EcrmanSupplierSelector.FillEcrmanSupplierCombo( m_nSuppNo );
	DataManagerNonDb.CloseEcrmanSupplierBuffer( info );
}

/**********************************************************************/

void CPluDeliveryDlg::OnCalculateProfit() 
{
	if ( UpdateData ( TRUE, FALSE ) == TRUE )
	{
		UpdatePrice1( GetTaxBand() );
		SetGPEnables();
		UpdateData ( FALSE );
	}
}

/**********************************************************************/

void CPluDeliveryDlg::SetGPEnables()
{
	switch ( m_nGPType )
	{
	case PLU_GPTYPE_VARIABLE:
	default:
		GetEditGPFixed() -> EnableWindow ( FALSE );
		GetEditGPPercent() -> EnableWindow ( FALSE );
		GetEditBasePrice() -> EnableWindow ( TRUE );
		break;

	case PLU_GPTYPE_FIXED:
		GetEditGPFixed() -> EnableWindow ( TRUE );
		GetEditGPPercent() -> EnableWindow ( FALSE );
		GetEditBasePrice() -> EnableWindow ( FALSE );
		break;

	case PLU_GPTYPE_PERCENT:
		GetEditGPFixed() -> EnableWindow ( FALSE );
		GetEditGPPercent() -> EnableWindow ( TRUE );
		GetEditBasePrice() -> EnableWindow ( FALSE );
		break;
	}
}

/**********************************************************************/

void CPluDeliveryDlg::UpdatePrice1( int nTaxBand )
{
	CTaxRateInfo TaxRateInfo;
	DataManager.TaxRates.GetTaxRateInfo( nTaxBand, TaxRateInfo );

	switch ( m_nGPType )
	{
	default:
	case PLU_GPTYPE_VARIABLE:
		m_dGPFixed = ReportHelpers.CalcGP ( m_dBasePrice, TaxRateInfo.m_dTaxRate, m_dCost );
		m_dGPPercent = ReportHelpers.CalcGPPercent ( m_dBasePrice, TaxRateInfo.m_dTaxRate, m_dCost );
		break;

	case PLU_GPTYPE_FIXED:
		SetPrice1GPFixed( TaxRateInfo.m_dTaxRate );
		m_dGPPercent = ReportHelpers.CalcGPPercent ( m_dBasePrice, TaxRateInfo.m_dTaxRate, m_dCost );
		break;

	case PLU_GPTYPE_PERCENT:
		SetPrice1GPPercent ( TaxRateInfo.m_dTaxRate );
		m_dGPFixed = ReportHelpers.CalcGP ( m_dBasePrice, TaxRateInfo.m_dTaxRate, m_dCost );
		break;
	}

	if ( m_dBasePrice < 0.0 )
		m_dBasePrice = 0.0;
}

/**********************************************************************/

void CPluDeliveryDlg::SetPrice1GPFixed ( double dTaxRate )
{
	m_dBasePrice = CPriceHelpers::GetPrice1GPFixed ( dTaxRate, m_dCost, m_dGPFixed );
}

/**********************************************************************/

void CPluDeliveryDlg::SetPrice1GPPercent ( double dTaxRate )
{
	m_dBasePrice = CPriceHelpers::GetPrice1GPPercent ( dTaxRate, m_dCost, m_dGPPercent );
}

/**********************************************************************/

void CPluDeliveryDlg::OnRadioGpType() 
{
	m_nGPType = 0;
	SetGPEnables();	
}

/**********************************************************************/

void CPluDeliveryDlg::OnRadioGpFixed() 
{
	m_nGPType = 1;
	SetGPEnables();	
}

/**********************************************************************/

void CPluDeliveryDlg::OnRadioGpPercent() 
{
	m_nGPType = 2;
	SetGPEnables();	
}

/**********************************************************************/

void CPluDeliveryDlg::OnOK() 
{
	if ( UpdateData( TRUE, TRUE ) == TRUE )
	{
		SaveRecord();
		DataManager.Plu.SavePluRecord( m_nPluIdx, m_PluRecord );
		EndDialog( IDOK );
	}
}

/**********************************************************************/

