/**********************************************************************/
 
/**********************************************************************/
#include "EposReportConsolAgeVerifyNoSale.h"
/**********************************************************************/

CEposReportConsolAgeVerifyNoSale::CEposReportConsolAgeVerifyNoSale()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolAgeVerifyNoSale::Reset()
{
	m_nServerLocOrDbSort = 0;
	m_nServerLocOrDbIdx = 0;
	m_nServerNo = 0;
	m_strDate = "";
	m_strTime = "";
	m_nLimit = 0;
	m_nResult = 0;
	m_nReasonIdx = 0;
}

/**********************************************************************/

void CEposReportConsolAgeVerifyNoSale::Add (CEposReportConsolAgeVerifyNoSale& source )
{
	m_nLimit = source.m_nLimit;
	m_nResult = source.m_nResult;
}

/**********************************************************************/

int CEposReportConsolAgeVerifyNoSale::Compare( CEposReportConsolAgeVerifyNoSale& source, int nHint )
{
	if ( m_nServerLocOrDbSort != source.m_nServerLocOrDbSort )
		return ( m_nServerLocOrDbSort > source.m_nServerLocOrDbSort ? 1 : -1 );

	if ( m_nServerLocOrDbIdx != source.m_nServerLocOrDbIdx )
		return ( m_nServerLocOrDbIdx > source.m_nServerLocOrDbIdx ? 1 : -1 );

	if ( m_nServerNo != source.m_nServerNo )
		return ( m_nServerNo > source.m_nServerNo ? 1 : -1 );

	if ( m_strDate != source.m_strDate )
		return ( m_strDate > source.m_strDate ? 1 : -1 );

	if ( m_strTime != source.m_strTime )
		return ( ( m_strTime > source.m_strTime ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

bool CEposReportConsolAgeVerifyNoSale::CompareServer( CEposReportConsolAgeVerifyNoSale& source )
{
	if ( m_nServerLocOrDbSort != source.m_nServerLocOrDbSort )
		return FALSE;

	if ( m_nServerLocOrDbIdx != source.m_nServerLocOrDbIdx )
		return FALSE;

	if ( m_nServerNo != source.m_nServerNo )
		return FALSE;

	return TRUE;
}

/**********************************************************************/
