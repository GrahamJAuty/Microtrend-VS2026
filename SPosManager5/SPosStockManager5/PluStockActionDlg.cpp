/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
#include "StockOptionsIni.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "PluStockActionDlg.h"
/**********************************************************************/

CPluStockActionDlg::CPluStockActionDlg( CRecipeCSVRecord& RecipeRecord, CWnd* pParent )
	: CSSDialog(CPluStockActionDlg::IDD, pParent), m_RecipeRecord( RecipeRecord )
{
	//{{AFX_DATA_INIT(CPluStockActionDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CPluStockActionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluStockActionDlg)
	DDX_Control(pDX, IDC_COMBO_STOCKPOINT, m_comboStockpoint);
	DDX_Control(pDX, IDC_COMBO_SALES, m_comboSalesAction);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluStockActionDlg, CDialog)
	//{{AFX_MSG_MAP(CPluStockActionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluStockActionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(60);
	m_comboSalesAction.SetTabStops( arrayStops );
			
	FillStockpointCombo();
	FillSalesActionCombo();

	if ( StockOptions.GetPluRedirectToStockpointFlag() == FALSE )
	{
		m_comboStockpoint.SetCurSel(0);
		m_comboStockpoint.EnableWindow( FALSE );
	}

	return FALSE;  
}

/**********************************************************************/

void CPluStockActionDlg::FillStockpointCombo()
{
	m_comboStockpoint.AddString( "As per EPOS link" );
	m_comboStockpoint.SetItemData( 0, 0 );

	int nStartSpIdx, nEndSpIdx;
	dbStockpoint.GetSpIdxRange( DataManager.GetActiveDbIdx(), nStartSpIdx, nEndSpIdx );

	for ( int n = nStartSpIdx; n <= nEndSpIdx; n++ )
	{
		int nPos = m_comboStockpoint.AddString( dbStockpoint.GetName(n) );
		m_comboStockpoint.SetItemData( nPos, dbStockpoint.GetSpNo(n) );
	}
	
	m_comboStockpoint.EnableWindow( m_comboStockpoint.GetCount() > 1 );

	m_comboStockpoint.SetCurSel(0);
	for ( int n = 1; n < m_comboStockpoint.GetCount(); n++ )
	{
		if ( m_comboStockpoint.GetItemData(n) == m_RecipeRecord.GetSpNo() )
		{
			m_comboStockpoint.SetCurSel(n);
			break;
		}
	}
}

/**********************************************************************/

void CPluStockActionDlg::FillSalesActionCombo()
{
	m_comboSalesAction.AddItem( "Normal,(reduce apparent stock)" );
	m_comboSalesAction.AddItem( "Reverse,(increase apparent stock)" );
	m_comboSalesAction.SetCurSel( m_RecipeRecord.GetReverseSaleFlag() ? 1 : 0 );
}

/**********************************************************************/

void CPluStockActionDlg::OnOK() 
{
	if ( StockOptions.GetPluRedirectToStockpointFlag() == TRUE )
	{
		int nSel = m_comboStockpoint.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_comboStockpoint.GetCount() ) )
			m_RecipeRecord.SetSpNo( m_comboStockpoint.GetItemData(nSel) );
	}

	m_RecipeRecord.SetReverseSaleFlag( m_comboSalesAction.GetCurSel() == 1 );

	EndDialog( IDOK );
}

/**********************************************************************/
