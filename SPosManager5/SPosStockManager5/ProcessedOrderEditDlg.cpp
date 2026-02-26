/**********************************************************************/
#include <math.h>
/**********************************************************************/
#include "Consolidation.h"
#include "DatabaseCSVArray.h"
#include "PriceHelpers.h"
#include "ProcessedOrderDatabaseDlg.h"
#include "ProcessedOrderQtyDlg.h"
#include "ReportConsolidationArray.h"
#include "StockOptionsIni.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ProcessedOrderEditDlg.h"
/**********************************************************************/

CProcessedOrderEditDlg::CProcessedOrderEditDlg( CProcessedOrderItem& OrderItemNew, CProcessedOrderItem& OrderItemOld, int nStockIdx, CStockCSVRecord& StockRecord, int nDbIdx, int nMode, int nSuppNo, int nSingleSpIdx, CWnd* pParent /*=NULL*/)
	: CSSDialog(CProcessedOrderEditDlg::IDD, pParent), m_OrderItemNew ( OrderItemNew ), m_OrderItemOld ( OrderItemOld ), m_StockRecord ( StockRecord )
{
	//{{AFX_DATA_INIT(CProcessedOrderEditDlg)
	//}}AFX_DATA_INIT
	m_nStockIdx = nStockIdx;
	m_nSuppNo = nSuppNo;
	m_nMode = nMode;
	m_bChangedStockRecord = FALSE;

	m_nDbIdx = nDbIdx;
	
	m_dDUCost = m_OrderItemNew.GetDUCost();
	m_nDUQty = m_OrderItemNew.GetDUQty();
	m_nDUItems = m_OrderItemNew.GetDUItems();
	m_dDUItemSize = m_OrderItemNew.GetDUItemSize();
	m_strSuppRef = m_OrderItemNew.GetSuppRef();

	if ( nSingleSpIdx != -1 )
	{
		m_OrderItemOld.ContractStockpointOrders();
		m_OrderItemNew.ContractStockpointOrders();
	}
}

/**********************************************************************/

void CProcessedOrderEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessedOrderEditDlg)
	DDX_Control(pDX, IDC_BUTTON_AUTO, m_buttonAuto);
	DDX_Control(pDX, IDC_EDIT_NEWQTY, m_editNewQty);
	DDX_Control(pDX, IDC_EDIT_NEWCOST, m_editNewCost);
	DDX_Control(pDX, IDC_EDIT_OLDQTY, m_editOldQty);
	DDX_Control(pDX, IDC_EDIT_OLDCOST, m_editOldCost);
	DDX_Control(pDX, IDC_EDIT_SUPPREF, m_editSuppRef);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_buttonEdit);
	DDX_Control(pDX, IDC_STATIC_EXPECTED, m_groupExpected);
	DDX_Control(pDX, IDC_STATIC_ACTUAL, m_groupActual);
	DDX_Control(pDX, IDC_COMBO_DELUNIT, m_comboDeliveryUnit);
	DDX_Control(pDX, IDC_STATIC_LIST, m_staticList );
	//}}AFX_DATA_MAP
	DDX_Text (pDX, IDC_EDIT_DUCOST, m_dDUCost );
	DDX_Text (pDX, IDC_EDIT_DUQTY, m_nDUQty );
	DDV_MinMaxInt (pDX, m_nDUQty, 1, 999999 ); 
	DDX_Text (pDX, IDC_EDIT_DUITEMSIZE, m_dDUItemSize );
	DDV_MinMaxDouble ( pDX, m_dDUItemSize, Stock::DUItemSize.Min, Stock::DUItemSize.Max );
	DDX_Text (pDX, IDC_EDIT_DUITEMS, m_nDUItems );
	DDV_MinMaxInt ( pDX, m_nDUItems, Stock::DUItems.Min, Stock::DUItems.Max ); 
	DDX_Text (pDX, IDC_EDIT_SUPPREF, m_strSuppRef );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CProcessedOrderEditDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessedOrderEditDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_DUCOST, OnKillFocusDUCost)
	ON_EN_KILLFOCUS(IDC_EDIT_DUQTY, OnKillFocusDUQty)
	ON_EN_KILLFOCUS(IDC_EDIT_DUITEMS, OnKillFocusDUYield)
	ON_CBN_SELCHANGE(IDC_COMBO_DELUNIT, OnSelectDeliveryUnit)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST, OnKillFocusList)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST, OnSetFocusList)
	ON_EN_KILLFOCUS(IDC_EDIT_DUITEMSIZE, OnKillFocusDUYield)
	ON_BN_CLICKED(IDC_BUTTON_AUTO, OnButtonAuto)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CProcessedOrderEditDlg::GetEditDUItems() { return GetEdit( IDC_EDIT_DUITEMS ); }
CEdit* CProcessedOrderEditDlg::GetEditDUItemSize() { return GetEdit( IDC_EDIT_DUITEMSIZE ); }
CEdit* CProcessedOrderEditDlg::GetEditDUQty() { return GetEdit( IDC_EDIT_DUQTY ); }
/**********************************************************************/

BOOL CProcessedOrderEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	m_listStockpoint.SubclassDlgItem ( IDC_LIST, this );
	SubclassEdit ( IDC_EDIT_DUCOST, SS_NUM_DP, 8, "%.*f", SysInfo.GetDPValue() );
	SubclassEdit ( IDC_EDIT_DUITEMS, SS_NUM, 6, "%d" );
	SubclassEdit ( IDC_EDIT_DUITEMSIZE, SS_NUM_DP, 8, "%.3f" );
	SubclassEdit ( IDC_EDIT_DUQTY, SS_NUM, 6, "%d" );

	switch ( m_nMode )
	{
	case ORDER_EDIT_PART:
	case ORDER_EDIT_REPLACE:
		m_listStockpoint.InsertColumn ( 0, "Stockpoint", LVCFMT_LEFT, 150 );
		m_listStockpoint.InsertColumn ( 1, "Apparent", LVCFMT_RIGHT, 90 );
		m_listStockpoint.InsertColumn ( 2, "Ordered", LVCFMT_RIGHT, 90 );
		m_listStockpoint.InsertColumn ( 3, "Delivered", LVCFMT_RIGHT, 90 );
		break;

	case ORDER_EDIT_COST:
		m_listStockpoint.InsertColumn ( 0, "Stockpoint", LVCFMT_LEFT, 200 );
		m_listStockpoint.InsertColumn ( 1, "Apparent", LVCFMT_RIGHT, 110 );
		m_listStockpoint.InsertColumn ( 2, "Delivered", LVCFMT_RIGHT, 110 );
		break;

	case ORDER_EDIT_NORMAL:
	default:
		m_nMode = ORDER_EDIT_NORMAL;
		m_listStockpoint.InsertColumn ( 0, "Stockpoint", LVCFMT_LEFT, 150 );
		m_listStockpoint.InsertColumn ( 1, "Apparent", LVCFMT_RIGHT, 90 );
		m_listStockpoint.InsertColumn ( 2, "Ordered", LVCFMT_RIGHT, 90 );
		m_listStockpoint.InsertColumn ( 3, "Required", LVCFMT_RIGHT, 90 );
		break;
	}
			
	AddList();

	m_DelUnitSelector.FillComboBox( &m_comboDeliveryUnit, m_OrderItemOld.GetStockIdx(), m_nSuppNo );
	m_DelUnitSelector.SelectDelUnitFromOrder ( m_OrderItemOld );

	CString strItemText;
	strItemText.Format ( "%s, %s", m_OrderItemNew.GetStockCode(), m_OrderItemNew.GetDescription() );

	switch ( m_nMode )
	{
	case ORDER_EDIT_PART: 
		{
			CString strTitle = "Accept Partial Delivery ";
			strTitle += strItemText;	
			SetWindowText ( strTitle );
			m_groupExpected.SetWindowText ( "Expected Delivery" );
			m_groupActual.SetWindowText ( "Actual Delivery" );
			m_buttonOk.SetWindowText ( "Deliver" );
			m_comboDeliveryUnit.EnableWindow( FALSE );
			GetEditDUItems() -> EnableWindow( FALSE );
			GetEditDUItemSize() -> EnableWindow( FALSE );
			m_editSuppRef.EnableWindow( FALSE );
		}
		break;

	case ORDER_EDIT_REPLACE:
		{
			CString strTitle = "Accept Replacement Delivery ";
			strTitle += strItemText;	
			SetWindowText ( strTitle );
			m_groupExpected.SetWindowText ( "Expected Delivery" );
			m_groupActual.SetWindowText ( "Actual Delivery" );
			m_buttonOk.SetWindowText ( "Deliver" );
			m_editSuppRef.EnableWindow( FALSE );
		}
		break;

	case ORDER_EDIT_NORMAL:
		{
			CString strTitle = "Edit Order Item ";
			strTitle += strItemText;	
			SetWindowText ( strTitle );
			m_groupExpected.SetWindowText ( "Current Order" );
			m_groupActual.SetWindowText ( "New Order" );
			m_buttonOk.SetWindowText ( "OK" );
		}
		break;

	case ORDER_EDIT_COST:
		{
			CString strTitle = "Edit Cost Price ";
			strTitle += strItemText;	
			SetWindowText ( strTitle );
			m_groupExpected.SetWindowText ( "Expected Delivery" );
			m_groupActual.SetWindowText ( "Actual Delivery" );
			m_buttonOk.SetWindowText ( "OK" );
			m_buttonEdit.ShowWindow ( SW_HIDE );
			m_buttonAuto.ShowWindow ( SW_HIDE );
			m_comboDeliveryUnit.EnableWindow( FALSE );
			m_editSuppRef.EnableWindow( FALSE );
			GetEditDUItems() -> EnableWindow( FALSE );
			GetEditDUItemSize() -> EnableWindow( FALSE );
			GetEditDUQty() -> EnableWindow( FALSE );
			m_staticList.SetWindowText( "Order Details" );
		}
		break;
	}
	
	if ( m_OrderItemNew.GetStockpointCount() < 2 )
	{
		m_buttonEdit.ShowWindow ( SW_HIDE );
		m_buttonAuto.ShowWindow ( SW_HIDE );
	}

	UpdateQtyAndCostDisplay();
	return TRUE;  
}

/**********************************************************************/

void CProcessedOrderEditDlg::UpdateQtyAndCostDisplay()
{
	CString strOldQty;
	strOldQty.Format ( "%d x %.3f", m_OrderItemOld.GetDUQty() * m_OrderItemOld.GetDUItems(), m_OrderItemOld.GetDUItemSize() );
	m_editOldQty.SetWindowText ( strOldQty );

	CString strOldCost;
	strOldCost.Format ( "%.*f", SysInfo.GetDPValue(), m_OrderItemOld.GetDUCost() * m_OrderItemOld.GetDUQty() );
	m_editOldCost.SetWindowText ( strOldCost );

	CString strNewQty;
	strNewQty.Format ( "%d x %.3f", m_OrderItemNew.GetDUQty() * m_OrderItemNew.GetDUItems(), m_OrderItemNew.GetDUItemSize() );
	m_editNewQty.SetWindowText ( strNewQty );

	CString strNewCost;
	strNewCost.Format ( "%.*f", SysInfo.GetDPValue(), m_OrderItemNew.GetDUCost() * m_OrderItemNew.GetDUQty() );
	m_editNewCost.SetWindowText ( strNewCost );
}

/**********************************************************************/

void CProcessedOrderEditDlg::SetSUQty()
{
	if ( m_OrderItemNew.GetStockpointCount() == 1 )
	{
		m_OrderItemNew.SetDUItems ( 0, m_OrderItemNew.GetDUItems() * m_OrderItemNew.GetDUQty() );
		AddList();
		UpdateQtyAndCostDisplay();
	}
}

/**********************************************************************/

void CProcessedOrderEditDlg::OnKillFocusDUCost() 
{
	UpdateData( TRUE, FALSE );
	m_OrderItemNew.SetDUCost( m_dDUCost );
	UpdateQtyAndCostDisplay();
}

/**********************************************************************/

void CProcessedOrderEditDlg::OnKillFocusDUQty() 
{
	UpdateData( TRUE, FALSE );
	m_OrderItemNew.SetDUQty( m_nDUQty );
	UpdateQtyAndCostDisplay();
	SetSUQty();	
}

/**********************************************************************/

void CProcessedOrderEditDlg::OnKillFocusDUYield() 
{
	UpdateData ( TRUE, FALSE );
	m_OrderItemNew.SetDUItems( m_nDUItems );
	m_OrderItemNew.SetDUItemSize( m_dDUItemSize );
	UpdateQtyAndCostDisplay();
	SetSUQty();
	AddList();
}

/**********************************************************************/

void CProcessedOrderEditDlg::OnSelectDeliveryUnit() 
{
	UpdateData( TRUE, FALSE );

	int nSel = m_comboDeliveryUnit.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboDeliveryUnit.GetCount() )
	{
		CDelUnitInfo infoDelUnit;
		m_DelUnitSelector.GetDelUnitInfo( infoDelUnit );
		
		m_nDUItems = infoDelUnit.m_nDUItems;
		m_dDUItemSize = infoDelUnit.m_dDUItemSize;
		
		m_OrderItemNew.SetDUItems( m_nDUItems );
		m_OrderItemNew.SetDUItemSize( m_dDUItemSize );
		m_OrderItemNew.SetCase( infoDelUnit.m_strCase );

		if ( infoDelUnit.m_nColour == DELUNIT_COLOUR_GREEN )
		{
			m_dDUCost = infoDelUnit.m_dDUCost;
			m_strSuppRef = m_StockRecord.GetSuppRefExternal( infoDelUnit.m_nStockSuppIdx, DealerFlags.GetSuppRefFlag() );

			m_OrderItemNew.SetDUCost( m_dDUCost );
			m_OrderItemNew.SetSuppRef( m_strSuppRef );
		}
		
		UpdateQtyAndCostDisplay();
		SetSUQty();
		AddList();
	}

	UpdateData ( FALSE );
}

/**********************************************************************/

void CProcessedOrderEditDlg::OnButtonEdit() 
{
	//AVOID POSSIBLE DIVISION BY ZERO 
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	int nListSel = m_listStockpoint.GetCurSel();
	if ( ( nListSel < 0 ) || ( nListSel >= m_arrayOrderSpIdx.GetSize() ) )
		return;

	int nSystemSpIdx = m_arrayOrderSpIdx[ nListSel ].m_nSystemSpIdx;
	int nOrderSpIdx = m_arrayOrderSpIdx[ nListSel ].m_nOrderSpIdx;

	int nType = ( StockOptions.GetOrderSeparationType() != ORDER_DUSEPARATE_NONE ) ? 2 : 1;
	CProcessedOrderQtyDlg dlg ( nSystemSpIdx, nType, m_nDUItems, m_dDUItemSize, this );
	dlg.SetDUItemOrder ( m_OrderItemNew.GetDUItems( nOrderSpIdx ) );
			
	if ( dlg.DoModal() != IDOK )
		return;

	switch( dlg.m_nType )
	{
	case 0:
		{
			int nDUItemsToOrder = int ( dlg.m_dStockUnits / m_dDUItemSize );
			if ( CPriceHelpers::CompareDoubles ( m_dDUItemSize * nDUItemsToOrder, dlg.m_dStockUnits, 3 ) == -1 )
				nDUItemsToOrder++;

			m_OrderItemNew.SetDUItems( nOrderSpIdx, nDUItemsToOrder );
		}
		break;

	case 1:	
		m_OrderItemNew.SetDUItems ( nOrderSpIdx, dlg.m_nSupplierItems );
		break;

	case 2:
		m_OrderItemNew.SetDUItems ( nOrderSpIdx, dlg.m_nDelUnits * m_nDUItems );
		break;
	}
	
	AddList();
	m_listStockpoint.SetCurSel ( nListSel );
}

/**********************************************************************/

void CProcessedOrderEditDlg::OnButtonAuto() 
{	
	int nTotalItemsWanted = 0;
	for ( int nIndex = 0; nIndex < m_OrderItemOld.GetStockpointCount(); nIndex++ )
		nTotalItemsWanted += m_OrderItemOld.GetDUItems( nIndex );

	int nTotalItemsAvailable = m_OrderItemNew.GetDUQty();
	int nMultiplier = m_OrderItemNew.GetDUItems();

	if ( StockOptions.GetOrderSeparationType() == ORDER_DUSEPARATE_NONE )
	{
		nTotalItemsAvailable *= nMultiplier;
		nMultiplier = 1;
	}

	int nTotalItemsAssigned = 0;
	
	if ( nTotalItemsAvailable == 0 || nTotalItemsWanted == 0 )
		return;

	CConsolidateArray ConsolArray;
	double dFactor = double ( nTotalItemsAvailable ) / double ( nTotalItemsWanted ); 
		
	for ( int nIndex = 0; nIndex < m_OrderItemOld.GetStockpointCount(); nIndex++ )
	{
		int nOldDUItems = m_OrderItemOld.GetDUItems( nIndex );
		double dNewDUItems = (double) nTotalItemsAvailable * ( (double) nOldDUItems / (double) nTotalItemsWanted );

		int nNewDUItems = int ( floor ( dNewDUItems ) );
		if ( nNewDUItems < 0 ) nNewDUItems = 0;

		int nRemain = int ( ( dNewDUItems - nNewDUItems ) * 1000 );
		if ( nRemain < 0 ) nRemain = 0;
		if ( nRemain > 999 ) nRemain = 999;
			
		CString strLabel;
		strLabel.Format ( "%3.3d%4.4d", 999 - nRemain, nIndex );
		ConsolArray.Add ( strLabel );
			
		m_OrderItemNew.SetDUItems( nIndex, nNewDUItems * nMultiplier );
		nTotalItemsAssigned += nNewDUItems;
	}

	int nSpareItems = nTotalItemsAvailable - nTotalItemsAssigned;
	if ( nSpareItems > 0 && nSpareItems <= ConsolArray.GetSize() )
	{
		for ( int nPos = 0; nPos < nSpareItems; nPos++ )
		{
			CString strLabel = ConsolArray.GetLabel( nPos );
			int nIndex = atoi ( strLabel.Right(4) );
			if ( ( nIndex >= 0 ) && ( nIndex < m_OrderItemNew.GetStockpointCount() ) )
				m_OrderItemNew.SetDUItems ( nIndex, m_OrderItemNew.GetDUItems( nIndex ) + nMultiplier );
		}
	}

	AddList();
}

/**********************************************************************/

void CProcessedOrderEditDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonEdit();
	*pResult = 0;
}

/**********************************************************************/

void CProcessedOrderEditDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;
	
	int nAssigned = 0;
	for ( int nSp = 0; nSp < m_OrderItemNew.GetStockpointCount(); nSp++ )
		nAssigned += m_OrderItemNew.GetDUItems( nSp );

	int nOrdered = m_OrderItemNew.GetDUItems() * m_OrderItemNew.GetDUQty();

	if ( nAssigned < nOrdered )
	{
		Prompter.Error ( "You must assign all the delivered stock to a stockpoint" );
		return;
	}

	if ( nAssigned > nOrdered )
	{
		Prompter.Error ( "You cannot assign more stock than the total to be delivered" );
		return;
	}

	m_OrderItemNew.SetSuppRef( m_strSuppRef );

	/*
	CString strDeliveryUnit;
	m_comboDeliveryUnit.GetWindowText( strDeliveryUnit );
	strDeliveryUnit = strDeliveryUnit.SpanExcluding ( "(" );
	::TrimSpaces ( strDeliveryUnit, FALSE );
	m_OrderItemNew.SetCase ( strDeliveryUnit );
	*/

	UpdateDeliveryMethod();

	EndDialog ( IDOK );
}

/**********************************************************************/

void CProcessedOrderEditDlg::AddList()
{
	m_arrayOrderSpIdx.RemoveAll();

	int nDbNo = dbDatabase.GetDbNo( m_nDbIdx );
	m_listStockpoint.DeleteAllItems();
	
	double dApparentTotal = 0;
	int nOldDUItemsTotal = 0;
	int nNewDUItemsTotal = 0;

	CStringArray arraySpInfo;
	for ( int nIndex = 0; nIndex < m_OrderItemOld.GetStockpointCount(); nIndex++ )
	{
		int nSpNo = m_OrderItemOld.GetSpNo( nIndex );
		int nOldDUItems = m_OrderItemOld.GetDUItems( nIndex );
		int nNewDUItems = m_OrderItemNew.GetDUItems( nIndex );
		
		double dApparentQty = 0.0;
		CString strSpName = "";

		int nSpIdx;
		if ( dbStockpoint.FindStockpointByNumber( nDbNo, nSpNo, nSpIdx ) == FALSE )
			strSpName.Format ( "Unknown Stockpoint %d", nSpNo );
		else
		{
			strSpName = dbStockpoint.GetName( nSpIdx );
			
			CStockLevelsCSVRecord StockLevels;
			DataManager.Stock.GetStockpointTotals( -1, m_StockRecord, nSpNo, StockLevels );
			dApparentQty = StockLevels.GetApparentQty();
		}

	
		CString strOldDUItems;
		strOldDUItems.Format ( "%d x %.3f", nOldDUItems, m_OrderItemOld.GetDUItemSize() );

		CString strNewDUItems;
		strNewDUItems.Format ( "%d x %.3f", nNewDUItems, m_OrderItemNew.GetDUItemSize() );

		CCSV csv ( '\t' );
		csv.Add ( nSpIdx );
		csv.Add ( nIndex );
		csv.Add ( strSpName );
		csv.Add ( DataManager.Stock.GetStockLevelString ( m_StockRecord, dApparentQty ) );
		csv.Add ( strOldDUItems );
		csv.Add ( strNewDUItems );
		arraySpInfo.Add( csv.GetLine() );
			
		dApparentTotal += dApparentQty;
		nOldDUItemsTotal += nOldDUItems;
		nNewDUItemsTotal += nNewDUItems;
	}

	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		for ( int n = 0; n < arraySpInfo.GetSize(); n++ )
		{
			CCSV csv ( arraySpInfo.GetAt(n), '\t' );
			
			COrderSpIdxInfo info;
			info.m_nSystemSpIdx = csv.GetInt(0);
			info.m_nOrderSpIdx = csv.GetInt(1);
			csv.RemoveAt(1);
			csv.RemoveAt(0);

			int nAddPos = m_listStockpoint.AddString ( csv.GetLine() );
			m_arrayOrderSpIdx.InsertAt( nAddPos, info );
		}
	}
	else
	{
		CReportConsolidationArray<CSortedStringByString> arraySpInfoSorted( TRUE, 1 );

		for ( int n = 0; n < arraySpInfo.GetSize(); n++ )
		{
			CCSV csv ( arraySpInfo.GetAt(n), '\t' );

			CSortedStringByString item;
			item.m_strKey = csv.GetString(2);
			item.m_strData = csv.GetLine();
			arraySpInfoSorted.Consolidate( item );
		}

		for ( int n = 0; n < arraySpInfoSorted.GetSize(); n++ )
		{
			CSortedStringByString item;
			arraySpInfoSorted.GetAt( n, item );

			CCSV csv ( item.m_strData, '\t' );
			
			COrderSpIdxInfo info;
			info.m_nSystemSpIdx = csv.GetInt(0);
			info.m_nOrderSpIdx = csv.GetInt(1);
			csv.RemoveAt(1);
			csv.RemoveAt(0);

			int nAddPos = m_listStockpoint.AddString ( csv.GetLine() );
			m_arrayOrderSpIdx.InsertAt( nAddPos, info );
		}
	}

	if ( m_OrderItemNew.GetStockpointCount() >= 2  )
	{
		CString strOldDUItems;
		strOldDUItems.Format ( "%d x %.3f", nOldDUItemsTotal, m_OrderItemOld.GetDUItemSize() );

		CString strNewDUItems;
		strNewDUItems.Format ( "%d x %.3f", nNewDUItemsTotal, m_OrderItemNew.GetDUItemSize() );

		CCSV csv ( '\t' );
		csv.Add ( "Totals" );
		csv.Add ( dApparentTotal, SysInfo.GetDPQty() );
		csv.Add ( strOldDUItems );
		csv.Add ( strNewDUItems );
		m_listStockpoint.AddString ( csv.GetLine() );
	}
}

/**********************************************************************/

void CProcessedOrderEditDlg::OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDefID ( IDC_BUTTON_EDIT );
	*pResult = 0;
}

/**********************************************************************/

void CProcessedOrderEditDlg::OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDefID ( IDOK );
	*pResult = 0;
}

/**********************************************************************/

void CProcessedOrderEditDlg::UpdateDeliveryMethod()
{
	if ( StockOptions.GetStockAdhocPriceChangeType() == 0 )
		return;

	CDelUnitInfo infoDelUnit;
	m_DelUnitSelector.GetDelUnitInfo( infoDelUnit );
	
	if ( ( infoDelUnit.m_nStockSuppIdx < 0 ) || ( infoDelUnit.m_nStockSuppIdx >= m_StockRecord.GetSupplierCount() ) )
		return;

	if ( infoDelUnit.m_nSuppNo != m_nSuppNo )
		return;

	bool bChanged = FALSE;

	if ( m_OrderItemNew.GetDUItems() != infoDelUnit.m_nDUItems )
		bChanged = TRUE;

	else if ( CPriceHelpers::CompareDoubles( m_OrderItemNew.GetDUItemSize(), infoDelUnit.m_dDUItemSize, 3 ) != 0 )
		bChanged = TRUE;

	else if ( CPriceHelpers::CompareDoubles( m_OrderItemNew.GetDUCost(), infoDelUnit.m_dDUCost, 3) != 0 )
		bChanged = TRUE;

	if ( FALSE == bChanged )
		return;

	if ( StockOptions.GetStockAdhocPriceChangeType() == 2 )
	{
		CString strMsg = "You have changed the cost price or delivery unit yield for this\n";
		strMsg += "item from the settings that are stored in the stock database.\n\n";
		strMsg += "Do you wish to save these changes to the stock database";

		if ( Prompter.YesNo ( strMsg, TRUE ) != IDYES )
			return;
	}

	m_StockRecord.SetCost ( infoDelUnit.m_nStockSuppIdx, m_OrderItemNew.GetDUCost() );
	m_StockRecord.SetDUItems ( infoDelUnit.m_nStockSuppIdx, m_OrderItemNew.GetDUItems() );
	m_StockRecord.SetDUItemSize ( infoDelUnit.m_nStockSuppIdx, m_OrderItemNew.GetDUItemSize() );
	DataManager.Stock.SetAt ( m_nStockIdx, m_StockRecord );

	m_bChangedStockRecord = TRUE;
}

/**********************************************************************/
