/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#include "TimePlansCSVArray.h"
/**********************************************************************/

CTimePlansCSVRecord::CTimePlansCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CTimePlansCSVRecord::ClearRecord()
{
	m_nPlanNo = 1;
	m_strPlanName = "";
}

/**********************************************************************/

void CTimePlansCSVRecord::ConvertFromCSV(CCSV& csv)
{
	ClearRecord();

	switch (csv.GetInt(0))
	{
	case 1:
		V1ConvertFromCSV(csv);
		break;
	}
}

/**********************************************************************/

void CTimePlansCSVRecord::V1ConvertFromCSV(CCSV& csv)
{
	SetPlanNo(csv.GetInt(1));
	SetPlanName(csv.GetString(2));
}

/**********************************************************************/
	
void CTimePlansCSVRecord::ConvertToCSV(CCSV& csv)
{
	csv.Add(TIMEPLANS_VERSION);
	csv.Add(m_nPlanNo);
	csv.Add(m_strPlanName);
}

/**********************************************************************/

const char* CTimePlansCSVRecord::GetDisplayName()
{
	m_strDisplayName = m_strPlanName;
	::TrimSpaces(m_strPlanName, FALSE);

	if (m_strPlanName == "")
	{
		m_strPlanName.Format("Time Plan %d", m_nPlanNo);
	}

	return m_strDisplayName;
}

/**********************************************************************/

void CTimePlansCSVRecord::SetPlanNo(int n)
{
	Set(m_nPlanNo, n, TimePlans::PlanNo);
}

/**********************************************************************/

void CTimePlansCSVRecord::SetPlanName(const char* sz)
{
	CString str = sz;
	::TrimSpaces(str, FALSE);

	Set(m_strPlanName, str, TimePlans::PlanName);

	if (m_strPlanName == "")
	{
		m_strPlanName.Format("Time Plan %d", m_nPlanNo);
	}
}

/**********************************************************************/

CTimePlansCSVArray::CTimePlansCSVArray()
{
	m_nSortMode = TIMEPLAN_SORT_NUMBER;
}

/**********************************************************************/

CTimePlansCSVArray::~CTimePlansCSVArray()
{
}

/**********************************************************************/

bool CTimePlansCSVArray::FindPlanByNumber(int nPlanNo, int& nPlanIdx)
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while (nStart <= nEnd)
	{
		nPlanIdx = (nStart + nEnd) / 2;

		CTimePlansCSVRecord Plan;
		GetAt(nPlanIdx, Plan);
		int nArrayNo = Plan.GetPlanNo();

		if (nPlanNo == nArrayNo)
		{
			return TRUE;
		}

		if (nPlanNo < nArrayNo)
		{
			nEnd = nPlanIdx - 1;
		}
		else
		{
			nStart = nPlanIdx + 1;
		}
	}

	nPlanIdx = nStart;
	return FALSE;
}

/**********************************************************************/

int CTimePlansCSVArray::FindFirstFreeNumber()
{
	int nResult = 0;

	CTimePlansCSVRecord Plan;
	for (int n = 0; n < GetSize(); n++)
	{
		GetAt(n, Plan);
		if (Plan.GetPlanNo() > n + 1)
		{
			nResult = n + 1;
			break;
		}
	}

	if (nResult == 0)
	{
		nResult = GetSize() + 1;
	}

	if ((nResult < TimePlans::PlanNo.Min) || (nResult > TimePlans::PlanNo.Max))
	{
		nResult = 0;
	}

	return nResult;
}

/**********************************************************************/

int CTimePlansCSVArray::Open(const char* szFilename, int nMode)
{
	RemoveAt(0, GetSize());

	int nReply = CSharedCSVArray::Open(szFilename, nMode);

	if (nReply != DB_ERR_SHARE)
	{
		if (nMode == DB_READONLY)	// if file does not exist use default values
		{
			nReply = DB_ERR_NONE;
		}
	}

	int nLastPlanNo = 0;
	int nValidRecords = 0;

	CTimePlansCSVRecord Plan;
	for (; nValidRecords < GetSize(); nValidRecords++)
	{
		GetAt(nValidRecords, Plan);

		int nPlanNo = Plan.GetPlanNo();
		if ((nPlanNo <= nLastPlanNo) || (nPlanNo > TimePlans::PlanNo.Max))
		{
			break;
		}

		nLastPlanNo = nPlanNo;
	}

	while (GetSize() > nValidRecords)
	{
		RemoveAt(GetSize() - 1);
	}

	return nReply;
}

/**********************************************************************/

void CTimePlansCSVArray::SetSortMode( int nMode )
{
	switch( nMode )
	{
	case TIMEPLAN_SORT_NUMBER:
	case TIMEPLAN_SORT_NAME_UP:
	case TIMEPLAN_SORT_NAME_DOWN:
		m_arraySort.RemoveAll();
		m_nSortMode = nMode;
		break;

	default:
		return;
	}

	if ( m_nSortMode != TIMEPLAN_SORT_NUMBER )
	{
		CReportConsolidationArray<CSortedIntByString> arrayTemp(TRUE);

		for ( int n = 0; n < GetSize(); n++ )
		{
			CTimePlansCSVRecord Plan;
			GetAt( n, Plan );

			CSortedIntByString item;
			item.m_strItem = Plan.GetDisplayName();
			item.m_strItem.MakeUpper();
			item.m_nItem = n;
			arrayTemp.Consolidate(item);
		}

		for (int n = 0; n < arrayTemp.GetSize(); n++)
		{
			CSortedIntByString item;
			arrayTemp.GetAt(n, item);

			if (TIMEPLAN_SORT_NAME_DOWN == m_nSortMode)
			{
				m_arraySort.InsertAt(0, item.m_nItem);
			}
			else
			{
				m_arraySort.Add(item.m_nItem);
			}
		}
	}	
}

/**********************************************************************/

int CTimePlansCSVArray::GetSortArraySize()
{
	switch( m_nSortMode )
	{
	case TIMEPLAN_SORT_NAME_UP:
	case TIMEPLAN_SORT_NAME_DOWN:
		return m_arraySort.GetSize();
	
	case TIMEPLAN_SORT_NUMBER:
	default:
		return GetSize();
	}
}

/**********************************************************************/

int CTimePlansCSVArray::GetArrayIdxFromSortIdx(int n)
{
	int nResult = 0;

	switch (m_nSortMode)
	{
	case TIMEPLAN_SORT_NAME_UP:
	case TIMEPLAN_SORT_NAME_DOWN:
		if ((n >= 0) && (n < m_arraySort.GetSize()))
		{
			nResult = m_arraySort.GetAt(n);
		}
		else
		{
			nResult = 0;
		}
		break;

	default:
		nResult = n;
		break;
	}

	return nResult;
}

/**********************************************************************/

int CTimePlansCSVArray::GetSortIdxFromArrayIdx(int n)
{
	int nResult = 0;

	switch (m_nSortMode)
	{
	case TIMEPLAN_SORT_NAME_UP:
	case TIMEPLAN_SORT_NAME_DOWN:
	{
		for (int x = 0; x < m_arraySort.GetSize(); x++)
		{
			if (m_arraySort.GetAt(x) == n)
			{
				nResult = x;
				break;
			}
		}
	}
	break;

	default:
		nResult = n;
		break;
	}

	return nResult;
}

/**********************************************************************/
