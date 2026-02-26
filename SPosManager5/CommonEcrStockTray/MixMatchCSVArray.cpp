/**********************************************************************/
#include "..\CommonEcrStock\CommsExportBase.h"
/**********************************************************************/
#include "SPOSVersions.h"
#include "NetworkCSVArray.h"
#include "PromotionsCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "MixMatchCSVArray.h"
/**********************************************************************/

CMixMatchCSVRecord::CMixMatchCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CMixMatchCSVRecord::ClearRecord()
{
	m_nMixMatchNo = 1;
	m_strDescription = "";

	m_bEnable = FALSE;
	m_nActivationType = 0;

	COleDateTime date = COleDateTime::GetCurrentTime();
	m_oleStartDate = date;
	m_oleEndDate = date;

	m_nOrderMode = 0;
	m_nUseGate = 0;

	m_nAction = 0;
	m_nActionPluNo = 1;
	m_nActionBucketFreeItem = 0;
	m_nActionBucketFreeBucket = 1;
	m_nActionBucketFixedBucket = 1;
	m_nActionPromo = 0;
	m_nActionCost = 1;

	m_nBucketCount1 = 0;
	m_nBucketCount2 = 0;
	m_nBucketCount3 = 0;
	m_nBucketCount4 = 0;
	m_nBucketCount5 = 0;

	m_nOfferGroup1 = 0;
	m_nOfferGroup2 = 0;
	m_nOfferGroup3 = 0;
	m_nOfferGroup4 = 0;
	m_nOfferGroup5 = 0;
	
	m_dMinSpend = 0.0;
	m_bAutoLock = FALSE;
	m_bUseCurrentPriceBand = FALSE;
	m_strTimeplanList = "";
}

/**********************************************************************/

void CMixMatchCSVRecord::ConvertFromCSV(CCSV& csv)
{
	ClearRecord();

	switch (csv.GetInt(0))
	{
	case 1:
		V1ConvertFromCSV(csv);
		break;

	case 2:
		V2To5ConvertFromCSV(csv, 2);
		break;

	case 3:
		V2To5ConvertFromCSV(csv, 3);
		break;

	case 4:
		V2To5ConvertFromCSV(csv, 4);
		break;

	case 5:
		V2To5ConvertFromCSV(csv, 5);
		break;
	}
}

/**********************************************************************/

void CMixMatchCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetMixMatchNo( csv.GetInt(1) );
	SetDescription( csv.GetString(2) );
	SetEnableFlag( csv.GetBool(3) );
	SetActivationType( csv.GetBool(4) ? 1 : 0 );
	SetStartDate( csv.GetString(5) );
	SetEndDate( csv.GetString(6) );

	int nAction = csv.GetInt(7);
	switch( nAction )
	{
	case MIXMATCH_ACTION_DISCOUNT_PLU:				
	case MIXMATCH_ACTION_FIXCOST_PLU:
	case MIXMATCH_ACTION_FIXCOST_PLU_2:
		SetActionPluNo( nAction, csv.GetInt(8) );
		break;
		
	case MIXMATCH_ACTION_FIXCOST_AMOUNT:
		SetActionCost( csv.GetInt(8) );
		break;

	case MIXMATCH_ACTION_PROMO:
		SetActionPromo( csv.GetInt(8) );
		break;

	case MIXMATCH_ACTION_FREE_ITEM:
		SetActionBucketFreeItem( csv.GetInt(8) );
		break;

	case MIXMATCH_ACTION_FREE_BUCKET:
		SetActionBucketFreeBucket( csv.GetInt(8) );
		break;

	case MIXMATCH_ACTION_NONE:
	default:
		SetActionNone();
		break;
	}

	SetBucketCount1( csv.GetInt(9) );
	SetBucketCount2( csv.GetInt(10) );
	SetBucketCount3( csv.GetInt(11) );
	SetBucketCount4( csv.GetInt(12) );
	SetBucketCount5( csv.GetInt(13) );
	SetOfferGroup1( csv.GetInt(14) );
	SetOfferGroup2( csv.GetInt(15) );
	SetOfferGroup3( csv.GetInt(16) );
	SetOfferGroup4( csv.GetInt(17) );
	SetOfferGroup5( csv.GetInt(18) );
	SetMinSpend( csv.GetDouble(19) );
}

/**********************************************************************/

void CMixMatchCSVRecord::V2To5ConvertFromCSV(CCSV& csv, int nVer)
{
	SetMixMatchNo(csv.GetInt(1));
	SetDescription(csv.GetString(2));
	SetEnableFlag(csv.GetBool(3));
	SetActivationType(csv.GetInt(4));
	SetStartDate(csv.GetString(5));
	SetEndDate(csv.GetString(6));

	int nAction = csv.GetInt(7);
	switch (nAction)
	{
	case MIXMATCH_ACTION_DISCOUNT_PLU:
	case MIXMATCH_ACTION_FIXCOST_PLU:
	case MIXMATCH_ACTION_FIXCOST_PLU_2:
		SetActionPluNo(nAction, csv.GetInt(8));
		break;

	case MIXMATCH_ACTION_FIXCOST_AMOUNT:
		SetActionCost(csv.GetInt(9));
		break;

	case MIXMATCH_ACTION_PROMO:
		SetActionPromo(csv.GetInt(10));
		break;

	case MIXMATCH_ACTION_FREE_ITEM:
		SetActionBucketFreeItem(csv.GetInt(11));
		break;

	case MIXMATCH_ACTION_FREE_BUCKET:
		SetActionBucketFreeBucket(csv.GetInt(23));
		break;

	case MIXMATCH_ACTION_FIXCOST_BUCKET:
		SetActionBucketFixedBucket(csv.GetInt(25), csv.GetInt(9));
		break;

	case MIXMATCH_ACTION_NONE:
	default:
		SetActionNone();
		break;
	}

	SetBucketCount1(csv.GetInt(12));
	SetBucketCount2(csv.GetInt(13));
	SetBucketCount3(csv.GetInt(14));
	SetBucketCount4(csv.GetInt(15));
	SetBucketCount5(csv.GetInt(16));
	SetOfferGroup1(csv.GetInt(17));
	SetOfferGroup2(csv.GetInt(18));
	SetOfferGroup3(csv.GetInt(19));
	SetOfferGroup4(csv.GetInt(20));
	SetOfferGroup5(csv.GetInt(21));
	SetMinSpend(csv.GetDouble(22));
	//23 = ACTION BUCKET FREE BUCKET
	SetOrderMode(csv.GetInt(24));
	//25 = ACTION BUCKET FIXED BUCKET
	SetUseGate(csv.GetInt(26));

	if (nVer >= 3)
	{
		SetAutoLockFlag(csv.GetBool(27));
	}

	if (nVer >= 4)
	{
		SetUseCurrentPriceBandFlag(csv.GetBool(28));
	}

	if (nVer >= 5)
	{
		SetTimeplanList(csv.GetString(29));
	}
}

/**********************************************************************/

void CMixMatchCSVRecord::ConvertToCSV(CCSV& csv)
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

	//MUST UPDATE GETMIXMATCHNO() IF ADDING NEW VERSION
	csv.Add(MIXMATCH_VERSION);
	csv.Add(m_nMixMatchNo);
	csv.Add(m_strDescription);
	csv.Add(m_bEnable);
	csv.Add(m_nActivationType);
	csv.Add(strStartDate);
	csv.Add(strEndDate);
	csv.Add(m_nAction);
	csv.Add(m_nActionPluNo);
	csv.Add(m_nActionCost);
	csv.Add(m_nActionPromo);
	csv.Add(m_nActionBucketFreeItem);
	csv.Add(m_nBucketCount1);
	csv.Add(m_nBucketCount2);
	csv.Add(m_nBucketCount3);
	csv.Add(m_nBucketCount4);
	csv.Add(m_nBucketCount5);
	csv.Add(m_nOfferGroup1);
	csv.Add(m_nOfferGroup2);
	csv.Add(m_nOfferGroup3);
	csv.Add(m_nOfferGroup4);
	csv.Add(m_nOfferGroup5);
	csv.Add(m_dMinSpend, 2);
	csv.Add(m_nActionBucketFreeBucket);
	csv.Add(m_nOrderMode);
	csv.Add(m_nActionBucketFixedBucket);
	csv.Add(m_nUseGate);
	csv.Add(m_bAutoLock);
	csv.Add(m_bUseCurrentPriceBand);
	csv.Add(m_strTimeplanList);
}

/**********************************************************************/

const char* CMixMatchCSVRecord::GetDisplayName()
{
	m_strDisplayName = m_strDescription;
	::TrimSpaces(m_strDisplayName, FALSE);

	if (m_strDisplayName == "")
	{
		m_strDisplayName.Format("Offer %d", m_nMixMatchNo);
	}

	return m_strDisplayName;
}

/**********************************************************************/

void CMixMatchCSVRecord::SetMixMatchNo(int n)
{
	Set(m_nMixMatchNo, n, MixMatch::MixMatchNo);
}

/**********************************************************************/

void CMixMatchCSVRecord::SetDescription(const char* sz)
{
	CString str = sz;
	::TrimSpaces(str, FALSE);
	Set(m_strDescription, str, MixMatch::Description);
}
	
/**********************************************************************/

void CMixMatchCSVRecord::SetActivationType(int n)
{
	Set(m_nActivationType, n, MixMatch::ActivationType);
}

/**********************************************************************/

void CMixMatchCSVRecord::SetStartDate(const char* sz)
{
	CString str = sz;

	if (str.GetLength() == 8)
	{
		COleDateTime date(atoi(str.Left(4)), atoi(str.Mid(4, 2)), atoi(str.Right(2)), 0, 0, 0);
		SetStartDate(date);
	}
}

/**********************************************************************/

void CMixMatchCSVRecord::SetStartDate(COleDateTime& date)
{
	if (date.GetStatus() == COleDateTime::valid)
	{
		m_oleStartDate = date;
	}
}

/**********************************************************************/

void CMixMatchCSVRecord::SetEndDate(const char* sz)
{
	CString str = sz;

	if (str.GetLength() == 8)
	{
		COleDateTime date(atoi(str.Left(4)), atoi(str.Mid(4, 2)), atoi(str.Right(2)), 0, 0, 0);
		SetEndDate(date);
	}
}

/**********************************************************************/

void CMixMatchCSVRecord::SetEndDate(COleDateTime& date)
{
	if (date.GetStatus() == COleDateTime::valid)
	{
		m_oleEndDate = date;
	}
}

/**********************************************************************/

void CMixMatchCSVRecord::SetOrderMode(int n)
{
	Set(m_nOrderMode, n, MixMatch::OrderMode);
}

/**********************************************************************/

void CMixMatchCSVRecord::SetActionNone()
{
	m_nAction = MIXMATCH_ACTION_NONE;
}

/**********************************************************************/

void CMixMatchCSVRecord::SetActionPluNo(int nAction, __int64 nPluNo)
{
	switch (nAction)
	{
	case MIXMATCH_ACTION_DISCOUNT_PLU:
	case MIXMATCH_ACTION_FIXCOST_PLU:
	case MIXMATCH_ACTION_FIXCOST_PLU_2:
	{
		m_nAction = nAction;
		Set(m_nActionPluNo, nPluNo, MixMatch::ActionPluNo);
	}
	break;
	}
}

/**********************************************************************/

void CMixMatchCSVRecord::SetActionCost(int nCost)
{
	m_nAction = MIXMATCH_ACTION_FIXCOST_AMOUNT;
	Set(m_nActionCost, nCost, MixMatch::ActionCost);
}

/**********************************************************************/

void CMixMatchCSVRecord::SetActionPromo(int nPromo)
{
	m_nAction = MIXMATCH_ACTION_PROMO;
	Set(m_nActionPromo, nPromo, MixMatch::ActionPromo);
}

/**********************************************************************/

void CMixMatchCSVRecord::SetActionBucketFreeItem(int nBucket)
{
	m_nAction = MIXMATCH_ACTION_FREE_ITEM;
	Set(m_nActionBucketFreeItem, nBucket, MixMatch::ActionBucketFreeItem);
}

/**********************************************************************/

void CMixMatchCSVRecord::SetActionBucketFreeBucket( int nBucket )
{
	m_nAction = MIXMATCH_ACTION_FREE_BUCKET;
	Set( m_nActionBucketFreeBucket, nBucket, MixMatch::ActionBucketFreeBucket );
}

/**********************************************************************/

void CMixMatchCSVRecord::SetActionBucketFixedBucket(int nBucket, int nCost)
{
	m_nAction = MIXMATCH_ACTION_FIXCOST_BUCKET;
	Set(m_nActionBucketFixedBucket, nBucket, MixMatch::ActionBucketFixedBucket);
	Set(m_nActionCost, nCost, MixMatch::ActionCost);
}

/**********************************************************************/

void CMixMatchCSVRecord::SetBucketCount1(int n)
{
	Set(m_nBucketCount1, n, MixMatch::BucketCount);
}

/**********************************************************************/

void CMixMatchCSVRecord::SetBucketCount2(int n)
{
	Set(m_nBucketCount2, n, MixMatch::BucketCount);
}

/**********************************************************************/

void CMixMatchCSVRecord::SetBucketCount3(int n)
{
	Set(m_nBucketCount3, n, MixMatch::BucketCount);
}

/**********************************************************************/

void CMixMatchCSVRecord::SetBucketCount4(int n)
{
	Set(m_nBucketCount4, n, MixMatch::BucketCount);
}

/**********************************************************************/

void CMixMatchCSVRecord::SetBucketCount5(int n)
{
	Set(m_nBucketCount5, n, MixMatch::BucketCount);
}

/**********************************************************************/

void CMixMatchCSVRecord::SetOfferGroup1( int n )
{
	Set ( m_nOfferGroup1, n, MixMatch::OfferGroup );
}

/**********************************************************************/

void CMixMatchCSVRecord::SetOfferGroup2( int n )
{
	Set ( m_nOfferGroup2, n, MixMatch::OfferGroup );
}

/**********************************************************************/

void CMixMatchCSVRecord::SetOfferGroup3( int n )
{
	Set ( m_nOfferGroup3, n, MixMatch::OfferGroup );
}

/**********************************************************************/

void CMixMatchCSVRecord::SetOfferGroup4( int n )
{
	Set ( m_nOfferGroup4, n, MixMatch::OfferGroup );
}

/**********************************************************************/

void CMixMatchCSVRecord::SetOfferGroup5( int n )
{
	Set ( m_nOfferGroup5, n, MixMatch::OfferGroup );
}

/**********************************************************************/

void CMixMatchCSVRecord::SetMinSpend( double dMinSpend )
{
	Set ( m_dMinSpend, dMinSpend, MixMatch::MinSpend );
}

/**********************************************************************/

void CMixMatchCSVRecord::SetUseGate( int nUseGate )
{
	Set ( m_nUseGate, nUseGate, MixMatch::UseGate );
}

/**********************************************************************/

void CMixMatchCSVRecord::SetTimeplanList(const char* sz)
{
	CString str = sz;
	::TrimSpaces(str, FALSE);
	Set(m_strTimeplanList, str, Promotions::TimeplanList);
}

/**********************************************************************/

void CMixMatchCSVRecord::GetTimeplanMap(CReportConsolidationArray<CSortedIntByInt>& TimeplanMap)
{
	DataManager.TimePlanTemplateEntry.GetTimeplanMap(m_strTimeplanList, TimeplanMap);
}

/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

void CMixMatchCSVRecord::CheckMinimumSPOSVersion(CMinimumSPOSVersion& version, __int64& nSharpPluNo, bool bIncludeWarning)
{
	version.m_nMinVer = SPOS_V4;
	version.m_strReason = "";
	version.m_bInvalidData = FALSE;
	nSharpPluNo = 0;

	int nAction = GetAction();

	if (TRUE == bIncludeWarning)
	{
		if (TRUE == bIncludeWarning)
		{
			switch (nAction)
			{
			case MIXMATCH_ACTION_FIXCOST_PLU:
			case MIXMATCH_ACTION_FIXCOST_PLU_2:
				if (GetUseCurrentPriceBandFlag() == TRUE)
				{
					version.m_nMinVer = SPOS_V4_3_18;
					version.m_strReason = "Use Current Price Band ignored for SPOS prior to v4.3.18";
					return;
				}
				break;
			}
		}
	}
	else
	{
		switch (nAction)
		{
		case MIXMATCH_ACTION_DISCOUNT_PLU:
		case MIXMATCH_ACTION_FIXCOST_PLU:
		case MIXMATCH_ACTION_FIXCOST_PLU_2:
		{
			bool bModifiable = FALSE;
			__int64 nSSPluNo = GetActionPluNo();
			CCommsExportBase::CheckPluNo(SPOS_V4, nSSPluNo, nSharpPluNo, bModifiable);
		}
		break;
		}

		if (GetUseGate() >= 101)
		{
			version.m_nMinVer = SPOS_V4_1096;
			version.m_strReason = "Use Gate 101 - 200";
			return;
		}

		if (GetUseGate() >= 1)
		{
			version.m_nMinVer = SPOS_V4_1092;
			version.m_strReason = "Use Gate 1 - 100";
			return;
		}

		switch (nAction)
		{
		case MIXMATCH_ACTION_FIXCOST_BUCKET:
			version.m_nMinVer = SPOS_V4_992;
			version.m_strReason = "Mix Match Action Fix Cost by Bucket";
			return;
		}

		if (GetMixMatchNo() >= 100)
		{
			version.m_nMinVer = SPOS_V4_827;
			version.m_strReason = "Mix Match Number 100 - 999";
			return;
		}

		switch (nAction)
		{
		case MIXMATCH_ACTION_FIXCOST_PLU_2:
			version.m_nMinVer = SPOS_V4_425;
			version.m_strReason = "Offer Type Fixed Cost by Plu (2)";
			return;

		case MIXMATCH_ACTION_FREE_BUCKET:
			version.m_nMinVer = SPOS_V4_210;
			version.m_strReason = "Offer Type One Bucket Free";
			return;
		}
	}
}

/**********************************************************************/
#endif
/**********************************************************************/

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CMixMatchCSVArray::CMixMatchCSVArray() 
{
}

/**********************************************************************/

CMixMatchCSVArray::~CMixMatchCSVArray() 
{
}

/**********************************************************************/

bool CMixMatchCSVArray::FindTableByNumber( int nTableNo, int& nTableIdx )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while (nStart <= nEnd)
	{
		nTableIdx = (nStart + nEnd) / 2;

		CMixMatchCSVRecord MixMatch;
		GetAt(nTableIdx, MixMatch);
		int nArrayNo = MixMatch.GetMixMatchNo();

		if (nTableNo == nArrayNo)
		{
			return TRUE;
		}

		if (nTableNo < nArrayNo)
		{
			nEnd = nTableIdx - 1;
		}
		else
		{
			nStart = nTableIdx + 1;
		}
	}

	nTableIdx = nStart;
	return FALSE;
}

/**********************************************************************/

int CMixMatchCSVArray::FindFirstFreeNumber()
{
	int nResult = 0;

	CMixMatchCSVRecord MixMatch;
	for ( int n = 0; n < GetSize(); n++ )
	{
		GetAt( n, MixMatch );
		if ( MixMatch.GetMixMatchNo() > n + 1 )
		{
			nResult = n + 1;
			break;
		}
	}

	if (nResult == 0)
	{
		nResult = GetSize() + 1;
	}

	if (nResult < MixMatch::MixMatchNo.Min || nResult > MixMatch::MixMatchNo.Max)
	{
		nResult = 0;
	}

	return nResult;
}

/**********************************************************************/

int CMixMatchCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if (nReply != DB_ERR_SHARE)
	{
		if (nMode == DB_READONLY)	// if file does not exist use default values
		{
			nReply = DB_ERR_NONE;
		}
	}

	int nLastMMNo = 0;
	int nValidRecords = 0;
	
	CMixMatchCSVRecord MixMatchRecord;
	for ( ; nValidRecords < GetSize(); nValidRecords++ )
	{
		GetAt( nValidRecords, MixMatchRecord );

		int nMMNo = MixMatchRecord.GetMixMatchNo();
		if (nMMNo <= nLastMMNo || nMMNo > MixMatch::MixMatchNo.Max)
		{
			break;
		}

		nLastMMNo = nMMNo;
	}

	while (GetSize() > nValidRecords)
	{
		RemoveAt(GetSize() - 1);
	}

	return nReply;
}

/**********************************************************************/

int CMixMatchCSVArray::GetMixMatchNoByIndex( int nMixMatchIdx )
{
	int nMixMatchNo = 0;

	CString strTemp;
	if ( ( strTemp = GetAt ( nMixMatchIdx ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 2 );

		switch( csv.GetInt(0) )
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			nMixMatchNo = csv.GetInt(1);
			break;
		}
	}
	
	return nMixMatchNo;
}

/**********************************************************************/

const char* CMixMatchCSVArray::GetTabbedComboText( int nMixMatchNo )
{
	CString strName = "";

	int nMixMatchIdx;
	if ( DataManager.MixMatch.FindTableByNumber( nMixMatchNo, nMixMatchIdx ) == TRUE )
	{
		CMixMatchCSVRecord MixMatch;
		DataManager.MixMatch.GetAt( nMixMatchIdx, MixMatch );
		strName = MixMatch.GetDisplayName();		
	}
	else
	{
		strName = "Unknown";
	}

	m_strTabbedComboText.Format( "M%2.2d,%s",
		nMixMatchNo,
		(const char*) strName );

	return m_strTabbedComboText;
}

/**********************************************************************/