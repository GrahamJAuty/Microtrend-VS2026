/**********************************************************************/
#include "PropPageStockpointTransfers.h"
/**********************************************************************/

CPropPageStockpointTransfers::CPropPageStockpointTransfers() : CPropertyPage(CPropPageStockpointTransfers::IDD)
{
	//{{AFX_DATA_INIT(CPropPageStockpointTransfers)
	//}}AFX_DATA_INIT
	m_pStockpointBuffer = NULL;
}

/**********************************************************************/

CPropPageStockpointTransfers::~CPropPageStockpointTransfers()
{
}

/**********************************************************************/

void CPropPageStockpointTransfers::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageStockpointTransfers)
	//}}AFX_DATA_MAP  
	DDX_Control(pDX, IDC_CHECK_TRANSFER1, m_checkTransfer1);
	DDX_Control(pDX, IDC_CHECK_TRANSFER3, m_checkTransfer3);
	DDX_Control(pDX, IDC_CHECK_TRANSFER4, m_checkTransfer4);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageStockpointTransfers, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageStockpointTransfers)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageStockpointTransfers::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	CString str;
	str.Format( "Mark this stockpoint as a %s in tree and list displays", DealerFlags.GetWarehouseType( FALSE ) );
	m_checkTransfer1.SetWindowText( str );

	m_checkTransfer1.SetCheck( m_pStockpointBuffer -> GetWarehouseFlag(0) );
	m_checkTransfer3.SetCheck( m_pStockpointBuffer -> GetTransferOutFlag(0) );
	m_checkTransfer4.SetCheck( m_pStockpointBuffer -> GetTransferInFlag(0) );
	
	return TRUE;    
}

/**********************************************************************/

BOOL CPropPageStockpointTransfers::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageStockpointTransfers::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPageStockpointTransfers::GetRecordData()
{
}

/**********************************************************************/

void CPropPageStockpointTransfers::SaveRecord()
{
	m_pStockpointBuffer -> SetWarehouseFlag( 0, m_checkTransfer1.GetCheck() != 0 );
	m_pStockpointBuffer -> SetTransferOutFlag( 0, m_checkTransfer3.GetCheck() != 0 );
	m_pStockpointBuffer -> SetTransferInFlag( 0, m_checkTransfer4.GetCheck() != 0 );
}

/**********************************************************************/



