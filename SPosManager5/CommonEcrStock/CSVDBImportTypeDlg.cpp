/**********************************************************************/
#include "CSVDBImportTypeDlg.h"
/**********************************************************************/

CCSVDBImportTypeDlg::CCSVDBImportTypeDlg( int nNodeType, CWnd* pParent )
	: CDialog(CCSVDBImportTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCSVDBImportTypeDlg)
	//}}AFX_DATA_INIT
	m_nNodeType = nNodeType;
	m_nImportMethod = 0;
}

/**********************************************************************/

void CCSVDBImportTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCSVDBImportTypeDlg)
	DDX_Control(pDX, IDC_RADIO_PREFIMP_ALL, m_radioAll);
	DDX_Control(pDX, IDC_RADIO_PREFIMP_NEW, m_radioNew);
	DDX_Radio(pDX, IDC_RADIO_PREFIMP_ALL, m_nImportMethod);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CCSVDBImportTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CCSVDBImportTypeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CCSVDBImportTypeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	switch (m_nNodeType)
	{
	case NODE_STOCK:
		SetWindowText("Import PLU/Stock Items");
		break;

	case NODE_SERVER:
		SetWindowText("Import Server Records");
		break;

	case NODE_PLU:
	default:
		SetWindowText("Import PLU Items");
		break;
	}

	switch (m_nNodeType)
	{
	case NODE_SERVER:
		m_radioAll.SetWindowText("Add all Servers");
		m_radioNew.SetWindowText("Add only new Servers");
		break;

	case NODE_STOCK:
	case NODE_PLU:
	default:
		m_radioAll.SetWindowText("Add all PLU Items");
		m_radioNew.SetWindowText("Add only new PLU Items");
		break;
	}

	return TRUE;  
}

/**********************************************************************/
