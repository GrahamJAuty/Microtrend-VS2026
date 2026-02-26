/**********************************************************************/
#include "EposReportCustomSettings.h"
#include "Password.h"
/**********************************************************************/
#include "EposReportCustomPluPriceBandDlg.h"
/**********************************************************************/

CEposReportCustomPluPriceBandDlg::CEposReportCustomPluPriceBandDlg( CEposReportSelect& EposReportSelect, CEposReportSelectInfo& infoReport, bool bAdhoc, CWnd* pParent)
: CSSDialog(CEposReportCustomPluPriceBandDlg::IDD, pParent), m_EposReportSelect( EposReportSelect ), m_infoReport( infoReport )
{
	m_bAdhoc = bAdhoc;
}

/**********************************************************************/

CEposReportCustomPluPriceBandDlg::~CEposReportCustomPluPriceBandDlg()
{
}

/**********************************************************************/

void CEposReportCustomPluPriceBandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_CHECK_ADHOC, m_checkAdhoc);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_buttonSave);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	DDX_Control(pDX, IDC_CHECK_BAND1, m_checkPriceBand[0]);
	DDX_Control(pDX, IDC_CHECK_BAND2, m_checkPriceBand[1]);
	DDX_Control(pDX, IDC_CHECK_BAND3, m_checkPriceBand[2]);
	DDX_Control(pDX, IDC_CHECK_BAND4, m_checkPriceBand[3]);
	DDX_Control(pDX, IDC_CHECK_BAND5, m_checkPriceBand[4]);
	DDX_Control(pDX, IDC_CHECK_BAND6, m_checkPriceBand[5]);
	DDX_Control(pDX, IDC_CHECK_BAND7, m_checkPriceBand[6]);
	DDX_Control(pDX, IDC_CHECK_BAND8, m_checkPriceBand[7]);
	DDX_Control(pDX, IDC_CHECK_BAND9, m_checkPriceBand[8]);
	DDX_Control(pDX, IDC_CHECK_BAND10, m_checkPriceBand[9]);
	DDX_Control(pDX, IDC_CHECK_COLUMNNAMES, m_checkColumnNames);
	DDX_Control(pDX, IDC_CHECK_SERVERSORT, m_checkServerSort);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CEposReportCustomPluPriceBandDlg, CDialog)
	ON_BN_CLICKED( IDC_BUTTON_SAVE,OnButtonSave)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CEposReportCustomPluPriceBandDlg::OnInitDialog()
{
	CEposReportCustomSettingsPluPriceBand Settings;
	CString strParams = m_infoReport.GetCustomReportParams();
	Settings.SetSettingsCSVLine(strParams);

	CDialog::OnInitDialog();

	m_editName.LimitText(EPOS_CUSTOM_MAXLEN_NAME);
	m_editName.SetWindowText(m_infoReport.GetCustomReportName());

	m_checkAdhoc.SetCheck(Settings.GetAllowAdhocFlag());

	bool bEnableSave = TRUE;
	bEnableSave &= PasswordArray.GetEnable(PasswordTicks::SetupMenu);
	bEnableSave &= PasswordArray.GetEnable(PasswordTicks::SetupMenuTexts);

	if (TRUE == m_bAdhoc)
	{
		m_editName.EnableWindow(FALSE);
		m_checkAdhoc.ShowWindow(SW_HIDE);
		m_checkAdhoc.EnableWindow(FALSE);
		SetWindowText("Adhoc Plu by Price Band Report Settings");
	}
	else
	{
		SetWindowText("Setup Custom Plu by Price Band Report");
	}

	m_comboType.AddString("Plu Sales");
	m_comboType.AddString("Plu Sales no Zero Skip");
	m_comboType.AddString("VIP Sales");
	m_comboType.AddString("Wastage");

	int nType = Settings.GetReportSubType();
	if (nType < 0 || nType >= m_comboType.GetCount())
	{
		nType = 0;
	}
	m_comboType.SetCurSel(nType);

	for (int i = 0; i < 10; i++)
	{
		CString strText = "";
		strText.Format("%2.2d, %s", i + 1, DataManager.PriceText.GetReportText(i));
		m_checkPriceBand[i].SetWindowText(strText);
		m_checkPriceBand[i].SetCheck(Settings.IsPriceBandEnabled(i));
	}

	m_checkColumnNames.SetCheck(Settings.GetColumnNamesFlag());
	m_checkServerSort.SetCheck(Settings.GetSortByServerFlag());

	if ((FALSE == m_bAdhoc) || (FALSE == bEnableSave))
	{
		m_buttonSave.ShowWindow(SW_HIDE);
		m_buttonSave.EnableWindow(FALSE);
	}

	return TRUE;
}

/**********************************************************************/

bool CEposReportCustomPluPriceBandDlg::GetUpdatedSettings(CEposReportSelectInfo& infoReport)
{
	UpdateData(TRUE);

	CEposReportCustomSettingsPluPriceBand Settings;
	Settings.SetAllowAdhocFlag(m_checkAdhoc.GetCheck() != 0);

	{
		CString strText = "";
		m_editName.GetWindowText(strText);
		infoReport.SetCustomReportName(strText);
	}

	Settings.SetReportSubType(m_comboType.GetCurSel());

	for (int i = 0; i < 10; i++)
	{
		Settings.SetPriceBandEnabled(i, IsTicked(m_checkPriceBand[i]));
	}

	Settings.SetColumnNamesFlag(IsTicked(m_checkColumnNames));
	Settings.SetSortByServerFlag(IsTicked(m_checkServerSort));

	CString strLine = "";
	Settings.GetSettingsCSVLine(strLine);
	infoReport.SetCustomReportParams(strLine);

	return TRUE;
}

/**********************************************************************/

void CEposReportCustomPluPriceBandDlg::OnButtonSave()
{
	CEposReportSelectInfo infoReport = m_infoReport;

	if (GetUpdatedSettings(infoReport) == TRUE)
	{
		m_EposReportSelect.SetReportInfo(infoReport);
		m_EposReportSelect.SaveReportSelection();
	}
}

/**********************************************************************/

void CEposReportCustomPluPriceBandDlg::OnOK()
{
	if (GetUpdatedSettings(m_infoReport) == TRUE)
	{
		EndDialog(IDOK);
	}
}

/**********************************************************************/
