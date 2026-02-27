//*******************************************************
#include "StandardCombos.h"
//*******************************************************
#include "SetupBackgroundOptionsDlg.h"
//*******************************************************

CSetupBackgroundOptionsDlg::CSetupBackgroundOptionsDlg(CWnd* pParent) :
	CSSAutoShutdownDialog(AutoShutdownHelper, CSetupBackgroundOptionsDlg::IDD, pParent)
{
	m_options.Read();

	m_nWebPaymentMinutes = m_options.m_nWebPaymentMinutes;
	m_nNewRecordsMinutes = m_options.m_nNewRecordsMinutes;
	m_nExportSalesMinutes = m_options.m_nExportSalesMinutes;

	m_strDateEOD = m_options.m_strDateNextEODCheck;
	m_strDateLowBalance = m_options.m_strDateNextLowBalanceCheck;
	m_strDatePurchases = m_options.m_strDateNextEmailPurchase;
	m_strDatePointsAchievement = m_options.m_strDateNextPointsAchievement;
	m_strDateWeeklyPoints = m_options.m_strDateNextWeeklyPoints;
}

//*******************************************************

void CSetupBackgroundOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_dtcTime3);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_dtcTime4);
	DDX_Control(pDX, IDC_DATETIMEPICKER5, m_dtcTime5);
	DDX_Control(pDX, IDC_DATETIMEPICKER6, m_dtcTime6);
	DDX_Control(pDX, IDC_DATETIMEPICKER7, m_dtcTime7);
	DDX_Control(pDX, IDC_DATETIMEPICKER8, m_dtcTime8);
	DDX_Control(pDX, IDC_DATETIMEPICKER9, m_dtcTime9);
	DDX_Control(pDX, IDC_DATETIMEPICKER10, m_dtcTime10);
	DDX_Control(pDX, IDC_DATETIMEPICKER11, m_dtcTime11);
	DDX_Control(pDX, IDC_CHECK_EOD, m_checkEODExport);
	DDX_Control(pDX, IDC_CHECK_EMAILLOWBALANCEALERT, m_checkEmailLowBalance);
	DDX_Control(pDX, IDC_CHECK_EMAILPURCHASES, m_checkEmailPurchases);
	DDX_Control(pDX, IDC_CHECK_EMAILPOINTSACHIEVEMENT, m_checkEmailPointsAchievement);
	DDX_Control(pDX, IDC_CHECK_EMAILWEEKLYPOINTS, m_checkEmailWeeklyPoints);
	DDX_Control(pDX, IDC_CHECK_MONDAY, m_checkMonday);
	DDX_Control(pDX, IDC_CHECK_TUESDAY, m_checkTuesday);
	DDX_Control(pDX, IDC_CHECK_WEDNESDAY, m_checkWednesday);
	DDX_Control(pDX, IDC_CHECK_THURSDAY, m_checkThursday);
	DDX_Control(pDX, IDC_CHECK_FRIDAY, m_checkFriday);
	DDX_Control(pDX, IDC_CHECK_SATURDAY, m_checkSaturday);
	DDX_Control(pDX, IDC_CHECK_SUNDAY, m_checkSunday);
	DDX_Control(pDX, IDC_CHECK_WEBSUSPEND, m_checkWebPaymentSuspendOnError);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
	DDX_Control(pDX, IDC_EDIT_DATE5, m_editDate5);
	DDX_Control(pDX, IDC_EDIT_DATE6, m_editDate6);
	DDX_Control(pDX, IDC_EDIT_DATE7, m_editDate7);
	DDX_Control(pDX, IDC_EDIT_DATE10, m_editDate10);
	DDX_Control(pDX, IDC_EDIT_DATE11, m_editDate11);
	DDX_Control(pDX, IDC_EDIT_INTERVAL, m_editInterval);

	DDX_Text(pDX, IDC_EDIT_INTERVAL, m_nWebPaymentMinutes);
	DDV_MinMaxInt(pDX, m_nWebPaymentMinutes, 0, 720);
	DDX_Text(pDX, IDC_EDIT_MINUTES2, m_nNewRecordsMinutes);
	DDV_MinMaxInt(pDX, m_nNewRecordsMinutes, 0, 720);
	DDX_Text(pDX, IDC_EDIT_MINUTES3, m_nExportSalesMinutes);
	DDV_MinMaxInt(pDX, m_nExportSalesMinutes, 0, 720);

	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER3, m_timeTime3);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER4, m_timeTime4);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER5, m_timeTime5);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER6, m_timeTime6);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER7, m_timeTime7);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER8, m_timeTime8);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER9, m_timeTime9);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER10, m_timeTime10);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER11, m_timeTime11);

	DDX_Text(pDX, IDC_EDIT_DATE5, m_strDateEOD);
	DDX_Text(pDX, IDC_EDIT_DATE6, m_strDateLowBalance);
	DDX_Text(pDX, IDC_EDIT_DATE7, m_strDatePurchases);
	DDX_Text(pDX, IDC_EDIT_DATE10, m_strDatePointsAchievement);
	DDX_Text(pDX, IDC_EDIT_DATE11, m_strDateWeeklyPoints);
}

//*******************************************************

BEGIN_MESSAGE_MAP(CSetupBackgroundOptionsDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_CHECK_EOD, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_EMAILLOWBALANCEALERT, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_EMAILPURCHASES, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_EMAILPOINTSACHIEVEMENT, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_EMAILWEEKLYPOINTS, SetDisplay)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
END_MESSAGE_MAP()

//***********************************************************************

void CSetupBackgroundOptionsDlg::CSSAutoShutdownPreInitDialog()
{
	SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_BACKGROUND), TRUE);

	CSSTime time;

	time.SetTime(m_options.m_strNewRecordStart);
	m_timeTime3.SetTime(time.GetHour(), time.GetMinute(), 0);

	time.SetTime(m_options.m_strNewRecordEnd);
	m_timeTime4.SetTime(time.GetHour(), time.GetMinute(), 0);

	time.SetTime(m_options.m_strTimeNextEODCheck);
	m_timeTime5.SetTime(time.GetHour(), time.GetMinute(), 0);

	time.SetTime(m_options.m_strTimeNextLowBalanceCheck);
	m_timeTime6.SetTime(time.GetHour(), time.GetMinute(), 0);

	time.SetTime(m_options.m_strTimeEmailPurchase);
	m_timeTime7.SetTime(time.GetHour(), time.GetMinute(), 0);

	time.SetTime(m_options.m_strExportSalesStart);
	m_timeTime8.SetTime(time.GetHour(), time.GetMinute(), 0);

	time.SetTime(m_options.m_strExportSalesEnd);
	m_timeTime9.SetTime(time.GetHour(), time.GetMinute(), 0);

	time.SetTime(m_options.m_strTimeNextPointsAchievement);
	m_timeTime10.SetTime(time.GetHour(), time.GetMinute(), 0);

	time.SetTime(m_options.m_strTimeNextWeeklyPoints);
	m_timeTime11.SetTime(time.GetHour(), time.GetMinute(), 0);
}

//***********************************************************************

BOOL CSetupBackgroundOptionsDlg::CSSAutoShutdownPostInitDialog()
{
	m_dtcTime3.SetFormat("HH:mm");
	m_dtcTime4.SetFormat("HH:mm");
	m_dtcTime5.SetFormat("HH:mm");				// eod closing balance
	m_dtcTime6.SetFormat("HH:mm");				// low balance
	m_dtcTime7.SetFormat("HH:mm");				// email purchases
	m_dtcTime8.SetFormat("HH:mm");				// Export Sales Start
	m_dtcTime9.SetFormat("HH:mm");				// Export Sales End
	m_dtcTime10.SetFormat("HH:mm");				// Points Achievement
	m_dtcTime11.SetFormat("HH:mm");				// Weekly Points

	m_checkMonday.SetCheck(m_options.IsEODDay(1));
	m_checkTuesday.SetCheck(m_options.IsEODDay(2));
	m_checkWednesday.SetCheck(m_options.IsEODDay(3));
	m_checkThursday.SetCheck(m_options.IsEODDay(4));
	m_checkFriday.SetCheck(m_options.IsEODDay(5));
	m_checkSaturday.SetCheck(m_options.IsEODDay(6));
	m_checkSunday.SetCheck(m_options.IsEODDay(0));

	m_checkEmailLowBalance.SetCheck(m_options.m_bEmailLowBalance);
	m_checkEmailPurchases.SetCheck(m_options.m_bEmailPurchases);
	m_checkEmailPointsAchievement.SetCheck(m_options.m_bEmailPointsAchievement);
	m_checkEmailWeeklyPoints.SetCheck(m_options.m_bEmailWeeklyPoints);

	m_checkWebPaymentSuspendOnError.SetCheck(m_options.m_bWebPaymentSuspendOnError);
	m_checkEODExport.SetCheck(m_options.m_bEODExport);

	m_emailAlert.Read();

	if (FALSE == m_emailAlert.m_bEnableLowBalanceAlerts)
	{
		m_checkEmailLowBalance.SetCheck(FALSE);
		m_checkEmailLowBalance.EnableWindow(FALSE);
	}

	if (FALSE == m_emailAlert.m_bEnablePurchaseAlerts)
	{
		m_checkEmailPurchases.SetCheck(FALSE);
		m_checkEmailPurchases.EnableWindow(FALSE);
	}

	if (FALSE == m_emailAlert.m_bEnablePointsAchievement)
	{
		m_checkEmailPointsAchievement.SetCheck(FALSE);
		m_checkEmailPointsAchievement.EnableWindow(FALSE);
	}

	if (FALSE == m_emailAlert.m_bEnableWeeklyPoints)
	{
		m_checkEmailWeeklyPoints.SetCheck(FALSE);
		m_checkEmailWeeklyPoints.EnableWindow(FALSE);
	}

	if (System.GetWebPaymentType() == nWEBPAYMENT_NONE)
	{
		m_editInterval.EnableWindow(FALSE);
	}

	m_editDate5.EnableWindow(FALSE);				// DateEOD
	m_editDate6.EnableWindow(FALSE);				// DateLowBalance
	m_editDate7.EnableWindow(FALSE);				// DatePurchases
	m_editDate10.EnableWindow(FALSE);				// DatePointsAchievement
	m_editDate11.EnableWindow(FALSE);				// DateWeeklyPoints

	SetDisplay();

	m_buttonAltKey1.ShowWindow(SW_HIDE);

	return TRUE;
}

//*******************************************************

void CSetupBackgroundOptionsDlg::OnButtonAltKey1()
{
	m_editDate5.EnableWindow(TRUE);				// DateEOD
	m_editDate6.EnableWindow(TRUE);				// DateLowBalance
	m_editDate7.EnableWindow(TRUE);				// DatePurchases
	m_editDate10.EnableWindow(TRUE);			// DatePointsAchievement
	m_editDate11.EnableWindow(TRUE);			// DateWeeklyPoints
}

//*******************************************************

void CSetupBackgroundOptionsDlg::SetDisplay()
{
	if (UpdateData() == TRUE)
	{
		{
			bool bEODExport = IsTicked(m_checkEODExport);
			m_dtcTime5.EnableWindow(bEODExport);
			m_checkMonday.EnableWindow(bEODExport);
			m_checkTuesday.EnableWindow(bEODExport);
			m_checkWednesday.EnableWindow(bEODExport);
			m_checkThursday.EnableWindow(bEODExport);
			m_checkFriday.EnableWindow(bEODExport);
			m_checkSaturday.EnableWindow(bEODExport);
			m_checkSunday.EnableWindow(bEODExport);
		}

		m_dtcTime6.EnableWindow(IsTicked(m_checkEmailLowBalance));
		m_dtcTime7.EnableWindow(IsTicked(m_checkEmailPurchases));
		m_dtcTime10.EnableWindow(IsTicked(m_checkEmailPointsAchievement));
		m_dtcTime11.EnableWindow(IsTicked(m_checkEmailWeeklyPoints));
	}
}

//*******************************************************

void CSetupBackgroundOptionsDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		m_options.m_bWebPaymentSuspendOnError = IsTicked(m_checkWebPaymentSuspendOnError);
		m_options.m_nWebPaymentMinutes = m_nWebPaymentMinutes;
		m_options.m_nNewRecordsMinutes = m_nNewRecordsMinutes;
		m_options.m_nExportSalesMinutes = m_nExportSalesMinutes;
		m_options.m_bEODExport = IsTicked(m_checkEODExport);

		m_options.m_bEmailLowBalance = IsTicked(m_checkEmailLowBalance);
		m_options.m_bEmailPurchases = IsTicked(m_checkEmailPurchases);
		m_options.m_bEmailPointsAchievement = IsTicked(m_checkEmailPointsAchievement);
		m_options.m_bEmailWeeklyPoints = IsTicked(m_checkEmailWeeklyPoints);

		COleDateTime time;
		m_dtcTime3.GetTime(time);	m_options.m_strNewRecordStart = time.Format("%H:%M");
		m_dtcTime4.GetTime(time);	m_options.m_strNewRecordEnd = time.Format("%H:%M");
		m_dtcTime5.GetTime(time);	m_options.m_strTimeNextEODCheck = time.Format("%H:%M");
		m_dtcTime6.GetTime(time);	m_options.m_strTimeNextLowBalanceCheck = time.Format("%H:%M");
		m_dtcTime7.GetTime(time);	m_options.m_strTimeEmailPurchase = time.Format("%H:%M");
		m_dtcTime8.GetTime(time);	m_options.m_strExportSalesStart = time.Format("%H:%M");
		m_dtcTime9.GetTime(time);	m_options.m_strExportSalesEnd = time.Format("%H:%M");
		m_dtcTime10.GetTime(time);	m_options.m_strTimeNextPointsAchievement = time.Format("%H:%M");
		m_dtcTime11.GetTime(time);	m_options.m_strTimeNextWeeklyPoints = time.Format("%H:%M");

		m_options.m_strDateNextEODCheck = m_strDateEOD;
		m_options.m_strDateNextLowBalanceCheck = m_strDateLowBalance;
		m_options.m_strDateNextEmailPurchase = m_strDatePurchases;
		m_options.m_strDateNextPointsAchievement = m_strDatePointsAchievement;
		m_options.m_strDateNextWeeklyPoints = m_strDateWeeklyPoints;

		m_options.SetEODDay(1, IsTicked(m_checkMonday));
		m_options.SetEODDay(2, IsTicked(m_checkTuesday));
		m_options.SetEODDay(3, IsTicked(m_checkWednesday));
		m_options.SetEODDay(4, IsTicked(m_checkThursday));
		m_options.SetEODDay(5, IsTicked(m_checkFriday));
		m_options.SetEODDay(6, IsTicked(m_checkSaturday));
		m_options.SetEODDay(0, IsTicked(m_checkSunday));

		m_options.Save();

		EndDialog(IDOK);
	}
}

//*******************************************************

