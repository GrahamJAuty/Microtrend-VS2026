/**********************************************************************/
 
/**********************************************************************/
#include "NodeTypes.h"
#include "Password.h"
#include "StockOptionsIni.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "StocktakeLevelsDlg.h"
/**********************************************************************/

CStocktakeLevelsDlg::CStocktakeLevelsDlg( int nSpIdx, CWnd* pParent)
	: CDialog(CStocktakeLevelsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStocktakeLevelsDlg)
	//}}AFX_DATA_INIT
	m_nSpIdx = nSpIdx;
}

/**********************************************************************/

void CStocktakeLevelsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStocktakeLevelsDlg)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStocktakeLevelsDlg, CDialog)
	//{{AFX_MSG_MAP(CStocktakeLevelsDlg)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ACTION, OnDoubleClickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStocktakeLevelsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strTitle;
	strTitle.Format( "Stocktake Levels (%s)", dbStockpoint.GetName(m_nSpIdx) );
	SetWindowText( strTitle );

	m_listAction.SubclassDlgItem( IDC_LIST_ACTION, this );
	m_listAction.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listAction.InsertColumn ( 0, "Action", LVCFMT_LEFT, 220 );

	int nListPos = 0;

	if ( PasswordArray.GetEnable( PasswordTicks::StocktakingLevels ) )
		AddOption( nListPos, "Set Closing Stock", NODE_CLOSING );

	AddOption( nListPos, "Period Deliveries", NODE_DELIVERY );
	AddOption( nListPos, "Period Returns", NODE_RETURN );

	switch( StockOptions.GetStockAdjustmentsEntryType() )
	{
	case 0:
		AddOption( nListPos, "Period Adjustments", NODE_ADJUSTMENT_STOCK );
		AddOption( nListPos, "Period Sales", NODE_MANUALSALE_STOCK );
		break;

	case 1:
		AddOption( nListPos, "Period Adjustments", NODE_ADJUSTMENT_PLU );
		AddOption( nListPos, "Period Sales", NODE_MANUALSALE_PLU );
		break;

	case 2:
		AddOption( nListPos, "Period Adjustments (Stock)", NODE_ADJUSTMENT_STOCK );
		AddOption( nListPos, "Period Adjustments (Plu)", NODE_ADJUSTMENT_PLU );
		AddOption( nListPos, "Period Sales (Stock)", NODE_MANUALSALE_STOCK );
		AddOption( nListPos, "Period Sales (Plu)", NODE_MANUALSALE_PLU );
		break;
	}

	m_listAction.SetCurSel(0);
	
	return TRUE;  
}

/**********************************************************************/

void CStocktakeLevelsDlg::AddOption( int& nListPos, const char* szText, int nNodeType )
{
	m_listAction.InsertItem ( nListPos, "" );
	m_listAction.SetItem ( nListPos, 0, LVIF_TEXT, szText, 0, 0, 0, NULL );
	m_arrayNodeTypes.Add( nNodeType );
	nListPos++;
}

/**********************************************************************/

void CStocktakeLevelsDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC_LIST_ACTION )	
	{
		m_listAction.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

void CStocktakeLevelsDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnOK();
	*pResult = 0;
}

/**********************************************************************/

void CStocktakeLevelsDlg::OnCancel() 
{
	EndDialog( IDCANCEL );
}

/**********************************************************************/

void CStocktakeLevelsDlg::OnOK() 
{
	m_nNodeType = NODE_CLOSING;
	
	int nSel = m_listAction.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_arrayNodeTypes.GetSize() ) )
		m_nNodeType = m_arrayNodeTypes.GetAt(nSel);

	EndDialog( IDOK );
}

/**********************************************************************/

