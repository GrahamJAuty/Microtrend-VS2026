/**********************************************************************/
#include "EposReportConsolCoversTimedGroup.h"
/**********************************************************************/

CEposReportConsolCoversTimedGroup::CEposReportConsolCoversTimedGroup()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolCoversTimedGroup::Reset()
{
	m_strDateOrTime = "";
	m_nDeptOrGroupNo = 0;
	m_nLineType = 0;
	m_nTaxDateCode = 0;
	m_nTaxBand = 0;
	m_nCoversCount = 0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolCoversTimedGroup::Add (CEposReportConsolCoversTimedGroup& source )
{
	m_nCoversCount += source.m_nCoversCount;
	BaseAdd(source);
}

/**********************************************************************/

int CEposReportConsolCoversTimedGroup::Compare( CEposReportConsolCoversTimedGroup& source, int nHint )
{
	if ( m_strDateOrTime != source.m_strDateOrTime )
		return ( ( m_strDateOrTime > source.m_strDateOrTime ) ? 1 : -1 );

	if ( m_nLineType != source.m_nLineType )
		return ( ( m_nLineType > source.m_nLineType ) ? 1 : -1 );
	
	if ( 0 == m_nLineType )
		return 0;

	if ( m_nDeptOrGroupNo != source.m_nDeptOrGroupNo )
		return ( ( m_nDeptOrGroupNo > source.m_nDeptOrGroupNo ) ? 1 : -1 );
	
	if ( m_nTaxBand != source.m_nTaxBand )
		return ( ( m_nTaxBand > source.m_nTaxBand ) ? 1 : -1 );

	if ( m_nTaxDateCode != source.m_nTaxDateCode )
		return ( ( m_nTaxDateCode > source.m_nTaxDateCode ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolCoversTimedGroupTax::CEposReportConsolCoversTimedGroupTax()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolCoversTimedGroupTax::Reset()
{
	m_strDateOrTime = "";
	m_nDeptOrGroupNo = 0;
	m_nLineType = 0;
	m_nCoversCount = 0;
	m_dTaxAmount = 0.0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolCoversTimedGroupTax::Add (CEposReportConsolCoversTimedGroup& source )
{
	m_nCoversCount += source.m_nCoversCount;
	BaseAdd(source);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CEposReportCoversTimedGroupBlock::GotData()
{
	if ( m_pSales -> GetSize() != 0 )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

