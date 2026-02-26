/**********************************************************************/
#include "FilterTypes.h"
#include "..\SPosEcrManager5\PluFilterDlgEcrman.h"
#include "..\SPosStockManager5\PluFilterDlgStockman.h"
/**********************************************************************/
#include "PluNumberDlg.h"
/**********************************************************************/

CPluNumberDlg::CPluNumberDlg( const char* szTitle, bool bBrowse, CWnd* pParent )
	: CSSDialog(CPluNumberDlg::IDD, pParent), m_strTitle ( szTitle ), m_FilterArray ( FILTERTYPE_BROWSE_PLU )
{
	//{{AFX_DATA_INIT(CPluNumberDlg)
	//}}AFX_DATA_INIT
	m_strPluNumber = "";
	m_bBrowse = bBrowse;
}

/**********************************************************************/

void CPluNumberDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluNumberDlg)
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse);
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC_EDIT_PLUNO, m_strPluNumber);
	DDV_IntegerString(pDX, m_strPluNumber, "1", Super.PluEnd() );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluNumberDlg, CDialog)
	//{{AFX_MSG_MAP(CPluNumberDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluNumberDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowText ( m_strTitle );
	SubclassEdit ( IDC_EDIT_PLUNO, SS_NUM, Super.MaxPluLen(), "%s" );

	if ( m_bBrowse == FALSE )
		m_buttonBrowse.ShowWindow ( SW_HIDE );

	return TRUE;  
}

/**********************************************************************/

void CPluNumberDlg::OnButtonBrowse() 
{
	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg( m_FilterArray, this, nDlgResult, nPluIdx );

	if ( IDOK != nDlgResult )
		return;

	if ( nPluIdx < 0 || nPluIdx >= DataManager.Plu.GetSize() )
		return;

	m_strPluNumber = DataManager.Plu.GetPluNoString ( nPluIdx );
	UpdateData ( FALSE );
}

/**********************************************************************/

