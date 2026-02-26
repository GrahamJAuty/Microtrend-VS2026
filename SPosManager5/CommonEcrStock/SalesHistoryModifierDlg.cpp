/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SalesHistoryModifierDlg.h"
/**********************************************************************/

CSalesHistoryModifierDlg::CSalesHistoryModifierDlg(CWnd* pParent)
	: CDialog(CSalesHistoryModifierDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSalesHistoryModifierDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CSalesHistoryModifierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSalesHistoryModifierDlg)
	DDX_Control(pDX, IDC_CHECK_MOD0, m_checkMod0);
	DDX_Control(pDX, IDC_CHECK_MOD1, m_checkMod1);
	DDX_Control(pDX, IDC_CHECK_MOD2, m_checkMod2);
	DDX_Control(pDX, IDC_CHECK_MOD3, m_checkMod3);
	DDX_Control(pDX, IDC_CHECK_MOD4, m_checkMod4);
	DDX_Control(pDX, IDC_CHECK_MOD5, m_checkMod5);
	DDX_Control(pDX, IDC_CHECK_MOD6, m_checkMod6);
	DDX_Control(pDX, IDC_CHECK_MOD7, m_checkMod7);
	DDX_Control(pDX, IDC_CHECK_MOD8, m_checkMod8);
	DDX_Control(pDX, IDC_CHECK_MOD9, m_checkMod9);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSalesHistoryModifierDlg, CDialog)
	//{{AFX_MSG_MAP(CSalesHistoryModifierDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSalesHistoryModifierDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	m_checkMod1.SetWindowText( DataManager.Modifier.GetDisplayName(1) );
	m_checkMod2.SetWindowText( DataManager.Modifier.GetDisplayName(2) );
	m_checkMod3.SetWindowText( DataManager.Modifier.GetDisplayName(3) );
	m_checkMod4.SetWindowText( DataManager.Modifier.GetDisplayName(4) );
	m_checkMod5.SetWindowText( DataManager.Modifier.GetDisplayName(5) );
	m_checkMod6.SetWindowText( DataManager.Modifier.GetDisplayName(6) );
	m_checkMod7.SetWindowText( DataManager.Modifier.GetDisplayName(7) );
	m_checkMod8.SetWindowText( DataManager.Modifier.GetDisplayName(8) );
	m_checkMod9.SetWindowText( DataManager.Modifier.GetDisplayName(9) );

	int nMask = SysInfo.GetCustomModifierFilter();
	m_checkMod0.SetCheck( ( nMask & 1 ) != 0 );
	m_checkMod1.SetCheck( ( nMask & 2 ) != 0 );
	m_checkMod2.SetCheck( ( nMask & 4 ) != 0 );
	m_checkMod3.SetCheck( ( nMask & 8 ) != 0 );
	m_checkMod4.SetCheck( ( nMask & 16 ) != 0 );
	m_checkMod5.SetCheck( ( nMask & 32 ) != 0 );
	m_checkMod6.SetCheck( ( nMask & 64 ) != 0 );
	m_checkMod7.SetCheck( ( nMask & 128 ) != 0 );
	m_checkMod8.SetCheck( ( nMask & 256 ) != 0 );
	m_checkMod9.SetCheck( ( nMask & 512 ) != 0 );

	return TRUE;  
}

/**********************************************************************/

void CSalesHistoryModifierDlg::OnOK() 
{
	int nMask = 0;
	if ( m_checkMod0.GetCheck() != 0 )	nMask += 1;
	if ( m_checkMod1.GetCheck() != 0 )	nMask += 2;
	if ( m_checkMod2.GetCheck() != 0 )	nMask += 4;
	if ( m_checkMod3.GetCheck() != 0 )	nMask += 8;
	if ( m_checkMod4.GetCheck() != 0 )	nMask += 16;
	if ( m_checkMod5.GetCheck() != 0 )	nMask += 32;
	if ( m_checkMod6.GetCheck() != 0 )	nMask += 64;
	if ( m_checkMod7.GetCheck() != 0 )	nMask += 128;
	if ( m_checkMod8.GetCheck() != 0 )	nMask += 256;
	if ( m_checkMod9.GetCheck() != 0 )	nMask += 512;
	SysInfo.SetCustomModifierFilter( nMask );

	EndDialog ( IDOK );
}

/**********************************************************************/
