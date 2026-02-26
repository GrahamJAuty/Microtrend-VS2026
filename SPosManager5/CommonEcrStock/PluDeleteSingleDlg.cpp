/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
#include "ReportHelpers.h"
/**********************************************************************/
#include "PluDeleteSingleDlg.h"
/**********************************************************************/

CPluDeleteSingleDlg::CPluDeleteSingleDlg( CPluCSVRecord& PluRecord, CWnd* pParent)
	: CDialog(CPluDeleteSingleDlg::IDD, pParent), m_PluRecord( PluRecord )
{
	//{{AFX_DATA_INIT(CPluDeleteSingleDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CPluDeleteSingleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluDeleteSingleDlg)
	DDX_Control(pDX, IDC_STATIC_PLU, m_staticPlu);
	DDX_Control(pDX, IDC_STATIC_DESCRIPTION, m_staticDescription);
	DDX_Control(pDX, IDC_STATIC_DEPT, m_staticDept);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluDeleteSingleDlg, CDialog)
	//{{AFX_MSG_MAP(CPluDeleteSingleDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluDeleteSingleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CReportHelpers ReportHelpers;
	m_staticPlu.SetWindowText( m_PluRecord.GetPluNoString() );
	m_staticDescription.SetWindowText( ReportHelpers.GetReportText( m_PluRecord ) );
	m_staticDept.SetWindowText( DataManager.Department.GetReportTextByDeptNo( m_PluRecord.GetBaseDeptNo()) );
	return TRUE;  
}

/**********************************************************************/
