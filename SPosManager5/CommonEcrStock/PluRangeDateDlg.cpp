/**********************************************************************/
 
/**********************************************************************/
#include "PluRangeDateDlg.h"
/**********************************************************************/

CPluRangeDateDlg::CPluRangeDateDlg( const char* szDate, CWnd* pParent)
	: CSSDialog(CPluRangeDateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPluRangeDateDlg)
	//}}AFX_DATA_INIT
	m_strDate = szDate;
}

/**********************************************************************/

void CPluRangeDateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluRangeDateDlg)
	DDX_Control(pDX, IDC_DATEPICKER, m_DatePicker);
	DDX_Control(pDX, IDC_RADIO_NOCHANGE, m_radioNoChange);
	DDX_Control(pDX, IDC_RADIO_DATE, m_radioDate);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluRangeDateDlg, CDialog)
	//{{AFX_MSG_MAP(CPluRangeDateDlg)
	ON_BN_CLICKED(IDC_RADIO_NOCHANGE, OnRadioNoChange)
	ON_BN_CLICKED(IDC_RADIO_DATE, OnRadioDate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluRangeDateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	COleDateTime start =	COleDateTime( 2016, 1, 1, 0, 0, 0 );
	COleDateTime end =		COleDateTime( 2099, 1, 1, 0, 0, 0 );
	m_DatePicker.SetRange	( &start, &end );
	m_DatePicker.SetFormat	( "ddd dd MMM yyy" );
	
	bool bGotDate = FALSE;
	if ( m_strDate.GetLength() == 10 )
	{
		COleDateTime date = COleDateTime( atoi(m_strDate.Right(4)), atoi(m_strDate.Mid(3,2)), atoi(m_strDate.Left(2)), 0, 0, 0 );
		
		if ( date.GetStatus() == COleDateTime::valid )
		{
			bGotDate = TRUE;
			m_DatePicker.SetTime( date );
		}
	}

	if ( TRUE == bGotDate )
		OnRadioDate();
	else
	{
		COleDateTime date = COleDateTime::GetCurrentTime();
		m_DatePicker.SetTime( date );
		OnRadioNoChange();
	}

	return TRUE;  
}

/**********************************************************************/

void CPluRangeDateDlg::OnRadioNoChange() 
{
	m_radioNoChange.SetCheck( TRUE );
	m_radioDate.SetCheck( FALSE );
	m_DatePicker.EnableWindow( FALSE );
}

/**********************************************************************/

void CPluRangeDateDlg::OnRadioDate() 
{
	m_radioNoChange.SetCheck( FALSE );
	m_radioDate.SetCheck( TRUE );
	m_DatePicker.EnableWindow( TRUE );
}

/**********************************************************************/

void CPluRangeDateDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_strDate = "";

		if ( m_radioDate.GetCheck() != 0 )
		{
			COleDateTime time;
			m_DatePicker.GetTime ( time );

			if ( time.m_status == COleDateTime::valid )
				m_strDate.Format ( "%2.2d/%2.2d/%4.4d", time.GetDay(), time.GetMonth(), time.GetYear() );
		}

		EndDialog ( IDOK );
	}
}

/**********************************************************************/