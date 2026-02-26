#pragma once
/**********************************************************************/
#include "PosTrayTask.h"
/**********************************************************************/

class CPosTrayTaskIterator
{
public:
	CPosTrayTaskIterator( CPosTrayTask& Task );
	
public:
	bool GetNextTaskRunTime( COleDateTime& oleTimeLastRun, COleDateTime& oleTimeNow, COleDateTime& oleTimeNextRun );

private:
	int GetIterationsPerWeek(){ return m_arrayDayStarts.GetSize() * m_nIterationsPerDay; }
	int GetIterationMinute( int nIteration );
	bool FindMinute( int nMinute, int& nPos );
	
private:
	CArray<int,int> m_arrayDayStarts;
	int m_nIterationsPerDay;
	int m_nTaskInterval;
};

/**********************************************************************/
