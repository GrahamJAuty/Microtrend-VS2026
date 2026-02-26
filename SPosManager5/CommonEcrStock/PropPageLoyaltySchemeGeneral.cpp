/**********************************************************************/
#include "PropertySheetLoyaltyScheme.h"
/**********************************************************************/
#include "PropPageLoyaltySchemeGeneral.h"
/**********************************************************************/

CPropPageLoyaltySchemeGeneral::CPropPageLoyaltySchemeGeneral() : CSSPropertyPage(CPropPageLoyaltySchemeGeneral::IDD)
{
	//{{AFX_DATA_INIT(CPropPageLoyaltySchemeGeneral)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPageLoyaltySchemeGeneral::~CPropPageLoyaltySchemeGeneral()
{
}

/**********************************************************************/

void CPropPageLoyaltySchemeGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageLoyaltySchemeGeneral)
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_CHECK_ACTIVE, m_checkActive);
	DDX_Control(pDX, IDC_EDIT_FORMAT, m_editFormat);
	DDX_Control(pDX, IDC_EDIT_LOWER, m_editLower);
	DDX_Control(pDX, IDC_EDIT_UPPER, m_editUpper);
	DDX_Control(pDX, IDC_CHECK_MANUAL, m_checkManual);
	DDX_Control(pDX, IDC_CHECK_STAFFCARD, m_checkStaffCard);
	DDX_Control(pDX, IDC_CHECK_STAFFCREDIT, m_checkStaffCredit);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageLoyaltySchemeGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageLoyaltySchemeGeneral)
	ON_BN_CLICKED( IDC_CHECK_STAFFCARD, OnToggleStaffCard )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageLoyaltySchemeGeneral::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();
	
	m_editName.LimitText( LoyaltyScheme::SchemeName.Max );
	m_editFormat.LimitText( LoyaltyScheme::Format.Max );
	m_editLower.LimitText( LoyaltyScheme::NumberStartLower.Max );
	m_editUpper.LimitText( LoyaltyScheme::NumberStartUpper.Max );
	
	SetRecordControls();

	return TRUE;  
}

/**********************************************************************/

void CPropPageLoyaltySchemeGeneral::OnToggleStaffCard()
{
	m_checkStaffCredit.EnableWindow(IsTicked(m_checkStaffCard));
}

/**********************************************************************/

BOOL CPropPageLoyaltySchemeGeneral::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageLoyaltySchemeGeneral::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageLoyaltySchemeGeneral::GetRecordData()
{
}

/**********************************************************************/

void CPropPageLoyaltySchemeGeneral::SetRecordControls()
{
	m_editName.SetWindowText( m_pSchemeRecord -> GetSchemeName() );
	m_checkActive.SetCheck( m_pSchemeRecord -> GetActiveFlag() );
	m_editFormat.SetWindowText( m_pSchemeRecord -> GetFormat() );
	m_editLower.SetWindowText( m_pSchemeRecord -> GetNumberStartLower() );
	m_editUpper.SetWindowText( m_pSchemeRecord -> GetNumberStartUpper() );
	m_checkManual.SetCheck( m_pSchemeRecord -> GetCheckDigitFlag() );
	m_checkStaffCard.SetCheck( m_pSchemeRecord -> GetStaffCardFlag() );
	m_checkStaffCredit.SetCheck( m_pSchemeRecord -> GetStaffCreditFlag() );
	
	OnToggleStaffCard();
}

/**********************************************************************/

bool CPropPageLoyaltySchemeGeneral::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageLoyaltySchemeGeneral::SaveRecord()
{
	CString strName;
	m_pSchemeRecord -> SetSchemeName( GetEditBoxText( m_editName ) );
	m_pSchemeRecord -> SetActiveFlag( IsTicked( m_checkActive ) );
	m_pSchemeRecord -> SetFormat( GetEditBoxText( m_editFormat ) );
	m_pSchemeRecord -> SetNumberStartLower( GetEditBoxText( m_editLower ) );
	m_pSchemeRecord -> SetNumberStartUpper( GetEditBoxText( m_editUpper ) );
	m_pSchemeRecord -> SetCheckDigitFlag( IsTicked( m_checkManual ) );
	m_pSchemeRecord -> SetStaffCardFlag( IsTicked( m_checkStaffCard ) );
	m_pSchemeRecord -> SetStaffCreditFlag( IsTicked( m_checkStaffCredit ) );
}

/**********************************************************************/

void CPropPageLoyaltySchemeGeneral::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/
