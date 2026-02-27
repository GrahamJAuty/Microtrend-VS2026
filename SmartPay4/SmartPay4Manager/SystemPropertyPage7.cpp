//$$******************************************************************
#include "DatabaseImportDlg.h"
#include "SetupBromComDlg.h"
#include "SetupCsvImportPropertySheet.h"
#include "SetupParentMailDlg.h"
#include "SetupParentPayDlg.h"
#include "SetupPebbleDlg.h"
#include "SetupSchoolcommsDlg.h"
#include "SetupSimsDlg.h"
#include "SetupSquidDlg.h"
#include "SetupTucasiDlg.h"
#include "SetupTucasiJsonDlg.h"
#include "SetupWebPaymentOptionsDlg.h"
#include "SetupWisepayDlg.h"
#include "SetupWondeDlg.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "SystemPropertyPage7.h"
//$$******************************************************************

CSystemPropertyPage7::CSystemPropertyPage7()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CSystemPropertyPage7::IDD)
{
	m_bRestartServerReqd = FALSE;
	m_nCurrentWebPayment = 0;
}

//$$******************************************************************

void CSystemPropertyPage7::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_WEBPAYMENT, m_comboWebPayment);
	DDX_Control(pDX, IDC_BUTTON_SETUP_WEBPAYMENT, m_buttonSetupWebPayment);
	DDX_Control(pDX, IDC_BUTTON_SETUP_SCHEDULE, m_buttonSetupSchedule);
	/*****/
	DDX_Control(pDX, IDC_CHECK_ENABLE_CHARTWELLS, m_checkEnableChartwells);
	/*****/
	DDX_Control(pDX, IDC_BUTTON_SETUP_CSVIMPORT, m_buttonSetupCSVImport);
	DDX_Control(pDX, IDC_CHECK_TRIMZERO_EXTREF1, m_checkTrimZeroExtRef1);
	/*****/
	DDX_Control(pDX, IDC_CHECK_ENABLE_AUTOIMPORT, m_checkEnableAutoImport);
	DDX_Control(pDX, IDC_BUTTON_SETUP_AUTOIMPORT, m_buttonSetupAutoImport);
	/*****/
	DDX_Control(pDX, IDC_CHECK_ENABLE_SIMSIMPORT, m_checkEnableSimsImport);
	DDX_Control(pDX, IDC_BUTTON_SETUP_SIMSIMPORT, m_buttonSetupSimsImport);
	/*****/
	DDX_Control(pDX, IDC_CHECK_ENABLE_WONDE, m_checkEnableWonde);
	DDX_Control(pDX, IDC_BUTTON_SETUP_WONDE, m_buttonSetupWonde);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSystemPropertyPage7, CSSAutoShutdownPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_WEBPAYMENT, OnSelectWebPaymentType)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_WEBPAYMENT, OnButtonSetupWebPayment)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_SCHEDULE, OnButtonSetupSchedule)
	/*****/
	ON_BN_CLICKED(IDC_BUTTON_SETUP_CSVIMPORT, OnButtonSetupCSVImport)
	/*****/
	ON_BN_CLICKED(IDC_CHECK_ENABLE_AUTOIMPORT, OnToggleAutoImport)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_AUTOIMPORT, OnButtonSetupAutoImport)
	/*****/
	ON_BN_CLICKED(IDC_CHECK_ENABLE_SIMSIMPORT, OnToggleSimsImport)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_SIMSIMPORT, OnButtonSetupSims)
	/*****/
	ON_BN_CLICKED(IDC_CHECK_ENABLE_WONDE, OnToggleWonde)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_WONDE, OnButtonSetupWonde)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CSystemPropertyPage7::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_nCurrentWebPayment = System.GetWebPaymentType();
	CStandardCombos::FillServiceProviderCombo( m_comboWebPayment, 0 );
	m_comboWebPayment.SelectUsingItemData(m_nCurrentWebPayment, 0);
	
	m_checkEnableChartwells.SetCheck(System.GetEnableChartwellsFlag());
	m_checkEnableSimsImport.SetCheck( System.GetEnableSimsFlag() );
	m_checkEnableAutoImport.SetCheck( System.GetEnableAutoImportFlag() );
	m_checkEnableWonde.SetCheck( System.GetEnableWondeFlag());
	m_checkTrimZeroExtRef1.SetCheck( System.GetImportExtRef1TrimZeroFlag() );
	
	SetDisplay();

	return TRUE;
}

//$$******************************************************************

BOOL CSystemPropertyPage7::OnKillActive()
{
	int nWebPaymentType = CStandardCombos::GetItemDataOfSelection(m_comboWebPayment, -1);
	System.SetWebPaymentType(nWebPaymentType);

	if (nWebPaymentType != m_nCurrentWebPayment)
	{
		m_bRestartServerReqd = TRUE;
	}

	System.SetEnableChartwellsFlag(IsTicked(m_checkEnableChartwells));
	System.SetEnableSimsFlag(IsTicked(m_checkEnableSimsImport));
	System.SetEnableAutoImportFlag(IsTicked(m_checkEnableAutoImport));
	System.SetEnableWondeFlag(IsTicked(m_checkEnableWonde));
	System.SetImportExtRef1TrimZeroFlag(IsTicked(m_checkTrimZeroExtRef1));

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************

void CSystemPropertyPage7::SetDisplay()
{
	int nWebPaymentType = CStandardCombos::GetItemDataOfSelection(m_comboWebPayment, -1);
	bool bEnable = (nWEBPAYMENT_NONE != nWebPaymentType);
	
	m_buttonSetupWebPayment.EnableWindow(bEnable);
	m_buttonSetupSchedule.EnableWindow(bEnable);

	if (TRUE == bEnable)
	{
		m_buttonSetupWebPayment.SetFocus();
	}

	m_buttonSetupSimsImport.EnableWindow( IsTicked( m_checkEnableSimsImport ) );
	m_buttonSetupWonde.EnableWindow( IsTicked( m_checkEnableWonde ) );
	m_buttonSetupAutoImport.EnableWindow( IsTicked( m_checkEnableAutoImport) );
}

//$$******************************************************************

void CSystemPropertyPage7::OnSelectWebPaymentType()	
{ 
	SetDisplay();	
}

//$$******************************************************************

void CSystemPropertyPage7::OnButtonSetupWebPayment()
{
	switch (CStandardCombos::GetItemDataOfSelection(m_comboWebPayment, -1))
	{
	case nWEBPAYMENT_PARENTPAY:
	{
		CSetupParentPayDlg dlg(this);
		dlg.DoModal();
	}
	break;

	case nWEBPAYMENT_TUCASI:
	{
		CSetupTucasiDlg dlg(this);
		dlg.DoModal();
	}	
	break;

	case nWEBPAYMENT_WISEPAY:
	{
		CSetupWisePayDlg dlg(this);
		dlg.DoModal();
	}
	break;

	case nWEBPAYMENT_SCHOOLCOMMS:
	{
		CSetupSchoolcommsDlg dlg(this);
		dlg.DoModal();
	}
	break;

	case nWEBPAYMENT_TUCASIv2:
	{
		CSetupTucasiJsonDlg dlg(this);
		dlg.DoModal();
	}
	break;

	case nWEBPAYMENT_SQUID:
	{
		CSetupSquidDlg dlg(this);
		dlg.DoModal();
	}
	break;

	case nWEBPAYMENT_PARENTMAIL:
	{
		CSetupParentMailDlg dlg(this);
		dlg.DoModal();
	}
	break;

	case nWEBPAYMENT_BUCKINGHAM:
	{
		Prompter.Error("This option is currently suspended!", "Access Denied");
	}
	break;

	case nWEBPAYMENT_BROMCOM:
	{
		CSetupBromComDlg dlg(this);
		dlg.DoModal();
	}
	break;

	case nWEBPAYMENT_PEBBLE:
	{
		CSetupPebbleDlg dlg(this);
		dlg.DoModal();
	}
	break;
	}
}

//$$******************************************************************

void CSystemPropertyPage7::OnButtonSetupSchedule()	
{ 
	int nWebPaymentType = CStandardCombos::GetItemDataOfSelection(m_comboWebPayment, -1);

	if ( nWebPaymentType != nWEBPAYMENT_BUCKINGHAM )
	{
		CSetupWebPaymentOptionsDlg dlg ( nWebPaymentType, this );		// show send balance as Send end of Day
		dlg.DoModal();													// no send transaction options
	}
	else
	{
		Prompter.Error("This option is currently suspended!", "Access Denied");
	}
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CSystemPropertyPage7::OnButtonSetupCSVImport()
{
	CSetupCsvImportPropertySheet dlg ( this );
	if (dlg.DoModal() == IDOK)
	{
		dlg.Save();
	}
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CSystemPropertyPage7::OnToggleAutoImport()
{
	SetDisplay();
	m_buttonSetupCSVImport.SetFocus();
}

//$$******************************************************************

void CSystemPropertyPage7::OnButtonSetupAutoImport()
{
	CDatabaseImportDlg dlg( this );
	dlg.DoModal();
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************


void CSystemPropertyPage7::OnToggleSimsImport()
{
	SetDisplay();
	m_buttonSetupSimsImport.SetFocus();
}

//$$******************************************************************

void CSystemPropertyPage7::OnButtonSetupSims()
{
	CSetupSimsDlg dlg( this );
	dlg.DoModal();
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CSystemPropertyPage7::OnToggleWonde()
{
	SetDisplay();
	m_buttonSetupWonde.SetFocus();
}

//$$******************************************************************

void CSystemPropertyPage7::OnButtonSetupWonde()
{
	CSetupWondeDlg dlg( this );
	dlg.DoModal();
}

//$$******************************************************************