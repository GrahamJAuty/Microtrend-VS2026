/**********************************************************************/
#include "Consolidation.h"
/**********************************************************************/

CSortedIntItem::CSortedIntItem()
{
	Reset();
}

/**********************************************************************/

void CSortedIntItem::Reset()
{
	m_nItem = 0;
}

/**********************************************************************/

void CSortedIntItem::Add ( CSortedIntItem& source )
{
}

/**********************************************************************/

int CSortedIntItem::Compare( CSortedIntItem& source, int nHint )
{
	if ( m_nItem != source.m_nItem )
		return ( m_nItem > source.m_nItem ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedInt64Item::CSortedInt64Item()
{
	Reset();
}

/**********************************************************************/

void CSortedInt64Item::Reset()
{
	m_nItem = 0;
}

/**********************************************************************/

void CSortedInt64Item::Add ( CSortedInt64Item& source )
{
}

/**********************************************************************/

int CSortedInt64Item::Compare( CSortedInt64Item& source, int nHint )
{
	if ( m_nItem != source.m_nItem )
		return ( m_nItem > source.m_nItem ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedStringItem::CSortedStringItem()
{
	Reset();
}

/**********************************************************************/

void CSortedStringItem::Reset()
{
	m_strItem = "";
}

/**********************************************************************/

void CSortedStringItem::Add ( CSortedStringItem& source )
{
}

/**********************************************************************/

int CSortedStringItem::Compare( CSortedStringItem& source, int nHint )
{
	if ( m_strItem != source.m_strItem )
		return ( m_strItem > source.m_strItem ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedDateTimeItem::CSortedDateTimeItem()
{
	Reset();
}

/**********************************************************************/

void CSortedDateTimeItem::Reset()
{
	SetDate( COleDateTime( 2015, 1, 1, 0, 0, 0 ) );
}

/**********************************************************************/

void CSortedDateTimeItem::SetDate( COleDateTime date )
{
	if ( date.m_status == COleDateTime::valid )
	{
		m_oleDateTime = COleDateTime(
			date.GetYear(),
			date.GetMonth(),
			date.GetDay(),
			0, 0, 0 );

		m_strDateTime.Format( "%4.4d%2.2d%2.2d",
			date.GetYear(),
			date.GetMonth(),
			date.GetDay() );
	}
}

/**********************************************************************/

void CSortedDateTimeItem::Add ( CSortedDateTimeItem& source )
{
}

/**********************************************************************/

int CSortedDateTimeItem::Compare( CSortedDateTimeItem& source, int nHint )
{
	if ( 1 == nHint )
	{
		if ( m_oleDateTime != source.m_oleDateTime )
			return ( m_oleDateTime > source.m_oleDateTime ? 1 : -1 );
		else
			return 0;
	}
	else
	{
		if ( m_strDateTime != source.m_strDateTime )
			return ( m_strDateTime > source.m_strDateTime ? 1 : -1 );
		else
			return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedStringAndIntItem::CSortedStringAndIntItem()
{
	Reset();
}

/**********************************************************************/

void CSortedStringAndIntItem::Reset()
{
	m_strItem = "";
	m_nItem = 0;
}

/**********************************************************************/

void CSortedStringAndIntItem::Add ( CSortedStringAndIntItem& source )
{
}

/**********************************************************************/

int CSortedStringAndIntItem::Compare( CSortedStringAndIntItem& source, int nHint )
{
	if ( m_strItem != source.m_strItem )
		return ( m_strItem > source.m_strItem ? 1 : -1 );

	if ( m_nItem != source.m_nItem )
		return ( m_nItem > source.m_nItem ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedIntAndStringItem::CSortedIntAndStringItem()
{
	Reset();
}

/**********************************************************************/

void CSortedIntAndStringItem::Reset()
{
	m_nItem = 0;
	m_strItem = "";
}

/**********************************************************************/

void CSortedIntAndStringItem::Add ( CSortedIntAndStringItem& source )
{
}

/**********************************************************************/

int CSortedIntAndStringItem::Compare( CSortedIntAndStringItem& source, int nHint )
{
	if ( m_nItem != source.m_nItem )
		return ( m_nItem > source.m_nItem ? 1 : -1 );

	if ( m_strItem != source.m_strItem )
		return ( m_strItem > source.m_strItem ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedInt64AndStringItem::CSortedInt64AndStringItem()
{
	Reset();
}

/**********************************************************************/

void CSortedInt64AndStringItem::Reset()
{
	m_nItem = 0;
	m_strItem = "";
}

/**********************************************************************/

void CSortedInt64AndStringItem::Add ( CSortedInt64AndStringItem& source )
{
}

/**********************************************************************/

int CSortedInt64AndStringItem::Compare( CSortedInt64AndStringItem& source, int nHint )
{
	if ( m_nItem != source.m_nItem )
		return ( m_nItem > source.m_nItem ? 1 : -1 );

	if ( m_strItem != source.m_strItem )
		return ( m_strItem > source.m_strItem ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedStringCounter::CSortedStringCounter()
{
	Reset();
}

/**********************************************************************/

void CSortedStringCounter::Reset()
{
	m_strItem = "";
	m_nCount = 1;
}

/**********************************************************************/

void CSortedStringCounter::Add ( CSortedStringCounter& source )
{
	m_nCount += source.m_nCount;
}

/**********************************************************************/

int CSortedStringCounter::Compare( CSortedStringCounter& source, int nHint )
{
	if ( m_strItem != source.m_strItem )
		return ( m_strItem > source.m_strItem ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedIntByInt64AndString::CSortedIntByInt64AndString()
{
	Reset();
}

/**********************************************************************/

void CSortedIntByInt64AndString::Reset()
{
	m_nKey = 0;
	m_strKey = "";
	m_nVal = 0;
}

/**********************************************************************/

void CSortedIntByInt64AndString::Add ( CSortedIntByInt64AndString& source )
{
}

/**********************************************************************/

int CSortedIntByInt64AndString::Compare( CSortedIntByInt64AndString& source, int nHint )
{
	if ( m_nKey != source.m_nKey )
		return ( m_nKey > source.m_nKey ? 1 : -1 );

	if (m_strKey != source.m_strKey)
		return (m_strKey > source.m_strKey ? 1 : -1);

	if (m_nVal != source.m_nVal)
		return (m_nVal > source.m_nVal ? 1 : -1);
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedIntByInt64::CSortedIntByInt64()
{
	Reset();
}

/**********************************************************************/

void CSortedIntByInt64::Reset()
{
	m_nKey = 0;
	m_nVal = 0;
}

/**********************************************************************/

void CSortedIntByInt64::Add(CSortedIntByInt64& source)
{
}

/**********************************************************************/

int CSortedIntByInt64::Compare(CSortedIntByInt64& source, int nHint)
{
	if (m_nKey != source.m_nKey)
		return (m_nKey > source.m_nKey ? 1 : -1);
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedIntByInt::CSortedIntByInt()
{
	Reset();
}

/**********************************************************************/

void CSortedIntByInt::Reset()
{
	m_nKey = 0;
	m_nVal = 0;
}

/**********************************************************************/

void CSortedIntByInt::Add ( CSortedIntByInt& source )
{
}

/**********************************************************************/

int CSortedIntByInt::Compare( CSortedIntByInt& source, int nHint )
{
	if ( m_nKey != source.m_nKey )
		return ( m_nKey > source.m_nKey ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedIntByString::CSortedIntByString()
{
	Reset();
}

/**********************************************************************/

void CSortedIntByString::Reset()
{
	m_strItem = "";
	m_nItem = 0;
}

/**********************************************************************/

void CSortedIntByString::Add ( CSortedIntByString& source )
{
}

/**********************************************************************/

int CSortedIntByString::Compare( CSortedIntByString& source, int nHint )
{
	if ( nHint != 1 )
	{
		if ( m_strItem != source.m_strItem )
			return ( m_strItem > source.m_strItem ? 1 : -1 );
		else
			return 0;
	}
	else
	{
		CString str1 = m_strItem;
		CString str2 = source.m_strItem;

		str1.MakeUpper();
		str2.MakeUpper();

		if ( str1 != str2 )
			return ( str1 > str2 ? 1 : -1 );
		else
			return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedStringByInt::CSortedStringByInt()
{
	Reset();
}

/**********************************************************************/

void CSortedStringByInt::Reset()
{
	m_nItem = 0;
	m_strItem = "";
}

/**********************************************************************/

void CSortedStringByInt::Add ( CSortedStringByInt& source )
{
}

/**********************************************************************/

int CSortedStringByInt::Compare( CSortedStringByInt& source, int nHint )
{
	if ( m_nItem != source.m_nItem )
		return ( m_nItem > source.m_nItem ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedStringByIntAndInt64::CSortedStringByIntAndInt64()
{
	Reset();
}

/**********************************************************************/

void CSortedStringByIntAndInt64::Reset()
{
	m_nIntKey = 0;
	m_nInt64Key = 0;
	m_strItem = "";
}

/**********************************************************************/

void CSortedStringByIntAndInt64::Add(CSortedStringByIntAndInt64& source)
{
}

/**********************************************************************/

int CSortedStringByIntAndInt64::Compare(CSortedStringByIntAndInt64& source, int nHint)
{
	if (m_nIntKey != source.m_nIntKey)
		return (m_nIntKey > source.m_nIntKey ? 1 : -1);

	if (m_nInt64Key != source.m_nInt64Key)
		return (m_nInt64Key > source.m_nInt64Key ? 1 : -1);
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedStringByInt64::CSortedStringByInt64()
{
	Reset();
}

/**********************************************************************/

void CSortedStringByInt64::Reset()
{
	m_nItem = 0;
	m_strItem = "";
}

/**********************************************************************/

void CSortedStringByInt64::Add(CSortedStringByInt64& source)
{
}

/**********************************************************************/

int CSortedStringByInt64::Compare(CSortedStringByInt64& source, int nHint)
{
	if (m_nItem != source.m_nItem)
		return (m_nItem > source.m_nItem ? 1 : -1);
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedStringByString::CSortedStringByString()
{
	Reset();
}

/**********************************************************************/

void CSortedStringByString::Reset()
{
	m_strKey = "";
	m_strData = "";
}

/**********************************************************************/

void CSortedStringByString::Add ( CSortedStringByString& source )
{
}

/**********************************************************************/

int CSortedStringByString::Compare( CSortedStringByString& source, int nHint )
{
	if ( nHint != 1 )
	{
		if ( m_strKey != source.m_strKey )
			return ( m_strKey > source.m_strKey ? 1 : -1 );
		else
			return 0;
	}
	else
	{
		CString str1 = m_strKey;
		CString str2 = source.m_strKey;

		str1.MakeUpper();
		str2.MakeUpper();

		if ( str1 != str2 )
			return ( str1 > str2 ? 1 : -1 );
		else
			return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedIntAndInt::CSortedIntAndInt()
{
	Reset();
}

/**********************************************************************/

void CSortedIntAndInt::Reset()
{
	m_nInt1 = 0;
	m_nInt2 = 0;
}

/**********************************************************************/

void CSortedIntAndInt::Add ( CSortedIntAndInt& source )
{
}

/**********************************************************************/

int CSortedIntAndInt::Compare( CSortedIntAndInt& source, int nHint )
{
	if ( m_nInt1 != source.m_nInt1 )
		return ( m_nInt1 > source.m_nInt1 ? 1 : -1 );

	if ( m_nInt2 != source.m_nInt2 )
		return ( m_nInt2 > source.m_nInt2 ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedIntWithFlag::CSortedIntWithFlag()
{
	Reset();
}

/**********************************************************************/

void CSortedIntWithFlag::Reset()
{
	m_nInt = 0;
	m_nFlag = 0;
}

/**********************************************************************/

void CSortedIntWithFlag::Add ( CSortedIntWithFlag& source )
{
}

/**********************************************************************/

int CSortedIntWithFlag::Compare( CSortedIntWithFlag& source, int nHint )
{
	if ( m_nInt != source.m_nInt )
		return ( m_nInt > source.m_nInt ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedInt64WithFlag::CSortedInt64WithFlag()
{
	Reset();
}

/**********************************************************************/

void CSortedInt64WithFlag::Reset()
{
	m_nInt64 = 0;
	m_nFlag = 0;
}

/**********************************************************************/

void CSortedInt64WithFlag::Add(CSortedInt64WithFlag& source)
{
	m_nFlag = source.m_nFlag;
}

/**********************************************************************/

int CSortedInt64WithFlag::Compare(CSortedInt64WithFlag& source, int nHint)
{
	if (m_nInt64 != source.m_nInt64)
		return (m_nInt64 > source.m_nInt64 ? 1 : -1);
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CConsolidatedIntByInt::CConsolidatedIntByInt()
{
	Reset();
}

/**********************************************************************/

void CConsolidatedIntByInt::Reset()
{
	m_nKey = 0;
	m_nVal = 0;
}

/**********************************************************************/

void CConsolidatedIntByInt::Add ( CConsolidatedIntByInt& source )
{
	m_nVal += source.m_nVal;
}

/**********************************************************************/

int CConsolidatedIntByInt::Compare( CConsolidatedIntByInt& source, int nHint )
{
	if ( m_nKey != source.m_nKey )
		return ( m_nKey > source.m_nKey ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CConsolidatedDoubleByInt::CConsolidatedDoubleByInt()
{
	Reset();
}

/**********************************************************************/

void CConsolidatedDoubleByInt::Reset()
{
	m_nKey = 0;
	m_dVal = 0.0;
}

/**********************************************************************/

void CConsolidatedDoubleByInt::Add ( CConsolidatedDoubleByInt& source )
{
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CConsolidatedDoubleByInt::Compare( CConsolidatedDoubleByInt& source, int nHint )
{
	if ( m_nKey != source.m_nKey )
		return ( m_nKey > source.m_nKey ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CConsolidatedDoubleByInt64::CConsolidatedDoubleByInt64()
{
	Reset();
}

/**********************************************************************/

void CConsolidatedDoubleByInt64::Reset()
{
	m_nKey = 0;
	m_dVal = 0.0;
}

/**********************************************************************/

void CConsolidatedDoubleByInt64::Add ( CConsolidatedDoubleByInt64& source )
{
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CConsolidatedDoubleByInt64::Compare( CConsolidatedDoubleByInt64& source, int nHint )
{
	if ( m_nKey != source.m_nKey )
		return ( m_nKey > source.m_nKey ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CConsolidatedDoubleByString::CConsolidatedDoubleByString()
{
	Reset();
}

/**********************************************************************/

void CConsolidatedDoubleByString::Reset()
{
	m_strKey = "";
	m_dVal = 0.0;
}

/**********************************************************************/

void CConsolidatedDoubleByString::Add ( CConsolidatedDoubleByString& source )
{
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CConsolidatedDoubleByString::Compare( CConsolidatedDoubleByString& source, int nHint )
{
	if ( m_strKey != source.m_strKey )
		return ( m_strKey > source.m_strKey ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CConsolidatedThreeValuesByString::CConsolidatedThreeValuesByString()
{
	Reset();
}

/**********************************************************************/

void CConsolidatedThreeValuesByString::Reset()
{
	m_strKey = "";
	m_dValue1 = 0.0;
	m_dValue2 = 0.0;
	m_dValue3 = 0.0;
}

/**********************************************************************/

void CConsolidatedThreeValuesByString::Add ( CConsolidatedThreeValuesByString& source )
{
	m_dValue1 += source.m_dValue1;
	m_dValue2 += source.m_dValue2;
	m_dValue3 += source.m_dValue3;
}

/**********************************************************************/

int CConsolidatedThreeValuesByString::Compare( CConsolidatedThreeValuesByString& source, int nHint )
{	
	if ( nHint != 1 )
	{
		if ( m_strKey != source.m_strKey )
			return ( m_strKey > source.m_strKey ? 1 : -1 );
		else
			return 0;
	}
	else
	{
		CString str1 = m_strKey;
		CString str2 = source.m_strKey;

		str1.MakeUpper();
		str2.MakeUpper();

		if ( str1 != str2 )
			return ( str1 > str2 ? 1 : -1 );
		else
			return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CConsolidatedCashlessReconcilePurse::CConsolidatedCashlessReconcilePurse()
{
	Reset();
}

/**********************************************************************/

void CConsolidatedCashlessReconcilePurse::Reset()
{
	m_nCardNo = 0;
	m_dPurse1 = 0.0;
	m_dPurse2 = 0.0;
}

/**********************************************************************/

void CConsolidatedCashlessReconcilePurse::Add ( CConsolidatedCashlessReconcilePurse& source )
{
	//INTENTIONAL REPLACMENT WITH SOURCE, NOT ADD
	m_dPurse1 = source.m_dPurse1;
	m_dPurse2 = source.m_dPurse2;
}

/**********************************************************************/

int CConsolidatedCashlessReconcilePurse::Compare( CConsolidatedCashlessReconcilePurse& source, int nHint )
{	
	if ( m_nCardNo != source.m_nCardNo )
		return ( m_nCardNo > source.m_nCardNo ) ? 1 : -1;
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedPasswordLine::CSortedPasswordLine()
{
	Reset();
}

/**********************************************************************/

void CSortedPasswordLine::Reset()
{
	m_nSortPasswordNo = 0;
	m_strDate = "";
	m_strTime = "";
	m_nLineNo = 0;
	m_nReportPasswordNo = 0;
	m_strUserName = "";
	m_nEventType = 0;
	m_strDate2 = "";
	m_strTime2 = "";
}

/**********************************************************************/

void CSortedPasswordLine::Add(CSortedPasswordLine& source)
{
}

/**********************************************************************/

int CSortedPasswordLine::Compare(CSortedPasswordLine& source, int nHint)
{
	if (m_nSortPasswordNo != source.m_nSortPasswordNo)
		return (m_nSortPasswordNo > source.m_nSortPasswordNo ? 1 : -1);

	if (m_strDate != source.m_strDate)
		return (m_strDate > source.m_strDate ? 1 : -1);

	if (m_strTime != source.m_strTime)
		return (m_strTime > source.m_strTime ? 1 : -1);

	if (m_nLineNo != source.m_nLineNo)
		return (m_nLineNo > source.m_nLineNo ? 1 : -1);
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedDbReportItem::CSortedDbReportItem()
{
	Reset();
}

/**********************************************************************/

void CSortedDbReportItem::Reset()
{
	m_nUserID = 0;
	m_strSortField = "";
	m_nGroupNo = 0;
	m_nQty = 0;
	m_dVal = 0.0;
}

/**********************************************************************/

void CSortedDbReportItem::Add(CSortedDbReportItem& source)
{
	m_nQty += source.m_nQty;
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CSortedDbReportItem::Compare(CSortedDbReportItem& source, int nHint)
{
	if (m_nGroupNo != source.m_nGroupNo)
		return (m_nGroupNo > source.m_nGroupNo ? 1 : -1);

	if (nHint != 1)
	{
		if (m_strSortField != source.m_strSortField)
			return (m_strSortField > source.m_strSortField ? 1 : -1);
	}

	if (m_nUserID != source.m_nUserID)
		return (m_nUserID > source.m_nUserID ? 1 : -1);
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedDbReportRankingItem::CSortedDbReportRankingItem()
{
	Reset();
}

/**********************************************************************/

void CSortedDbReportRankingItem::Reset()
{
	m_nRanking = 0;
	m_nUserID = 0;
	m_strSortField = "";
	m_nGroupNo = 0;
	m_nConsolIndex = 0;
}

/**********************************************************************/

void CSortedDbReportRankingItem::Add(CSortedDbReportRankingItem& source)
{
	m_nConsolIndex = source.m_nConsolIndex;
}

/**********************************************************************/

int CSortedDbReportRankingItem::Compare(CSortedDbReportRankingItem& source, int nHint)
{
	if (m_nRanking != source.m_nRanking)
		return (m_nRanking < source.m_nRanking ? 1 : -1);

	if (m_nGroupNo != source.m_nGroupNo)
		return (m_nGroupNo > source.m_nGroupNo ? 1 : -1);

	if (nHint != 1)
	{
		if (m_strSortField != source.m_strSortField)
			return (m_strSortField > source.m_strSortField ? 1 : -1);
	}

	if (m_nUserID != source.m_nUserID)
		return (m_nUserID > source.m_nUserID ? 1 : -1);
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CBirthdayReportItem::CBirthdayReportItem()
{
	Reset();
}

/**********************************************************************/

void CBirthdayReportItem::Reset()
{
	m_nMonth = 1;
	m_nDay = 1;
	m_strSortLabel = "";
	m_nGroupNo = 0;
	m_nUserID = 0;
}

/**********************************************************************/

void CBirthdayReportItem::Add(CBirthdayReportItem& source)
{
}

/**********************************************************************/

int CBirthdayReportItem::Compare(CBirthdayReportItem& source, int nHint)
{
	if (m_nMonth != source.m_nMonth)
		return (m_nMonth > source.m_nMonth ? 1 : -1);

	if (m_nDay != source.m_nDay)
		return (m_nDay > source.m_nDay ? 1 : -1);

	if (m_strSortLabel != source.m_strSortLabel)
		return (m_strSortLabel > source.m_strSortLabel ? 1 : -1);
	
	if (m_nGroupNo != source.m_nGroupNo)
		return (m_nGroupNo > source.m_nGroupNo ? 1 : -1);
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CStampOfferRSPBuffer::CStampOfferRSPBuffer()
{
	Reset();
}

/**********************************************************************/

void CStampOfferRSPBuffer::Reset()
{
	m_nOfferID = 0;
	m_nEdition = 1;
	m_strOfferName = "";
	m_nStampCount = 0;
	m_nThreshold = 0;
}

/**********************************************************************/

void CStampOfferRSPBuffer::Add(CStampOfferRSPBuffer& source)
{
	m_nStampCount += source.m_nStampCount;
}

/**********************************************************************/

int CStampOfferRSPBuffer::Compare(CStampOfferRSPBuffer& source, int nHint)
{
	if (m_nOfferID != source.m_nOfferID)
		return (m_nOfferID > source.m_nOfferID ? 1 : -1);

	if (m_nEdition != source.m_nEdition)
		return (m_nEdition > source.m_nEdition ? 1 : -1);
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedInt64ByString::CSortedInt64ByString()
{
	Reset();
}

/**********************************************************************/

void CSortedInt64ByString::Reset()
{
	m_strKey = "";
	m_nData = 0;
}

/**********************************************************************/

void CSortedInt64ByString::Add(CSortedInt64ByString& source)
{
}

/**********************************************************************/

int CSortedInt64ByString::Compare(CSortedInt64ByString& source, int nHint)
{
	if (nHint != 1)
	{
		if (m_strKey != source.m_strKey)
			return (m_strKey > source.m_strKey ? 1 : -1);
		else
			return 0;
	}
	else
	{
		CString str1 = m_strKey;
		CString str2 = source.m_strKey;

		str1.MakeUpper();
		str2.MakeUpper();

		if (str1 != str2)
			return (str1 > str2 ? 1 : -1);
		else
			return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPluRankingReportItem::CPluRankingReportItem()
{
	Reset();
}

/**********************************************************************/

void CPluRankingReportItem::Reset()
{
	m_nPluNo = 0;
	m_strUserID = "";
	m_dQty = 0.0;
	m_dValue = 0.0;
	m_dPoints = 0.0;
}

/**********************************************************************/

void CPluRankingReportItem::Add(CPluRankingReportItem& source)
{
	m_dQty += source.m_dQty;
	m_dValue += source.m_dValue;
	m_dPoints += source.m_dPoints;
}

/**********************************************************************/

int CPluRankingReportItem::Compare(CPluRankingReportItem& source, int nHint)
{
	if (m_nPluNo != source.m_nPluNo)
	{
		return (m_nPluNo > source.m_nPluNo ? 1 : -1);
	}

	if (m_strUserID != source.m_strUserID)
	{
		return (m_strUserID > source.m_strUserID ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPluRankingUserSort::CPluRankingUserSort()
{
	Reset();
}

/**********************************************************************/

void CPluRankingUserSort::Reset()
{
	m_strRankLabel = "";
	m_nIndex;
}

/**********************************************************************/

void CPluRankingUserSort::Add(CPluRankingUserSort& source)
{
}

/**********************************************************************/

int CPluRankingUserSort::Compare(CPluRankingUserSort& source, int nHint)
{
	if (m_strInfoLabel != source.m_strInfoLabel)
	{
		return (m_strInfoLabel > source.m_strInfoLabel ? 1 : -1);
	}

	//USE DESCENDING ORDER FOR RANK LABEL
	if (m_strRankLabel != source.m_strRankLabel)
	{
		return (m_strRankLabel < source.m_strRankLabel ? 1 : -1);
	}

	if (m_nIndex != source.m_nIndex)
	{
		return (m_nIndex > source.m_nIndex ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CTidyUpFileInfo::CTidyUpFileInfo()
{
	Reset();
}

/**********************************************************************/

void CTidyUpFileInfo::Reset()
{
	m_strFilename = "";
	m_strModTime = "";
	m_FirstSeenTime = 0;
}

/**********************************************************************/

void CTidyUpFileInfo::Add(CTidyUpFileInfo& source)
{
}

/**********************************************************************/

int CTidyUpFileInfo::Compare(CTidyUpFileInfo& source, int nHint)
{
	if (m_strFilename != source.m_strFilename)
	{
		return (m_strFilename > source.m_strFilename ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
