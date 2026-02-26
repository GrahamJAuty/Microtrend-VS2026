/**********************************************************************/
#include "..\CommonEcrStockTray\AutoBackupOptions.h"
/**********************************************************************/
#include "..\CommonEcrStockTray\PCOptions.h"
/**********************************************************************/
#include "AutoBackupProcessor.h"
#include "BackupInfoDlg.h"
#include "DefEditDlg.h"
#include "FileRemover.h"
#include "PropertySheetBackup.h"
/**********************************************************************/
#include "PropPageOptionsBackup.h"
/**********************************************************************/

CPropPageOptionsBackup::CPropPageOptionsBackup() : CSSPropertyPage(CPropPageOptionsBackup::IDD)
{
	//{{AFX_DATA_INIT(CPropPageOptionsBackup)
	//}}AFX_DATA_INIT
	m_nType = 0;
	m_nActiveTab = 0;
}

/**********************************************************************/

CPropPageOptionsBackup::~CPropPageOptionsBackup()
{
}

/**********************************************************************/

void CPropPageOptionsBackup::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageOptionsBackup)

	DDX_Control(pDX, IDC_TAB_ACTIONS, m_TabActions);

	DDX_Control(pDX, IDC_RADIO_NONE, m_radioNone);
	DDX_Control(pDX, IDC_RADIO_HOURLY, m_radioHourly);
	DDX_Control(pDX, IDC_RADIO_DAILY, m_radioDaily);
	DDX_Control(pDX, IDC_RADIO_WEEKLY, m_radioWeekly);
	DDX_Control(pDX, IDC_RADIO_CUSTOM, m_radioCustom);
	DDX_Control(pDX, IDC_COMBO_MINUTE_HOURLY, m_comboMinuteHourly);
	DDX_Control(pDX, IDC_STATIC_SCHEDULE1, m_staticSchedule1);
	DDX_Control(pDX, IDC_TIMEPICKER_DAILY, m_TimePickerDaily);
	DDX_Control(pDX, IDC_TIMEPICKER_WEEKLY, m_TimePickerWeekly);
	DDX_Control(pDX, IDC_COMBO_WEEKDAY, m_comboWeekday);
	DDX_Control(pDX, IDC_COMBO_MINUTE_CUSTOM, m_comboMinuteCustom);
	DDX_Control(pDX, IDC_STATIC_SCHEDULE2, m_staticSchedule2);
	DDX_Control(pDX, IDC_BUTTON_INFO, m_buttonInfo);
	DDX_Control(pDX, IDC_BUTTON_SCHEDULE, m_buttonSchedule);

	DDX_Control(pDX, IDC_STATIC_OPTIONS1, m_staticOptions1);
	DDX_Control(pDX, IDC_EDIT_FOLDER, m_editFolder);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_buttonEdit);
	DDX_Control(pDX, IDC_BUTTON_CHECK, m_buttonCheck);
	DDX_Control(pDX, IDC_STATIC_OPTIONS2, m_staticOptions2);
	DDX_Control(pDX, IDC_COMBO_PREVIOUS, m_comboPrevious);
	DDX_Control(pDX, IDC_CHECK_ALWAYSPROMPT, m_checkAlwaysPrompt);
	DDX_Control(pDX, IDC_CHECK_PROMPT, m_checkPrompt);
	DDX_Control(pDX, IDC_COMBO_PROMPT, m_comboPrompt);
	DDX_Control(pDX, IDC_STATIC_OPTIONS3, m_staticOptions3);
	DDX_Control(pDX, IDC_COMBO_PROMPT2, m_comboPrompt2);

	DDX_Control(pDX, IDC_CHECK_EXCLUDESALES, m_checkExcludeSales);
	DDX_Control(pDX, IDC_CHECK_EXCLUDEZERO, m_checkExcludeZero);
	DDX_Control(pDX, IDC_STATIC_ACCESSTYPE, m_staticAccessType);
	DDX_Control(pDX, IDC_COMBO_ACCESSTYPE, m_comboAccessType);
	DDX_Control(pDX, IDC_STATIC_COMPRESSION, m_staticCompression);
	DDX_Control(pDX, IDC_COMBO_COMPRESSION, m_comboCompression);

	DDX_Control(pDX, IDC_STATIC_WARNING, m_staticWarning);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageOptionsBackup, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageOptionsBackup)
	
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ACTIONS, OnTcnSelchangeTabs)
	
	ON_BN_CLICKED(IDC_RADIO_NONE, OnRadioNone)
	ON_BN_CLICKED(IDC_RADIO_HOURLY, OnRadioHourly)
	ON_BN_CLICKED(IDC_RADIO_DAILY, OnRadioDaily)
	ON_BN_CLICKED(IDC_RADIO_WEEKLY, OnRadioWeekly)
	ON_BN_CLICKED(IDC_RADIO_CUSTOM, OnRadioCustom)
	ON_BN_CLICKED(IDC_BUTTON_INFO, OnButtonInfo)
	ON_BN_CLICKED(IDC_BUTTON_SCHEDULE, OnButtonSchedule)

	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, OnButtonCheck)
	ON_BN_CLICKED(IDC_CHECK_PROMPT, OnTogglePrompt)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageOptionsBackup::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_TabActions.InsertItem(0, "Auto Backup Schedule");
	m_TabActions.InsertItem(1, "Auto Backup Options");
	m_TabActions.InsertItem(2, "General");
	m_TabActions.SetCurSel(0);

	MoveControl(&m_radioNone, 16, 38);
	MoveControl(&m_radioHourly, 16, 62);
	MoveControl(&m_radioDaily, 16, 86);
	MoveControl(&m_radioWeekly, 16, 110);
	MoveControl(&m_radioCustom, 16, 134);
	MoveControl(&m_comboMinuteHourly, 117, 61);
	MoveControl(&m_staticSchedule1, 164, 63);
	MoveControl(&m_TimePickerDaily, 117, 85);
	MoveControl(&m_TimePickerWeekly, 117, 109);
	MoveControl(&m_comboWeekday, 164, 110);
	MoveControl(&m_comboMinuteCustom, 117, 133);
	MoveControl(&m_staticSchedule2, 164, 135);
	MoveControl(&m_buttonInfo, 260, 61);
	MoveControl(&m_buttonSchedule, 260, 133);
	/*****/
	MoveControl(&m_staticOptions1, 16, 38);
	MoveControl(&m_editFolder, 117, 38);
	MoveControl(&m_buttonBrowse, 321, 38);
	MoveControl(&m_buttonEdit, 361, 38);
	MoveControl(&m_buttonCheck, 401, 38);
	MoveControl(&m_staticOptions2, 16, 62);
	MoveControl(&m_comboPrevious, 117, 62);
	MoveControl(&m_checkAlwaysPrompt, 16, 86);
	MoveControl(&m_checkPrompt, 16, 110);
	MoveControl(&m_comboPrompt, 117, 110);
	MoveControl(&m_staticOptions3, 213, 110);
	MoveControl(&m_comboPrompt2, 269, 110);
	/*****/
	MoveControl(&m_checkExcludeSales, 16, 38);
	MoveControl(&m_checkExcludeZero, 16, 60);
	MoveControl(&m_staticAccessType, 16, 85);
	MoveControl(&m_comboAccessType, 150, 83);
	MoveControl(&m_staticCompression, 16, 105);
	MoveControl(&m_comboCompression, 150, 103);

	m_TimePickerDaily.SetFormat("HH:mm");
	m_TimePickerWeekly.SetFormat("HH:mm");

	for (int n = 0; n <= 55; n += 5)
	{
		CString str;
		str.Format("%d", n);
		m_comboMinuteHourly.AddString(str);
		m_comboMinuteCustom.AddString(str);
	}

	m_comboWeekday.AddString("Sunday");
	m_comboWeekday.AddString("Monday");
	m_comboWeekday.AddString("Tuesday");
	m_comboWeekday.AddString("Wednesday");
	m_comboWeekday.AddString("Thursday");
	m_comboWeekday.AddString("Friday");
	m_comboWeekday.AddString("Saturday");

	m_comboPrevious.AddString("All backups");
	m_comboPrevious.AddString("Previous 10 backups");
	m_comboPrevious.AddString("Previous 9 backups");
	m_comboPrevious.AddString("Previous 8 backups");
	m_comboPrevious.AddString("Previous 7 backups");
	m_comboPrevious.AddString("Previous 6 backups");
	m_comboPrevious.AddString("Previous 5 backups");
	m_comboPrevious.AddString("Previous 4 backups");
	m_comboPrevious.AddString("Previous 3 backups");
	m_comboPrevious.AddString("Previous 2 backups");

	m_comboPrompt.AddString(" -- --");
	m_comboPrompt.AddString("After 1 hour");
	m_comboPrompt.AddString("After 2 hours");
	m_comboPrompt.AddString("After 3 hours");
	m_comboPrompt.AddString("After 4 hours");
	m_comboPrompt.AddString("After 5 hours");
	m_comboPrompt.AddString("After 6 hours");
	m_comboPrompt.AddString("After 12 hours");
	m_comboPrompt.AddString("After 18 hours");
	m_comboPrompt.AddString("After 1 day");
	m_comboPrompt.AddString("After 2 days");
	m_comboPrompt.AddString("After 3 days");
	m_comboPrompt.AddString("After 4 days");
	m_comboPrompt.AddString("After 5 days");
	m_comboPrompt.AddString("After 6 days");
	m_comboPrompt.AddString("After 7 days");

	m_comboPrompt2.AddString(" -- --");
	m_comboPrompt2.AddString("5 minutes");
	m_comboPrompt2.AddString("10 minutes");
	m_comboPrompt2.AddString("15 minutes");
	m_comboPrompt2.AddString("20 minutes");
	m_comboPrompt2.AddString("30 minutes");
	m_comboPrompt2.AddString("45 minutes");
	m_comboPrompt2.AddString("60 minutes");

	if (Sysset.GetMaxUsers() < 2)
	{
		m_staticWarning.ShowWindow(SW_HIDE);
	}

	m_comboAccessType.AddString("All backup options");
	m_comboAccessType.AddString("No auto backup");
	m_comboAccessType.AddString("Restore only");
	m_comboAccessType.AddString("None");

	m_comboCompression.AddString("0  No Compression");
	m_comboCompression.AddString("1");
	m_comboCompression.AddString("2");
	m_comboCompression.AddString("3");
	m_comboCompression.AddString("4");
	m_comboCompression.AddString("5");
	m_comboCompression.AddString("6  Default");
	m_comboCompression.AddString("7");
	m_comboCompression.AddString("8");
	m_comboCompression.AddString("9  Max Compression");

	SetRecordControls();

	m_nActiveTab = 0;
	SetTabControls(FALSE);

	return TRUE;
}

/**********************************************************************/

void CPropPageOptionsBackup::OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	m_nActiveTab = m_TabActions.GetCurSel();
	if ((m_nActiveTab < 0) || (m_nActiveTab > 4))
	{
		m_nActiveTab = 0;
	}

	SetTabControls( FALSE );
}

/**********************************************************************/

void CPropPageOptionsBackup::SetActiveTabControls()
{
	SetTabControls( TRUE );
};

/**********************************************************************/

void CPropPageOptionsBackup::SetTabControls(bool bActiveTabOnly)
{
	if ((FALSE == bActiveTabOnly) || (0 == m_nActiveTab))
	{
		ShowAndEnableWindow(&m_radioNone, (0 == m_nActiveTab));
		ShowAndEnableWindow(&m_radioHourly, (0 == m_nActiveTab));
		ShowAndEnableWindow(&m_radioDaily, (0 == m_nActiveTab));
		ShowAndEnableWindow(&m_radioWeekly, (0 == m_nActiveTab));
		ShowAndEnableWindow(&m_radioCustom, (0 == m_nActiveTab));
		ShowAndEnableWindow(&m_comboMinuteHourly, (0 == m_nActiveTab), (1 == m_nType));
		ShowAndEnableWindow(&m_staticSchedule1, (0 == m_nActiveTab));
		ShowAndEnableWindow(&m_TimePickerDaily, (0 == m_nActiveTab), (2 == m_nType));
		ShowAndEnableWindow(&m_TimePickerWeekly, (0 == m_nActiveTab), (3 == m_nType));
		ShowAndEnableWindow(&m_comboWeekday, (0 == m_nActiveTab), (3 == m_nType));
		ShowAndEnableWindow(&m_comboMinuteCustom, (0 == m_nActiveTab), (4 == m_nType));
		ShowAndEnableWindow(&m_staticSchedule2, (0 == m_nActiveTab));
		ShowAndEnableWindow(&m_buttonInfo, (0 == m_nActiveTab));
		ShowAndEnableWindow(&m_buttonSchedule, (0 == m_nActiveTab), (4 == m_nType));
	}

	if ((FALSE == bActiveTabOnly) || (1 == m_nActiveTab))
	{
		bool bPrompt = (m_checkPrompt.GetCheck() != 0);
		ShowAndEnableWindow(&m_staticOptions1, (1 == m_nActiveTab));
		ShowAndEnableWindow(&m_editFolder, (1 == m_nActiveTab), FALSE);
		ShowAndEnableWindow(&m_buttonBrowse, (1 == m_nActiveTab));
		ShowAndEnableWindow(&m_buttonEdit, (1 == m_nActiveTab));
		ShowAndEnableWindow(&m_buttonCheck, (1 == m_nActiveTab));
		ShowAndEnableWindow(&m_staticOptions2, (1 == m_nActiveTab));
		ShowAndEnableWindow(&m_comboPrevious, (1 == m_nActiveTab));
		ShowAndEnableWindow(&m_checkAlwaysPrompt, (1 == m_nActiveTab));
		ShowAndEnableWindow(&m_checkPrompt, (1 == m_nActiveTab));
		ShowAndEnableWindow(&m_comboPrompt, (1 == m_nActiveTab), (TRUE == bPrompt));
		ShowAndEnableWindow(&m_staticOptions3, (1 == m_nActiveTab));
		ShowAndEnableWindow(&m_comboPrompt2, (1 == m_nActiveTab), (TRUE == bPrompt) && (m_nType != 0));
	}

	if ((FALSE == bActiveTabOnly) || (2 == m_nActiveTab))
	{
		ShowAndEnableWindow(&m_checkExcludeSales, (2 == m_nActiveTab));
		ShowAndEnableWindow(&m_checkExcludeZero, (2 == m_nActiveTab));
		ShowAndEnableWindow(&m_staticAccessType, (2 == m_nActiveTab));
		ShowAndEnableWindow(&m_comboAccessType, (2 == m_nActiveTab));
		ShowAndEnableWindow(&m_staticCompression, (2 == m_nActiveTab));
		ShowAndEnableWindow(&m_comboCompression, (2 == m_nActiveTab));
	}
}

/**********************************************************************/

BOOL CPropPageOptionsBackup::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageOptionsBackup::OnSetActive() 
{
	SetRecordControls();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPageOptionsBackup::SetRecordControls()
{
	switch( AutoBackupOptions.GetAutoBackupType() )
	{
	case AUTOBACKUP_TYPE_CUSTOM:		OnRadioCustom();	break;
	case AUTOBACKUP_TYPE_WEEKLY:		OnRadioWeekly();	break;
	case AUTOBACKUP_TYPE_DAILY:			OnRadioDaily();		break;
	case AUTOBACKUP_TYPE_HOURLY:		OnRadioHourly();	break;
	case AUTOBACKUP_TYPE_NONE:
	default:							OnRadioNone();		break;
	}

	m_checkAlwaysPrompt.SetCheck( AutoBackupOptions.GetAlwaysPromptFlag() );

	//HOURLY MINUTES
	int nSel = AutoBackupOptions.GetHourlyMinute() / 5;
	if ( ( nSel >=0 ) && ( nSel < m_comboMinuteHourly.GetCount() ) )
		m_comboMinuteHourly.SetCurSel( nSel );
	else
		m_comboMinuteHourly.SetCurSel( 0 );

	//DAILY TIME
	int nHour = AutoBackupOptions.GetDailyHour();
	if ( ( nHour < 0 ) || ( nHour > 23 ) ) nHour = 0;
	
	int nMinute = AutoBackupOptions.GetDailyMinute();
	if ( ( nMinute < 0 ) || ( nMinute > 59 ) ) nMinute = 0;

	COleDateTime timeDaily = COleDateTime( 2008, 1, 1, nHour, nMinute, 0 );
	m_TimePickerDaily.SetTime( timeDaily );

	//WEEKLY TIME
	nHour = AutoBackupOptions.GetWeeklyHour();
	if ( ( nHour < 0 ) || ( nHour > 23 ) ) nHour = 0;
	
	nMinute = AutoBackupOptions.GetWeeklyMinute();
	if ( ( nMinute < 0 ) || ( nMinute > 59 ) ) nMinute = 0;

	COleDateTime timeWeekly = COleDateTime( 2008, 1, 1, nHour, nMinute, 0 );
	m_TimePickerWeekly.SetTime( timeWeekly );

	//WEEKLY WEEKDAY
	int nWeekday = AutoBackupOptions.GetWeeklyDay();
	if ( ( nWeekday < 0 ) || ( nWeekday > 6 ) )
		nWeekday = 0;

	m_comboWeekday.SetCurSel( nWeekday );

	//CUSTOM MINUTES
	nSel = AutoBackupOptions.GetCustomMinute() / 5;
	if ( ( nSel >=0 ) && ( nSel < m_comboMinuteCustom.GetCount() ) )
		m_comboMinuteCustom.SetCurSel( nSel );
	else
		m_comboMinuteCustom.SetCurSel( 0 );

	int nHours = AutoBackupOptions.GetHoursBeforePrompt();
	int nMinutes = AutoBackupOptions.GetMinutesBeforeOverdue();
	bool bPrompt = ( ( nHours != 0 ) || ( nMinutes != 0) );
	m_checkPrompt.SetCheck( bPrompt );
	OnTogglePrompt();
	
	//PROMPT
	switch( nHours )
	{
	case 1:		m_comboPrompt.SetCurSel(1);		break;
	case 2:		m_comboPrompt.SetCurSel(2);		break;
	case 3:		m_comboPrompt.SetCurSel(3);		break;
	case 4:		m_comboPrompt.SetCurSel(4);		break;
	case 5:		m_comboPrompt.SetCurSel(5);		break;
	case 6:		m_comboPrompt.SetCurSel(6);		break;
	case 12:	m_comboPrompt.SetCurSel(7);		break;
	case 18:	m_comboPrompt.SetCurSel(8);		break;
	case 24:	m_comboPrompt.SetCurSel(9);		break;
	case 48:	m_comboPrompt.SetCurSel(10);	break;
	case 72:	m_comboPrompt.SetCurSel(11);	break;
	case 96:	m_comboPrompt.SetCurSel(12);	break;
	case 120:	m_comboPrompt.SetCurSel(13);	break;
	case 144:	m_comboPrompt.SetCurSel(14);	break;
	case 168:	m_comboPrompt.SetCurSel(15);	break;
	default:
		m_comboPrompt.SetCurSel(0);
		break;
	}

	//PROMPT2
	switch( nMinutes )
	{
	case 5:		m_comboPrompt2.SetCurSel(1);		break;
	case 10:	m_comboPrompt2.SetCurSel(2);		break;
	case 15:	m_comboPrompt2.SetCurSel(3);		break;
	case 20:	m_comboPrompt2.SetCurSel(4);		break;
	case 30:	m_comboPrompt2.SetCurSel(5);		break;
	case 45:	m_comboPrompt2.SetCurSel(6);		break;
	case 60:	m_comboPrompt2.SetCurSel(7);		break;
	default:
		m_comboPrompt2.SetCurSel(0);
		break;
	}

	//PREVIOUS
	int nCount = AutoBackupOptions.GetArchiveCount();
	if ((nCount >= 2) && (nCount <= 10))
	{
		m_comboPrevious.SetCurSel(11 - nCount);
	}
	else
	{
		m_comboPrevious.SetCurSel(0);
	}

	//FOLDER
	m_editFolder.SetWindowText(AutoBackupOptions.GetPath());

	//DAILY SALE FLAG
	m_checkExcludeSales.SetCheck( AutoBackupOptions.GetExcludeDailySalesFlag() );
	m_checkExcludeZero.SetCheck( AutoBackupOptions.GetExcludeZeroSizeFilesFlag() );

	//ACCESS TYPE
	{
		int nType = PCOptionsHost.GetBackupAccessType();

		if ((nType < 0) || (nType > 3))
		{
			nType = 0;
		}

		m_comboAccessType.SetCurSel(nType);
	}

	//COMPRESSION
	{
		int nCompression = AutoBackupOptions.GetCompressionLevel();
		if ((nCompression < 0) || (nCompression > 9))
		{
			nCompression = 6;
		}
		m_comboCompression.SetCurSel(nCompression);
	}
}

/**********************************************************************/

void CPropPageOptionsBackup::SaveRecord()
{
	AutoBackupOptions.SetAutoBackupType( m_nType );
	AutoBackupOptions.SetHourlyMinute( m_comboMinuteHourly.GetCurSel() * 5 );

	COleDateTime timeDaily;
	m_TimePickerDaily.GetTime( timeDaily );
	AutoBackupOptions.SetDailyHour( timeDaily.GetHour() );
	AutoBackupOptions.SetDailyMinute( timeDaily.GetMinute() );

	COleDateTime timeWeekly;
	m_TimePickerWeekly.GetTime( timeWeekly );
	AutoBackupOptions.SetWeeklyHour( timeWeekly.GetHour() );
	AutoBackupOptions.SetWeeklyMinute( timeWeekly.GetMinute() );

	AutoBackupOptions.SetWeeklyDay( m_comboWeekday.GetCurSel() );

	AutoBackupOptions.SetCustomMinute( m_comboMinuteCustom.GetCurSel() * 5 );

	AutoBackupOptions.SetAlwaysPromptFlag( m_checkAlwaysPrompt.GetCheck() != 0 );

	int nHoursBeforePrompt = 0;
	if ( m_checkPrompt.GetCheck() == TRUE )
	{
		switch( m_comboPrompt.GetCurSel() )
		{
		case 1:		nHoursBeforePrompt = 1;		break;
		case 2:		nHoursBeforePrompt = 2;		break;
		case 3:		nHoursBeforePrompt = 3;		break;
		case 4:		nHoursBeforePrompt = 4;		break;
		case 5:		nHoursBeforePrompt = 5;		break;
		case 6:		nHoursBeforePrompt = 6;		break;
		case 7:		nHoursBeforePrompt = 12;	break;
		case 8:		nHoursBeforePrompt = 18;	break;
		case 9:		nHoursBeforePrompt = 24;	break;
		case 10:	nHoursBeforePrompt = 48;	break;
		case 11:	nHoursBeforePrompt = 72;	break;
		case 12:	nHoursBeforePrompt = 96;	break;
		case 13:	nHoursBeforePrompt = 120;	break;
		case 14:	nHoursBeforePrompt = 144;	break;
		case 15:	nHoursBeforePrompt = 168;	break;
		}
	}
	AutoBackupOptions.SetHoursBeforePrompt( nHoursBeforePrompt );

	int nMinutesBeforeOverdue = 0;
	if ( m_checkPrompt.GetCheck() == TRUE )
	{
		switch( m_comboPrompt2.GetCurSel() )
		{
		case 1:		nMinutesBeforeOverdue = 5;		break;
		case 2:		nMinutesBeforeOverdue = 10;		break;
		case 3:		nMinutesBeforeOverdue = 15;		break;
		case 4:		nMinutesBeforeOverdue = 20;		break;
		case 5:		nMinutesBeforeOverdue = 30;		break;
		case 6:		nMinutesBeforeOverdue = 45;		break;
		case 7:		nMinutesBeforeOverdue = 60;		break;
		}
	}
	AutoBackupOptions.SetMinutesBeforeOverdue( nMinutesBeforeOverdue );

	int nBackupsToKeep = 0;
	int nSel = m_comboPrevious.GetCurSel();
	if ( ( nSel >= 1 ) && ( nSel <= 9 ) )
		nBackupsToKeep = 11 - nSel;
	AutoBackupOptions.SetArchiveCount( nBackupsToKeep );

	AutoBackupOptions.SetPath( GetEditBoxText(m_editFolder) );

	AutoBackupOptions.SetExcludeDailySalesFlag( m_checkExcludeSales.GetCheck() != 0 );
	AutoBackupOptions.SetExcludeZeroSizeFilesFlag( m_checkExcludeZero.GetCheck() != 0 );
	AutoBackupOptions.SetCompressionLevel(m_comboCompression.GetCurSel());

	PCOptionsHost.SetBackupAccessType(m_comboAccessType.GetCurSel());
}

/**********************************************************************/
void CPropPageOptionsBackup::OnRadioNone(){ SetType(0); }
void CPropPageOptionsBackup::OnRadioHourly(){ SetType(1); }
void CPropPageOptionsBackup::OnRadioDaily(){ SetType(2); }
void CPropPageOptionsBackup::OnRadioWeekly(){ SetType(3); }
void CPropPageOptionsBackup::OnRadioCustom(){ SetType(4); }
/**********************************************************************/

void CPropPageOptionsBackup::SetType( int n )
{
	m_nType = n;
	m_radioNone.SetCheck(m_nType == 0);
	m_radioHourly.SetCheck(m_nType == 1);
	m_radioDaily.SetCheck(m_nType == 2);
	m_radioWeekly.SetCheck(m_nType == 3);
	m_radioCustom.SetCheck(m_nType == 4);
	m_comboMinuteHourly.EnableWindow(m_nType == 1);
	m_TimePickerDaily.EnableWindow(m_nType == 2);
	m_TimePickerWeekly.EnableWindow(m_nType == 3);
	m_comboWeekday.EnableWindow(m_nType == 3);
	m_comboMinuteCustom.EnableWindow( m_nType == 4 );
	m_buttonInfo.EnableWindow( m_nType != 0 );
	m_buttonSchedule.EnableWindow( m_nType == 4 );
	m_buttonBrowse.EnableWindow( m_nType != 0 );
	m_buttonEdit.EnableWindow(m_nType != 0);
	m_buttonCheck.EnableWindow(m_nType != 0);
	m_comboPrevious.EnableWindow( m_nType != 0 );
	m_checkAlwaysPrompt.EnableWindow( m_nType != 0 );
	OnTogglePrompt();
}

/**********************************************************************/

void CPropPageOptionsBackup::OnTogglePrompt()
{
	bool bEnable = ( m_checkPrompt.GetCheck() != 0 );
	m_comboPrompt.EnableWindow( bEnable );
	m_comboPrompt2.EnableWindow( bEnable && ( m_nType != 0 ) );
}

/**********************************************************************/

void CPropPageOptionsBackup::OnButtonBrowse()
{
	CString strPathname = "";
	if ( BrowseFolder ( strPathname, "Select folder for automatic backups", NULL, this ) == TRUE )
	{
		m_editFolder.SetWindowText( strPathname );
	}
}

/**********************************************************************/

void CPropPageOptionsBackup::OnButtonEdit()
{
	CString strPath = GetEditBoxText(m_editFolder);
	CDefEditDlg dlg("Set folder for automatic backups", strPath, _MAX_PATH, "Path", this);

	if (dlg.DoModal() == IDOK)
	{
		m_editFolder.SetWindowText(dlg.m_strName);
		CheckBackupPath(FALSE);
	}
}

/**********************************************************************/

void CPropPageOptionsBackup::OnButtonCheck()
{
	CheckBackupPath(TRUE);
}

/**********************************************************************/

void CPropPageOptionsBackup::CheckBackupPath( bool bMessageIfOK )
{
	CString strBackupFolderPath = GetEditBoxText(m_editFolder);

	if (::ExistSubdirectory(strBackupFolderPath) == FALSE)
	{
		Prompter.Error("Unable to access backup folder.");
	}
	else
	{
		if (ValidateFolderAccess(strBackupFolderPath) == FALSE)
		{
			Prompter.Error("Unable to write to backup folder.");
		}
		else if ( TRUE == bMessageIfOK )
		{
			Prompter.Info("Backup folder exists and is accessible.");
		}
	}
}

/**********************************************************************/

bool CPropPageOptionsBackup::ValidateFolderAccess(const char* szPath)
{
	bool bReply = FALSE;
	
	CString strFilename = "";
	strFilename.Format("%s\\test.dat", szPath );			
	
	CSSFile file;
	if ( file.Open(strFilename, "wb") == TRUE)
	{
		file.Close();
		CFileRemover FileRemover(strFilename);
		bReply = TRUE;
	}
	return bReply;
}

/**********************************************************************/

void CPropPageOptionsBackup::OnButtonSchedule()
{
	int nMode = AutoBackupOptions.GetCustomMode();
	int nMonday = AutoBackupOptions.GetMondayMask();
	int nTuesday = AutoBackupOptions.GetTuesdayMask();
	int nWednesday = AutoBackupOptions.GetWednesdayMask();
	int nThursday = AutoBackupOptions.GetThursdayMask();
	int nFriday = AutoBackupOptions.GetFridayMask();
	int nSaturday = AutoBackupOptions.GetSaturdayMask();
	int nSunday = AutoBackupOptions.GetSundayMask();
	int nMonFri = AutoBackupOptions.GetMonFriMask();
	int nMonSat = AutoBackupOptions.GetMonSatMask();
	int nEveryday = AutoBackupOptions.GetEverydayMask();

	int nMinutes = m_comboMinuteCustom.GetCurSel() * 5;

	CPropertySheetBackup propSheet( nMinutes, this );
	if ( propSheet.DoModal() != IDOK )
	{
		AutoBackupOptions.SetCustomMode( nMode );
		AutoBackupOptions.SetMondayMask( nMonday );
		AutoBackupOptions.SetTuesdayMask( nTuesday );
		AutoBackupOptions.SetWednesdayMask( nWednesday );
		AutoBackupOptions.SetThursdayMask( nThursday );
		AutoBackupOptions.SetFridayMask( nFriday );
		AutoBackupOptions.SetSaturdayMask( nSaturday );
		AutoBackupOptions.SetSundayMask( nSunday );
		AutoBackupOptions.SetMonFriMask( nMonFri );
		AutoBackupOptions.SetMonSatMask( nMonSat );
		AutoBackupOptions.SetEverydayMask( nEveryday );
	}
}

/**********************************************************************/

void CPropPageOptionsBackup::OnButtonInfo()
{
	SaveRecord();
	CBackupInfoDlg dlg( this );
	dlg.DoModal();
}

/**********************************************************************/
