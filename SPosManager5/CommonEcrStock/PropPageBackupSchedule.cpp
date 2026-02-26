/**********************************************************************/
#include "..\CommonEcrStockTray\AutoBackupOptions.h"
/**********************************************************************/
#include "PropPageBackupSchedule.h"
/**********************************************************************/

CPropPageBackupSchedule::CPropPageBackupSchedule() : CPropertyPage(CPropPageBackupSchedule::IDD)
{
	//{{AFX_DATA_INIT(CPropPageBackupSchedule)
	//}}AFX_DATA_INIT
	m_nDayFlags = 0;
	m_nMinutes = 0;
	m_wIDHour[0] = IDC_CHECK_HOUR0;
	m_wIDHour[1] = IDC_CHECK_HOUR1;
	m_wIDHour[2] = IDC_CHECK_HOUR2;
	m_wIDHour[3] = IDC_CHECK_HOUR3;
	m_wIDHour[4] = IDC_CHECK_HOUR4;
	m_wIDHour[5] = IDC_CHECK_HOUR5;
	m_wIDHour[6] = IDC_CHECK_HOUR6;
	m_wIDHour[7] = IDC_CHECK_HOUR7;
	m_wIDHour[8] = IDC_CHECK_HOUR8;
	m_wIDHour[9] = IDC_CHECK_HOUR9;
	m_wIDHour[10] = IDC_CHECK_HOUR10;
	m_wIDHour[11] = IDC_CHECK_HOUR11;
	m_wIDHour[12] = IDC_CHECK_HOUR12;
	m_wIDHour[13] = IDC_CHECK_HOUR13;
	m_wIDHour[14] = IDC_CHECK_HOUR14;
	m_wIDHour[15] = IDC_CHECK_HOUR15;
	m_wIDHour[16] = IDC_CHECK_HOUR16;
	m_wIDHour[17] = IDC_CHECK_HOUR17;
	m_wIDHour[18] = IDC_CHECK_HOUR18;
	m_wIDHour[19] = IDC_CHECK_HOUR19;
	m_wIDHour[20] = IDC_CHECK_HOUR20;
	m_wIDHour[21] = IDC_CHECK_HOUR21;
	m_wIDHour[22] = IDC_CHECK_HOUR22;
	m_wIDHour[23] = IDC_CHECK_HOUR23;
}

/**********************************************************************/

CPropPageBackupSchedule::~CPropPageBackupSchedule()
{
}

/**********************************************************************/

void CPropPageBackupSchedule::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageBackupSchedule)
	//}}AFX_DATA_MAP
	for ( int n = 0; n < 24; n++ )
		DDX_Control(pDX, m_wIDHour[n], m_checkHour[n] );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageBackupSchedule, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageBackupSchedule)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ALL, &CPropPageBackupSchedule::OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, &CPropPageBackupSchedule::OnButtonNone)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageBackupSchedule::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	for ( int n = 0; n < 24; n++ )
	{
		CString str;
		str.Format( "Backup at %2.2d:%2.2d", n, m_nMinutes );
		m_checkHour[n].SetWindowText( str );
	}

	return TRUE;  
}

/**********************************************************************/

BOOL CPropPageBackupSchedule::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageBackupSchedule::OnSetActive() 
{
	SetRecordControls();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPageBackupSchedule::SetRecordControls()
{
	int nMask = 0;
	switch( m_nDayFlags )
	{
	case DAYFLAGS_SUN:
		nMask = AutoBackupOptions.GetSundayMask();
		break;

	case DAYFLAGS_MON:
		nMask = AutoBackupOptions.GetMondayMask();
		break;

	case DAYFLAGS_TUE:
		nMask = AutoBackupOptions.GetTuesdayMask();
		break;

	case DAYFLAGS_WED:
		nMask = AutoBackupOptions.GetWednesdayMask();
		break;

	case DAYFLAGS_THU:
		nMask = AutoBackupOptions.GetThursdayMask();
		break;

	case DAYFLAGS_FRI:
		nMask = AutoBackupOptions.GetFridayMask();
		break;

	case DAYFLAGS_SAT:
		nMask = AutoBackupOptions.GetSaturdayMask();
		break;

	case DAYFLAGS_MON_FRI:
		nMask = AutoBackupOptions.GetMonFriMask();
		break;

	case DAYFLAGS_MON_SAT:
		nMask = AutoBackupOptions.GetMonSatMask();
		break;

	case DAYFLAGS_EVERYDAY:
		nMask = AutoBackupOptions.GetEverydayMask();
		break;
	}

	int x = 1;
	for ( int n = 0; n < 24; n++ )
	{
		m_checkHour[n].SetCheck( ( nMask & x ) == x );
		x *= 2;
	}
}

/**********************************************************************/

void CPropPageBackupSchedule::SaveRecord()
{
	int x = 1;
	int nMask = 0;
	for ( int n = 0; n < 24; n++ )
	{
		if ( m_checkHour[n].GetCheck() != 0 )
			nMask += x;
		
		x *= 2;
	}

	switch( m_nDayFlags )
	{
	case DAYFLAGS_SUN:
		AutoBackupOptions.SetSundayMask( nMask );
		break;

	case DAYFLAGS_MON:
		AutoBackupOptions.SetMondayMask( nMask );
		break;

	case DAYFLAGS_TUE:
		AutoBackupOptions.SetTuesdayMask( nMask );
		break;

	case DAYFLAGS_WED:
		AutoBackupOptions.SetWednesdayMask( nMask );
		break;

	case DAYFLAGS_THU:
		AutoBackupOptions.SetThursdayMask( nMask );
		break;

	case DAYFLAGS_FRI:
		AutoBackupOptions.SetFridayMask( nMask );
		break;

	case DAYFLAGS_SAT:
		AutoBackupOptions.SetSaturdayMask( nMask );
		break;

	case DAYFLAGS_MON_FRI:
		AutoBackupOptions.SetMonFriMask( nMask );
		break;

	case DAYFLAGS_MON_SAT:
		AutoBackupOptions.SetMonSatMask( nMask );
		break;

	case DAYFLAGS_EVERYDAY:
		AutoBackupOptions.SetEverydayMask( nMask );
		break;
	}
}

/**********************************************************************/

void CPropPageBackupSchedule::OnButtonAll()
{
	for ( int n = 0; n < 24; n++ )
		m_checkHour[n].SetCheck(TRUE);
}

/**********************************************************************/

void CPropPageBackupSchedule::OnButtonNone()
{
	for ( int n = 0; n < 24; n++ )
		m_checkHour[n].SetCheck(FALSE);
}

/**********************************************************************/
