/**********************************************************************/
#include "GlobalFunctions.h" 
#include "LocationCSVArray.h" 
/**********************************************************************/
#include "DefEditCustomerDateDlg.h"
/**********************************************************************/

CDefEditCustomerDateDlg::CDefEditCustomerDateDlg( int nLocIdx, CWnd* pParent)
	: CDialog(CDefEditCustomerDateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDefEditCustomerDateDlg)
	//}}AFX_DATA_INIT
	m_nLocIdx = nLocIdx;
	m_bRightClicked = FALSE;
	m_bAdminOptions = FALSE;
}

/**********************************************************************/

void CDefEditCustomerDateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditCustomerDateDlg)
	DDX_Control(pDX, IDC_BUTTON_ADMIN, m_buttonAdmin);
	DDX_Control(pDX, IDC_DATEPICKER_DATE, m_DatePickerDate);
	DDX_Control(pDX, IDC_DATEPICKER_TIME, m_DatePickerTime);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditCustomerDateDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditCustomerDateDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADMIN, OnButtonAdmin)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditCustomerDateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_buttonAdmin.ShowWindow( SW_HIDE );

	CString strTitle;
	strTitle.Format( "Set Customer Sales Start Date (%s)",
		dbLocation.GetName( m_nLocIdx ) );
	SetWindowText( strTitle );

	COleDateTime timeStart = COleDateTime( 2011, 1, 1, 0, 0, 0 );
	COleDateTime timeEnd = COleDateTime( 2050, 1, 1, 0, 0, 0 );
	m_DatePickerDate.SetRange ( &timeStart, &timeEnd );
	m_DatePickerDate.SetFormat( "ddd dd MMM yyy" );

	m_DatePickerTime.SetFormat ( "HH:mm" );
	
	GetData();

	return TRUE;  
}

/**********************************************************************/

void CDefEditCustomerDateDlg::GetData()
{
	CCustomerCSVRecord CustomerRecord;
	DataManagerNonDb.CustomerAccount.GetStartDateRecord( CustomerRecord );

	COleDateTime dateStart;
	GetOleDateSince2011( CustomerRecord.GetStartDay(), dateStart );
	m_DatePickerDate.SetTime( dateStart );

	COleDateTime timeStart = COleDateTime( 2013, 1, 1, CustomerRecord.GetStartHour(), CustomerRecord.GetStartMinute(), 0 );
	m_DatePickerTime.SetTime( timeStart );
}

/**********************************************************************/

void CDefEditCustomerDateDlg::SaveData()
{
	COleDateTime dateStart;
	m_DatePickerDate.GetTime( dateStart );

	CCustomerCSVRecord CustomerRecord;
	CustomerRecord.SetStartDay( GetDayNumberSince2011( dateStart ) );

	COleDateTime timeStart;
	m_DatePickerTime.GetTime( timeStart );
	CustomerRecord.SetStartHour( timeStart.GetHour() );
	CustomerRecord.SetStartMinute( timeStart.GetMinute() );
	
	DataManagerNonDb.CustomerAccount.SetStartDateRecord( CustomerRecord );
}

/**********************************************************************/

void CDefEditCustomerDateDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_bRightClicked = TRUE;
	CDialog::OnRButtonDown(nFlags, point);
}

/**********************************************************************/

void CDefEditCustomerDateDlg::OnButtonAdmin()
{
	if ( ( TRUE == m_bRightClicked ) && ( FALSE == m_bAdminOptions ) )
	{
		Prompter.Info( "Admin Options Enabled" );
		m_bAdminOptions = TRUE;
	}
}

/**********************************************************************/

void CDefEditCustomerDateDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveData();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
