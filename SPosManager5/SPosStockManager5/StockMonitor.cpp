/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "ExplodePlu.h"
#include "MessageLogger.h"
#include "NodeTypes.h"
#include "PriceHelpers.h"
#include "ReportHelpers.h"
#include "RepCSV.h"
#include "SalesHistoryFields.h"
#include "SalesTypes.h"
#include "StockAuditBaseReport.h"
#include "StockOptionsIni.h"
#include "StockpointCSVArray.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "StockMonitor.h"
/**********************************************************************/

CStockMonitor::CStockMonitor()
{
	m_bQuickMode = FALSE;
}

/**********************************************************************/

CStockMonitor::~CStockMonitor()
{
	DeleteStockLevels();
}

/**********************************************************************/

void CStockMonitor::DeleteStockLevels()
{
	for ( int nIndex = 0; nIndex < m_arrayStockMonitorInfo.GetSize(); nIndex++ )
	{
		CTransferDetailCSVArray* pArray1 = m_arrayStockMonitorInfo[nIndex].m_pTransferDetail;
		delete pArray1;

		CStockLevelsCSVArray* pArray2 = m_arrayStockMonitorInfo[nIndex].m_pStockLevels;
		delete pArray2;

		CArray<double,double>* pSales = m_arrayStockMonitorInfo[nIndex].m_pSalesLevels;
		delete pSales;
	}

	m_arrayStockMonitorInfo.RemoveAll();
}

/**********************************************************************/

void CStockMonitor::CreateStockLevels( int nStartSpIdx, int nEndSpIdx )
{
	for ( int nSpIdx = nStartSpIdx; nSpIdx <= nEndSpIdx; nSpIdx++ )
	{
		CStockMonitorInfo info;

		int nSpNo = dbStockpoint.GetSpNo ( nSpIdx );
		info.m_pTransferDetail = new CTransferDetailCSVArray( nSpNo );
		info.m_pStockLevels = new CStockLevelsCSVArray ( nSpNo );
		info.m_pSalesLevels = new CArray<double,double>;
		info.m_nDataManagerStatusStockLevels = DATAMANAGER_CLOSED;
		info.m_nDataManagerStatusTransferDetail = DATAMANAGER_CLOSED;

		m_arrayStockMonitorInfo.Add( info );
	}
}

/**********************************************************************/

bool CStockMonitor::InsertRecord ( CStockCSVRecord& NewRecord, int& nStockIdx )
{
	CString strStockCode = NewRecord.GetStockCode();

	if ( FindStockItemByCode ( strStockCode, nStockIdx ) )
		return FALSE;

	m_StockDatabase.InsertAt ( nStockIdx, NewRecord );
	
	CStockLevelsCSVRecord StockLevels;
	for ( int nIndex = 0; nIndex < m_arrayStockMonitorInfo.GetSize(); nIndex++ )
	{
		CStockLevelsCSVArray* pArray = m_arrayStockMonitorInfo[nIndex].m_pStockLevels;

		int nLevelIdx;
		if ( pArray -> FindStockItemByCode ( -1, strStockCode, nLevelIdx ) == FALSE )
		{
			StockLevels.SetStockCode ( strStockCode );
			pArray -> InsertAt ( nLevelIdx, StockLevels );
		}
	}

	return TRUE;
}

/**********************************************************************/

void CStockMonitor::RemoveAt ( int nStockIdx )
{
	if ( nStockIdx < 0 || nStockIdx >= m_StockDatabase.GetSize() )
		return;

	CString strStockCode = m_StockDatabase.GetStockCode ( nStockIdx );
	m_StockDatabase.RemoveAt ( nStockIdx );

	for ( int nIndex = 0; nIndex < m_arrayStockMonitorInfo.GetSize(); nIndex++ )
	{
		CStockLevelsCSVArray* pArray = m_arrayStockMonitorInfo[nIndex].m_pStockLevels;	

		int nLevelIdx;
		if ( pArray -> FindStockItemByCode ( nStockIdx, strStockCode, nLevelIdx ) )
			pArray -> RemoveAt ( nLevelIdx );
	}

	for ( int nIndex = 0; nIndex < m_arrayStockMonitorInfo.GetSize(); nIndex++ )
	{
		CTransferDetailCSVArray* pArray = m_arrayStockMonitorInfo[nIndex].m_pTransferDetail;	

		int nLevelIdx;
		if ( pArray -> FindStockItemByCode ( nStockIdx, strStockCode, nLevelIdx ) )
			pArray -> RemoveAt ( nLevelIdx );
	}
}

/**********************************************************************/

void CStockMonitor::GetStockpointTotals ( int nMasterIdx, CStockCSVRecord& StockRecord, int nSpNo, CStockLevelsCSVRecord& StockLevels )
{
	CStockFilterMinMaxFlags MinMaxFlags( FALSE );
	GetStockpointTotals( nMasterIdx, StockRecord, nSpNo, StockLevels, MinMaxFlags );
}

/**********************************************************************/

void CStockMonitor::GetStockpointTotals ( int nMasterIdx, CStockCSVRecord& StockRecord, int nSpNo, CStockLevelsCSVRecord& StockLevels, CStockFilterMinMaxFlags& MinMaxFlags )
{
	MinMaxFlags.ResetResults( TRUE );

	if ( m_bQuickMode == FALSE )
	{
		CString strStockCode = StockRecord.GetStockCode();
		StockLevels.Reset();
	
		for ( int nIndex = 0; nIndex < m_arrayStockMonitorInfo.GetSize(); nIndex++ )
		{
			CStockLevelsCSVArray* pArray = m_arrayStockMonitorInfo[nIndex].m_pStockLevels;
		
			if ( pArray -> GetSpNo() == nSpNo )
			{
				bool bOldSync = pArray -> IsInSync();

				int nStockIdx;
				if ( pArray -> FindStockItemByCode ( nMasterIdx, strStockCode, nStockIdx ) )
					pArray -> GetAt ( nStockIdx, StockLevels );
				else
					StockLevels.SetStockCode( strStockCode );

				if ( MinMaxFlags.BelowMin() == FALSE )
					MinMaxFlags.SetBelowMin( StockLevels.GetApparentQty() < StockLevels.GetMinimumLevelQty() );

				if ( MinMaxFlags.AboveMax() == FALSE )
				{
					double dMaximum = StockLevels.GetMaximumLevelQty();
					MinMaxFlags.SetAboveMax( ( dMaximum > 0.0 ) && ( StockLevels.GetApparentQty() > dMaximum ) );
				}

				bool bNewSync = pArray -> IsInSync();

				if ( ( bOldSync == TRUE ) && ( bNewSync == FALSE ) )
				{
					int nDbIdx = DataManager.GetActiveDbIdx();

					if ( nDbIdx >= 0 && nDbIdx < dbDatabase.GetSize() )
					{
						int nDbNo = dbDatabase.GetDbNo( nDbIdx );
						dbStockpoint.AddSyncRequestStockLevels( nDbNo, pArray -> GetSpNo());
					}
				}

				break;
			}
		}

		if ( StockOptions.GetStockSubUnitsFlag() )
			StockLevels.SetAutoWastagePercent( StockRecord.GetWastePercent() );
		else
			StockLevels.SetAutoWastagePercent( 0.0 );
	}
	else
	{
		StockLevels.Reset();
	
		for ( int nIndex = 0; nIndex < m_arrayStockMonitorInfo.GetSize(); nIndex++ )
		{
			CStockLevelsCSVArray* pArray = m_arrayStockMonitorInfo[nIndex].m_pStockLevels;
		
			if ( pArray -> GetSpNo() == nSpNo )
			{
				int nPos = nMasterIdx * 8;
				CArray<double,double>* pSalesLevels = m_arrayStockMonitorInfo[nIndex].m_pSalesLevels;
					
				if ( nPos >= 0 && nPos < ( pSalesLevels -> GetSize() - 7 ) )
				{
					StockLevels.SetEposSalesQty( 0, pSalesLevels -> GetAt( nPos ) );
					StockLevels.SetEposSalesValue( 0, pSalesLevels -> GetAt( nPos + 1) );
					StockLevels.SetManualSalesQty( pSalesLevels -> GetAt( nPos + 2) );
					StockLevels.SetManualSalesValue( pSalesLevels -> GetAt( nPos + 3) );
					StockLevels.SetImportSalesQty( pSalesLevels -> GetAt( nPos + 4 ) );
					StockLevels.SetImportSalesValue( pSalesLevels -> GetAt( nPos + 5 ) );
				}
				break;
			}
		}
	}

	if ( MinMaxFlags.CheckMin() == FALSE )
		MinMaxFlags.SetBelowMin( FALSE );

	if ( MinMaxFlags.CheckMax() == FALSE )
		MinMaxFlags.SetAboveMax( FALSE );
}

/**********************************************************************/

void CStockMonitor::SetStockpointTotals ( int nMasterIdx, CStockCSVRecord& StockRecord, int nSpNo, CStockLevelsCSVRecord& StockLevels )
{
	CString strStockCode = StockRecord.GetStockCode();
	StockLevels.SetStockCode ( strStockCode );

	for ( int nIndex = 0; nIndex < m_arrayStockMonitorInfo.GetSize(); nIndex++ )
	{
		CStockLevelsCSVArray* pArray = m_arrayStockMonitorInfo[nIndex].m_pStockLevels;
		
		if ( pArray -> GetSpNo() == nSpNo )
		{
			int nStockIdx;
			if ( pArray -> FindStockItemByCode ( nMasterIdx, strStockCode, nStockIdx ) )
				pArray -> SetAt ( nStockIdx, StockLevels );
			else
				pArray -> InsertAt ( nStockIdx, StockLevels );

			break;
		}
	}
}

/**********************************************************************/

void CStockMonitor::GetSiteTotals ( int nMasterIdx, CStockCSVRecord& StockRecord, int nSiteNo, CStockLevelsCSVRecord& StockLevels )
{
	CStockFilterMinMaxFlags MinMaxFlags( FALSE );
	GetSiteTotals( nMasterIdx, StockRecord, nSiteNo, StockLevels, MinMaxFlags );
}

/**********************************************************************/

void CStockMonitor::GetSiteTotals ( int nMasterIdx, CStockCSVRecord& StockRecord, int nSiteNo, CStockLevelsCSVRecord& StockLevels, CStockFilterMinMaxFlags& MinMaxFlags )
{
	MinMaxFlags.ResetResults( TRUE );

	if ( m_bQuickMode == FALSE )
	{
		CString strStockCode = StockRecord.GetStockCode();
		StockLevels.Reset();
		StockLevels.SetStocktakeNo(99);

		int nDbIdx = DataManager.GetActiveDbIdx();
		int nDbNo = dbDatabase.GetDbNo( nDbIdx );

		for ( int nIndex = 0; nIndex < m_arrayStockMonitorInfo.GetSize(); nIndex++ )
		{
			CStockLevelsCSVArray* pArray = m_arrayStockMonitorInfo[nIndex].m_pStockLevels;

			int nSpIdx;
			int nSpNo = pArray -> GetSpNo();
			if ( dbStockpoint.FindStockpointByNumber ( nDbNo, nSpNo, nSpIdx ) == FALSE )
				continue;

			if ( nSiteNo != dbStockpoint.GetSiteNo ( nSpIdx ) )
				continue;
	
			bool bOldSync = pArray -> IsInSync();

			int nStockIdx;
			if ( pArray -> FindStockItemByCode ( nMasterIdx, strStockCode, nStockIdx ) )
			{
				CStockLevelsCSVRecord TempStockLevels;
				pArray -> GetAt ( nStockIdx, TempStockLevels );
				StockLevels = StockLevels + TempStockLevels;

				if ( MinMaxFlags.BelowMin() == FALSE )
					MinMaxFlags.SetBelowMin( StockLevels.GetApparentQty() < StockLevels.GetMinimumLevelQty() );

				if ( MinMaxFlags.AboveMax() == FALSE )
				{
					double dMaximum = StockLevels.GetMaximumLevelQty();
					MinMaxFlags.SetAboveMax( ( dMaximum > 0.0 ) && ( StockLevels.GetApparentQty() > dMaximum ) );
				}
			}

			bool bNewSync = pArray -> IsInSync();

			if ( ( bOldSync == TRUE ) && ( bNewSync == FALSE ) )
			{
				int nDbIdx = DataManager.GetActiveDbIdx();

				if ( nDbIdx >= 0 && nDbIdx < dbDatabase.GetSize() )
				{
					int nDbNo = dbDatabase.GetDbNo( nDbIdx );
					dbStockpoint.AddSyncRequestStockLevels( nDbNo, pArray -> GetSpNo());
				}
			}
		}

		if ( StockOptions.GetStockSubUnitsFlag() )
			StockLevels.SetAutoWastagePercent( StockRecord.GetWastePercent() );
		else
			StockLevels.SetAutoWastagePercent( 0.0 );
	}
	else
	{
		StockLevels.Reset();
		StockLevels.SetStocktakeNo(99);

		int nDbNo = DataManager.GetActiveDbIdx();

		for ( int nIndex = 0; nIndex < m_arrayStockMonitorInfo.GetSize(); nIndex++ )
		{
			CStockLevelsCSVArray* pArray = m_arrayStockMonitorInfo[nIndex].m_pStockLevels;

			int nSpIdx;
			int nSpNo = pArray -> GetSpNo();
			if ( dbStockpoint.FindStockpointByNumber ( nDbNo, nSpNo, nSpIdx ) == FALSE )
				continue;

			if ( nSiteNo != dbStockpoint.GetSiteNo ( nSpIdx ) )
				continue;
	
			int nPos = nMasterIdx * 8;
			CArray<double,double>* pSalesLevels = m_arrayStockMonitorInfo[nIndex].m_pSalesLevels;
				
			if ( nPos >= 0 && nPos < ( pSalesLevels -> GetSize() - 7 ) )
			{
				CStockLevelsCSVRecord TempStockLevels;
				TempStockLevels.SetEposSalesQty( 0, pSalesLevels -> GetAt( nPos ) );
				TempStockLevels.SetEposSalesValue( 0, pSalesLevels -> GetAt( nPos + 1) );
				TempStockLevels.SetManualSalesQty( pSalesLevels -> GetAt( nPos + 2) );
				TempStockLevels.SetManualSalesValue( pSalesLevels -> GetAt( nPos + 3) );
				TempStockLevels.SetImportSalesQty( pSalesLevels -> GetAt( nPos + 4 ) );
				TempStockLevels.SetImportSalesValue( pSalesLevels -> GetAt( nPos + 5 ) );
				StockLevels = StockLevels + TempStockLevels;
			}
		}
	}

	if ( MinMaxFlags.CheckMin() == FALSE )
		MinMaxFlags.SetBelowMin( FALSE );

	if ( MinMaxFlags.CheckMax() == FALSE )
		MinMaxFlags.SetAboveMax( FALSE );
}

/**********************************************************************/

void CStockMonitor::GetDatabaseTotals ( int nMasterIdx, CStockCSVRecord& StockRecord, CStockLevelsCSVRecord& StockLevels )
{
	CStockFilterMinMaxFlags MinMaxFlags( FALSE );
	GetDatabaseTotals( nMasterIdx, StockRecord, StockLevels, MinMaxFlags );
}

/**********************************************************************/

void CStockMonitor::GetDatabaseTotals ( int nMasterIdx, CStockCSVRecord& StockRecord, CStockLevelsCSVRecord& StockLevels, CStockFilterMinMaxFlags& MinMaxFlags )
{
	MinMaxFlags.ResetResults( TRUE );

	if ( m_bQuickMode == FALSE )
	{
		CString strStockCode = StockRecord.GetStockCode();
		StockLevels.Reset();
		StockLevels.SetStocktakeNo(99);

		for ( int nIndex = 0; nIndex < m_arrayStockMonitorInfo.GetSize(); nIndex++ )
		{
			CStockLevelsCSVArray* pArray = m_arrayStockMonitorInfo[nIndex].m_pStockLevels;

			bool bOldSync = pArray -> IsInSync();

			int nStockIdx;
			if ( pArray -> FindStockItemByCode ( nMasterIdx, strStockCode, nStockIdx ) )
			{
				CStockLevelsCSVRecord TempStockLevels;
				pArray -> GetAt ( nStockIdx, TempStockLevels );
				StockLevels = StockLevels + TempStockLevels;

				if ( MinMaxFlags.BelowMin() == FALSE )
					MinMaxFlags.SetBelowMin( StockLevels.GetApparentQty() < StockLevels.GetMinimumLevelQty() );

				if ( MinMaxFlags.AboveMax() == FALSE )
				{
					double dMaximum = StockLevels.GetMaximumLevelQty();
					MinMaxFlags.SetAboveMax( ( dMaximum > 0.0 ) && ( StockLevels.GetApparentQty() > dMaximum ) );
				}
			}

			bool bNewSync = pArray -> IsInSync();

			if ( ( bOldSync == TRUE ) && ( bNewSync == FALSE ) )
			{
				int nDbIdx = DataManager.GetActiveDbIdx();

				if ( nDbIdx >= 0 && nDbIdx < dbDatabase.GetSize() )
				{
					int nDbNo = dbDatabase.GetDbNo( nDbIdx );
					dbStockpoint.AddSyncRequestStockLevels( nDbNo, pArray -> GetSpNo());
				}
			}
		}

		if ( StockOptions.GetStockSubUnitsFlag() )
			StockLevels.SetAutoWastagePercent( StockRecord.GetWastePercent() );
		else
			StockLevels.SetAutoWastagePercent( 0.0 );
	}
	else
	{
		StockLevels.Reset();
		StockLevels.SetStocktakeNo(99);

		for ( int nIndex = 0; nIndex < m_arrayStockMonitorInfo.GetSize(); nIndex++ )
		{
			CStockLevelsCSVArray* pArray = m_arrayStockMonitorInfo[nIndex].m_pStockLevels;

			int nPos = nMasterIdx * 8;
			CArray<double,double>* pSalesLevels = m_arrayStockMonitorInfo[nIndex].m_pSalesLevels;
				
			if ( nPos >= 0 && nPos < ( pSalesLevels -> GetSize() - 7 ) )
			{
				CStockLevelsCSVRecord TempStockLevels;
				TempStockLevels.SetEposSalesQty( 0, pSalesLevels -> GetAt( nPos ) );
				TempStockLevels.SetEposSalesValue( 0, pSalesLevels -> GetAt( nPos + 1) );
				TempStockLevels.SetManualSalesQty( pSalesLevels -> GetAt( nPos + 2) );
				TempStockLevels.SetManualSalesValue( pSalesLevels -> GetAt( nPos + 3) );
				TempStockLevels.SetImportSalesQty( pSalesLevels -> GetAt( nPos + 4 ) );
				TempStockLevels.SetImportSalesValue( pSalesLevels -> GetAt( nPos + 5 ) );
				StockLevels = StockLevels + TempStockLevels;
			}
		}
	}

	if ( MinMaxFlags.CheckMin() == FALSE )
		MinMaxFlags.SetBelowMin( FALSE );

	if ( MinMaxFlags.CheckMax() == FALSE )
		MinMaxFlags.SetAboveMax( FALSE );
}

/**********************************************************************/

void CStockMonitor::GetTransferTotals ( int nMasterIdx, CStockCSVRecord& StockRecord, int nSpNo, CTransferDetailCSVRecord& TransferTotals )
{
	CString strStockCode = StockRecord.GetStockCode();
	TransferTotals.Reset();

	for ( int nIndex = 0; nIndex < m_arrayStockMonitorInfo.GetSize(); nIndex++ )
	{
		CTransferDetailCSVArray* pArray = m_arrayStockMonitorInfo[nIndex].m_pTransferDetail;
	
		if ( pArray -> GetSpNo() == nSpNo )
		{
			int nStockIdx;
			if ( pArray -> FindStockItemByCode ( nMasterIdx, strStockCode, nStockIdx ) )
				pArray -> GetAt ( nStockIdx, TransferTotals );
			else
				TransferTotals.SetStockCode( strStockCode );

			break;
		}
	}
}

/**********************************************************************/

void CStockMonitor::SetTransferTotals ( int nMasterIdx, CStockCSVRecord& StockRecord, int nSpNo, CTransferDetailCSVRecord& TransferTotals )
{
	CString strStockCode = StockRecord.GetStockCode();
	TransferTotals.SetStockCode ( strStockCode );

	for ( int nIndex = 0; nIndex < m_arrayStockMonitorInfo.GetSize(); nIndex++ )
	{
		CTransferDetailCSVArray* pArray = m_arrayStockMonitorInfo[nIndex].m_pTransferDetail;
		
		if ( pArray -> GetSpNo() == nSpNo )
		{
			int nStockIdx;
			if ( pArray -> FindStockItemByCode ( nMasterIdx, strStockCode, nStockIdx ) )
				pArray -> SetAt ( nStockIdx, TransferTotals );
			else
				pArray -> InsertAt ( nStockIdx, TransferTotals );

			break;
		}
	}
}

/**********************************************************************/

void CStockMonitor::RemoveTransferTotals ( int nMasterIdx, CStockCSVRecord& StockRecord, int nSpNo )
{
	CString strStockCode = StockRecord.GetStockCode();

	for ( int nIndex = 0; nIndex < m_arrayStockMonitorInfo.GetSize(); nIndex++ )
	{
		CTransferDetailCSVArray* pArray = m_arrayStockMonitorInfo[nIndex].m_pTransferDetail;
		
		if ( pArray -> GetSpNo() == nSpNo )
		{
			int nStockIdx;
			if ( pArray -> FindStockItemByCode ( nMasterIdx, strStockCode, nStockIdx ) )
				pArray -> RemoveAt ( nStockIdx );
			
			break;
		}
	}
}

/**********************************************************************/

bool CStockMonitor::IsModified()
{
	if ( m_StockDatabase.IsModified() )
		return TRUE;

	for ( int nIndex = 0; nIndex < m_arrayStockMonitorInfo.GetSize(); nIndex++ )
	{
		if ( m_arrayStockMonitorInfo[nIndex].m_pStockLevels -> IsModified() )
			return TRUE;

		if ( m_arrayStockMonitorInfo[nIndex].m_pTransferDetail -> IsModified() )
			return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

void CStockMonitor::GenerateFromAuditedSales( CLocationSelectorEntity& LocSelEntity, const char* szFrom, const char* szTo, bool bIncludeReorderedItems )
{
	CSSDate dateFrom ( szFrom );
	CSSDate dateTo ( szTo );

	//REMOVE STOCK LEVELS
	for ( int nIndex = 0; nIndex < m_arrayStockMonitorInfo.GetSize(); nIndex++ )
	{	
		//CStockLevelsCSVArray* pArray = m_arrayStockMonitorInfo[nIndex].m_pStockLevels;
		//pArray -> RemoveAll();

		CArray<double,double>* pSalesArray = m_arrayStockMonitorInfo[nIndex].m_pSalesLevels;
		pSalesArray -> RemoveAll();
		if ( GetSize() > 0 ) pSalesArray -> InsertAt( 0, 0.0, GetSize() * 8 );
	}

	int nStartIdx, nEndIdx;
	dbStockpoint.GetSpIdxRange( LocSelEntity.GetDbIdx(), nStartIdx, nEndIdx );
	
	//LOOP THROUGH ALL THE STOCKPOINTS ON THIS DATABASE
	for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
	{
		bool bMatch = FALSE;
		//FIND OUT IF THIS STOCKPOINT IS INCLUDED IN THE REPORT
		switch ( LocSelEntity.GetConType() )
		{
		case NODE_STOCKPOINT:	bMatch = ( dbStockpoint.GetSpNo( nSpIdx ) == LocSelEntity.GetEntityNo() );	break;
		case NODE_SITE:			bMatch = ( dbStockpoint.GetSiteNo( nSpIdx ) == LocSelEntity.GetEntityNo() );	break;
		case NODE_DATABASE:		bMatch = TRUE;		break;
		default:				bMatch = FALSE;		break;
		}
					
		//NOTHING TO DO IF THIS STOCKPOINT IS NOT REQUIRED
		if ( bMatch == FALSE )
			continue;
		
		StatusProgress.Lock( TRUE, "Checking audited sales" );
		GenerateFromAuditedSalesInternal ( nSpIdx, szFrom, szTo, bIncludeReorderedItems );
		StatusProgress.Unlock();	
	}
}

/**********************************************************************/

void CStockMonitor::GenerateFromAuditedSalesInternal ( int nSpIdx, const char* szFrom, const char* szTo, bool bIncludeReorderedItems )
{
	CString strDateFrom = szFrom;
	if ( strDateFrom.GetLength() != 10 )
		return;
	
	CString strDateTo = szTo;
	if ( strDateTo.GetLength() != 10 )
		return;

	CString strSearchDateFrom = strDateFrom.Right(4) + strDateFrom.Mid(3,2) + strDateFrom.Left(2);
	if ( ::TestNumeric( strSearchDateFrom ) == FALSE )
		return;
	
	CString strSearchDateTo = strDateTo.Right(4) + strDateTo.Mid(3,2) + strDateTo.Left(2);
	if ( ::TestNumeric( strSearchDateTo ) == FALSE )
		return;

	if ( strSearchDateFrom > strSearchDateTo )
	{
		CString strTemp = strSearchDateFrom;
		strSearchDateFrom = strSearchDateTo;
		strSearchDateTo = strTemp;
	}

	CString strSearchMonthFrom = strSearchDateFrom.Left(6);
	CString strSearchMonthTo = strSearchDateTo.Left(6);
	int nMDayFrom = atoi ( strSearchDateFrom.Right(2) );
	int nMDayTo = atoi ( strSearchDateTo.Right(2) );

	__int64 nTotalLength = 0;
	CStringArray FileArray, MonthArray;
	dbStockpoint.GetHistoryFileList( nSpIdx, strSearchMonthFrom, strSearchMonthTo, FileArray, MonthArray, nTotalLength, FALSE );

	__int64 nProgress = 0;
	int nSpNo = dbStockpoint.GetSpNo( nSpIdx );

	//NOTHING TO DO IF WE CANNOT FIND THE STOCKPOINT
	bool bFoundStockpoint = FALSE;
	CStockLevelsCSVArray* pStockLevelsArray = NULL;
	CArray<double,double>* pSalesArray = NULL;
	for ( int n = 0; n < m_arrayStockMonitorInfo.GetSize(); n++ )
	{
		pStockLevelsArray = m_arrayStockMonitorInfo[n].m_pStockLevels;
		pSalesArray = m_arrayStockMonitorInfo[n].m_pSalesLevels;
		
		if ( pStockLevelsArray -> GetSpNo() == nSpNo )
		{
			bFoundStockpoint = TRUE;
			break;
		}
	}
	if ( bFoundStockpoint == FALSE )
		return;

	//STOCK LEVEL ARRAY MUST BE SAME SIZE AS STOCK DATABASE
	if ( pSalesArray -> GetSize() != GetSize() * 8 )
		return;
	
	for ( int nIndex = 0; nIndex < FileArray.GetSize(); nIndex++ )
	{
		//NOTHING TO DO IF WE CANNOT OPEN THE SALES AUDIT FILE
		CSSFile fileAudit;
		if ( fileAudit.Open ( FileArray.GetAt( nIndex ), "rb" ) == FALSE )
			continue;

		CString strMonth = MonthArray.GetAt( nIndex );

		CString strBuffer;
		while ( fileAudit.ReadString ( strBuffer ) == TRUE )
		{
			StatusProgress.SetPos ( nProgress + fileAudit.Tell(), nTotalLength );
			
			//EXTRACT DETAILS FROM THIS SALES LINE
			CCSVSalesHistory csv ( strBuffer );
		
			if ( bIncludeReorderedItems == FALSE && csv.GetReorderedFlag() == TRUE )
				continue;

			if ( strMonth == strSearchMonthFrom )
				if ( csv.GetMDay() < nMDayFrom )
					continue;

			if ( strMonth == strSearchMonthTo )
				if ( csv.GetMDay() > nMDayTo )
					continue;

			CString strStockCode = csv.GetStockCode();
			
			//NOTHING TO DO IF THE STOCK CODE COULD NOT BE FOUND
			int nStockIdx;
			if ( FindStockItemByCode ( strStockCode, nStockIdx ) == FALSE )
				continue;

			{
				CStockCSVRecord StockRecord;
				GetAt( nStockIdx, StockRecord );

				//NOTHING TO DO IF INACTIVE AT THIS STOCKPOINT
				
				SetQuickModeFlag( FALSE );
				CStockLevelsCSVRecord StockLevels;
				GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
				SetQuickModeFlag( TRUE );
				
				if ( StockLevels.GetActiveFlag() == FALSE )
					continue;
			}

			int nType = csv.GetType();
			double dQty = csv.GetStockQty();
			double dValue = csv.GetStockValue();
			int nSalesPos = 8 * nStockIdx;

			switch ( nType )
			{
			case SALES_TYPE_E1:			break;
			case SALES_TYPE_MANUAL:		nSalesPos += 2;		break;
			case SALES_TYPE_IMPORT:		nSalesPos += 4;		break;
			}
				
			pSalesArray -> SetAt ( nSalesPos, pSalesArray -> GetAt ( nSalesPos ) + dQty );
			pSalesArray -> SetAt ( nSalesPos + 1, pSalesArray -> GetAt ( nSalesPos + 1 ) + dValue );	
		}
		nProgress += fileAudit.GetLength();
	}
}

/**********************************************************************/

bool CStockMonitor::ChangeStockCodes( CStockFilterArray* pFilterArray )
{
	//MAKE SURE THAT ALL STOCK LEVEL ARRAYS CONTAIN IDENTICAL STOCK
	//CODES TO THE MAIN STOCK DATABASE

	int nSpCount = m_arrayStockMonitorInfo.GetSize();
	int nProgressTarget = GetSize() * nSpCount * 5;
	int nProgressSoFar = 0;

	StatusProgress.Lock( TRUE, "Synchronising stock level files" );
	for ( int n = 0; n < m_arrayStockMonitorInfo.GetSize(); n++ )
	{
		SynchroniseStockLevels( n, nProgressSoFar, nProgressTarget );
		nProgressSoFar += ( GetSize() * 5 );
	}
	StatusProgress.Unlock();

	nProgressSoFar = 0;
	StatusProgress.Lock( TRUE, "Synchronising transfer files" );
	for ( int n = 0; n < m_arrayStockMonitorInfo.GetSize(); n++ )
	{
		SynchroniseTransferDetail( n, nProgressSoFar, nProgressTarget );
		nProgressSoFar += ( GetSize() * 5 );
	}
	StatusProgress.Unlock();

	CStockCSVArray tempArray;

	bool bFail = FALSE;
	int nSize = GetSize();
	StatusProgress.Lock( TRUE, "Changing stock codes");
	for ( int nStockIdx = 0; nStockIdx < nSize; nStockIdx++ )
	{
		StatusProgress.SetPos ( nStockIdx, nSize, 0, 50 );
		CStockCSVRecord StockRecord;
		GetAt ( nStockIdx, StockRecord );
		StockRecord.SetStockCode ( pFilterArray -> GetNewStockCode ( nStockIdx ) );
		tempArray.Add ( StockRecord );
	}

	m_StockDatabase.RemoveAt ( 0, m_StockDatabase.GetSize() );
		
	for ( int nStockIdx = 0; nStockIdx < nSize; nStockIdx++ )
	{
		StatusProgress.SetPos ( nStockIdx, nSize, 50, 50 );
		CStockCSVRecord StockRecord;
		tempArray.GetAt ( nStockIdx, StockRecord );
		
		int nNewIdx;
		m_StockDatabase.FindStockItemByCode ( StockRecord.GetStockCode(), nNewIdx );
		m_StockDatabase.InsertAt ( nNewIdx, StockRecord );
	}

	int nRecCount = m_StockDatabase.GetSize();
	nSize = nSpCount * nRecCount * 4;
	int nProgress = 0;

	for ( int nIndex = 0; nIndex < m_arrayStockMonitorInfo.GetSize(); nIndex++ )
	{
		{
			CStockLevelsCSVArray* pOldArray = m_arrayStockMonitorInfo[nIndex].m_pStockLevels;
			CStockLevelsCSVArray* pNewArray = new CStockLevelsCSVArray ( pOldArray -> GetSpNo() );

			for ( int nStockIdx = 0; nStockIdx < pOldArray -> GetSize(); nStockIdx++ )
			{
				StatusProgress.SetPos ( nProgress++, nSize );
			
				CStockLevelsCSVRecord StockLevels;
				pOldArray -> GetAt ( nStockIdx, StockLevels );
				StockLevels.SetStockCode ( pFilterArray -> GetNewStockCode ( nStockIdx ) );

				int nNewIdx;
				pNewArray -> FindStockItemByCode ( -1, StockLevels.GetStockCode(), nNewIdx );
				pNewArray -> InsertAt ( nNewIdx, StockLevels );
			}

			//WE CANNOT DELETE THE OLD ARRAY, AS IT MUST BE LOCKED FOR WRITING
			pOldArray -> RemoveAll();

			for ( int nStockIdx = 0; nStockIdx < pNewArray -> GetSize(); nStockIdx++ )
			{
				StatusProgress.SetPos( nProgress++, nSize );

				CStockLevelsCSVRecord StockLevels;
				pNewArray -> GetAt ( nStockIdx, StockLevels );
				pOldArray -> Add( StockLevels );
			}

			pNewArray -> RemoveAll();
			delete pNewArray;	
		}

		{
			CTransferDetailCSVArray* pOldArray = m_arrayStockMonitorInfo[nIndex].m_pTransferDetail;
			CTransferDetailCSVArray* pNewArray = new CTransferDetailCSVArray ( pOldArray -> GetSpNo() );

			for ( int nStockIdx = 0; nStockIdx < pOldArray -> GetSize(); nStockIdx++ )
			{
				StatusProgress.SetPos ( nProgress++, nSize );
			
				CTransferDetailCSVRecord TransferDetail;
				pOldArray -> GetAt ( nStockIdx, TransferDetail );
				TransferDetail.SetStockCode ( pFilterArray -> GetNewStockCode ( nStockIdx ) );

				int nNewIdx;
				pNewArray -> FindStockItemByCode ( -1, TransferDetail.GetStockCode(), nNewIdx );
				pNewArray -> InsertAt ( nNewIdx, TransferDetail );
			}

			//WE CANNOT DELETE THE OLD ARRAY, AS IT MUST BE LOCKED FOR WRITING
			pOldArray -> RemoveAll();

			for ( int nStockIdx = 0; nStockIdx < pNewArray -> GetSize(); nStockIdx++ )
			{
				StatusProgress.SetPos( nProgress++, nSize );

				CTransferDetailCSVRecord TransferDetail;
				pNewArray -> GetAt ( nStockIdx, TransferDetail );
				
				if ( TransferDetail.GetSpCount() > 0 )
					pOldArray -> Add( TransferDetail );
			}

			pNewArray -> RemoveAll();
			delete pNewArray;	
		}
	}

	StatusProgress.Unlock();
	return TRUE;
}

/**********************************************************************/

void CStockMonitor::GetReorderInfo ( int nDbIdx, int nStockIdx, CLocationSelectorMiniLookup& LocSelMiniLookup, CStringArray& arrayReorderInfo, bool bFullApparent, bool bAllowOrder, bool bRequireOrder )
{
	arrayReorderInfo.RemoveAll();

	if ( nDbIdx < 0 || nDbIdx >= dbDatabase.GetSize() )
		return;

	int nDbNo = dbDatabase.GetDbNo ( nDbIdx );

	if ( nStockIdx < 0 || nStockIdx >= GetSize() )
		return;

	CStockCSVRecord StockRecord;
	GetAt ( nStockIdx, StockRecord );
	CString strStockCode = StockRecord.GetStockCode();

	int nDUItems = 1;
	double dDUItemSize = 1.0;

	if ( StockRecord.GetSupplierCount() >= 1 )
	{
		dDUItemSize = StockRecord.GetDUItemSize(0);
		nDUItems = StockRecord.GetDUItems(0);
		if ( dDUItemSize <= 0.0 ) dDUItemSize = 1.0;
		if ( nDUItems <= 0 ) nDUItems = 1;
	}

	double dTotalApparent = 0.0;
	double dTotalBelow = 0.0;
	double dTotalSURequired = 0.0;
	double dTotalSUOnOrder = 0.0;
	double dTotalSUShort = 0.0;
	int nTotalDUItemRequired = 0;		
	int nTotalDURequired = 0;
	int nTotalDUItemSurplus = 0;
	
	int nStartSpIdx, nEndSpIdx;
	dbStockpoint.GetSpIdxRange( nDbIdx, nStartSpIdx, nEndSpIdx );

	for ( int nSpIdx = nStartSpIdx; nSpIdx <= nEndSpIdx; nSpIdx++ )
	{
		//FIND OUT IF WE ARE INTERESTED IN THIS STOCKPOINT
		bool bMatch = FALSE;
		switch ( LocSelMiniLookup.GetConType() )
		{
		case NODE_STOCKPOINT:	bMatch = ( dbStockpoint.GetSpNo( nSpIdx ) == LocSelMiniLookup.GetEntityNo() );		break;
		case NODE_SITE:			bMatch = ( dbStockpoint.GetSiteNo( nSpIdx ) == LocSelMiniLookup.GetEntityNo() );	break;
		case NODE_DATABASE:		bMatch = TRUE;		break;
		default:				bMatch = FALSE;		break;
		}

		//IGNORE NON MATCHING STOCKPOINTS
		if ( bMatch == FALSE )
			continue;

		//GET ORDER LEVELS FOR THIS STOCKPOINT
		CStockLevelsCSVRecord StockLevels;
		GetStockpointTotals( nStockIdx, StockRecord, dbStockpoint.GetSpNo( nSpIdx ), StockLevels );

		if ( StockLevels.GetActiveFlag() == FALSE )
			continue;
		
		double dApparent = StockLevels.GetApparentQty();
		double dMinimum = StockLevels.GetMinimumLevelQty();
		double dReorder = StockLevels.GetReorderLevelQty();
		double dSUOnOrder = StockLevels.GetQtyOnOrder();
		double dBelow = 0.0;
		double dSURequired = 0.0;
		double dSUShort = 0.0;
		int nDUItemRequired = 0;
		int nDURequired = 0;
		int nDUItemSurplus = 0;

		//CONSOLIDATE APPARENT STOCK NOW FOR STOCK SUPPLIER REPORT
		if ( TRUE == bFullApparent )
			dTotalApparent += dApparent;

		bool bGotOrder = FALSE;

		if ( TRUE == bAllowOrder )
		{
			dBelow = dMinimum - dApparent;

			//NOTHING ELSE TO DO IF WE ARE NOT BELOW MINIMUM LEVEL
			if ( dBelow > 0 )
			{
				//WORK OUT THE LEVEL OF STOCK WE NEED TO REACH
				double dSUTarget = dReorder;
				if ( StockOptions.GetOrderGenerationMethod() == ORDER_TYPE_FIXED )
					dSUTarget += dApparent;

				//NOTHING ELSE TO DO IF EXISTING ORDER WILL SUFFICE
				if ( dApparent + dSUOnOrder < dSUTarget )
				{
					bGotOrder = TRUE;

					//CONSOLIDATE APPARENT STOCK NOW FOR STOCK REORDER REPORT
					if ( bFullApparent == FALSE )
						dTotalApparent += dApparent;

					dSURequired = dSUTarget - dApparent;
					dSUShort = dSURequired - dSUOnOrder;

					dTotalSURequired += dSURequired;
					dTotalSUShort += dSUShort;
					dTotalBelow += dBelow;
					dTotalSUOnOrder += dSUOnOrder;

					//WORK OUT DU ITEM REQ AND SURPLUS UNITS
					nDUItemRequired = int ( dSUShort / dDUItemSize );
					if ( ( nDUItemRequired * dDUItemSize ) < dSUShort )
						nDUItemRequired++;

					nTotalDUItemRequired += nDUItemRequired;
					
					nDURequired = ( nDUItemRequired / nDUItems );
					if ( nDURequired * nDUItems < nDUItemRequired )
						nDURequired++;

					nDUItemSurplus = ( nDURequired * nDUItems ) - nDUItemRequired;
						
					if ( StockOptions.GetOrderSeparationType() != ORDER_DUSEPARATE_NONE )
					{
						nTotalDURequired += nDURequired;
						nTotalDUItemSurplus += nDUItemSurplus;
					}
				}
			}
		}

		if ( TRUE == bRequireOrder )
			if ( FALSE == bGotOrder )
				continue;

		CCSVStockAudit csv;
		csv.Add ( nSpIdx );							//STOCK POINT INDEX
		csv.AddStockAuditQty ( dApparent );			//APPARENT STOCK
		csv.AddStockAuditQty ( dBelow );			//STOCK UNITS BELOW MIN
		csv.AddStockAuditQty ( dSURequired );		//STOCK UNITS REQUIRED
		csv.AddStockAuditQty ( dSUOnOrder );		//STOCK UNITS ON ORDER
		csv.AddStockAuditQty ( dSUShort );			//STOCK UNITS SHORT
		csv.Add ( dDUItemSize, 3 );					//STOCK UNITS PER DELIVERY ITEM
		csv.Add ( nDUItemRequired );				//DELIVERY ITEMS REQUIRED
		csv.Add ( nDUItems );						//DELIVERY ITEMS PER DELIVERY UNIT
		csv.Add ( nDURequired );					//DELIVERY UNITS REQUIRED
		csv.Add ( nDUItemSurplus );					//DELIVERY ITEMS SURPLUS
		arrayReorderInfo.Add ( csv.GetLine() );
	}

	if ( ( TRUE == bAllowOrder ) && ( StockOptions.GetOrderSeparationType() == ORDER_DUSEPARATE_NONE ) )
	{
		nTotalDURequired = ( nTotalDUItemRequired / nDUItems );
		if ( ( nTotalDURequired * nDUItems ) < nTotalDUItemRequired )
			nTotalDURequired++;

		nTotalDUItemSurplus = ( nTotalDURequired * nDUItems ) - nTotalDUItemRequired;
	}

	CCSVStockAudit csv;
	csv.Add ( -1 );
	csv.AddStockAuditQty ( dTotalApparent );
	csv.AddStockAuditQty ( dTotalBelow );
	csv.AddStockAuditQty ( dTotalSURequired );
	csv.AddStockAuditQty ( dTotalSUOnOrder );
	csv.AddStockAuditQty ( dTotalSUShort );
	csv.Add ( dDUItemSize, 3 );
	csv.Add ( nTotalDUItemRequired );
	csv.Add ( nDUItems );
	csv.Add ( nTotalDURequired );
	csv.Add ( nTotalDUItemSurplus );
		
	arrayReorderInfo.Add ( csv.GetLine() );
}

/**********************************************************************/

bool CStockMonitor::SynchroniseStockLevels( int nIndex, int nProgressSoFar, int nProgressTarget, bool bProgress )
{
	if ( nIndex < 0 || nIndex >= m_arrayStockMonitorInfo.GetSize() )
		return FALSE;
	
	CStockLevelsCSVArray* pOldArray = m_arrayStockMonitorInfo[ nIndex ].m_pStockLevels;
	CStockLevelsCSVArray* pNewArray = new CStockLevelsCSVArray ( pOldArray -> GetSpNo() );

	//PLACE EXISTING STOCK LEVELS INTO A TEMPORARY ARRAY
	//MAKING SURE THEY ARE SORTED WITH NO DUPLICATES
	
	int nSize = pOldArray -> GetSize();
	for ( int nStockIdx = 0; nStockIdx < nSize; nStockIdx++ )
	{
		if ( bProgress == TRUE )
			StatusProgress.SetPos ( nProgressSoFar + nStockIdx, nProgressTarget );
			
		CStockLevelsCSVRecord StockLevels;
		pOldArray -> GetAt ( nStockIdx, StockLevels );
			
		CString strStockCode = StockLevels.GetStockCode();
			
		int nNewIdx; 
		if ( pNewArray -> FindStockItemByCode ( -1, strStockCode, nNewIdx ) == FALSE )
			pNewArray -> InsertAt ( nNewIdx, StockLevels );
	}

	nProgressSoFar += GetSize();
	
	//REBUILD STOCK LEVEL ARRAY TO EXACTLY MATCH DATABASE
	pOldArray -> RemoveAll();
	for ( int nStockIdx = 0; nStockIdx < GetSize(); nStockIdx++ )
	{
		if ( bProgress == TRUE )
			StatusProgress.SetPos ( nProgressSoFar, nProgressTarget );
		
		nProgressSoFar += 4;
	
		CString strStockCode = GetStockCode ( nStockIdx );

		int nNewIdx;
		if ( pNewArray -> FindStockItemByCode ( -1, strStockCode, nNewIdx ) == FALSE )
		{
			CStockLevelsCSVRecord StockLevels;
			StockLevels.SetStockCode ( strStockCode );
			pOldArray -> Add ( StockLevels );
		}
		else
		{
			CStockLevelsCSVRecord StockLevels;
			pNewArray -> GetAt ( nNewIdx, StockLevels );
			pOldArray -> Add ( StockLevels );
		}
	}
	
	pNewArray -> RemoveAll();
	delete pNewArray;
	pOldArray ->SetSyncFlag ( TRUE );

	return TRUE;
}

/**********************************************************************/

bool CStockMonitor::SynchroniseTransferDetail( int nIndex, int nProgressSoFar, int nProgressTarget, bool bProgress )
{
	if ( nIndex < 0 || nIndex >= m_arrayStockMonitorInfo.GetSize() )
		return FALSE;
	
	CTransferDetailCSVArray* pOldArray = m_arrayStockMonitorInfo[ nIndex ].m_pTransferDetail;
	CTransferDetailCSVArray* pNewArray = new CTransferDetailCSVArray ( pOldArray -> GetSpNo() );

	//PLACE EXISTING STOCK LEVELS INTO A TEMPORARY ARRAY
	//MAKING SURE THEY ARE SORTED WITH NO DUPLICATES
	
	int nSize = pOldArray -> GetSize();
	for ( int nStockIdx = 0; nStockIdx < nSize; nStockIdx++ )
	{
		if ( bProgress == TRUE )
			StatusProgress.SetPos ( nProgressSoFar + nStockIdx, nProgressTarget );
			
		CTransferDetailCSVRecord TransferDetail;
		pOldArray -> GetAt ( nStockIdx, TransferDetail );
			
		CString strStockCode = TransferDetail.GetStockCode();
			
		int nNewIdx; 
		if ( pNewArray -> FindStockItemByCode ( -1, strStockCode, nNewIdx ) == FALSE )
			pNewArray -> InsertAt ( nNewIdx, TransferDetail );
	}

	nProgressSoFar += GetSize();
	
	//REBUILD STOCK LEVEL ARRAY TO EXACTLY MATCH DATABASE
	pOldArray -> RemoveAll();
	for ( int nStockIdx = 0; nStockIdx < GetSize(); nStockIdx++ )
	{
		if ( bProgress == TRUE )
			StatusProgress.SetPos ( nProgressSoFar, nProgressTarget );
		
		nProgressSoFar += 4;
	
		CString strStockCode = GetStockCode ( nStockIdx );

		int nNewIdx;
		if ( pNewArray -> FindStockItemByCode ( -1, strStockCode, nNewIdx ) == FALSE )
		{
			CTransferDetailCSVRecord TransferDetail;
			TransferDetail.SetStockCode ( strStockCode );
			pOldArray -> Add ( TransferDetail );
		}
		else
		{
			CTransferDetailCSVRecord TransferDetail;
			pNewArray -> GetAt ( nNewIdx, TransferDetail );
			pOldArray -> Add ( TransferDetail );
		}
	}
	
	pNewArray -> RemoveAll();
	delete pNewArray;
	pOldArray ->SetSyncFlag ( TRUE );
	
	return TRUE;
}

/**********************************************************************/

void CStockMonitor::ClearOrders( int nMasterIdx )
{
	if ( nMasterIdx < 0 || nMasterIdx > GetSize() )
		return;

	CStockCSVRecord StockRecord;
	GetAt ( nMasterIdx, StockRecord );
	CString strStockCode = StockRecord.GetStockCode();

	for ( int nIndex = 0; nIndex < m_arrayStockMonitorInfo.GetSize(); nIndex++ )
	{
		CStockLevelsCSVArray* pArray = m_arrayStockMonitorInfo[nIndex].m_pStockLevels;

		int nStockIdx;
		if ( pArray -> FindStockItemByCode ( nMasterIdx, strStockCode, nStockIdx ) )
		{
			CStockLevelsCSVRecord StockLevels;
			pArray -> GetAt ( nStockIdx, StockLevels );
			StockLevels.SetQtyOnOrder ( 0 );
			pArray -> SetAt ( nStockIdx, StockLevels );
		}
	}
}

/**********************************************************************/

int CStockMonitor::ImportLine ( int nImportMethod, CSSImportFile& import, const char* szImportLine, int nSupplierCount )
{
	CPtrArray CustomFieldPtrArray;
	DataManager.GetCustomFieldPtrArray( CustomFieldPtrArray );

	return m_StockDatabase.ImportLine( nImportMethod, import, szImportLine, nSupplierCount, DataManager.Supplier, CustomFieldPtrArray, StockOptions.GetStockMaxSuppliersPerItem() );
}

/**********************************************************************/
	
int CStockMonitor::PurgeSupplier( int nSuppNo )
{
	return m_StockDatabase.PurgeSupplier( nSuppNo, StockOptions.GetStockMaxSuppliersPerItem() );
}

/**********************************************************************/

const char* CStockMonitor::GetStockLevelString( CStockCSVRecord& StockRecord, double dLevel, int nDps )
{
	m_strStockLevel = "0/0";
	if ( nDps < 0 ) nDps = SysInfo.GetDPQty();
		
	if ( GetSubUnits( StockRecord ) > 1 )
	{
		__int64 nStock;
		__int64 nSub;
			
		if ( dLevel >= 0.0 )
		{
			StockRecord.ConvertStockLevelToStockSub ( dLevel, nStock, nSub );
			m_strStockLevel.Format ( "%I64d/%I64d", nStock, nSub );
		}
		else
		{
			dLevel = -dLevel;
			StockRecord.ConvertStockLevelToStockSub ( dLevel, nStock, nSub );
			m_strStockLevel.Format ( "-%I64d/%I64d", nStock, nSub );
		}
	}
	else
		m_strStockLevel.Format ( "%.*f", nDps, dLevel );
	
	return m_strStockLevel;
}

/**********************************************************************/

const char* CStockMonitor::GetStockLevelStringForPluEdit( CStockCSVRecord& StockRecord, double dLevel, int nDps )
{
	if ( nDps < 0 ) nDps = SysInfo.GetDPQty();
	m_strStockLevel.Format ( "%.*f", nDps, dLevel );

	if ( StockOptions.GetStockSubUnitsFlag() == TRUE )
	{
		if ( StockRecord.GetSubUnits(TRUE) > 1 )
		{	
			CString strMinus = "";
			if ( dLevel < 0.0 )
			{ 
				dLevel = - dLevel;
				strMinus = "-";
			}

			__int64 nStock;
			double dRemainder;
			StockRecord.ConvertStockLevelToStockSubForPluEdit ( dLevel, nStock, dRemainder );

			CCSV csvOut( ',', '"', FALSE, FALSE, TRUE);
			csvOut.Add ( dRemainder, 3 );

			CString strStockLevel;
			strStockLevel.Format ( "%I64d / ", nStock );
			m_strStockLevel = strMinus + strStockLevel + csvOut.GetLine();
		}
		else if ( CPriceHelpers::CompareDoubles ( StockRecord.GetSUWeight(TRUE), 0.0, 3 ) != 0 )
			m_strStockLevel.Format ( "%.*f", nDps, dLevel * StockRecord.GetQtyToWeightFactor() );
	}

	return m_strStockLevel;
}

/**********************************************************************/

const char* CStockMonitor::GetStockLevelStringForPluTab( CStockCSVRecord& StockRecord, double dLevel, int nDps )
{
	CString strStockUnit = StockRecord.GetStockUnitName();
	::TrimSpaces( strStockUnit, FALSE );
	if ( strStockUnit == "" ) strStockUnit = "Stk Unit";

	CString strSubUnit = StockRecord.GetSubUnitName();
	::TrimSpaces( strSubUnit, FALSE );
	if ( strSubUnit == "" ) strSubUnit = "Sub";

	//BOG STANDARD DECIMAL STOCK QTY
	if ( nDps < 0 ) nDps = SysInfo.GetDPQty();
	m_strStockLevel.Format ( "%.*f %s", 
		nDps, 
		dLevel,
		(const char*) strStockUnit );

	if ( StockOptions.GetStockSubUnitsFlag() == TRUE )
	{
		//SUB UNITS
		if ( StockRecord.GetSubUnits(TRUE) > 1 )
		{
			CString strMinus = "";
			if ( dLevel < 0.0 )
			{ 
				dLevel = - dLevel;
				strMinus = "-";
			}

			__int64 nStock;
			double dRemainder;
			StockRecord.ConvertStockLevelToStockSubForPluEdit ( dLevel, nStock, dRemainder );
			
			//ONLY DO SUB UNITS IF SELLING LESS THAN A WHOLE UNIT
			if ( nStock < 1 )
			{
				//STRIP TRAILING ZEROS FROM SUB UNIT COUNT
				CCSV csvOut( ',', '"', FALSE, FALSE, TRUE);
				csvOut.Add ( dRemainder, 3 );

				m_strStockLevel.Format ( "%s%s %s (%d per %s)",
					(const char*) strMinus,
					csvOut.GetLine(),
					(const char*) strSubUnit,
					StockRecord.GetSubUnits(TRUE),
					(const char*) strStockUnit );
			}
		}
		else	if ( CPriceHelpers::CompareDoubles ( StockRecord.GetSUWeight(TRUE), 0.0, 3 ) != 0 )
		{
			CString strUnitName = "";
			switch ( StockRecord.GetSaleWeightType() )
			{
			case WEIGHTTYPE_GALLON:		strUnitName = "Gallon";		break;
			case WEIGHTTYPE_PINT:		strUnitName = "Pint";		break;
			case WEIGHTTYPE_FLOUNCE:	strUnitName = "Fluid Ounce";	break;
			case WEIGHTTYPE_LITRE:		strUnitName = "Litre";		break;
			case WEIGHTTYPE_CLITRE:		strUnitName = "Centilitre";	break;
			case WEIGHTTYPE_MLITRE:		strUnitName = "Millilitre";	break;
			case WEIGHTTYPE_KILO:		strUnitName = "Kilogram";	break;
			case WEIGHTTYPE_GRAM:		strUnitName = "Gram";		break;
			default:					strUnitName = "Unknown";		break;
			}

			m_strStockLevel.Format ( "%.*f %s", 
				nDps, 
				dLevel * StockRecord.GetQtyToWeightFactor(),
				(const char*) strUnitName );
		}
	}

	return m_strStockLevel;
}

/**********************************************************************/

const char* CStockMonitor::GetStockSubUnitName( CStockCSVRecord& StockRecord, bool bNoBlank )
{
	CString strStockUnit = StockRecord.GetStockUnitName();
	if ( TRUE == bNoBlank )
	{
		::TrimSpaces( strStockUnit, FALSE );
		if ( strStockUnit == "" )
			strStockUnit = "Stock Unit";
	}
	
	CString strSubUnit = "";
	if ( GetSubUnits( StockRecord ) > 1 )
	{
		strSubUnit = StockRecord.GetSubUnitName();

		if ( TRUE == bNoBlank )
		{
			::TrimSpaces( strSubUnit, FALSE );
			if ( strSubUnit == "" )
				strSubUnit = "Sub Unit";
		}
	}

	m_strStockSubUnitName = strStockUnit;

	if ( ( strStockUnit != "" ) && ( strSubUnit != "" ) )
		m_strStockSubUnitName += "/";

	m_strStockSubUnitName += strSubUnit;
	return m_strStockSubUnitName;
}

/**********************************************************************/

const char* CStockMonitor::GetStockSubUnitNameForPluEdit( CStockCSVRecord& StockRecord )
{
	m_strStockSubUnitName = StockRecord.GetStockUnitName();
	::TrimSpaces( m_strStockSubUnitName, FALSE );
	
	if ( m_strStockSubUnitName == "" ) 
		m_strStockSubUnitName = "Stock";
	
	if ( StockOptions.GetStockSubUnitsFlag() )
	{
		if ( StockRecord.GetSubUnits(TRUE) > 1 )
		{
			CString strSubUnit = StockRecord.GetSubUnitName();
			::TrimSpaces( strSubUnit, FALSE );
			if ( strSubUnit == "" ) strSubUnit = "Sub";

			CString strYield;
			strYield.Format( " (%d)", StockRecord.GetSubUnits(TRUE) );

			m_strStockSubUnitName += " / ";
			m_strStockSubUnitName += strSubUnit;
			m_strStockSubUnitName += strYield;
		}
		else if ( CPriceHelpers::CompareDoubles ( StockRecord.GetSUWeight(TRUE), 0.0, 3 ) != 0 )
		{
			switch ( StockRecord.GetSaleWeightType() )
			{
			case WEIGHTTYPE_GALLON:		m_strStockSubUnitName = "Gallon";		break;
			case WEIGHTTYPE_PINT:		m_strStockSubUnitName = "Pint";			break;
			case WEIGHTTYPE_FLOUNCE:	m_strStockSubUnitName = "Fluid Ounce";	break;
			case WEIGHTTYPE_LITRE:		m_strStockSubUnitName = "Litre";		break;
			case WEIGHTTYPE_CLITRE:		m_strStockSubUnitName = "Centilitre";	break;
			case WEIGHTTYPE_MLITRE:		m_strStockSubUnitName = "Millilitre";	break;
			case WEIGHTTYPE_KILO:		m_strStockSubUnitName = "Kilogram";		break;
			case WEIGHTTYPE_GRAM:		m_strStockSubUnitName = "Gram";			break;
			default:					m_strStockSubUnitName = "Unknown";		break;
			}
		}
	}

	return m_strStockSubUnitName;
}
/**********************************************************************/

int CStockMonitor::GetSubUnits( CStockCSVRecord& StockRecord )
{
	return StockRecord.GetSubUnits( StockOptions.GetStockSubUnitsFlag() );
}

/**********************************************************************/

double CStockMonitor::CalculateRetailValue ( CStockCSVRecord& StockRecord, double dStockQty )
{
	double dRetailValue = 0.0;

	int nStockPluIdx;
	CString strStockCode = StockRecord.GetStockCode();
	if ( DataManager.StockPlu.FindStockCode ( strStockCode, nStockPluIdx ) == TRUE )
	{
		if ( DataManager.StockPlu.GetPluNoCount ( nStockPluIdx ) > 0 )
		{
			__int64 nPluNo = DataManager.StockPlu.GetPluNo ( nStockPluIdx, 0 );
	
			int nPluIdx;
			if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == TRUE )
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt ( nPluIdx, PluRecord );

				double dStockQtyPerPlu;
				DataManager.Plu.GetPluStockQty ( nPluNo, dStockQtyPerPlu );
				
				double dPluPrice = PluRecord.GetPrice(0);

				if ( dStockQtyPerPlu != 0.0 )
					dRetailValue = ( dPluPrice * dStockQty ) / dStockQtyPerPlu;
			}
		}
		else if ( ( DealerFlags.GetStockReportUseRecipeCostLinkFlag() == TRUE ) && ( DataManager.StockPlu.GetRecipePluNoCount( nStockPluIdx ) > 0 ) )
		{
			__int64 nPluNo = DataManager.StockPlu.GetRecipePluNo ( nStockPluIdx, 0 );
	
			int nPluIdx;
			if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == TRUE )
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt ( nPluIdx, PluRecord );

				CExplodePluDatabase ExplodePlu;
				
				CStringArray StockDetails;
				if ( ExplodePlu.ExplodePlu( nPluNo, StockDetails, AUDIT_SALES, 1.00, PluRecord.GetPrice(0) ) == PLU_EXCEPTION_NONE )
				{
					int nRecipePos = -1;
	
					for ( int nIndex = 0; nIndex < StockDetails.GetSize(); nIndex++ )
					{
						CCSVExplodedPlu csv ( StockDetails.GetAt ( nIndex ) );
					
						if ( strStockCode == csv.GetStockCode() )
						{
							nRecipePos = nIndex;
							break;
						}
					}
					
					if ( nRecipePos != -1 )
					{
						CCSVExplodedPlu csvSale ( StockDetails.GetAt ( nRecipePos ) );
						dRetailValue = ( csvSale.GetStockValue() * dStockQty ) / csvSale.GetStockQty(); 
					}
				}
			}
		}
	}
	return dRetailValue;
}

/**********************************************************************/

double CStockMonitor::CalculateTaxContentFromValue( CStockCSVRecord& StockRecord, double dValue )
{
	double dTaxValue = 0.0;

	int nStockPluIdx;
	if ( DataManager.StockPlu.FindStockCode ( StockRecord.GetStockCode(), nStockPluIdx ) == TRUE )
	{
		__int64 nPluNo = 0;
		if ( DataManager.StockPlu.GetPluNoCount ( nStockPluIdx ) > 0 )
			nPluNo = DataManager.StockPlu.GetPluNo ( nStockPluIdx, 0 );
		else if ( ( DealerFlags.GetStockReportUseRecipeCostLinkFlag() == TRUE ) && ( DataManager.StockPlu.GetRecipePluNoCount( nStockPluIdx ) > 0 ) )
			nPluNo = DataManager.StockPlu.GetRecipePluNo ( nStockPluIdx, 0 );
	
		if ( nPluNo != 0 )
		{
			int nPluIdx;
			if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == TRUE )
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt ( nPluIdx, PluRecord );

				CTaxRateInfo TaxRateInfo;
				DataManager.TaxRates.GetTaxRateInfo( PluRecord.GetBaseTaxBandInt(), TaxRateInfo );
				dTaxValue = dValue - ReportHelpers.CalcNonTax ( dValue, TaxRateInfo.m_dTaxRate );
			}
		}
	}
	return dTaxValue;
}

/**********************************************************************/

double CStockMonitor::CalculateHistoricalTaxContentFromValue( CStockCSVRecord& StockRecord, double dValue, int nTaxDateCode )
{
	double dTaxValue = 0.0;

	int nStockPluIdx;
	if ( DataManager.StockPlu.FindStockCode ( StockRecord.GetStockCode(), nStockPluIdx ) == TRUE )
	{
		__int64 nPluNo = 0;
		if ( DataManager.StockPlu.GetPluNoCount ( nStockPluIdx ) > 0 )
			nPluNo = DataManager.StockPlu.GetPluNo ( nStockPluIdx, 0 );
		else if ( ( DealerFlags.GetStockReportUseRecipeCostLinkFlag() == TRUE ) && ( DataManager.StockPlu.GetRecipePluNoCount( nStockPluIdx ) > 0 ) )
			nPluNo = DataManager.StockPlu.GetRecipePluNo ( nStockPluIdx, 0 );

		if ( nPluNo != 0 )
		{
			int nPluIdx;
			if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == TRUE )
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt ( nPluIdx, PluRecord );
				double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate( nTaxDateCode, PluRecord.GetBaseTaxBandInt() );
				dTaxValue = dValue - ReportHelpers.CalcNonTax ( dValue, dTaxRate );
			}
		}
	}
	return dTaxValue;
}

/**********************************************************************/

double CStockMonitor::GetUnitWastageCost ( CStockCSVRecord& StockRecord, int nIndex )
{
	if ( StockOptions.GetStockSubUnitsFlag() == FALSE )
		return 0.0;

	double dWastePercent = StockRecord.GetWastePercent();
	if ( dWastePercent == 0.0 )
		return 0.0;

	double dSUWastageCost = 0.0;

	if ( nIndex >= 0 && nIndex < MAX_ITEM_SUPPLIERS )
	{	
		double dDUCost = StockRecord.GetCost( nIndex );
		double dDUYield = StockRecord.GetYield ( nIndex );
		double dSUCost = 0.0;

		if ( dDUYield != 0 ) 
			dSUCost = ( dDUCost / dDUYield );
		else
			dSUCost = 0.0;

		double dNonWastePercent = 100 - dWastePercent;

		if ( dNonWastePercent != 0.0 )
			dSUWastageCost = ( dSUCost * dWastePercent ) / dNonWastePercent;
	}
	return dSUWastageCost;
}

/**********************************************************************/

void CStockMonitor::UpdateTransferDetail( int nSpIdxSource, int nSpIdxDest, int nStockIdx, CStockCSVRecord& StockRecord, double dQty )
{
	int nSpNoSource = dbStockpoint.GetSpNo( nSpIdxSource );
	int nSpNoDest = dbStockpoint.GetSpNo( nSpIdxDest );

	CStockLevelsCSVRecord StockLevelsSource, StockLevelsDest;
	GetStockpointTotals( nStockIdx, StockRecord, nSpNoSource, StockLevelsSource );
	GetStockpointTotals( nStockIdx, StockRecord, nSpNoDest, StockLevelsDest );
	
	double dFromQty = StockLevelsSource.GetTransferOutQty() + dQty;
	double dToQty = StockLevelsDest.GetTransferInQty() + dQty;
	
	StockLevelsSource.SetTransferOutQty ( dFromQty );
	StockLevelsDest.SetTransferInQty ( dToQty );

	SetStockpointTotals( nStockIdx, StockRecord, nSpNoSource, StockLevelsSource );
	SetStockpointTotals( nStockIdx, StockRecord, nSpNoDest, StockLevelsDest );

	{
		CTransferDetailCSVRecord TransferDetail;
		GetTransferTotals ( nStockIdx, StockRecord, nSpNoSource, TransferDetail );
			
		CTransferDetailInfo info;
		info.m_nSpNo = nSpNoDest;
		TransferDetail.GetTransferDetailInfo( info );
		info.m_dTransferOutQty += dQty;
		TransferDetail.SetTransferDetailInfo( info );

		SetTransferTotals ( nStockIdx, StockRecord, nSpNoSource, TransferDetail );
	}

	{
		CTransferDetailCSVRecord TransferDetail;
		GetTransferTotals ( nStockIdx, StockRecord, nSpNoDest, TransferDetail );
		
		CTransferDetailInfo info;
		info.m_nSpNo = nSpNoSource;
		TransferDetail.GetTransferDetailInfo( info );
		info.m_dTransferInQty += dQty;
		TransferDetail.SetTransferDetailInfo( info );

		SetTransferTotals ( nStockIdx, StockRecord, nSpNoDest, TransferDetail );
	}
}	

/**********************************************************************/

double CStockMonitor::CalculateTaxOnCost( CStockCSVRecord& StockRecord, double dValue )
{
	CTaxRateInfo infoTaxRate;
	int nCategory = StockRecord.GetCategory();
	int nTaxBand = DataManager.Category.GetTaxBand( nCategory );
	DataManager.TaxRates.GetTaxRateInfo( nTaxBand, infoTaxRate );
	return ( dValue * infoTaxRate.m_dTaxRate ) / 100.0;
}

/**********************************************************************/

int CStockMonitor::CheckStockLevelsForImport( int nStockIdx, CStockCSVRecord& StockRecord, CStockLevelsCSVRecord& StockLevels, int nSpNo, int nStocktakeNo )
{
	int nResult = STOCKLEVEL_IMPORT_ACCEPT;

	GetAt ( nStockIdx, StockRecord );
	GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );

	if ( nStocktakeNo != -1 )
	{
		if ( StockLevels.GetStocktakeNo() != nStocktakeNo )
			nResult = STOCKLEVEL_IMPORT_REJECT_STOCKTAKE;
	}

	//DON'T CHECK THE ACTIVE FLAG IF WE ARE WORKING WITH ITEMS IN A STOCKTAKE
	if ( ( STOCKLEVEL_IMPORT_ACCEPT == nResult ) && ( nStocktakeNo <= 0 ) )
	{
		if ( StockLevels.GetActiveFlag() == FALSE )
			nResult = STOCKLEVEL_IMPORT_REJECT_INACTIVE;
	}

	return nResult;
}

/**********************************************************************/

int CStockMonitor::CheckStockLevelsForTransfer( int nStockIdx, CStockCSVRecord& StockRecord, int nSpNo1, int nSpNo2 )
{
	int nResult = STOCKLEVEL_IMPORT_ACCEPT;

	GetAt ( nStockIdx, StockRecord );

	CStockLevelsCSVRecord StockLevels;
	GetStockpointTotals( nStockIdx, StockRecord, nSpNo1, StockLevels );

	if ( StockLevels.GetActiveFlag() == FALSE )
		nResult = STOCKLEVEL_IMPORT_REJECT_INACTIVE_SOURCE;
	else
	{
		GetStockpointTotals( nStockIdx, StockRecord, nSpNo2, StockLevels );

		if ( StockLevels.GetActiveFlag() == FALSE )
			nResult = STOCKLEVEL_IMPORT_REJECT_INACTIVE_TARGET;
	}

	return nResult;
}

/**********************************************************************/
