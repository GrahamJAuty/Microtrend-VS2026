/**********************************************************************/
#include "EposReportCustomSettings.h"
#include "Password.h"
/**********************************************************************/
#include "EposReportCustomPaymentSummaryDlg.h"
/**********************************************************************/

CEposReportCustomPaymentSummaryDlg::CEposReportCustomPaymentSummaryDlg( CEposReportSelect& EposReportSelect, CEposReportSelectInfo& infoReport, bool bAdhoc, CWnd* pParent)
: CSSDialog(CEposReportCustomPaymentSummaryDlg::IDD, pParent), m_EposReportSelect( EposReportSelect ), m_infoReport( infoReport )
{
	m_bAdhoc = bAdhoc;
}

/**********************************************************************/

CEposReportCustomPaymentSummaryDlg::~CEposReportCustomPaymentSummaryDlg()
{
}

/**********************************************************************/

void CEposReportCustomPaymentSummaryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_CHECK_DATE, m_checkDate);
	DDX_Control(pDX, IDC_CHECK_ADHOC, m_checkAdhoc);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_buttonSave);
	DDX_Control(pDX, IDC_RADIO_SUBTYPE1, m_radioSubType1);
	DDX_Control(pDX, IDC_RADIO_SUBTYPE2, m_radioSubType2);
	DDX_Control(pDX, IDC_RADIO_SUBTYPE3, m_radioSubType3);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CEposReportCustomPaymentSummaryDlg, CDialog)
	ON_BN_CLICKED( IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED( IDC_RADIO_SUBTYPE1, OnRadioSubType1)
	ON_BN_CLICKED( IDC_RADIO_SUBTYPE2, OnRadioSubType2)
	ON_BN_CLICKED( IDC_RADIO_SUBTYPE3, OnRadioSubType3)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CEposReportCustomPaymentSummaryDlg::OnInitDialog()
{
	CEposReportCustomSettingsPaymentSummary Settings;
	CString strParams = m_infoReport.GetCustomReportParams();
	Settings.SetSettingsCSVLine(strParams);

	CDialog::OnInitDialog();

	m_editName.LimitText(EPOS_CUSTOM_MAXLEN_NAME);
	m_editName.SetWindowText(m_infoReport.GetCustomReportName());

	m_checkAdhoc.SetCheck(Settings.GetAllowAdhocFlag());
	m_checkDate.SetCheck(Settings.GetSeparateByDateFlag());

	switch (Settings.GetReportSubType())
	{
	case 1: 
		SetSubType(1);
		break;
	
	case 2: 
		SetSubType(2);
		break;
	
	case 0: 
	default:
		SetSubType(0);
		break;
	}

	bool bEnableSave = TRUE;
	bEnableSave &= PasswordArray.GetEnable(PasswordTicks::SetupMenu);
	bEnableSave &= PasswordArray.GetEnable(PasswordTicks::SetupMenuTexts);

	if (TRUE == m_bAdhoc)
	{
		m_editName.EnableWindow(FALSE);
		m_checkAdhoc.ShowWindow(SW_HIDE);
		m_checkAdhoc.EnableWindow(FALSE);
		SetWindowText("Adhoc Payment Summary Report Settings");
	}
	else
	{
		SetWindowText("Setup Custom Payment Summary Report");
	}

	if ((FALSE == m_bAdhoc) || (FALSE == bEnableSave))
	{
		m_buttonSave.ShowWindow(SW_HIDE);
		m_buttonSave.EnableWindow(FALSE);
	}

	return TRUE;
}

/**********************************************************************/

bool CEposReportCustomPaymentSummaryDlg::GetUpdatedSettings(CEposReportSelectInfo& infoReport)
{
	UpdateData(TRUE);

	CEposReportCustomSettingsPaymentSummary Settings;
	Settings.SetSeparateByDateFlag(m_checkDate.GetCheck() != 0);
	Settings.SetAllowAdhocFlag(m_checkAdhoc.GetCheck() != 0);
	Settings.SetReportSubType(0);

	if (IsTicked(m_radioSubType2))
	{
		Settings.SetReportSubType(1);
	}
	else if (IsTicked(m_radioSubType3))
	{
		Settings.SetReportSubType(2);
	}

	infoReport.SetCustomReportFamily(EPOS_CUSTOM_FAMILY_PAYMENTSUMMARY);

	{
		CString strText = "";
		m_editName.GetWindowText(strText);
		infoReport.SetCustomReportName(strText);
	}

	CString strLine = "";
	Settings.GetSettingsCSVLine(strLine);
	infoReport.SetCustomReportParams(strLine);

	return TRUE;
}

/**********************************************************************/

void CEposReportCustomPaymentSummaryDlg::OnRadioSubType1()
{
	SetSubType(0);
}

/**********************************************************************/

void CEposReportCustomPaymentSummaryDlg::OnRadioSubType2()
{
	SetSubType(1);
}

/**********************************************************************/

void CEposReportCustomPaymentSummaryDlg::OnRadioSubType3()
{
	SetSubType(2);
}

/**********************************************************************/

void CEposReportCustomPaymentSummaryDlg::SetSubType(int n)
{
	m_radioSubType1.SetCheck(0 == n);
	m_radioSubType2.SetCheck(1 == n);
	m_radioSubType3.SetCheck(2 == n);
}

/**********************************************************************/

void CEposReportCustomPaymentSummaryDlg::OnButtonSave()
{
	CEposReportSelectInfo infoReport = m_infoReport;
	
	if ( GetUpdatedSettings( infoReport ) == TRUE )
	{
		m_EposReportSelect.SetReportInfo( infoReport );
		m_EposReportSelect.SaveReportSelection();
	}
}

/**********************************************************************/

void CEposReportCustomPaymentSummaryDlg::OnOK()
{
	if (GetUpdatedSettings(m_infoReport) == TRUE)
	{
		EndDialog(IDOK);
	}
}

/**********************************************************************/
