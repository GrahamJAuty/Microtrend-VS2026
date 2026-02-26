/**********************************************************************/
#include "PropPageHiddenOptionsStockman.h"
/**********************************************************************/

CPropPageHiddenOptionsStockman::CPropPageHiddenOptionsStockman() : CSSPropertyPage(CPropPageHiddenOptionsStockman::IDD)
{
	//{{AFX_DATA_INIT(CPropPageHiddenOptionsStockman)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPageHiddenOptionsStockman::~CPropPageHiddenOptionsStockman()
{
}

/**********************************************************************/

void CPropPageHiddenOptionsStockman::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageHiddenOptionsStockman)
	DDX_Control(pDX, IDC_COMBO_PLUFLAG9, m_comboPluFlag9);
	DDX_Control(pDX, IDC_CHECK_PLUFLAG9, m_checkPluFlag9);
	DDX_Control(pDX, IDC_CHECK_PLUFLAG11, m_checkPluFlag11);
	DDX_Control(pDX, IDC_CHECK_STOCKFLAG6, m_checkStockFlag6);
	DDX_Control(pDX, IDC_CHECK_STOCKFLAG7, m_checkStockFlag7);
	DDX_Control(pDX, IDC_CHECK_STOCKFLAG8, m_checkStockFlag8);
	DDX_Control(pDX, IDC_CHECK_STOCKFLAG10, m_checkStockFlag10);
	DDX_Control(pDX, IDC_CHECK_STOCKFLAG11, m_checkStockFlag11);
	DDX_Control(pDX, IDC_CHECK_STOCKFLAG12, m_checkStockFlag12);
	DDX_Control(pDX, IDC_CHECK_STOCKFLAG13, m_checkStockFlag13);
	DDX_Control(pDX, IDC_CHECK_AUDITFLAG2, m_checkAuditFlag2);
	DDX_Control(pDX, IDC_CHECK_STOCKFLAG2, m_checkStockFlag2);
	DDX_Control(pDX, IDC_CHECK_STOCKFLAG1, m_checkStockFlag1);
	DDX_Control(pDX, IDC_CHECK_ORDERFLAG1, m_checkOrderFlag1);
	DDX_Control(pDX, IDC_CHECK_AUDITFLAG1, m_checkAuditFlag1);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_LEVELS_PLU, m_comboStockFlag4);
	DDX_Control(pDX, IDC_COMBO_LEVELS_STOCK, m_comboStockFlag5);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageHiddenOptionsStockman, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageHiddenOptionsStockman)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_PLUFLAG9, OnCheckPluFlag9)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageHiddenOptionsStockman::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();	
	
	m_comboStockFlag4.AddString( "Read/write access to stock levels" );
	m_comboStockFlag4.AddString( "Read only access to stock levels" );
	m_comboStockFlag4.AddString( "No access to stock levels" );
	m_comboStockFlag4.AddString( "Prompt when opening database" );
	m_comboStockFlag4.SetCurSel(0);
	m_comboStockFlag5.AddString( "Read/write access to stock levels" );
	m_comboStockFlag5.AddString( "Read only access to stock levels" );
	m_comboStockFlag5.AddString( "No access to stock levels" );
	m_comboStockFlag5.AddString( "Prompt when opening database" );
	m_comboStockFlag5.SetCurSel(0);

	UpdateControls();

	return TRUE;  
}

/**********************************************************************/

void CPropPageHiddenOptionsStockman::UpdateControls()
{		
	//PLU FLAG 9
	m_checkPluFlag9.SetCheck( DealerFlags.GetPluCSVImportType() != 0 );
	OnCheckPluFlag9();
	
	int nSel = DealerFlags.GetPluCSVImportType();
	if ( nSel >= 0 && nSel < m_comboPluFlag9.GetCount() )
		m_comboPluFlag9.SetCurSel( nSel );

	//PLU FLAG 11
	m_checkPluFlag11.SetCheck( DealerFlags.GetImportBarcodeFlag() );
	
	//STOCK FLAG 1
	m_checkStockFlag1.SetCheck( DealerFlags.GetSuppRefFlag() );
	
	//STOCK FLAG 2
	m_checkStockFlag2.SetCheck( DealerFlags.GetCopyPluToStockFlag() );
	
	//STOCK FLAG 6
	m_checkStockFlag6.SetCheck( DealerFlags.GetTransitionalOrderFlag() );

	//STOCK FLAG 7
	m_checkStockFlag7.SetCheck( DealerFlags.GetStockLevelFilterFlag() );

	//STOCK FLAG 8
	m_checkStockFlag8.SetCheck( DealerFlags.GetExtendedStocktakeReportsFlag() );

	//STOCK FLAG 10
	m_checkStockFlag10.SetCheck( DealerFlags.GetWarehouseToggleFlag() );

	//STOCK FLAG 11
	m_checkStockFlag11.SetCheck( DealerFlags.GetExportLiveApparentStockFlag() );

	//STOCK FLAG 12
	m_checkStockFlag12.SetCheck( DealerFlags.GetPeriodDeliveryCostProfitFlag() );

	//STOCK FLAG 13
	m_checkStockFlag13.SetCheck( DealerFlags.GetStockReportUseRecipeCostLinkFlag() );

	//STOCK FLAG 4
	nSel = DealerFlags.GetStockLevelsPluType();
	if ( nSel >= 0 && nSel < m_comboStockFlag4.GetCount() )
		m_comboStockFlag4.SetCurSel(nSel);

	//STOCK FLAG 4
	nSel = DealerFlags.GetStockLevelsStockType();
	if ( nSel >= 0 && nSel < m_comboStockFlag5.GetCount() )
		m_comboStockFlag5.SetCurSel(nSel);

	//AUDIT FLAG 1
	m_checkAuditFlag1.SetCheck( DealerFlags.GetDeliveryLinesFlag() );
	
	//AUDIT FLAG 2
	m_checkAuditFlag2.SetCheck( DealerFlags.GetDeliveryDetailFlag() );
	
	//ORDER FLAG 1
	m_checkOrderFlag1.SetCheck( DealerFlags.GetPurchaseOrderLinesFlag() );
}

/***************************************************************/

void CPropPageHiddenOptionsStockman::OnCheckPluFlag9() 
{
	bool bEnable = ( m_checkPluFlag9.GetCheck() != 0 );
	
	m_comboPluFlag9.ResetContent();
	m_comboPluFlag9.AddString( "PLU, QTY, VALUE" );

	if ( bEnable == TRUE )
	{	
		m_comboPluFlag9.AddString( "PLU, QTY" );
		m_comboPluFlag9.AddString( "PLU, LEVEL, QTY, VALUE" );
		m_comboPluFlag9.AddString( "PLU, LEVEL, QTY" );
	}
	
	m_comboPluFlag9.EnableWindow( bEnable );
	m_comboPluFlag9.SetCurSel(0);
}

/***************************************************************/

BOOL CPropPageHiddenOptionsStockman::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	
	//PLU FLAG 9
	DealerFlags.SetPluCSVImportType( m_comboPluFlag9.GetCurSel() );

	//PLU FLAG 11
	DealerFlags.SetImportBarcodeFlag( m_checkPluFlag11.GetCheck() != 0 );

	//STOCK FLAG 1
	DealerFlags.SetSuppRefFlag( m_checkStockFlag1.GetCheck() != 0 );
		
	//STOCK FLAG 2
	DealerFlags.SetCopyPluToStockFlag( m_checkStockFlag2.GetCheck() != 0 );
	
	//STOCK FLAG 4
	DealerFlags.SetStockLevelsPluType( m_comboStockFlag4.GetCurSel() );

	//STOCK FLAG 5
	DealerFlags.SetStockLevelsStockType( m_comboStockFlag5.GetCurSel() );

	//STOCK FLAG 6
	DealerFlags.SetTransitionalOrderFlag( m_checkStockFlag6.GetCheck() != 0 );

	//STOCK FLAG 7
	DealerFlags.SetStockLevelFilterFlag( m_checkStockFlag7.GetCheck() != 0 );

	//STOCK FLAG 8
	DealerFlags.SetExtendedStocktakeReportsFlag( m_checkStockFlag8.GetCheck() != 0 );

	//STOCK FLAG 10
	DealerFlags.SetWarehouseToggleFlag( m_checkStockFlag10.GetCheck() != 0 );

	//STOCK FLAG 11
	DealerFlags.SetExportLiveApparentStockFlag( m_checkStockFlag11.GetCheck() != 0 );

	//STOCK FLAG 12
	DealerFlags.SetPeriodDeliveryCostProfitFlag( m_checkStockFlag12.GetCheck() != 0 );

	//STOCK FLAG 13
	DealerFlags.SetStockReportUseRecipeCostLinkFlag( m_checkStockFlag13.GetCheck() != 0 );

	//AUDIT FLAG 1
	DealerFlags.SetDeliveryLinesFlag( m_checkAuditFlag1.GetCheck() != 0 );
		
	//AUDIT FLAG 2
	DealerFlags.SetDeliveryDetailFlag( m_checkAuditFlag2.GetCheck() != 0 );
		
	//ORDER FLAG 1
	DealerFlags.SetPurchaseOrderLinesFlag( m_checkOrderFlag1.GetCheck() != 0 );

	return bResult;
}

/**********************************************************************/

BOOL CPropPageHiddenOptionsStockman::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/
