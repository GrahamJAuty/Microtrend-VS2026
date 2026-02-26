/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
#include "DatabaseCSVArray.h"
//#include "DealerFlags.h"
//#include "globalfunctions.h"
#include "NodeTypes.h"
#include "StatusProgress.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "SyncStockLevels.h"
/**********************************************************************/

CSyncStockLevels::CSyncStockLevels(void)
{
}

/**********************************************************************/

void CSyncStockLevels::CheckStockpoint()
{
	int nDbNo, nSpNo;
	if ( dbStockpoint.GetSyncRequestStockLevels( nDbNo, nSpNo ) == FALSE )
		return;

	int nDbIdx;
	if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
	{
		dbStockpoint.RemoveSyncRequestStockLevels();
		return;
	}

	int nSpIdx;
	if ( dbStockpoint.FindStockpointByNumber( nDbNo, nSpNo, nSpIdx ) == FALSE )
	{
		dbStockpoint.RemoveSyncRequestStockLevels();
		return;
	}

	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_STOCK );

	CWordArray arrayDummy;

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayDummy,info, FALSE ) == FALSE )
		return;

	if ( DataManager.OpenStockLevels( nSpIdx, DATAMANAGER_OPEN_READWRITE, info ) == FALSE )
	{
		DataManager.CloseDatabase();
		return;
	}

	DealerFlags.SetBlockExportLiveApparentStockFlag( TRUE );

	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange( nDbIdx, nStartIdx, nEndIdx );

	int nProgressTarget = ( 5 * DataManager.Stock.GetSize() );

	StatusProgress.Lock( TRUE, "Synchronising stock level file" );
	
	CString str;
	str.Format( "(%s)", dbStockpoint.GetName( nSpIdx ) );
	StatusProgress.SetDialogText( str );

	::MicrotrendDebugMessageBox( str );

	DataManager.Stock.SynchroniseStockLevels( nSpIdx - nStartIdx, 0, nProgressTarget );
	
	StatusProgress.Unlock();

	DataManager.WriteStockLevels( nSpIdx, info );	
	DataManager.CloseDatabase();

	dbStockpoint.RemoveSyncRequestStockLevels();

	DealerFlags.SetBlockExportLiveApparentStockFlag( FALSE );
}

/**********************************************************************/

