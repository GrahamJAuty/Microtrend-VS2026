/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "MaxLengths.h"
#include "PMSRoomCSVArray.h"
/**********************************************************************/
#include "DefEditRoomTypeDlg.h"
/**********************************************************************/

CDefEditRoomTypeDlg::CDefEditRoomTypeDlg( int nSleeps, CString& strLine, CWnd* pParent )
	: CSSDialog(CDefEditRoomTypeDlg::IDD, pParent), m_strLine( strLine )
{
	//{{AFX_DATA_INIT(CDefEditRoomTypeDlg)
	//}}AFX_DATA_INIT
	m_nSleeps = nSleeps;
}

/**********************************************************************/

void CDefEditRoomTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditRoomTypeDlg)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_CHECK_TYPE2, m_checkType2 );
	DDX_Control( pDX, IDC_CHECK_TYPE3, m_checkType3 );
	DDX_Control( pDX, IDC_CHECK_TYPE4, m_checkType4 );
	DDX_Control( pDX, IDC_CHECK_TYPE5, m_checkType5 );
	DDX_Control( pDX, IDC_CHECK_TYPE6, m_checkType6 );
	DDX_Control( pDX, IDC_EDIT_TYPE1, m_editType1 );
	DDX_Control( pDX, IDC_EDIT_TYPE2, m_editType2 );
	DDX_Control( pDX, IDC_EDIT_TYPE3, m_editType3 );
	DDX_Control( pDX, IDC_EDIT_TYPE4, m_editType4 );
	DDX_Control( pDX, IDC_EDIT_TYPE5, m_editType5 );
	DDX_Control( pDX, IDC_EDIT_TYPE6, m_editType6 );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditRoomTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditRoomTypeDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_TYPE2, OnCheckType2)
	ON_BN_CLICKED(IDC_CHECK_TYPE3, OnCheckType3)
	ON_BN_CLICKED(IDC_CHECK_TYPE4, OnCheckType4)
	ON_BN_CLICKED(IDC_CHECK_TYPE5, OnCheckType5)
	ON_BN_CLICKED(IDC_CHECK_TYPE6, OnCheckType6)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditRoomTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_editType1.LimitText( PMSRoom::RoomTypeText.Max );
	m_editType2.LimitText( PMSRoom::RoomTypeText.Max );
	m_editType3.LimitText( PMSRoom::RoomTypeText.Max );
	m_editType4.LimitText( PMSRoom::RoomTypeText.Max );
	m_editType5.LimitText( PMSRoom::RoomTypeText.Max );
	m_editType6.LimitText( PMSRoom::RoomTypeText.Max );

	CString strTitle;
	strTitle.Format( "Edit Room Type Texts (Sleeps %d)", m_nSleeps );
	SetWindowText( strTitle );

	CString strLine = DataManagerNonDb.PMSRoomType.GetRoomTypes( m_nSleeps );

	CCSV csvLine( m_strLine );

	m_editType1.SetWindowText( csvLine.GetString(0) );

	InitialiseType( csvLine, 2,		m_checkType2, m_editType2 );
	InitialiseType( csvLine, 4,		m_checkType3, m_editType3 );
	InitialiseType( csvLine, 6,		m_checkType4, m_editType4 );
	InitialiseType( csvLine, 8,		m_checkType5, m_editType5 );
	InitialiseType( csvLine, 10,	m_checkType6, m_editType6 );

	return TRUE;  
}

/**********************************************************************/

void CDefEditRoomTypeDlg::InitialiseType( CCSV& csvLine, int nOffset, CButton& check, CEdit& edit )
{
	bool bEnable = ( csvLine.GetBool( nOffset + 1 ) == TRUE );
	check.SetCheck( bEnable );
	edit.EnableWindow( bEnable );
	edit.SetWindowText( csvLine.GetString( nOffset ) );
}

/**********************************************************************/

void CDefEditRoomTypeDlg::AddToLine( CCSV& csvLine, bool bEnable, CEdit& edit, bool bForceText )
{
	CString str;
	edit.GetWindowText( str );
	::TrimSpaces( str, FALSE );

	if ( str == "" )
	{
		if ( TRUE == bForceText )
			str.Format( "Room for %d", m_nSleeps );
		else
			bEnable = FALSE;
	}

	csvLine.Add( str );
	csvLine.Add( bEnable );
}

/**********************************************************************/

void CDefEditRoomTypeDlg::OnCheckType2()
{
	m_editType2.EnableWindow( m_checkType2.GetCheck() != 0 );
}

/**********************************************************************/

void CDefEditRoomTypeDlg::OnCheckType3()
{
	m_editType3.EnableWindow( m_checkType3.GetCheck() != 0 );
}

/**********************************************************************/

void CDefEditRoomTypeDlg::OnCheckType4()
{
	m_editType4.EnableWindow( m_checkType4.GetCheck() != 0 );
}

/**********************************************************************/

void CDefEditRoomTypeDlg::OnCheckType5()
{
	m_editType5.EnableWindow( m_checkType5.GetCheck() != 0 );
}

/**********************************************************************/

void CDefEditRoomTypeDlg::OnCheckType6()
{
	m_editType6.EnableWindow( m_checkType6.GetCheck() != 0 );
}

/**********************************************************************/

void CDefEditRoomTypeDlg::OnOK()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CCSV csv;
		AddToLine( csv, TRUE, m_editType1, TRUE );
		AddToLine( csv, ( m_checkType2.GetCheck() != 0 ), m_editType2, FALSE );
		AddToLine( csv, ( m_checkType3.GetCheck() != 0 ), m_editType3, FALSE );
		AddToLine( csv, ( m_checkType4.GetCheck() != 0 ), m_editType4, FALSE );
		AddToLine( csv, ( m_checkType5.GetCheck() != 0 ), m_editType5, FALSE );
		AddToLine( csv, ( m_checkType6.GetCheck() != 0 ), m_editType6, FALSE );
		m_strLine = csv.GetLine();
		EndDialog( IDOK );
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
