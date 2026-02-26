/**********************************************************************/
#include "StockReportSupplierProfitDlg.h"
/**********************************************************************/

CStockReportSupplierProfitDlg::CStockReportSupplierProfitDlg( CWordArray& wSuppSelectArray, CWnd* pParent)
	: CDialog(CStockReportSupplierProfitDlg::IDD, pParent), m_wSuppSelectArray ( wSuppSelectArray ), m_Helpers( m_listSuppliers, m_comboSupplier, m_SupplierFlags, wSuppSelectArray )
{
	//{{AFX_DATA_INIT(CStockReportSupplierProfitDlg)
	//}}AFX_DATA_INIT
	m_bAlphaSort = FALSE;
}

/**********************************************************************/

void CStockReportSupplierProfitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockReportSupplierProfitDlg)
	DDX_Control(pDX, IDC_COMBO_SUPPLIER, m_comboSupplier);
	DDX_Check(pDX, IDC_CHECK_ALPHASORT, m_bAlphaSort);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockReportSupplierProfitDlg, CDialog)
	//{{AFX_MSG_MAP(CStockReportSupplierProfitDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	ON_CBN_SELCHANGE(IDC_COMBO_SUPPLIER, OnSelectSupplierSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockReportSupplierProfitDlg::OnInitDialog() 
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

void CStockReportSupplierProfitDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC_LIST )	
	{
		m_listSuppliers.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

void CStockReportSupplierProfitDlg::OnSelectSupplierSet() 
{
	m_Helpers.HandleComboSelect();
}

/**********************************************************************/

void CStockReportSupplierProfitDlg::OnButtonAll() 
{
	m_listSuppliers.SelectAll();
}

/**********************************************************************/

void CStockReportSupplierProfitDlg::OnButtonNone() 
{
	m_listSuppliers.SelectNone();
}

/**********************************************************************/

void CStockReportSupplierProfitDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_Helpers.UpdateSupplierSelection();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/


