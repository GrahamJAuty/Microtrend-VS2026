/**********************************************************************/
 
/**********************************************************************/
#include "EposReportConsolTransactionSorted.h"
/**********************************************************************/

CEposReportConsolTransactionSorted::CEposReportConsolTransactionSorted()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolTransactionSorted::Reset()
{
	m_nSequenceNo = 0;
	m_nServerNo = 0;
	m_nInternalNo = 0;
	m_strDateTime = "";
	m_strDisplayDate = "";
	m_strDisplayTime = "";
	m_nTranStartIdx = 0;
	m_nTranEndIdx = 0;
	m_nFileIdx = 0;
	m_nCashRSPVersionNo = 0;
}

/**********************************************************************/

void CEposReportConsolTransactionSorted::Add (CEposReportConsolTransactionSorted& source )
{
}

/**********************************************************************/

int CEposReportConsolTransactionSorted::Compare( CEposReportConsolTransactionSorted& source, int nHint )
{
	switch( nHint )
	{
	case 1:
		
		if ( m_nSequenceNo != source.m_nSequenceNo )
			return ( ( m_nSequenceNo > source.m_nSequenceNo ) ? 1 : -1 );

		if ( m_strDateTime != source.m_strDateTime )
			return ( ( m_strDateTime > source.m_strDateTime ) ? 1 : -1 );
		
		break;
	
	case 2:

		if ( m_strDateTime != source.m_strDateTime )
			return ( ( m_strDateTime > source.m_strDateTime ) ? 1 : -1 );

		if ( m_nSequenceNo != source.m_nSequenceNo )
			return ( ( m_nSequenceNo > source.m_nSequenceNo ) ? 1 : -1 );

		break;
	}

	if ( m_nInternalNo != source.m_nInternalNo )
		return ( ( m_nInternalNo > source.m_nInternalNo ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
