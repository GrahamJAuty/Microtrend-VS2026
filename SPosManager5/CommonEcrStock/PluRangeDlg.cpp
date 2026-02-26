/**********************************************************************/
#include "PluCSVArray.h"
#include "BarcodePluNoTable.h"
/**********************************************************************/
#include "PluRangeDlg.h"
/**********************************************************************/

CPluRangeDlg::CPluRangeDlg( const char* szTitle, CWnd* pParent )
	: CSSDialog(CPluRangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPluRangeDlg)
	//}}AFX_DATA_INIT
	m_strTitle = szTitle;
}

/**********************************************************************/

void CPluRangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluRangeDlg)
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC_EDIT_PLUNO_FROM, m_strPluFrom);
	DDV_IntegerString(pDX, m_strPluFrom, "1", Super.PluEnd() );
	DDX_IntegerString(pDX, IDC_EDIT_PLUNO_TO, m_strPluTo );
	DDV_IntegerString(pDX, m_strPluTo, "1", Super.PluEnd() );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluRangeDlg, CDialog)
	//{{AFX_MSG_MAP(CPluRangeDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO_FROM, OnKillFocusPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO_TO, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC_STATIC_FROM, OnStaticFrom)
	ON_BN_CLICKED(IDC_STATIC_TO, OnStaticTo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPluRangeDlg::GetEditPluFrom() { return GetEdit( IDC_EDIT_PLUNO_FROM ); }
CEdit* CPluRangeDlg::GetEditPluTo() { return GetEdit( IDC_EDIT_PLUNO_TO ); }
/**********************************************************************/

BOOL CPluRangeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	SetWindowText ( m_strTitle );
	SubclassEdit ( IDC_EDIT_PLUNO_FROM, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLUNO_TO, SS_NUM, Super.MaxPluLen(), "%s" );
	return TRUE;  
}

/**********************************************************************/

void CPluRangeDlg::OnKillFocusPluNo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluFrom = BarcodePluNoTable.ConvertPlu ( m_strPluFrom );
	m_strPluTo = BarcodePluNoTable.ConvertPlu ( m_strPluTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPluRangeDlg::OnStaticFrom() 
{
	CString strStart = "1";
	::AddLeading ( strStart, Super.MaxPluLen(), '0' );
	GetEditPluFrom() -> SetWindowText ( strStart );
}

/**********************************************************************/

void CPluRangeDlg::OnStaticTo() 
{
	CString strEnd = "9";
	::AddLeading ( strEnd, Super.MaxPluLen(), '9' );
	GetEditPluTo() -> SetWindowText( strEnd );
}

/**********************************************************************/

void CPluRangeDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		OnKillFocusPluNo();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/

