//**********************************************************************
#include "..\SQLConnectionPool.h"
#include "..\SQLDb.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRepositoryAuditPeriod.h"
#include "SQLRecordSetAuditPeriod.h"
//**********************************************************************
#include "SQLRowSetAuditPeriod.h"
//**********************************************************************

CSQLRowSetAuditPeriod::CSQLRowSetAuditPeriod()
{
	for (int n = 0; n < nMAXPERIODS; n++)
	{
		m_arrayPeriods[n].SetPeriodID(n);
	}
}

//**********************************************************************

bool CSQLRowSetAuditPeriod::IsValidPeriodIndex(int n)
{
	return ((n >= 0) && (n < nMAXPERIODS));
}

//**********************************************************************

void CSQLRowSetAuditPeriod::LoadPeriods(CDatabase* pDatabase)
{
	CSQLRecordSetAuditPeriod RecordSet(pDatabase, RSParams_AuditPeriod_NormalNoParams{});

	int n = 0;
	CSQLRowAuditPeriod RowAP;
	while ((n < nMAXPERIODS) && (RecordSet.StepSelectAll(RowAP) == TRUE))
	{
		m_arrayPeriods[n++] = RowAP;
	}

	for (int n = 0; n < nMAXPERIODS; n++)
	{
		m_arrayPeriods[n].SetPeriodID(n);
	}
}

//**********************************************************************

void CSQLRowSetAuditPeriod::SavePeriods(CDatabase* pDatabase)
{
	CString strSQL = "DELETE FROM AuditPeriods WHERE PeriodID < 0 OR PeriodID > 5";

	if ( NULL == pDatabase )
	{
		int nConnection = -1;
		CDatabase* pLocalDatabase = SQLConnectionPool.GetPooledConnection(nConnection);

		if (nConnection != -1)
		{
			CSQLHelpers::ExecuteSQL(pLocalDatabase, strSQL);
			SQLConnectionPool.FreeConnection(nConnection);
		}
	}
	else
	{
		CSQLHelpers::ExecuteSQL(pDatabase, strSQL);
	}

	CSQLRepositoryAuditPeriod PrepStat;

	for (int n = 0; n < nMAXPERIODS; n++)
	{
		PrepStat.UpsertRow(m_arrayPeriods[n],pDatabase);
	}
}

//**********************************************************************

CString CSQLRowSetAuditPeriod::GetPeriodName(int n)
{
	CString strName = "";
	if ( IsValidPeriodIndex(n) )
	{
		strName = m_arrayPeriods[n].GetPeriodName();
	}
	return strName;
}

/**********************************************************************/

int CSQLRowSetAuditPeriod::GetStartTime(int n)
{
	int nTime = 0;
	if (IsValidPeriodIndex(n))
	{
		nTime = m_arrayPeriods[n].GetStartTime();
	}
	return nTime;
}

/**********************************************************************/

void CSQLRowSetAuditPeriod::SetPeriodName(int n, CString strName)
{
	if (IsValidPeriodIndex(n))
	{
		m_arrayPeriods[n].SetPeriodName(strName);
	}
}

/**********************************************************************/

void CSQLRowSetAuditPeriod::SetStartTime(int n, int nTime)
{
	if (IsValidPeriodIndex(n))
	{
		m_arrayPeriods[n].SetStartTime(nTime);
	}
}

/**********************************************************************/

int CSQLRowSetAuditPeriod::GetPeriodIndex(CSSTime* pTime)
{
	CString strTime{};

	if (pTime == NULL)
	{
		strTime = CSSTime::GetCurrentTime().GetTime();
	}
	else
	{
		strTime.Format("%2.2d:%2.2d", pTime->GetHour(), pTime->GetMinute());
	}
	return GetPeriodIndex(strTime);
}

//**********************************************************************

int CSQLRowSetAuditPeriod::GetPeriodIndex(CString strTime)
{
	CString strStart = "";

	for (int n = nMAXPERIODS - 1; n >= 0; n--)
	{
		strStart = m_arrayPeriods[n].FormatStartTime();

		if (((strStart != "00:00") || (0 == n)) && (strTime >= strStart))
		{
			return n;
		}
	}

	for (int n = nMAXPERIODS - 1; n >= 1; n--)
	{
		strStart = m_arrayPeriods[n].FormatStartTime();

		if (strStart != "00:00")
		{
			return n;
		}
	}

	return 0;
}

/**********************************************************************/

CString CSQLRowSetAuditPeriod::FormatStartTime(int n)
{
	CString strTime = "";
	if (IsValidPeriodIndex(n))
	{
		strTime = m_arrayPeriods[n].FormatStartTime();
	}
	return strTime;
}

/**********************************************************************/

bool CSQLRowSetAuditPeriod::IsPeriodSet(int n)
{
	bool bResult = FALSE;
	if (IsValidPeriodIndex(n))
	{
		CString strName = GetPeriodName(n);
		bResult = (strName != "");
	}
	return bResult;
}

/**********************************************************************/

bool CSQLRowSetAuditPeriod::GetTimeRange(const char* szLabel, int& nStartHour, int& nStartMins, int& nEndHour, int& nEndMins)
{
	nStartHour = 0;
	nStartMins = 0;
	nEndHour = 23;
	nEndMins = 59;

	CString strLabel = szLabel;
	if (strLabel == szTIMERANGE_ALL)
	{
		return TRUE;
	}

	if (strLabel == szTIMERANGE_NOW)
	{
		CTime tm = CTime::GetCurrentTime();
		nStartHour = tm.GetHour();
		nStartMins = tm.GetMinute();
		return TRUE;
	}

	for (int nIndex = 0; nIndex < nMAXPERIODS; nIndex++)
	{
		if (strLabel == m_arrayPeriods[nIndex].GetPeriodName())
		{
			int nStartTime = m_arrayPeriods[nIndex].GetStartTime();
			nStartHour = nStartTime / 60;
			nStartMins = nStartTime % 60;

			if (IsPeriodSet(nIndex + 1))
			{
				int nNextStartTime = m_arrayPeriods[nIndex+1].GetStartTime();

				if (nNextStartTime != 0)
				{
					nEndHour = nNextStartTime / 60;
					nEndMins = nNextStartTime % 60;
				}
			}

			return TRUE;
		}
	}

	return FALSE;
}

/**********************************************************************/

int CSQLRowSetAuditPeriod::GetPeriodCount()
{
	int nCount = 0;
	for (int nIndex = 0; nIndex < nMAXPERIODS; nIndex++)
	{
		if (IsPeriodSet(nIndex) == TRUE)
		{
			++nCount;
		}
	}

	return nCount;
}

/**********************************************************************/