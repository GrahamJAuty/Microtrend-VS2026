#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
#include "..\SmartPay4Shared\TLogDatabase.h"
/**********************************************************************/

class CTLogCache 
{
public:
	CTLogCache();
	~CTLogCache();
	void Reset();
	
public:
	bool FindMapEntry(CSortedIntByInt64& item, int& nPos)
	{
		return m_arrayTLogMap.Find(item, nPos);
	}

	void GetMapEntry(int nPos, CSortedIntByInt64& item)
	{
		m_arrayTLogMap.GetAt(nPos, item);
	}

	void InsertMapEntry(int nPos, CSortedIntByInt64& item)
	{
		m_arrayTLogMap.InsertAt(nPos, item);
	}

	CTLogDatabase* GetTLogDatabase(int nPos )
	{
		return m_arrayTLogDatabases.GetAt(nPos);
	}

	void AddTLogDatabase(CTLogDatabase* pDb )
	{
		m_arrayTLogDatabases.Add(pDb);
	}

	int GetTLogDatabaseCount() { return m_arrayTLogDatabases.GetSize(); }

private:
	CReportConsolidationArray<CSortedIntByInt64> m_arrayTLogMap;
	CArray<CTLogDatabase*> m_arrayTLogDatabases;
};

/**********************************************************************/

