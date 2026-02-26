/**********************************************************************/
#include "Repspawn.h"
/**********************************************************************/
#include "ProcessedOrderExceptionsDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CProcessedOrderExceptionsDlg, CDialog)
/**********************************************************************/

CProcessedOrderExceptionsDlg::CProcessedOrderExceptionsDlg( const char* szTitle, CWnd* pParent)
	: CDialog(CProcessedOrderExceptionsDlg::IDD, pParent)
{
	m_strTitle = szTitle;
}

/**********************************************************************/

CProcessedOrderExceptionsDlg::~CProcessedOrderExceptionsDlg()
{
}

/**********************************************************************/

void CProcessedOrderExceptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ORDER, m_staticOrder );
}

/**********************************************************************/
BEGIN_MESSAGE_MAP(CProcessedOrderExceptionsDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_EXCEPTIONS, &CProcessedOrderExceptionsDlg::OnBnClickedButtonExceptions)
	ON_BN_CLICKED(IDC_BUTTON_PROCEED, &CProcessedOrderExceptionsDlg::OnBnClickedButtonProceed)
END_MESSAGE_MAP()
/**********************************************************************/

BOOL CProcessedOrderExceptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString strText = "REPEAT ORDER : ";
	m_staticOrder.SetWindowText( strText + m_strTitle );
	return TRUE;  
}

/**********************************************************************/

void CProcessedOrderExceptionsDlg::OnOK()
{
}

/**********************************************************************/

void CProcessedOrderExceptionsDlg::OnBnClickedButtonExceptions()
{
	CReportManager ReportManager;
	CFilenameUpdater FnUp ( SysFiles::OrderExceptionsPrm );
	ReportManager.DisplayReport( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}

/**********************************************************************/

void CProcessedOrderExceptionsDlg::OnBnClickedButtonProceed()
{
	EndDialog (IDOK );
}

/**********************************************************************/
