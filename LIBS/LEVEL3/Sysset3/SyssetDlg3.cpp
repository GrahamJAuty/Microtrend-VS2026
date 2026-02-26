/**********************************************************************/

#include "sysset3.h"
#include "SyssetDlg3.h"

/**********************************************************************/

IMPLEMENT_DYNAMIC(CSyssetDlg3, CDialog)

/**********************************************************************/

CSyssetDlg3::CSyssetDlg3(const char* szText, CWnd* pParent /*=NULL*/)
	: CDialog()
{
	Create ( CSyssetDlg3::IDD, pParent );
	CenterWindow();
	m_staticWaitMessage.SetWindowText ( szText );
}

/**********************************************************************/

CSyssetDlg3::~CSyssetDlg3()
{
}

/**********************************************************************/

void CSyssetDlg3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SYSSET_STATIC_WAIT_MESSAGE, m_staticWaitMessage);
}


/**********************************************************************/

BEGIN_MESSAGE_MAP(CSyssetDlg3, CDialog)
END_MESSAGE_MAP()

/**********************************************************************/
