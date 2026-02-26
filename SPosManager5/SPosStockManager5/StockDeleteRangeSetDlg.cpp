/**********************************************************************/
 
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "CategorySetCSVArray.h"
#include "DefTextCategory.h"
#include "..\CommonStockTray\StockCSVArray.h"
/**********************************************************************/
#include "StockDeleteRangeSetDlg.h"
/**********************************************************************/

CStockDeleteRangeSetDlg::CStockDeleteRangeSetDlg( CWnd* pParent )
	: CSSDialog(CStockDeleteRangeSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStockDeleteRangeSetDlg)
	//}}AFX_DATA_INIT
	m_nDelete = 0;
	m_nCategoryFilter = 0;
}

/**********************************************************************/

void CStockDeleteRangeSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockDeleteRangeSetDlg)
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Radio(pDX, IDC_RADIO_YES, m_nDelete);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_STOCK_RANGE_START, m_strStockCodeFrom);
	DDV_MaxChars(pDX, m_strStockCodeFrom, Stock::StockCode.Max );
	DDX_Text(pDX, IDC_EDIT_STOCK_RANGE_END, m_strStockCodeTo);
	DDV_MaxChars(pDX, m_strStockCodeTo, Stock::StockCode.Max );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockDeleteRangeSetDlg, CDialog)
	//{{AFX_MSG_MAP(CStockDeleteRangeSetDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCK_RANGE_END, OnKillFocusStockCode)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCK_RANGE_START, OnKillFocusStockCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockDeleteRangeSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	FillCategoryCombo();
	m_comboCategory.SetCurSel(0);
	return TRUE;  
}

/**********************************************************************/

void CStockDeleteRangeSetDlg::OnKillFocusStockCode() 
{
	UpdateData( TRUE, FALSE );
	m_strStockCodeFrom = BarcodePluNoTable.ConvertStockCode ( m_strStockCodeFrom );
	m_strStockCodeTo = BarcodePluNoTable.ConvertStockCode ( m_strStockCodeTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CStockDeleteRangeSetDlg::FillCategoryCombo()
{
	m_CategorySelector.SelectCategorySet(0);
	m_CategorySelector.IncludeAll();
	m_CategorySelector.IncludeCategorySets();
	m_CategorySelector.FillCategoryCombo ( &m_comboCategory );
}

/**********************************************************************/

void CStockDeleteRangeSetDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_nCategoryFilter = m_CategorySelector.GetSelection();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/

