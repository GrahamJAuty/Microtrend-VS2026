/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "AdhocPreferredSupplierDlg.h"
/**********************************************************************/

CAdhocPreferredSupplierDlg::CAdhocPreferredSupplierDlg( int nStockIdx, int nStockSuppIdx, CWnd* pParent)
	: CDialog("IDD_ADHOC_PREFERRED", pParent)
{
	//{{AFX_DATA_INIT(CAdhocPreferredSupplierDlg)
	//}}AFX_DATA_INIT
	m_nStockIdx = nStockIdx;
	m_nStockSuppIdx = nStockSuppIdx;
}

/**********************************************************************/

void CAdhocPreferredSupplierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdhocPreferredSupplierDlg)
	DDX_Control(pDX, IDC2_STATIC_DUITEMSIZE2, m_staticDUItemSize2);
	DDX_Control(pDX, IDC2_STATIC_DUITEMSIZE1, m_staticDUItemSize1);
	DDX_Control(pDX, IDC2_STATIC_DUITEMS2, m_staticDUItems2);
	DDX_Control(pDX, IDC2_STATIC_DUITEMS1, m_staticDUItems1);
	DDX_Control(pDX, IDC2_STATIC_COST2, m_staticCost2);
	DDX_Control(pDX, IDC2_STATIC_COST1, m_staticCost1);
	DDX_Control(pDX, IDC2_STATIC_WRAPPER2, m_staticWrapper2);
	DDX_Control(pDX, IDC2_STATIC_WRAPPER1, m_staticWrapper1);
	DDX_Control(pDX, IDC2_STATIC_UNIT2, m_staticUnit2);
	DDX_Control(pDX, IDC2_STATIC_UNIT1, m_staticUnit1);
	DDX_Control(pDX, IDC2_STATIC_CODE2, m_staticCode2);
	DDX_Control(pDX, IDC2_STATIC_CODE1, m_staticCode1);
	DDX_Control(pDX, IDC2_STATIC_NAME2, m_staticName2);
	DDX_Control(pDX, IDC2_STATIC_NAME1, m_staticName1);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CAdhocPreferredSupplierDlg, CDialog)
	//{{AFX_MSG_MAP(CAdhocPreferredSupplierDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CAdhocPreferredSupplierDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt( m_nStockIdx, StockRecord );

	CString strTitle;
	strTitle.Format ( "Change Preferred Delivery Method (%s, %s)",
		StockRecord.GetStockCode(),
		StockRecord.GetDescription() );

	SetWindowText( strTitle );

	int nSuppIdx1;
	int nSuppNo1 = StockRecord.GetSuppNo( m_nStockSuppIdx );
	
	if (DataManager.Supplier.FindSupplierByNumber(nSuppNo1, nSuppIdx1) == TRUE)
	{
		m_staticName1.SetWindowText(DataManager.Supplier.GetName(nSuppIdx1));
	}
	else
	{
		m_staticName1.SetWindowText("Unknown");
	}

	int nSuppIdx2;
	int nSuppNo2 = StockRecord.GetSuppNo( 0 );
	
	if (DataManager.Supplier.FindSupplierByNumber(nSuppNo2, nSuppIdx2) == TRUE)
	{
		m_staticName2.SetWindowText(DataManager.Supplier.GetName(nSuppIdx2));
	}
	else
	{
		m_staticName2.SetWindowText("Unknown");
	}

	m_staticUnit1.SetWindowText( StockRecord.GetCase( m_nStockSuppIdx ) );
	m_staticUnit2.SetWindowText( StockRecord.GetCase( 0 ) );

	m_staticCode1.SetWindowText( StockRecord.GetSuppRefExternal( m_nStockSuppIdx, DealerFlags.GetSuppRefFlag() ) );
	m_staticCode2.SetWindowText( StockRecord.GetSuppRefExternal( 0, DealerFlags.GetSuppRefFlag() ) );

	m_staticWrapper1.SetWindowText( StockRecord.GetWrapper( m_nStockSuppIdx ) );
	m_staticWrapper2.SetWindowText( StockRecord.GetWrapper( 0 ) );

	CString strItems;
	strItems.Format ( "%d", StockRecord.GetDUItems( m_nStockSuppIdx ) );
	m_staticDUItems1.SetWindowText( strItems );
	strItems.Format ( "%d", StockRecord.GetDUItems( 0 ) );
	m_staticDUItems2.SetWindowText( strItems );

	CString strItemSize;
	strItemSize.Format ( "%.3f", StockRecord.GetDUItemSize( m_nStockSuppIdx ) );
	m_staticDUItemSize1.SetWindowText( strItemSize );
	strItemSize.Format ( "%.3f", StockRecord.GetDUItemSize( 0 ) );
	m_staticDUItemSize2.SetWindowText( strItemSize );
	
	CString strCost;
	strCost.Format( "%.*f", SysInfo.GetDPValue(), StockRecord.GetCost( m_nStockSuppIdx ) );
	m_staticCost1.SetWindowText( strCost );
	strCost.Format( "%.*f", SysInfo.GetDPValue(), StockRecord.GetCost( 0 ) );
	m_staticCost2.SetWindowText( strCost );

	return TRUE;  
}

/**********************************************************************/
