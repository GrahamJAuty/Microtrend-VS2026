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

void CSortedIntByInt64::Add ( CSortedIntByInt64& source )
{
}

/**********************************************************************/

int CSortedIntByInt64::Compare( CSortedIntByInt64& source, int nHint )
{
	if ( m_nKey != source.m_nKey )
		return ( m_nKey > source.m_nKey ? 1 : -1 );
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

CSortedIntAndStringByInt::CSortedIntAndStringByInt()
{
	Reset();
}

/**********************************************************************/

void CSortedIntAndStringByInt::Reset()
{
	m_nItem = 0;
	m_nData = 0;
	m_strData = "";
}

/**********************************************************************/

void CSortedIntAndStringByInt::Add ( CSortedIntAndStringByInt& source )
{
}

/**********************************************************************/

int CSortedIntAndStringByInt::Compare( CSortedIntAndStringByInt& source, int nHint )
{
	if ( m_nItem != source.m_nItem )
		return ( m_nItem > source.m_nItem ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedIntAndStringByInt64::CSortedIntAndStringByInt64()
{
	Reset();
}

/**********************************************************************/

void CSortedIntAndStringByInt64::Reset()
{
	m_nItem = 0;
	m_nData = 0;
	m_strData = "";
}

/**********************************************************************/

void CSortedIntAndStringByInt64::Add(CSortedIntAndStringByInt64& source)
{
}

/**********************************************************************/

int CSortedIntAndStringByInt64::Compare(CSortedIntAndStringByInt64& source, int nHint)
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

CSortedImageInfo::CSortedImageInfo()
{
	Reset();
}

/**********************************************************************/

void CSortedImageInfo::Reset()
{
	m_strFileName = "";
	m_strFilePath = "";
	m_bImageOK = FALSE;
}

/**********************************************************************/

void CSortedImageInfo::Add ( CSortedImageInfo& source )
{
}

/**********************************************************************/

int CSortedImageInfo::Compare( CSortedImageInfo& source, int nHint )
{
	if ( m_strFileName != source.m_strFileName )
		return ( m_strFileName > source.m_strFileName ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSPOSKeyboardSortedString::CSPOSKeyboardSortedString()
{
	Reset();
}

/**********************************************************************/

void CSPOSKeyboardSortedString::Reset()
{
	m_strItem = "";
}

/**********************************************************************/

void CSPOSKeyboardSortedString::Add ( CSPOSKeyboardSortedString& source )
{
}

/**********************************************************************/

int CSPOSKeyboardSortedString::Compare( CSPOSKeyboardSortedString& source, int nHint )
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

CCustomerTrackTraceInfo::CCustomerTrackTraceInfo()
{
	Reset();
}

/**********************************************************************/

void CCustomerTrackTraceInfo::Reset()
{
	m_nCustID = 0;
	m_nCustIndex = 0;
	m_nStartMinute = 0;
	m_bGotEndMinute = FALSE;
	m_nEndMinute = 0;
}

/**********************************************************************/

void CCustomerTrackTraceInfo::Add ( CCustomerTrackTraceInfo& source )
{
}

/**********************************************************************/

int CCustomerTrackTraceInfo::Compare( CCustomerTrackTraceInfo& source, int nHint )
{
	if ( m_nCustID != source.m_nCustID )
		return ( m_nCustID > source.m_nCustID ) ? 1 : -1;
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSortedTimeplanEntry::CSortedTimeplanEntry()
{
	Reset();
}

/**********************************************************************/

void CSortedTimeplanEntry::Reset()
{
	m_nPromoNo = 0;
	m_nPlanNo = 0;
	m_nTemplate = 0;
	m_nLineNo = 0;
	m_nEntryNo = 0;
}

/**********************************************************************/

void CSortedTimeplanEntry::Add(CSortedTimeplanEntry& source)
{
}

/**********************************************************************/

int CSortedTimeplanEntry::Compare(CSortedTimeplanEntry& source, int nHint)
{
	if (m_nPromoNo != source.m_nPromoNo)
		return (m_nPromoNo > source.m_nPromoNo ? 1 : -1);
	else if (m_nPlanNo != source.m_nPlanNo)
		return (m_nPlanNo > source.m_nPlanNo ? 1 : -1);
	else if (m_nTemplate != source.m_nTemplate)
		return (m_nTemplate > source.m_nTemplate ? 1 : -1);
	else if (m_nLineNo != source.m_nLineNo)
		return (m_nLineNo > source.m_nLineNo ? 1 : -1);
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
