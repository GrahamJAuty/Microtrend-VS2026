/**********************************************************************/
#include "NodeTypes.h"
/**********************************************************************/
#include "StockImportFilterCloseDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CStockImportFilterCloseDlg, CDialog)
/**********************************************************************/

CStockImportFilterCloseDlg::CStockImportFilterCloseDlg( const char* szTitle, CWnd* pParent)
	: CDialog(CStockImportFilterCloseDlg::IDD, pParent)
{
	m_nAction = STOCKIMPORT_YES;
	m_strTitle = szTitle;
}

/**********************************************************************/

CStockImportFilterCloseDlg::~CStockImportFilterCloseDlg()
{
}

/**********************************************************************/

void CStockImportFilterCloseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockImportFilterCloseDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_YES, OnButtonYes)
	ON_BN_CLICKED(IDC_BUTTON_NO, OnButtonNo)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockImportFilterCloseDlg::OnInitDialog()
{
	SetDefID( IDC_BUTTON_YES );
	CDialog::OnInitDialog();

	SetWindowText( m_strTitle );

	return FALSE;  
}

/**********************************************************************/

void CStockImportFilterCloseDlg::OnOK()
{
	OnButtonCancel();
}

/**********************************************************************/

void CStockImportFilterCloseDlg::OnCancel()
{
	OnButtonCancel();
}

/**********************************************************************/

void CStockImportFilterCloseDlg::OnButtonYes()
{
	m_nAction = STOCKIMPORT_YES;
	EndDialog( IDOK );
}

/**********************************************************************/

void CStockImportFilterCloseDlg::OnButtonNo()
{
	m_nAction = STOCKIMPORT_NO;
	EndDialog( IDOK );
}

/**********************************************************************/

void CStockImportFilterCloseDlg::OnButtonCancel()
{
	m_nAction = STOCKIMPORT_CANCEL;
	EndDialog( IDCANCEL );
}

/**********************************************************************/
