/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSStaticCalendar.h"
/**********************************************************************/
#include "PMSHighlightDlg.h"
/**********************************************************************/

IMPLEMENT_DYNAMIC(CPMSHighlightDlg, CDialog)

CPMSHighlightDlg::CPMSHighlightDlg( int nHighlightMode, CWnd* pParent )
	: CDialog(CPMSHighlightDlg::IDD, pParent)
{
	switch( nHighlightMode )
	{
	case PMS_SPECIAL_HIGHLIGHT_NONE:
	case PMS_SPECIAL_HIGHLIGHT_CREDIT:
	case PMS_SPECIAL_HIGHLIGHT_DEBIT:
	case PMS_SPECIAL_HIGHLIGHT_BOTH:
		m_nHighlightMode = nHighlightMode;
		break;

	default:
		m_nHighlightMode = PMS_SPECIAL_HIGHLIGHT_NONE;
		break;
	}
}

/**********************************************************************/

CPMSHighlightDlg::~CPMSHighlightDlg()
{
}

/**********************************************************************/

void CPMSHighlightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_CHECK_CREDIT, m_checkCredit );
	DDX_Control( pDX, IDC_CHECK_DEBIT, m_checkDebit );
}

/**********************************************************************/
BEGIN_MESSAGE_MAP(CPMSHighlightDlg, CDialog)
END_MESSAGE_MAP()
/**********************************************************************/

BOOL CPMSHighlightDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_checkDebit.SetCheck( ( PMS_SPECIAL_HIGHLIGHT_DEBIT == m_nHighlightMode ) || ( PMS_SPECIAL_HIGHLIGHT_BOTH == m_nHighlightMode ) );
	m_checkCredit.SetCheck( ( PMS_SPECIAL_HIGHLIGHT_CREDIT == m_nHighlightMode ) || ( PMS_SPECIAL_HIGHLIGHT_BOTH == m_nHighlightMode ) );
	return TRUE;  
}

/**********************************************************************/

void CPMSHighlightDlg::OnOK() 
{
	m_nHighlightMode = 0;
	
	if ( m_checkDebit.GetCheck() != 0 )
		m_nHighlightMode = ( m_checkCredit.GetCheck() != 0 ) ? PMS_SPECIAL_HIGHLIGHT_BOTH : PMS_SPECIAL_HIGHLIGHT_DEBIT;
	else
		m_nHighlightMode = ( m_checkCredit.GetCheck() != 0 ) ? PMS_SPECIAL_HIGHLIGHT_CREDIT : PMS_SPECIAL_HIGHLIGHT_NONE;
	
	EndDialog( IDOK );
}

/**********************************************************************/
#endif
/**********************************************************************/
