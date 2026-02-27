//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//$$******************************************************************
#include "EmailAlertPropertyPage2.h"
//$$******************************************************************

CEmailAlertPropertyPage2::CEmailAlertPropertyPage2( CEmailAlertData& data )
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CEmailAlertPropertyPage2::IDD), m_data( data )
{
}

//$$******************************************************************

CEmailAlertPropertyPage2::~CEmailAlertPropertyPage2()
{
}

//$$******************************************************************

void CEmailAlertPropertyPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_EMAILCONTACTNO, m_comboEmailContactNo);
	DDX_Control(pDX, IDC_EDIT_EMAILSIGNATURE, m_editEmailSignatureText);
	/*****/
	DDX_Control(pDX, IDC_CHECK_ENABLE, m_checkEnableEmail);
	DDX_Control(pDX, IDC_EDIT_EMAILTEXT, m_editEmailText);
	DDX_Control(pDX, IDC_COMBO_EMAIL, m_comboEmail);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CEmailAlertPropertyPage2, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED( IDC_CHECK_ENABLE, OnToggleEnable )
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CEmailAlertPropertyPage2::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	CSQLRepositoryUserText RepoText;

	m_comboEmailContactNo.AddString("Email");
	for (int n = 1; n <= 8; n++)
	{
		m_comboEmailContactNo.AddString(RepoText.GetDisplayInfoText(n));
	}

	/*****/

	m_checkEnableEmail.SetCheck(m_data.m_bEnablePointsAchievement);

	{
		CString strText = "None specific";
		m_comboEmail.AddString(strText);

		for (int n = 1; n <= 4; n++)
		{
			strText = RepoText.GetTickText(n);
			m_comboEmail.AddString(strText);
		}
		m_comboEmail.SetCurSel(m_data.m_nPointsAchievementTickNo);
	}

	m_editEmailText.SetWindowText(m_data.m_strPointsAchievementEmailText);

	OnToggleEnable();

	return TRUE;
}

//$$******************************************************************

BOOL CEmailAlertPropertyPage2::OnSetActive()
{
	m_comboEmailContactNo.SetCurSel ( m_data.m_nEmailContactNo);
	m_editEmailSignatureText.SetWindowText( m_data.m_strSignatureText1 );
	return CPropertyPage::OnSetActive();
}

//$$******************************************************************

BOOL CEmailAlertPropertyPage2::OnKillActive()
{
	m_data.m_strSignatureText1				= GetEditBoxText( m_editEmailSignatureText );
	m_data.m_nEmailContactNo				= m_comboEmailContactNo.GetCurSel();
	/*****/
	m_data.m_bEnablePointsAchievement		= IsTicked( m_checkEnableEmail );
	m_data.m_strPointsAchievementEmailText	= GetEditBoxText( m_editEmailText );
	m_data.m_nPointsAchievementTickNo		= m_comboEmail.GetCurSel();

	{
		CString strMsg = m_data.CheckIndicatorConflict(1);

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

void CEmailAlertPropertyPage2::OnToggleEnable()
{
	bool bEnable = IsTicked( m_checkEnableEmail );
	
	m_editEmailText.EnableWindow( bEnable );
	m_comboEmail.EnableWindow( bEnable );
}

//$$******************************************************************
