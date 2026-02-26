/**********************************************************************/
 
/**********************************************************************/
#include "SmartEntertainmentNetworkDateDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CSmartEntertainmentNetworkDateDlg, CDialog)
/**********************************************************************/

CSmartEntertainmentNetworkDateDlg::CSmartEntertainmentNetworkDateDlg(  CSmartEntertainmentWebAPISalesDate& dates, CWnd* pParent)
	: CDialog(CSmartEntertainmentNetworkDateDlg::IDD, pParent), m_dates( dates )
{
}

/**********************************************************************/

CSmartEntertainmentNetworkDateDlg::~CSmartEntertainmentNetworkDateDlg()
{
}

/**********************************************************************/

void CSmartEntertainmentNetworkDateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATEPICKER, m_DatePicker);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSmartEntertainmentNetworkDateDlg, CDialog)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSmartEntertainmentNetworkDateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	COleDateTime timeFirstRead; 
	m_dates.GetNextReadDate( timeFirstRead );
	COleDateTime date = COleDateTime( timeFirstRead.GetYear(), timeFirstRead.GetMonth(), timeFirstRead.GetDay(), 0, 0, 0 );
	
	COleDateTime dateStart = COleDateTime( 2015, 1, 1, 0, 0, 0 );
	COleDateTime dateEnd = COleDateTime( 2500, 1, 1, 0, 0, 0 );
	m_DatePicker.SetRange ( &dateStart, &dateEnd );
	m_DatePicker.SetFormat( "ddd dd MMM yyy" );
	m_DatePicker.SetTime( date );

	return TRUE;
}

/**********************************************************************/

void CSmartEntertainmentNetworkDateDlg::OnOK()
{
	COleDateTime date;
	m_DatePicker.GetTime( date );

	COleDateTime dateTime = COleDateTime(
			date.GetYear(),
			date.GetMonth(),
			date.GetDay(),
			0, 0, 0 );
			
	m_dates.SetNextReadDate( dateTime );
	EndDialog( IDOK );
}

/**********************************************************************/
