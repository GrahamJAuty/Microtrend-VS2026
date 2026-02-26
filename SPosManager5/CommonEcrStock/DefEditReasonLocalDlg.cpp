/**********************************************************************/
#include "DefEditReasonLocalDlg.h"
/**********************************************************************/

CDefEditReasonLocalDlg::CDefEditReasonLocalDlg( int nIndex, int nLocalType, bool bAllowSpin, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent /*=NULL*/)
	: CSSDialog(CDefEditReasonLocalDlg::IDD, pParent), m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	//{{AFX_DATA_INIT(CDefEditReasonLocalDlg)
	//}}AFX_DATA_INIT
	m_nIndex = nIndex;
	m_bAllowSpin = bAllowSpin;

	switch( nLocalType )
	{
	case NODE_DATABASE:
		m_nLocalType = NODE_DATABASE;
		m_pReasonArray = &DataManagerNonDb.CCReasonDatabase;
		break;

	case NODE_LOCATION:
	default:
		m_nLocalType = NODE_LOCATION;
		m_pReasonArray = &DataManagerNonDb.CCReasonLocation;
		break;
	}
}

/**********************************************************************/

void CDefEditReasonLocalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditReasonLocalDlg)
	DDX_Control(pDX, IDC_EDIT_PARENT_TYPE, m_editParentType);
	DDX_Control(pDX, IDC_EDIT_PARENT_TEXT, m_editParentText);
	DDX_Control(pDX, IDC_CHECK_PARENT_MANUAL, m_checkParentManual);
	DDX_Control(pDX, IDC_STATIC_LOCAL, m_staticLocal);
	DDX_Control(pDX, IDC_CHECK_LOCAL, m_checkLocal);
	DDX_Control(pDX, IDC_EDIT_LOCAL_TEXT, m_editLocalText);
	DDX_Control(pDX, IDC_CHECK_LOCAL_MANUAL, m_checkLocalManual);
	DDX_Control(pDX, IDC_SPIN, m_buttonSpin);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditReasonLocalDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditReasonLocalDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	ON_BN_CLICKED(IDC_CHECK_LOCAL, OnToggleLocal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditReasonLocalDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if ( FALSE == m_bAllowSpin )
	{
		ShowAndEnableWindow( &m_buttonSpin, FALSE );
	}
	
	m_editLocalText.LimitText( CascadingReason::ReasonText.Max );

	switch( m_nLocalType )
	{
	case NODE_DATABASE:
		m_staticLocal.SetWindowText( "Database Reason Text" );
		m_checkLocal.SetWindowText( "Override text for this database" );
		break;

	case NODE_LOCATION:
	default:
		m_staticLocal.SetWindowText( "Location Reason Text" );
		m_checkLocal.SetWindowText( "Override text for this location" );
		break;
	}

	GetData();
	return TRUE;  
}

/**********************************************************************/

void CDefEditReasonLocalDlg::GetData()
{
	CCascadingReasonCSVRecord Reason;
	m_pReasonArray -> GetAt( m_nIndex, Reason );

	CString strParentType = "";
	switch( Reason.GetParentReasonConLevelExternal() )
	{
	case NODE_SYSTEM:	strParentType = "System";		break;
	case NODE_DATABASE:	strParentType = "Database";		break;
	}

	CString strName = "";

	if ( strParentType != "" )
	{
		m_editParentType.SetWindowText( strParentType );
		m_editParentText.SetWindowText( Reason.GetParentReasonText() );
		m_checkParentManual.SetCheck( Reason.GetParentManualEntryFlag() );
		m_checkLocal.EnableWindow( TRUE );
		
		if ( Reason.GetLocalReasonFlag() == TRUE )
		{
			m_checkLocal.SetCheck( TRUE );
			m_editLocalText.EnableWindow( TRUE );
			m_checkLocalManual.EnableWindow( TRUE );
			m_editLocalText.SetWindowText(Reason.GetLocalReasonText() );
			m_checkLocalManual.SetCheck( Reason.GetLocalManualEntryFlag() );
		}
		else
		{
			m_checkLocal.SetCheck( FALSE );
			m_editLocalText.EnableWindow( FALSE );
			m_checkLocalManual.EnableWindow( FALSE );
			m_editLocalText.SetWindowText( "" );
			m_checkLocalManual.SetCheck( FALSE );
		}
	}
	else
	{
		m_editParentType.SetWindowText( "None" );
		m_editParentText.SetWindowText( "" );
		m_checkParentManual.SetCheck( FALSE );
		m_checkLocal.EnableWindow( FALSE );
		m_checkLocal.SetCheck( TRUE );
		m_editLocalText.EnableWindow( TRUE );
		m_checkLocalManual.EnableWindow( TRUE );
		m_editLocalText.SetWindowText( Reason.GetLocalReasonText() );
		m_checkLocalManual.SetCheck( Reason.GetLocalManualEntryFlag() );
	}

	CString strTitle;
	strTitle.Format( "R%2.2d Reason %d", Reason.GetReasonNo(), Reason.GetReasonNo() );
	SetWindowText( strTitle );
}

/**********************************************************************/

void CDefEditReasonLocalDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if ( TRUE == m_bAllowSpin )
	{
		NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
		
		if ( pNMUpDown -> iDelta > 0 )
			SpinPrevious();
		else
			SpinNext();
	}

	*pResult = 0;
}

/**********************************************************************/

void CDefEditReasonLocalDlg::OnDefaultButton() 
{
	if ( TRUE == m_bAllowSpin )
	{
		SpinNext();
	}
}

/**********************************************************************/

void CDefEditReasonLocalDlg::SpinPrevious()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex > 0 )
		{
			SaveData();
			m_nIndex--;
			GetData();

			if ( m_pListDataDlg != NULL )
				m_pListDataDlg -> SelectLine ( m_nIndex );
		}
	}
}

/**********************************************************************/

void CDefEditReasonLocalDlg::SpinNext()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex < m_pReasonArray -> GetSize() - 1 )
		{
			SaveData();
			m_nIndex++;
			GetData();

			if ( m_pListDataDlg != NULL )
				m_pListDataDlg -> SelectLine( m_nIndex );
		}
	}
}

/**********************************************************************/

void CDefEditReasonLocalDlg::OnToggleLocal()
{
	bool bEnable = IsTicked( m_checkLocal );
	m_editLocalText.EnableWindow( bEnable );
	m_checkLocalManual.EnableWindow( bEnable );
}

/**********************************************************************/

void CDefEditReasonLocalDlg::SaveData()
{
	CCascadingReasonCSVRecord Reason;
	m_pReasonArray -> GetAt( m_nIndex, Reason );
	
	if ( IsTicked( m_checkLocal ) )
	{
		Reason.SetLocalReasonFlag( TRUE );
		Reason.SetLocalReasonText( GetEditBoxText( m_editLocalText ) );
		Reason.SetLocalManualEntryFlag( IsTicked(m_checkLocalManual) );
	}
	else
	{
		Reason.SetLocalReasonFlag( FALSE );
		Reason.SetLocalReasonText( "" );
		Reason.SetLocalManualEntryFlag( FALSE );
	}

	m_pReasonArray -> SetAt( m_nIndex, Reason );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CDefEditReasonLocalDlg::OnOK() 
{
	SaveData();
	EndDialog ( IDOK );
}

/**********************************************************************/
