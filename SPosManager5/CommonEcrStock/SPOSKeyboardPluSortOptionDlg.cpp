/**********************************************************************/
#include "SPOSKeyboardPluSortOptionDlg.h"
/**********************************************************************/

IMPLEMENT_DYNAMIC(CSPOSKeyboardPluSortOptionDlg, CDialog)

/**********************************************************************/

CSPOSKeyboardPluSortOptionDlg::CSPOSKeyboardPluSortOptionDlg(CWnd* pParent)
	: CDialog(CSPOSKeyboardPluSortOptionDlg::IDD, pParent)
{
	m_bAll = FALSE;
}

/**********************************************************************/

CSPOSKeyboardPluSortOptionDlg::~CSPOSKeyboardPluSortOptionDlg()
{
}

/**********************************************************************/

void CSPOSKeyboardPluSortOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardPluSortOptionDlg, CDialog)
	ON_BN_CLICKED( IDC_BUTTON_CURRENT, OnButtonCurrent )
	ON_BN_CLICKED( IDC_BUTTON_ALL, OnButtonAll )
END_MESSAGE_MAP()

/**********************************************************************/

void CSPOSKeyboardPluSortOptionDlg::OnButtonCurrent()
{
	m_bAll = FALSE;
	EndDialog( IDOK );
}

/**********************************************************************/

void CSPOSKeyboardPluSortOptionDlg::OnButtonAll()
{
	m_bAll = TRUE;
	EndDialog( IDOK );
}

/**********************************************************************/

void CSPOSKeyboardPluSortOptionDlg::OnOK()
{
}

/**********************************************************************/

void CSPOSKeyboardPluSortOptionDlg::OnCancel()
{
	EndDialog( IDCANCEL );
}

/**********************************************************************/
