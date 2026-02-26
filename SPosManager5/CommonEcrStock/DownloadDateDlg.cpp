/**********************************************************************/
 
/**********************************************************************/
//#include "EcrmanOptionsIni.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "Password.h"
/**********************************************************************/
#include "DownloadDateDlg.h"
/**********************************************************************/

CDownloadDateDlg::CDownloadDateDlg( const char* szTitle, CWnd* pParent)
	: CSSDialog(CDownloadDateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDownloadDateDlg)
	//}}AFX_DATA_INIT
	m_bImages = FALSE;
	m_nDownloadType = 0;
	m_bV3Keyboard = FALSE;
	m_strTitle = szTitle;
}

/**********************************************************************/

bool CDownloadDateDlg::RunIfNeeded()
{
	m_bImmediate = TRUE;
	m_strDate = "20160101";

	bool bResult = TRUE;
	if ( EcrmanOptions.GetFeaturesContextForceNowFlag() == FALSE )
		bResult = ( DoModal() == IDOK );
	
	return bResult;
}

/**********************************************************************/

void CDownloadDateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownloadDateDlg)
	DDX_Control(pDX, IDC_STATIC_GROUP, m_staticGroup);
	DDX_Control(pDX, IDC_CHECK_IMAGES, m_checkImages);
	DDX_Control(pDX, IDC_DATEPICKER, m_DatePicker);
	DDX_Radio(pDX, IDC_RADIO_IMMEDIATE, m_nDownloadType);
	DDX_Check(pDX, IDC_CHECK_IMAGES, m_bImages);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RADIO_IMMEDIATE, m_radioImmediate);
	DDX_Control(pDX, IDC_RADIO_DEFERRED, m_radioDeferred);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDownloadDateDlg, CDialog)
	//{{AFX_MSG_MAP(CDownloadDateDlg)
	ON_BN_CLICKED(IDC_RADIO_DEFERRED, OnRadioDeferred)
	ON_BN_CLICKED(IDC_RADIO_IMMEDIATE, OnRadioImmediate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDownloadDateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CTime start = CTime::GetCurrentTime();
	CTime end ( 2035, 1, 1, 0, 0, 0, -1 ); 
	m_DatePicker.SetRange ( &start, &end );
	CTime time = start + CTimeSpan ( 1, 0, 0, 0 );
	m_DatePicker.SetTime ( &time );
	m_DatePicker.SetFormat( "ddd dd MMM yyy" );
	
	SetWindowText( m_strTitle );
	OnRadioImmediate();

	if ( EcrmanOptions.GetFeaturesContextForceNowFlag() == TRUE )
	{
		m_radioImmediate.EnableWindow( FALSE );
		m_radioDeferred.EnableWindow( FALSE );
	}

	if ( m_bV3Keyboard == FALSE )
		m_checkImages.ShowWindow( SW_HIDE );
	else
		m_staticGroup.SetWindowText( "Download Settings" );

	return TRUE;  
}

/**********************************************************************/

void CDownloadDateDlg::OnRadioDeferred() 
{
	m_DatePicker.EnableWindow( TRUE );
}

/**********************************************************************/

void CDownloadDateDlg::OnRadioImmediate() 
{
	m_DatePicker.EnableWindow( FALSE );
}

/**********************************************************************/

void CDownloadDateDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_bImmediate = ( ( m_nDownloadType == 0 ) || ( EcrmanOptions.GetFeaturesContextForceNowFlag() == TRUE ) );

		CTime time;
		m_DatePicker.GetTime ( time );
		int nYear = time.GetYear();
		int nMonth = time.GetMonth();
		int nMDay = time.GetDay();
		m_strDate.Format ( "%4.4d%2.2d%2.2d", nYear, nMonth, nMDay );
			
		EndDialog ( IDOK );
	}
}

/**********************************************************************/


