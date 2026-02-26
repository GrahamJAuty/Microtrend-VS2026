/**********************************************************************/
 
/**********************************************************************/
#include "LocalPluFilterDlg.h"
/**********************************************************************/

CLocalPluFilterDlg::CLocalPluFilterDlg( CPluFilterArray& FilterArray, CWnd* pParent)
	: CDialog(CLocalPluFilterDlg::IDD, pParent), m_FilterArray ( FilterArray )
{
	//{{AFX_DATA_INIT(CLocalPluFilterDlg)
	m_bExclude = FALSE;
	m_bLocation = FALSE;
	m_bMaster = FALSE;
	//}}AFX_DATA_INIT
	m_bLocation = m_FilterArray.GetLocalPluLocationFilter();
	m_bMaster = m_FilterArray.GetLocalPluMasterFilter();
	m_bExclude = m_FilterArray.GetLocalPluExcludeFilter();
}

/**********************************************************************/

void CLocalPluFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocalPluFilterDlg)
	DDX_Check(pDX, IDC_CHECK_EXCLUDE, m_bExclude);
	DDX_Check(pDX, IDC_CHECK_LOCATION, m_bLocation);
	DDX_Check(pDX, IDC_CHECK_MASTER, m_bMaster);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CLocalPluFilterDlg, CDialog)
	//{{AFX_MSG_MAP(CLocalPluFilterDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CLocalPluFilterDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_FilterArray.SetLocalPluLocationFilter ( m_bLocation != 0 );
		m_FilterArray.SetLocalPluMasterFilter ( m_bMaster != 0 );
		m_FilterArray.SetLocalPluExcludeFilter ( m_bExclude != 0 );
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
