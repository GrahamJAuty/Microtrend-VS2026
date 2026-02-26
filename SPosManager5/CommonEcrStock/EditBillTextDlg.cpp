/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "LocationCSVArray.h"
#include "MaxLengths.h"
/**********************************************************************/
#include "EditBillTextDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CEditBillTextDlg, CDialog)
/**********************************************************************/

CEditBillTextDlg::CEditBillTextDlg( CLocationSelectorEntity& entity, CBillAndReceiptTexts& Text, CWnd* pParent)
: CSSDialog(CEditBillTextDlg::IDD, pParent), m_LocSelEntity( entity ), m_Text( Text )
{
	if ( m_Text.GetParentConType() != NODE_PLACEHOLDER )
	{
		m_ParentText.SetFilename( m_Text.GetParentFilename() );
		m_ParentText.Read();
	}

	m_bUseParent = FALSE;
}

/**********************************************************************/

CEditBillTextDlg::~CEditBillTextDlg()
{
}

/**********************************************************************/

void CEditBillTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_STATUS, m_TabStatus );
	DDX_Control(pDX, IDC_EDIT_TEXT1, m_editText1 );
	DDX_Control(pDX, IDC_EDIT_TEXT2, m_editText2 );
	DDX_Control(pDX, IDC_EDIT_TEXT3, m_editText3 );
	DDX_Control(pDX, IDC_EDIT_TEXT4, m_editText4 );
	DDX_Control(pDX, IDC_EDIT_TEXT5, m_editText5 );
	DDX_Control(pDX, IDC_EDIT_TEXT6, m_editText6 );
	DDX_Control(pDX, IDC_STATIC_TEXT1, m_staticText1 );
	DDX_Control(pDX, IDC_STATIC_TEXT2, m_staticText2 );
	DDX_Control(pDX, IDC_STATIC_TEXT3, m_staticText3 );
	DDX_Control(pDX, IDC_STATIC_TEXT4, m_staticText4 );
	DDX_Control(pDX, IDC_STATIC_TEXT5, m_staticText5 );
	DDX_Control(pDX, IDC_STATIC_TEXT6, m_staticText6 );
	DDX_Control(pDX, IDC_CHECK_OVERRIDE, m_checkOverride );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CEditBillTextDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_STATUS, OnTcnSelchangeTabs)
	ON_BN_CLICKED( IDC_CHECK_OVERRIDE, OnToggleOverride)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CEditBillTextDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	{
		int nDbIdx = m_LocSelEntity.GetDbIdx();
		int nEntityIdx = m_LocSelEntity.GetEntityIdx();
		int nTermIdx = m_LocSelEntity.GetTermIdx();

		CString strExtra;
		switch( m_LocSelEntity.GetConType() )
		{
		case NODE_SYSTEM:
			strExtra = " (System)";
			break;

		case NODE_DATABASE:
			strExtra.Format( " (%s)", dbDatabase.GetName( nDbIdx ) );
			break;

		case NODE_LOCATION:
			strExtra.Format( " (%s)", dbLocation.GetName( nEntityIdx ) );
			break;

		case NODE_LOCATION_TERMINAL:
			strExtra.Format( " (%s, %s)", 
				dbLocation.GetName( nEntityIdx ),
				dbLocation.GetTerminalName( nEntityIdx, nTermIdx ) );
			break;
		}

		CString strTitle = "Edit Receipt and Bill Texts";
		SetWindowText( strTitle + strExtra );
	}

	m_TabStatus.InsertItem( 0, "Receipt Texts" );
	m_TabStatus.InsertItem( 1, "Bill Texts" );
	
	m_editText1.LimitText( MAX_LENGTH_BILL_TEXT );
	m_editText2.LimitText( MAX_LENGTH_BILL_TEXT );
	m_editText3.LimitText( MAX_LENGTH_BILL_TEXT );
	m_editText4.LimitText( MAX_LENGTH_BILL_TEXT );
	m_editText5.LimitText( MAX_LENGTH_BILL_TEXT );
	m_editText6.LimitText( MAX_LENGTH_BILL_TEXT );

	m_nTabPage = 0;
	
	if ( m_Text.GetParentConType() == NODE_PLACEHOLDER )
	{
		m_checkOverride.ShowWindow( SW_HIDE );
		m_checkOverride.EnableWindow( FALSE );
		m_bUseParent = FALSE;
	}
	else
	{
		switch( m_Text.GetParentConType() )
		{
		case NODE_SYSTEM:
			m_checkOverride.SetWindowText( "Override shared system texts" );
			break;

		case NODE_DATABASE:
			m_checkOverride.SetWindowText( "Override shared database texts" );
			break;

		case NODE_LOCATION:
			m_checkOverride.SetWindowText( "Override shared location texts" );
			break;
		}

		m_bUseParent = ( FALSE == m_Text.GetDataFileExistsFlag() );
		m_checkOverride.SetCheck( FALSE == m_bUseParent );

		MoveControl ( &m_checkOverride, 7, 10 );
		MoveControl ( &m_TabStatus, 7, 30 );

		MoveControl ( &m_staticText1, 23, 64 );
		MoveControl ( &m_staticText2, 23, 84 );
		MoveControl ( &m_staticText3, 23, 104 );
		MoveControl ( &m_staticText4, 23, 124 );
		MoveControl ( &m_staticText5, 23, 144 );
		MoveControl ( &m_staticText6, 23, 164 );

		MoveControl ( &m_editText1, 82, 63 );
		MoveControl ( &m_editText2, 82, 83 );
		MoveControl ( &m_editText3, 82, 103 );
		MoveControl ( &m_editText4, 82, 123 );
		MoveControl ( &m_editText5, 82, 143 );
		MoveControl ( &m_editText6, 82, 163 );
	}

	SelectTab();
	m_editText1.EnableWindow( FALSE == m_bUseParent );
	m_editText2.EnableWindow( FALSE == m_bUseParent );
	m_editText3.EnableWindow( FALSE == m_bUseParent );
	m_editText4.EnableWindow( FALSE == m_bUseParent );
	m_editText5.EnableWindow( FALSE == m_bUseParent );
	m_editText6.EnableWindow( FALSE == m_bUseParent );

	return TRUE;  
}

/**********************************************************************/

void CEditBillTextDlg::OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	SaveTabChanges();

	m_nTabPage = m_TabStatus.GetCurSel();
	if ( ( m_nTabPage < 0 ) || ( m_nTabPage > 1 ) )
		m_nTabPage = 0;

	SelectTab();
}

/**********************************************************************/

void CEditBillTextDlg::SelectTab()
{
	if ( 1 == m_nTabPage )
	{
		m_staticText1.SetWindowText( "Bill line 1" );
		m_staticText2.SetWindowText( "Bill line 2" );
		m_staticText3.SetWindowText( "Bill line 3" );
		m_staticText4.SetWindowText( "Bill line 4" );
		m_staticText5.SetWindowText( "Bill line 5" );
		m_staticText6.SetWindowText( "Bill line 6" );
		m_editText1.SetWindowText( m_bUseParent ? m_ParentText.GetBillText1() : m_Text.GetBillText1() );
		m_editText2.SetWindowText( m_bUseParent ? m_ParentText.GetBillText2() : m_Text.GetBillText2() );
		m_editText3.SetWindowText( m_bUseParent ? m_ParentText.GetBillText3() : m_Text.GetBillText3() );
		m_editText4.SetWindowText( m_bUseParent ? m_ParentText.GetBillText4() : m_Text.GetBillText4() );
		m_editText5.SetWindowText( m_bUseParent ? m_ParentText.GetBillText5() : m_Text.GetBillText5() );
		m_editText6.SetWindowText( m_bUseParent ? m_ParentText.GetBillText6() : m_Text.GetBillText6() );
	}
	else
	{
		m_staticText1.SetWindowText( "Receipt line 1" );
		m_staticText2.SetWindowText( "Receipt line 2" );
		m_staticText3.SetWindowText( "Receipt line 3" );
		m_staticText4.SetWindowText( "Receipt line 4" );
		m_staticText5.SetWindowText( "Receipt line 5" );
		m_staticText6.SetWindowText( "Receipt line 6" );
		m_editText1.SetWindowText( m_bUseParent ? m_ParentText.GetReceiptText1() : m_Text.GetReceiptText1() );
		m_editText2.SetWindowText( m_bUseParent ? m_ParentText.GetReceiptText2() : m_Text.GetReceiptText2() );
		m_editText3.SetWindowText( m_bUseParent ? m_ParentText.GetReceiptText3() : m_Text.GetReceiptText3() );
		m_editText4.SetWindowText( m_bUseParent ? m_ParentText.GetReceiptText4() : m_Text.GetReceiptText4() );
		m_editText5.SetWindowText( m_bUseParent ? m_ParentText.GetReceiptText5() : m_Text.GetReceiptText5() );
		m_editText6.SetWindowText( m_bUseParent ? m_ParentText.GetReceiptText6() : m_Text.GetReceiptText6() );
	}
}

/**********************************************************************/

void CEditBillTextDlg::SaveTabChanges()
{
	if ( FALSE == m_bUseParent )
	{
		CString strText1, strText2, strText3, strText4, strText5, strText6;
		m_editText1.GetWindowText( strText1 );
		m_editText2.GetWindowText( strText2 );
		m_editText3.GetWindowText( strText3 );
		m_editText4.GetWindowText( strText4 );
		m_editText5.GetWindowText( strText5 );
		m_editText6.GetWindowText( strText6 );

		if ( 1 == m_nTabPage )
		{	
			m_Text.SetBillText1( strText1 );
			m_Text.SetBillText2( strText2 );
			m_Text.SetBillText3( strText3 );
			m_Text.SetBillText4( strText4 );
			m_Text.SetBillText5( strText5 );
			m_Text.SetBillText6( strText6 );
		}
		else
		{
			m_Text.SetReceiptText1( strText1 );
			m_Text.SetReceiptText2( strText2 );
			m_Text.SetReceiptText3( strText3 );
			m_Text.SetReceiptText4( strText4 );
			m_Text.SetReceiptText5( strText5 );
			m_Text.SetReceiptText6( strText6 );
		}
	}
}

/**********************************************************************/

void CEditBillTextDlg::OnToggleOverride()
{
	if ( m_Text.GetParentConType() != NODE_PLACEHOLDER )
	{
		m_bUseParent = ( m_checkOverride.GetCheck() == 0 );

		if ( FALSE == m_bUseParent )
		{
			m_Text.SetBillText1( m_ParentText.GetBillText1() );
			m_Text.SetBillText2( m_ParentText.GetBillText2() );
			m_Text.SetBillText3( m_ParentText.GetBillText3() );
			m_Text.SetBillText4( m_ParentText.GetBillText4() );
			m_Text.SetBillText5( m_ParentText.GetBillText5() );
			m_Text.SetBillText6( m_ParentText.GetBillText6() );

			m_Text.SetReceiptText1( m_ParentText.GetReceiptText1() );
			m_Text.SetReceiptText2( m_ParentText.GetReceiptText2() );
			m_Text.SetReceiptText3( m_ParentText.GetReceiptText3() );
			m_Text.SetReceiptText4( m_ParentText.GetReceiptText4() );
			m_Text.SetReceiptText5( m_ParentText.GetReceiptText5() );
			m_Text.SetReceiptText6( m_ParentText.GetReceiptText6() );
		}

		SelectTab();
		m_editText1.EnableWindow( FALSE == m_bUseParent );
		m_editText2.EnableWindow( FALSE == m_bUseParent );
		m_editText3.EnableWindow( FALSE == m_bUseParent );
		m_editText4.EnableWindow( FALSE == m_bUseParent );
		m_editText5.EnableWindow( FALSE == m_bUseParent );
		m_editText6.EnableWindow( FALSE == m_bUseParent );
	}
}

/**********************************************************************/

void CEditBillTextDlg::OnOK()
{
	SaveTabChanges();
	EndDialog( IDOK );
}

/**********************************************************************/

void CEditBillTextDlg::OnCancel()
{
	EndDialog( IDCANCEL );
}

/**********************************************************************/

