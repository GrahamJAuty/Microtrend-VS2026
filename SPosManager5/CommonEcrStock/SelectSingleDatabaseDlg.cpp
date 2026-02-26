/**********************************************************************/
 
/**********************************************************************/
#include "NodeTypes.h"
/**********************************************************************/
#include "SelectSingleDatabaseDlg.h"
/**********************************************************************/

CSelectSingleDatabaseDlg::CSelectSingleDatabaseDlg( const char* szTitle, int nDbIdx, CWnd* pParent )
	: CDialog(CSelectSingleDatabaseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectSingleDatabaseDlg)
	//}}AFX_DATA_INIT
	m_nDbIdx = nDbIdx;
	m_strTitle = szTitle;
}

/**********************************************************************/

void CSelectSingleDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectSingleDatabaseDlg)
	DDX_Control(pDX, IDC_COMBO_DATABASE, m_comboDatabase);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSelectSingleDatabaseDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectSingleDatabaseDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSelectSingleDatabaseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	CWordArray arrayTypes;
	arrayTypes.Add ( NODE_DATABASE );
	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboDatabase );
	m_LocationSelector.ForceSelection( NODE_DATABASE, m_nDbIdx );
	SetWindowText (m_strTitle);
	return TRUE;  
}

/**********************************************************************/

void CSelectSingleDatabaseDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );
		m_nDbIdx = LocSelEntity.GetDbIdx();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
