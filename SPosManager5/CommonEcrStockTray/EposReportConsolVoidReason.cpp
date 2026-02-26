 
/**********************************************************************/
#include "EposReportConsolVoidReason.h"
/**********************************************************************/

CEposReportConsolVoidReason::CEposReportConsolVoidReason()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolVoidReason::Reset()
{
	m_bGrandTotal = FALSE;
	m_nServerLocOrDbSort = 0;
	m_nServerLocOrDbIdx = 0;
	m_nServerNo = 0;
	m_nBlock = 0;
	m_strReportDate = "";
	m_nTableNo = 0;
	m_nSeqNo = 0;
	m_strReportTime = "";
	m_strOtherTime = "";
	m_nLineNo = 0;

	m_nPluNo = 0;
	m_nDeptNo = 0;
	m_nModifier = 0;
	m_strPluText = "";
	m_strReason = "";
	m_bWaste = FALSE;

	m_strDiscountType = "";
	m_strPromo = "";
	m_strRate = "";

	m_dModQty = 0.0;
	m_dPluQty = 0.0;
	m_dValue = 0.0;
}

/**********************************************************************/

void CEposReportConsolVoidReason::Add (CEposReportConsolVoidReason& source )
{
	m_dModQty += source.m_dModQty;
	m_dPluQty += source.m_dPluQty;
	m_dValue += source.m_dValue;
}

/**********************************************************************/

int CEposReportConsolVoidReason::Compare( CEposReportConsolVoidReason& source, int nHint )
{
	if ( m_bGrandTotal != source.m_bGrandTotal )
		return ( m_bGrandTotal ? 1 : -1 );

	if ( FALSE == m_bGrandTotal )
	{
		if ( m_nServerLocOrDbSort != source.m_nServerLocOrDbSort )
			return ( m_nServerLocOrDbSort > source.m_nServerLocOrDbSort ? 1 : -1 );

		if ( m_nServerLocOrDbIdx != source.m_nServerLocOrDbIdx )
			return ( m_nServerLocOrDbIdx > source.m_nServerLocOrDbIdx ? 1 : -1 );

		if ( m_nServerNo != source.m_nServerNo )
			return ( m_nServerNo > source.m_nServerNo ? 1 : -1 );
	}

	if ( m_nBlock != source.m_nBlock )
		return ( m_nBlock > source.m_nBlock ? 1 : -1 );

	if ( TRUE == m_bGrandTotal )
		return 0;

	switch( m_nBlock )
	{
	case VOID_SERVER_SALES_BEFORE_TOTAL:
	case VOID_SERVER_SALES_AFTER_TOTAL:
	case VOID_SERVER_SALES_COMBINED_TOTAL:
	case VOID_SERVER_DISCOUNT_BEFORE_TOTAL:
	case VOID_SERVER_DISCOUNT_AFTER_TOTAL:
	case VOID_SERVER_DISCOUNT_COMBINED_TOTAL:
	case VOID_SERVER_REFUND_BEFORE_TOTAL:
	case VOID_SERVER_REFUND_AFTER_TOTAL:
	case VOID_SERVER_REFUND_COMBINED_TOTAL:
		return 0;
	}

	if ( m_strReportDate != source.m_strReportDate )
		return ( m_strReportDate > source.m_strReportDate ? 1 : -1 );

	if ( m_strReportTime != source.m_strReportTime )
		return ( ( m_strReportTime > source.m_strReportTime ) ? 1 : -1 );

	if ( m_nSeqNo != source.m_nSeqNo )
		return ( m_nSeqNo > source.m_nSeqNo ? 1 : -1 );
	
	if ( m_nTableNo != source.m_nTableNo )
		return ( m_nTableNo > source.m_nTableNo ? 1 : -1 );

	if ( m_strOtherTime != source.m_strOtherTime )
		return ( ( m_strOtherTime > source.m_strOtherTime ) ? 1 : -1 );
	
	if ( m_nLineNo != source.m_nLineNo )
		return ( ( m_nLineNo > source.m_nLineNo ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

bool CEposReportConsolVoidReason::CompareServer( CEposReportConsolVoidReason& source )
{
	if ( m_bGrandTotal != source.m_bGrandTotal )
		return FALSE;

	if ( TRUE == m_bGrandTotal )
		return TRUE;

	if ( m_nServerLocOrDbSort != source.m_nServerLocOrDbSort )
		return FALSE;

	if ( m_nServerLocOrDbIdx != source.m_nServerLocOrDbIdx )
		return FALSE;

	if ( m_nServerNo != source.m_nServerNo )
		return FALSE;

	return TRUE;
}

/**********************************************************************/
