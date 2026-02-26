/**********************************************************************/
#include "SageOptions.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPagePromotionReport.h"
/**********************************************************************/

CPropPagePromotionReport::CPropPagePromotionReport() : CSSPropertyPage(CPropPagePromotionReport::IDD),
	m_DepartmentSelectorReport( m_editDeptNoReport, m_comboDeptNameReport )
{
	//{{AFX_DATA_INIT(CPropPagePromotionReport)
	//}}AFX_DATA_INIT
	m_bUpdatingTabs = FALSE;
	m_nReportAsPluType = 1;
	m_pPromoRecord = NULL;
}

/**********************************************************************/

CPropPagePromotionReport::~CPropPagePromotionReport()
{
}

/**********************************************************************/

void CPropPagePromotionReport::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePromotionReport)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RADIO_PLU1, m_radioPlu1);
	DDX_Control(pDX, IDC_RADIO_PLU2, m_radioPlu2);
	DDX_Control(pDX, IDC_RADIO_PLU3, m_radioPlu3);
	DDX_Control(pDX, IDC_CHECK_ALWAYSTAX, m_checkAlwaysTax);
	DDX_Control(pDX, IDC_EDIT_PLUNO, m_editPluNo);
	DDX_Control(pDX, IDC_EDIT_DEPTNO_REPORT, m_editDeptNoReport);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME_REPORT, m_comboDeptNameReport);
	DDX_Control(pDX, IDC_COMBO_SALESQTY, m_comboSalesQty);
	DDX_Control(pDX, IDC_STATIC_SAGE, m_staticSage);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePromotionReport, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePromotionReport)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_PLU1, OnRadioPlu1)
	ON_BN_CLICKED(IDC_RADIO_PLU2, OnRadioPlu2)
	ON_BN_CLICKED(IDC_RADIO_PLU3, OnRadioPlu3)
	ON_BN_CLICKED(IDC_CHECK_ALWAYSTAX, OnToggleAlwaysTax)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO_REPORT, OnKillFocusDeptNoReport)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME_REPORT, OnSelectDepartmentReport)
END_MESSAGE_MAP()

/**********************************************************************/

void CPropPagePromotionReport::SetRadioPlu2Text()
{
	bool bCanHaveNonVATableDiscount = TRUE;
	bCanHaveNonVATableDiscount &= (DealerFlags.GetAllDiscountsVATableFlag() == FALSE);
	bCanHaveNonVATableDiscount &= (IsTicked(m_checkAlwaysTax) == FALSE);
		
	if (TRUE == bCanHaveNonVATableDiscount)
	{
		m_radioPlu2.SetWindowText(EcrmanOptions.GetTaxString("Only for discounts, markdowns and surcharges to which %T does not apply."));
	}
	else
	{
		m_radioPlu2.SetWindowText(EcrmanOptions.GetTaxString("Only for surcharges to which %T does not apply."));
	}
}

/**********************************************************************/

BOOL CPropPagePromotionReport::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_checkAlwaysTax.SetWindowText(EcrmanOptions.GetTaxString("Always apply %T when processing discounts and markdowns."));

	if (DealerFlags.GetAllDiscountsVATableFlag() == TRUE)
	{
		ShowAndEnableWindow(&m_checkAlwaysTax,FALSE);
	}

	m_comboSalesQty.AddString( "Zero" );
	m_comboSalesQty.AddString( "One" );

	SubclassEdit ( IDC_EDIT_SAGE, SS_NUM, 8, "%d" );

	if ( SysInfo.IsSageExportSystem() == FALSE )
	{
		m_staticSage.ShowWindow( SW_HIDE );
		GetEditSage() -> ShowWindow( SW_HIDE );
	}

	return TRUE;  
}

/**********************************************************************/
CEdit* CPropPagePromotionReport::GetEditSage(){ return GetEdit( IDC_EDIT_SAGE ); }
/**********************************************************************/

void CPropPagePromotionReport::OnToggleAlwaysTax()
{
	SetRadioPlu2Text();
}

/**********************************************************************/

BOOL CPropPagePromotionReport::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePromotionReport::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPagePromotionReport::OnRadioPlu1()
{
	m_radioPlu1.SetCheck( TRUE );
	m_radioPlu2.SetCheck( FALSE );
	m_radioPlu3.SetCheck( FALSE );
	EnableSeparatePluSettings( TRUE );
	m_nReportAsPluType = 0;
}

/**********************************************************************/

void CPropPagePromotionReport::OnRadioPlu2()
{
	m_radioPlu1.SetCheck( FALSE );
	m_radioPlu2.SetCheck( TRUE );
	m_radioPlu3.SetCheck( FALSE );
	EnableSeparatePluSettings( TRUE );
	m_nReportAsPluType = 1;
}

/**********************************************************************/

void CPropPagePromotionReport::OnRadioPlu3()
{
	m_radioPlu1.SetCheck( FALSE );
	m_radioPlu2.SetCheck( FALSE );
	m_radioPlu3.SetCheck( TRUE );
	EnableSeparatePluSettings( FALSE );
	m_nReportAsPluType = 2;
}

/**********************************************************************/

void CPropPagePromotionReport::EnableSeparatePluSettings( bool b )
{
	m_editDeptNoReport.EnableWindow(b);
	m_comboDeptNameReport.EnableWindow(b);
	m_comboSalesQty.EnableWindow(b);
}

/**********************************************************************/

void CPropPagePromotionReport::OnKillFocusDeptNoReport() 
{
	m_DepartmentSelectorReport.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CPropPagePromotionReport::OnSelectDepartmentReport() 
{
	m_DepartmentSelectorReport.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CPropPagePromotionReport::SetRecordControls()
{
	bool bLockSeparate = FALSE;
	if (m_pPromoRecord->GetServiceChargeVATFixBeforeEditing() == TRUE)
	{
		if (m_pPromoRecord->GetServiceChargeVATFixFlag() == TRUE)
		{
			m_nReportAsPluType = 0;
			OnRadioPlu1();
			bLockSeparate = TRUE;
		}
	}

	if (FALSE == bLockSeparate)
	{
		m_nReportAsPluType = m_pPromoRecord->GetReportAsPluType();

		switch (m_nReportAsPluType)
		{
		case 0:
			OnRadioPlu1();
			break;

		case 2:
			OnRadioPlu3();
			break;

		case 1:
		default:
			OnRadioPlu2();
			break;
		}
	}

	m_radioPlu1.EnableWindow(FALSE == bLockSeparate);
	m_radioPlu2.EnableWindow(FALSE == bLockSeparate);
	m_radioPlu3.EnableWindow(FALSE == bLockSeparate);

	m_checkAlwaysTax.SetCheck(m_pPromoRecord->GetDiscountAlwaysVATableFlag());

	CString strPluNo;
	strPluNo.Format( "990000000000%2.2d", m_pPromoRecord -> GetPromoNo() );
	m_editPluNo.SetWindowText( strPluNo );

	m_DepartmentSelectorReport.FillDepartmentComboDatabase( m_pPromoRecord -> GetDeptNoReport(), DEPARTMENT_SELECTOR_SINGLE );
	m_comboSalesQty.SetCurSel( m_pPromoRecord ->GetReportAsZeroQtyFlag() ? 0 : 1 );

	if ( m_pPromoRecord -> GetSageNominalCode() == 0 )
		GetEditSage() -> SetWindowText( "" );
	else
	{
		CString str;
		int nMaxLen = SageOptions.GetNominalCodeLength();
		str.Format( "%*.*d", nMaxLen, nMaxLen, m_pPromoRecord -> GetSageNominalCode() );
		GetEditSage() -> SetWindowText ( str );
	}

	SetRadioPlu2Text();
}

/**********************************************************************/

bool CPropPagePromotionReport::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPagePromotionReport::SaveRecord()
{
	if (FALSE == m_bUpdatingTabs)
	{
		m_pPromoRecord->SetReportAsPluType(m_nReportAsPluType);
		m_pPromoRecord->SetDiscountAlwaysVATableFlag(IsTicked(m_checkAlwaysTax));

		m_pPromoRecord->SetDeptNoReport(m_DepartmentSelectorReport.GetDepartmentFilter());
		m_pPromoRecord->SetReportAsZeroQtyFlag(m_comboSalesQty.GetCurSel() == 0);

		if (SysInfo.IsSageExportSystem() == TRUE)
		{
			CString str;
			GetEditSage()->GetWindowText(str);
			m_pPromoRecord->SetSageNominalCode(atoi(str));
		}
	}
}

/**********************************************************************/

void CPropPagePromotionReport::Refresh()
{
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/