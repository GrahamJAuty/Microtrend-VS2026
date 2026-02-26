/**********************************************************************/
#include "DepartmentCSVArray.h"
#include "MixMatchCSVArray.h"
#include "PromotionsCSVArray.h"
#include "SPOSVersions.h"
/**********************************************************************/
#include "TimePlanEntriesCSVArray.h"
/**********************************************************************/

CTimePlanEntriesCSVRecord::CTimePlanEntriesCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CTimePlanEntriesCSVRecord::ClearRecord()
{
	m_nPlanNo = 1;
	m_nEntryNo = 1;
	m_bActive = TRUE;
	m_nType = 0;
	m_nSelection = 0;
	m_strName = "";
	m_bSunday = FALSE;
	m_bMonday = FALSE;
	m_bTuesday = FALSE;
	m_bWednesday = FALSE;
	m_bThursday = FALSE;
	m_bFriday = FALSE;
	m_bSaturday = FALSE;
	m_nStartHour = 0;
	m_nStartMinute = 0;
	m_nEndHour = 23;
	m_nEndMinute = 59;
	m_nEndPeriod = 0;
	m_oleStartDate = COleDateTime(2008, 1, 1, 0, 0, 0);
	m_oleEndDate = COleDateTime(2100, 1, 1, 0, 0, 0);
}

/**********************************************************************/

void CTimePlanEntriesCSVRecord::MakeDummy(int n)
{
	m_nPlanNo = n;
	m_nEntryNo = 1;
}

/**********************************************************************/

void CTimePlanEntriesCSVRecord::ConvertFromCSV(CCSV& csv)
{
	ClearRecord();

	switch (csv.GetInt(0))
	{
	case 1:
		V1ConvertFromCSV(csv);
		break;
	}
}

/**********************************************************************/

void CTimePlanEntriesCSVRecord::V1ConvertFromCSV(CCSV& csv)
{
	SetPlanNo(csv.GetInt(1));
	SetEntryNo(csv.GetInt(2));
	SetActiveFlag(csv.GetBool(3));
	SetTypeAndSelection(csv.GetInt(4), csv.GetInt(5));
	SetName(csv.GetString(6));
	SetSundayFlag(csv.GetBool(7));
	SetMondayFlag(csv.GetBool(8));
	SetTuesdayFlag(csv.GetBool(9));
	SetWednesdayFlag(csv.GetBool(10));
	SetThursdayFlag(csv.GetBool(11));
	SetFridayFlag(csv.GetBool(12));
	SetSaturdayFlag(csv.GetBool(13));
	SetStartHour(csv.GetInt(14));
	SetStartMinute(csv.GetInt(15));
	SetEndHour(csv.GetInt(16));
	SetEndMinute(csv.GetInt(17));
	SetEndPeriod(csv.GetInt(18));
	SetStartDate(csv.GetString(19));
	SetEndDate(csv.GetString(20));
}

/**********************************************************************/
	
void CTimePlanEntriesCSVRecord::ConvertToCSV(CCSV& csv)
{
	CString strStartDate;
	strStartDate.Format("%4.4d%2.2d%2.2d",
		m_oleStartDate.GetYear(),
		m_oleStartDate.GetMonth(),
		m_oleStartDate.GetDay());

	CString strEndDate;
	strEndDate.Format("%4.4d%2.2d%2.2d",
		m_oleEndDate.GetYear(),
		m_oleEndDate.GetMonth(),
		m_oleEndDate.GetDay());

	csv.Add(TIMEPLANENTRIES_VERSION);
	csv.Add(m_nPlanNo);
	csv.Add(m_nEntryNo);
	csv.Add(m_bActive);
	csv.Add(m_nType);
	csv.Add(m_nSelection);
	csv.Add(m_strName);
	csv.Add(m_bSunday);
	csv.Add(m_bMonday);
	csv.Add(m_bTuesday);
	csv.Add(m_bWednesday);
	csv.Add(m_bThursday);
	csv.Add(m_bFriday);
	csv.Add(m_bSaturday);
	csv.Add(m_nStartHour);
	csv.Add(m_nStartMinute);
	csv.Add(m_nEndHour);
	csv.Add(m_nEndMinute);
	csv.Add(m_nEndPeriod);
	csv.Add(strStartDate);
	csv.Add(strEndDate);
}

/**********************************************************************/

void CTimePlanEntriesCSVRecord::SetPlanNo(int n)
{
	Set(m_nPlanNo, n, TimePlanEntries::PlanNo);
}

/**********************************************************************/

void CTimePlanEntriesCSVRecord::SetEntryNo(int n)
{
	Set(m_nEntryNo, n, TimePlanEntries::EntryNo);
}

/**********************************************************************/

const char* CTimePlanEntriesCSVRecord::GetLookupKey()
{
	m_strKey.Format("%3.3d%3.3d",
		m_nPlanNo,
		m_nEntryNo);

	return m_strKey;
}

/**********************************************************************/

void CTimePlanEntriesCSVRecord::SetTypeAndSelection(int t, int s)
{
	switch (t)
	{
	case 0:

		m_nType = 0;

		if ((s >= 0) && (s <= MAX_PLU_PRICES))
		{
			m_nSelection = s;
		}
		else
		{
			m_nSelection = 0;
		}

		break;

	case 1:

		m_nType = 1;

		if ((s >= 0) && (s <= Promotions::PromoNo.Max))
		{
			m_nSelection = s;
		}
		else
		{
			m_nSelection = 0;
		}

		break;

	case 2:

		m_nType = 2;

		if ((s >= 0) && (s <= MixMatch::MixMatchNo.Max))
		{
			m_nSelection = s;
		}
		else
		{
			m_nSelection = 0;
		}

		break;

	case 3:	//use gate
	case 4:	//screen mode

		m_nType = t;

		if ((s >= 0) && (s <= 0x7FFFFFFF))
		{
			m_nSelection = s;
		}
		else
		{
			m_nSelection = 0;
		}

		break;
	}
}

/**********************************************************************/

void CTimePlanEntriesCSVRecord::SetName(const char* sz)
{
	Set(m_strName, sz, TimePlanEntries::Name);
}

/**********************************************************************/

void CTimePlanEntriesCSVRecord::SetStartHour(int n)
{
	Set(m_nStartHour, n, TimePlanEntries::StartHour);
}

/**********************************************************************/

void CTimePlanEntriesCSVRecord::SetStartMinute(int n)
{
	Set(m_nStartMinute, n, TimePlanEntries::StartMinute);
}

/**********************************************************************/

void CTimePlanEntriesCSVRecord::SetEndHour(int n)
{
	Set(m_nEndHour, n, TimePlanEntries::EndHour);
}

/**********************************************************************/

void CTimePlanEntriesCSVRecord::SetEndMinute(int n)
{
	Set(m_nEndMinute, n, TimePlanEntries::EndMinute);
}

/**********************************************************************/

void CTimePlanEntriesCSVRecord::SetEndPeriod(int n)
{
	Set(m_nEndPeriod, n, TimePlanEntries::EndPeriod);
}

/**********************************************************************/

void CTimePlanEntriesCSVRecord::SetStartDate(double d)
{
	DATE d1 = DATE(d);
	COleDateTime date(d1);
	SetStartDate(date);
}

/**********************************************************************/

void CTimePlanEntriesCSVRecord::SetStartDate(const char* sz)
{
	CString str = sz;

	if (str.GetLength() == 8)
	{
		COleDateTime date(atoi(str.Left(4)), atoi(str.Mid(4, 2)), atoi(str.Right(2)), 0, 0, 0);
		SetStartDate(date);
	}
}

/**********************************************************************/

void CTimePlanEntriesCSVRecord::SetStartDate(COleDateTime& date)
{
	if (date.GetStatus() == COleDateTime::valid)
	{
		m_oleStartDate = date;
	}
}

/**********************************************************************/

void CTimePlanEntriesCSVRecord::SetEndDate(double d)
{
	DATE d1 = DATE(d);
	COleDateTime date(d1);
	SetEndDate(date);
}

/**********************************************************************/

void CTimePlanEntriesCSVRecord::SetEndDate(const char* sz)
{
	CString str = sz;

	if (str.GetLength() == 8)
	{
		COleDateTime date(atoi(str.Left(4)), atoi(str.Mid(4, 2)), atoi(str.Right(2)), 0, 0, 0);
		SetEndDate(date);
	}
}

/**********************************************************************/

void CTimePlanEntriesCSVRecord::SetEndDate(COleDateTime& date)
{
	if (date.GetStatus() == COleDateTime::valid)
	{
		m_oleEndDate = date;
	}
}

/**********************************************************************/

const char* CTimePlanEntriesCSVRecord::GetDefaultText( bool bCombo)
{
	m_strDefaultText = "";

	switch( m_nType )
	{
	case 0:
		m_strDefaultText.Format( "Price Band %d", m_nSelection );
		break;

	case 1:
		{
			int nIdx;
			if ( DataManager.Promotion.FindTableByNumber( m_nSelection, nIdx ) == TRUE )
			{
				CPromotionsCSVRecord Promo;
				DataManager.Promotion.GetAt( nIdx, Promo );
				
				if ( TRUE == bCombo )
				{
					m_strDefaultText.Format( "P%2.2d,  %s",
						m_nSelection,
						Promo.GetDisplayName() );
				}
				else
				{
					m_strDefaultText = Promo.GetDisplayName();
				}
			}		
			else if ( 0 == m_nSelection )
			{
				if (TRUE == bCombo)
				{
					m_strDefaultText = "P00, None";
				}
				else
				{
					m_strDefaultText = "None";
				}
			}
			else
			{
				if (TRUE == bCombo)
				{
					m_strDefaultText.Format("P%2.2d,  Unknown",
						m_nSelection);
				}
				else
				{
					m_strDefaultText = "Unknown";
				}
			}
		}
		break;

	case 2:
		{
			int nIdx;
			if ( DataManager.MixMatch.FindTableByNumber( m_nSelection, nIdx ) == TRUE )
			{
				CMixMatchCSVRecord MixMatch;
				DataManager.MixMatch.GetAt( nIdx, MixMatch );
				
				if ( TRUE == bCombo )
				{
					m_strDefaultText.Format( "M%2.2d,  %s",
						m_nSelection,
						MixMatch.GetDisplayName() );
				}
				else
				{
					m_strDefaultText = MixMatch.GetDisplayName();
				}
			}		
			else if ( 0 == m_nSelection )
			{
				if (TRUE == bCombo)
				{
					m_strDefaultText = "M00, None";
				}
				else
				{
					m_strDefaultText = "None";
				}
			}
			else
			{
				if ( TRUE == bCombo )	
					m_strDefaultText.Format( "M%2.2d,  Unknown",
						m_nSelection);
				else
					m_strDefaultText = "Unknown";
			}
		}
		break;

	case 3:
		m_strDefaultText.Format( "Operate Gate %d", m_nSelection );
		break;

	case 4:
		m_strDefaultText.Format( "Screen Mode %d", m_nSelection );
		break;
	}

	return m_strDefaultText;
}

/**********************************************************************/

void CTimePlanEntriesCSVRecord::CheckMinimumSPOSVersion( CMinimumSPOSVersion& version )
{
	version.m_nMinVer = SPOS_V4;
	version.m_strReason = "";
	version.m_bInvalidData = FALSE;

	switch( GetType() )
	{
	case 0:	//PRICE LEVEL
		if ( GetSelection() > 4 )
		{
			version.m_nMinVer = SPOS_V4_1071;
			version.m_strReason = "Price level 5 to 10";
		}
		break;

	case 3: //OPERATE GATE
		version.m_nMinVer = SPOS_V4_2455;
		version.m_strReason = "Time Plan Operate Gate";
		break;

	case 4: //SCREEN MODE
		version.m_nMinVer = SPOS_V4_2_919;
		version.m_strReason = "Entry Type Screen Mode";
		break;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CTimePlanEntriesCSVArray::CTimePlanEntriesCSVArray(bool bTemplateMode) 
{
	m_bTemplateMode = bTemplateMode;
}

/**********************************************************************/

CTimePlanEntriesCSVArray::~CTimePlanEntriesCSVArray() 
{
}

/**********************************************************************/

int CTimePlanEntriesCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	return nReply;
}

/**********************************************************************/

void CTimePlanEntriesCSVArray::Validate()
{
	CString strLastKey = "";

	for ( int n = 0; n < GetSize(); n++ )
	{
		CTimePlanEntriesCSVRecord record;
		GetAt( n, record );

		CString strThisKey = record.GetLookupKey();

		if ( strThisKey <= strLastKey )
		{
			RemoveAt( n, GetSize() - n );
			break;
		}

		strLastKey = strThisKey;
	}
}

/**********************************************************************/

bool CTimePlanEntriesCSVArray::FindKey( CTimePlanEntriesCSVRecord& record, int& nIndex )
{
	CString strKey = record.GetLookupKey();

	int nStart = 0;
	int nEnd = GetSize() - 1;

	while( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;

		CTimePlanEntriesCSVRecord arrayRecord;
		GetAt( nIndex, arrayRecord );
		CString strArrayKey = arrayRecord.GetLookupKey();

		if ( strArrayKey < strKey )
			nStart = nIndex + 1;
		
		else if ( strArrayKey > strKey )
			nEnd = nIndex - 1;

		else
			return TRUE;
	}

	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

bool CTimePlanEntriesCSVArray::InsertRecord( CTimePlanEntriesCSVRecord& record, int& nIndex )
{
	if ( FindKey( record, nIndex ) == FALSE )
	{
		InsertAt( nIndex, record );
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

void CTimePlanEntriesCSVArray::PurgePlan( int nPlanNo )
{
	int nIdx1, nIdx2;
	GetPlanRange( nPlanNo, nIdx1, nIdx2 );

	if ( nIdx2 >= nIdx1 )
		RemoveAt( nIdx1, ( nIdx2 - nIdx1 ) + 1 );
}

/**********************************************************************/

void CTimePlanEntriesCSVArray::GetPlanRange( int nPlanNo, int& nIdx1, int& nIdx2 )
{
	CTimePlanEntriesCSVRecord record1, record2;
	record1.MakeDummy( nPlanNo );
	record2.MakeDummy( nPlanNo + 1 );

	FindKey( record1, nIdx1 );
	FindKey( record2, nIdx2 );

	nIdx2--;
}

/**********************************************************************/

int CTimePlanEntriesCSVArray::FindFirstFreeNumber( int nPlanNo )
{
	int nResult = 0;

	int nStart, nEnd;
	GetPlanRange( nPlanNo, nStart, nEnd );

	CTimePlanEntriesCSVRecord Entry;
	for ( int n = nStart; n <= nEnd; n++ )
	{
		GetAt( n, Entry );
		if ( Entry.GetEntryNo() > ( n - nStart ) + 1 )
		{
			nResult = ( n - nStart ) + 1;
			break;
		}
	}

	if ( nResult == 0 )
		nResult = ( nEnd - nStart ) + 2;

	if ( nResult < TimePlanEntries::EntryNo.Min || nResult > TimePlanEntries::EntryNo.Max )
		nResult = 0;

	return nResult;
}

/**********************************************************************/

void CTimePlanEntriesCSVArray::GetTimeplanMap( CString& strTimeplanList, CReportConsolidationArray<CSortedIntByInt>& TimeplanMap)
{
	TimeplanMap.RemoveAll();

	if ( FALSE == m_bTemplateMode )
	{
		return;
	}

	CCSV csvMap(strTimeplanList);

	int nLastPlanNo = -1;
	for (int n = 0; n < csvMap.GetSize() - 1; n += 2)
	{
		int nPlanNo = csvMap.GetInt(n);
		int nTemplateNo = csvMap.GetInt(n + 1);

		if ((nPlanNo < TimePlans::PlanNo.Min) || (nPlanNo > TimePlans::PlanNo.Max) || (nPlanNo <= nLastPlanNo))
		{
			break;
		}

		nLastPlanNo = nPlanNo;

		int nPos = 0;
		if (DataManager.TimePlan.FindPlanByNumber(nPlanNo, nPos) == FALSE)
		{
			continue;
		}

		CTimePlanEntriesCSVRecord Entry;
		Entry.SetPlanNo(nPlanNo);
		Entry.SetEntryNo(nTemplateNo);

		if (FindKey(Entry, nPos) == FALSE)
		{
			continue;
		}

		CSortedIntByInt Item;
		Item.m_nKey = nPlanNo;
		Item.m_nVal = nTemplateNo;
		TimeplanMap.Consolidate(Item);
	}
}

/**********************************************************************/
