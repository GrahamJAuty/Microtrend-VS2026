/**********************************************************************/
 
/**********************************************************************/
#include "TimeSliceQuickSetDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CTimeSliceQuickSetDlg, CDialog)
/**********************************************************************/

CTimeSliceQuickSetDlg::CTimeSliceQuickSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeSliceQuickSetDlg::IDD, pParent)
{
	m_nAction = 0;
	m_bSort = FALSE;
}

/**********************************************************************/

CTimeSliceQuickSetDlg::~CTimeSliceQuickSetDlg()
{
}

/**********************************************************************/

void CTimeSliceQuickSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nAction );
	DDX_Check(pDX, IDC_CHECK_SORT, m_bSort);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CTimeSliceQuickSetDlg, CDialog)
END_MESSAGE_MAP()

/**********************************************************************/
