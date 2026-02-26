/**********************************************************************/
#include "EposReportConsolEODStatus.h"
/**********************************************************************/

CEposReportConsolEODStatus::CEposReportConsolEODStatus()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolEODStatus::Reset()
{
	m_nLocIdx = 0;
	m_nEODCount = 0;
	m_strLatestEOD = "";
}

/**********************************************************************/

void CEposReportConsolEODStatus::Add (CEposReportConsolEODStatus& source )
{
	m_nEODCount += source.m_nEODCount;
	if ( source.m_strLatestEOD > m_strLatestEOD )
		m_strLatestEOD = source.m_strLatestEOD;
}

/**********************************************************************/

int CEposReportConsolEODStatus::Compare( CEposReportConsolEODStatus& source, int nHint )
{
	if ( m_nLocIdx != source.m_nLocIdx )
		return ( ( m_nLocIdx > source.m_nLocIdx ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
