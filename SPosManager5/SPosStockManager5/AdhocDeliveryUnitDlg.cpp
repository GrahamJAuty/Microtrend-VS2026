/**********************************************************************/
#include "StockOptionsIni.h"
#include "SysInfo.h"
/**********************************************************************/
#include "AdhocDeliveryUnitDlg.h"
/**********************************************************************/

CAdhocDeliveryUnitDlg::CAdhocDeliveryUnitDlg( CStockCSVRecord& StockRecord, int nStockSuppIdx, int nInvoiceSuppNo, CWnd* pParent /*=NULL*/)
	: CDialog("IDD_ADHOC_DELIVERY_UNIT", pParent), m_StockRecord(StockRecord)
{
	//{{AFX_DATA_INIT(CAdhocDeliveryUnitDlg)
	//}}AFX_DATA_INIT
	m_nStockSuppIdx = nStockSuppIdx;
	m_nInvoiceSuppNo = nInvoiceSuppNo;
	m_nActionType = 1;
}

/**********************************************************************/

void CAdhocDeliveryUnitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdhocDeliveryUnitDlg)
	DDX_Control(pDX, IDC2_CHECK_PREFERRED, m_buttonPreferred);
	DDX_Control(pDX, IDC2_RADIO_ACTION1, m_radioAction1);
	DDX_Control(pDX, IDC2_RADIO_ACTION2, m_radioAction2);
	DDX_Control(pDX, IDC2_RADIO_ACTION3, m_radioAction3);
	DDX_Control(pDX, IDC2_RADIO_ACTION4, m_radioAction4);
	DDX_Control(pDX, IDC2_STATIC_UNIT, m_staticDeliveryUnit);
	DDX_Control(pDX, IDC2_STATIC_SUPPLIER, m_staticSupplier);
	DDX_Control(pDX, IDC2_STATIC_STOCKCODE, m_staticStockcode);
	DDX_Control(pDX, IDC2_STATIC_DESCRIPTION, m_staticDescription);
	DDX_Control(pDX, IDC2_STATIC_COST, m_staticCost);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CAdhocDeliveryUnitDlg, CDialog)
	//{{AFX_MSG_MAP(CAdhocDeliveryUnitDlg)
	ON_BN_CLICKED(IDC2_RADIO_ACTION1, OnRadioAction1)
	ON_BN_CLICKED(IDC2_RADIO_ACTION2, OnRadioAction2)
	ON_BN_CLICKED(IDC2_RADIO_ACTION3, OnRadioAction3)
	ON_BN_CLICKED(IDC2_RADIO_ACTION4, OnRadioAction4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CAdhocDeliveryUnitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int nDefault1 = 1;
	int nDefault2 = 1;

	CFilenameUpdater FnUp( SysFiles::AdhocDelConfig );

	CSSFile fileTest;
	if (fileTest.Open(FnUp.GetFilenameToUse(), "rb") == TRUE)
	{
		CString strBuffer;
		fileTest.ReadString(strBuffer);

		CCSV csv(strBuffer);
		nDefault1 = csv.GetInt(0);
		nDefault2 = csv.GetInt(1);

		if (nDefault1 < 1 || nDefault1 > 4)
		{
			nDefault1 = 1;
		}

		if (nDefault2 < 1 || nDefault2 > 2)
		{
			nDefault2 = 1;
		}
	}

	if (m_nStockSuppIdx < 0 || m_nStockSuppIdx >= m_StockRecord.GetSupplierCount())
	{
		m_nStockSuppIdx = 0;
	}

	int nInvoiceSuppIdx;
	DataManager.Supplier.FindSupplierByNumber( m_nInvoiceSuppNo, nInvoiceSuppIdx );
	
	CString strTitle;
	strTitle.Format ( "Edit Delivery Unit (%s)", DataManager.Supplier.GetName( nInvoiceSuppIdx ) );
	SetWindowText( strTitle );
	
	m_staticStockcode.SetWindowText( m_StockRecord.GetStockCode() );
	m_staticDescription.SetWindowText( m_StockRecord.GetDescription() );
	
	int nItemSuppIdx;
	int nItemSuppNo = m_StockRecord.GetSuppNo( m_nStockSuppIdx );
	if (DataManager.Supplier.FindSupplierByNumber(nItemSuppNo, nItemSuppIdx) == TRUE)
	{
		m_staticSupplier.SetWindowText(DataManager.Supplier.GetName(nItemSuppIdx));
	}
	else
	{
		m_staticSupplier.SetWindowText("Unknown");
	}

	CString strDelUnit;
	strDelUnit.Format ( "%s (%d x %.3f)",
		m_StockRecord.GetCase( m_nStockSuppIdx ),
		m_StockRecord.GetDUItems( m_nStockSuppIdx ),
		m_StockRecord.GetDUItemSize( m_nStockSuppIdx ) );

	m_staticDeliveryUnit.SetWindowText( strDelUnit );

	CString strCost = "";
	strCost.Format( "%.*f", SysInfo.GetDPValue(), m_StockRecord.GetCost( m_nStockSuppIdx ) );
	m_staticCost.SetWindowText( strCost );
	
	if ( m_StockRecord.GetSupplierCount() >= StockOptions.GetStockMaxSuppliersPerItem() )
	{
		m_radioAction3.ShowWindow( SW_HIDE );
		m_radioAction4.ShowWindow( SW_HIDE );

		switch ( nDefault2 )
		{
		case 1:	
			OnRadioAction1();
			break;

		case 2:	
			OnRadioAction2();
			break;

		default:	
			OnRadioAction1();
			break;
		}
	}
	else
	{
		switch ( nDefault1 )
		{
		case 1:
			OnRadioAction1();
			break;

		case 2:	
			OnRadioAction2();
			break;

		case 3:	
			OnRadioAction3();
			break;

		case 4:	
			OnRadioAction4();
			break;

		default:	
			OnRadioAction1();
			break;
		}
	}

	if ( StockOptions.GetStockMaxSuppliersPerItem() <= 1 )
	{
		m_buttonPreferred.SetCheck( FALSE );
		m_buttonPreferred.ShowWindow( SW_HIDE );
	}
	
	return TRUE;  
}

/**********************************************************************/

void CAdhocDeliveryUnitDlg::OnRadioAction1() 
{	
	m_radioAction1.SetCheck( TRUE );
	m_radioAction2.SetCheck( FALSE );
	m_radioAction3.SetCheck( FALSE );
	m_radioAction4.SetCheck( FALSE );
	m_nActionType = 1;
}

/**********************************************************************/

void CAdhocDeliveryUnitDlg::OnRadioAction2() 
{	
	m_radioAction1.SetCheck( FALSE );
	m_radioAction2.SetCheck( TRUE );
	m_radioAction3.SetCheck( FALSE );
	m_radioAction4.SetCheck( FALSE );
	m_nActionType = 2;
}

/**********************************************************************/

void CAdhocDeliveryUnitDlg::OnRadioAction3() 
{	
	m_radioAction1.SetCheck( FALSE );
	m_radioAction2.SetCheck( FALSE );
	m_radioAction3.SetCheck( TRUE );
	m_radioAction4.SetCheck( FALSE );
	m_nActionType = 3;
}

/**********************************************************************/

void CAdhocDeliveryUnitDlg::OnRadioAction4() 
{
	m_radioAction1.SetCheck( FALSE );
	m_radioAction2.SetCheck( FALSE );
	m_radioAction3.SetCheck( FALSE );
	m_radioAction4.SetCheck( TRUE );
	m_nActionType = 4;
}

/**********************************************************************/

void CAdhocDeliveryUnitDlg::OnOK() 
{
	if ( UpdateData( TRUE ) == TRUE )
	{
		switch( m_nActionType )
		{
		case 1:
			break;

		case 2:
			m_StockRecord.SetSuppNo( m_nStockSuppIdx, m_nInvoiceSuppNo );
			break;

		case 3:
			if ( m_StockRecord.GetSupplierCount() < StockOptions.GetStockMaxSuppliersPerItem() )
			{
				int nNewStockSuppIdx = m_StockRecord.GetSupplierCount();
				m_StockRecord.SetSuppNo( nNewStockSuppIdx, m_nInvoiceSuppNo );
				m_StockRecord.SetSuppRef( nNewStockSuppIdx, m_StockRecord.GetSuppRefInternal( m_nStockSuppIdx ) );
				m_StockRecord.SetWrapper( nNewStockSuppIdx, m_StockRecord.GetWrapper( m_nStockSuppIdx ) );
				m_StockRecord.SetCase( nNewStockSuppIdx, m_StockRecord.GetCase( m_nStockSuppIdx ) );
				m_StockRecord.SetDUItems( nNewStockSuppIdx, m_StockRecord.GetDUItems( m_nStockSuppIdx ) );
				m_StockRecord.SetDUItemSize( nNewStockSuppIdx, m_StockRecord.GetDUItemSize( m_nStockSuppIdx ) );
				m_StockRecord.SetCost( nNewStockSuppIdx, m_StockRecord.GetCost( m_nStockSuppIdx ) );
				m_StockRecord.TidySuppliers( StockOptions.GetStockMaxSuppliersPerItem() );
				m_nStockSuppIdx = nNewStockSuppIdx;
			}
			break;

		case 4:
			if ( m_StockRecord.GetSupplierCount() < StockOptions.GetStockMaxSuppliersPerItem() )
			{
				int nNewStockSuppIdx = m_StockRecord.GetSupplierCount();
				m_StockRecord.SetSuppNo( nNewStockSuppIdx, m_nInvoiceSuppNo );
				m_StockRecord.SetSuppRef( nNewStockSuppIdx, "" );
				m_StockRecord.SetWrapper( nNewStockSuppIdx, "" );
				m_StockRecord.SetCase( nNewStockSuppIdx, "" );
				m_StockRecord.SetDUItems( nNewStockSuppIdx, 1 );
				m_StockRecord.SetDUItemSize( nNewStockSuppIdx, 1.0 );
				m_StockRecord.SetCost( nNewStockSuppIdx, 0.0 );
				m_StockRecord.TidySuppliers( StockOptions.GetStockMaxSuppliersPerItem() );
				m_nStockSuppIdx = nNewStockSuppIdx;
			}
			break;
		}

		if ( m_buttonPreferred.GetCheck() != 0 )
		{
			m_StockRecord.MakePreferred( m_nStockSuppIdx );
			m_nStockSuppIdx = 0;
		}

		EndDialog ( IDOK );
	}
}

/**********************************************************************/
