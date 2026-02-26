/**********************************************************************/
 
/**********************************************************************/
#include "SalesHistoryFields.h"
/**********************************************************************/
#include "SalesHistoryCSVRecordAddDlg.h"
/**********************************************************************/

CSalesHistoryCSVRecordAddDlg::CSalesHistoryCSVRecordAddDlg( CWnd* pParent)
	: CDialog(CSalesHistoryCSVRecordAddDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSalesHistoryCSVRecordAddDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CSalesHistoryCSVRecordAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSalesHistoryCSVRecordAddDlg)
	DDX_Control(pDX, IDC_COMBO_REPORT, m_comboReport);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSalesHistoryCSVRecordAddDlg, CDialog)
	//{{AFX_MSG_MAP(CSalesHistoryCSVRecordAddDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSalesHistoryCSVRecordAddDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	PrepareComboAndTitle();
	return TRUE;
}

/**********************************************************************/

void CSalesHistoryCSVRecordAddDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	int nSel = m_comboReport.GetCurSel();
	
	if ( nSel >= 0 && nSel < m_arrayReportTypes.GetSize() )
		m_nReportType = m_arrayReportTypes.GetAt(nSel);
	else
		m_nReportType = SH_CUSTOM;
		
	EndDialog ( IDOK );
}

/**********************************************************************/
