/*********************************************************************/
 
/*********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DealerFlags.h"
#include "NodeTypes.h"
#include "StockpointCSVArray.h"
/*********************************************************************/
#include "SetupWarehouseDlg.h"
/*********************************************************************/

IMPLEMENT_DYNAMIC(CSetupWarehouseDlg, CDialog)

CSetupWarehouseDlg::CSetupWarehouseDlg( int nDbIdx, CWnd* pParent /*=NULL*/)
	: CDialog(CSetupWarehouseDlg::IDD, pParent)
{
	m_nDbIdx = nDbIdx;
	m_ImageList.Create ( IDB_ICONS_COMBO, 31, 0, COLORREF(0xFFFFFF) );
	
	m_nSpIdx = -1;
	m_nTransfer1 = 0;
	m_nTransfer2 = 0;
}

/*********************************************************************/

CSetupWarehouseDlg::~CSetupWarehouseDlg()
{
}

/*********************************************************************/

void CSetupWarehouseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_STATIC1, m_static1);
	DDX_Control(pDX,IDC_STATIC2, m_static2);
	DDX_Control(pDX,IDC_COMBO_LOCATION, m_comboStockpoint);
	DDX_Control(pDX,IDC_COMBO_TRANSFER1, m_comboTransfer1);
	DDX_Control(pDX,IDC_COMBO_TRANSFER2, m_comboTransfer2);
}

/*********************************************************************/
BEGIN_MESSAGE_MAP(CSetupWarehouseDlg, CDialog)
END_MESSAGE_MAP()
/*********************************************************************/

BOOL CSetupWarehouseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_comboStockpoint.SetImageList( &m_ImageList );

	CString str;
	str.Format( "Select %s Stockpoint (%s)",
		DealerFlags.GetWarehouseType( TRUE ),
		dbDatabase.GetName( m_nDbIdx ) );
	SetWindowText( str );

	str.Format( "%s Stockpoint",
		DealerFlags.GetWarehouseType( TRUE ) );
	m_static1.SetWindowText( str );

	str.Format( "Into %s",
		DealerFlags.GetWarehouseType( FALSE ) );
	m_static2.SetWindowText( str );

	m_comboTransfer1.AddString( "No Change" );
	m_comboTransfer1.AddString( "Allow" );
	m_comboTransfer1.AddString( "Prevent" );
	m_comboTransfer1.SetCurSel(0);

	m_comboTransfer2.AddString( "No Change" );
	m_comboTransfer2.AddString( "Allow" );
	m_comboTransfer2.AddString( "Prevent" );
	m_comboTransfer2.SetCurSel(0);

	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange( m_nDbIdx, nStartIdx, nEndIdx );

	AddStockpointToCombo( "No Change", NODE_COMBOICON_NONE );
	
	for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
	{
		int nType;
		if ( dbStockpoint.GetWarehouseFlag( nSpIdx ) == TRUE )
			nType = NODE_COMBOICON_WAREHOUSE;
		else
			nType = NODE_COMBOICON_STOCKPOINT;

		AddStockpointToCombo( dbStockpoint.GetName( nSpIdx ), nType );
	}

	AddStockpointToCombo( "None", NODE_COMBOICON_NONE );
	
	m_comboStockpoint.SetCurSel(0);

	return TRUE;  
}

/*********************************************************************/

void CSetupWarehouseDlg::AddStockpointToCombo ( const char* szText, int nImage )
{
	char* p = (char*) szText;

	COMBOBOXEXITEM item;
	item.mask = CBEIF_DI_SETITEM | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
	item.iItem = m_comboStockpoint.GetCount();
	item.pszText = p;
	item.iImage = nImage;
	item.iSelectedImage = nImage;

	m_comboStockpoint.InsertItem ( &item );
}

/**********************************************************************/

void CSetupWarehouseDlg::OnOK()
{
	m_nTransfer1 = m_comboTransfer1.GetCurSel();
	m_nTransfer2 = m_comboTransfer2.GetCurSel();

	int nSel = m_comboStockpoint.GetCurSel();
	
	if ( 0 == nSel )
		m_nSpIdx = -1;
	else if ( ( m_comboStockpoint.GetCount() - 1 ) == nSel )
		m_nSpIdx = -2;
	else
	{
		int nStartIdx, nEndIdx;
		dbStockpoint.GetSpIdxRange( m_nDbIdx, nStartIdx, nEndIdx );
		m_nSpIdx = nStartIdx + nSel - 1;
	}

	EndDialog( IDOK );
}

/**********************************************************************/
