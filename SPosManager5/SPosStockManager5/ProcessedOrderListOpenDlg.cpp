/**********************************************************************/
 
/**********************************************************************/
#include "..\CommonEcrStock\DefEditDlg.h"
 
#include "ProcessedOrderDatabaseDlg.h"
#include "ProcessedOrderNum.h"
/**********************************************************************/
#include "ProcessedOrderListOpenDlg.h"
/**********************************************************************/

CProcessedOrderListOpenDlg::CProcessedOrderListOpenDlg( bool bDbOrders, CArray<int,int>& arrayEntityIdx, CWnd* pParent )
	: CProcessedOrderListDlg( FALSE, bDbOrders, arrayEntityIdx, pParent )
{
}

/**********************************************************************/

void CProcessedOrderListOpenDlg::ConfigureDialog()
{
	if ( TRUE == m_bDbOrders )
		SetWindowText( "Process Database Orders" );
	else
		SetWindowText( "Process Stockpoint Orders" );

	SetDefID( IDC_BUTTON2 );
}

/**********************************************************************/

void CProcessedOrderListOpenDlg::HandleDoubleClick()
{
	HandleButton2();
}

/**********************************************************************/

void CProcessedOrderListOpenDlg::HandleButton1()
{
	DataManager.LockSuppliers();
	DataManager.LockSupplierSets();
	Button1Internal();
	DataManager.CloseDatabase();
	DataManager.UnlockSuppliers();
	DataManager.UnlockSupplierSets();
}

/**********************************************************************/

void CProcessedOrderListOpenDlg::Button1Internal()
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
	
	CProcessedOrderArray OrderArray;

	if ( TRUE == m_bDbOrders )
		OrderArray.LoadDatabaseOrders( nSuppNo, m_ContentHelpers.GetDbIdx(), "", "", ORDER_TYPE_OPEN, TRUE );
	else
		OrderArray.LoadStockpointOrders( nSuppNo, m_ContentHelpers.GetSpIdx(), "", "", ORDER_TYPE_OPEN, TRUE );

	CProcessedOrderDatabaseDlg dlg ( OrderArray, PROCESSMODE_EDIT, nSuppIdx, m_ContentHelpers.GetDbIdx(), m_ContentHelpers.GetSpIdx(), "", this );
	
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

void CProcessedOrderListOpenDlg::HandleButton2()
{
	DataManager.LockSuppliers();
	DataManager.LockSupplierSets();
	Button2Internal();
	DataManager.CloseDatabase();
	DataManager.UnlockSuppliers();
	DataManager.UnlockSupplierSets();
}

/**********************************************************************/

void CProcessedOrderListOpenDlg::Button2Internal()
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

	CDefEditDlg dlg ( "Enter Delivery Reference", "", 20, "Reference", this );

	if ( dlg.DoModal() == IDCANCEL )
		return;

	CProcessedOrderArray OrderArray;

	if ( TRUE == m_bDbOrders )
		OrderArray.LoadDatabaseOrders( nSuppNo, m_ContentHelpers.GetDbIdx(), "", "", ORDER_TYPE_OPEN, TRUE );
	else
		OrderArray.LoadStockpointOrders( nSuppNo, m_ContentHelpers.GetSpIdx(), "", "", ORDER_TYPE_OPEN, TRUE );

	CProcessedOrderDatabaseDlg dlgOrder ( OrderArray, PROCESSMODE_DELIVER, nSuppIdx, m_ContentHelpers.GetDbIdx(), m_ContentHelpers.GetSpIdx(), dlg.m_strName, this );	
	
	if ( dlgOrder.DoModal() == IDOK )
	{
		int nSuppIdx = GetSelectedSuppIdx();
		UpdateOrderLines( nSuppIdx );
		SelectSupplier( nSuppIdx );
	}
	else
	{	
		int nSuppIdx = GetSelectedSuppIdx();
		BuildSupplierList( m_nLocLineIdx);
		m_listSupplier.SetItemCountEx( GetSupplierListSize() );
		m_listSupplier.Invalidate();
		SelectSupplier( nSuppIdx );
	}
}

/**********************************************************************/

void CProcessedOrderListOpenDlg::HandleButton3()
{
	DataManager.LockSuppliers();
	DataManager.LockSupplierSets();
	Button3Internal();
	DataManager.CloseDatabase();
	DataManager.UnlockSuppliers();
	DataManager.UnlockSupplierSets();
}

/**********************************************************************/

void CProcessedOrderListOpenDlg::Button3Internal()
{
	int nIndex = GetSafeSupplierSelection();
	if ( nIndex == -1 )
		return;

	CSupplierOrderInfo info;
	GetOrderInfo( nIndex, info );
	int nSuppIdx = info.m_nSuppIdx;
	int nSuppNo = info.m_nSuppNo;
	int nLineCount = info.m_nLineCount;

	if ( nSuppIdx == -2 )
	{
		Prompter.Error( "You must select a supplier before you can create a new order" );
		return;
	}

	{
		CDataManagerInfo infoDM;
		if ( ReadDatabaseFiles( infoDM ) == FALSE )
		{
			DataManager.CloseDatabase();
			Prompter.ShareError( infoDM );
			return;
		}
	}
	
	if ( DataManager.Stock.GetSize() == 0 )
	{
		Prompter.Error ( "You must create some stock records before you can use this option." );
		return;
	}

	CProcessedOrderArray OrderArray;

	if ( TRUE == m_bDbOrders )
		OrderArray.LoadDatabaseOrders( nSuppNo, m_ContentHelpers.GetDbIdx(), "", "", ORDER_TYPE_OPEN, FALSE );
	else
		OrderArray.LoadStockpointOrders( nSuppNo, m_ContentHelpers.GetSpIdx(), "", "", ORDER_TYPE_OPEN, FALSE );
	
	CProcessedOrderDatabaseDlg dlg ( OrderArray, PROCESSMODE_NEWORDER, nSuppIdx, m_ContentHelpers.GetDbIdx(), m_ContentHelpers.GetSpIdx(), "", this );	
		
	if ( dlg.DoModal() == IDOK )
	{
		CProcessedOrderNum OrderNum;
		OrderNum.NextOrder();
		
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
