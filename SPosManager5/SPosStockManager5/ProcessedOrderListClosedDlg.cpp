/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "ProcessedOrderDatabaseDlg.h"
#include "ProcessedOrderPrintDlg.h"
 
/**********************************************************************/
#include "ProcessedOrderListClosedDlg.h"
/**********************************************************************/

CProcessedOrderListClosedDlg::CProcessedOrderListClosedDlg( bool bDbOrders, CArray<int,int>& arrayEntityIdx, CWnd* pParent )
	: CProcessedOrderListDlg( TRUE, bDbOrders, arrayEntityIdx, pParent )
{
}

/**********************************************************************/

void CProcessedOrderListClosedDlg::ConfigureDialog()
{
	if ( TRUE == m_bDbOrders )
		SetWindowText( "Adjust Cost Prices for Database Orders" );
	else
		SetWindowText( "Adjust Cost Prices for Stockpoint Orders" );

	m_button2.ShowWindow( SW_HIDE );
	m_button3.ShowWindow( SW_HIDE );
	SetDefID( IDC_BUTTON1 );
}

/**********************************************************************/

void CProcessedOrderListClosedDlg::HandleDoubleClick()
{
	HandleButton1();
}

/**********************************************************************/

void CProcessedOrderListClosedDlg::HandleButton1()
{
	DataManager.LockSuppliers();
	DataManager.LockSupplierSets();
	Button1Internal();
	DataManager.CloseDatabase();
	DataManager.UnlockSuppliers();
	DataManager.UnlockSupplierSets();
}

/**********************************************************************/

void CProcessedOrderListClosedDlg::Button1Internal()
{
	int nIndex = GetSafeSupplierSelection();
	if ( nIndex == -1 )
		return;

	CSupplierOrderInfo info;
	GetOrderInfo( nIndex, info );
	int nSuppIdx = info.m_nSuppIdx;
	int nSuppNo = info.m_nSuppNo;
	int nLineCount = info.m_nLineCount;

	{
		CDataManagerInfo infoDM;
		if ( ReadDatabaseFiles( infoDM ) == FALSE )
		{
			DataManager.CloseDatabase();
			Prompter.ShareError( infoDM );
			return;
		}
	}

	{
		CString strError = "";
		if ( CanProcessOrders( nSuppIdx, nLineCount, strError ) == FALSE )
		{
			DataManager.CloseDatabase();
			Prompter.Error( strError );
			return;
		}
	}

	CProcessedOrderPrintDlg dlgDate( "Adjust cost prices for delivered orders", TRUE, this );
	if ( dlgDate.DoModal() != IDOK )
		return;
	
	CProcessedOrderArray OrderArray;

	if ( TRUE == m_bDbOrders )
		OrderArray.LoadDatabaseOrders( nSuppNo, m_ContentHelpers.GetDbIdx(), dlgDate.m_strDateFrom, dlgDate.m_strDateTo, ORDER_TYPE_CLOSED, TRUE );
	else
		OrderArray.LoadStockpointOrders( nSuppNo, m_ContentHelpers.GetSpIdx(), dlgDate.m_strDateFrom, dlgDate.m_strDateTo, ORDER_TYPE_CLOSED, TRUE );

	CProcessedOrderDatabaseDlg dlg ( OrderArray, PROCESSMODE_COSTPRICE, nSuppIdx, m_ContentHelpers.GetDbIdx(), m_ContentHelpers.GetSpIdx(), "", this );
	
	if ( OrderArray.GetOrderCount() == 0 )
	{
		DataManager.CloseDatabase();
		Prompter.Error( "There are no closed orders for the specified date range." );
		return;
	}

	if ( dlg.DoModal() == IDOK )
	{
		int nSuppIdx = GetSelectedSuppIdx();
		UpdateOrderLines( nSuppIdx );
		SelectSupplier( nSuppIdx );
	}
	else
	{
		int nSuppIdx = GetSelectedSuppIdx();
		BuildSupplierList( m_nLocLineIdx );
		m_listSupplier.SetItemCountEx( GetSupplierListSize() );
		m_listSupplier.Invalidate();
		SelectSupplier( nSuppIdx );
	}
}

/**********************************************************************/