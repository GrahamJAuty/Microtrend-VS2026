//$$******************************************************************
#include "SQLFieldValidatorBase.h"
//$$******************************************************************
#include "GroupShiftLogger.h"
//$$******************************************************************

CGroupShiftLogger::CGroupShiftLogger()
{
}

//$$******************************************************************

void CGroupShiftLogger::LogManualChange(CSQLRowAccountFull& RowAccount, CGroupShiftValidator& GroupShiftValidator, int nManualType)
{
	LogChangeInternal(RowAccount, GroupShiftValidator, nManualType);
}

//$$******************************************************************

void CGroupShiftLogger::LogSystemChange(CSQLRowAccountFull& RowAccount, CGroupShiftValidator& GroupShiftValidator)
{
	LogChangeInternal(RowAccount, GroupShiftValidator, 0);
}

//$$******************************************************************

void CGroupShiftLogger::LogChangeInternal(CSQLRowAccountFull& RowAccount, CGroupShiftValidator& GroupShiftValidator, int nManualType)
{
	CString strLog = GroupShiftValidator.GetChangeLogString(RowAccount);

	CSortedStringByInt64 item;
	item.m_nItem = RowAccount.GetUserID();

	int nPos = 0;
	bool bFound = FALSE;
	if (m_logCache.Find(item, nPos) == TRUE)
	{
		bFound = TRUE;

		m_logCache.GetAt(nPos, item);

		if (item.m_strItem == strLog)
		{
			return;
		}
	}

	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	CString strTime = "";
	strTime.Format("%2.2d:%2.2d:%2.2d",
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond());

	int nChangeType = 0;

#ifdef SYSTEMTYPE_SERVER
	nChangeType = 1;
#endif

#ifdef SYSTEMTYPE_BACKGROUND
	nChangeType = 2;
#endif

	if (0 == nChangeType)
	{
		if ((3 == nManualType) || (4 == nManualType))
		{
			nChangeType = nManualType;
		}
	}

	CCSV csvLog;
	csvLog.Add(1);
	csvLog.Add(CSQLFieldValidatorBase::OleDateToString(timeNow));
	csvLog.Add(strTime);
	csvLog.Add(nChangeType);
	csvLog.Add(RowAccount.GetUserID());
	csvLog.Add(RowAccount.GetUsername());
	
	CString strFullLog = csvLog.GetLine();
	strFullLog += ",";
	strFullLog += strLog;

	CSSFile fileLog;
	if (fileLog.Open(Filenames.GetGroupShiftLogFilename(), "ab") == TRUE)
	{
		fileLog.WriteLine(strFullLog);

		item.m_strItem = strLog;
		if (TRUE == bFound)
		{
			m_logCache.SetAt(nPos, item);
		}
		else
		{
			m_logCache.InsertAt(nPos, item);
		}
	}
}

//$$******************************************************************
