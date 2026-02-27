//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//$$******************************************************************
#include "StandardCombos.h"
//$$******************************************************************
#include "EmailAlertPropertyPage3.h"
//$$******************************************************************

CEmailAlertPropertyPage3::CEmailAlertPropertyPage3( CEmailAlertData& data )
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CEmailAlertPropertyPage3::IDD), m_data( data )
{
}

//$$******************************************************************

CEmailAlertPropertyPage3::~CEmailAlertPropertyPage3()
{
}

//$$******************************************************************

void CEmailAlertPropertyPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_EMAILCONTACTNO, m_comboEmailContactNo);
	DDX_Control(pDX, IDC_EDIT_EMAILSIGNATURE, m_editEmailSignatureText);
	/*****/
	DDX_Control(pDX, IDC_CHECK_ENABLE, m_checkEnableEmail);
	DDX_Control(pDX, IDC_EDIT_EMAILTEXT, m_editEmailText);
	DDX_Control(pDX, IDC_COMBO_EMAIL, m_comboEmail);
	DDX_Control(pDX, IDC_COMBO_STARTOFWEEK, m_comboStartOfWeek);
	DDX_Control(pDX, IDC_CHECK_EXCLUDENOCHANGE, m_checkExcludeNoChange);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CEmailAlertPropertyPage3, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED( IDC_CHECK_ENABLE, OnToggleEnable )
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CEmailAlertPropertyPage3::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	CSQLRepositoryUserText RepoText;

	m_comboEmailContactNo.AddString("Email");
	for (int n = 1; n <= 8; n++)
	{
		m_comboEmailContactNo.AddString(RepoText.GetDisplayInfoText(n));
	}

	/*****/

	m_checkEnableEmail.SetCheck(m_data.m_bEnableWeeklyPoints);

	{
		CString strText = "None specific";
		m_comboEmail.AddString(strText);

		for (int n = 1; n <= 4; n++)
		{
			strText = RepoText.GetTickText(n);
			m_comboEmail.AddString(strText);
		}
		m_comboEmail.SetCurSel(m_data.m_nWeeklyPointsTickNo);
	}

	m_editEmailText.SetWindowText(m_data.m_strWeeklyPointsEmailText);

	CStandardCombos::FillWeekdayCombo(m_comboStartOfWeek, m_data.m_nWeeklyPointsDay);
	m_checkExcludeNoChange.SetCheck(m_data.m_bWeeklyPointsExcludeNoChange);

	OnToggleEnable();

	return TRUE;
}

//$$******************************************************************

BOOL CEmailAlertPropertyPage3::OnSetActive()
{
	m_comboEmailContactNo.SetCurSel ( m_data.m_nEmailContactNo);
	m_editEmailSignatureText.SetWindowText( m_data.m_strSignatureText1 );
	return CPropertyPage::OnSetActive();
}

//$$******************************************************************

BOOL CEmailAlertPropertyPage3::OnKillActive()
{
	m_data.m_strSignatureText1				= GetEditBoxText( m_editEmailSignatureText );
	m_data.m_nEmailContactNo				= m_comboEmailContactNo.GetCurSel();
	/*****/
	m_data.m_bEnableWeeklyPoints			= IsTicked( m_checkEnableEmail );
	m_data.m_strWeeklyPointsEmailText		= GetEditBoxText( m_editEmailText );
	m_data.m_nWeeklyPointsTickNo			= m_comboEmail.GetCurSel();
	m_data.m_nWeeklyPointsDay				= m_comboStartOfWeek.GetCurSel();
	m_data.m_bWeeklyPointsExcludeNoChange	= IsTicked( m_checkExcludeNoChange );

	{
		CString strMsg = m_data.CheckIndicatorConflict(2);

		if ( strMsg != "" )
		{
			Prompter.Error( strMsg, "Entry Error" );
			m_comboEmail.SetFocus();
			return FALSE;
		}
	}

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************

void CEmailAlertPropertyPage3::OnToggleEnable()
{
	bool bEnable = IsTicked( m_checkEnableEmail );
	
	m_editEmailText.EnableWindow( bEnable );
	m_comboEmail.EnableWindow( bEnable );
	m_comboStartOfWeek.EnableWindow( bEnable );
	m_checkExcludeNoChange.EnableWindow( bEnable );
}

//$$******************************************************************
