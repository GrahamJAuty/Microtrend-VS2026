/**********************************************************************/
#include "EcrmanSupplierCSVArray.h"
#include "ListDataEcrmanSupplierDlg.h"
#include "PluApparentDlg.h"
#include "Password.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPagePluStock.h"
/**********************************************************************/

CPropPagePluStock::CPropPagePluStock() : CSSPropertyPage(CPropPagePluStock::IDD),
	m_EcrmanSupplierSelector( m_editSupplier, m_comboSupplier, FALSE )
{
	//{{AFX_DATA_INIT(CPropPagePluStock)
	m_dApparent = 0.0;
	m_dMinimum = 0.0;
	m_strSupplierRef = _T("");
	m_bNonStockItem = FALSE;
	//}}AFX_DATA_INIT
	m_pPluRecord = NULL;
	m_bReadOnly = FALSE;

	CDataManagerInfo info;
	DataManagerNonDb.OpenEcrmanSupplier( DB_READONLY, info );
}

/**********************************************************************/

CPropPagePluStock::~CPropPagePluStock()
{
	CDataManagerInfo info;
	DataManagerNonDb.CloseEcrmanSupplier( info );
}

/**********************************************************************/

void CPropPagePluStock::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePluStock)
	DDX_Control(pDX, IDC_BUTTON_SUPPLIER, m_buttonSupplier);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_buttonClear);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_buttonAdd);
	DDX_Control(pDX, IDC_EDIT_SUPPREF, m_editSuppRef);
	DDX_Control(pDX, IDC_EDIT_SUPPLIER, m_editSupplier);
	DDX_Control(pDX, IDC_COMBO_SUPPLIER, m_comboSupplier);
	DDX_Control(pDX, IDC_CHECK_NONSTOCK, m_checkNonStockItem);
	DDX_Check(pDX, IDC_CHECK_NONSTOCK, m_bNonStockItem);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_SUPPREF, m_strSupplierRef);
	if ( m_bReadOnly == FALSE ) DDV_MaxChars(pDX, m_strSupplierRef, Plu::SupplierRef.Max);
	DDX_Text(pDX, IDC_EDIT_APPARENT, m_dApparent );
	DDX_Text(pDX, IDC_EDIT_MINIMUM, m_dMinimum );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePluStock, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePluStock)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_EN_KILLFOCUS(IDC_EDIT_SUPPLIER, OnKillFocusSupplier)
	ON_CBN_SELCHANGE(IDC_COMBO_SUPPLIER, OnSelectSupplier)
	ON_BN_CLICKED(IDC_BUTTON_SUPPLIER, OnButtonSupplier)
	ON_BN_CLICKED(IDC_CHECK_NONSTOCK, OnToggleNonStockItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPropPagePluStock::GetEditMinimum() { return GetEdit ( IDC_EDIT_MINIMUM ); }
/**********************************************************************/

BOOL CPropPagePluStock::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();
	SubclassEdit ( IDC_EDIT_APPARENT, SS_NUM_SDP, 8, "%.*f", SysInfo.GetDPQty() );
	SubclassEdit ( IDC_EDIT_MINIMUM, SS_NUM_DP, 8, "%.*f", SysInfo.GetDPQty() );
	
	m_editSupplier.LimitText(3);
	m_EcrmanSupplierSelector.FillEcrmanSupplierCombo( m_nSuppNo );

	SetRecordControls();
	return TRUE;  
}

/**********************************************************************/

BOOL CPropPagePluStock::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePluStock::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPagePluStock::GetRecordData()
{
	m_bNonStockItem = m_pPluRecord -> GetNonStockExternalFlag();
	m_dApparent = m_pPluRecord -> GetApparentLevel();
	m_dMinimum = m_pPluRecord -> GetMinimumLevel();
	m_strSupplierRef = m_pPluRecord -> GetSupplierRef();
	m_nSuppNo = m_pPluRecord -> GetSupplierNo();
}

/**********************************************************************/

void CPropPagePluStock::SaveRecord()
{
	if ( m_bReadOnly == TRUE )
		return;

	OnKillFocusSupplier();

	m_pPluRecord -> SetNonStockInternalFlag( m_bNonStockItem != 0 );
	m_pPluRecord -> SetApparentLevel ( m_dApparent );
	m_pPluRecord -> SetMinimumLevel ( m_dMinimum );
	m_pPluRecord -> SetSupplierRef( m_strSupplierRef );
	m_pPluRecord -> SetSupplierNo( m_nSuppNo );
}

/**********************************************************************/

bool CPropPagePluStock::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPagePluStock::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/

void CPropPagePluStock::SetRecordControls()
{
	m_EcrmanSupplierSelector.UpdateEcrmanSupplierCombo( m_nSuppNo );
	m_EcrmanSupplierSelector.UpdateEcrmanSupplierEditBox();
	m_nSuppNo = m_EcrmanSupplierSelector.GetEcrmanSupplierNo();
	UpdateEnables();
}

/**********************************************************************/

void CPropPagePluStock::OnButtonAdd() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	CPluApparentDlg dlg ( this );
		
	if ( dlg.DoModal() != IDOK || dlg.m_dApparent == 0.0 )
		return;

	m_dApparent += dlg.m_dApparent;	
	UpdateData ( FALSE );

	if ( EcrmanOptions.GetFeaturesPluAuditFlag() == FALSE )
		return;

	CSSTime timeNow;
	CSSDate dateNow;
	timeNow.SetCurrentTime();
	dateNow.SetCurrentDate();

	CString strPluNo = m_pPluRecord -> GetPluNoString();
	::TrimSpaces ( strPluNo, TRUE );
	
	CCSV csv ( ',', '"', FALSE, FALSE, TRUE );
	csv.Add ( dateNow.GetCSVDate() );
	csv.Add ( timeNow.GetCSVTime() );
	csv.Add ( strPluNo );
	csv.Add ( m_pPluRecord -> GetReportText() );
	csv.Add ( m_pPluRecord -> GetBaseDeptNo() );
	csv.Add ( 0 );
	csv.Add ( dlg.m_dApparent, SysInfo.GetDPQty() );
	csv.Add ( m_nSuppNo );
			
	m_pPluAuditMemory -> Add ( csv.GetLine() );				
}

/**********************************************************************/

void CPropPagePluStock::OnButtonClear() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	m_dApparent = 0.0;
	UpdateData ( FALSE );

	if ( EcrmanOptions.GetFeaturesPluAuditFlag() == FALSE )
		return;

	CSSTime timeNow;
	CSSDate dateNow;
	timeNow.SetCurrentTime();
	dateNow.SetCurrentDate();

	CString strPluNo = m_pPluRecord -> GetPluNoString();
	::TrimSpaces ( strPluNo, TRUE );
	
	CCSV csv ( ',', '"', FALSE, FALSE, TRUE );
	csv.Add ( dateNow.GetCSVDate() );
	csv.Add ( timeNow.GetCSVTime() );
	csv.Add ( strPluNo );
	csv.Add ( m_pPluRecord -> GetReportText() );	
	csv.Add ( m_pPluRecord -> GetBaseDeptNo() );
	csv.Add ( 1 );
	csv.Add ( -m_dApparent, SysInfo.GetDPQty() );
	csv.Add ( m_nSuppNo );
			
	m_pPluAuditMemory -> Add ( csv.GetLine() );
}

/**********************************************************************/

void CPropPagePluStock::OnKillFocusSupplier() 
{
	m_EcrmanSupplierSelector.SelectEcrmanSupplierFromEditBox();
	m_nSuppNo = m_EcrmanSupplierSelector.GetEcrmanSupplierNo();
}

/**********************************************************************/

void CPropPagePluStock::OnSelectSupplier() 
{
	m_EcrmanSupplierSelector.SelectEcrmanSupplierFromCombo();
	m_nSuppNo = m_EcrmanSupplierSelector.GetEcrmanSupplierNo();	
}

/**********************************************************************/

void CPropPagePluStock::OnButtonSupplier() 
{
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenEcrmanSupplierBuffer( info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	//READING FROM THE FILE MAY CHANGE THE SUPPLIER LIST
	m_EcrmanSupplierSelector.FillEcrmanSupplierCombo( m_nSuppNo );

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

			m_EcrmanSupplierSelector.FillEcrmanSupplierCombo( m_nSuppNo );
		}
	}
	
	DataManagerNonDb.CloseEcrmanSupplierBuffer( info );
}

/**********************************************************************/

void CPropPagePluStock::OnToggleNonStockItem() 
{
	UpdateData( TRUE, FALSE );
	UpdateEnables();
}

/**********************************************************************/

void CPropPagePluStock::UpdateEnables()
{
	if ( m_bReadOnly == FALSE )
	{
		bool bEnable = ( m_bNonStockItem == 0 );

		m_checkNonStockItem.EnableWindow( m_pPluRecord -> GotEcrmanRecipeTableArray() == FALSE ); 
		m_buttonClear.EnableWindow( bEnable );
		m_buttonAdd.EnableWindow( bEnable );
		m_buttonSupplier.EnableWindow( bEnable);
		m_editSupplier.EnableWindow( bEnable );
		m_comboSupplier.EnableWindow( bEnable );
		m_editSuppRef.EnableWindow( bEnable );
		GetEditMinimum() -> EnableWindow( bEnable );
	}
	else
	{
		m_buttonAdd.ShowWindow( SW_HIDE );
		m_buttonClear.ShowWindow( SW_HIDE );
		m_buttonSupplier.ShowWindow( SW_HIDE );
		m_comboSupplier.EnableWindow( FALSE );
		m_editSupplier.EnableWindow( FALSE );
		m_editSuppRef.EnableWindow( FALSE );
		m_checkNonStockItem.EnableWindow( FALSE );
		GetEditMinimum() -> EnableWindow( FALSE );
	}

	if ( PasswordArray.GetEnable( PasswordTicks::PluAddStock ) == FALSE )
	{
		m_buttonAdd.EnableWindow( FALSE );
		m_buttonAdd.ShowWindow( SW_HIDE );
	}

	if ( PasswordArray.GetEnable( PasswordTicks::PluClearStock ) == FALSE )
	{
		m_buttonClear.EnableWindow( FALSE );
		m_buttonClear.ShowWindow( SW_HIDE );
	}
}

/**********************************************************************/
