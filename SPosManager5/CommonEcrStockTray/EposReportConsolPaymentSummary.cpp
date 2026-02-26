/**********************************************************************/
 
/**********************************************************************/
#include "EposReportConsolPaymentSummary.h"
/**********************************************************************/

CEposReportConsolPaySum::CEposReportConsolPaySum()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolPaySum::Reset()
{
	m_nReportGroup = 0;
	m_nSubType = -1;
	m_bIsCash = FALSE;
	m_nQty = 0;
	m_nTender = 0;
	m_nAmount = 0;
	m_nGratuity = 0;
	m_nCashback = 0;
	m_nSurplus = 0;
	m_nChange = 0;
	m_nDepositSpend = 0;
	m_nCustomerSpend = 0;
	m_nRoomSpend = 0;
	m_nLoyaltySpend = 0;
	m_nSmartPaySpend = 0;
	m_nSmartPhoneSpend = 0;
	m_nSptBookSpend = 0;
}

/**********************************************************************/

void CEposReportConsolPaySum::Add( CEposReportConsolPaySum& source )
{
	m_nQty += source.m_nQty;
	m_nTender += source.m_nTender;
	m_nAmount += source.m_nAmount;
	m_nGratuity += source.m_nGratuity;
	m_nCashback += source.m_nCashback;
	m_nSurplus += source.m_nSurplus;
	m_nChange += source.m_nChange;
	m_nDepositSpend += source.m_nDepositSpend;
	m_nCustomerSpend += source.m_nCustomerSpend;
	m_nRoomSpend += source.m_nRoomSpend;
	m_nLoyaltySpend += source.m_nLoyaltySpend;
	m_nSmartPaySpend += source.m_nSmartPaySpend;
	m_nSmartPhoneSpend += source.m_nSmartPhoneSpend;
	m_nSptBookSpend += source.m_nSptBookSpend;
}

/**********************************************************************/

int CEposReportConsolPaySum::Compare( CEposReportConsolPaySum& source, int nHint )
{
	if ( m_nReportGroup != source.m_nReportGroup )
		return ( ( m_nReportGroup > source.m_nReportGroup ) ? 1 : -1 );

	if ( m_nSubType != source.m_nSubType )
		return ( ( m_nSubType > source.m_nSubType ) ? 1 : -1 );
	
	if ( m_bIsCash != source.m_bIsCash )
		return ( m_bIsCash ) ? 1 : -1;
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolPaySumOneVal::CEposReportConsolPaySumOneVal()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolPaySumOneVal::Reset()
{
	m_nReportGroup = 0;
	m_nSubType = -1;
	m_nValue = 0;
}

/**********************************************************************/

void CEposReportConsolPaySumOneVal::Add( CEposReportConsolPaySumOneVal& source )
{
	m_nValue += source.m_nValue;
}

/**********************************************************************/

int CEposReportConsolPaySumOneVal::Compare( CEposReportConsolPaySumOneVal& source, int nHint )
{
	if ( m_nReportGroup != source.m_nReportGroup )
		return ( ( m_nReportGroup > source.m_nReportGroup ) ? 1 : -1 );

	if ( m_nSubType != source.m_nSubType )
		return ( ( m_nSubType > source.m_nSubType ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolServerBlockMap::CEposReportConsolServerBlockMap()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolServerBlockMap::Reset()
{
	m_nServerLocOrDbSort = 0;
	m_nServerLocOrDbIdx = 0;
	m_nServerNo = 0;
	m_nBlockIdxStart = 0;
}

/**********************************************************************/

void CEposReportConsolServerBlockMap::Add( CEposReportConsolServerBlockMap& source )
{
}

/**********************************************************************/

int CEposReportConsolServerBlockMap::Compare( CEposReportConsolServerBlockMap& source, int nHint )
{
	if ( m_nServerLocOrDbSort != source.m_nServerLocOrDbSort )
		return ( ( m_nServerLocOrDbSort > source.m_nServerLocOrDbSort ) ? 1 : -1 );

	if ( m_nServerLocOrDbIdx != source.m_nServerLocOrDbIdx )
		return ( ( m_nServerLocOrDbIdx > source.m_nServerLocOrDbIdx ) ? 1 : -1 );

	if ( m_nServerNo != source.m_nServerNo )
		return ( ( m_nServerNo > source.m_nServerNo ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CEposReportPaySumBlock::GotData()
{
	return ( GetLineCount() != 0 );
}

/**********************************************************************/

__int64 CEposReportPaySumBlock::GetLineCount()
{
	__int64 nLineCount = 0;

	nLineCount += m_pPaymentsAll -> GetSize();
	nLineCount += m_pPaymentsItem -> GetSize();
	nLineCount += m_pPaymentsAccount -> GetSize();
	nLineCount += m_pPaymentsDeposit -> GetSize();
	nLineCount += m_pPaymentsCustomer -> GetSize();
	nLineCount += m_pPaymentsRoom -> GetSize();
	nLineCount += m_pPaymentsLoyalty -> GetSize();
	nLineCount += m_pPaymentsSmartPay -> GetSize();
	nLineCount += m_pPaymentsSmartPhone -> GetSize();
	nLineCount += m_pPaymentsSptBook -> GetSize();
	nLineCount += m_pPaymentsMixed -> GetSize();

	nLineCount += m_pPaidIn -> GetSize();
	nLineCount += m_pPaidOut -> GetSize();
	nLineCount += m_pNet -> GetSize();

	return nLineCount;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

