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
#include "Append.h"
#include "BarcodePluNoTable.h"
#include "CommsHandshaker.h"
#include "FileRemover.h"
#include "FilterTypes.h"
#include "MessageLogger.h"
#include "PluNumberEcrStockDlg.h"
#include "PosTrayTokenFiles.h"
#include "PropertySheetPluEcrman.h"
#include "StatusProgress.h"
#include "SysInfo.h"
#include "SystemMap.h"
/**********************************************************************/
#include "SPOSKeyboardPluEditEcrman.h"
/**********************************************************************/

CSPOSKeyboardPluEditEcrman::CSPOSKeyboardPluEditEcrman( __int64 nPluNo, int nMode, int& nNewPluDeptNo, const char* szImageFilename, CWnd* pParent ) : m_FilterArray( FILTERTYPE_DATABASE ), m_nNewPluDeptNo( nNewPluDeptNo )
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

CSPOSKeyboardPluEditEcrman::~CSPOSKeyboardPluEditEcrman(void)
{
	DataManager.ResourceWrapperPlu.Reset();
	DataManager.ListKeyImageWrapperPlu.Reset();
}

/**********************************************************************/

bool CSPOSKeyboardPluEditEcrman::LoadDatabase()
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

bool CSPOSKeyboardPluEditEcrman::LoadDatabaseMaster()
{
	int nDbIdx = SPOSKeyboardManager.GetDbIdx();

	if (EcrmanOptions.GetFeaturesPluStockFlag())
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

	return bResult;
}

/**********************************************************************/

bool CSPOSKeyboardPluEditEcrman::LoadDatabasePriceSet(int nPriceSet)
{
	int nLocIdx = SPOSKeyboardManager.GetLocIdx();
	int nDbIdx = SPOSKeyboardManager.GetDbIdx();

	if (EcrmanOptions.GetFeaturesPluStockFlag())
	{
		CommsHandshaker.ProcessSales();
	}

	PluAutoImporterCSV.ImportPluExternal(nDbIdx);
	PluAutoImporterEOD.ImportPluExternal(nDbIdx);
	PluAutoImporterEODPriceSet.ImportPluExternal(nLocIdx);
	BarcodeAutoImporter.ImportBarcodeExternal(nDbIdx);

	bool bReadOnly = FALSE;
	bool bContinue = TRUE;

	PriceSetItemManager.SelectPriceSet(nLocIdx, nPriceSet);

	if (m_PriceSetLockFile.OpenLock(PriceSetItemManager.GetLockFilename()) == FALSE)
	{
		CDataManagerInfo info;
		info.m_nNodeType = NODE_PRICESET1;
		Prompter.ShareError(info);
		return FALSE;
	}

	CDataManagerInfo info;
	DataManager.SetPluPriceSetModeFlag(TRUE);
	DataManager.OpenDatabaseReadOnly(nDbIdx, info, FALSE);
	DataManager.SetPluPriceSetModeFlag(FALSE);

	CWaitCursor wait;
	PriceSetItemManager.SelectPriceSet(nLocIdx, nPriceSet);
	PriceSetItemManager.ReadPriceSetFile(FALSE, DataManager.Plu);

	DataManager.Plu.SetModifiedFlag(FALSE);

	KbdListNameArray.SetFakeReadFlag();
	KbdListNameArray.Read(nDbIdx, nLocIdx, nPriceSet);
	KbdListNameArray.ClearFakeReadFlag();

	DataManagerNonDb.OpenAllergen(DB_READONLY, info);
	DataManagerNonDb.Allergen.AddAllergenZero();
	DataManagerNonDb.OpenOPGroupSystem(DB_READONLY, info);
	DataManagerNonDb.OpenOPGroupDatabase(nDbIdx, DB_READONLY, info);
	DataManagerNonDb.OpenOPGroupLocation(nLocIdx, DB_READONLY, info);
	DataManagerNonDb.OpenOPNameSystem(DB_READONLY, info);
	DataManagerNonDb.OpenOPNameDatabase(nDbIdx, DB_READONLY, info);
	DataManagerNonDb.OpenOPNameLocation(nLocIdx, DB_READONLY, info);

	return TRUE;
}

/**********************************************************************/

bool CSPOSKeyboardPluEditEcrman::EditPlu()
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

bool CSPOSKeyboardPluEditEcrman::EditExistingPlu()
{
	bool bResult = FALSE;

	int nPluIdx;
	if (DataManager.Plu.FindPluByNumber(m_nPluNo, nPluIdx) == TRUE)
	{
		bResult = EditPluInternal(nPluIdx);
	}

	return bResult;
}

/**********************************************************************/

bool CSPOSKeyboardPluEditEcrman::EditPluInternal(int nPluIdx)
{
	bool bResult = FALSE;

	m_FilterArray.SetArrayIdx(nPluIdx);

	CPropertySheetPluEcrman propSheet(IDC_SPIN, &m_FilterArray, FALSE, m_pParent);

	if (propSheet.DoModal() == IDOK)
	{
		bResult = TRUE;
		propSheet.SaveRecord();
	}
	else
	{
		propSheet.UndoChanges();
	}

	return bResult;
}

/**********************************************************************/

bool CSPOSKeyboardPluEditEcrman::CreateNewPlu()
{
	bool bResult = FALSE;

	CString strPluNo = "";
	::FormatPluNo( m_nPluNo, strPluNo, FALSE, FALSE );
		
	CPluAddOrCopyInfo PluInfo;
	PluInfo.m_nNewPluSuppNo = 0;
	PluInfo.m_nNewPluDeptNo = m_nNewPluDeptNo;
	PluInfo.m_strNewPluNo = strPluNo;
	
	CPluNumberEcrStockDlg dlg( PluInfo, m_pParent );
	
	while ( dlg.DoModalWrapper() == IDOK )
	{
		CString strEnteredPluNo = PluInfo.m_strNewPluNo;
		CString strConvertedPluNo = BarcodePluNoTable.ConvertPlu ( strEnteredPluNo );
		__int64 nConvertedPluNo = _atoi64( strConvertedPluNo );

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
			NewRecord.SetPluNo ( nConvertedPluNo );

			if (DataManager.Plu.CopyDefaultPlu(m_nNewPluDeptNo, NewRecord) == FALSE)
			{
				NewRecord.SetDefaultText();
			}

			NewRecord.SetBaseDeptNo( m_nNewPluDeptNo );
			NewRecord.SetModifiedDateTimePlu();
			NewRecord.SetSupplierNo( PluInfo.m_nNewPluSuppNo );
			NewRecord.SetImageFilename( m_strImageFilename );
		
			//if ( m_FilterArray.GetDepartmentFilter() != DEPARTMENTSET_ALL )
			//	NewRecord.SetDeptNo ( m_FilterArray.GetDepartmentFilter() );

			int nInsertionPoint = m_FilterArray.InsertPluRecord ( NewRecord, TRUE );
			EditPluInternal ( nInsertionPoint );

			m_nSharpPosPluNo = 0;
			bool bModifiable = FALSE;
			CCommsExportBase::CheckPluNo( SPOS_V4, NewRecord.GetPluNoInt(), m_nSharpPosPluNo, bModifiable );
			
			bResult = TRUE;
			break;
		}
		else
		{
			CString strMsg = "";
			strMsg.Format("Plu number %s already exists in your database\n\nDo you wish to search for the next free number",
				(const char*)strConvertedPluNo);

			if (Prompter.YesNo(strMsg, TRUE) != IDYES)
			{
				continue;
			}

			int nProgress = 0;
			bool bGotFreeNumber = FALSE;

			StatusProgress.Lock(TRUE, "Finding next PLU number");
			do
			{
				StatusProgress.SetPos(nProgress++, DataManager.Plu.GetSize());

				if (GetNextPluNo(strEnteredPluNo) == FALSE)
				{
					break;
				}

				__int64 nConvertedPluNo = _atoi64(BarcodePluNoTable.ConvertPlu(strEnteredPluNo));
				if (DataManager.Plu.FindPluByNumber(nConvertedPluNo, nIndex) == FALSE)
				{
					bGotFreeNumber = TRUE;
					break;
				}
			} 
			while (TRUE);

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

bool CSPOSKeyboardPluEditEcrman::LinkPlu()
{
	int nResult, nPluIdx;
	CPluFilterArray LocalFilterArray( FILTERTYPE_DATABASE );
	::ShowPluFilterDlg( LocalFilterArray, m_pParent, nResult, nPluIdx );

	bool bResult = FALSE;
	
	if  ( ( IDOK == nResult ) && ( nPluIdx >= 0 ) )
	{
		bool bModifiable;
		CCommsExportBase::CheckPluNo( SPOS_V4, DataManager.Plu.GetPluNoInt( nPluIdx ), m_nSharpPosPluNo, bModifiable );
		bResult = EditPluInternal( nPluIdx );
	}

	return bResult;
}

/**********************************************************************/

void CSPOSKeyboardPluEditEcrman::SaveChanges()
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

void CSPOSKeyboardPluEditEcrman::SaveChangesMaster()
{
	CDWordArray dw;
	dw.Add(DataManager.Plu.GetSize());

	SPOSKeyboardManager.SetCanDeleteBarcodeFlag();

	if ((DataManager.Plu.IsModified() == TRUE) || (DataManager.Barcode.IsModified() == TRUE) || (DataManager.ResourceWrapperPlu.IsModified() == TRUE) || (DataManager.ListKeyImageWrapperPlu.IsModified() == TRUE))
	{
		CWaitCursor wait;

		CDataManagerInfo info;
		if (DataManager.WritePlu(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else if (DataManager.WriteBarcode(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else
		{
			DataManager.ResourceWrapperPlu.SaveDatabaseChanges();
			DataManager.ListKeyImageWrapperPlu.SaveDatabaseChanges();

			SystemMapFull.WriteSystem(FALSE);
			CPosTrayTokenFiles::CreateSystemUpdateFile();
			CCustomExportManagerPlu CustomExport;
			CKeyboardHelpers::RemovePluFile(DataManager.GetActiveDbIdx());

			if (EcrmanOptions.GetFeaturesPluAuditFlag() == TRUE)
			{
				int nDbIdx = DataManager.GetActiveDbIdx();
				CString strSource = dbDatabase.GetFilePathDataPluStockAuditTemp(nDbIdx);
				CString strDest = dbDatabase.GetFilePathDataPluStockAudit(nDbIdx);
				::Append(strSource, strDest);
				CFileRemover FileRemover(strSource);
			}
		}

		MessageLogger.LogSystem(14, 1, FALSE, dw);
		SPOSKeyboardManager.SetCanDoChangesTaskFlag();
	}
}

/**********************************************************************/

void CSPOSKeyboardPluEditEcrman::SaveChangesPriceSet()
{
	CDWordArray dw;
	dw.Add( DataManager.Plu.GetSize() );
	
	SPOSKeyboardManager.SetCanDeleteBarcodeFlag();

	bool bPluModified = DataManager.Plu.IsModified();
	bPluModified |= PriceSetItemManager.GetPluStatusModifiedFlag();

	if ( ( TRUE == bPluModified ) || ( DataManager.Barcode.IsModified() == TRUE  ) || ( DataManager.ResourceWrapperPlu.IsModified() == TRUE ) || ( DataManager.ListKeyImageWrapperPlu.IsModified() == TRUE ) )
	{
		CWaitCursor wait;

		PriceSetItemManager.WritePriceSetFile();
		DataManager.ResourceWrapperPlu.SaveDatabaseChanges();
		DataManager.ListKeyImageWrapperPlu.SaveDatabaseChanges();
		CPriceSetExportInfo info;
		PriceSetItemManager.GetPriceSetExportInfo( info );
		CCustomExportManagerPriceSet CustomExport( info );
		CKeyboardHelpers::RemovePluFile ( DataManager.GetActiveDbIdx() );
			
		MessageLogger.LogSystem( 14, 1, FALSE, dw );
		SPOSKeyboardManager.SetCanDoChangesTaskFlag();
	}
}

/**********************************************************************/

void CSPOSKeyboardPluEditEcrman::Finalise()
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

void CSPOSKeyboardPluEditEcrman::FinaliseMaster()
{
	CDataManagerInfo info;
	DataManagerNonDb.Allergen.RemoveAllergenZero();
	DataManagerNonDb.CloseAllergen( info );
	DataManagerNonDb.CloseOPGroupSystem( info );
	DataManagerNonDb.CloseOPGroupDatabase( info );
	DataManagerNonDb.CloseOPNameSystem(info);
	DataManagerNonDb.CloseOPNameDatabase(info);
}

/**********************************************************************/

void CSPOSKeyboardPluEditEcrman::FinalisePriceSet()
{
	CDataManagerInfo info;
	DataManagerNonDb.Allergen.RemoveAllergenZero();
	DataManagerNonDb.CloseAllergen( info );
	DataManagerNonDb.CloseOPGroupSystem( info );
	DataManagerNonDb.CloseOPGroupDatabase( info );
	DataManagerNonDb.CloseOPGroupLocation( info );
	DataManagerNonDb.CloseOPNameSystem(info);
	DataManagerNonDb.CloseOPNameDatabase(info);
	DataManagerNonDb.CloseOPNameLocation(info);

	m_PriceSetLockFile.Close();
}

/**********************************************************************/

void CSPOSKeyboardPluEditEcrman::LoadReadOnlyDatabase()
{
	CDataManagerInfo dbInfo;
	DataManager.OpenDatabaseReadOnly( SPOSKeyboardManager.GetDbIdx(), dbInfo, FALSE );
	SPOSKeyboardManager.GetPluDatabase() -> ReRead();
}

/**********************************************************************/

bool CSPOSKeyboardPluEditEcrman::GetNextPluNo(CString& strPluNo)
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
