/**********************************************************************/
#include "..\CommonEcrStock\AutoExpireHistory.h"
#include "..\CommonEcrStock\ButtonActionHandler.h"
#include "CommsHandshaker.h"
#include "DatabaseCSVArray.h"
#include "DeliveryNoteDlg.h"
#include "ExplodePlu.h"
#include "..\CommonEcrStock\ListLabelsDlg.h"
#include "ManualSalesDatabasePluDlg.h"
#include "ManualSalesDatabaseStockDlg.h"
#include "NodeTypes.h"
#include "Password.h"
#include "..\CommonEcrStock\PresetReportManager.h"
#include "ReportAuditDlg.h"
#include "ReportStockNewDlg.h"
#include "SalesNoteDlg.h"
#include "StatusProgress.h"
#include "StockDatabaseDlg.h"
#include "StockListItemSetDlg.h"
#include "StockListOpeningDlg.h"
#include "StockListReorderDlg.h"
#include "StockListTransferDlg.h"
#include "StockLevelsAccessDlg.h"
#include "StockpointCSVArray.h"
#include "TransferNoteDlg.h"
/**********************************************************************/
#include "DataAccessHelpersStockData.h"
/**********************************************************************/

CDataAccessHelpersStockData::CDataAccessHelpersStockData()
{
}

/**********************************************************************/

void CDataAccessHelpersStockData::EditStock( int nDbNo, bool bFromButton )
{
	if ( FALSE == bFromButton )
		CommsHandshaker.ProcessSales();

	int nDbIdx;
	if ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) == FALSE )
		return;
	
	//FIND OUT IF WE NEED STOCK LEVEL ACCESS
	int nType = DealerFlags.GetStockLevelsStockType(); 
	if ( nType == STOCKLEVELS_ACCESS_PROMPT )
	{
		CStockLevelsAccessDlg dlg( TRUE, m_pParentWnd );
		if ( dlg.DoModal() != IDOK )
			return;

		nType = dlg.m_nType;
	}
	DealerFlags.SetCurrentStockLevelsAccessType(nType);

	CWaitCursor wait;
	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_PLU );
	arrayReadWrite.Add( NODE_STOCK );

	bool bReadWrite = FALSE;
	
	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange( nDbIdx, nStartIdx, nEndIdx );

	CDataManagerInfo info;
	//ATTEMPT TO OPEN THE DATABASE
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, info, FALSE ) == FALSE )
	{
		if ( info.m_nErrorType != DB_ERR_SHARE )
		{
			Prompter.ShareError( info );
			return;
		}
	}
	//DATABASE WAS OK, SO TRY THE STOCK LEVELS
	else
	{
		switch( DealerFlags.GetCurrentStockLevelsAccessType() )
		{
		case STOCKLEVELS_ACCESS_READWRITE:
			{		
				StatusProgress.Lock( TRUE, "Opening stock level files" );
				for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
				{
					StatusProgress.SetPos( nSpIdx - nStartIdx, nEndIdx + 1 - nStartIdx );

					if ( DataManager.OpenStockLevels( nSpIdx, DATAMANAGER_OPEN_READWRITE, info ) == FALSE )
					{
						DataManager.CloseDatabase();
							
						if ( info.m_nErrorType != DB_ERR_SHARE )
							Prompter.ShareError( info );
						
						break;
					}

					if ( DataManager.OpenTransferDetail( nSpIdx, DATAMANAGER_OPEN_READWRITE, info ) == FALSE )
					{
						DataManager.CloseDatabase();
							
						if ( info.m_nErrorType != DB_ERR_SHARE )
							Prompter.ShareError( info );
						
						break;
					}
				}
				StatusProgress.Unlock();

				if ( info.m_nErrorType == DATAMANAGER_ERROR_NONE )
					bReadWrite = TRUE;
			}
			break;

		case STOCKLEVELS_ACCESS_READONLY:
			{
				StatusProgress.Lock( TRUE, "Opening stock level files" );
				for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
				{
					StatusProgress.SetPos( nSpIdx - nStartIdx, nEndIdx + 1 - nStartIdx );
					DataManager.OpenStockLevels( nSpIdx, DATAMANAGER_OPEN_READONLY, info );
					DataManager.OpenTransferDetail( nSpIdx, DATAMANAGER_OPEN_READONLY, info );
				}
				StatusProgress.Unlock();
					
				bReadWrite = TRUE;
			}
			break;

		default:
			bReadWrite = TRUE;
			break;
		}
	}

	bool bReadOnly = FALSE;

	if ( bReadWrite == FALSE )
	{
		info.m_nNodeType = NODE_PLU;
		if ( Prompter.SuggestReadOnly( info ) != IDYES )
			return;
			
		DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
		
		switch( DealerFlags.GetCurrentStockLevelsAccessType() )
		{
		case STOCKLEVELS_ACCESS_READONLY:
		case STOCKLEVELS_ACCESS_READWRITE:
			{
				StatusProgress.Lock( TRUE, "Opening stock level files" );
				for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
				{
					StatusProgress.SetPos( nSpIdx - nStartIdx, nEndIdx + 1 - nStartIdx );
					DataManager.OpenStockLevels( nSpIdx, DATAMANAGER_OPEN_READONLY, info );
					DataManager.OpenTransferDetail( nSpIdx, DATAMANAGER_OPEN_READONLY, info );
				}
				StatusProgress.Unlock();
			}
			break;
		}
		
		bReadOnly = TRUE;
	}

	EditStockInternal( nDbIdx, bReadOnly );
	DataManager.CloseDatabase();
}

/**********************************************************************/

void CDataAccessHelpersStockData::EditStockInternal( int nDbIdx, bool bReadOnly )
{
	DataManager.StockPlu.Invalidate();

	CDataManagerInfo info;
	DataManagerNonDb.OpenCustomFieldNames( DB_READONLY, info );

	CStockDatabaseDlg dlg ( nDbIdx, m_pParentWnd );
	dlg.SetReadOnlyFlag( bReadOnly );
	dlg.DoModal();

	DataManagerNonDb.CloseCustomFieldNames( info );

	//CHECK FOR CHANGED LABELS
	if ( bReadOnly == TRUE )
		return;

	if ( ( EcrmanOptions.GetLabelsShelfAutoBatchFlag() && EcrmanOptions.GetLabelsShelfPromptBatchFlag() ) == FALSE )
		return;

	CDataManagerInfo info2;
	if ( DataManager.OpenShelfLabels( DB_READWRITE, info2 ) == FALSE )
	{
		Prompter.ShareError( info2 );
		return;
	}
	
	DataManager.ShelfLabels.CreateShelfChangesList();
	
	if ( DataManager.ShelfLabels.GetSize() == 0 )
		return;
	
	CString strMsg = "One or more Plu records has changed since you last printed\n";      
	strMsg += "shelf edge labels for this price set.\n\n";
	strMsg += "Do you wish to print updated labels now.";

	if ( Prompter.YesNo( strMsg, TRUE ) != IDYES )
		return;

	CListLabelsDlg dlgLabels ( FALSE, FALSE, TRUE, DataManager.ShelfLabels, "", m_pParentWnd );

	int nResult = dlgLabels.DoModal();
	m_pParentWnd -> GetParent() -> RedrawWindow();
	
	if ( nResult == IDOK )
	{
		DataManager.ShelfLabels.UpdateShelfChangesList();
		
		if ( DataManager.WriteShelfLabels( info ) == FALSE )
			Prompter.WriteError( info );
	}
}

/**********************************************************************/

void CDataAccessHelpersStockData::EditStockLevel ( int nSpIdx, int nType, bool bFromButton )
{		
	if ( FALSE == bFromButton )
		CommsHandshaker.ProcessSales();

	int nDbIdx = GetDbIdx ( dbStockpoint.GetDbNo( nSpIdx ) );
	if ( nDbIdx == -1 ) return;
	
	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	if ( DataManager.OpenStockLevels( nSpIdx, DATAMANAGER_OPEN_READWRITE, info ) == FALSE )
	{
		DataManager.CloseDatabase();
		Prompter.ShareError( info );
		return;
	}

	if ( NODE_TRANSFER == nType )
	{
		if ( DataManager.OpenTransferDetail( nSpIdx, DATAMANAGER_OPEN_READWRITE, info ) == FALSE )
		{
			DataManager.CloseDatabase();
			Prompter.ShareError( info );
			return;
		}
	}

	int nResult = EditStockLevelInternal( nSpIdx, nType );
	DataManager.CloseDatabase();

	switch( nResult )
	{
	case 1:
		Prompter.Error ( "You must set up some stock records\nbefore selecting this option" );
		break;
	}
}

/**********************************************************************/

int CDataAccessHelpersStockData::EditStockLevelInternal( int nSpIdx, int nType )
{
	if ( DataManager.Stock.GetSize() == 0 )
		return 1;
		
	switch ( nType )
	{
	case NODE_STOCK_ACTIVE:
		{
			CStockListItemSetDlg dlg ( nSpIdx, m_pParentWnd );
			dlg.DoModal();
		}
		break;

	case NODE_OPENING:
		{
			CStockListOpeningDlg dlg ( nSpIdx, m_pParentWnd );
			dlg.DoModal();
		}
		break;

	case NODE_REORDER:
		{
			CStockListReorderDlg dlg ( nSpIdx, m_pParentWnd );
			dlg.DoModal();
		}
		break;

	case NODE_TRANSFER:
		{
			CTransferNoteDlg dlg ( nSpIdx, m_pParentWnd );
			
			if ( ( dlg.IsRelevant() == FALSE ) || ( dlg.DoModal() == IDOK ) )
			{
				CString strReference1 = dlg.GetReference1();
				CString strReference2 = dlg.GetReference2();
				::TrimSpaces ( strReference1, FALSE );
				::TrimSpaces ( strReference2, FALSE );

				int nDestSpIdx = dlg.GetDestSpIdx();
			
				bool bError = FALSE;
				
				CDataManagerInfo info;
				if ( DataManager.OpenStockLevels( nDestSpIdx, DATAMANAGER_OPEN_READWRITE, info ) == FALSE )
				{
					Prompter.ShareError( info );
					bError = TRUE;
				}

				if ( FALSE == bError )
				{
					if ( DataManager.OpenTransferDetail( nDestSpIdx, DATAMANAGER_OPEN_READWRITE, info ) == FALSE )
					{
						Prompter.ShareError( info );
						bError = TRUE;
					}
				}

				if ( FALSE == bError )
				{
					CStockListTransferDlg dlgTransfer ( nSpIdx, nDestSpIdx, strReference1, strReference2, m_pParentWnd );
					dlgTransfer.DoModal();
				}
			}
		}
		break;
	}
	
	return 0;
}

/**********************************************************************/

void CDataAccessHelpersStockData::EditManualSales( int nSpIdx, int nType, bool bFromButton )
{
	if ( FALSE == bFromButton )
		CommsHandshaker.ProcessSales();

	if ( nSpIdx < 0 || nSpIdx >= dbStockpoint.GetSize() )
		return;

	int nDbNo = dbStockpoint.GetDbNo( nSpIdx );

	int nDbIdx;
	if ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) == FALSE )
		return;

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	if ( DataManager.OpenStockLevels( nSpIdx, DATAMANAGER_OPEN_READWRITE, info ) == FALSE )
	{
		DataManager.CloseDatabase();
		Prompter.ShareError( info );
		return;
	}

	int nResult = EditManualSalesInternal( nSpIdx, nType );
	DataManager.CloseDatabase();

	switch( nResult )
	{
	case 1:
		Prompter.Error ( "You must set up some plu records\nbefore selecting this option" );
		break;

	case 2:
		Prompter.Error ( "You must set up some stock records\nbefore selecting this option" );
		break;
	}
}

/**********************************************************************/

int CDataAccessHelpersStockData::EditManualSalesInternal( int nSpIdx, int nType )
{
	if ( DataManager.Plu.GetSize() == 0 )
		return 1;
	
	if ( DataManager.Stock.GetSize() == 0 )
		return 2;
	
	DataManager.StockPlu.Invalidate();

	if ( NODE_MANUALSALE_STOCK == nType )
	{
		CString strFilename = "";
		CSalesNoteDlg dlgSession( nSpIdx, NODE_MANUALSALE_STOCK, m_pParentWnd );
		if ( dlgSession.GetSessionCount() != 0 )
		{
			if ( dlgSession.DoModal() != IDOK )
				return 0;

			strFilename = dlgSession.GetFilename();
		}

		CManualSalesDatabaseStockDlg dlg( nSpIdx, NULL, m_pParentWnd );
		dlg.SetPreviousSession( strFilename );
		dlg.DoModal();

		if ( dlg.m_bSaveChanges == TRUE )
		{
			CDataManagerInfo info;
			if ( DataManager.WriteStockLevels( nSpIdx, info ) == FALSE )
				Prompter.WriteError( info );
		}
	}
	else
	{
		CString strPreviousSession = "";
		CSalesNoteDlg dlgSession( nSpIdx, NODE_MANUALSALE_PLU, m_pParentWnd );
		if ( dlgSession.GetSessionCount() != 0 )
		{
			if ( dlgSession.DoModal() != IDOK )
				return 0;

			strPreviousSession = dlgSession.GetFilename();
		}

		CString strAutoImportFile = "";
			
		if ( strPreviousSession == "" )
		{
			CString strFilename;
			strFilename.Format( "%s\\auto_sales_plu.dat",
				dbStockpoint.GetFolderPath( nSpIdx ) );

			if ( ::FileExists( strFilename ) == TRUE )
				strAutoImportFile = strFilename;
		}
		
		CManualSalesDatabasePluDlg dlg ( nSpIdx, NULL, m_pParentWnd );
		dlg.SetPreviousSession( strPreviousSession );
		dlg.SetAutoImportFile( strAutoImportFile );
		dlg.DoModal();

		if ( dlg.m_bSaveChanges == TRUE )
		{
			CDataManagerInfo info;
			if ( DataManager.WriteStockLevels( nSpIdx, info ) == FALSE )
				Prompter.WriteError( info );
		}
	}

	return 0;
}

/**********************************************************************/

void CDataAccessHelpersStockData::EditMiscStockLevels ( int nSpIdx, int nType, bool bFromButton )
{
	if ( FALSE == bFromButton )
		CommsHandshaker.ProcessSales();

	if ( nSpIdx < 0 || nSpIdx >= dbStockpoint.GetSize() )
		return;

	int nDbNo = dbStockpoint.GetDbNo( nSpIdx );

	int nDbIdx;
	if ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) == FALSE )
		return;

	switch( nType )
	{
	case NODE_RETURN:
	case NODE_ADJUSTMENT_PLU:
	case NODE_ADJUSTMENT_STOCK:
		{
			CDataManagerInfo info;
			if ( DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE ) == FALSE )
			{
				Prompter.ShareError( info );
				return;
			}
		}
		break;

	case NODE_DELIVERY:
		{
			CWordArray arrayReadWrite;
			arrayReadWrite.Add( NODE_PLU );
			arrayReadWrite.Add( NODE_STOCK );
			arrayReadWrite.Add( NODE_BARCODE );

			if ( ( Sysset.IsBarmanSystem() == TRUE ) && ( EcrmanOptions.GetLabelsProductAutoBatchFlag() == TRUE ) )
				arrayReadWrite.Add( NODE_LABELS_PRODUCT );
		
			CDataManagerInfo info;
			if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, info, FALSE ) == FALSE )
			{
				Prompter.ShareError( info );
				return;
			}
		}
		break;

	default:
		return;
	}

	CDataManagerInfo info;
	if ( DataManager.OpenStockLevels( nSpIdx, DATAMANAGER_OPEN_READWRITE, info ) == FALSE )
	{
		DataManager.CloseDatabase();
		Prompter.ShareError( info );
		return;
	}

	int nResult = EditMiscStockLevelsInternal( nSpIdx, nType );
	
	DataManager.CloseDatabase();

	switch( nResult )
	{
	case 1:		::StockMovementError( nType, TRUE );	break;
	case 2:		::StockMovementError( nType, FALSE );	break;
	}
}

/**********************************************************************/

int CDataAccessHelpersStockData::EditMiscStockLevelsInternal( int nSpIdx, int nType )
{
	switch( nType )
	{
	case NODE_RETURN:
	case NODE_ADJUSTMENT_PLU:
	case NODE_ADJUSTMENT_STOCK:

		if ( DataManager.Plu.GetSize() == 0 )
			return 1;

		if ( DataManager.Stock.GetSize() == 0 )
			return 2;

		break;
	}

	CDeliveryNoteDlg dlg ( nSpIdx, nType, NULL, m_pParentWnd );
	dlg.CreateSessionList();
	dlg.DoModal();

	return 0;
}

/**********************************************************************/

void CDataAccessHelpersStockData::HandleStockReports( int nPresetNo, bool bAutoRunPreset, CLocationSelectorEntity& LocSelEntity )
{
	AutoExpireHistory.ExpireReportsNow();

	if ( dbStockpoint.GetSize() == 0 )
		ShowStockpointError();
	else if ( PasswordArray.CheckLocationAccessSp() == FALSE )
		Prompter.StockpointAccessError();
	else
	{
		DataManager.StockPlu.Invalidate();

		CDataManagerInfo info;
		DataManagerNonDb.OpenCustomFieldNames( DB_READONLY, info );
		PresetReportManager.Open( NODE_PRESET_STOCK_LEVEL, DB_READONLY, info );
		DataManagerNonDb.OpenAllStockReportSortOrders();

		CReportStockNewDlg dlg ( nPresetNo, bAutoRunPreset, m_pParentWnd );
		dlg.SetDefaultSelection ( LocSelEntity.GetEntityIdx(), LocSelEntity.GetConType() );
		dlg.DoModal();

		switch( dlg.GetPresetError() )
		{
		case 1:
			{
				CString strMsg;
				strMsg.Format( "Unknown preset report #%3.3d", nPresetNo );
				Prompter.Error( strMsg );
			}
			break;

		case 2:
			ButtonActionHandler.ShowAuthorisationError(6);
			break;
		}

		DataManagerNonDb.CloseCustomFieldNames( info );
		PresetReportManager.Close( NODE_PRESET_STOCK_LEVEL, info );
		DataManagerNonDb.CloseAllStockReportSortOrders();
	}
}

/**********************************************************************/

void CDataAccessHelpersStockData::HandleStockAudits( int nPresetNo, bool bAutoRunPreset, CLocationSelectorEntity& LocSelEntity )
{
	AutoExpireHistory.ExpireReportsNow();

	if ( dbStockpoint.GetSize() == 0 )
		ShowStockpointError();
	else if ( PasswordArray.CheckLocationAccessSp() == FALSE )
		Prompter.StockpointAccessError();
	else
	{
		CDataManagerInfo info;
		PresetReportManager.Open( NODE_PRESET_STOCK_AUDIT, DB_READONLY, info );

		CReportAuditDlg dlg ( nPresetNo, bAutoRunPreset, m_pParentWnd );
		dlg.SetDefaultSelection ( LocSelEntity.GetEntityIdx(), LocSelEntity.GetConType() );
		dlg.DoModal();

		PresetReportManager.Close( NODE_PRESET_STOCK_AUDIT, info );
	}
}

/**********************************************************************/

void CDataAccessHelpersStockData::BuildDbTransferCounts()
{
	m_arrayDbTransferTargets.RemoveAll();
	for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
	{
		int nStartIdx, nEndIdx;
		dbStockpoint.GetSpIdxRange( nDbIdx, nStartIdx, nEndIdx );
		
		int nTransferTargets = 0;

		for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
		{
			if ( dbStockpoint.GetTransferInFlag(nSpIdx) == TRUE )
				nTransferTargets++;
		}

		m_arrayDbTransferTargets.Add( nTransferTargets );
	}
}

/**********************************************************************/

int CDataAccessHelpersStockData::GetDbTransferTargets( int nDbIdx )
{
	if ( ( nDbIdx >= 0 ) && ( nDbIdx < m_arrayDbTransferTargets.GetSize() ) )
		return m_arrayDbTransferTargets.GetAt( nDbIdx );
	else
		return 0;
}

/**********************************************************************/




