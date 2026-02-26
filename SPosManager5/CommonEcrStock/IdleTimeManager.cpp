/**********************************************************************/
#include "..\CommonEcrStockTray\PCOptions.h"
#include "..\CommonEcrStockTray\PosTrayOptions.h"
/**********************************************************************/
#include "..\SPosStockManager5\DorbiereImporter.h"
#include "..\SPosStockManager5\LiveApparentStockExporter.h"
#include "..\SPosStockManager5\SalesXXXDetector.h"
#include "..\SPosStockManager5\StockOptionsIni.h"
#include "..\SPosStockManager5\SyncStockLevels.h"
/**********************************************************************/
#include "BarcodeAutoImporter.h"
#include "BatchSalesExporter.h"
#include "CustomerAutoDeleter.h"
#include "CustomerAutoImporterEOD.h"
#include "IDraughtExportManager.h"
#include "LocationCSVArray.h"
#include "PluAutoImporterCSV.h"
#include "PluAutoImporterEOD.h"
#include "PluAutoImporterEODPriceSet.h"
#include "SageExportManager.h"
#include "SageOptions.h"
#include "SysInfo.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "IdleTimeManager.h"
/**********************************************************************/
static const int IDLETIME_IMPORT_PLUCSV = 0;
static const int IDLETIME_IMPORT_CUSTOMER = 1;
static const int IDLETIME_IMPORT_BARCODE = 2;
static const int IDLETIME_SALES_XXX = 3;
static const int IDLETIME_SYNC_STOCK = 4;
static const int IDLETIME_IDRAUGHT_EXPORT = 5;
static const int IDLETIME_BATCH_EXPORT = 6;
static const int IDLETIME_SAGE_EXPORT = 7;
static const int IDLETIME_IMPORT_DORBIERE = 8;
/**********************************************************************/

CIdleTimeManager::CIdleTimeManager(void)
{
	m_nNextIdleTask = 0;
	m_timeNextCheck = COleDateTime::GetCurrentTime();
}

/**********************************************************************/

void CIdleTimeManager::DoNextTask( CWnd* pMainWnd )
{
	if ( ( SysInfo.GetScriptModeFlag() == FALSE ) && ( COleDateTime::GetCurrentTime() >= m_timeNextCheck ) )
	{
		if (0 == m_nNextIdleTask)
		{
			m_arrayTasks.RemoveAll();

			m_arrayTasks.Add(IDLETIME_IMPORT_PLUCSV);

			if (SysInfo.IsEcrLiteSystem() == FALSE)
			{
				if (EcrmanOptions.HasIdleTimeCustomerTask())
				{
					m_arrayTasks.Add(IDLETIME_IMPORT_CUSTOMER);
				}
			}

			m_arrayTasks.Add(IDLETIME_IMPORT_BARCODE);

#ifdef STOCKMAN_SYSTEM
			m_arrayTasks.Add(IDLETIME_SALES_XXX);
			m_arrayTasks.Add(IDLETIME_SYNC_STOCK);

			if (DealerFlags.GetDorbiereImportFlag() == TRUE)
			{
				m_arrayTasks.Add(IDLETIME_IMPORT_DORBIERE);
			}
#endif

			if (SysInfo.IsIDraughtExportSystem() == TRUE)
			{
				m_arrayTasks.Add(IDLETIME_IDRAUGHT_EXPORT);
			}

			if (DealerFlags.GetBatchExportFlag() == TRUE)
			{
				m_arrayTasks.Add(IDLETIME_BATCH_EXPORT);
			}

			if (SysInfo.IsEcrLiteSystem() == FALSE)
			{
				if ((SysInfo.IsSageExportSystem() == TRUE) && (SageOptions.GetExportEnableFlag() == TRUE))
				{
					m_arrayTasks.Add(IDLETIME_SAGE_EXPORT);
				}
			}
		}

		if ( ( m_nNextIdleTask >= 0 ) && ( m_nNextIdleTask < m_arrayTasks.GetSize() ) )
		{
			int nTask = m_arrayTasks.GetAt( m_nNextIdleTask );

			CString strIdleTimeTrace = "";
			strIdleTimeTrace.Format ( "%2.2d:%2.2d:%2.2d Idle Time Index %d Task %d\n", m_timeNextCheck.GetHour(), m_timeNextCheck.GetMinute(), m_timeNextCheck.GetSecond(), m_nNextIdleTask, nTask );
			TRACE( strIdleTimeTrace );

#ifndef STOCKMAN_SYSTEM

			switch( nTask )
			{
			case IDLETIME_IMPORT_PLUCSV:	
				AutoImportPluCSV();
				break;

			case IDLETIME_IMPORT_CUSTOMER:
				AutoImportCustomer();
				break;

			case IDLETIME_IMPORT_BARCODE:
				AutoImportBarcode();
				break;

			case IDLETIME_IDRAUGHT_EXPORT:
				IDraughtExport( pMainWnd );	
				break;

			case IDLETIME_BATCH_EXPORT:			
				ExportBatchSales();
				break;

			case IDLETIME_SAGE_EXPORT:
				SageExport( pMainWnd );
				break;
			}

#else
			if ( DealerFlags.GetExportLiveApparentStockFlag() == TRUE )
			{
				CLiveApparentStockExporter Exporter;
				Exporter.CheckAllDatabases();
			}

			switch( nTask )
			{
			case IDLETIME_IMPORT_PLUCSV:		
				AutoImportPluCSV();
				break;

			case IDLETIME_IMPORT_CUSTOMER:
				AutoImportCustomer();
				break;

			case IDLETIME_IMPORT_BARCODE:
				AutoImportBarcode();
				break;

			case IDLETIME_IDRAUGHT_EXPORT:
				IDraughtExport( pMainWnd );	
				break;

			case IDLETIME_BATCH_EXPORT:	
				ExportBatchSales();
				break;

			case IDLETIME_SAGE_EXPORT:
				SageExport( pMainWnd );
				break;

			case IDLETIME_SALES_XXX:			
				ProcessSalesXXX();
				break;

			case IDLETIME_SYNC_STOCK:			
				SyncStockLevels();
				break;

			case IDLETIME_IMPORT_DORBIERE:		
				DorbiereImport();
				break;
			}
#endif
		}

		m_nNextIdleTask++;
		if ((m_nNextIdleTask < 0) || (m_nNextIdleTask >= m_arrayTasks.GetSize()))
		{
			m_nNextIdleTask = 0;

			if (PosTrayOptions.GetManageBackupFlag() == FALSE)
			{
				if (PCOptionsHost.GetBackupAccessType() == 0)
				{
					m_timeNextCheck = COleDateTime::GetCurrentTime() + COleDateTimeSpan(0, 0, 0, 1);
					pMainWnd->SendMessage(WM_AUTOBACKUP, 0, 0);
				}
			}
		}
	}
}

/**********************************************************************/

void CIdleTimeManager::AutoImportPluCSV()
{
	PluAutoImporterCSV.ImportPluIdleTime();
	PluAutoImporterCSV.GotoNextDb();

	if ( EcrmanOptions.GetPluProdCSVImportType() != 0 )
	{
		PluAutoImporterEOD.ImportPluIdleTime();
		PluAutoImporterEOD.GotoNextLoc();

		if ( EcrmanOptions.GetPluPriceSetsFlag() == TRUE )
		{
			PluAutoImporterEODPriceSet.ImportPluIdleTime();
			PluAutoImporterEODPriceSet.GotoNextLoc();
		}
	}
}

/**********************************************************************/

void CIdleTimeManager::AutoImportCustomer()
{
	if ( EcrmanOptions.GetFeaturesCustomerNameImportType() != 0 )		
	{
		CustomerAutoImporterEOD.ImportCustomerIdleTime();
		CustomerAutoImporterEOD.GotoNextLoc();
	}

	if ( EcrmanOptions.GetFeaturesCustomerAccountAutoDeleteFlag() == TRUE )
	{
		CustomerAutoDeleterAccount.DeleteCustomerIdleTime();
		CustomerAutoDeleterAccount.GotoNextLoc();
	}
	
	if ( EcrmanOptions.GetFeaturesCustomerOneOffAutoDeleteFlag() == TRUE )
	{
		CustomerAutoDeleterOneOff.DeleteCustomerIdleTime();
		CustomerAutoDeleterOneOff.GotoNextLoc();
	}
}

/**********************************************************************/

void CIdleTimeManager::AutoImportBarcode()
{
	BarcodeAutoImporter.ImportBarcodeIdleTime();
	BarcodeAutoImporter.GotoNextDb();
}

/**********************************************************************/

void CIdleTimeManager::IDraughtExport( CWnd* pMainWnd )
{
	if ( IDraughtExportManager.CheckExportTime() == TRUE )
	{
		long lResult = pMainWnd -> SendMessage ( WM_IDRAUGHT_EXPORT, 0, 0 );
		
		if ( IDraughtExportManager.SetNetworkError( lResult ) == TRUE )
			IDraughtExportManager.LogResult( lResult );
	}
	
	IDraughtExportManager.NextNetworkMapIdx();

	if ( IDraughtExportManager.GetNetworkMapIdx() == 0 )
	{
		if ( IDraughtExportManager.CheckConnectTime() == TRUE )
		{
			long lResult = pMainWnd -> SendMessage ( WM_IDRAUGHT_TRANSFER, 0, 0 );
			IDraughtExportManager.LogResult( lResult );
			
			if ( lResult != 0 )
				IDraughtExportManager.SetConnectTime();
		}
	}
}

/**********************************************************************/

void CIdleTimeManager::SageExport( CWnd* pMainWnd )
{
	if ( ( SageOptions.GetExportEnableFlag() == TRUE ) && ( SageExportManager.CheckExportTime() == TRUE ) )
	{
		long lResult = pMainWnd -> SendMessage ( WM_SAGE_EXPORT, 0, 0 );
		
		if ( SageExportManager.SetDatabaseError( lResult ) == TRUE )
			SageExportManager.LogResult( lResult );
	}
	
	SageExportManager.NextDatabaseMapIdx();
}

/**********************************************************************/

void CIdleTimeManager::ExportBatchSales()
{
	BatchSalesExporter.ExportBatchSalesIdleTime();
	BatchSalesExporter.GotoNextNetwork();
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

void CIdleTimeManager::ProcessSalesXXX()
{
	SalesXXXDetector.LookForSalesXXX();
}

/**********************************************************************/

void CIdleTimeManager::SyncStockLevels()
{
	GlobalStockSync.CheckStockpoint();
}

/**********************************************************************/

void CIdleTimeManager::DorbiereImport()
{
	CDorbiereImporter DorbiereImporter;
	DorbiereImporter.CheckForImport();
}

/**********************************************************************/
#endif
/**********************************************************************/
