/**********************************************************************/
#include "CustomerVisitDlg.h"
/**********************************************************************/

CCustomerVisitDlg::CCustomerVisitDlg( int nCustIdx, CWnd* pParent )
	: CSSDialog(CCustomerVisitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCustomerVisitDlg)
	//}}AFX_DATA_INIT
	m_nCustIdx = nCustIdx;
}

/**********************************************************************/

void CCustomerVisitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomerVisitDlg)
	DDX_Control( pDX, IDC_EDIT_NAME, m_editName );
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CCustomerVisitDlg, CDialog)
	//{{AFX_MSG_MAP(CCustomerVisitDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CCustomerVisitDlg::GetEditBeforeThis() { return GetEdit( IDC_EDIT_BEFORE_THIS ); }
CEdit* CCustomerVisitDlg::GetEditAfterThis() { return GetEdit( IDC_EDIT_AFTER_THIS ); }
CEdit* CCustomerVisitDlg::GetEditBeforeOther() { return GetEdit( IDC_EDIT_BEFORE_OTHER ); }
CEdit* CCustomerVisitDlg::GetEditAfterOther() { return GetEdit( IDC_EDIT_AFTER_OTHER ); }
CEdit* CCustomerVisitDlg::GetEditVisit() { return GetEdit( IDC_EDIT_VISIT ); }
/**********************************************************************/

BOOL CCustomerVisitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	
	SubclassEdit ( IDC_EDIT_BEFORE_THIS, SS_NUM, 2, "%d" );
	SubclassEdit ( IDC_EDIT_AFTER_THIS, SS_NUM, 2, "%d" );
	SubclassEdit ( IDC_EDIT_BEFORE_OTHER, SS_NUM, 2, "%d" );
	SubclassEdit ( IDC_EDIT_AFTER_OTHER, SS_NUM, 2, "%d" );
	SubclassEdit ( IDC_EDIT_VISIT, SS_NUM, 2, "%d" );
	GetEditVisit() -> SetWindowText( "60" );

	CCustomerCSVRecord Customer;
	DataManagerNonDb.CustomerOneOff.GetAt( m_nCustIdx, Customer );
	m_editName.SetWindowText( Customer.GetDisplayNameFull() );

	SetEditBoxInt( *GetEditBeforeThis(), EcrmanOptions.GetReportsContactBeforeTime() );
	SetEditBoxInt( *GetEditAfterThis(), EcrmanOptions.GetReportsContactAfterTime() );
	SetEditBoxInt( *GetEditBeforeOther(), EcrmanOptions.GetReportsContactBeforeTime() );
	SetEditBoxInt( *GetEditAfterOther(), EcrmanOptions.GetReportsContactAfterTime() );
	SetEditBoxInt( *GetEditVisit(), EcrmanOptions.GetReportsContactVisitTime() );

	return TRUE;  
}

/**********************************************************************/

void CCustomerVisitDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_nBeforeThis = GetEditBoxInt( *GetEditBeforeThis() );
		m_nAfterThis = GetEditBoxInt( *GetEditAfterThis() );
		m_nBeforeOther = GetEditBoxInt( *GetEditBeforeOther() );
		m_nAfterOther = GetEditBoxInt( *GetEditAfterOther() );
		m_nVisit = GetEditBoxInt( *GetEditVisit() );
		EndDialog ( IDOK );
	}
}

/**********************************************************************/

