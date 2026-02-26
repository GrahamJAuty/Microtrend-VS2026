/**********************************************************************/
#include "PropPageLoyaltySchemeStaffLimits.h"
/**********************************************************************/

CPropPageLoyaltySchemeStaffLimits::CPropPageLoyaltySchemeStaffLimits() : CSSPropertyPage(CPropPageLoyaltySchemeStaffLimits::IDD)
{
	//{{AFX_DATA_INIT(CPropPageLoyaltySchemeStaffLimits)
	//}}AFX_DATA_INIT
	m_wIDMonth[0] = IDC_EDIT_MONTH1;
	m_wIDMonth[1] = IDC_EDIT_MONTH2;
	m_wIDMonth[2] = IDC_EDIT_MONTH3;
	m_wIDMonth[3] = IDC_EDIT_MONTH4;
	m_wIDMonth[4] = IDC_EDIT_MONTH5;
	m_wIDMonth[5] = IDC_EDIT_MONTH6;
	m_wIDMonth[6] = IDC_EDIT_MONTH7;
	m_wIDMonth[7] = IDC_EDIT_MONTH8;
	m_wIDMonth[8] = IDC_EDIT_MONTH9;
	m_wIDMonth[9] = IDC_EDIT_MONTH10;
	m_wIDMonth[10] = IDC_EDIT_MONTH11;
	m_wIDMonth[11] = IDC_EDIT_MONTH12;
}

/**********************************************************************/

CPropPageLoyaltySchemeStaffLimits::~CPropPageLoyaltySchemeStaffLimits()
{
}

/**********************************************************************/

void CPropPageLoyaltySchemeStaffLimits::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageLoyaltySchemeStaffLimits)
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_CHECK_ACTIVE, m_checkActive);
	DDX_Control(pDX, IDC_BUTTON_COPY, m_buttonCopy);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageLoyaltySchemeStaffLimits, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageLoyaltySchemeStaffLimits)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_COPY, OnCopyJan)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageLoyaltySchemeStaffLimits::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();
	
	m_editName.LimitText( LoyaltyScheme::SchemeName.Max );
	
	SubclassEdit( IDC_EDIT_DAY, SS_NUM, 5, "%d" );
	SubclassEdit( IDC_EDIT_WEEK, SS_NUM, 5, "%d" );

	for ( int nMth = 0; nMth < 12; nMth++ )
	{
		SubclassEdit( m_wIDMonth[nMth], SS_NUM, 5, "%d" );
	}
	
	SetRecordControls();

	return TRUE;  
}

/**********************************************************************/
CEdit* CPropPageLoyaltySchemeStaffLimits::GetEditDay(){ return GetEdit( IDC_EDIT_DAY ); }
CEdit* CPropPageLoyaltySchemeStaffLimits::GetEditWeek(){ return GetEdit( IDC_EDIT_WEEK ); }
/**********************************************************************/

CEdit* CPropPageLoyaltySchemeStaffLimits::GetEditMonth( int nMth )
{
	if ( ( nMth >= 0 ) && ( nMth < 12 ) )
		return GetEdit( m_wIDMonth[nMth] );
	else
		return NULL;
}

/**********************************************************************/

BOOL CPropPageLoyaltySchemeStaffLimits::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageLoyaltySchemeStaffLimits::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageLoyaltySchemeStaffLimits::GetRecordData()
{
}

/**********************************************************************/

void CPropPageLoyaltySchemeStaffLimits::SetRecordControls()
{
	bool bEnable = m_pSchemeRecord -> GetStaffCardFlag();
	
	GetEditDay() -> EnableWindow( bEnable );
	GetEditWeek() -> EnableWindow( bEnable );

	for ( int nMth = 0; nMth < 12; nMth++ )
	{
		GetEditMonth( nMth ) -> EnableWindow( bEnable );
	}

	m_buttonCopy.EnableWindow( bEnable );

	/*****/

	m_editName.SetWindowText( m_pSchemeRecord -> GetSchemeName() );
	m_checkActive.SetCheck( m_pSchemeRecord -> GetActiveFlag() );

	SetEditBoxInt( *GetEditDay(), m_pSchemeRecord -> GetStaffLimitDay() );
	SetEditBoxInt( *GetEditWeek(), m_pSchemeRecord -> GetStaffLimitWeek() );

	for ( int nMth = 0; nMth < 12; nMth++ )
	{
		SetEditBoxInt( *GetEditMonth(nMth), m_pSchemeRecord -> GetStaffLimitMonth(nMth) );
	}
}

/**********************************************************************/

bool CPropPageLoyaltySchemeStaffLimits::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageLoyaltySchemeStaffLimits::OnCopyJan()
{
	if ( Prompter.YesNo( "Copy January Limit to all other months" ) == IDYES )
	{
		int nLimit = GetEditBoxInt( *GetEditMonth(0) );

		for ( int nMth = 1; nMth < 12; nMth++ )
		{
			SetEditBoxInt( *GetEditMonth(nMth), nLimit );
		}
	}
}

/**********************************************************************/

void CPropPageLoyaltySchemeStaffLimits::SaveRecord()
{
	CString strName;
	m_pSchemeRecord -> SetSchemeName( GetEditBoxText( m_editName ) );
	m_pSchemeRecord -> SetActiveFlag( IsTicked( m_checkActive ) );

	if ( m_pSchemeRecord -> GetStaffCardFlag() == TRUE )
	{
		m_pSchemeRecord -> SetStaffLimitDay( GetEditBoxInt( *GetEditDay() ) );
		m_pSchemeRecord -> SetStaffLimitWeek( GetEditBoxInt( *GetEditWeek() ) );

		for ( int nMth = 0; nMth < 12; nMth++ )
		{
			m_pSchemeRecord -> SetStaffLimitMonth( nMth, GetEditBoxInt( *GetEditMonth( nMth ) ) );
		}
	}
}

/**********************************************************************/

void CPropPageLoyaltySchemeStaffLimits::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/
