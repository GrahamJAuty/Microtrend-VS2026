/**********************************************************************/
#include "SQLTranProcessGroupShifts.h"
/**********************************************************************/
#include "SQLGroupShiftChecker.h"
/**********************************************************************/

CSQLGroupShiftChecker::CSQLGroupShiftChecker()
{
	m_oleDateLastCheck = COleDateTime(1980, 1, 1, 0, 0, 0);
}

/**********************************************************************/

void CSQLGroupShiftChecker::CheckGroupShifts()
{
	/*
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	COleDateTime timeToday = COleDateTime(timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay(), 0, 0, 0);

	if (timeToday != m_oleDateLastCheck)
	{
		CSQLTranProcessGroupShifts Tran;

		Tran.BeginTrans();
		Tran.DoWork(timeToday);

		switch (Tran.EndTrans())
		{
		case SQLTRAN_STATE_COMMITOK:
			m_oleDateLastCheck = timeToday;
			break;
		}
	}
	*/
}

/**********************************************************************/
