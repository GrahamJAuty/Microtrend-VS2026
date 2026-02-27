//$$******************************************************************
#include "AutoShutdownStacker.h"
#include "StandardCombos.h"
#include "TerminalSetsDlg.h"
//$$******************************************************************
#include "SetupEODTextsDlg.h"
//$$******************************************************************

CSetupEODTextsDlg::CSetupEODTextsDlg(CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper,CSetupEODTextsDlg::IDD, pParent)
{
	m_texts.Read();
}

//$$******************************************************************

CSetupEODTextsDlg::~CSetupEODTextsDlg()
{
}

//$$******************************************************************

void CSetupEODTextsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POSTOPUP, m_strPosTopup);
	DDX_Text(pDX, IDC_EDIT_POSREFUND, m_strPosTopupRefund);
	DDX_Text(pDX, IDC_EDIT_OFFICETOPUP, m_strOfficeTopup);
	DDX_Text(pDX, IDC_EDIT_OFFICEREFUND, m_strOfficeTopupRefund);
	DDX_Text(pDX, IDC_EDIT_IMPORTTOPUP, m_strImportTopup);
	DDX_Text(pDX, IDC_EDIT_IMPORTREFUND, m_strImportTopupRefund);
	DDX_Text(pDX, IDC_EDIT_CREDITADJUSTUP, m_strCreditAdjustUp);
	DDX_Text(pDX, IDC_EDIT_CREDITADJUSTDOWN, m_strCreditAdjustDown);
	DDX_Text(pDX, IDC_EDIT_OFFICEADJUSTMENT, m_strOfficeAdjustment);
	DDX_Text(pDX, IDC_EDIT_KIOSKTOPUP, m_strKioskTopup);
	DDX_Text(pDX, IDC_EDIT_WEBTOPUP, m_strWebpaymentTopup);
	DDX_Text(pDX, IDC_EDIT_WEBREFUND, m_strWebpaymentRefund);
	DDX_Text(pDX, IDC_EDIT_POSSALEREFUND, m_strPosSaleRefund);
	DDX_Text(pDX, IDC_EDIT_OFFICESALEADJUSTMENT, m_strOfficeSaleAdjustment);
	DDX_Text(pDX, IDC_EDIT_OFFICEDELETION, m_strOfficeDeletion);
	DDX_Text(pDX, IDC_EDIT_PURSE1, m_strPurse1Text);
	DDX_Text(pDX, IDC_EDIT_PURSE2, m_strPurse2Text);
	DDX_Text(pDX, IDC_EDIT_PURSE3, m_strPurse3Text);
	DDX_Text(pDX, IDC_EDIT_CASH, m_strCashText);
	DDX_Text(pDX, IDC_EDIT_PEBBLEORDER, m_strPebbleOrderText);
	DDX_Text(pDX, IDC_EDIT_PEBBLEORDERREFUND, m_strPebbleOrderRefundText);
	DDX_Check(pDX, IDC_CHECK_ZEROSKIP, m_bZeroSkip);
	DDX_Check(pDX, IDC_CHECK_ZEROSKIPVAT, m_bZeroSkipVAT);
	DDX_Control(pDX, IDC_COMBO_OUTPUT, m_comboOutput);
	DDX_CBIndex(pDX, IDC_COMBO_OUTPUT, m_nSaveAsType);
	DDX_Check(pDX, IDC_CHECK_EXCLUDEPERIOD, m_bExcludePeriodSales);
	DDX_Check(pDX, IDC_CHECK_EXCLUDEPAYMENTS, m_bExcludeEposPayments);
	DDX_Check(pDX, IDC_CHECK_EXCLUDESALESBREAKDOWN, m_bExcludeSalesBreakdown);
	DDX_Text(pDX, IDC_EDIT_OFFICEMEAL, m_strOfficeMeal);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSetupEODTextsDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
END_MESSAGE_MAP()

//$$******************************************************************

void CSetupEODTextsDlg::CSSAutoShutdownPreInitDialog()
{
	m_bZeroSkip = m_texts.IsZeroSkip();
	m_bZeroSkipVAT = m_texts.IsZeroSkipVAT();
	m_nSaveAsType = m_texts.GetSaveAsType();
	m_bExcludePeriodSales = m_texts.ExcludePeriodSales();
	m_bExcludeEposPayments = m_texts.ExcludeEposPayments();
	m_bExcludeSalesBreakdown = m_texts.ExcludeSalesBreakdown();

	m_strPosTopup = m_texts.GetText(nPOSTOPUP);
	m_strPosTopupRefund = m_texts.GetText(nPOSTOPUPREFUND);
	m_strOfficeTopup = m_texts.GetText(nOFFICETOPUP);
	m_strOfficeTopupRefund = m_texts.GetText(nOFFICETOPUPREFUND);
	m_strImportTopup = m_texts.GetText(nIMPORTTOPUP);
	m_strImportTopupRefund = m_texts.GetText(nIMPORTTOPUPREFUND);
	m_strCreditAdjustUp = m_texts.GetText(nCREDITADJUSTPCUP);
	m_strCreditAdjustDown = m_texts.GetText(nCREDITADJUSTPCDOWN);
	m_strOfficeAdjustment = m_texts.GetText(nOFFICEADJUSTMENT);
	m_strKioskTopup = m_texts.GetText(nKIOSKTOPUP);
	m_strWebpaymentTopup = m_texts.GetText(nWEBPAYMENTTOPUP);
	m_strWebpaymentRefund = m_texts.GetText(nWEBPAYMENTREFUND);
	m_strPosSaleRefund = m_texts.GetText(nPOSSALEREFUND);
	m_strOfficeSaleAdjustment = m_texts.GetText(nOFFICESALEADJUSTMENT);
	m_strOfficeDeletion = m_texts.GetText(nOFFICEDELETION);
	m_strPurse1Text = m_texts.GetText(nPURSE1TEXT);
	m_strPurse2Text = m_texts.GetText(nPURSE2TEXT);
	m_strPurse3Text = m_texts.GetText(nPURSE3TEXT);
	m_strCashText = m_texts.GetText(nCASHTEXT);
	m_strOfficeMeal = m_texts.GetText(nOFFICEMEAL);
	m_strPebbleOrderText = m_texts.GetText(nPEBBLEORDER);
	m_strPebbleOrderRefundText = m_texts.GetText(nPEBBLEORDERREFUND);
}

//$$******************************************************************

BOOL CSetupEODTextsDlg::CSSAutoShutdownPostInitDialog()
{
	CStandardCombos::FillOutputTypeCombo( m_comboOutput, m_nSaveAsType );
	return TRUE;
}

//$$******************************************************************

void CSetupEODTextsDlg::OnButtonSet()
{
	CAutoShutdownDlgStacker Stacker(this);
	CTerminalSetsDlg dlg ( this );
	dlg.DoModal();
}

//$$******************************************************************

void CSetupEODTextsDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		m_texts.SetZeroSkip(m_bZeroSkip != 0);
		m_texts.SetZeroSkipVAT(m_bZeroSkipVAT != 0);
		m_texts.SetSaveAsType(m_nSaveAsType);
		m_texts.SetExcludePeriodSales(m_bExcludePeriodSales != 0);
		m_texts.SetExcludeEposPayments(m_bExcludeEposPayments != 0);
		m_texts.SetExcludeSalesBreakdown(m_bExcludeSalesBreakdown != 0);

		m_texts.SetText(nPOSTOPUP, m_strPosTopup);
		m_texts.SetText(nPOSTOPUPREFUND, m_strPosTopupRefund);
		m_texts.SetText(nOFFICETOPUP, m_strOfficeTopup);
		m_texts.SetText(nOFFICETOPUPREFUND, m_strOfficeTopupRefund);
		m_texts.SetText(nIMPORTTOPUP, m_strImportTopup);
		m_texts.SetText(nIMPORTTOPUPREFUND, m_strImportTopupRefund);
		m_texts.SetText(nCREDITADJUSTPCUP, m_strCreditAdjustUp);
		m_texts.SetText(nCREDITADJUSTPCDOWN, m_strCreditAdjustDown);
		m_texts.SetText(nOFFICEADJUSTMENT, m_strOfficeAdjustment);
		m_texts.SetText(nKIOSKTOPUP, m_strKioskTopup);
		m_texts.SetText(nWEBPAYMENTTOPUP, m_strWebpaymentTopup);
		m_texts.SetText(nWEBPAYMENTREFUND, m_strWebpaymentRefund);
		m_texts.SetText(nPOSSALEREFUND, m_strPosSaleRefund);
		m_texts.SetText(nOFFICESALEADJUSTMENT, m_strOfficeSaleAdjustment);
		m_texts.SetText(nOFFICEDELETION, m_strOfficeDeletion);
		m_texts.SetText(nPURSE1TEXT, m_strPurse1Text);
		m_texts.SetText(nPURSE2TEXT, m_strPurse2Text);
		m_texts.SetText(nPURSE3TEXT, m_strPurse3Text);
		m_texts.SetText(nCASHTEXT, m_strCashText);
		m_texts.SetText(nOFFICEMEAL, m_strOfficeMeal);
		m_texts.SetText(nPEBBLEORDER, m_strPebbleOrderText);
		m_texts.SetText(nPEBBLEORDERREFUND, m_strPebbleOrderRefundText);
		m_texts.Save();

		EndDialog(IDOK);
	}
}

//$$******************************************************************

