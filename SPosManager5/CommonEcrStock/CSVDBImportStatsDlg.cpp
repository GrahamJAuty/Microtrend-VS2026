/**********************************************************************/
#include "NodeTypes.h"
/**********************************************************************/
#include "CSVDBImportStatsDlg.h"
/**********************************************************************/

CCSVDBImportStatsDlg::CCSVDBImportStatsDlg( int nType, CWnd* pParent )
: CDialog (CCSVDBImportStatsDlg::IDD, pParent )
{
	//{{AFX_DATA_INIT(CCSVDBImportStatsDlg)
	//}}AFX_DATA_INIT
	m_nType = nType;
}

/**********************************************************************/

void CCSVDBImportStatsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCSVDBImportStatsDlg)
	DDX_Control(pDX, IDC_STATIC_IGNORED, m_staticIgnored);
	DDX_Control(pDX, IDC_STATIC_TEXT2, m_staticText2);
	DDX_Control(pDX, IDC_STATIC_TEXT1, m_staticText1);
	DDX_Control(pDX, IDC_STATIC_TOTALLINES, m_staticTotalLines);
	DDX_Control(pDX, IDC_STATIC_RECORDSMODIFIED, m_staticRecordsModified);
	DDX_Control(pDX, IDC_STATIC_RECORDSADDED, m_staticRecordsAdded);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CCSVDBImportStatsDlg, CDialog)
	//{{AFX_MSG_MAP(CCSVDBImportStatsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CCSVDBImportStatsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	switch( m_nType )
	{
	case NODE_BARCODE:
		m_staticText1.SetWindowText ( "Number of barcodes added" );
		m_staticText2.SetWindowText ( "Number of barcodes modified" );
		break;

	case NODE_SUPPLIER:
		m_staticText1.SetWindowText ( "Number of suppliers added" );
		m_staticText2.SetWindowText ( "Number of suppliers modified" );
		break;

	case NODE_SERVER:
		m_staticText1.SetWindowText("Number of servers added");
		m_staticText2.SetWindowText("Number of servers modified");
		break;

	case NODE_PLU:
	default:
		m_staticText1.SetWindowText ( "Number of PLU items added" );
		m_staticText2.SetWindowText ( "Number of PLU items modified" );
		break;
	}

	CString strBuf;
	strBuf.Format ( "%d", m_nTotalLines );
	m_staticTotalLines.SetWindowText ( strBuf );

	strBuf.Format ( "%d", m_nIgnoredLines );
	m_staticIgnored.SetWindowText ( strBuf );
	
	strBuf.Format ( "%d", m_nRecordsAdded );
	m_staticRecordsAdded.SetWindowText ( strBuf );

	strBuf.Format ( "%d", m_nRecordsModified );
	m_staticRecordsModified.SetWindowText ( strBuf );

	return TRUE;  
}

/**********************************************************************/
