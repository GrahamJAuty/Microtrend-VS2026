/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSBookEditExtendDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPMSBookEditExtendDlg, CDialog)
/**********************************************************************/

CPMSBookEditExtendDlg::CPMSBookEditExtendDlg( int nRoomIdx, CWnd* pParent )
	: CSSDialog(CPMSBookEditExtendDlg::IDD, pParent)
{
	DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, m_Room );
	m_nMaxNights = 14;
	m_nNights = 1;
}

/**********************************************************************/

CPMSBookEditExtendDlg::~CPMSBookEditExtendDlg()
{
}

/**********************************************************************/

void CPMSBookEditExtendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_STATIC_MAXNIGHTS, m_staticMaxNights );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSBookEditExtendDlg, CDialog)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPMSBookEditExtendDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strRoomName;
	::GetPMSRoomText( m_Room.GetRoomNo(), m_Room.GetRoomName(), strRoomName );

	CString strTitle;
	strTitle.Format( "Extend Booking (%s, %s)", 
		(const char*) strRoomName,
		m_Room.GetRoomTypeName(FALSE) );

	SetWindowText( strTitle );

	SubclassEdit( IDC_EDIT_NIGHTS, SS_NUM, 2, "%d" );

	GetEditNights() -> SetWindowText( "1" );

	CString strMax;
	strMax.Format( "(Max %d)", m_nMaxNights );
	m_staticMaxNights.SetWindowText( strMax );
	
	return TRUE;  
}

/**********************************************************************/
CEdit* CPMSBookEditExtendDlg::GetEditNights(){ return GetEdit( IDC_EDIT_NIGHTS ); }
/**********************************************************************/

void CPMSBookEditExtendDlg::OnOK()
{	
	CString strNights;
	GetEditNights() -> GetWindowText( strNights );
	m_nNights = atoi( strNights );

	if ( ( m_nNights < 1 ) || ( m_nNights > m_nMaxNights ) )
	{
		CString strError;
		strError.Format( "Please specify the number of nights to extend\nthis booking (max %d)", m_nMaxNights );
		Prompter.Error( strError );

		GetEditNights() -> SetFocus();
		GetEditNights() -> SetSel( 0, -1 );
		return;
	}
	
	EndDialog( IDOK );
}

/**********************************************************************/

void CPMSBookEditExtendDlg::OnCancel()
{
	EndDialog( IDCANCEL );
}

/**********************************************************************/
#endif
/**********************************************************************/
