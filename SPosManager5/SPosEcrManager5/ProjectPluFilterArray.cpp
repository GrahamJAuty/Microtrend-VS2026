/**********************************************************************/
#include "..\CommonEcrStockTray\PluCSVArray.h"
#include "..\CommonEcrStockTray\SysInfo.h"
/**********************************************************************/
#include "..\SPosEcrManager5\EcrmanSupplierSelector.h"
/**********************************************************************/
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

bool CPluFilterArray::IsExcludedByStockDetail( int nPluIdx, CPluCSVRecord& PluRecord )
{
	if (EcrmanOptions.GetFeaturesPluStockFlag() == TRUE)
	{
		if ( m_EcrmanFilters.m_nSupplierFilter != ECRMANSUPPLIER_ALL)
		{
			if (m_EcrmanFilters.m_nSupplierFilter != PluRecord.GetSupplierNo())
			{
				return TRUE;
			}
		}
	
		if ( m_EcrmanFilters.m_bShowNonStockItems == FALSE)
		{
			if (PluRecord.GetNonStockExternalFlag() == TRUE)
			{
				return TRUE;
			}
		}

		if ( m_EcrmanFilters.m_strSuppRefFilter != "")
		{
			CString strTest1 = m_EcrmanFilters.m_strSuppRefFilter;
			strTest1.MakeUpper();

			CString strTest2 = PluRecord.GetSupplierRef();
			strTest2.MakeUpper();

			if (strTest2.Find(strTest1) == -1)
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
	if (EcrmanOptions.GetFeaturesPluStockFlag() == TRUE)
	{
		if (m_EcrmanFilters.m_nSupplierFilter != ECRMANSUPPLIER_ALL)
		{
			return TRUE;
		}

		if (m_EcrmanFilters.m_strSuppRefFilter != "")
		{
			return TRUE;
		}

		if (m_EcrmanFilters.m_bShowNonStockItems == FALSE)
		{
			return TRUE;
		}
	}

	return FALSE;
}

/**********************************************************************/

int CPluFilterArray::InsertPluRecordInternal(CPluCSVRecord& PluRecord, bool bExportFlag)
{
	return DataManager.Plu.InsertRecord(PluRecord, bExportFlag);
}

/**********************************************************************/

void CPluFilterArray::GetLine_Init()
{
	m_EcrmanFilters.m_bNegativeStock = FALSE;
}

/**********************************************************************/

void CPluFilterArray::GetLine_GetStockCode(CPluCSVRecord& PluRecord, CString& strStockCode)
{
	strStockCode = PluRecord.GetSupplierRef();
}

/**********************************************************************/

void CPluFilterArray::GetLine_AppendStockCode( CCSV& csv, CString strStockCode )
{
	//Function intentionally left empty
}

/**********************************************************************/

void CPluFilterArray::GetLine_AppendEcrmanStock(CCSV& csv, CPluCSVRecord& PluRecord)
{
	if (EcrmanOptions.GetFeaturesPluStockFlag() == TRUE)
	{
		if (PluRecord.GetNonStockExternalFlag() == FALSE)
		{
			csv.Add(PluRecord.GetApparentLevel(), SysInfo.GetDPQty());
			m_EcrmanFilters.m_bNegativeStock = (PluRecord.GetApparentLevel() < 0.0);
		}
		else
		{
			csv.Add("");
		}
	}
}

/**********************************************************************/

void CPluFilterArray::GetLine_AppendStockmanSupplier(CCSV& csv, CPluCSVRecord& PluRecord)
{
	//function intentionally left blank
}

/**********************************************************************/

bool CPluFilterArray::GetLine_BrowseStockCodes()
{
	return FALSE;
}

/**********************************************************************/
