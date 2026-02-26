/**********************************************************************/
 
/**********************************************************************/
#include "NodeTypes.h"
/**********************************************************************/
#include "SelectSingleNetworkDlg.h"
/**********************************************************************/

CSelectSingleNetworkDlg::CSelectSingleNetworkDlg( const char* szTitle, CWnd* pParent )
	: CDialog(CSelectSingleNetworkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectSingleNetworkDlg)
	//}}AFX_DATA_INIT
	m_strTitle = szTitle;
}

/**********************************************************************/

void CSelectSingleNetworkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectSingleNetworkDlg)
	DDX_Control(pDX, IDC_COMBO_DATABASE, m_comboDatabase);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSelectSingleNetworkDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectSingleNetworkDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSelectSingleNetworkDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	CWordArray arrayTypes;
	arrayTypes.Add ( NODE_NETWORK );
	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboDatabase );
	SetWindowText (m_strTitle);
	return TRUE;  
}

/**********************************************************************/

void CSelectSingleNetworkDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );
		m_nNetworkIdx = LocSelEntity.GetEntityIdx();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
