/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSOptions.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PMSManualPaymentEditDlg.h"
/**********************************************************************/

CPMSManualPaymentEditDlg::CPMSManualPaymentEditDlg( bool bAdd, CWnd* pParent)
	: CSSDialog(CPMSManualPaymentEditDlg::IDD, pParent)
{
	DataInit();
	m_bAdd = bAdd;
}

/**********************************************************************/

void CPMSManualPaymentEditDlg::DataInit()
{
	//{{AFX_DATA_INIT(CPMSManualPaymentEditDlg)
	//}}AFX_DATA_INIT
	m_bModified = FALSE;
	m_dPayment = 0.0;
	
	m_strInputLine = "";
	m_strOutputLine = "";
}

/**********************************************************************/

void CPMSManualPaymentEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPMSManualPaymentEditDlg)
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDOK, m_buttonOK);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_PAYMENT, m_comboPayment);
	DDX_Text(pDX, IDC_EDIT_PAYMENT, m_dPayment );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSManualPaymentEditDlg, CDialog)
	//{{AFX_MSG_MAP(CPMSManualPaymentEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPMSManualPaymentEditDlg::GetEditPayment() { return GetEdit( IDC_EDIT_PAYMENT ); }
/**********************************************************************/

BOOL CPMSManualPaymentEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SubclassEdit ( IDC_EDIT_PAYMENT, SS_NUM_SDP, 8, "%.*f", SysInfo.GetDPValue() );
	
	CString strTitle;

	if ( TRUE == m_bAdd )
		strTitle = "Add Payment";
	else
		strTitle = "Edit Payment";
		
	if ( FALSE == m_bAdd )
	{
		m_dPayment = m_SaleInfo.m_dValue;	
		UpdateData( FALSE );
		
		GetEditPayment() -> SetFocus();
		GetEditPayment() -> SetSel( 0, -1 );
	}

	for ( int n = 0; n < PMSOptions.GetValidatedPaymentTypeArraySize(); n++ )
	{
		CCSV csv( PMSOptions.GetValidatedPaymentTypeString(n) );
		int nPos = m_comboPayment.AddString( csv.GetString(1) );
		m_comboPayment.SetItemData( nPos, csv.GetInt(0) );
	}

	if ( m_SaleInfo.m_nBasePluNo == 0 )
		m_comboPayment.SetCurSel(0);
	else
	{
		bool bGotMatch = FALSE;

		for ( int n = 0; n < m_comboPayment.GetCount(); n++ )
		{
			if ( m_comboPayment.GetItemData(n) == m_SaleInfo.m_nBasePluNo )
			{
				m_comboPayment.SetCurSel(n);
				bGotMatch = TRUE;
				break;
			}
		}

		if ( FALSE == bGotMatch )
		{
			m_comboPayment.InsertString( 0, m_SaleInfo.m_strDescription );
			m_comboPayment.SetItemData( 0, (int) m_SaleInfo.m_nBasePluNo );
		}
	}

	if ( m_comboPayment.GetCount() == 1 )
	{
		m_comboPayment.SetCurSel(0);
		m_comboPayment.EnableWindow(FALSE);
	}

	SetWindowText ( strTitle );
	CenterWindow();
	
	//WE HAVE SET FOCUS TO A CONTROL
	return FALSE;  
}

/**********************************************************************/

void CPMSManualPaymentEditDlg::OnOK() 
{
	m_buttonOK.SetFocus();

	if ( UpdateData ( TRUE ) == FALSE )
		return;

	HandleAdd();
	EndDialog ( IDOK );
}

/**********************************************************************/

void CPMSManualPaymentEditDlg::HandleAdd() 
{
	m_buttonOK.SetFocus();

	int nSel = m_comboPayment.GetCurSel();

	m_SaleInfo.m_nBasePluNo = m_comboPayment.GetItemData(nSel);
	m_SaleInfo.m_nModifier = 0;
	m_comboPayment.GetLBText(nSel, m_SaleInfo.m_strDescription );
	m_SaleInfo.m_nLevel = 0;
	m_SaleInfo.m_dEposPluQty = 0.0;
	m_SaleInfo.m_dBasePluQty = 0.0;
	m_SaleInfo.m_dValue = 0.0;
	m_SaleInfo.m_dValue = m_dPayment;
}

/**********************************************************************/

void CPMSManualPaymentEditDlg::OnCancel() 
{
	EndDialog( IDCANCEL );
}

/**********************************************************************/
#endif
/**********************************************************************/
