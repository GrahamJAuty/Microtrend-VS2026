/**********************************************************************/
#include "..\SPosStockManager5\StockOptionsIni.h"
/**********************************************************************/
#include "..\CommonEcrStockTray\PluCSVArray.h"
#include "..\CommonEcrStockTray\StatusProgress.h"
/**********************************************************************/
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

bool CPluFilterArray::IsExcludedByStockDetail(int nPluIdx, CPluCSVRecord& PluRecord)
{
	if (m_StockmanFilters.m_strStockCode != "")
	{
		CString strPluStockCode;
		DataManager.Plu.GetPluStockCode(PluRecord.GetPluNoInt(), strPluStockCode, FALSE);

		if (strPluStockCode.Find(m_StockmanFilters.m_strStockCode) == -1)
		{
			return TRUE;
		}
	}

	if ((FALSE == m_StockmanFilters.m_bShowUnlinkedItems) || (FALSE == m_StockmanFilters.m_bShowLinkedItems) || (FALSE == m_StockmanFilters.m_bShowRecipeItems))
	{
		CRecipeCSVRecord Recipe;
		DataManager.Plu.GetRecipe(nPluIdx, PluRecord.GetPluNoInt(), Recipe);

		int nItemCount = Recipe.GetSize();

		if (FALSE == m_StockmanFilters.m_bShowUnlinkedItems)
		{
			if (0 == nItemCount)
			{
				return TRUE;
			}
		}

		if (FALSE == m_StockmanFilters.m_bShowLinkedItems)
		{
			if (1 == nItemCount)
			{
				return TRUE;
			}
		}

		if (FALSE == m_StockmanFilters.m_bShowRecipeItems)
		{
			if (nItemCount > 1)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

/**********************************************************************/

bool CPluFilterArray::CheckFilteredStock()
{
	if (m_StockmanFilters.m_strStockCode != "")
	{
		return TRUE;
	}

	if (FALSE == m_StockmanFilters.m_bShowUnlinkedItems)
	{
		return TRUE;
	}

	if (FALSE == m_StockmanFilters.m_bShowLinkedItems)
	{
		return TRUE;
	}

	if (FALSE == m_StockmanFilters.m_bShowRecipeItems)
	{
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

int CPluFilterArray::InsertPluRecord(CPluCSVRecord& PluRecord, const char* szStockCode, bool bExportFlag)
{
	int nPluIdx = 0;
	int nListIdx = 0;

	__int64 nPluNo = PluRecord.GetPluNoInt();

	if (DataManager.Plu.FindPluByNumber(nPluNo, nPluIdx) == FALSE)
	{
		CRecipeCSVRecord RecipeRecord;

		CString strStockCode = szStockCode;

		::TrimSpaces(strStockCode, FALSE);
		if (strStockCode != "")
		{
			RecipeRecord.AddStockCode(szStockCode, 1.0);
		}

		m_pRecipeRecord = &RecipeRecord;
		InsertPluRecordInternal(PluRecord, bExportFlag);
		nListIdx = GetListIdxForNewPluIdx(PluRecord, nPluIdx);
	}

	return nListIdx;
}

/**********************************************************************/

int CPluFilterArray::InsertPluRecord(CPluCSVRecord& PluRecord, CRecipeCSVRecord& RecipeRecord, bool bExportFlag)
{
	int nPluIdx = 0;
	int nListIdx = 0;

	__int64 nPluNo = PluRecord.GetPluNoInt();

	if (DataManager.Plu.FindPluByNumber(nPluNo, nPluIdx) == FALSE)
	{
		m_pRecipeRecord = &RecipeRecord;
		InsertPluRecordInternal(PluRecord, bExportFlag);
		nListIdx = GetListIdxForNewPluIdx(PluRecord, nListIdx);
	}

	return nListIdx;
}

/**********************************************************************/

int CPluFilterArray::InsertPluRecordInternal(CPluCSVRecord& PluRecord, bool bExportFlag)
{
	return DataManager.Plu.InsertRecord(PluRecord, *m_pRecipeRecord, bExportFlag);
}

/**********************************************************************/

void CPluFilterArray::GetLine_Init()
{
	//Intentionally empty function
}

/**********************************************************************/

void CPluFilterArray::GetLine_GetStockCode(CPluCSVRecord& PluRecord, CString& strStockCode)
{
	DataManager.Plu.GetPluStockCode(PluRecord.GetPluNoInt(), strStockCode, TRUE);
}

/**********************************************************************/

void CPluFilterArray::GetLine_AppendStockCode(CCSV& csv, CString strStockCode)
{
	csv.Add(strStockCode);
}

/**********************************************************************/

void CPluFilterArray::GetLine_AppendEcrmanStock(CCSV& csv, CPluCSVRecord& PluRecord )
{
	//Intentionally empty function
}

/**********************************************************************/

void CPluFilterArray::GetLine_AppendStockmanSupplier(CCSV& csv, CPluCSVRecord& PluRecord)
{
	CString strSupplier = "";
	DataManager.Plu.GetPluSupplier(PluRecord.GetPluNoInt(), strSupplier);
	csv.Add(strSupplier);
}

/**********************************************************************/

bool CPluFilterArray::GetLine_BrowseStockCodes()
{
	return StockOptions.GetPluBrowseStockCodesFlag();
}

/**********************************************************************/

void CPluFilterArray::QuickBuildUnlinked(bool bOnlyIfActive)
{
	StatusProgress.Lock(TRUE, "Building deletion list");
	QuickBuildUnlinkedInternal(bOnlyIfActive);
	StatusProgress.Unlock();
}

/**********************************************************************/

void CPluFilterArray::QuickBuildUnlinkedInternal(bool bOnlyIfActive)
{
	Reset();
	m_bFiltered = TRUE;

	m_index.RemoveAll();
	m_nIndexVersion = 0;

	int nSize = DataManager.Plu.GetSize();
	for (int nPluIdx = 0; nPluIdx < nSize; nPluIdx++)
	{
		StatusProgress.SetPos(nPluIdx, nSize);

		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt(nPluIdx, PluRecord);
		__int64 nPluNo = PluRecord.GetPluNoInt();

		CRecipeCSVRecord RecipeRecord;
		DataManager.Plu.GetRecipe(nPluIdx, nPluNo, RecipeRecord);

		if (RecipeRecord.GetSize() != 0)
			continue;

		if ((FALSE == bOnlyIfActive) || (PluRecord.GetActiveItemFlag() == TRUE))
		{
			CPluIndexItem item;
			item.m_nPluNo = nPluNo;
			item.m_nDeptNo = PluRecord.GetBaseDeptNo();
			item.m_strText = PluRecord.GetReportText();
			item.m_strText.MakeUpper();
			item.m_strStockCode = GetStockCodeForSorting(PluRecord);
			item.m_nIndex = nPluIdx;
			item.m_nIndexVersion = 0;
			item.m_bDeletionFlag = FALSE;
			m_index.DirectAdd(item);
		}
	}
}

/**********************************************************************/


