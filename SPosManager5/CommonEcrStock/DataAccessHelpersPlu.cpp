/**********************************************************************/
#include "BarcodeAutoImporter.h"
#include "BarcodeDeleteDlg.h"
#include "CommsBatchDlg.h"
#include "CommsHandshaker.h"
#include "DownloadDateDlg.h"
#include "KbdListNameArray.h"
#include "ListLabelsDlg.h"
#include "LocationCSVArray.h"
#include "MessageLogger.h"
#include "MyCSSLockFile.h"
#include "Password.h"
#include "PluAutoImporterCSV.h"
#include "PluAutoImporterEOD.h"
#include "PluAutoImporterEODPriceSet.h"
#include "..\SPosEcrManager5\PluDatabaseDlgEcrman.h"
#include "..\SPosStockManager5\PluDatabaseDlgStockman.h"
#include "PriceSetItemManager.h"
#include "StatusProgress.h"
#include "..\SPosStockManager5\StockDeleteDatabaseDlg.h"
#include "..\SPosStockManager5\StockLevelsAccessDlg.h"
#include "..\SPosStockManager5\StockOptionsIni.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DataAccessHelpersPlu.h"
/**********************************************************************/
extern CBarcodeAutoImporter BarcodeAutoImporter;
extern CPluAutoImporterCSV PluAutoImporterCSV;
/**********************************************************************/

CDataAccessHelpersPlu::CDataAccessHelpersPlu()
{
}

/**********************************************************************/

void CDataAccessHelpersPlu::RunPluDatabase( int nDbIdx, bool bReadOnly, bool& bCanDoChangesTask, bool& bCanDeleteStock, bool& bCanDeleteBarcode )
{
#ifdef STOCKMAN_SYSTEM

	CPluDatabaseDlgStockman dlg ( nDbIdx, m_pParentWnd );

	dlg.SetReadOnlyFlag( bReadOnly );
	dlg.DoModal();

	bCanDoChangesTask = dlg.GetCanDoChangesTaskFlag();
	bCanDeleteStock = dlg.GetCanDeleteStockFlag();
	bCanDeleteBarcode = dlg.GetCanDeleteBarcodeFlag();

#else

	CPluDatabaseDlgEcrman dlg ( nDbIdx, m_pParentWnd );

	dlg.SetReadOnlyFlag( bReadOnly );
	dlg.DoModal();

	bCanDoChangesTask = dlg.GetCanDoChangesTaskFlag();
	bCanDeleteStock = FALSE;
	bCanDeleteBarcode = dlg.GetCanDeleteBarcodeFlag();
	
#endif
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

void CDataAccessHelpersPlu::EditPlu( int nDbNo, bool bFromButton )
{
	int nDbIdx;
	if (dbDatabase.FindDatabaseByNumber(nDbNo, nDbIdx) == FALSE)
	{
		return;
	}

	//AUTO IMPORT PLU ITEMS
	PluAutoImporterCSV.ImportPluExternal( nDbIdx );
	PluAutoImporterEOD.ImportPluExternal( nDbIdx );
	
	//AUTO IMPORT BARCODE ITEMS
	BarcodeAutoImporter.ImportBarcodeExternal( nDbIdx );
	
	//FIND OUT IF WE NEED STOCK LEVEL ACCESS
	int nType = DealerFlags.GetStockLevelsPluType(); 
	if (nType == STOCKLEVELS_ACCESS_PROMPT)
	{
		CStockLevelsAccessDlg dlg(FALSE, m_pParentWnd);
		if (dlg.DoModal() != IDOK)
		{
			return;
		}

		nType = dlg.m_nType;
	}
	DealerFlags.SetCurrentStockLevelsAccessType(nType);

	CWaitCursor wait;
	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_PLU );
	arrayReadWrite.Add( NODE_BARCODE );
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
							
						if (info.m_nErrorType != DB_ERR_SHARE)
						{
							Prompter.ShareError(info);
						}

						break;
					}

					if ( DataManager.OpenTransferDetail( nSpIdx, DATAMANAGER_OPEN_READWRITE, info ) == FALSE )
					{
						DataManager.CloseDatabase();
							
						if (info.m_nErrorType != DB_ERR_SHARE)
						{
							Prompter.ShareError(info);
						}
						
						break;
					}
				}
				StatusProgress.Unlock();

				if (info.m_nErrorType == DATAMANAGER_ERROR_NONE)
				{
					bReadWrite = TRUE;
				}
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
		if (Prompter.SuggestReadOnly(info) != IDYES)
		{
			return;
		}
			
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

	EditPluInternal( nDbIdx, bReadOnly );
	DataManager.CloseDatabase();
}

/**********************************************************************/
#else
/**********************************************************************/

void CDataAccessHelpersPlu::EditPlu( int nDbNo, bool bFromButton )
{
	int nDbIdx;
	if (dbDatabase.FindDatabaseByNumber(nDbNo, nDbIdx) == FALSE)
	{
		return;
	}

	if ((EcrmanOptions.GetFeaturesPluStockFlag()) && (FALSE == bFromButton))
	{
		CommsHandshaker.ProcessSales();
	}

	PluAutoImporterCSV.ImportPluExternal( nDbIdx );
	PluAutoImporterEOD.ImportPluExternal( nDbIdx );
	BarcodeAutoImporter.ImportBarcodeExternal( nDbIdx );
	
	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_PLU );
	arrayReadWrite.Add( NODE_BARCODE );

	CDataManagerInfo info;
	
	bool bReadOnly = FALSE;
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, info, FALSE ) == FALSE )
	{
		if ( info.m_nErrorType != DB_ERR_SHARE )
		{
			Prompter.ShareError( info );
			return;
		}

		info.m_nNodeType = NODE_PLU;
		if (Prompter.SuggestReadOnly(info) != IDYES)
		{
			return;
		}
			
		DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
		bReadOnly = TRUE;
	}

	EditPluInternal( nDbIdx, bReadOnly );
	DataManager.CloseDatabase();
}

/**********************************************************************/
#endif
/**********************************************************************/

void CDataAccessHelpersPlu::EditPriceSet(int nNetworkNo, int nDbNo, int nLocNo, int nPriceSet, bool bFromButton)
{
	int nLocIdx;
	if (dbLocation.FindLocationByNumber(nNetworkNo, nLocNo, nLocIdx) == FALSE)
	{
		return;
	}

	int nDbIdx = 0;
	if (dbDatabase.FindDatabaseByNumber(nDbNo, nDbIdx) == FALSE)
	{
		return;
	}

	if ((EcrmanOptions.GetFeaturesPluStockFlag()) && (FALSE == bFromButton))
	{
		CommsHandshaker.ProcessSales();
	}

	PluAutoImporterCSV.ImportPluExternal( nDbIdx );
	PluAutoImporterEOD.ImportPluExternal( nDbIdx );
	PluAutoImporterEODPriceSet.ImportPluExternal( nLocIdx );
	BarcodeAutoImporter.ImportBarcodeExternal( nDbIdx );

	bool bReadOnly = FALSE;
	bool bContinue = TRUE;

	PriceSetItemManager.SelectPriceSet ( nLocIdx, nPriceSet );

	CMyCSSLockFile LockFile;
	if ( LockFile.OpenLock( PriceSetItemManager.GetLockFilename() ) == FALSE )
	{
		CDataManagerInfo info;
		info.m_nNodeType = NODE_PRICESET1;
		if (Prompter.SuggestReadOnly(info) != IDYES)
		{
			bContinue = FALSE;
		}
		else
		{
			bReadOnly = TRUE;
		}
	}

	if ( TRUE == bContinue )
	{
		CDataManagerInfo info;	
		DataManager.SetPluPriceSetModeFlag(TRUE);
		DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
		DataManager.SetPluPriceSetModeFlag(FALSE);
		EditPriceSetInternal( nDbIdx, nLocIdx, nPriceSet, bReadOnly );
		DataManager.CloseDatabase();
		DataManager.Plu.RemoveAll();
	}

	if ((TRUE == bContinue) && (FALSE == bReadOnly))
	{
		LockFile.Close();
	}
}

/**********************************************************************/

void CDataAccessHelpersPlu::EditPriceSetInternal(int nDbIdx, int nLocIdx, int nPriceSet, bool bReadOnly)
{
	if (bReadOnly == FALSE)
	{
		int nUseCount = dbLocation.GetPriceSetUsageCount(nLocIdx, nPriceSet);
		if (nUseCount >= 2)
		{
			CString strMsg;
			strMsg.Format("The price set that you have chosen to edit is shared by %d locations.\n\n", nUseCount);
			strMsg += "Editing this price set will affect all the locations that are linked to it.\n\n";
			strMsg += "Are you sure you wish to edit this price set";

			if (Prompter.YesNo(strMsg, TRUE) != IDYES)
			{
				return;
			}
		}
	}

	CWaitCursor wait;
	PriceSetItemManager.SelectPriceSet(nLocIdx, nPriceSet);
	PriceSetItemManager.ReadPriceSetFile(FALSE, DataManager.Plu);

	DataManager.Plu.SetModifiedFlag(FALSE);

	KbdListNameArray.Read(nDbIdx, nLocIdx, nPriceSet);

	CDataManagerInfo info;
	DataManagerNonDb.OpenAllergen(DB_READONLY, info);
	DataManagerNonDb.Allergen.AddAllergenZero();
	DataManagerNonDb.OpenOPGroupSystem(DB_READONLY, info);
	DataManagerNonDb.OpenOPGroupDatabase(nDbIdx, DB_READONLY, info);
	DataManagerNonDb.OpenOPGroupLocation(nLocIdx, DB_READONLY, info);
	DataManagerNonDb.OpenOPNameSystem(DB_READONLY, info);
	DataManagerNonDb.OpenOPNameDatabase(nDbIdx, DB_READONLY, info);
	DataManagerNonDb.OpenOPNameLocation(nLocIdx, DB_READONLY, info);

#ifdef STOCKMAN_SYSTEM
	DataManagerNonDb.OpenCustomFieldNames(DB_READONLY, info);
#endif

	bool bCanDoChangesTask = FALSE;
	bool bCanDeleteStock = FALSE;
	bool bCanDeleteBarcode = FALSE;
	RunPluDatabase(nDbIdx, bReadOnly, bCanDoChangesTask, bCanDeleteStock, bCanDeleteBarcode);

	DataManagerNonDb.Allergen.RemoveAllergenZero();
	DataManagerNonDb.CloseAllergen(info);
	DataManagerNonDb.CloseOPGroupSystem(info);
	DataManagerNonDb.CloseOPGroupDatabase(info);
	DataManagerNonDb.CloseOPGroupLocation(info);
	DataManagerNonDb.CloseOPNameSystem(info);
	DataManagerNonDb.CloseOPNameDatabase(info);
	DataManagerNonDb.CloseOPNameLocation(info);

#ifdef STOCKMAN_SYSTEM
	DataManagerNonDb.CloseCustomFieldNames(info);
#endif

	if (dbLocation.GetDefaultPriceSet(nLocIdx) == nPriceSet)
	{
		if (TRUE == bCanDoChangesTask)
		{
			RunPluChangesTask(nDbIdx);
		}
	}

	if (bReadOnly == TRUE)
	{
		return;
	}

	//CHECK FOR CHANGED LABELS
	if ((EcrmanOptions.GetLabelsShelfAutoBatchFlag() && EcrmanOptions.GetLabelsShelfPromptBatchFlag()) == FALSE)
	{
		return;
	}

	CDataManagerInfo info2;
	if (DataManager.OpenShelfLabels(DB_READWRITE, nLocIdx, nPriceSet, info2) == FALSE)
	{
		Prompter.ShareError(info2);
		return;
	}

	DataManager.ShelfLabels.CreateShelfChangesList();

	if (DataManager.ShelfLabels.GetSize() == 0)
	{
		return;
	}

	CString strMsg = "One or more Plu records has changed since you last printed\n";
	strMsg += "shelf edge labels for this price set.\n\n";
	strMsg += "Do you wish to print updated labels now.";

	if (Prompter.YesNo(strMsg, TRUE) != IDYES)
	{
		return;
	}

	CListLabelsDlg dlgLabels(FALSE, FALSE, TRUE, DataManager.ShelfLabels, "", m_pParentWnd);

	int nResult = dlgLabels.DoModal();
	m_pParentWnd->GetParent()->RedrawWindow();

	if (nResult == IDOK)
	{
		DataManager.ShelfLabels.UpdateShelfChangesList();

		CDataManagerInfo info2;
		if (DataManager.WriteShelfLabels(info2) == FALSE)
		{
			Prompter.WriteError(info2);
		}
	}
}

/**********************************************************************/

void CDataAccessHelpersPlu::EditPluInternal(int nDbIdx, bool bReadOnly)
{
	CDataManagerInfo info;
	DataManagerNonDb.OpenAllergen(DB_READONLY, info);
	DataManagerNonDb.Allergen.AddAllergenZero();
	DataManagerNonDb.OpenOPGroupSystem(DB_READONLY, info);
	DataManagerNonDb.OpenOPGroupDatabase(nDbIdx, DB_READONLY, info);
	DataManagerNonDb.OpenOPNameSystem(DB_READONLY, info);
	DataManagerNonDb.OpenOPNameDatabase(nDbIdx, DB_READONLY, info);

#ifdef STOCKMAN_SYSTEM
	DataManagerNonDb.OpenCustomFieldNames(DB_READONLY, info);
#endif

	bool bCanDoChangesTask = FALSE;
	bool bCanDeleteStock = FALSE;
	bool bCanDeleteBarcode = FALSE;
	RunPluDatabase(nDbIdx, bReadOnly, bCanDoChangesTask, bCanDeleteStock, bCanDeleteBarcode);

	DataManagerNonDb.Allergen.RemoveAllergenZero();
	DataManagerNonDb.CloseAllergen(info);
	DataManagerNonDb.CloseOPGroupSystem(info);
	DataManagerNonDb.CloseOPGroupDatabase(info);
	DataManagerNonDb.CloseOPNameSystem(info);
	DataManagerNonDb.CloseOPNameDatabase(info);

#ifdef STOCKMAN_SYSTEM
	DataManagerNonDb.CloseCustomFieldNames(info);
#endif

	if (TRUE == bCanDoChangesTask)
	{
		RunPluChangesTask(nDbIdx);
	}

#ifdef STOCKMAN_SYSTEM
	if (TRUE == bCanDeleteStock)
	{
		AutoDeleteStock();
	}
#endif

	if (TRUE == bCanDeleteBarcode)
	{
		AutoDeleteBarcode();
	}

	if (bReadOnly == TRUE)
	{
		return;
	}

	//CHECK FOR CHANGED LABELS
	if ((EcrmanOptions.GetLabelsShelfAutoBatchFlag() && EcrmanOptions.GetLabelsShelfPromptBatchFlag()) == FALSE)
	{
		return;
	}

	CDataManagerInfo info2;
	if (DataManager.OpenShelfLabels(DB_READWRITE, info2) == FALSE)
	{
		Prompter.ShareError(info2);
		return;
	}

	DataManager.ShelfLabels.CreateShelfChangesList();

	if (DataManager.ShelfLabels.GetSize() == 0)
	{
		return;
	}

	CString strMsg = "One or more Plu records has changed since you last printed\n";
	strMsg += "shelf edge labels for this database.\n\n";

	if (EcrmanOptions.GetPluPriceSetsFlag())
	{
		strMsg += "You should also check any price sets that are linked to this\n";
		strMsg += "database to see if updated labels need to be printed.\n\n";
		strMsg += "Do you wish to print the updated shelf edge labels for the\n";
		strMsg += "master Plu database.";
	}
	else
	{
		strMsg += "Do you wish to print updated labels now.";
	}

	if (Prompter.YesNo(strMsg, TRUE) != IDYES)
	{
		return;
	}

	CListLabelsDlg dlgLabels(FALSE, FALSE, TRUE, DataManager.ShelfLabels, "", m_pParentWnd);

	int nResult = dlgLabels.DoModal();
	m_pParentWnd->GetParent()->RedrawWindow();

	if (nResult == IDOK)
	{
		DataManager.ShelfLabels.UpdateShelfChangesList();

		CDataManagerInfo info2;
		if (DataManager.WriteShelfLabels(info2) == FALSE)
		{
			Prompter.WriteError(info2);
		}
	}
}

/**********************************************************************/

void CDataAccessHelpersPlu::AutoDeleteBarcode()
{
	int nType = EcrmanOptions.GetPluAutoDeleteBarcodeType(); 
	if ((nType < 1) || (nType > 3))
	{
		return;
	}
	
	CArray<int,int> arrayBarcodeIdx;
	DataManager.Barcode.GetUnlinkedBarcodeList( arrayBarcodeIdx );

	int nUnlinked = arrayBarcodeIdx.GetSize();	
	if (nUnlinked == 0)
	{
		return;
	}

	if (nType != 3)
	{
		CString strMsg = "";

		if (nUnlinked == 1)
		{
			strMsg.Format("%s has detected 1 barcode that is not linked to any Plu.\n\n", AfxGetAppName());

			if (nType != 2)
			{
				strMsg += "Would you like to view this barcode and delete it if required.";
			}
			else
			{
				strMsg += "Would you like to delete this barcode now.";
			}
		}
		else
		{
			strMsg.Format("%s has detected %d barcodes that are not linked to any Plu.\n\n", AfxGetAppName(), nUnlinked);

			if (nType != 2)
			{
				strMsg += "Would you like to view these barcodes and delete them if required.";
			}
			else
			{
				strMsg += "Would you like to delete these barcodes now.";
			}
		}

		if (Prompter.YesNo(strMsg, (1 == nType)) != IDYES)
		{
			return;
		}
	}
	
	{
		if (1 == nType)
		{
			CBarcodeDeleteDlg dlgDelete(arrayBarcodeIdx, m_pParentWnd);
			if (dlgDelete.DoModal() != IDOK)
			{
				return;
			}
		}

		DataManager.Barcode.DeleteBarcodes( arrayBarcodeIdx );

		CDataManagerInfo info;
		if (DataManager.WriteBarcode(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else
		{
			CDWordArray dw;
			dw.Add( nType );
			dw.Add(nUnlinked);
			dw.Add(arrayBarcodeIdx.GetSize());
			dw.Add(DataManager.Barcode.GetSize() );
			MessageLogger.LogSystem( 11, 1, TRUE, dw );

			if (nType != 3)
			{
				if (arrayBarcodeIdx.GetSize() != 0)
				{
					Prompter.Info("Barcodes were deleted as requested.");
				}
			}
		}
	}
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

void CDataAccessHelpersPlu::AutoDeleteStock()
{
	if (DealerFlags.GetCurrentStockLevelsAccessType() != STOCKLEVELS_ACCESS_READWRITE)
	{
		return;
	}

	if (StockOptions.GetPluDeleteUnlinkedStockFlag() == FALSE)
	{
		return;
	}

	DataManager.StockPlu.Invalidate();
	CStockDeleteDatabaseDlg dlgDelete( FALSE, FALSE, FALSE, m_pParentWnd );

	int nUnlinked = dlgDelete.GetFilterSize();	
	if (nUnlinked == 0)
	{
		return;
	}
	
	CString strMsg = "";
			
	if ( nUnlinked == 1 )
	{
		strMsg = "Stock Manager has detected 1 stock item that is not linked to any Plu.\n\n";
		strMsg += "Would you like to view this item now with an option to delete it if required.";
	}
	else
	{
		strMsg.Format ( "Stock Manager has detected %d stock items that are not linked to any Plu.\n\n", nUnlinked );
		strMsg += "Would you like to view these items now with an option to delete them if required.";
	}

	if (Prompter.YesNo(strMsg, TRUE) == IDYES)
	{
		CDWordArray dw;
		dw.Add(nUnlinked);
		dw.Add(DataManager.Stock.GetSize());
		MessageLogger.LogSystem(4, 1, TRUE, dw);

		dlgDelete.DoModal();

		if (dlgDelete.GetDoneDeleteFlag() == TRUE)
		{
			Prompter.Info("Stock records were deleted as requested.");
		}
	}
}

/**********************************************************************/
#endif
/**********************************************************************/

void CDataAccessHelpersPlu::PrintShelfLabels(int nDbNo)
{
	int nDbIdx = GetDbIdx(nDbNo);
	if (nDbIdx == -1) return;

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly(nDbIdx, info, FALSE);

	if (DataManager.OpenShelfLabels(DB_READWRITE, info) == FALSE)
	{
		Prompter.ShareError(info);
		return;
	}

	DataManager.ShelfLabels.CreateShelfChangesList();

	CListLabelsDlg dlg(FALSE, FALSE, TRUE, DataManager.ShelfLabels, "", m_pParentWnd);

	if (dlg.DoModal() == IDOK)
	{
		DataManager.ShelfLabels.UpdateShelfChangesList();

		if (DataManager.WriteShelfLabels(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	DataManager.CloseShelfLabels(info);
}

/**********************************************************************/

void CDataAccessHelpersPlu::PrintShelfLabels(int nNetworkNo, int nDbNo, int nLocNo, int nPriceSet)
{
	int nLocIdx = 0;
	if (dbLocation.FindLocationByNumber(nNetworkNo, nLocNo, nLocIdx) == FALSE)
	{
		return;
	}

	int nDbIdx = 0;
	if (dbDatabase.FindDatabaseByNumber(nDbNo, nDbIdx) == FALSE)
	{
		return;
	}

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly(nDbIdx, info, FALSE);

	if (DataManager.OpenShelfLabels(DB_READWRITE, nLocIdx, nPriceSet, info) == FALSE)
	{
		Prompter.ShareError(info);
		return;
	}

	PriceSetItemManager.SelectPriceSet(nLocIdx, nPriceSet);
	PriceSetItemManager.ReadPriceSetFile(FALSE, DataManager.Plu);

	DataManager.ShelfLabels.CreateShelfChangesList();

	CListLabelsDlg dlg(FALSE, FALSE, TRUE, DataManager.ShelfLabels, "", m_pParentWnd);

	if (dlg.DoModal() == IDOK)
	{
		DataManager.ShelfLabels.UpdateShelfChangesList();

		if (DataManager.WriteShelfLabels(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	DataManager.CloseShelfLabels(info);
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

void CDataAccessHelpersPlu::PrintBarcodeLabels(int nDbNo)
{
	int nDbIdx = GetDbIdx(nDbNo);

	if (nDbIdx == -1)
	{
		return;
	}

	CWordArray arrayReadWrite;
	arrayReadWrite.Add(NODE_LABELS_PRODUCT);

	CDataManagerInfo info;
	if (DataManager.OpenDatabaseReadWrite(nDbIdx, arrayReadWrite, info, FALSE) == FALSE)
	{
		Prompter.ShareError(info);
		return;
	}

	CListLabelsDlg dlg(FALSE, FALSE, TRUE, DataManager.ProductLabels, "", m_pParentWnd);

	if (dlg.DoModal() == IDOK)
	{
		if (DataManager.WriteProductLabels(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	DataManager.CloseDatabase();
}

/**********************************************************************/

void CDataAccessHelpersPlu::PrintBarcodeLabels(int nNetworkNo, int nDbNo, int nLocNo, int nPriceSet)
{
	int nLocIdx = 0;
	if (dbLocation.FindLocationByNumber(nNetworkNo, nLocNo, nLocIdx) == FALSE)
	{
		return;
	}

	int nDbIdx = 0;
	if (dbDatabase.FindDatabaseByNumber(nDbNo, nDbIdx) == FALSE)
	{
		return;
	}

	CWordArray arrayReadWrite;
	arrayReadWrite.Add(NODE_LABELS_PRODUCT);

	CDataManagerInfo info;
	if (DataManager.OpenDatabaseReadWrite(nDbIdx, arrayReadWrite, info, FALSE) == FALSE)
	{
		Prompter.ShareError(info);
		return;
	}

	PriceSetItemManager.SelectPriceSet(nLocIdx, nPriceSet);
	PriceSetItemManager.ReadPriceSetFile(FALSE, DataManager.Plu);

	CListLabelsDlg dlg(FALSE, FALSE, TRUE, DataManager.ProductLabels, "", m_pParentWnd);

	if (dlg.DoModal() == IDOK)
	{
		if (DataManager.WriteProductLabels(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
	}

	DataManager.CloseDatabase();
}

/**********************************************************************/
#endif
/**********************************************************************/

void CDataAccessHelpersPlu::RunPluChangesTask( int nDbIdx )
{
	if (EcrmanOptions.GetPluTaskAfterChangesFlag() == FALSE)
	{
		return;
	}

	if (PasswordArray.GetEnable(PasswordTicks::PluChangesTask) == FALSE)
	{
		return;
	}

	CEposTaskDatabase TaskDatabase;
	if (TaskDatabase.ReadPluChangesTask() == FALSE)
	{
		return;
	}

	if (TaskDatabase.GetEditableTaskJobCount() == 0)
	{
		return;
	}

	CEposTaskHeader TaskHeader;
	TaskDatabase.GetEditableTaskHeader( TaskHeader );

	CString strDate = "";
	if (TaskHeader.GetImportType() == IMPORT_SPECIFIED)
	{
		if (TaskDatabase.HasEditableTaskDownloadJob() == TRUE)
		{
			CString strTitle;
			strTitle.Format("Epos Import Date (%s)", TaskHeader.GetName());

			CDownloadDateDlg dlgDate(strTitle, m_pParentWnd);
			if (dlgDate.RunIfNeeded() == FALSE)
			{
				return;
			}

			if (dlgDate.m_bImmediate == TRUE)
			{
				TaskHeader.SetImportType(IMPORT_IMMEDIATE);
			}
			else
			{
				strDate = dlgDate.m_strDate;
				TaskHeader.SetImportType(IMPORT_SPECIFIED);
			}
		}
	}

	TaskDatabase.SetEditableTaskHeader( TaskHeader );

	CKeyboardMap KeyboardMap;
	CCommsBatchDlg BatchDlg ( TaskDatabase, strDate, FALSE, KeyboardMap, nDbIdx, 0, 0, m_pParentWnd );
	BatchDlg.DoModal();
}

/**********************************************************************/
