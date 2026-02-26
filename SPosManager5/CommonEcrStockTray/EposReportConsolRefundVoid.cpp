/**********************************************************************/
#include "EposReportConsolRefundVoid.h"
/**********************************************************************/

CEposReportConsolRefundVoid::CEposReportConsolRefundVoid()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolRefundVoid::Reset()
{
	m_nServerLocOrDbSort = 0;
	m_nServerLocOrDbIdx = 0;
	m_nServerNo = 0;
	m_nBlock = 0;
	m_strDate = "";
	m_strTime = "";
	m_nLineVoidBAType = EPOSREPORT_VOIDTYPE_NONE;
	m_nLineNo = 0;
	m_nPluNo = 0;
	m_nDeptNo = 0;
	m_nModifier = 0;
	m_nAuthServerNo = 0;
	m_strText = "";
	m_dModQty = 0.0;
	m_dPluQty = 0.0;
	m_dValue = 0.0;
}

/**********************************************************************/

void CEposReportConsolRefundVoid::Add (CEposReportConsolRefundVoid& source )
{
	m_nPluNo = source.m_nPluNo;
	m_nDeptNo = source.m_nDeptNo;
	m_nModifier = source.m_nModifier;
	m_strText = source.m_strText;
	m_nAuthServerNo = source.m_nAuthServerNo;
	m_dModQty = source.m_dModQty;
	m_dPluQty = source.m_dPluQty;
	m_dValue = source.m_dValue;
}

/**********************************************************************/

int CEposReportConsolRefundVoid::Compare( CEposReportConsolRefundVoid& source, int nHint )
{
	if ( m_nServerLocOrDbSort != source.m_nServerLocOrDbSort )
		return ( m_nServerLocOrDbSort > source.m_nServerLocOrDbSort ? 1 : -1 );

	if ( m_nServerLocOrDbIdx != source.m_nServerLocOrDbIdx )
		return ( m_nServerLocOrDbIdx > source.m_nServerLocOrDbIdx ? 1 : -1 );

	if ( m_nServerNo != source.m_nServerNo )
		return ( m_nServerNo > source.m_nServerNo ? 1 : -1 );

	if ( m_nBlock != source.m_nBlock )
		return ( m_nBlock > source.m_nBlock ? 1 : -1 );

	if ( m_strDate != source.m_strDate )
		return ( m_strDate > source.m_strDate ? 1 : -1 );

	if ( m_strTime != source.m_strTime )
		return ( ( m_strTime > source.m_strTime ) ? 1 : -1 );

	if ( m_nLineVoidBAType != source.m_nLineVoidBAType )
		return ( ( m_nLineVoidBAType > source.m_nLineVoidBAType ) ? 1 : -1 );

	if ( m_nLineNo != source.m_nLineNo )
		return ( ( m_nLineNo > source.m_nLineNo ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

bool CEposReportConsolRefundVoid::CompareServer( CEposReportConsolRefundVoid& source )
{
	if ( m_nServerLocOrDbSort != source.m_nServerLocOrDbSort )
		return FALSE;

	if ( m_nServerLocOrDbIdx != source.m_nServerLocOrDbIdx )
		return FALSE;

	if ( m_nServerNo != source.m_nServerNo )
		return FALSE;

	if ( m_nBlock != source.m_nBlock )
		return FALSE;

	return TRUE;
}

/**********************************************************************/
