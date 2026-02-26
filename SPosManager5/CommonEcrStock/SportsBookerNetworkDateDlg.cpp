/**********************************************************************/
 
/**********************************************************************/
#include "SportsBookerNetworkDateDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CSportsBookerNetworkDateDlg, CDialog)
/**********************************************************************/

CSportsBookerNetworkDateDlg::CSportsBookerNetworkDateDlg(  CSportsBookerWebAPISalesDateTime& dates, CWnd* pParent)
	: CDialog(CSportsBookerNetworkDateDlg::IDD, pParent), m_dates( dates )
{
}

/**********************************************************************/

CSportsBookerNetworkDateDlg::~CSportsBookerNetworkDateDlg()
{
}

/**********************************************************************/

void CSportsBookerNetworkDateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TIMEPICKER, m_TimePicker);
	DDX_Control(pDX, IDC_DATEPICKER, m_DatePicker);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSportsBookerNetworkDateDlg, CDialog)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSportsBookerNetworkDateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	COleDateTime timeFirstRead; 
	m_dates.GetNextReadDateTime( timeFirstRead );
	COleDateTime date = COleDateTime( timeFirstRead.GetYear(), timeFirstRead.GetMonth(), timeFirstRead.GetDay(), 0, 0, 0 );
	COleDateTime time = COleDateTime( 2015, 1, 1, timeFirstRead.GetHour(), timeFirstRead.GetMinute(), 0 );
	
	COleDateTime dateStart = COleDateTime( 2015, 1, 1, 0, 0, 0 );
	COleDateTime dateEnd = COleDateTime( 2500, 1, 1, 0, 0, 0 );
	m_DatePicker.SetRange ( &dateStart, &dateEnd );
	m_DatePicker.SetFormat( "ddd dd MMM yyy" );
	m_DatePicker.SetTime( date );

	m_TimePicker.SetFormat ( "HH:mm" );
	m_TimePicker.SetTime( time );

	return TRUE;
}

/**********************************************************************/

void CSportsBookerNetworkDateDlg::OnOK()
{
	COleDateTime date;
	m_DatePicker.GetTime( date );

	COleDateTime time;
	m_TimePicker.GetTime( time );

	COleDateTime dateTime = COleDateTime(
			date.GetYear(),
			date.GetMonth(),
			date.GetDay(),
			time.GetHour(),
			time.GetMinute(), 
			0 );

	m_dates.SetFirstReadDateTime( dateTime );
	EndDialog( IDOK );
}

/**********************************************************************/
