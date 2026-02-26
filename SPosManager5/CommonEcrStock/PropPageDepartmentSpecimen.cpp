/**********************************************************************/
#include "FilterTypes.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPageDepartmentSpecimen.h"
/**********************************************************************/

CPropPageDepartmentSpecimen::CPropPageDepartmentSpecimen() : CSSPropertyPage(CPropPageDepartmentSpecimen::IDD),
	m_PluFilterArray ( FILTERTYPE_BROWSE_PLU )
{
	//{{AFX_DATA_INIT(CPropPageDepartmentSpecimen)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPageDepartmentSpecimen::~CPropPageDepartmentSpecimen()
{
}

/**********************************************************************/

void CPropPageDepartmentSpecimen::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageDepartmentSpecimen)
		
	DDX_IntegerString(pDX, IDC_EDIT_PLUNO, m_strPluNumber);
	DDV_IntegerString(pDX, m_strPluNumber, "0", Super.PluEnd() );
	DDX_Control(pDX, IDC_COMBO_MODIFIER, m_comboModifier);
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_editDescription);
	
	DDX_Control(pDX, IDC_CHECK_STOCK_TEMPLATE, m_checkStockTemplate);
	DDX_Control(pDX, IDC_CHECK_STOCK_DESCRIPTION, m_checkStockDescription);
	DDX_Control(pDX, IDC_CHECK_STOCK_CATEGORY, m_checkStockCategory);
	DDX_Control(pDX, IDC_CHECK_STOCK_QTY, m_checkStockQty);
	
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageDepartmentSpecimen, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageDepartmentSpecimen)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_CHECK_STOCK_TEMPLATE, OnToggleStock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageDepartmentSpecimen::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	SubclassEdit ( IDC_EDIT_PLUNO, SS_NUM, Super.MaxPluLen(), "%s" );

	for ( int n = 0; n <= MODIFIER_COUNT; n++ )
	{
		if ( n == 0 )
			m_comboModifier.AddString( "No modifier" );
		else
		{
			CString str;
			str.Format( "M%2.2d %s",
				n,
				DataManager.Modifier.GetDisplayName(n) );

			m_comboModifier.AddString( str );
		}
	}

#ifndef STOCKMAN_SYSTEM
	ShowAndEnableWindow( &m_checkStockTemplate, FALSE );
	ShowAndEnableWindow( &m_checkStockDescription, FALSE );
	ShowAndEnableWindow( &m_checkStockCategory, FALSE );
	ShowAndEnableWindow( &m_checkStockQty, FALSE );
#endif

	return TRUE;  
}

/**********************************************************************/
CEdit* CPropPageDepartmentSpecimen::GetEditPluNo() { return GetEdit( IDC_EDIT_PLUNO ); }
/**********************************************************************/

BOOL CPropPageDepartmentSpecimen::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageDepartmentSpecimen::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageDepartmentSpecimen::GetRecordData()
{
}

/**********************************************************************/

void CPropPageDepartmentSpecimen::SetRecordControls()
{
#ifdef STOCKMAN_SYSTEM
	m_checkStockTemplate.SetCheck( m_pDeptRecord -> GetStockTemplateFlag() );
	m_checkStockDescription.SetCheck( m_pDeptRecord -> GetStockDescriptionFlag() );
	m_checkStockCategory.SetCheck( m_pDeptRecord -> GetStockCategoryFlag() );
	m_checkStockQty.SetCheck( m_pDeptRecord -> GetStockQtyFlag() );
	OnToggleStock();
#endif

	LoadSQLPluNo( m_pDeptRecord -> GetSpecimenLearnPlu() );
}

/**********************************************************************/

bool CPropPageDepartmentSpecimen::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageDepartmentSpecimen::SaveRecord()
{
	__int64 nEPOSPluNo = m_nBasePluNo;
	if ( SysInfo.IsModifiableSSPluNo( nEPOSPluNo ) == TRUE )
	{
		nEPOSPluNo *= 10;
		nEPOSPluNo += m_comboModifier.GetCurSel();
	}

	m_pDeptRecord -> SetSpecimenLearnPlu( nEPOSPluNo );

#ifdef STOCKMAN_SYSTEM
	m_pDeptRecord -> SetStockTemplateFlag( m_checkStockTemplate.GetCheck() != 0 );
	m_pDeptRecord -> SetStockDescriptionFlag( m_checkStockDescription.GetCheck() != 0 );
	m_pDeptRecord -> SetStockCategoryFlag( m_checkStockCategory.GetCheck() != 0 );
	m_pDeptRecord -> SetStockQtyFlag( m_checkStockQty.GetCheck() != 0 );
#endif
}

/**********************************************************************/

void CPropPageDepartmentSpecimen::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/

void CPropPageDepartmentSpecimen::LoadSQLPluNo( __int64 nEposPluNo )
{
	CSQLPluNoInfo infoPluNo;
	infoPluNo.m_nEposPluNo = nEposPluNo;
	::ProcessPluNo( infoPluNo );

	m_nBasePluNo = infoPluNo.m_nBasePluNo;

	CString strPluNo;
	::FormatPluNo( m_nBasePluNo, strPluNo );
	GetEditPluNo() -> SetWindowText( strPluNo );

	int nPluIdx;
	if ( DataManager.Plu.FindPluByNumber( m_nBasePluNo, nPluIdx ) == TRUE )
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt( nPluIdx, PluRecord );
		m_editDescription.SetWindowText( PluRecord.GetReportText() );
	}
	else
		m_editDescription.SetWindowText( "Unknown" );

	if ( TRUE == infoPluNo.m_bModifiable )
	{
		m_comboModifier.EnableWindow( TRUE );
		m_comboModifier.SetCurSel( infoPluNo.m_nModifier );
	}
	else
	{
		m_comboModifier.EnableWindow( FALSE );
		m_comboModifier.SetCurSel(0);
	}
}

/**********************************************************************/

void CPropPageDepartmentSpecimen::OnToggleStock()
{
	bool bEnable = ( m_checkStockTemplate.GetCheck() != 0 );
	m_checkStockDescription.EnableWindow( bEnable );
	m_checkStockCategory.EnableWindow( bEnable );
	m_checkStockQty.EnableWindow( bEnable );
}

/**********************************************************************/

void CPropPageDepartmentSpecimen::OnButtonBrowse()
{
	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg( m_PluFilterArray, this, nDlgResult, nPluIdx );

	if ( IDOK != nDlgResult )
		return;

	if ( nPluIdx < 0 )
		return;

	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt( nPluIdx, PluRecord );
	
	__int64 nBasePluNo = PluRecord.GetPluNoInt();
	
	__int64 nEPOSPluNo = nBasePluNo;
	if ( SysInfo.IsModifiableSSPluNo( nEPOSPluNo ) == TRUE )
		nEPOSPluNo *= 10;

	LoadSQLPluNo( nEPOSPluNo );
	GetEditPluNo() -> SetFocus();
	GetEditPluNo() -> SetSel( 0, -1 );
}

/**********************************************************************/

void CPropPageDepartmentSpecimen::OnKillFocusPluNo()
{
	CString strBasePluNo;
	GetEditPluNo() -> GetWindowText( strBasePluNo );
	__int64 nBasePluNo = _atoi64( strBasePluNo );

	if ( nBasePluNo != m_nBasePluNo )
	{
		__int64 nEPOSPluNo = nBasePluNo;
		if ( SysInfo.IsModifiableSSPluNo( nEPOSPluNo ) == TRUE )
			nEPOSPluNo *= 10;

		LoadSQLPluNo( nEPOSPluNo );
	}
	else
	{
		CString strPluNo;
		::FormatPluNo( m_nBasePluNo, strPluNo );
		GetEditPluNo() -> SetWindowText( strPluNo );
	}
}

/**********************************************************************/
