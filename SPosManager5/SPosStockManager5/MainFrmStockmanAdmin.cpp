/**********************************************************************/
#include "..\CommonEcrStock\BarcodeDeleteDlg.h"
#include "..\CommonEcrStock\ClearHistoryDlgEcrman.h"
#include "ClearHistoryDlgStockman.h"
#include "CommsHandshaker.h"
#include "FileRemover.h"
#include "ListDataSupplierDeleteDlg.h"
#include "LockManager.h"
#include "MessageLogger.h"
#include "NodeTypes.h"
#include "..\CommonEcrStock\PasswordListDlg.h"
#include "PluDeleteUnlinkedDlg.h"
#include "..\CommonEcrStock\PluDeleteUnusedDlg.h"
#include "SalesXXXDetector.h"
#include "SalesXXXDlg.h"
#include "..\CommonEcrStock\SelectSingleDatabaseDlg.h"
#include "StatusProgress.h"
#include "StockDeleteDlg.h"
#include "StockDeleteUnusedDlg.h"
#include "StockListStockCodeDlg.h"
/**********************************************************************/
#include "MainFrmStockman.h"
/**********************************************************************/

void CMainFrameStockman::OnAdminPasswords() 
{
	if ( DataManager.LockAllUsers() == FALSE )
		Prompter.LockError( "edit passwords" );
	else
	{
		PasswordArray.RememberSettings();

		CPasswordListDlg dlg ( this );
		dlg.m_bLogPasswords = EcrmanOptions.GetPasswordsLogFlag();

		if ( dlg.DoModal() == IDOK )
		{
			PasswordArray.Write();
			EcrmanOptions.SetPasswordsLogFlag ( dlg.m_bLogPasswords != 0 );
			EcrmanOptions.Write();
		}
		else
			PasswordArray.RestoreSettings();

		DataManager.UnlockAllUsers();
	}
}

/**********************************************************************/

void CMainFrameStockman::OnAdminSalesXXX() 
{
	if ( dbStockpoint.GetSize() == 0 )
		ShowStockpointError();
	else if ( PasswordArray.CheckLocationAccessSp() == FALSE )
		Prompter.StockpointAccessError();
	else
	{
		CSalesXXXDlg dlg( this );
		dlg.DoModal();
		SalesXXXDetector.StartDelay();
	}
}

/**********************************************************************/

void CMainFrameStockman::OnAdminDeleteReports() 
{
	if ( dbLocation.GetSize() == 0 )
	{
		ShowLocationError();
		return;
	}

	if ( PasswordArray.CheckLocationAccessLoc() == FALSE )
	{
		Prompter.LocationAccessError();
		return;
	}

	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = TRUE;
	infoLock.m_strLockAllUsersPrompt = "delete reports";
	infoLock.m_nCommsAction = COMMS_ACTION_DISABLE;
	infoLock.m_bStopComms = TRUE;
	infoLock.m_bStopCommsPMSExemption = FALSE;
	infoLock.m_strStopCommsPrompt = "Delete reports";
	infoLock.m_bStopPosTray = TRUE;
	infoLock.m_pParentWnd = this;

	CLockManager LockManager( infoLock );

	if ( LockManager.GetLock() == TRUE )
	{
		if ( PasswordArray.CheckLocationAccessSp() == TRUE )
		{
			CClearHistoryDlgStockman dlg ( this );
			dlg.DoModal();
		}
		else
		{
			CClearHistoryDlgEcrman dlg ( this );
			dlg.DoModal();
		}
	}
}

/**********************************************************************/

void CMainFrameStockman::OnAdminDeleteAudits() 
{
	int nDbIdx = 0;
	if ( PasswordArray.GetLocationAccessType() != LOCATION_ACCESS_SYSTEM )
		nDbIdx = PasswordArray.GetLocationAccessDbIdx();
	else if ( dbDatabase.GetSize() > 1 )
	{
		CSelectSingleDatabaseDlg dlgSelect ( "Delete Order Processing Audits", -1, this );
		
		if ( dlgSelect.DoModal() != IDOK )	
			return;
		
		nDbIdx = dlgSelect.GetDbIdx();
	}

	CString strTitle;
	strTitle.Format ( "Delete Order Processing Audits (%s)", dbDatabase.GetName( nDbIdx ) );

	CStringArray warning;
	warning.Add( strTitle );
	warning.Add( "All order processing audits for this database will be deleted." );
	warning.Add( "" );
	warning.Add( "Adhoc delivery audits will NOT be affected." );
	warning.Add( "" );
	warning.Add( "Pending orders and apparent stock will NOT be affected." );

	if ( Prompter.Warning( warning ) != IDYES )
	{
		CString strMsg;
		strMsg = "The deletion of processed order audits was cancelled.\n\n";
		strMsg += "Your database has not been changed";
		Prompter.Info( strMsg );
		return;
	}

	CWordArray arrayReadOnly;
	arrayReadOnly.Add( NODE_STOCK );
	arrayReadOnly.Add( NODE_ORDER );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadOnly, info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	CString strPath = dbDatabase.GetFolderPathData ( nDbIdx );
	strPath += "\\";

	for ( int nSuppIdx = -1; nSuppIdx < DataManager.Supplier.GetSize(); nSuppIdx++ )
	{
		CString strFilename = strPath + DataManager.Supplier.GetAuditFilename( nSuppIdx );
		CFileRemover FileRemover ( strFilename );
	}
	
	DataManager.CloseDatabase();

	CString strMsg;
	strMsg = "Processed order audits have been deleted for ";
	strMsg += dbDatabase.GetName( nDbIdx );
	Prompter.Info( strMsg );
}

/**********************************************************************/

void CMainFrameStockman::OnAdminDeleteOrders() 
{
	if ( DataManager.LockAllUsers() == FALSE )
		Prompter.LockError( "delete all outstanding orders" );
	else
	{
		CDataManagerInfo info;
		int nDbIdx;

		int nResult = DeleteOrdersInternal( info, nDbIdx );
		
		DataManager.CloseDatabase();
		DataManager.UnlockAllUsers();

		switch( nResult )
		{
		case 1:
			{
				CString strMsg;
				strMsg = "The deletion of outstanding orders was cancelled.\n\n";
				strMsg += "Your database has not been changed";
				Prompter.Info( strMsg );
			}
			break;

		case 2:
			Prompter.ShareError( info );
			break;

		case 3:
			Prompter.WriteError( info );
			break;

		case 4:
			{
				CString strMsg;
				strMsg = "Outstanding orders have been deleted for ";
				strMsg += dbDatabase.GetName( nDbIdx );
				Prompter.Info( strMsg );
			}
			break;
		}
	}
}

/**********************************************************************/

int CMainFrameStockman::DeleteOrdersInternal( CDataManagerInfo& info, int& nDbIdx )
{
	nDbIdx = 0;
	if ( PasswordArray.GetLocationAccessType() != LOCATION_ACCESS_SYSTEM )
		nDbIdx = PasswordArray.GetLocationAccessDbIdx();
	else if ( dbDatabase.GetSize() > 1 )
	{
		CSelectSingleDatabaseDlg dlgSelect ( "Delete Outstanding Orders", -1, this );
		
		if ( dlgSelect.DoModal() != IDOK )	
			return 0;
		
		nDbIdx = dlgSelect.GetDbIdx();
	}

	CString strTitle;
	strTitle.Format ( "Delete Outstanding Orders (%s)", dbDatabase.GetName( nDbIdx ) );

	CStringArray warning;
	warning.Add( strTitle );
	warning.Add( "All outstanding orders for this database will be deleted." );
	warning.Add( "" );
	warning.Add( "The on order totals for each stock item will be reset to zero." );

	if ( Prompter.Warning( warning ) != IDYES )
		return 1;

	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_STOCK );
	arrayReadWrite.Add( NODE_ORDER );

	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, info ) == FALSE )
		return 2;

	DataManager.Order.RemoveAll();
	if ( DataManager.WriteOrder( info ) == FALSE )
		return 3;

	int nDummy = 0;
	CFileRemover FileRemover;
	CString strFolder = "";
	
	strFolder = dbDatabase.GetFolderPathDataOrdersType( nDbIdx, 0, ORDER_TYPE_OPEN );
	FileRemover.RemoveFolderContents( strFolder, nDummy );

	strFolder = dbDatabase.GetFolderPathDataOrdersType( nDbIdx, 0, ORDER_TYPE_MIXED );
	FileRemover.RemoveFolderContents( strFolder, nDummy );
	
	for ( int nSuppIdx = 0; nSuppIdx < DataManager.Supplier.GetSize(); nSuppIdx++ )
	{
		nDummy = 0;
		int nSuppNo = DataManager.Supplier.GetSuppNo( nSuppIdx );
		CString strFolder = "";
		
		strFolder = dbDatabase.GetFolderPathDataOrdersType( nDbIdx, nSuppNo, ORDER_TYPE_OPEN );
		FileRemover.RemoveFolderContents( strFolder, nDummy );

		strFolder = dbDatabase.GetFolderPathDataOrdersType( nDbIdx, nSuppNo, ORDER_TYPE_MIXED );
		FileRemover.RemoveFolderContents( strFolder, nDummy );
	}

	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange( nDbIdx, nStartIdx, nEndIdx );

	for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
	{
		DataManager.CloseOrder( info );
		if ( DataManager.OpenOrderSp( nSpIdx, DATAMANAGER_OPEN_READWRITE, info ) == FALSE )
			return 2;

		DataManager.Order.RemoveAll();
		if ( DataManager.WriteOrder( info ) == FALSE )
			return 3;

		nDummy = 0;
		CFileRemover FileRemover;
		CString strFolder = "";
		
		strFolder = dbStockpoint.GetFolderPathDataOrdersType( nSpIdx, 0, ORDER_TYPE_OPEN );
		FileRemover.RemoveFolderContents( strFolder, nDummy );

		strFolder = dbStockpoint.GetFolderPathDataOrdersType( nSpIdx, 0, ORDER_TYPE_MIXED );
		FileRemover.RemoveFolderContents( strFolder, nDummy );
	
		for ( int nSuppIdx = 0; nSuppIdx < DataManager.Supplier.GetSize(); nSuppIdx++ )
		{
			nDummy = 0;
			int nSuppNo = DataManager.Supplier.GetSuppNo( nSuppIdx );
			CString strFolder = "";
			
			strFolder = dbStockpoint.GetFolderPathDataOrdersType( nSpIdx, nSuppNo, ORDER_TYPE_OPEN );
			FileRemover.RemoveFolderContents( strFolder, nDummy );

			strFolder = dbStockpoint.GetFolderPathDataOrdersType( nSpIdx, nSuppNo, ORDER_TYPE_MIXED );
			FileRemover.RemoveFolderContents( strFolder, nDummy );
		}
	}

	StatusProgress.Lock( TRUE, "Clearing orders" );
	int nSize = DataManager.Stock.GetSize();
	for ( int nStockIdx = 0; nStockIdx < nSize; nStockIdx++ )
	{
		StatusProgress.SetPos( nStockIdx, nSize );
		DataManager.Stock.ClearOrders( nStockIdx );
	}
	StatusProgress.Unlock();

	if ( DataManager.WriteStock( info, TRUE, TRUE ) == FALSE )
		return 3;
	
	return 4;
}

/**********************************************************************/

void CMainFrameStockman::OnAdminDeletePluUnlinked() 
{
	CPluDeleteUnlinkedDlg dlg ( this );
	dlg.DoModal();
}

/**********************************************************************/

void CMainFrameStockman::OnAdminDeletePluUnsold() 
{
	CPluDeleteUnusedDlg dlg( this );
	dlg.DoModal();
}

/**********************************************************************/

void CMainFrameStockman::OnAdminDeleteBarcodes() 
{
	bool bContinue = TRUE;
	
	while( TRUE == bContinue )
	{
		bContinue = FALSE;

		int nDbIdx = 0;
		if ( PasswordArray.GetLocationAccessType() != LOCATION_ACCESS_SYSTEM )
			nDbIdx = PasswordArray.GetLocationAccessDbIdx();
		else if ( dbDatabase.GetSize() > 1 )
		{
			CSelectSingleDatabaseDlg dlg ( "Delete Unlinked Barcodes", nDbIdx, this );
			
			if ( dlg.DoModal() != IDOK )
				return;

			nDbIdx = dlg.GetDbIdx();
			bContinue = TRUE;
		}

		CDataManagerInfo info;
		
		CWordArray arrayReadWrite;
		arrayReadWrite.Add( NODE_BARCODE );

		if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, info, FALSE ) == TRUE )
		{
			CArray<int,int> arrayBarcodeIdx;
			DataManager.Barcode.GetUnlinkedBarcodeList( arrayBarcodeIdx );
			int nUnlinked = arrayBarcodeIdx.GetSize();

			int nMessage = 0;

			if ( 0 == nUnlinked )
				nMessage = 1;
			else
			{
				CBarcodeDeleteDlg dlgDelete( arrayBarcodeIdx, this );
				
				if ( dlgDelete.DoModal() == IDOK )
				{
					DataManager.Barcode.DeleteBarcodes( arrayBarcodeIdx );
					
					if ( DataManager.WriteBarcode( info ) == TRUE )
					{
						CDWordArray dw;
						dw.Add( 4 );
						dw.Add(nUnlinked);
						dw.Add(arrayBarcodeIdx.GetSize());
						dw.Add(DataManager.Barcode.GetSize() );
						MessageLogger.LogSystem( 11, 1, TRUE, dw );
					}
					else
						nMessage = 3;
				}
			}

			DataManager.CloseDatabase();

			switch( nMessage )
			{
			case 1:
				Prompter.Info( "No unlinked barcodes were found" );
				break;

			case 2:
				Prompter.Info( "Barcodes were deleted as requested" );
				break;

			case 3:
				Prompter.WriteError( info );
				break;
			}

		}
	}
}

/**********************************************************************/

void CMainFrameStockman::OnAdminDeleteStockUnlinked() 
{
	CStockDeleteDlg dlg ( this );	
	dlg.DoModal();
}

/**********************************************************************/

void CMainFrameStockman::OnAdminDeleteStockUnsold() 
{
	CStockDeleteUnusedDlg dlg ( this );
	dlg.DoModal();
}

/**********************************************************************/

void CMainFrameStockman::OnAdminDeleteSuppliers() 
{
	if ( DataManager.LockAllUsers() == FALSE )
	{
		Prompter.LockError ( "delete suppliers" );
		return;
	}
	
	CDataManagerInfo info;
	int nResult = DeleteSuppliers( info );
	DataManager.UnlockAllUsers();

	switch ( nResult )
	{
	case 1:
		Prompter.ShareError( info );
		break;

	case 2:
		Prompter.Error( "You must create some suppliers before selecting this option." );
		break;

	case 3:
		Prompter.Error ( "You cannot delete any suppliers because they all\nhave outstanding orders on file." );
		break;

	case 4:
		Prompter.SupplierError();
		break;
	}
}

/**********************************************************************/

int CMainFrameStockman::DeleteSuppliers( CDataManagerInfo& info )
{
	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_SUPPLIER );
	arrayReadWrite.Add( NODE_SUPPLIER_SET );

	if ( DataManager.OpenDatabaseReadWrite(0, arrayReadWrite, info, FALSE) == FALSE )
		return 1;
		
	DataManager.LockSuppliers();
	DataManager.LockSupplierSets();
		
	int nResult = DeleteSuppliersInternal();

	DataManager.UnlockSuppliers();
	DataManager.UnlockSupplierSets();

	DataManager.ForceCloseSupplier( info );
	DataManager.ForceCloseSupplierSet( info );
		
	DataManager.CloseDatabase();

	return nResult;
}

/**********************************************************************/

int CMainFrameStockman::DeleteSuppliersInternal()
{
	if ( DataManager.Supplier.GetSize() == 0 )
		return 2;

	if ( DataManager.Supplier.IsValid() == FALSE )
		return 4;

	CWordArray arrayProtected;
	for ( int nSuppIdx = 0; nSuppIdx < DataManager.Supplier.GetSize(); nSuppIdx++ )
		arrayProtected.Add(0);
	
	StatusProgress.Lock( TRUE, "Checking database orders" );
		
	for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
	{
		DataManager.OpenDatabaseQuickNoFiles( nDbIdx, FALSE );

		CDataManagerInfo info;
		DataManager.OpenOrder( DATAMANAGER_OPEN_READONLY, info );

		CString str;
		str.Format ( "(%s)", dbDatabase.GetName( nDbIdx ) );
		StatusProgress.SetDialogText( str );

		int nSize = DataManager.Order.GetSize();
		for ( int nOrderIdx = 0; nOrderIdx < nSize; nOrderIdx++ )
		{
			StatusProgress.SetPos( nOrderIdx, nSize );

			COrderCSVRecord Order;
			DataManager.Order.GetAt( nOrderIdx, Order );

			int nSuppIdx;
			int nSuppNo = Order.GetSuppNo();
			if ( DataManager.Supplier.FindSupplierByNumber( nSuppNo, nSuppIdx ) == TRUE )
				arrayProtected.SetAt( nSuppIdx, 1 );
		}

		DataManager.CloseDatabase();

		for ( int nSuppIdx = 0; nSuppIdx < DataManager.Supplier.GetSize(); nSuppIdx++ )
		{
			int nSuppNo = DataManager.Supplier.GetSuppNo( nSuppIdx );
			
			CProcessedOrderArray arrayOrders;
			if ( arrayOrders.CheckForOpenOrders( nSuppNo, NODE_DATABASE, nDbIdx ) == TRUE )
				arrayProtected.SetAt( nSuppIdx, 1 );
		}
	}
	StatusProgress.Unlock();

	StatusProgress.Lock( TRUE, "Checking stockpoint orders" );	
	for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
	{
		StatusProgress.SetPos( nSpIdx, dbStockpoint.GetSize() );

		DataManager.OpenDatabaseQuickNoFiles( 0, FALSE );

		CDataManagerInfo info;
		DataManager.OpenOrderSp( nSpIdx, DATAMANAGER_OPEN_READONLY, info );

		CString str;
		str.Format ( "(%s)", dbStockpoint.GetName( nSpIdx ) );
		StatusProgress.SetDialogText( str );

		int nSize = DataManager.Order.GetSize();
		for ( int nOrderIdx = 0; nOrderIdx < nSize; nOrderIdx++ )
		{
			COrderCSVRecord Order;
			DataManager.Order.GetAt( nOrderIdx, Order );

			int nSuppIdx;
			int nSuppNo = Order.GetSuppNo();
			if ( DataManager.Supplier.FindSupplierByNumber( nSuppNo, nSuppIdx ) == TRUE )
				arrayProtected.SetAt( nSuppIdx, 1 );
		}
	
		DataManager.CloseDatabase();

		for ( int nSuppIdx = 0; nSuppIdx < DataManager.Supplier.GetSize(); nSuppIdx++ )
		{
			int nSuppNo = DataManager.Supplier.GetSuppNo( nSuppIdx );
			
			CProcessedOrderArray arrayOrders;
			if ( arrayOrders.CheckForOpenOrders( nSuppNo, NODE_STOCKPOINT, nSpIdx ) == TRUE )
				arrayProtected.SetAt( nSuppIdx, 1 );
		}
	}
	StatusProgress.Unlock();
		
	CWordArray arraySuppIdx;
	for ( int nSuppIdx = 0; nSuppIdx < DataManager.Supplier.GetSize(); nSuppIdx++ )
		if ( arrayProtected.GetAt( nSuppIdx ) == 0 )
			arraySuppIdx.Add( nSuppIdx );
	
	if ( arraySuppIdx.GetSize() == 0 )
		return 3;
		
	CListDataSupplierDeleteDlg dlg ( arraySuppIdx, this );
	dlg.DoModal();
	
	return 0;
}

/**********************************************************************/

void CMainFrameStockman::OnAdminChangeStockCodes() 
{
	if ( DataManager.LockAllUsers() == FALSE )
		Prompter.LockError( "change stock codes" );
	else
	{
		CDataManagerInfo info;
		int nResult = ChangeStockCodes( info );
		DataManager.UnlockAllUsers();

		switch( nResult )
		{
		case 1:
			Prompter.ShareError( info );
			break;

		case 2:
			Prompter.Error( "You must create some stock records before you can use this option" );
			break;
		}
	}
}

/**********************************************************************/

int CMainFrameStockman::ChangeStockCodes( CDataManagerInfo& info )
{
	int nDbIdx = 0;
	if ( PasswordArray.GetLocationAccessType() != LOCATION_ACCESS_SYSTEM )
		nDbIdx = PasswordArray.GetLocationAccessDbIdx();
	else if ( dbDatabase.GetSize() > 1 )
	{
		CSelectSingleDatabaseDlg dlg ( "Change Stock Codes", -1, this );
		if ( dlg.DoModal() != IDOK ) 
			return 0;
		
		nDbIdx = dlg.GetDbIdx();
	}

	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_PLU );
	arrayReadWrite.Add( NODE_STOCK );
	
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, info, TRUE, TRUE ) == FALSE )
		return 1;

	int nResult = ChangeStockCodesInternal();
	DataManager.CloseDatabase();

	return nResult;
}

/**********************************************************************/

int CMainFrameStockman::ChangeStockCodesInternal()
{
	if ( DataManager.Stock.GetSize() == 0 )
		return 2;

	MessageLogger.LogSystem( 6, 1, TRUE, DataManager.Stock.GetSize() );

	CStockListStockCodeDlg dlg ( this );
	dlg.DoModal();

	return 0;
}

/**********************************************************************/
