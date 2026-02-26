#pragma once
/**********************************************************************/
#include "MinimumSPOSVersion.h"
/**********************************************************************/
#define MIXMATCH_VERSION 5
/**********************************************************************/
#define MIXMATCH_ACTION_NONE 0
#define MIXMATCH_ACTION_DISCOUNT_PLU 1
#define MIXMATCH_ACTION_FIXCOST_AMOUNT 2
#define MIXMATCH_ACTION_PROMO 3
#define MIXMATCH_ACTION_FREE_ITEM 4
#define MIXMATCH_ACTION_FIXCOST_PLU 5
#define MIXMATCH_ACTION_FREE_BUCKET 6
#define MIXMATCH_ACTION_FIXCOST_PLU_2 7
#define MIXMATCH_ACTION_FIXCOST_BUCKET 8
/**********************************************************************/

namespace MixMatch
{
const recInt MixMatchNo				= { "MixMatchNo", 1, 999 };
const recString Description			= { "Description", 0, 50 };
const recBool Enable				= { "Enable" };
const recInt ActivationType			= { "ActivationType", 0, 2 };
const recInt Action					= { "Action", 0, 8 };
const recInt64 ActionPluNo			= { "ActionPluNo", 1, 99999999999999 };
const recInt ActionBucketFreeItem	= { "ActionBucketFreeItem", 0, 5 };
const recInt ActionBucketFreeBucket	= { "ActionBucketFreeBucket", 1, 5 };
const recInt ActionBucketFixedBucket= { "ActionBucketFixedBucket", 1, 5 };
const recInt ActionPromo			= { "ActionPromo", 0, 99 };
const recInt ActionCost				= { "ActionCost", 0, 999999999 };
const recInt BucketCount			= { "BucketCount", 0, 99 };
const recInt OfferGroup				= { "OfferGroup", 0, 999 };
const recDouble MinSpend			= { "MinSpend", 0.0, 99999999 };
const recInt OrderMode				= { "OrderMode", 0, 1 };
const recInt UseGate				= { "UseGate", 0, 200 };
const recBool AutoLock				= { "AutoLock" };
const recBool UseCurrentPriceBand	= { "UseCurrentPriceBand" };
const recString TimeplanList		= { "TimeplanList", 0, 200 };
};

/**********************************************************************/

class CMixMatchCSVRecord : public CCSVRecord
{
public:
	CMixMatchCSVRecord(); 
	virtual ~CMixMatchCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
public:
	int GetMixMatchNo() { return m_nMixMatchNo; }
	const char* GetDescription(){ return m_strDescription; }
	bool GetEnableFlag() { return m_bEnable; }
	int GetActivationType() { return m_nActivationType; }
	COleDateTime GetStartDate(){ return m_oleStartDate; }
	COleDateTime GetEndDate(){ return m_oleEndDate; }
	int GetOrderMode(){ return m_nOrderMode; }
	int GetUseGate(){ return m_nUseGate; }

	int GetAction() { return m_nAction; }
	__int64 GetActionPluNo() { return m_nActionPluNo; }
	int GetActionBucketFreeItem() { return m_nActionBucketFreeItem; }
	int GetActionBucketFreeBucket() { return m_nActionBucketFreeBucket; }
	int GetActionBucketFixedBucket() { return m_nActionBucketFixedBucket; }
	int GetActionPromo() { return m_nActionPromo; }
	int GetActionCost() { return m_nActionCost; }

	int GetBucketCount1() { return m_nBucketCount1; }
	int GetBucketCount2() { return m_nBucketCount2; }
	int GetBucketCount3() { return m_nBucketCount3; }
	int GetBucketCount4() { return m_nBucketCount4; }
	int GetBucketCount5() { return m_nBucketCount5; }
	
	int GetOfferGroup1() { return m_nOfferGroup1; }
	int GetOfferGroup2() { return m_nOfferGroup2; }
	int GetOfferGroup3() { return m_nOfferGroup3; }
	int GetOfferGroup4() { return m_nOfferGroup4; }
	int GetOfferGroup5() { return m_nOfferGroup5; }

	double GetMinSpend() { return m_dMinSpend; }
	bool GetAutoLockFlag() { return m_bAutoLock; }
	bool GetUseCurrentPriceBandFlag() { return m_bUseCurrentPriceBand; }
	CString GetTimeplanList() { return m_strTimeplanList; }
	
	const char* GetDisplayName();
	void GetTimeplanMap(CReportConsolidationArray<CSortedIntByInt>& TimeplanMap);

	void SetMixMatchNo( int n );
	void SetDescription( const char* sz );
	void SetEnableFlag(bool b) { m_bEnable = b; }
	void SetActivationType( int n );
	void SetStartDate( const char* sz );
	void SetStartDate( COleDateTime& date );
	void SetEndDate( const char* sz );
	void SetEndDate( COleDateTime& date );
	void SetOrderMode( int n );
	void SetUseGate( int n );

	void SetActionNone();
	void SetActionPluNo( int nAction, __int64 nPluNo );
	void SetActionCost( int nCost );
	void SetActionPromo( int nPromo );
	void SetActionBucketFreeItem( int nBucket );
	void SetActionBucketFreeBucket( int nBucket );
	void SetActionBucketFixedBucket( int nBucket, int nCost );
	
	void SetBucketCount1( int n );
	void SetBucketCount2( int n );
	void SetBucketCount3( int n );
	void SetBucketCount4( int n );
	void SetBucketCount5( int n );
	
	void SetOfferGroup1( int n );
	void SetOfferGroup2( int n );
	void SetOfferGroup3( int n );
	void SetOfferGroup4( int n );
	void SetOfferGroup5( int n );
	
	void SetMinSpend( double dMinSpend );
	void SetAutoLockFlag(bool b) { m_bAutoLock = b; }
	void SetUseCurrentPriceBandFlag(bool b) { m_bUseCurrentPriceBand = b; }
	void SetTimeplanList(const char* sz);

#ifndef POSTRAY_UTILITY
	void CheckMinimumSPOSVersion( CMinimumSPOSVersion& Version, __int64& nSharpPluNo, bool bIncludeWarning = FALSE );
#endif

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	void V2To5ConvertFromCSV( CCSV& csv, int nVer );
	
private:
	int m_nMixMatchNo;
	CString m_strDescription;
	bool m_bEnable;
	int m_nActivationType;
	COleDateTime m_oleStartDate;
	COleDateTime m_oleEndDate;
	int m_nOrderMode;
	int m_nUseGate;
	int m_nAction;
	__int64 m_nActionPluNo;
	int m_nActionBucketFreeItem;
	int m_nActionBucketFreeBucket;
	int m_nActionBucketFixedBucket;
	int m_nActionPromo;
	int m_nActionCost;
	int m_nBucketCount1;
	int m_nBucketCount2;
	int m_nBucketCount3;
	int m_nBucketCount4;
	int m_nBucketCount5;
	int m_nOfferGroup1;
	int m_nOfferGroup2;
	int m_nOfferGroup3;
	int m_nOfferGroup4;
	int m_nOfferGroup5;
	double m_dMinSpend;
	bool m_bAutoLock;
	bool m_bUseCurrentPriceBand;
	CString m_strTimeplanList;

	CString m_strDisplayName;
};

/**********************************************************************/

class CMixMatchCSVArray : public CSharedCSVArray
{
public:
	CMixMatchCSVArray();
	~CMixMatchCSVArray();

public:
	int Open( const char* szFilename, int nMode );
	int GetMixMatchNoByIndex( int nMixMatchIdx );

public:
	const char* GetTabbedComboText( int nMixMatchNo );

public:
	bool FindTableByNumber( int nTableNo, int& nTableIdx );
	int FindFirstFreeNumber();
	
private:
	CString m_strTabbedComboText;
};

/**********************************************************************/
