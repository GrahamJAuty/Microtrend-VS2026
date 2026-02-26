/**********************************************************************/
#include "..\CommonEcrStock\BarcodeAutoImporter.h"
#include "..\CommonEcrStock\CustomExportManager.h"
#include "..\CommonEcrStock\KbdListNameArray.h"
#include "..\CommonEcrStock\KeyboardHelpers.h"
#include "..\CommonEcrStock\PluAutoImporterCSV.h"
#include "..\CommonEcrStock\PluAutoImporterEOD.h"
/**********************************************************************/
#include "Append.h"
#include "CommsHandshaker.h"
#include "FileRemover.h"
#include "FilterTypes.h"
#include "MessageLogger.h"
#include "PosTrayTokenFiles.h"
#include "PropertySheetPluEcrman.h"
#include "SysInfo.h"
#include "SystemMap.h"
/**********************************************************************/
#include "SPOSOfferGroupPluEditEcrman.h"
/**********************************************************************/

CSPOSOfferGroupPluEditEcrman::CSPOSOfferGroupPluEditEcrman( __int64 nPluNo, CWnd* pParent ) : m_FilterArray( FILTERTYPE_DATABASE )
{
	m_nPluNo = nPluNo;
	m_pParent = pParent;
	DataManager.ResourceWrapperPlu.Reset();
	DataManager.ListKeyImageWrapperPlu.Reset();
}

/**********************************************************************/

CSPOSOfferGroupPluEditEcrman::~CSPOSOfferGroupPluEditEcrman(void)
{
	DataManager.ResourceWrapperPlu.Reset();
	DataManager.ListKeyImageWrapperPlu.Reset();
}

/**********************************************************************/

bool CSPOSOfferGroupPluEditEcrman::LoadDatabase()
{
	int nDbIdx = DataManager.GetActiveDbIdx();

	if (EcrmanOptions.GetFeaturesPluStockFlag())
	{
		CommsHandshaker.ProcessSales();
	}

	PluAutoImporterCSV.ImportPluExternal( nDbIdx );
	PluAutoImporterEOD.ImportPluExternal( nDbIdx );
	BarcodeAutoImporter.ImportBarcodeExternal( nDbIdx );
	
	KbdListNameArray.SetFakeReadFlag();
	KbdListNameArray.Read(nDbIdx);
	KbdListNameArray.ClearFakeReadFlag();

	CDataManagerInfo info;
	DataManager.ClosePlu(info);
	DataManager.CloseBarcode(info);
	
	if ( DataManager.OpenPlu( DB_READWRITE, info ) == FALSE )
	{ 
		Prompter.ShareError( info );
		return FALSE;
	}
	
	if (DataManager.OpenBarcode(DB_READWRITE, info) == FALSE)
	{
		Prompter.ShareError(info);
		return FALSE;
	}

	DataManagerNonDb.OpenAllergen(DB_READONLY, info);
	DataManagerNonDb.Allergen.AddAllergenZero();
	DataManagerNonDb.OpenOPGroupSystem(DB_READONLY, info);
	DataManagerNonDb.OpenOPGroupDatabase(nDbIdx, DB_READONLY, info);
	DataManagerNonDb.OpenOPNameSystem(DB_READONLY, info);
	DataManagerNonDb.OpenOPNameDatabase(nDbIdx, DB_READONLY, info);
	
	return TRUE;
}

/**********************************************************************/

bool CSPOSOfferGroupPluEditEcrman::EditPlu()
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

bool CSPOSOfferGroupPluEditEcrman::EditPluInternal(int nPluIdx)
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

void CSPOSOfferGroupPluEditEcrman::SaveChanges()
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

void CSPOSOfferGroupPluEditEcrman::Finalise()
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

void CSPOSOfferGroupPluEditEcrman::LoadReadOnlyDatabase()
{
	CDataManagerInfo info;
	DataManager.ClosePlu(info);
	DataManager.CloseBarcode(info);
	DataManager.OpenPlu(DB_READONLY, info);
	DataManager.OpenBarcode(DB_READONLY, info);
}

/**********************************************************************/