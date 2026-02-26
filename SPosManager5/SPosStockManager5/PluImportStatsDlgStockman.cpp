/**********************************************************************/
 
/**********************************************************************/
#include "PluImportStatsDlgStockman.h"
/**********************************************************************/

CPluImportStatsDlgStockman::CPluImportStatsDlgStockman ( bool bStockMode, CWnd* pParent )
: CDialog ( CPluImportStatsDlgStockman::IDD, pParent )
{
	//{{AFX_DATA_INIT(CPluImportStatsDlgStockman)
	//}}AFX_DATA_INIT
	m_bStockMode = bStockMode;
}

/**********************************************************************/

void CPluImportStatsDlgStockman::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluImportStatsDlgStockman)
	DDX_Control(pDX, IDC_STATIC_IGNORED, m_staticIgnored);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_staticLabel3);
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_staticLabel2);
	DDX_Control(pDX, IDC_STATIC_LABEL1, m_staticLabel1);
	DDX_Control(pDX, IDC_STATIC_TOTALLINES, m_staticTotalLines);
	DDX_Control(pDX, IDC_STATIC_STOCKADDED, m_staticStockAdded);
	DDX_Control(pDX, IDC_STATIC_PLUMODIFIED, m_staticPluModified);
	DDX_Control(pDX, IDC_STATIC_PLUADDED, m_staticPluAdded);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluImportStatsDlgStockman, CDialog)
	//{{AFX_MSG_MAP(CPluImportStatsDlgStockman)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluImportStatsDlgStockman::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if ( m_bStockMode == FALSE )
	{
		m_staticLabel1.SetWindowText ( "Number of PLU items added" );
		m_staticLabel2.SetWindowText ( "Number of PLU items modified" );
		m_staticLabel3.SetWindowText ( "Number of stock items added" );
	}
	else
	{
		m_staticLabel1.SetWindowText ( "Number of stock items added" );
		m_staticLabel2.SetWindowText ( "Number of stock items modified" );
		m_staticLabel3.SetWindowText ( "Number of suppliers added" );
	}

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
	
	return TRUE;  
}

/**********************************************************************/
