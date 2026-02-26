/**********************************************************************/
#include "MaxLengths.h"
/**********************************************************************/
#include "PropertySheetPromotion.h"
#include "PropPagePromotionProgram.h"
/**********************************************************************/

CPropPagePromotionProgram::CPropPagePromotionProgram() : CSSPropertyPage(CPropPagePromotionProgram::IDD),
	m_DepartmentSelectorEpos( m_editDeptNoEpos, m_comboDeptNameEpos )
{
	//{{AFX_DATA_INIT(CPropPagePromotionProgram)
	//}}AFX_DATA_INIT
	m_pPromoRecord = NULL;
	m_pPropertySheet = NULL;
	m_dPercent = 0.0;
	m_dValue = 0.0;
	m_dTrip = 0.0;
	m_nUseGate = 0;
	m_bUpdatingTabs = FALSE;
	m_bSubtotalOnly = FALSE;
}

/**********************************************************************/

CPropPagePromotionProgram::~CPropPagePromotionProgram()
{
}

/**********************************************************************/

void CPropPagePromotionProgram::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePromotionProgram)
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_editDescription);
	DDX_Control(pDX, IDC_COMBO_PROMO_TYPE, m_comboPromoType);
	DDX_Control(pDX, IDC_COMBO_VALUE_TYPE, m_comboValueType);
	DDX_Control(pDX, IDC_COMBO_USAGE, m_comboUsage);
	DDX_Control(pDX, IDC_STATIC_VALUE, m_staticValue);
	DDX_Control(pDX, IDC_STATIC_PERCENT, m_staticPercent);
	DDX_Control(pDX, IDC_STATIC_TRIP, m_staticTrip);
	DDX_Control(pDX, IDC_EDIT_DEPTNO_EPOS, m_editDeptNoEpos);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME_EPOS, m_comboDeptNameEpos);
	DDX_Control(pDX, IDC_COMBO_TIMEPLAN, m_comboTimeplan);
	DDX_Control(pDX, IDC_CHECK_GRATUITY, m_checkGratuity);
	DDX_Control(pDX, IDC_CHECK_TAX, m_checkTax);
	DDX_Control(pDX, IDC_CHECK_MYPROMOONPROMO, m_checkMyPromoOnPromo);
	DDX_Control(pDX, IDC_CHECK_PROMOONMYPROMO, m_checkPromoOnMyPromo);
	DDX_Control(pDX, IDC_CHECK_REASONS, m_checkReasons);
	DDX_Control(pDX, IDC_CHECK_SCVATFIX, m_checkServiceChargeVATFix);
	DDX_Control(pDX, IDC_CHECK_UPTO, m_checkUpTo);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_strDescription);
	DDX_Text(pDX, IDC_EDIT_PERCENT, m_dPercent);
	DDV_MinMaxDouble(pDX, m_dPercent, Promotions::Percent.Min, Promotions::Percent.Max);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_dValue);
	DDV_MinMaxDouble(pDX, m_dValue, Promotions::Markdown.Min, Promotions::Markdown.Max);
	DDX_Text(pDX, IDC_EDIT_TRIP, m_dTrip);
	DDV_MinMaxDouble(pDX, m_dTrip, Promotions::Trip.Min, Promotions::Trip.Max);
	DDX_Text(pDX, IDC_EDIT_USEGATE, m_nUseGate);
	DDV_MinMaxInt(pDX, m_nUseGate, Promotions::UseGate.Min,Promotions::UseGate.Max);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePromotionProgram, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePromotionProgram)
	ON_CBN_SELCHANGE(IDC_COMBO_PROMO_TYPE, OnSelectPromoType)
	ON_CBN_SELCHANGE(IDC_COMBO_VALUE_TYPE, OnSelectPromoType)
	ON_CBN_SELCHANGE(IDC_COMBO_USAGE, OnSelectPromoType)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO_EPOS, OnKillFocusDeptNoEpos)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME_EPOS, OnSelectDepartmentEpos)
	ON_BN_CLICKED(IDC_CHECK_SCVATFIX,OnToggleSCVATFix)
	ON_BN_CLICKED(IDC_CHECK_UPTO, OnToggleUpTo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPropPagePromotionProgram::GetEditValue() { return GetEdit( IDC_EDIT_VALUE ); }
CEdit* CPropPagePromotionProgram::GetEditPercent() { return GetEdit( IDC_EDIT_PERCENT ); }
CEdit* CPropPagePromotionProgram::GetEditTrip() { return GetEdit( IDC_EDIT_TRIP ); }
CEdit* CPropPagePromotionProgram::GetEditUseGate() { return GetEdit( IDC_EDIT_USEGATE ); }
/**********************************************************************/

BOOL CPropPagePromotionProgram::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_checkTax.SetWindowText( EcrmanOptions.GetTaxString( "SPOS Apply %T" ) );

	SubclassEdit ( IDC_EDIT_VALUE, SS_NUM_DP, 9, "%.2f" );
	SubclassEdit ( IDC_EDIT_TRIP, SS_NUM_DP, 9, "%.2f" );
	SubclassEdit ( IDC_EDIT_PERCENT, SS_NUM_SDP, 7, "%.2f" );
	SubclassEdit ( IDC_EDIT_USEGATE, SS_NUM, 3, "%d" );

	m_comboPromoType.AddString( "Discount" );
	m_comboPromoType.AddString( "Surcharge" );
	m_comboPromoType.AddString( "Markdown" );

	m_comboValueType.AddString( "Fixed" );
	m_comboValueType.AddString( "By Value" );
	m_comboValueType.AddString( "By Covers" );

	m_comboTimeplan.AddString( "Override prompt" );
	m_comboTimeplan.AddString( "Silent reject" );
	m_comboTimeplan.AddString( "Warn & reject" );

	m_editDescription.LimitText( Promotions::Description.Max );
	
	m_editDeptNoEpos.LimitText ( MAX_LENGTH_DEPTNO );

	MoveControl( &m_staticPercent, 19, 114 );
	MoveControl( GetEditPercent(), 105, 113 );

	{
		CString strText = "";
		strText.Format("Lock service charge to %s A",
			EcrmanOptions.GetTaxBandString());

		m_checkServiceChargeVATFix.SetWindowText(strText);
	}

	return TRUE;  
}

/**********************************************************************/

BOOL CPropPagePromotionProgram::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePromotionProgram::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPagePromotionProgram::GetRecordData()
{
	m_strDescription = m_pPromoRecord -> GetDisplayName();
}

/**********************************************************************/

void CPropPagePromotionProgram::OnToggleSCVATFix()
{
	if (IsTicked(m_checkServiceChargeVATFix) == FALSE)
	{
		m_checkTax.EnableWindow(TRUE);
		m_comboPromoType.EnableWindow(TRUE);
		m_comboUsage.EnableWindow(TRUE);
	}
	m_pPromoRecord->ClrServiceChargeVATFixBeforeEditing();
}

/**********************************************************************/

void CPropPagePromotionProgram::OnSelectPromoType() 
{
	int nPromoType = m_comboPromoType.GetCurSel();
	int nUsage = m_comboUsage.GetCurSel();

	bool bShowPercent = FALSE;
	bool bShowValue = FALSE;

	switch( nPromoType )
	{
	case 2:		
		bShowValue = TRUE;
		break;

	case 0:
	case 1:
	default:	
		bShowPercent = TRUE;	
		break;
	}

	bool bEnableValueType = TRUE;
	bEnableValueType &= ( nPromoType != 2 );
	bEnableValueType &= ( ( 1 == nUsage ) || ( 3 == nUsage ) );

	if ( FALSE == bEnableValueType )
	{
		m_comboValueType.SetCurSel(0);
		m_comboValueType.EnableWindow(FALSE);
	}
	else
	{
		m_comboValueType.EnableWindow(TRUE);
	}

	if ( m_comboValueType.GetCurSel() != 0 )
	{
		bShowValue = FALSE;
		bShowPercent = FALSE;
	}

	ShowAndEnableWindow( &m_staticValue, bShowValue );
	ShowAndEnableWindow( GetEditValue(), bShowValue );
	ShowAndEnableWindow( &m_staticPercent, bShowPercent );
	ShowAndEnableWindow( GetEditPercent(), bShowPercent );
	ShowAndEnableWindow(&m_checkUpTo, 2 == nPromoType);
	OnToggleUpTo();
	
	if ( ( FALSE == bShowValue ) && ( FALSE == bShowPercent ) )
	{
		MoveControl( &m_staticTrip, 19, 114 );
		MoveControl( GetEditTrip(), 105, 113 );
	}
	else
	{
		MoveControl( &m_staticTrip, 19, 134 );
		MoveControl( GetEditTrip(), 105, 133 );
	}
	m_pPromoRecord -> SetFlexType( m_comboValueType.GetCurSel() );
	( (CPropertySheetPromotion*) m_pPropertySheet ) -> AddFlexTabs();
}

/**********************************************************************/

void CPropPagePromotionProgram::OnKillFocusDeptNoEpos() 
{
	m_DepartmentSelectorEpos.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CPropPagePromotionProgram::OnSelectDepartmentEpos() 
{
	m_DepartmentSelectorEpos.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CPropPagePromotionProgram::OnToggleUpTo()
{
	bool bEnableUsage = TRUE;
	if (FALSE == m_bSubtotalOnly)
	{
		if (2 == m_comboPromoType.GetCurSel())
		{
			if (IsTicked(m_checkUpTo))
			{
				bEnableUsage = FALSE;
			}
		}
	}

	if (FALSE == bEnableUsage)
	{
		m_comboUsage.SetCurSel(3);
		m_comboUsage.EnableWindow(FALSE);
	}
	else
	{
		m_comboUsage.EnableWindow(TRUE);
	}
}

/**********************************************************************/

void CPropPagePromotionProgram::SetRecordControls()
{
	m_pPromoRecord->ValidateUpToFlag();

	{
		int nType = m_pPromoRecord->GetPromoType();

		if ((nType >= 0) && (nType <= 2))
		{
			m_comboPromoType.SetCurSel(nType);
		}
		else
		{
			m_comboPromoType.SetCurSel(0);
		}

		ShowAndEnableWindow(&m_checkUpTo, (2 == nType));
		m_checkUpTo.SetCheck(m_pPromoRecord->GetUpToFlag());
	}

	{
		int nType = m_pPromoRecord->GetFlexType();
		if ((nType >= 0) && (nType <= 2))
		{
			m_comboValueType.SetCurSel(nType);
		}
		else
		{
			m_comboValueType.SetCurSel(0);
		}
	}

	int nMode = m_pPromoRecord -> GetTimeplanMode();
	if ((nMode >= 0) && (nMode <= 2))
	{
		m_comboTimeplan.SetCurSel(nMode);
	}
	else
	{
		m_comboTimeplan.SetCurSel(0);
	}

	CString strNumber = "";
	strNumber.Format( "%.2f", m_pPromoRecord -> GetMarkdown() );
	GetEditValue() -> SetWindowText( strNumber );

	strNumber.Format( "%.2f", m_pPromoRecord -> GetPercent() );
	GetEditPercent() -> SetWindowText( strNumber );

	strNumber.Format( "%.2f", m_pPromoRecord -> GetTrip() );
	GetEditTrip() -> SetWindowText( strNumber );

	strNumber.Format( "%d", m_pPromoRecord -> GetUseGate() );
	GetEditUseGate() -> SetWindowText( strNumber );

	m_checkGratuity.SetCheck( m_pPromoRecord -> GetGratuityFlag() );
	m_checkTax.SetCheck( m_pPromoRecord -> GetSPOSApplyTaxFlag() );
	m_checkMyPromoOnPromo.SetCheck( m_pPromoRecord -> GetMyPromoOnPromoFlag() );
	m_checkPromoOnMyPromo.SetCheck( m_pPromoRecord -> GetPromoOnMyPromoFlag() );
	m_checkReasons.SetCheck( m_pPromoRecord -> GetReasonsFlag() );
	m_checkServiceChargeVATFix.SetCheck(m_pPromoRecord->GetServiceChargeVATFixFlag());

	m_comboUsage.ResetContent();
	
	m_comboUsage.AddString("Apply to item");
	m_comboUsage.AddString("Apply as subtotal");
	m_comboUsage.AddString("Item or subtotal");
	m_comboUsage.AddString("Deferred subtotal only");
	
	m_comboUsage.SetItemData(0, 1);
	m_comboUsage.SetItemData(1, 2);
	m_comboUsage.SetItemData(2, 3);
	m_comboUsage.SetItemData(3, 6);
	
	if ( TRUE == m_bSubtotalOnly )
	{
		m_comboUsage.DeleteString(3);
		m_comboUsage.DeleteString(0);
	}
	
	if (m_comboUsage.SelectUsingItemData(m_pPromoRecord->GetUsage(), -1) == FALSE)
	{
		CString str = "";
		str.Format("Unknown (%d)", m_pPromoRecord->GetUsage());
		m_comboUsage.AddString(str);
		m_comboUsage.SetCurSel(m_comboUsage.GetCount() - 1);
	}

	OnToggleUpTo();

	m_DepartmentSelectorEpos.FillDepartmentComboDatabase( m_pPromoRecord -> GetDeptNoEpos(), DEPARTMENT_SELECTOR_SINGLE );
	
	OnSelectPromoType();
	
	m_editDescription.SetSel ( 0, -1 );
	m_editDescription.SetFocus();

	if (m_pPromoRecord->GetServiceChargeVATFixBeforeEditing() == TRUE)
	{
		if (m_pPromoRecord->GetServiceChargeVATFixFlag() == TRUE)
		{
			m_checkTax.SetCheck(TRUE);
			m_checkTax.EnableWindow(FALSE);

			m_comboPromoType.SetCurSel(1);
			m_comboPromoType.EnableWindow(FALSE);

			m_comboUsage.SelectUsingItemData(2);
			m_comboUsage.EnableWindow(FALSE);
		}
		else
		{
			m_checkTax.EnableWindow(TRUE);
			m_comboPromoType.EnableWindow(TRUE);
			m_comboUsage.EnableWindow(TRUE);
		}
	}
}

/**********************************************************************/

bool CPropPagePromotionProgram::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPagePromotionProgram::SaveRecord()
{
	if (FALSE == m_bUpdatingTabs)
	{
		OnKillFocusDeptNoEpos();
		m_pPromoRecord->SetDescription(m_strDescription);

		m_pPromoRecord->SetPromoType(m_comboPromoType.GetCurSel());
		m_pPromoRecord->SetFlexType(m_comboValueType.GetCurSel());
		m_pPromoRecord->SetMarkdown(m_dValue);
		m_pPromoRecord->SetPercent(m_dPercent);
		m_pPromoRecord->SetTrip(m_dTrip);
		m_pPromoRecord->SetSPOSApplyTaxFlag(IsTicked(m_checkTax));
		m_pPromoRecord->SetGratuityFlag(IsTicked(m_checkGratuity));
		m_pPromoRecord->SetMyPromoOnPromoFlag(IsTicked(m_checkMyPromoOnPromo));
		m_pPromoRecord->SetPromoOnMyPromoFlag(IsTicked(m_checkPromoOnMyPromo));
		m_pPromoRecord->SetReasonsFlag(IsTicked(m_checkReasons));
		m_pPromoRecord->SetServiceChargeVATFixFlag(IsTicked(m_checkServiceChargeVATFix));
		m_pPromoRecord->SetUpToFlag(IsTicked(m_checkUpTo));

		if (FALSE == m_bSubtotalOnly)
		{
			switch (m_comboUsage.GetCurSel())
			{
			case 0:	
				m_pPromoRecord->SetUsage(1);
				break;

			case 1:	
				m_pPromoRecord->SetUsage(2);
				break;

			case 2:	
				m_pPromoRecord->SetUsage(3);
				break;

			case 3:	
				m_pPromoRecord->SetUsage(6);
				break;
			}
		}
		else
		{
			switch (m_comboUsage.GetCurSel())
			{
			case 0:	
				m_pPromoRecord->SetUsage(2);
				break;

			case 1:	
				m_pPromoRecord->SetUsage(3);
				break;
			}
		}

		m_pPromoRecord->SetDeptNoEpos(m_DepartmentSelectorEpos.GetDepartmentFilter());
		m_pPromoRecord->SetTimeplanMode(m_comboTimeplan.GetCurSel());
		m_pPromoRecord->SetUseGate(m_nUseGate);

		m_pPromoRecord->ValidateUpToFlag();
		m_pPromoRecord->ValidateFlexSettings();
	}
}

/**********************************************************************/

void CPropPagePromotionProgram::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/