//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//$$******************************************************************
#include "StandardCombos.h"
//$$******************************************************************
#include "EmailAlertPropertyPage4.h"
//$$******************************************************************

CEmailAlertPropertyPage4::CEmailAlertPropertyPage4( CEmailAlertData& data )
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CEmailAlertPropertyPage4::IDD), m_data( data )
{
	m_bEnableStartDate = FALSE;
}

//$$******************************************************************

CEmailAlertPropertyPage4::~CEmailAlertPropertyPage4()
{
}

//$$******************************************************************

void CEmailAlertPropertyPage4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_EMAILCONTACTNO, m_comboEmailContactNo);
	DDX_Control(pDX, IDC_EDIT_EMAILSIGNATURE, m_editEmailSignatureText);
	/*****/
	DDX_Control(pDX, IDC_CHECK_ENABLE, m_checkEnableEmail);
	DDX_Control(pDX, IDC_EDIT_EMAILTEXT, m_editEmailText);
	DDX_Control(pDX, IDC_COMBO_EMAIL, m_comboEmail);
	DDX_Control(pDX, IDC_COMBO_STARTOFWEEK, m_comboStartOfWeek);
	DDX_Control(pDX, IDC_CHECK_EXCLUDEZERO, m_checkExcludeZeroPrice);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DateTimePicker);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CEmailAlertPropertyPage4, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED( IDC_CHECK_ENABLE, OnToggleEnable )
	ON_STN_CLICKED(IDC_STATIC_PURCHASESTARTDATE, OnStaticStartDate)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CEmailAlertPropertyPage4::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	CSQLRepositoryUserText RepoText;

	m_comboEmailContactNo.AddString("Email");
	for (int n = 1; n <= 8; n++)
	{
		m_comboEmailContactNo.AddString(RepoText.GetDisplayInfoText(n));
	}

	m_checkEnableEmail.SetCheck(m_data.m_bEnablePurchaseAlerts);

	{
		CString strText = "None specific";
		m_comboEmail.AddString(strText);

		for (int n = 1; n <= 4; n++)
		{
			strText = RepoText.GetTickText(n);
			m_comboEmail.AddString(strText);
		}
		m_comboEmail.SetCurSel(m_data.m_nPurchaseTickNo);
	}

	m_editEmailText.SetWindowText(m_data.m_strPurchaseEmailText);

	CStandardCombos::FillWeekdayCombo(m_comboStartOfWeek, m_data.m_nPurchaseDay);
	m_checkExcludeZeroPrice.SetCheck(m_data.m_bPurchaseExcludeZeroPriced);

	{
		CString strDate = m_data.m_strPurchaseStartFrom;

		if (strDate.GetLength() == 10)
		{
			COleDateTime date = COleDateTime(
				atoi(strDate.Right(4)),
				atoi(strDate.Mid(3, 2)),
				atoi(strDate.Left(2)),
				0, 0, 0);

			if (date.m_status == COleDateTime::valid)
			{
				m_DateTimePicker.SetTime(date);
			}
		}
	}

	m_DateTimePicker.EnableWindow(FALSE);

	OnToggleEnable();

	return TRUE;
}

//$$******************************************************************

BOOL CEmailAlertPropertyPage4::OnSetActive()
{
	m_comboEmailContactNo.SetCurSel ( m_data.m_nEmailContactNo);
	m_editEmailSignatureText.SetWindowText( m_data.m_strSignatureText1 );
	return CPropertyPage::OnSetActive();
}

//$$******************************************************************

BOOL CEmailAlertPropertyPage4::OnKillActive()
{
	m_data.m_strSignatureText1				= GetEditBoxText( m_editEmailSignatureText );
	m_data.m_nEmailContactNo				= m_comboEmailContactNo.GetCurSel();
	/*****/
	m_data.m_bEnablePurchaseAlerts			= IsTicked( m_checkEnableEmail );
	m_data.m_strPurchaseEmailText			= GetEditBoxText( m_editEmailText );
	m_data.m_nPurchaseTickNo				= m_comboEmail.GetCurSel();
	m_data.m_nPurchaseDay					= m_comboStartOfWeek.GetCurSel();
	m_data.m_bPurchaseExcludeZeroPriced		= IsTicked( m_checkExcludeZeroPrice ); 
	
	m_DateTimePicker.GetWindowText( m_data.m_strPurchaseStartFrom );

	{
		CString strMsg = m_data.CheckIndicatorConflict(3);

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

void CEmailAlertPropertyPage4::OnToggleEnable()
{
	bool bEnable = IsTicked( m_checkEnableEmail );
	
	m_editEmailText.EnableWindow( bEnable );
	m_comboEmail.EnableWindow( bEnable );
	m_comboStartOfWeek.EnableWindow( bEnable );
	m_checkExcludeZeroPrice.EnableWindow( bEnable );
	m_DateTimePicker.EnableWindow( bEnable & m_bEnableStartDate );
}

//$$******************************************************************

void CEmailAlertPropertyPage4::OnStaticStartDate()
{
	if ( IsTicked( m_checkEnableEmail ) )
	{
		m_bEnableStartDate = TRUE;
		m_DateTimePicker.EnableWindow( TRUE );
	}
}

//$$******************************************************************
