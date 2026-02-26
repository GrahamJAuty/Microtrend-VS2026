/**********************************************************************/
 
/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "TerminalRangeDlg.h"
/**********************************************************************/

CTerminalRangeDlg::CTerminalRangeDlg( bool bRemove, CWnd* pParent )
	: CSSDialog(CTerminalRangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTerminalRangeDlg)
	//}}AFX_DATA_INIT
	m_bRemove = bRemove;
}

/**********************************************************************/

void CTerminalRangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTerminalRangeDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_FROM, m_staticFrom);
	DDX_Control(pDX, IDC_STATIC_TO, m_staticTo);
	DDX_Text(pDX, IDC_EDIT_FROM, m_nFrom );
	DDV_MinMaxInt( pDX, m_nFrom, 1, 999 );
	DDX_Text(pDX, IDC_EDIT_TO, m_nTo );
	DDV_MinMaxInt( pDX, m_nTo, 1, 999 );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CTerminalRangeDlg, CDialog)
	//{{AFX_MSG_MAP(CTerminalRangeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CTerminalRangeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	if (FALSE == m_bRemove)
	{
		SetWindowText("Add Terminals");
	}
	else
	{
		SetWindowText("Remove Terminals");
	}

	SubclassEdit ( IDC_EDIT_FROM, SS_NUM, 3, "%s" );
	SubclassEdit ( IDC_EDIT_TO, SS_NUM, 3, "%s" );

	m_staticFrom.SetWindowText( "From" );
	m_staticTo.SetWindowText( "To" );

	return TRUE;  
}

/**********************************************************************/
CEdit* CTerminalRangeDlg::GetEditFrom(){ return GetEdit( IDC_EDIT_FROM ); }
CEdit* CTerminalRangeDlg::GetEditTo(){ return GetEdit( IDC_EDIT_TO ); }
/**********************************************************************/

void CTerminalRangeDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )	
		EndDialog ( IDOK );
}

/**********************************************************************/

