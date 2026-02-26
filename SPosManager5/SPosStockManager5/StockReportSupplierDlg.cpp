/**********************************************************************/
#include "StockReportSupplierDlg.h"
/**********************************************************************/

CStockReportSupplierDlg::CStockReportSupplierDlg( CWordArray& wSuppSelectArray, CWnd* pParent)
	: CDialog(CStockReportSupplierDlg::IDD, pParent), m_wSuppSelectArray ( wSuppSelectArray ), m_Helpers( m_listSuppliers, m_comboSupplier, m_SupplierFlags, wSuppSelectArray )
{
	//{{AFX_DATA_INIT(CStockReportSupplierDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CStockReportSupplierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockReportSupplierDlg)
	DDX_Control(pDX, IDC_COMBO_SUPPLIER, m_comboSupplier);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockReportSupplierDlg, CDialog)
	//{{AFX_MSG_MAP(CStockReportSupplierDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	ON_CBN_SELCHANGE(IDC_COMBO_SUPPLIER, OnSelectSupplierSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockReportSupplierDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_listSuppliers.SubclassDlgItem ( IDC_LIST, this );
	m_listSuppliers.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listSuppliers.InsertColumn ( 0, "", LVCFMT_LEFT, 250 );
	m_listSuppliers.SetLockItemZeroFlag( FALSE );

	m_Helpers.InitialiseSupplierFlags();
	m_Helpers.AddList ( m_SupplierFlags );
	m_Helpers.FillSupplierCombo();

	return TRUE;  
}

/**********************************************************************/

void CStockReportSupplierDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC_LIST )	
	{
		m_listSuppliers.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

void CStockReportSupplierDlg::OnSelectSupplierSet() 
{
	m_Helpers.HandleComboSelect();
}

/**********************************************************************/

void CStockReportSupplierDlg::OnButtonAll() 
{
	m_listSuppliers.SelectAll();
}

/**********************************************************************/

void CStockReportSupplierDlg::OnButtonNone() 
{
	m_listSuppliers.SelectNone();
}

/**********************************************************************/

void CStockReportSupplierDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_Helpers.UpdateSupplierSelection();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/

