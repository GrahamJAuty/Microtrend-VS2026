 /**********************************************************************/
#include "PosTrayTaskIterator.h"
/**********************************************************************/

CPosTrayTaskIterator::CPosTrayTaskIterator(CPosTrayTask& Task)
{
	int nStartMinute = 0;
	nStartMinute += Task.GetTaskTimeHourStart() * 60;
	nStartMinute += Task.GetTaskTimeMinuteStart();

	int nEndMinute = 0;
	nEndMinute += Task.GetTaskTimeHourEnd() * 60;
	nEndMinute += Task.GetTaskTimeMinuteEnd();

	if (nEndMinute < nStartMinute)
	{
		nEndMinute += 1440;
	}

	{
		CArray<bool, bool> arrayWeekdays;
		arrayWeekdays.Add(Task.GetTaskDaySunFlag());
		arrayWeekdays.Add(Task.GetTaskDayMonFlag());
		arrayWeekdays.Add(Task.GetTaskDayTueFlag());
		arrayWeekdays.Add(Task.GetTaskDayWedFlag());
		arrayWeekdays.Add(Task.GetTaskDayThuFlag());
		arrayWeekdays.Add(Task.GetTaskDayFriFlag());
		arrayWeekdays.Add(Task.GetTaskDaySatFlag());

		int nDayStartMinute = nStartMinute;
		for (int n = 0; n < arrayWeekdays.GetSize(); n++)
		{
			if (arrayWeekdays.GetAt(n) == TRUE)
			{
				m_arrayDayStarts.Add(nDayStartMinute);
			}

			nDayStartMinute += 1440;
		}
	}

	m_nTaskInterval = Task.GetTaskTimeFrequency();
	if (0 == m_nTaskInterval) m_nTaskInterval = 60;
	m_nIterationsPerDay = 1 + ((nEndMinute - nStartMinute) / m_nTaskInterval);
}

/**********************************************************************/

int CPosTrayTaskIterator::GetIterationMinute(int nIteration)
{
	int nResult = 0;

	if (m_nIterationsPerDay > 0)
	{
		int nDay = nIteration / m_nIterationsPerDay;

		if (nDay < m_arrayDayStarts.GetSize())
		{
			nResult += m_arrayDayStarts.GetAt(nDay);
			nResult += (nIteration % m_nIterationsPerDay) * m_nTaskInterval;
		}
	}

	return nResult;
}

/**********************************************************************/

bool CPosTrayTaskIterator::FindMinute(int nMinute, int& nPos)
{
	int nSize = GetIterationsPerWeek();

	int nStart = 0;
	int nEnd = nSize - 1;

	while (nStart <= nEnd)
	{
		nPos = (nStart + nEnd) / 2;
		int nTestMinute = GetIterationMinute(nPos);

		if (nTestMinute < nMinute)
		{
			nStart = nPos + 1;
		}
		else if (nTestMinute > nMinute)
		{
			nEnd = nPos - 1;
		}
		else
		{
			return TRUE;
		}
	}

	nPos = nStart;
	return FALSE;
}

/**********************************************************************/

bool CPosTrayTaskIterator::GetNextTaskRunTime(COleDateTime& oleTimeLastRun, COleDateTime& oleTimeNow, COleDateTime& oleTimeNextRun)
{
	bool bResult = FALSE;
	int nSize = GetIterationsPerWeek();

	if (nSize != 0)
	{
		int nLastRunMinute = 0;
		nLastRunMinute += (1440 * (oleTimeLastRun.GetDayOfWeek() - 1));
		nLastRunMinute += (oleTimeLastRun.GetHour() * 60);
		nLastRunMinute += oleTimeLastRun.GetMinute();

		int nPos = 0;

		{
			FindMinute(nLastRunMinute + 1, nPos);
			if (nPos >= nSize)
			{
				nPos = 0;
			}

			int	nOffset = GetIterationMinute(nPos) - nLastRunMinute;

			if (nOffset <= 0)
			{
				nOffset += (10080); //MINUTES IN A WEEK
			}

			oleTimeNextRun = oleTimeLastRun + COleDateTimeSpan(0, 0, nOffset, 0);
		}

		int nPosWithinDay = nPos % m_nIterationsPerDay;

		if (0 != nPosWithinDay)
		{
			//IF NEXT RUN TIME IS IN THE PAST THEN ONLY DO TASK ONE TIME PER DAY
			if (oleTimeNextRun < oleTimeNow)
			{
				for (int n = 0; n < m_nIterationsPerDay - nPosWithinDay; n++)
				{
					nPos++;
					if (nPos >= nSize)
					{
						nPos = 0;
					}

					int	nOffset = GetIterationMinute(nPos) - nLastRunMinute;

					if (nOffset <= 0)
					{
						nOffset += (10080); //MINUTES IN A WEEK
					}

					oleTimeNextRun = oleTimeLastRun + COleDateTimeSpan(0, 0, nOffset, 0);

					if (oleTimeNextRun >= oleTimeNow)
					{
						break;
					}
				}
			}
		}

		bResult = TRUE;
	}

	CString str;
	str.Format("%s\nTIME NOW: %2.2d:%2.2d:%4.4d %2.2d:%2.2d:%2.2d\nLAST RUN: %2.2d:%2.2d:%4.4d %2.2d:%2.2d:%2.2d\nNEXT RUN: %2.2d:%2.2d:%4.4d %2.2d:%2.2d:%2.2d\n\n",
		bResult ? "TRUE" : "FALSE",
		oleTimeNow.GetDay(),
		oleTimeNow.GetMonth(),
		oleTimeNow.GetYear(),
		oleTimeNow.GetHour(),
		oleTimeNow.GetMinute(),
		oleTimeNow.GetSecond(),
		oleTimeLastRun.GetDay(),
		oleTimeLastRun.GetMonth(),
		oleTimeLastRun.GetYear(),
		oleTimeLastRun.GetHour(),
		oleTimeLastRun.GetMinute(),
		oleTimeLastRun.GetSecond(),
		oleTimeNextRun.GetDay(),
		oleTimeNextRun.GetMonth(),
		oleTimeNextRun.GetYear(),
		oleTimeNextRun.GetHour(),
		oleTimeNextRun.GetMinute(),
		oleTimeNextRun.GetSecond());

	//TRACE ( str );

	return bResult;
}

/**********************************************************************/
