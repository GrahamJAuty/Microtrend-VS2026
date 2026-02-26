/**********************************************************************/
 
/**********************************************************************/
#include "ReportHelpers.h"
/**********************************************************************/
#include "PrintSingleLabelDlg.h"
/**********************************************************************/

CPrintSingleLabelDlg::CPrintSingleLabelDlg( bool bProduct, CPluCSVRecord& PluRecord, CWnd* pParent )
: CSSDialog(CPrintSingleLabelDlg::IDD, pParent), m_PluRecord( PluRecord )
{
	//{{AFX_DATA_INIT(CPrintSingleLabelDlg)
	//}}AFX_DATA_INIT
	m_bProduct = bProduct;
	m_nPos = 1;
	m_nQty = 1;
}

/**********************************************************************/

void CPrintSingleLabelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrintSingleLabelDlg)
	DDX_Control( pDX, IDC_STATIC_PLU, m_staticPlu );
	DDX_Control( pDX, IDC_STATIC_DESCRIPTION, m_staticDescription );
	DDX_Text( pDX, IDC_EDIT_POS, m_nPos );
	DDV_MinMaxInt( pDX, m_nPos, 1, 999 );
	DDX_Text( pDX, IDC_EDIT_QTY, m_nQty );
	DDV_MinMaxInt( pDX, m_nQty, 1, 99999 );
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPrintSingleLabelDlg, CDialog)
	//{{AFX_MSG_MAP(CPrintSingleLabelDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPrintSingleLabelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SubclassEdit( IDC_EDIT_POS, SS_NUM, 3, "%d" );
	SubclassEdit( IDC_EDIT_QTY, SS_NUM, 5, "%d" );
	
	if ( TRUE == m_bProduct )
		SetWindowText( "Print Product Label for Single Item" );
	else
		SetWindowText( "Print Shelf Edge Label for Single Item" );

	CReportHelpers ReportHelpers;
	m_staticPlu.SetWindowText( m_PluRecord.GetPluNoString() );
	m_staticDescription.SetWindowText( ReportHelpers.GetReportText( m_PluRecord ) );

	return TRUE;
}

/**********************************************************************/

void CPrintSingleLabelDlg::OnOK() 
{
	if ( UpdateData( TRUE, TRUE ) == TRUE )
		EndDialog( IDOK );
}

/**********************************************************************/
