/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "PMSManualDiscountEditDlg.h"
/**********************************************************************/

CPMSManualDiscountEditDlg::CPMSManualDiscountEditDlg( bool bAdd, CWnd* pParent)
	: CSSDialog(CPMSManualDiscountEditDlg::IDD, pParent)
{
	DataInit();
	m_bAdd = bAdd;
}

/**********************************************************************/

void CPMSManualDiscountEditDlg::DataInit()
{
	//{{AFX_DATA_INIT(CPMSManualDiscountEditDlg)
	//}}AFX_DATA_INIT
	m_bModified = FALSE;
	m_dDiscount = 0.0;
	
	m_strInputLine = "";
	m_strOutputLine = "";
}

/**********************************************************************/

void CPMSManualDiscountEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPMSManualDiscountEditDlg)
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDC_STATIC_DISCOUNT, m_staticDiscount);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_DISCOUNT, m_dDiscount );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSManualDiscountEditDlg, CDialog)
	//{{AFX_MSG_MAP(CPMSManualDiscountEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPMSManualDiscountEditDlg::GetEditDiscount() { return GetEdit( IDC_EDIT_DISCOUNT ); }
/**********************************************************************/

BOOL CPMSManualDiscountEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SubclassEdit ( IDC_EDIT_DISCOUNT, SS_NUM_SDP, 8, "%.*f", SysInfo.GetDPValue() );
	
	switch( m_SaleInfo.m_nLineType )
	{
	case PMS_LINE_SDISC:
		m_staticDiscount.SetWindowText( "Subtotal discount" );
		break;

	case PMS_LINE_LDISC:
		m_staticDiscount.SetWindowText( "Loyalty discount" );
		break;

	default:
		break;
	}

	CString strTitle;

	if ( m_bAdd == TRUE )
	{
		switch( m_SaleInfo.m_nLineType )
		{
		case PMS_LINE_SDISC:
			strTitle = "Add Subtotal Discount";
			break;

		case PMS_LINE_LDISC:
			strTitle = "Add Loyalty Discount";
			break;

		default:
			strTitle= "";
			break;
		}
	}
	else
	{
		switch( m_SaleInfo.m_nLineType )
		{
		case PMS_LINE_SDISC:
			strTitle = "Edit Subtotal Discount";
			break;

		case PMS_LINE_LDISC:
			strTitle = "Edit Loyalty Discount";
			break;

		default:
			strTitle= "";
			break;
		}
		
		m_dDiscount = m_SaleInfo.m_dDiscount;
		
		UpdateData( FALSE );
		
		GetEditDiscount() -> SetFocus();
		GetEditDiscount() -> SetSel( 0, -1 );
	}

	SetWindowText ( strTitle );
	CenterWindow();
	
	//WE HAVE SET FOCUS TO A CONTROL
	return FALSE;  
}

/**********************************************************************/

void CPMSManualDiscountEditDlg::OnOK() 
{
	m_buttonOK.SetFocus();

	if ( UpdateData ( TRUE ) == FALSE )
		return;
	
	HandleAdd();
	EndDialog ( IDOK );
}

/**********************************************************************/

void CPMSManualDiscountEditDlg::HandleAdd() 
{
	m_buttonOK.SetFocus();

	m_SaleInfo.m_nBasePluNo = 0;
	m_SaleInfo.m_nModifier = 0;
	m_SaleInfo.m_strDescription = "";
	m_SaleInfo.m_nLevel = 0;
	m_SaleInfo.m_dEposPluQty = 0.0;
	m_SaleInfo.m_dBasePluQty = 0.0;
	m_SaleInfo.m_dValue = 0.0;
	m_SaleInfo.m_dDiscount = m_dDiscount;
}

/**********************************************************************/

void CPMSManualDiscountEditDlg::OnCancel() 
{
	EndDialog( IDCANCEL );
}

/**********************************************************************/
#endif
/**********************************************************************/
