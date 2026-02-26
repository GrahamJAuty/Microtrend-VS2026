/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
/**********************************************************************/
#include "StockDeleteSingleDlg.h"
/**********************************************************************/

CStockDeleteSingleDlg::CStockDeleteSingleDlg( CStockCSVRecord& StockRecord, CStockLevelsCSVRecord& StockLevels, CWnd* pParent)
	: CDialog(CStockDeleteSingleDlg::IDD, pParent), m_StockRecord( StockRecord ), m_StockLevels( StockLevels )
{
	//{{AFX_DATA_INIT(CStockDeleteSingleDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CStockDeleteSingleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockDeleteSingleDlg)
	DDX_Control(pDX, IDC_STATIC_ONORDER, m_staticOnOrder);
	DDX_Control(pDX, IDC_STATIC_DESCRIPTION, m_staticDescription);
	DDX_Control(pDX, IDC_STATIC_CODE, m_staticStockCode);
	DDX_Control(pDX, IDC_STATIC_CATEGORY, m_staticCategory);
	DDX_Control(pDX, IDC_STATIC_APPARENT, m_staticApparent);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockDeleteSingleDlg, CDialog)
	//{{AFX_MSG_MAP(CStockDeleteSingleDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockDeleteSingleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_staticStockCode.SetWindowText( m_StockRecord.GetStockCode() );
	m_staticDescription.SetWindowText ( m_StockRecord.GetDescription() );
	m_staticCategory.SetWindowText( DataManager.Category.GetText( m_StockRecord.GetCategory() ) );
	m_staticApparent.SetWindowText( DataManager.Stock.GetStockLevelStringForPluTab( m_StockRecord, m_StockLevels.GetApparentQty() ) );
	m_staticOnOrder.SetWindowText( DataManager.Stock.GetStockLevelStringForPluTab( m_StockRecord, m_StockLevels.GetQtyOnOrder() ) );
	return TRUE;  
}

/**********************************************************************/
