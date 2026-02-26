/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSManualSalesCloseDlg.h"
#include "PMSManualSalesDatabaseDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPMSManualSalesCloseDlg, CDialog)
/**********************************************************************/

CPMSManualSalesCloseDlg::CPMSManualSalesCloseDlg( int nPMSActionType, CWnd* pParent)
	: CDialog(CPMSManualSalesCloseDlg::IDD, pParent)
{
	m_nAction = PMSSALES_CONTINUE;
	m_nPMSActionType = nPMSActionType;
}

/**********************************************************************/

CPMSManualSalesCloseDlg::~CPMSManualSalesCloseDlg()
{
}

/**********************************************************************/

void CPMSManualSalesCloseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CONFIRM, m_buttonConfirm);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_buttonCancel);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSManualSalesCloseDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CONFIRM, &CPMSManualSalesCloseDlg::OnButtonConfirm)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CPMSManualSalesCloseDlg::OnButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONTINUE, &CPMSManualSalesCloseDlg::OnButtonContinue)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPMSManualSalesCloseDlg::OnInitDialog()
{
	SetDefID( IDC_BUTTON_CONTINUE );
	CDialog::OnInitDialog();

	switch( m_nPMSActionType )
	{
	case PMS_MANUAL_PAYMENT:
		SetWindowText( "Save Changes to Guest Payments" );
		break;

	case PMS_MANUAL_DEPOSIT:
		SetWindowText( "Save Changes to Guest Deposits" );
		break;

	case PMS_MANUAL_SALES:
	default:
		SetWindowText( "Save Changes to Guest Sales" );
		break;
	}
		
	return FALSE;  
}

/**********************************************************************/

void CPMSManualSalesCloseDlg::OnOK()
{
	OnButtonCancel();
}

/**********************************************************************/

void CPMSManualSalesCloseDlg::OnCancel()
{
	OnButtonCancel();
}

/**********************************************************************/

void CPMSManualSalesCloseDlg::OnButtonConfirm()
{
	m_nAction = PMSSALES_CONFIRM;
	EndDialog( IDOK );
}

/**********************************************************************/

void CPMSManualSalesCloseDlg::OnButtonCancel()
{
	m_nAction = PMSSALES_CANCEL;
	EndDialog( IDOK );
}

/**********************************************************************/

void CPMSManualSalesCloseDlg::OnButtonContinue()
{
	m_nAction = PMSSALES_CONTINUE;
	EndDialog( IDOK );
}

/**********************************************************************/
#endif
/**********************************************************************/
