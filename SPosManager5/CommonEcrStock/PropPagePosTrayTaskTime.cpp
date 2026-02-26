/**********************************************************************/
#include "PosTrayMonthDaysDlg.h"
#include "TimerNumbers.h"
/**********************************************************************/
#include "PropPagePosTrayTaskTime.h"
/**********************************************************************/

CPropPagePosTrayTaskTime::CPropPagePosTrayTaskTime() : CSSPropertyPage(CPropPagePosTrayTaskTime::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePosTrayTaskTime)
	//}}AFX_DATA_INIT
	m_hTimer = NULL;
	m_bTimerReady = FALSE;
	m_timeLastTaskCheck = COleDateTime( 1900, 1, 1, 0, 0, 0 );
	m_bGotNotBeforeTime = FALSE;
}

/**********************************************************************/

CPropPagePosTrayTaskTime::~CPropPagePosTrayTaskTime()
{
}

/**********************************************************************/

void CPropPagePosTrayTaskTime::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePosTrayTaskTime)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TIMEPICKER_FROM, m_TimePickerFrom);
	DDX_Control(pDX, IDC_TIMEPICKER_TO, m_TimePickerTo);
	DDX_Control(pDX, IDC_COMBO_INTERVAL, m_comboInterval);
	DDX_Control(pDX, IDC_CHECK_SUNDAY, m_checkSunday);
	DDX_Control(pDX, IDC_CHECK_MONDAY, m_checkMonday);
	DDX_Control(pDX, IDC_CHECK_TUESDAY, m_checkTuesday);
	DDX_Control(pDX, IDC_CHECK_WEDNESDAY, m_checkWednesday);
	DDX_Control(pDX, IDC_CHECK_THURSDAY, m_checkThursday);
	DDX_Control(pDX, IDC_CHECK_FRIDAY, m_checkFriday);
	DDX_Control(pDX, IDC_CHECK_SATURDAY, m_checkSaturday);
	DDX_Control(pDX, IDC_EDIT_MONTHDAY, m_editMonthDay);
	DDX_Control(pDX, IDC_DATEPICKER_NOTBEFORE, m_DatePickerNotBefore);
	DDX_Control(pDX, IDC_TIMEPICKER_NOTBEFORE, m_TimePickerNotBefore);
	DDX_Control(pDX, IDC_DATEPICKER_NEXT, m_DatePickerNext);
	DDX_Control(pDX, IDC_TIMEPICKER_NEXT, m_TimePickerNext);
	DDX_Control(pDX, IDC_EDIT_NEXT, m_editNext);
	DDX_Control(pDX, IDC_STATIC_RESTART, m_staticRestart);
	DDX_Control(pDX, IDC_COMBO_RESTART, m_comboRestart);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePosTrayTaskTime, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePosTrayTaskTime)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_BN_CLICKED( IDC_CHECK_SUNDAY, OnChangeSettings )
	ON_BN_CLICKED( IDC_CHECK_MONDAY, OnChangeSettings )
	ON_BN_CLICKED( IDC_CHECK_TUESDAY, OnChangeSettings )
	ON_BN_CLICKED( IDC_CHECK_WEDNESDAY, OnChangeSettings )
	ON_BN_CLICKED( IDC_CHECK_THURSDAY, OnChangeSettings )
	ON_BN_CLICKED( IDC_CHECK_FRIDAY, OnChangeSettings )
	ON_BN_CLICKED( IDC_CHECK_SATURDAY, OnChangeSettings )
	ON_BN_CLICKED(IDC_BUTTON_MONTHDAY, OnButtonMonthDay)
	ON_CBN_SELCHANGE( IDC_COMBO_INTERVAL, OnChangeSettings )
	ON_CBN_SELCHANGE( IDC_COMBO_RESTART, OnChangeSettings )
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TIMEPICKER_FROM, OnDateTimeChange)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TIMEPICKER_TO, OnDateTimeChange)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_NOTBEFORE, OnChangeLastTaskTime)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TIMEPICKER_NOTBEFORE, OnChangeLastTaskTime)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePosTrayTaskTime::OnInitDialog()
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_TimePickerFrom.SetFormat("HH:mm");
	m_TimePickerTo.SetFormat("HH:mm");
	m_TimePickerNotBefore.SetFormat("HH:mm");
	m_TimePickerNext.SetFormat("HH:mm");

	COleDateTime dateStart = COleDateTime(2013, 1, 1, 0, 0, 0);
	COleDateTime dateEnd = COleDateTime(2500, 1, 1, 0, 0, 0);

	m_DatePickerNotBefore.SetRange(&dateStart, &dateEnd);
	m_DatePickerNotBefore.SetFormat("ddd dd MMM yyy");
	m_DatePickerNext.SetRange(&dateStart, &dateEnd);
	m_DatePickerNext.SetFormat("ddd dd MMM yyy");

	m_DatePickerNext.EnableWindow(FALSE);
	m_TimePickerNext.EnableWindow(FALSE);

	m_editNext.SetWindowText("N/A : No weekdays selected");
	MoveControl(&m_editNext, 112, 192);

	{
		CString strLastTaskTime = m_pTask->GetLastRunTime(TRUE);

		if (strLastTaskTime == "")
			strLastTaskTime = m_pTask->GetLastRunTime(FALSE);

		m_bGotNotBeforeTime = FALSE;
		if (strLastTaskTime.GetLength() == 12)
		{
			m_oleNotBeforeTime = COleDateTime(
				atoi(strLastTaskTime.Left(4)),
				atoi(strLastTaskTime.Mid(4, 2)),
				atoi(strLastTaskTime.Mid(6, 2)),
				atoi(strLastTaskTime.Mid(8, 2)),
				atoi(strLastTaskTime.Right(2)),
				0);

			if (m_oleNotBeforeTime.m_status == COleDateTime::valid)
			{
				m_bGotNotBeforeTime = TRUE;
				m_oleNotBeforeTime += COleDateTimeSpan(0, 0, 1, 0);
			}
		}

		if (FALSE == m_bGotNotBeforeTime)
		{
			COleDateTime timeNow = COleDateTime::GetCurrentTime();
			m_oleNotBeforeTime = COleDateTime(
				timeNow.GetYear(),
				timeNow.GetMonth(),
				timeNow.GetDay(),
				0, 0, 0);
		}
	}

	if ((m_oleNotBeforeTime < dateStart) || (m_oleNotBeforeTime > dateEnd))
	{
		m_oleNotBeforeTime = dateStart;
	}

	m_bGotNotBeforeTime = TRUE;
	m_DatePickerNotBefore.SetTime(m_oleNotBeforeTime);
	m_TimePickerNotBefore.SetTime(m_oleNotBeforeTime);

	ShowNextTaskTime();

	{
		int nHour = m_pTask->GetTaskTimeHourStart();
		if ((nHour < 0) || (nHour > 23))
		{
			nHour = 0;
		}

		int nMinute = m_pTask->GetTaskTimeMinuteStart();
		if ((nMinute < 0) || (nMinute > 59))
		{
			nMinute = 0;
		}

		CTime time(1999, 1, 1, nHour, nMinute, 0, -1);
		m_TimePickerFrom.SetTime(&time);
	}

	{
		int nHour = m_pTask->GetTaskTimeHourEnd();
		if ((nHour < 0) || (nHour > 23))
		{
			nHour = 0;
		}

		int nMinute = m_pTask->GetTaskTimeMinuteEnd();
		if ((nMinute < 0) || (nMinute > 59))
		{
			nMinute = 0;
		}

		CTime time(1999, 1, 1, nHour, nMinute, 0, -1);
		m_TimePickerTo.SetTime(&time);
	}

	m_comboInterval.AddString("1 minute");
	m_comboInterval.AddString("5 minutes");
	m_comboInterval.AddString("10 minutes");
	m_comboInterval.AddString("15 minutes");
	m_comboInterval.AddString("20 minutes");
	m_comboInterval.AddString("30 minutes");
	m_comboInterval.AddString("60 minutes");

	switch (m_pTask->GetTaskTimeFrequency())
	{
	case 1:		m_comboInterval.SetCurSel(0);	break;
	case 5:		m_comboInterval.SetCurSel(1);	break;
	case 10:	m_comboInterval.SetCurSel(2);	break;
	case 15:	m_comboInterval.SetCurSel(3);	break;
	case 20:	m_comboInterval.SetCurSel(4);	break;
	case 30:	m_comboInterval.SetCurSel(5);	break;
	case 60:
	default:	m_comboInterval.SetCurSel(6);	break;
	}

	m_checkSunday.SetCheck(m_pTask->GetTaskDaySunFlag());
	m_checkMonday.SetCheck(m_pTask->GetTaskDayMonFlag());
	m_checkTuesday.SetCheck(m_pTask->GetTaskDayTueFlag());
	m_checkWednesday.SetCheck(m_pTask->GetTaskDayWedFlag());
	m_checkThursday.SetCheck(m_pTask->GetTaskDayThuFlag());
	m_checkFriday.SetCheck(m_pTask->GetTaskDayFriFlag());
	m_checkSaturday.SetCheck(m_pTask->GetTaskDaySatFlag());
	m_editMonthDay.SetWindowText(m_pTask->GetMonthDaysDisplayString());

	if ((m_pTask->IsExportTask() == TRUE) || (m_pTask->IsProcessTranTask() == TRUE))
	{
		ShowAndEnableWindow(&m_staticRestart, FALSE);
		ShowAndEnableWindow(&m_comboRestart, FALSE);
	}
	else
	{
		m_comboRestart.AddString("Perform task once for each missed day");
		m_comboRestart.AddString("Resume task schedule from current time");
		m_comboRestart.SetCurSel(m_pTask->GetTaskBacklogFlag() ? 0 : 1);
	}

	m_bTimerReady = TRUE;

	return TRUE;
}

/**********************************************************************/

void CPropPagePosTrayTaskTime::OnDateTimeChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	OnChangeSettings();
	*pResult = 0;
}

/**********************************************************************/

void CPropPagePosTrayTaskTime::OnChangeLastTaskTime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	UpdateNotBeforeTime();
	OnChangeSettings();
	*pResult = 0;
}

/**********************************************************************/

void CPropPagePosTrayTaskTime::UpdateNotBeforeTime()
{
	COleDateTime dateNotBefore;
	m_DatePickerNotBefore.GetTime( dateNotBefore );

	COleDateTime timeNotBefore;
	m_TimePickerNotBefore.GetTime( timeNotBefore );

	m_oleNotBeforeTime = COleDateTime(
		dateNotBefore.GetYear(),
		dateNotBefore.GetMonth(),
		dateNotBefore.GetDay(),
		timeNotBefore.GetHour(),
		timeNotBefore.GetMinute(),
		0 );
}

/**********************************************************************/

void CPropPagePosTrayTaskTime::OnButtonMonthDay()
{
	CPosTrayMonthDaysDlg dlg(this);
	dlg.m_nMonthDays = m_pTask->GetTaskMonthDays();

	if (dlg.DoModal() == IDOK)
	{
		m_pTask->SetTaskMonthDays(dlg.m_nMonthDays);
		m_editMonthDay.SetWindowText(m_pTask->GetMonthDaysDisplayString());
		OnChangeSettings();
	}
}

/**********************************************************************/

void CPropPagePosTrayTaskTime::OnChangeSettings()
{
	UpdateTask();
	ShowNextTaskTime();
}

/**********************************************************************/

void CPropPagePosTrayTaskTime::ShowNextTaskTime() 
{
	COleDateTime oleLastTaskTime = m_oleNotBeforeTime;
	oleLastTaskTime -= COleDateTimeSpan( 0, 0, 1, 0 );

	CString strLastRunDate;
	strLastRunDate.Format( "%4.4d%2.2d%2.2d%2.2d%2.2d",
		oleLastTaskTime.GetYear(),
		oleLastTaskTime.GetMonth(),
		oleLastTaskTime.GetDay(),
		oleLastTaskTime.GetHour(),
		oleLastTaskTime.GetMinute(),
		0 );
	
	COleDateTime dateRun;
	if ( m_pTask -> GetNextRunTime( dateRun, strLastRunDate ) == FALSE )
	{
		m_DatePickerNext.ShowWindow( SW_HIDE );
		m_TimePickerNext.ShowWindow( SW_HIDE );
		m_editNext.ShowWindow( SW_SHOW );
	}
	else
	{
		m_DatePickerNext.ShowWindow( SW_SHOW );
		m_TimePickerNext.ShowWindow( SW_SHOW );
		m_editNext.ShowWindow( SW_HIDE );
		m_DatePickerNext.SetTime( dateRun );
		m_TimePickerNext.SetTime( dateRun );
	}
}

/**********************************************************************/

BOOL CPropPagePosTrayTaskTime::OnKillActive() 
{
	if ( m_hTimer > 0 )
	{
		KillTimer ( m_hTimer );
		m_hTimer = 0;
	}

	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePosTrayTaskTime::OnSetActive() 
{
	if (m_hTimer == 0)
	{
		m_hTimer = SetTimer(TIMER_POSTRAYEDIT, 1000, NULL);
	}

	GetRecordData();
	return ( CPropertyPage::OnSetActive() != 0 );
}

/**********************************************************************/

void CPropPagePosTrayTaskTime::GetRecordData()
{
}

/**********************************************************************/

void CPropPagePosTrayTaskTime::UpdateTask()
{
	{
		CTime time;
		m_TimePickerFrom.GetTime(time);
		m_pTask->SetTaskTimeHourStart(time.GetHour());
		m_pTask->SetTaskTimeMinuteStart(time.GetMinute());
	}

	{
		CTime time;
		m_TimePickerTo.GetTime(time);
		m_pTask->SetTaskTimeHourEnd(time.GetHour());
		m_pTask->SetTaskTimeMinuteEnd(time.GetMinute());
	}

	switch (m_comboInterval.GetCurSel())
	{
	case 0:		m_pTask->SetTaskTimeFrequency(1);	break;
	case 1:		m_pTask->SetTaskTimeFrequency(5);	break;
	case 2:		m_pTask->SetTaskTimeFrequency(10);	break;
	case 3:		m_pTask->SetTaskTimeFrequency(15);	break;
	case 4:		m_pTask->SetTaskTimeFrequency(20);	break;
	case 5:		m_pTask->SetTaskTimeFrequency(30);	break;
	case 6:
	default:	m_pTask->SetTaskTimeFrequency(60);	break;
	}

	m_pTask->SetTaskDaySunFlag(m_checkSunday.GetCheck() != 0);
	m_pTask->SetTaskDayMonFlag(m_checkMonday.GetCheck() != 0);
	m_pTask->SetTaskDayTueFlag(m_checkTuesday.GetCheck() != 0);
	m_pTask->SetTaskDayWedFlag(m_checkWednesday.GetCheck() != 0);
	m_pTask->SetTaskDayThuFlag(m_checkThursday.GetCheck() != 0);
	m_pTask->SetTaskDayFriFlag(m_checkFriday.GetCheck() != 0);
	m_pTask->SetTaskDaySatFlag(m_checkSaturday.GetCheck() != 0);

	if (m_pTask->IsExportTask() == FALSE)
	{
		m_pTask->SetTaskBacklogFlag(m_comboRestart.GetCurSel() == 0);
	}
}

/**********************************************************************/

void CPropPagePosTrayTaskTime::OnTimer(UINT nIDEvent) 
{
	switch( nIDEvent )
	{
	case TIMER_POSTRAYEDIT:
		if ( TRUE == m_bTimerReady )
		{
			COleDateTime timeNow = COleDateTime::GetCurrentTime();
			timeNow -= COleDateTimeSpan( 0, 0, 0, timeNow.GetSecond() );

			if ( timeNow != m_timeLastTaskCheck )
			{
				m_bTimerReady = FALSE;
				m_timeLastTaskCheck = timeNow;
				UpdateTask();
				ShowNextTaskTime();
				m_bTimerReady = TRUE;
			}
		}

		break;
	}
}

/**********************************************************************/

void CPropPagePosTrayTaskTime::SaveRecord()
{
	UpdateNotBeforeTime();
	UpdateTask();
}

/**********************************************************************/

bool CPropPagePosTrayTaskTime::GetNotBeforeTime( COleDateTime& date)
{
	date = m_oleNotBeforeTime;
	return m_bGotNotBeforeTime;
}

/**********************************************************************/
