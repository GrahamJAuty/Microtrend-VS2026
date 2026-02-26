/**********************************************************************/
#include "..\CommonEcrStock\BarcodeAutoImporter.h"
#include "..\CommonEcrStock\CommsExportBase.h"
#include "..\CommonEcrStock\CustomExportManager.h"
#include "..\CommonEcrStock\KbdListNameArray.h"
#include "..\CommonEcrStock\KeyboardHelpers.h"
#include "..\CommonEcrStock\PluAutoImporterCSV.h"
#include "..\CommonEcrStock\PluAutoImporterEOD.h"
#include "..\CommonEcrStock\PluAutoImporterEODPriceSet.h"
/**********************************************************************/
#include "..\SPosStockManager5\OpeningStockAuditer.h"
#include "..\SPosStockManager5\PropertySheetPluStock.h"
#include "..\SPosStockManager5\PropertySheetPluStockman.h"
#include "..\SPosStockManager5\StockOptionsIni.h"
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "EditStockTypes.h"
#include "FilterTypes.h"
#include "MessageLogger.h"
#include "PluNumberFullStockDlg.h"
#include "PosTrayTokenFiles.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SPOSKeyboardPluEditStockman.h"
/**********************************************************************/

CSPOSKeyboardPluEditStockman::CSPOSKeyboardPluEditStockman( __int64 nPluNo, int nMode, int& nNewPluDeptNo, const char* szImageFilename, CWnd* pParent ) : m_FilterArray( FILTERTYPE_DATABASE ), m_nNewPluDeptNo( nNewPluDeptNo )
{
	m_nPluNo = nPluNo;
	m_nMode = nMode;
	m_strImageFilename = szImageFilename;
	m_pParent = pParent;
	DataManager.ResourceWrapperPlu.Reset();
	DataManager.ListKeyImageWrapperPlu.Reset();

	bool bDummy;
	CCommsExportBase::CheckPluNo( SPOS_V4, m_nPluNo, m_nSharpPosPluNo, bDummy );
}

/**********************************************************************/

CSPOSKeyboardPluEditStockman::~CSPOSKeyboardPluEditStockman(void)
{
	DataManager.ResourceWrapperPlu.Reset();
	DataManager.ListKeyImageWrapperPlu.Reset();
}

/**********************************************************************/

bool CSPOSKeyboardPluEditStockman::LoadDatabase()
{
	bool bResult = FALSE;

	switch( m_nMode )
	{
	case SPOSKEYBOARD_PLUEDIT_PRICESET1:
		bResult = LoadDatabasePriceSet(1);
		break;

	case SPOSKEYBOARD_PLUEDIT_PRICESET2:
		bResult = LoadDatabasePriceSet(2);
		break;

	case SPOSKEYBOARD_PLUEDIT_PRICESET3:
		bResult = LoadDatabasePriceSet(3);
		break;

	case SPOSKEYBOARD_PLUEDIT_MASTER:
	case SPOSKEYBOARD_PLUEDIT_CREATE:
	case SPOSKEYBOARD_PLUEDIT_LINK:
	default:
		bResult = LoadDatabaseMaster();
		break;
	}

	return bResult;
}

/**********************************************************************/

bool CSPOSKeyboardPluEditStockman::LoadDatabaseMaster()
{
	int nDbIdx = SPOSKeyboardManager.GetDbIdx();

	PluAutoImporterCSV.ImportPluExternal( nDbIdx );
	PluAutoImporterEOD.ImportPluExternal( nDbIdx );
	BarcodeAutoImporter.ImportBarcodeExternal( nDbIdx );
	
	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_PLU );
	arrayReadWrite.Add( NODE_BARCODE );
	arrayReadWrite.Add( NODE_STOCK );

	CDataManagerInfo info;
	bool bResult = FALSE;

	KbdListNameArray.SetFakeReadFlag();
	KbdListNameArray.Read(nDbIdx);
	KbdListNameArray.ClearFakeReadFlag();
	
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, info, FALSE ) == FALSE )
	{
		Prompter.ShareError( info );
		bResult = FALSE;
	}
	else 
	{
		bResult = TRUE;
	}

	DataManagerNonDb.OpenAllergen(DB_READONLY, info);
	DataManagerNonDb.Allergen.AddAllergenZero();
	DataManagerNonDb.OpenOPGroupSystem(DB_READONLY, info);
	DataManagerNonDb.OpenOPGroupDatabase(nDbIdx, DB_READONLY, info);
	DataManagerNonDb.OpenOPNameSystem(DB_READONLY, info);
	DataManagerNonDb.OpenOPNameDatabase(nDbIdx, DB_READONLY, info);
	DataManagerNonDb.OpenCustomFieldNames(DB_READONLY, info);

	return bResult;
}

/**********************************************************************/

bool CSPOSKeyboardPluEditStockman::LoadDatabasePriceSet( int nPriceSet )
{
	int nLocIdx = SPOSKeyboardManager.GetLocIdx();
	int nDbIdx = SPOSKeyboardManager.GetDbIdx();
	
	PluAutoImporterCSV.ImportPluExternal( nDbIdx );
	PluAutoImporterEOD.ImportPluExternal( nDbIdx );
	PluAutoImporterEODPriceSet.ImportPluExternal( nLocIdx );
	BarcodeAutoImporter.ImportBarcodeExternal( nDbIdx );

	bool bReadOnly = FALSE;
	bool bContinue = TRUE;

	PriceSetItemManager.SelectPriceSet ( nLocIdx, nPriceSet );

	if ( m_PriceSetLockFile.OpenLock( PriceSetItemManager.GetLockFilename() ) == FALSE )
	{
		CDataManagerInfo info;
		info.m_nNodeType = NODE_PRICESET1;
		Prompter.ShareError( info );
		return FALSE;
	}

	CDataManagerInfo info;	
	DataManager.SetPluPriceSetModeFlag(TRUE);
	DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
	DataManager.SetPluPriceSetModeFlag(FALSE);

	CWaitCursor wait;
	PriceSetItemManager.SelectPriceSet ( nLocIdx, nPriceSet );
	PriceSetItemManager.ReadPriceSetFile( FALSE, DataManager.Plu );
	
	DataManager.Plu.SetModifiedFlag ( FALSE );

	KbdListNameArray.SetFakeReadFlag();
	KbdListNameArray.Read(nDbIdx, nLocIdx, nPriceSet);
	KbdListNameArray.ClearFakeReadFlag();
		
	DataManagerNonDb.OpenAllergen( DB_READONLY, info );
	DataManagerNonDb.Allergen.AddAllergenZero();
	DataManagerNonDb.OpenOPGroupSystem( DB_READONLY, info );
	DataManagerNonDb.OpenOPGroupDatabase( nDbIdx, DB_READONLY, info );
	DataManagerNonDb.OpenOPGroupLocation( nLocIdx, DB_READONLY, info );
	DataManagerNonDb.OpenOPNameSystem(DB_READONLY, info);
	DataManagerNonDb.OpenOPNameDatabase(nDbIdx, DB_READONLY, info);
	DataManagerNonDb.OpenOPNameLocation(nLocIdx, DB_READONLY, info);
	DataManagerNonDb.OpenCustomFieldNames( DB_READONLY, info );

	return TRUE;
}

/**********************************************************************/

bool CSPOSKeyboardPluEditStockman::EditPlu()
{
	bool bResult = FALSE;

	switch( m_nMode )
	{
	case SPOSKEYBOARD_PLUEDIT_CREATE:
		bResult = CreateNewPlu();
		break;

	case SPOSKEYBOARD_PLUEDIT_LINK:
		bResult = LinkPlu();
		break;

	case SPOSKEYBOARD_PLUEDIT_PRICESET1:
	case SPOSKEYBOARD_PLUEDIT_PRICESET2:
	case SPOSKEYBOARD_PLUEDIT_PRICESET3:
	case SPOSKEYBOARD_PLUEDIT_MASTER:
	default:
		bResult = EditExistingPlu();
		break;
	}

	return bResult;
}

/**********************************************************************/

bool CSPOSKeyboardPluEditStockman::EditExistingPlu()
{
	bool bResult = FALSE;

	int nPluIdx = 0;
	if (DataManager.Plu.FindPluByNumber(m_nPluNo, nPluIdx) == TRUE)
	{
		bResult = EditPluInternal(nPluIdx, FALSE, TRUE);
	}

	return bResult;
}

/**********************************************************************/

bool CSPOSKeyboardPluEditStockman::EditPluInternal(int nPluIdx, bool bNewStock, bool bAllowSimple)
{
	bool bResult = FALSE;

	m_FilterArray.SetArrayIdx(nPluIdx);

	int nStockIdx;
	bool bAllowSpin;
	bool bSimpleEditMode;

	if (StockOptions.GetPluSimplifiedEditingFlag() == FALSE || PriceSetItemManager.IsPluMaster() == FALSE)
	{
		bAllowSpin = TRUE;
		bSimpleEditMode = FALSE;
	}
	else if (bAllowSimple == FALSE)
	{
		bAllowSpin = FALSE;
		bSimpleEditMode = FALSE;
	}
	else
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt(m_FilterArray.GetPluItemIndex(), PluRecord);

		if (DataManager.Plu.IsRecipe(PluRecord.GetPluNoInt()))
		{
			bAllowSpin = FALSE;
			bSimpleEditMode = FALSE;
		}
		else
		{
			CString strStockCode;
			DataManager.Plu.GetPluStockCode(PluRecord.GetPluNoInt(), strStockCode, FALSE);

			if (strStockCode == "")
			{
				bAllowSpin = FALSE;
				bSimpleEditMode = FALSE;
			}
			else
			{
				bAllowSpin = DataManager.Stock.FindStockItemByCode(strStockCode, nStockIdx);
				bSimpleEditMode = bAllowSpin;
			}
		}
	}

	//IF WE WANT TO DISPLAY THE APPARENT TAB OR THE STOCK LEVEL TAB WITH CORRECT VALUES
	//THEN WE NEED TO LOAD STOCK LEVELS INTO MEMORY. THIS IS NOT DONE HERE AS IT IS NOT
	//NECESSARY FOR THE PLU EDIT FUNCTIONALITY. IF IT IS REQUIRED THEN THE STOCK LEVELS
	//SHOULD BE LOADED INTO MEMORY IN THE LOAD DATABASE FUNCTIONS.

	if (bSimpleEditMode == TRUE)
	{
		CPropertySheetPluStockInfo info;
		info.m_bEnableSpin = TRUE;
		info.m_wSpinID = IDC_SPIN;
		info.m_pPluFilter = &m_FilterArray;
		info.m_nStockIdx = nStockIdx;
		info.m_bNewStock = bNewStock;
		info.m_nDefaultSuppNo = 0;
		info.m_bReadOnly = FALSE;
		//info.m_bStockLevelTab = (DealerFlags.GetCurrentStockLevelsAccessType() == STOCKLEVELS_ACCESS_READWRITE);
		//info.m_bApparentTab = (DealerFlags.GetCurrentStockLevelsAccessType() != STOCKLEVELS_ACCESS_NONE);
		info.m_bApparentTab = FALSE;
		info.m_bStockLevelTab = FALSE;

		CPropertySheetPluStock propSheet(info, m_pParent);
		propSheet.EnableStackedTabs(FALSE);

		if (propSheet.DoModal() == IDOK)
		{
			propSheet.SavePluRecord();
			bResult = TRUE;
		}
		else
		{
			propSheet.UndoChanges();
		}
	}
	else
	{
		int nEditStock = (PriceSetItemManager.IsPluMaster() == TRUE) ? EDIT_STOCK_FULL : EDIT_STOCK_NONE;

		CPropertySheetPluStockmanInfo info;
		info.m_bEnableSpin = bAllowSpin;
		info.m_wSpinID = IDC_SPIN;
		info.m_pPluFilter = &m_FilterArray;
		info.m_nEditMode = nEditStock;
		info.m_bNewStock = bNewStock;
		info.m_bReadOnly = FALSE;
		//info.m_bStockLevelTab = (DealerFlags.GetCurrentStockLevelsAccessType() == STOCKLEVELS_ACCESS_READWRITE);
		//info.m_bApparentTab = (DealerFlags.GetCurrentStockLevelsAccessType() != STOCKLEVELS_ACCESS_NONE);
		info.m_bApparentTab = FALSE;
		info.m_bStockLevelTab = FALSE;

		CPropertySheetPluStockman propSheet(info, m_pParent);

		if (propSheet.DoModal() == IDOK)
		{
			bResult = TRUE;
			propSheet.SaveRecord();
		}
		else
		{
			propSheet.UndoChanges();
		}
	}

	return bResult;
}

/**********************************************************************/

bool CSPOSKeyboardPluEditStockman::LinkPlu()
{
	int nResult, nPluIdx;
	CPluFilterArray LocalFilterArray( FILTERTYPE_DATABASE );
	::ShowPluFilterDlg( LocalFilterArray, m_pParent, nResult, nPluIdx );

	bool bResult = FALSE;
	
	if  ( ( IDOK == nResult ) && ( nPluIdx >= 0 ) )
	{
		bool bModifiable;
		CCommsExportBase::CheckPluNo( SPOS_V4, DataManager.Plu.GetPluNoInt( nPluIdx ), m_nSharpPosPluNo, bModifiable );
		bResult = EditPluInternal( nPluIdx, FALSE, TRUE );
	}

	return bResult;
}

/**********************************************************************/

bool CSPOSKeyboardPluEditStockman::CreateNewPlu()
{
	CString strPluNo = "";
	::FormatPluNo( m_nPluNo, strPluNo, FALSE, FALSE );

	CPluAddOrCopyInfo PluInfo;
	PluInfo.m_nNewPluSuppNo = 0;
	PluInfo.m_nNewPluDeptNo = m_nNewPluDeptNo;
	PluInfo.m_strNewPluNo = strPluNo;

	CPluNumberFullStockDlg dlg ( TRUE, PluInfo, m_pParent );
	
	bool bResult = FALSE;
	
	while ( dlg.DoModal() == IDOK )
	{
		int nSuppNo = PluInfo.m_nNewPluSuppNo;

		CString strEnteredPluNo = PluInfo.m_strNewPluNo;
		CString strConvertedPluNo = BarcodePluNoTable.ConvertPlu ( strEnteredPluNo );
		__int64 nConvertedPluNo = _atoi64( strConvertedPluNo );

		CString strNewStockCode = PluInfo.m_strNewPluStockCode;;
		
		int nIndex;
		if ( DataManager.Plu.FindPluByNumber ( nConvertedPluNo, nIndex ) == FALSE )
		{
			int nMaxBaseLen = SysInfo.GetMaxBasePluLen();
			if ( nMaxBaseLen != 0 )
			{
				CString strTestPluNo;
				strTestPluNo.Format( "%I64d", nConvertedPluNo );

				if ( strTestPluNo.GetLength() == nMaxBaseLen + 1 )
				{
					CString strMessage;
					strMessage.Format( "%s is not configured to allow %d digit PLU items.", 
						AfxGetAppName(),
						nMaxBaseLen + 1 );
					
					Prompter.ContactDealer( strMessage );
					continue;
				}
			}

			m_nNewPluDeptNo = PluInfo.m_nNewPluDeptNo;

			CPluCSVRecord NewRecord;
			NewRecord.SetPluNo( nConvertedPluNo );

			if (DataManager.Plu.CopyDefaultPlu(m_nNewPluDeptNo, NewRecord) == FALSE)
			{
				NewRecord.SetDefaultText();
			}

			NewRecord.SetBaseDeptNo( m_nNewPluDeptNo );
			NewRecord.SetModifiedDateTimePlu();

			NewRecord.SetImageFilename( m_strImageFilename );
			
			if (m_FilterArray.GetDepartmentFilter() != DEPARTMENTSET_ALL)
			{
				NewRecord.SetBaseDeptNo(m_FilterArray.GetDepartmentFilter());
			}

			bool bNewStockCode = FALSE;

			if ( strNewStockCode != "" )
			{
				::TrimSpaces ( strNewStockCode, FALSE );
					
				int nStockIdx;
				if ( DataManager.Stock.FindStockItemByCode( strNewStockCode, nStockIdx ) == FALSE )
				{
					bNewStockCode = TRUE;

					CStockCSVRecord StockRecord;
					StockRecord.SetStockCode ( strNewStockCode );
					StockRecord.SetDescription ( "Plu" );

					if (m_FilterArray.GetDepartmentFilter() != DEPARTMENTSET_ALL)
					{
						StockRecord.SetCategory(m_FilterArray.GetDepartmentFilter());
					}

					if ( PluInfo.m_nNewPluSuppNo != 0 )
					{
						StockRecord.SetSuppNo( 0, PluInfo.m_nNewPluSuppNo );
						StockRecord.TidySuppliers( StockOptions.GetStockMaxSuppliersPerItem() );
					}

					DataManager.Stock.InsertRecord ( StockRecord, nStockIdx );
				}
			}
			
			int nInsertionPoint = m_FilterArray.InsertPluRecord ( NewRecord, strNewStockCode, TRUE );
			EditPluInternal( nInsertionPoint, bNewStockCode, TRUE );

			m_nSharpPosPluNo = 0;
			bool bModifiable = FALSE;
			CCommsExportBase::CheckPluNo( SPOS_V4, NewRecord.GetPluNoInt(), m_nSharpPosPluNo, bModifiable );
			
			bResult = TRUE;
			break;
		}
		else
		{
			CString strMsg;
			strMsg.Format ( "Plu number %s already exists in your database\n\nDo you wish to search for the next free number", 
				(const char*) strConvertedPluNo );

			if (Prompter.YesNo(strMsg, TRUE) != IDYES)
			{
				continue;
			}

			int nProgress = 0;
			bool bGotFreeNumber = FALSE;

			StatusProgress.Lock( TRUE, "Finding next PLU number" );
			do
			{
				StatusProgress.SetPos ( nProgress++, DataManager.Plu.GetSize() );

				if (GetNextPluNo(strEnteredPluNo) == FALSE)
				{
					break;
				}

				__int64 nConvertedPluNo = _atoi64(BarcodePluNoTable.ConvertPlu( strEnteredPluNo ) );
				if ( DataManager.Plu.FindPluByNumber ( nConvertedPluNo, nIndex ) == FALSE )
				{
					bGotFreeNumber = TRUE;
					break;
				}				
			}
			while ( TRUE );
			StatusProgress.Unlock();

			if (bGotFreeNumber == FALSE)
			{
				Prompter.Error("Unable to find another free Plu number");
			}
			else
			{
				PluInfo.m_strNewPluNo = strEnteredPluNo;
			}
		}
	}

	return bResult;
}

/**********************************************************************/

void CSPOSKeyboardPluEditStockman::SaveChanges()
{
	switch( m_nMode )
	{
	case SPOSKEYBOARD_PLUEDIT_PRICESET1:
	case SPOSKEYBOARD_PLUEDIT_PRICESET2:
	case SPOSKEYBOARD_PLUEDIT_PRICESET3:
		SaveChangesPriceSet();
		break;

	case SPOSKEYBOARD_PLUEDIT_MASTER:
	case SPOSKEYBOARD_PLUEDIT_CREATE:
	case SPOSKEYBOARD_PLUEDIT_LINK:
	default:
		SaveChangesMaster();
		break;
	}
}

/**********************************************************************/

void CSPOSKeyboardPluEditStockman::SaveChangesMaster()
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

		COpeningStockAuditer OpeningStockAuditer;

		MessageLogger.LogSystem(14, 1, FALSE, dw);

		SPOSKeyboardManager.SetCanDeleteStockFlag();
		SPOSKeyboardManager.SetCanDoChangesTaskFlag();
	}
}

/**********************************************************************/

void CSPOSKeyboardPluEditStockman::SaveChangesPriceSet()
{
	CDWordArray dw;
	dw.Add(DataManager.Plu.GetSize());
	dw.Add(DataManager.Stock.GetSize());

	SPOSKeyboardManager.SetCanDeleteBarcodeFlag();

	bool bPluModified = DataManager.Plu.IsModified();

	if (FALSE == PriceSetItemManager.IsPluMaster())
	{
		bPluModified |= PriceSetItemManager.GetPluStatusModifiedFlag();
	}

	if ((TRUE == bPluModified) || (DataManager.Stock.IsModified() == TRUE) || (DataManager.ResourceWrapperPlu.IsModified() == TRUE) || (DataManager.Barcode.IsModified() == TRUE) || (DataManager.ListKeyImageWrapperPlu.IsModified() == TRUE))
	{
		DataManager.StockPlu.Invalidate();

		PriceSetItemManager.WritePriceSetFile();
		DataManager.ResourceWrapperPlu.SaveDatabaseChanges();
		DataManager.ListKeyImageWrapperPlu.SaveDatabaseChanges();
		CPriceSetExportInfo info;
		PriceSetItemManager.GetPriceSetExportInfo(info);
		CCustomExportManagerPriceSet CustomExport(info);
		CKeyboardHelpers::RemovePluFile(DataManager.GetActiveDbIdx());

		COpeningStockAuditer OpeningStockAuditer;

		MessageLogger.LogSystem(14, 1, FALSE, dw);

		SPOSKeyboardManager.SetCanDeleteStockFlag();
		SPOSKeyboardManager.SetCanDoChangesTaskFlag();
	}
}

/**********************************************************************/

void CSPOSKeyboardPluEditStockman::Finalise()
{
	switch( m_nMode )
	{
	case SPOSKEYBOARD_PLUEDIT_PRICESET1:
	case SPOSKEYBOARD_PLUEDIT_PRICESET2:
	case SPOSKEYBOARD_PLUEDIT_PRICESET3:
		FinalisePriceSet();
		break;

	case SPOSKEYBOARD_PLUEDIT_MASTER:
	case SPOSKEYBOARD_PLUEDIT_CREATE:
	case SPOSKEYBOARD_PLUEDIT_LINK:
	default:
		FinaliseMaster();
		break;
	}
}

/**********************************************************************/

void CSPOSKeyboardPluEditStockman::FinaliseMaster()
{
	CDataManagerInfo info;
	DataManagerNonDb.Allergen.RemoveAllergenZero();
	DataManagerNonDb.CloseAllergen(info);
	DataManagerNonDb.CloseOPGroupSystem(info);
	DataManagerNonDb.CloseOPGroupDatabase(info);
	DataManagerNonDb.CloseOPNameSystem(info);
	DataManagerNonDb.CloseOPNameDatabase(info);
	DataManagerNonDb.CloseCustomFieldNames(info);
}

/**********************************************************************/

void CSPOSKeyboardPluEditStockman::FinalisePriceSet()
{
	CDataManagerInfo info;
	DataManagerNonDb.Allergen.RemoveAllergenZero();
	DataManagerNonDb.CloseAllergen(info);
	DataManagerNonDb.CloseOPGroupSystem(info);
	DataManagerNonDb.CloseOPGroupDatabase(info);
	DataManagerNonDb.CloseOPGroupLocation(info);
	DataManagerNonDb.CloseOPNameSystem(info);
	DataManagerNonDb.CloseOPNameDatabase(info);
	DataManagerNonDb.CloseOPNameLocation(info);
	DataManagerNonDb.CloseCustomFieldNames(info);

	m_PriceSetLockFile.Close();
}

/**********************************************************************/

void CSPOSKeyboardPluEditStockman::LoadReadOnlyDatabase()
{
	CDataManagerInfo dbInfo;
	DataManager.OpenDatabaseReadOnly( SPOSKeyboardManager.GetDbIdx(), dbInfo, FALSE );
	SPOSKeyboardManager.GetPluDatabase() -> ReRead();
}

/**********************************************************************/

bool CSPOSKeyboardPluEditStockman::GetNextPluNo(CString& strPluNo)
{
	int nLength = strPluNo.GetLength();

	CString strNewPluNo = strPluNo;
	::TrimSpaces(strNewPluNo, TRUE);

	if (strNewPluNo == "")
	{
		return FALSE;
	}

	__int64 nNewPluNo = _atoi64(strNewPluNo) + 1;
	strNewPluNo.Format("%I64d", nNewPluNo);

	::AddLeading(strNewPluNo, nLength, '0');
	if (strNewPluNo.GetLength() > Super.MaxPluLen())
	{
		return FALSE;
	}

	strPluNo = strNewPluNo;
	return TRUE;
}

/**********************************************************************/
