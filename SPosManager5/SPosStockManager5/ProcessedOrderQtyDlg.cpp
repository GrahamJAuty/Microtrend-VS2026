/**********************************************************************/
 
/**********************************************************************/
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "ProcessedOrderQtyDlg.h"
/**********************************************************************/

CProcessedOrderQtyDlg::CProcessedOrderQtyDlg( int nSpIdx, int nType, int nDUItems, double dDUItemSize, CWnd* pParent)
	: CSSDialog(CProcessedOrderQtyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcessedOrderQtyDlg)
	m_dStockUnits = 0.0;
	m_nSupplierItems = 0;
	m_nDelUnits = 0;
	m_nType = 0;
	//}}AFX_DATA_INIT
	m_nSpIdx = nSpIdx;
	m_nType = nType;
	m_nDUItems = nDUItems;
	m_dDUItemSize = dDUItemSize;
}

/**********************************************************************/

void CProcessedOrderQtyDlg::SetDUItemOrder( int nOrder )
{
	m_nSupplierItems = nOrder;
	m_dStockUnits = m_dDUItemSize * nOrder;

	if ( m_nDUItems >= 1 )
	{
		m_nDelUnits = m_nSupplierItems / m_nDUItems;
		if ( ( m_nDelUnits * m_nDUItems ) < m_nSupplierItems )
			m_nDelUnits++;
	}
	else
		m_nDelUnits = 0;
}

/**********************************************************************/

void CProcessedOrderQtyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessedOrderQtyDlg)
	DDX_Control(pDX, IDC_STATIC_SUPPLIERITEMS, m_staticSupplierItems);
	DDX_Control(pDX, IDC_STATIC_DELUNITS, m_staticDelUnits);
	DDX_Control(pDX, IDC_RADIO_DELUNITS, m_radioDelUnits);
	DDX_Control(pDX, IDC_RADIO_SUPPLIERITEMS, m_radioSupplierItems);
	DDX_Control(pDX, IDC_RADIO_STOCKUNIT, m_radioStockUnits);
	DDX_Radio(pDX, IDC_RADIO_STOCKUNIT, m_nType);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_STOCKUNITS, m_dStockUnits);
	DDX_Text(pDX, IDC_EDIT_SUPPLIERITEMS, m_nSupplierItems);
	DDX_Text(pDX, IDC_EDIT_DELUNITS, m_nDelUnits);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CProcessedOrderQtyDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessedOrderQtyDlg)
	ON_BN_CLICKED(IDC_RADIO_SUPPLIERITEMS, OnRadioSupplierItems)
	ON_BN_CLICKED(IDC_RADIO_STOCKUNIT, OnRadioStockUnits)
	ON_BN_CLICKED(IDC_RADIO_DELUNITS, OnRadioDelUnits)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CProcessedOrderQtyDlg::GetEditStockUnits() { return GetEdit( IDC_EDIT_STOCKUNITS ); }
CEdit* CProcessedOrderQtyDlg::GetEditSupplierItems() { return GetEdit( IDC_EDIT_SUPPLIERITEMS ); }
CEdit* CProcessedOrderQtyDlg::GetEditDelUnits() { return GetEdit( IDC_EDIT_DELUNITS ); }
/**********************************************************************/

BOOL CProcessedOrderQtyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit ( IDC_EDIT_STOCKUNITS, SS_NUM_DP, 8, "%.3f" );
	SubclassEdit ( IDC_EDIT_SUPPLIERITEMS, SS_NUM, 6, "%d" );
	SubclassEdit ( IDC_EDIT_DELUNITS, SS_NUM, 6, "%d" );

	CString strTitle = "Edit Quantity";

	CString strExtra;
	strExtra.Format ( " (%s)", dbStockpoint.GetName( m_nSpIdx ) );
	strTitle += strExtra;
	
	SetWindowText ( strTitle );

	switch ( m_nType )
	{
	case 1:		OnRadioSupplierItems();		break;
	case 2:		OnRadioDelUnits();			break;
	case 0:
	default:		OnRadioStockUnits();		break;
	}

	CString strLabel;
	strLabel.Format ( "( 1 x %.3f )", m_dDUItemSize );
	m_staticSupplierItems.SetWindowText( strLabel );
	strLabel.Format ( "( %d x %.3f )", m_nDUItems, m_dDUItemSize );
	m_staticDelUnits.SetWindowText( strLabel );
		
	return TRUE;  
}

/**********************************************************************/

void CProcessedOrderQtyDlg::OnRadioStockUnits() 
{
	GetEditStockUnits() -> EnableWindow( TRUE );
	GetEditSupplierItems() -> EnableWindow( FALSE );
	GetEditDelUnits() -> EnableWindow( FALSE );
}

/**********************************************************************/

void CProcessedOrderQtyDlg::OnRadioSupplierItems() 
{
	GetEditStockUnits() -> EnableWindow( FALSE );
	GetEditSupplierItems() -> EnableWindow( TRUE );
	GetEditDelUnits() -> EnableWindow( FALSE );
}

/**********************************************************************/

void CProcessedOrderQtyDlg::OnRadioDelUnits() 
{
	GetEditStockUnits() -> EnableWindow( FALSE );
	GetEditSupplierItems() -> EnableWindow( FALSE );
	GetEditDelUnits() -> EnableWindow( TRUE );
}

/**********************************************************************/
