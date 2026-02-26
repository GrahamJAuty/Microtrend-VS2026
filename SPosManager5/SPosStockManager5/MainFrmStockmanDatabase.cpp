/**********************************************************************/
#include "..\CommonEcrStock\AutoExpireHistory.h"
#include "CommsHandshaker.h"
#include "DatabaseCSVArray.h"
#include "..\CommonEcrStock\DatabaseValidator.h"
#include "LockManager.h"
#include "NodeTypes.h"
#include "Password.h"
#include "ProcessedOrderGenerateStockDlg.h"
#include "ProcessedOrderGenerateSalesDlg.h"
#include "ProcessedOrderListOpenDlg.h"
#include "ProcessedOrderListClosedDlg.h"
#include "ProcessedOrderListOpenDlg.h"
#include "ProcessedOrderRepeatDlg.h"
#include "..\CommonEcrStock\PropertySheetSystemDataList.h"
#include "..\CommonEcrStock\SelectSingleDatabaseDlg.h"
#include "StockOptionsIni.h"
#include "StockpointCSVArray.h"
#include "StocktakeDlg.h"
/**********************************************************************/
#include "MainFrmStockman.h"
/**********************************************************************/

void CMainFrameStockman::OnDatabaseEditData() 
{
	AutoExpireHistory.ExpireReportsNow();

	CPropertySheetSystemDataList propSheet( this );
	propSheet.DoModal();

	if ( propSheet.GetDataValidationErrorFlag() == TRUE )
	{
		DatabaseValidator.SetErrorFlag( TRUE );
		PostMessage( WM_CLOSE );
	}
}

/**********************************************************************/

void CMainFrameStockman::OnDatabaseStocktake() 
{
	CArray<int,int> arrayEntityIdx;
	for( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
	{
		if ( PasswordArray.CheckLocationAccessSpIdx( nSpIdx ) == TRUE )
			arrayEntityIdx.Add( nSpIdx );
	}

	if ( arrayEntityIdx.GetSize() == 0 )
	{
		Prompter.StockpointAccessError();
		return;
	}

	dbStockpoint.SortSpListByName( arrayEntityIdx );

	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = FALSE;
	infoLock.m_nCommsAction = COMMS_ACTION_DISABLE;
	infoLock.m_bStopComms = TRUE;
	infoLock.m_bStopCommsPMSExemption = FALSE;
	infoLock.m_strStopCommsPrompt = "Stocktaking";
	infoLock.m_bStopPosTray = TRUE;
	infoLock.m_pParentWnd = this;

	CLockManager LockManager( infoLock );

	if ( LockManager.GetLock() == TRUE )
	{
		CStocktakeDlg dlgStocktake( arrayEntityIdx, this );
		dlgStocktake.DoModal();
	}
}

/**********************************************************************/

void CMainFrameStockman::OnDatabaseOrderGenerate() 
{
	CArray<int,int> arrayEntityIdx;
	for( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
	{
		if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
		{
			int nSpIdxStart, nSpIdxEnd;
			dbStockpoint.GetSpIdxRange( nDbIdx, nSpIdxStart, nSpIdxEnd );
			
			if ( nSpIdxEnd >= nSpIdxStart ) 
				arrayEntityIdx.Add( nDbIdx );
		}
	}

	if ( arrayEntityIdx.GetSize() == 0 )
	{
		Prompter.StockpointAccessError();
		return;
	}

	dbDatabase.SortDbListByName( arrayEntityIdx );

	CDataManagerInfo info;
	DataManager.OpenDatabaseQuickNoFiles(0,FALSE);
	DataManager.OpenSupplier( DB_READONLY, info );
	DataManager.LockSuppliers();
	DataManager.LockSupplierSets();
	DataManager.CloseDatabase();

	{
		CDataManagerInfo infoDM;
		DataManager.OpenDatabaseReadOnly( arrayEntityIdx.GetAt(0), infoDM, TRUE, FALSE );
	}

	if ( StockOptions.GetOrderGenerationMethod() == ORDER_TYPE_SALES )
	{
		CProcessedOrderGenerateSalesDlg dlg ( arrayEntityIdx, this );
		dlg.DoModal();
	}
	else
	{
		CProcessedOrderGenerateStockDlg dlg ( arrayEntityIdx, this );
		dlg.DoModal();
	}

	{
		CDataManagerInfo infoDM;
		DataManager.OpenDatabaseQuickNoFiles(0,FALSE);
		DataManager.UnlockSuppliers();
		DataManager.UnlockSupplierSets();
		DataManager.CloseSupplier( infoDM );
		DataManager.CloseDatabase();
	}
}

/**********************************************************************/

void CMainFrameStockman::OnDatabaseOrderPrevious() 
{
	CArray<int,int> arrayEntityIdx;
	for( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
	{
		if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
		{
			int nSpIdxStart, nSpIdxEnd;
			dbStockpoint.GetSpIdxRange( nDbIdx, nSpIdxStart, nSpIdxEnd );
			
			if ( nSpIdxEnd >= nSpIdxStart ) 
				arrayEntityIdx.Add( nDbIdx );
		}
	}

	if ( arrayEntityIdx.GetSize() == 0 )
	{
		Prompter.StockpointAccessError();
		return;
	}

	dbDatabase.SortDbListByName( arrayEntityIdx );

	CProcessedOrderRepeatDlg dlg( arrayEntityIdx, this );
	dlg.DoModal();
}

/**********************************************************************/

bool CMainFrameStockman::UpdateDatabaseOrders()
{
	COrderCSVArray OldOrders;
	if ( OldOrders.NeedUpdateDatabaseOrders() == TRUE )
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
		{
			CWordArray arrayReadWrite;
			arrayReadWrite.Add( NODE_STOCK );
			arrayReadWrite.Add( NODE_PLU );

			CDataManagerInfo info;
			if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, info ) == FALSE )
			{
				DataManager.CloseDatabase();
				OldOrders.ShowUpgradeError();
				return FALSE;
			}

			if ( OldOrders.UpdateDatabaseOrders( nDbIdx ) == FALSE )
			{
				DataManager.CloseDatabase();
				OldOrders.ShowUpgradeError();
				return FALSE;
			}

			DataManager.CloseDatabase();
		}
	}

	return TRUE;
}

/**********************************************************************/

void CMainFrameStockman::OnDatabaseOrderProcess() 
{
	ProcessDatabaseOrders( FALSE );
}

/**********************************************************************/

void CMainFrameStockman::OnDatabaseOrderProcessCost() 
{
	ProcessDatabaseOrders( TRUE );
}

/**********************************************************************/

void CMainFrameStockman::ProcessDatabaseOrders( bool bCost ) 
{
	//MUST HAVE AT LEAST DATABASE ACCESS TO PROCESS DATABASE ORDERS
	int nLocationAccessType = PasswordArray.GetLocationAccessType();
	switch( nLocationAccessType )
	{
	case LOCATION_ACCESS_SYSTEM:
	case LOCATION_ACCESS_DB:
		break;

	default:
		return;
	}

	CArray<int,int> arrayEntityIdx;
	for( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
	{
		if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
		{
			int nSpIdxStart, nSpIdxEnd;
			dbStockpoint.GetSpIdxRange( nDbIdx, nSpIdxStart, nSpIdxEnd );
			
			if ( nSpIdxEnd >= nSpIdxStart ) 
				arrayEntityIdx.Add( nDbIdx );
		}
	}

	if ( arrayEntityIdx.GetSize() == 0 )
	{
		Prompter.StockpointAccessError();
		return;
	}

	if ( UpdateDatabaseOrders() == FALSE )
		return;

	dbDatabase.SortDbListByName( arrayEntityIdx );

	if ( FALSE == bCost )
	{
		CProcessedOrderListOpenDlg dlgOrder( TRUE, arrayEntityIdx, this );
		dlgOrder.DoModal();
	}
	else
	{
		CProcessedOrderListClosedDlg dlgOrder( TRUE, arrayEntityIdx, this );
		dlgOrder.DoModal();
	}
}

/**********************************************************************/

bool CMainFrameStockman::UpdateStockpointOrders()
{
	COrderCSVArray OldOrders;
	if ( OldOrders.NeedUpdateStockpointOrders() == TRUE )
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
		{
			int nSpIdxStart, nSpIdxEnd;
			dbStockpoint.GetSpIdxRange( nDbIdx, nSpIdxStart, nSpIdxEnd );

			if ( nSpIdxEnd >= nSpIdxStart )
			{
				CWordArray arrayReadWrite;
				arrayReadWrite.Add( NODE_STOCK );
				arrayReadWrite.Add( NODE_PLU );

				CDataManagerInfo info;
				if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, info ) == FALSE )
				{
					DataManager.CloseDatabase();
					OldOrders.ShowUpgradeError();
					return FALSE;
				}

				for ( int nSpIdx = nSpIdxStart; nSpIdx <= nSpIdxEnd; nSpIdx++ )
				{
					if ( OldOrders.UpdateStockpointOrders( nSpIdx ) == FALSE )
					{
						DataManager.CloseDatabase();
						OldOrders.ShowUpgradeError();
						return FALSE;
					}
				}

				DataManager.CloseDatabase();
			}
		}
	}

	return TRUE;
}

/**********************************************************************/

void CMainFrameStockman::OnDatabaseOrderProcessStockpoint() 
{
	ProcessStockpointOrders( FALSE );
}

/**********************************************************************/

void CMainFrameStockman::OnDatabaseOrderProcessCostStockpoint() 
{
	ProcessStockpointOrders( TRUE );
}

/**********************************************************************/

void CMainFrameStockman::ProcessStockpointOrders( bool bCost ) 
{
	CArray<int,int> arrayEntityIdx;
	for( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
	{
		if ( PasswordArray.CheckLocationAccessSpIdx( nSpIdx ) == TRUE )
			arrayEntityIdx.Add( nSpIdx );
	}

	if ( arrayEntityIdx.GetSize() == 0 )
	{
		Prompter.StockpointAccessError();
		return;
	}

	if ( UpdateStockpointOrders() == FALSE )
		return;

	dbStockpoint.SortSpListByName( arrayEntityIdx );

	if ( FALSE == bCost )
	{
		CProcessedOrderListOpenDlg dlgOrder( FALSE, arrayEntityIdx, this );
		dlgOrder.DoModal();
	}
	else
	{
		CProcessedOrderListClosedDlg dlgOrder( FALSE, arrayEntityIdx, this );
		dlgOrder.DoModal();
	}
}

/**********************************************************************/