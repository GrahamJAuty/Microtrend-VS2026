/**********************************************************************/
 
/**********************************************************************/
#include "PluImportStatsComplexDlg.h"
/**********************************************************************/

CPluImportStatsComplexDlg::CPluImportStatsComplexDlg ( CWnd* pParent )
: CDialog ( CPluImportStatsComplexDlg::IDD, pParent )
{
	//{{AFX_DATA_INIT(CPluImportStatsComplexDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CPluImportStatsComplexDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluImportStatsComplexDlg)
	DDX_Control(pDX, IDC_STATIC_IGNORED, m_staticIgnored);
	DDX_Control(pDX, IDC_STATIC_SUPPLIERADDED, m_staticSupplierAdded);
	DDX_Control(pDX, IDC_STATIC_STOCKMODIFIED, m_staticStockModified);
	DDX_Control(pDX, IDC_STATIC_TOTALLINES, m_staticTotalLines);
	DDX_Control(pDX, IDC_STATIC_STOCKADDED, m_staticStockAdded);
	DDX_Control(pDX, IDC_STATIC_PLUMODIFIED, m_staticPluModified);
	DDX_Control(pDX, IDC_STATIC_PLUADDED, m_staticPluAdded);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluImportStatsComplexDlg, CDialog)
	//{{AFX_MSG_MAP(CPluImportStatsComplexDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluImportStatsComplexDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strBuf;
	strBuf.Format ( "%d", m_nTotalLines );
	m_staticTotalLines.SetWindowText ( strBuf );

	strBuf.Format ( "%d", m_nIgnoredLines );
	m_staticIgnored.SetWindowText ( strBuf );
	
	strBuf.Format ( "%d", m_nPluAdded );
	m_staticPluAdded.SetWindowText ( strBuf );

	strBuf.Format ( "%d", m_nPluModified );
	m_staticPluModified.SetWindowText ( strBuf );

	strBuf.Format ( "%d", m_nStockAdded );
	m_staticStockAdded.SetWindowText ( strBuf );
	
	strBuf.Format ( "%d", m_nStockModified );
	m_staticStockModified.SetWindowText ( strBuf );
	
	strBuf.Format ( "%d", m_nSupplierAdded );
	m_staticSupplierAdded.SetWindowText ( strBuf );
		
	return TRUE;  
}

/**********************************************************************/
