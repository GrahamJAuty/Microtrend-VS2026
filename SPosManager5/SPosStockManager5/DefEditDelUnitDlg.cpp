/**********************************************************************/
#include "MaxLengths.h"
#include "..\CommonStockTray\StockCSVArray.h"
#include "..\CommonEcrStockTray\SysInfo.h"
/**********************************************************************/
#include "DefEditDelUnitDlg.h"
/**********************************************************************/

CDefEditDelUnitDlg::CDefEditDelUnitDlg( const char* szTitle, CWnd* pParent )
	: CSSDialog(CDefEditDelUnitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDefEditDelUnitDlg)
	m_dDUItemSize = 0.0;
	m_nDUItems = 0;
	//}}AFX_DATA_INIT
	m_strTitle = szTitle;
}

/**********************************************************************/

void CDefEditDelUnitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditDelUnitDlg)
	DDX_Control(pDX, IDC_EDIT_YIELD, m_EditDelUnit);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_ITEMS, m_nDUItems);
	DDV_MinMaxInt(pDX, m_nDUItems, Stock::DUItems.Min, Stock::DUItems.Max); 
	DDX_Text(pDX, IDC_EDIT_SUPERITEM, m_dDUItemSize);
	DDV_MinMaxDouble(pDX, m_dDUItemSize, Stock::DUItemSize.Min, Stock::DUItemSize.Max); 
	DDX_Text(pDX, IDC_EDIT_DELIVERY_UNIT, m_strDeliveryUnit);
	DDV_MaxChars(pDX, m_strDeliveryUnit, MAX_LENGTH_UNIT_NAME); 
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditDelUnitDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditDelUnitDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_SUPERITEM, OnKillFocusYield)
	ON_EN_KILLFOCUS(IDC_EDIT_ITEMS, OnKillFocusYield)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditDelUnitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowText ( m_strTitle );
	SubclassEdit ( IDC_EDIT_SUPERITEM, SS_NUM_DP, 8, "%.3f" );
	SubclassEdit ( IDC_EDIT_ITEMS, SS_NUM, 6, "%d" );
	UpdateYieldDisplay();
	return TRUE;  
}

/**********************************************************************/

void CDefEditDelUnitDlg::OnKillFocusYield() 
{
	UpdateData( TRUE, FALSE );
	UpdateYieldDisplay();
}

/**********************************************************************/

void CDefEditDelUnitDlg::UpdateYieldDisplay()
{
	CString strYield;
	strYield.Format ( "%.3f", m_dDUItemSize * m_nDUItems );
	m_EditDelUnit.SetWindowText( strYield );
}

/**********************************************************************/


