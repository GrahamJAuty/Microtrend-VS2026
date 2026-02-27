//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//$$******************************************************************
#include "EmailAlertPropertyPage1.h"
//$$******************************************************************

CEmailAlertPropertyPage1::CEmailAlertPropertyPage1( CEmailAlertData& data )
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CEmailAlertPropertyPage1::IDD), m_data( data )
{
}

//$$******************************************************************

CEmailAlertPropertyPage1::~CEmailAlertPropertyPage1()
{
}

//$$******************************************************************

void CEmailAlertPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_EMAILCONTACTNO, m_comboEmailContactNo);
	DDX_Control(pDX, IDC_EDIT_EMAILSIGNATURE, m_editEmailSignatureText);
	/*****/
	DDX_Control(pDX, IDC_CHECK_ENABLE, m_checkEnableEmail);
	DDX_Control(pDX, IDC_EDIT_EMAILTEXT, m_editEmailText);
	DDX_Control(pDX, IDC_COMBO_EMAIL, m_comboEmail);
	DDX_Control(pDX, IDC_CHECK_PURSE1, m_checkPurse1);
	DDX_Control(pDX, IDC_CHECK_PURSE2, m_checkPurse2);
	DDX_Control(pDX, IDC_CHECK_PURSE3, m_checkPurse3);
	DDX_Control(pDX, IDC_CHECK_SHOWPURSE1, m_checkShowPurse1);
	DDX_Control(pDX, IDC_CHECK_SHOWPURSE2, m_checkShowPurse2);
	DDX_Control(pDX, IDC_CHECK_SHOWPURSE3, m_checkShowPurse3);
	DDX_Control(pDX, IDC_CHECK_SHOWPOINTS, m_checkShowPoints);
	DDX_Control(pDX, IDC_CHECK_POUNDPURSE1, m_checkPoundPurse1);
	DDX_Control(pDX, IDC_CHECK_POUNDPURSE2, m_checkPoundPurse2);
	DDX_Control(pDX, IDC_CHECK_POUNDPURSE3, m_checkPoundPurse3);
	DDX_Control(pDX, IDC_EDIT_BALANCE1, m_editBalance1);
	DDX_Control(pDX, IDC_EDIT_BALANCE2, m_editBalance2);
	DDX_Control(pDX, IDC_EDIT_BALANCE3, m_editBalance3);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CEmailAlertPropertyPage1, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED( IDC_CHECK_ENABLE, OnToggleEnable )
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CEmailAlertPropertyPage1::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	CSQLRepositoryUserText RepoText;

	m_comboEmailContactNo.AddString ( "Email" );
	for ( int n = 1 ; n <= 8 ; n++ )
	{
		m_comboEmailContactNo.AddString ( RepoText.GetDisplayInfoText(n) );
	}

	/*****/

	m_checkEnableEmail.SetCheck( m_data.m_bEnableLowBalanceAlerts );

	{
		CString strText = "None specific";
		m_comboEmail.AddString ( strText );
		
		for ( int n = 1 ; n <= 4 ; n++ )
		{
			strText = RepoText.GetTickText(n);
			m_comboEmail.AddString ( strText );
		}
		m_comboEmail.SetCurSel ( m_data.m_nLowBalanceTickNo );
	}

	m_editEmailText.SetWindowText( m_data.m_strLowBalanceEmailText );

	m_checkPurse1.SetCheck( m_data.m_bLowBalancePurse1Reqd );
	m_checkPurse2.SetCheck( m_data.m_bLowBalancePurse2Reqd );
	m_checkPurse3.SetCheck( m_data.m_bLowBalancePurse3Reqd );

	m_checkShowPurse1.SetCheck( m_data.m_bLowBalanceShowPurse1 );
	m_checkShowPurse2.SetCheck( m_data.m_bLowBalanceShowPurse2 );
	m_checkShowPurse3.SetCheck( m_data.m_bLowBalanceShowPurse3 );
	m_checkShowPoints.SetCheck( m_data.m_bLowBalanceShowPoints );

	m_checkPoundPurse1.SetCheck( m_data.m_bLowBalancePurse1Symbol );
	m_checkPoundPurse2.SetCheck( m_data.m_bLowBalancePurse2Symbol );
	m_checkPoundPurse3.SetCheck( m_data.m_bLowBalancePurse3Symbol );

	m_editBalance1.LimitText(20);
	m_editBalance1.SetWindowText( m_data.m_strLowBalancePurse1Text );

	m_editBalance2.LimitText(20);
	m_editBalance2.SetWindowText( m_data.m_strLowBalancePurse2Text );

	m_editBalance3.LimitText(20);
	m_editBalance3.SetWindowText( m_data.m_strLowBalancePurse3Text );

	OnToggleEnable();

	return TRUE;
}

//$$******************************************************************

BOOL CEmailAlertPropertyPage1::OnSetActive()
{
	m_comboEmailContactNo.SetCurSel ( m_data.m_nEmailContactNo);
	m_editEmailSignatureText.SetWindowText( m_data.m_strSignatureText1 );
	return CPropertyPage::OnSetActive();
}

//$$******************************************************************

BOOL CEmailAlertPropertyPage1::OnKillActive()
{
	m_data.m_strSignatureText1				= GetEditBoxText( m_editEmailSignatureText );
	m_data.m_nEmailContactNo				= m_comboEmailContactNo.GetCurSel();
	/****/
	m_data.m_bEnableLowBalanceAlerts		= IsTicked( m_checkEnableEmail );
	m_data.m_strLowBalanceEmailText			= GetEditBoxText( m_editEmailText );
	m_data.m_nLowBalanceTickNo				= m_comboEmail.GetCurSel();
	m_data.m_bLowBalancePurse1Reqd			= IsTicked( m_checkPurse1 );
	m_data.m_bLowBalancePurse2Reqd			= IsTicked( m_checkPurse2 );
	m_data.m_bLowBalancePurse3Reqd			= IsTicked( m_checkPurse3 );
	m_data.m_bLowBalanceShowPurse1			= IsTicked( m_checkShowPurse1 );
	m_data.m_bLowBalanceShowPurse2			= IsTicked( m_checkShowPurse2 );
	m_data.m_bLowBalanceShowPurse3			= IsTicked( m_checkShowPurse3 );
	m_data.m_bLowBalanceShowPoints			= IsTicked( m_checkShowPoints );
	m_data.m_bLowBalancePurse1Symbol		= IsTicked( m_checkPoundPurse1 );
	m_data.m_bLowBalancePurse2Symbol		= IsTicked( m_checkPoundPurse2 );
	m_data.m_bLowBalancePurse3Symbol		= IsTicked( m_checkPoundPurse3 );
	m_data.m_strLowBalancePurse1Text		= GetEditBoxText( m_editBalance1 );
	m_data.m_strLowBalancePurse2Text		= GetEditBoxText( m_editBalance2 );
	m_data.m_strLowBalancePurse3Text		= GetEditBoxText( m_editBalance3 );

	{
		int nErrorType = 0;
		CString strMsg = "";

		if ( TRUE == m_data.m_bEnableLowBalanceAlerts )				
		{
			if ( ( FALSE == m_data.m_bLowBalancePurse1Reqd ) && ( FALSE == m_data.m_bLowBalancePurse2Reqd ) && ( FALSE == m_data.m_bLowBalancePurse3Reqd ) )
			{
				strMsg = "No Low Balance purses have been set to validate on!";
				nErrorType = 1;
			}
		
			if ( 0 == nErrorType )
			{
				strMsg = m_data.CheckIndicatorConflict(0);

				if ( strMsg != "" )
				{
					nErrorType = 2;
				}
			}
		}

		if ( nErrorType != 0 )
		{
			Prompter.Error( strMsg, "Entry Error" );

			switch( nErrorType )
			{
			case 1:	m_checkPurse1.SetFocus();	break;
			case 2:	m_comboEmail.SetFocus();	break;
			}

			return FALSE;
		}
	}

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************

void CEmailAlertPropertyPage1::OnToggleEnable()
{
	bool bEnable = IsTicked( m_checkEnableEmail );
	
	m_editEmailText.EnableWindow( bEnable );
	m_comboEmail.EnableWindow( bEnable );
	m_checkPurse1.EnableWindow( bEnable );
	m_checkPurse2.EnableWindow( bEnable );
	m_checkPurse3.EnableWindow( bEnable );
	m_checkShowPurse1.EnableWindow( bEnable );
	m_checkShowPurse2.EnableWindow( bEnable );
	m_checkShowPurse3.EnableWindow( bEnable );
	m_checkShowPoints.EnableWindow( bEnable );
	m_checkPoundPurse1.EnableWindow( bEnable );
	m_checkPoundPurse2.EnableWindow( bEnable );
	m_checkPoundPurse3.EnableWindow( bEnable );
	m_editBalance1.EnableWindow( bEnable );
	m_editBalance2.EnableWindow( bEnable );
	m_editBalance3.EnableWindow( bEnable );
}

//$$******************************************************************
