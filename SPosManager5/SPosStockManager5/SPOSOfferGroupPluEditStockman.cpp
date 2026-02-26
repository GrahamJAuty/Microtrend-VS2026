/**********************************************************************/
#include "..\SPosStockManager5\OpeningStockAuditer.h"
#include "..\SPosStockManager5\PropertySheetPluStock.h"
#include "..\SPosStockManager5\StockOptionsIni.h"
/**********************************************************************/
#include "..\CommonEcrStock\BarcodeAutoImporter.h"
#include "..\CommonEcrStock\CustomExportManager.h"
#include "..\CommonEcrStock\KbdListNameArray.h"
#include "..\CommonEcrStock\KeyboardHelpers.h"
#include "..\CommonEcrStock\PluAutoImporterCSV.h"
#include "..\CommonEcrStock\PluAutoImporterEOD.h"
/**********************************************************************/
#include "..\CommonEcrStockTray\EditStockTypes.h"
/**********************************************************************/
#include "Append.h"
#include "CommsHandshaker.h"
#include "FileRemover.h"
#include "FilterTypes.h"
#include "MessageLogger.h"
#include "PosTrayTokenFiles.h"
#include "PropertySheetPluStockman.h"
#include "SysInfo.h"
#include "SystemMap.h"
/**********************************************************************/
#include "SPOSOfferGroupPluEditStockman.h"
/**********************************************************************/

CSPOSOfferGroupPluEditStockman::CSPOSOfferGroupPluEditStockman( __int64 nPluNo, CWnd* pParent ) : m_FilterArray( FILTERTYPE_DATABASE )
{
	m_nPluNo = nPluNo;
	m_pParent = pParent;
	DataManager.ResourceWrapperPlu.Reset();
	DataManager.ListKeyImageWrapperPlu.Reset();
}

/**********************************************************************/

CSPOSOfferGroupPluEditStockman::~CSPOSOfferGroupPluEditStockman(void)
{
	DataManager.ResourceWrapperPlu.Reset();
	DataManager.ListKeyImageWrapperPlu.Reset();
}

/**********************************************************************/

bool CSPOSOfferGroupPluEditStockman::LoadDatabase()
{
	int nDbIdx = DataManager.GetActiveDbIdx();

	PluAutoImporterCSV.ImportPluExternal( nDbIdx );
	PluAutoImporterEOD.ImportPluExternal( nDbIdx );
	BarcodeAutoImporter.ImportBarcodeExternal( nDbIdx );
	
	KbdListNameArray.SetFakeReadFlag();
	KbdListNameArray.Read(nDbIdx);
	KbdListNameArray.ClearFakeReadFlag();

	CDataManagerInfo info;
	DataManager.ClosePlu(info);
	DataManager.CloseBarcode(info);
	DataManager.CloseStock(info);
	DataManager.CloseCategory(info);
	DataManager.CloseSupplier(info);

	for (int n = 1; n < MAX_ITEM_CUSTOM_FIELDS; n++)
	{
		DataManager.CloseCustomFieldValue(n, info);
	}

	/*****/

	if (DataManager.OpenPlu(DB_READWRITE, info) == FALSE)
	{
		Prompter.ShareError(info);
		return FALSE;
	}

	if (DataManager.OpenStock(DB_READWRITE, info) == FALSE)
	{
		Prompter.ShareError(info);
		return FALSE;
	}

	if (DataManager.OpenBarcode(DB_READWRITE, info) == FALSE)
	{
		Prompter.ShareError(info);
		return FALSE;
	}

	if (DataManager.OpenCategory(DB_READONLY, info) == FALSE)
	{
		Prompter.ShareError(info);
		return FALSE;
	}

	if (DataManager.OpenSupplier(DB_READONLY, info) == FALSE)
	{
		Prompter.ShareError(info);
		return FALSE;
	}

	for (int n = 1; n < MAX_ITEM_CUSTOM_FIELDS; n++)
	{
		if (DataManager.OpenCustomFieldValue(n, DB_READONLY, info) == FALSE)
		{
			Prompter.ShareError(info);
			return FALSE;
		}
	}

	DataManagerNonDb.OpenAllergen(DB_READONLY, info);
	DataManagerNonDb.Allergen.AddAllergenZero();
	DataManagerNonDb.OpenOPGroupSystem(DB_READONLY, info);
	DataManagerNonDb.OpenOPGroupDatabase(nDbIdx, DB_READONLY, info);
	DataManagerNonDb.OpenOPNameSystem(DB_READONLY, info);
	DataManagerNonDb.OpenOPNameDatabase(nDbIdx, DB_READONLY, info);
	DataManagerNonDb.OpenCustomFieldNames(DB_READONLY, info);
	
	return TRUE;
}

/**********************************************************************/

bool CSPOSOfferGroupPluEditStockman::EditPlu()
{
	bool bResult = FALSE;

	int nPluIdx = 0;
	if (DataManager.Plu.FindPluByNumber(m_nPluNo, nPluIdx) == TRUE)
	{
		bResult = EditPluInternal(nPluIdx);
	}

	return bResult;
}

/**********************************************************************/

bool CSPOSOfferGroupPluEditStockman::EditPluInternal( int nPluIdx )
{
	bool bResult = FALSE;

	m_FilterArray.SetArrayIdx ( nPluIdx );

	int nStockIdx = 0;
	bool bAllowSpin = FALSE; 
	bool bSimpleEditMode = FALSE; 

	if ( StockOptions.GetPluSimplifiedEditingFlag() == FALSE || PriceSetItemManager.IsPluMaster() == FALSE )
	{
		bAllowSpin = TRUE;
		bSimpleEditMode = FALSE;
	}
	else
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( m_FilterArray.GetPluItemIndex(), PluRecord );
	
		if ( DataManager.Plu.IsRecipe ( PluRecord.GetPluNoInt() ) )
		{	
			bAllowSpin = FALSE;
			bSimpleEditMode = FALSE;
		}
		else
		{
			CString strStockCode;
			DataManager.Plu.GetPluStockCode ( PluRecord.GetPluNoInt(), strStockCode, FALSE );

			if ( strStockCode == "" )
			{
				bAllowSpin = FALSE;
				bSimpleEditMode = FALSE;
			}
			else 
			{
				bAllowSpin = DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx );
				bSimpleEditMode = bAllowSpin;
			}
		}		
	}

	//IF WE WANT TO DISPLAY THE APPARENT TAB OR THE STOCK LEVEL TAB WITH CORRECT VALUES
	//THEN WE NEED TO LOAD STOCK LEVELS INTO MEMORY. THIS IS NOT DONE HERE AS IT IS NOT
	//NECESSARY FOR THE PLU EDIT FUNCTIONALITY. IF IT IS REQUIRED THEN THE STOCK LEVELS
	//SHOULD BE LOADED INTO MEMORY IN THE LOAD DATABASE FUNCTIONS.
	
	if ( bSimpleEditMode == TRUE )
	{	
		CPropertySheetPluStockInfo info;
		info.m_bEnableSpin = TRUE;
		info.m_wSpinID = IDC_SPIN;
		info.m_pPluFilter = &m_FilterArray;
		info.m_nStockIdx = nStockIdx;
		info.m_bNewStock = FALSE;
		info.m_nDefaultSuppNo = 0;
		info.m_bReadOnly = FALSE;
		//info.m_bStockLevelTab = ( DealerFlags.GetCurrentStockLevelsAccessType() == STOCKLEVELS_ACCESS_READWRITE );
		//info.m_bApparentTab = ( DealerFlags.GetCurrentStockLevelsAccessType() != STOCKLEVELS_ACCESS_NONE );
		info.m_bStockLevelTab = FALSE;
		info.m_bApparentTab = FALSE;

		CPropertySheetPluStock propSheet ( info, m_pParent );
		propSheet.EnableStackedTabs ( FALSE );

		if ( propSheet.DoModal() == IDOK )
		{
			propSheet.SavePluRecord();
			bResult = TRUE;
		}
		else
			propSheet.UndoChanges();
	}
	else
	{
		int nEditStock = ( PriceSetItemManager.IsPluMaster() == TRUE ) ? EDIT_STOCK_FULL : EDIT_STOCK_NONE;
		
		CPropertySheetPluStockmanInfo info;
		info.m_bEnableSpin = bAllowSpin;
		info.m_wSpinID = IDC_SPIN;
		info.m_pPluFilter = &m_FilterArray;
		info.m_nEditMode = nEditStock;
		info.m_bNewStock = FALSE;
		info.m_bReadOnly = FALSE;
		//info.m_bStockLevelTab = ( DealerFlags.GetCurrentStockLevelsAccessType() == STOCKLEVELS_ACCESS_READWRITE );
		//info.m_bApparentTab = ( DealerFlags.GetCurrentStockLevelsAccessType() != STOCKLEVELS_ACCESS_NONE );
		info.m_bStockLevelTab = FALSE;
		info.m_bApparentTab = FALSE;

		CPropertySheetPluStockman propSheet ( info, m_pParent );
		
		if ( propSheet.DoModal() == IDOK )
		{
			bResult = TRUE;
			propSheet.SaveRecord();
		}
		else
			propSheet.UndoChanges();
	}

	return bResult;
}

/**********************************************************************/

void CSPOSOfferGroupPluEditStockman::SaveChanges()
{
	CDWordArray dw;
	dw.Add(DataManager.Plu.GetSize());
	dw.Add(DataManager.Stock.GetSize());

	SPOSKeyboardManager.SetCanDeleteBarcodeFlag();

	if ((DataManager.Plu.IsModified() == TRUE) || (DataManager.Stock.IsModified() == TRUE) || (DataManager.ResourceWrapperPlu.IsModified() == TRUE) || (DataManager.Barcode.IsModified() == TRUE) || (DataManager.ListKeyImageWrapperPlu.IsModified() == TRUE))
	{
		DataManager.StockPlu.Invalidate();

		CWaitCursor wait;

		CDataManagerInfo info;
		if (DataManager.WriteStock(info, TRUE, TRUE) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else if (DataManager.WriteBarcode(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else
		{
			DataManager.Plu.UpdateSalesManCostPrice();
			if (DataManager.WritePlu(info) == FALSE)
			{
				Prompter.WriteError(info);
			}
			else
			{
				DataManager.ResourceWrapperPlu.SaveDatabaseChanges();
				DataManager.ListKeyImageWrapperPlu.SaveDatabaseChanges();
				SystemMapFull.WriteSystem(FALSE);
				CPosTrayTokenFiles::CreateSystemUpdateFile();
				CCustomExportManagerPlu CustomExport(DealerFlags.GetCurrentStockLevelsAccessType() != STOCKLEVELS_ACCESS_NONE);
				CKeyboardHelpers::RemovePluFile(DataManager.GetActiveDbIdx());
			}
		}

		//COpeningStockAuditer OpeningStockAuditer;

		MessageLogger.LogSystem(14, 1, FALSE, dw);

		SPOSKeyboardManager.SetCanDeleteStockFlag();
		SPOSKeyboardManager.SetCanDoChangesTaskFlag();
	}
}

/**********************************************************************/

void CSPOSOfferGroupPluEditStockman::Finalise()
{
	CDataManagerInfo info;
	DataManager.CloseCategory(info);
	DataManager.CloseSupplier(info);

	for (int n = 1; n < MAX_ITEM_CUSTOM_FIELDS; n++)
	{
		DataManager.CloseCustomFieldValue(n, info);
	}

	DataManagerNonDb.Allergen.RemoveAllergenZero();
	DataManagerNonDb.CloseAllergen( info );
	DataManagerNonDb.CloseOPGroupSystem( info );
	DataManagerNonDb.CloseOPGroupDatabase( info );
	DataManagerNonDb.CloseOPNameSystem(info);
	DataManagerNonDb.CloseOPNameDatabase(info);
	DataManagerNonDb.CloseCustomFieldNames(info);
}

/**********************************************************************/

void CSPOSOfferGroupPluEditStockman::LoadReadOnlyDatabase()
{
	CDataManagerInfo info;
	DataManager.ClosePlu(info);
	DataManager.CloseBarcode(info);
	DataManager.CloseStock(info);
	DataManager.OpenPlu(DB_READONLY, info);
	DataManager.OpenBarcode(DB_READONLY, info);
	DataManager.OpenStock(DB_READONLY, info);
}

/**********************************************************************/

