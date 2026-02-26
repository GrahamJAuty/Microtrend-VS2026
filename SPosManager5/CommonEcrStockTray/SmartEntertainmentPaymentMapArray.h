#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#define SEPAYMAP_VERSION_NO 1
/**********************************************************************/

namespace SmartEntPaymentMap
{
const recInt SmartEntPayNo		= { "SmartEntPayNo", 0, 9999 };
const recString SmartEntPayName = { "SmartEntPayName", 0, 30 };
const recInt SharpPosPayNo		= { "SharpPosPayNo", 1, 50 };
};

/**********************************************************************/

struct CSmartEntPaymentMapEntry
{
public:
	CSmartEntPaymentMapEntry();
	void Reset();

public:
	int Compare ( CSmartEntPaymentMapEntry& source, int nHint = 0 );
	void Add	( CSmartEntPaymentMapEntry& source );
	
public:
	int m_nSmartEntPayNo;
	CString m_strSmartEntPayName;
	int m_nSharpPosPayNo;
};

/**********************************************************************/

class CSmartEntertainmentPaymentMapCSVRecord : public CCSVRecord
{
public:
	CSmartEntertainmentPaymentMapCSVRecord();
	virtual ~CSmartEntertainmentPaymentMapCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv );
	virtual void ConvertFromCSV ( CCSV& csv );

public:
	int GetSmartEntPayNo(){ return m_nSmartEntPayNo; }
	const char* GetSmartEntPayName(){ return m_strSmartEntPayName; }
	int GetSharpPosPayNo(){ return m_nSharpPosPayNo; }

public:
	void SetSmartEntPayNo( int n );
	void SetSmartEntPayName( const char* sz );
	void SetSharpPosPayNo( int n );
	
private:
	void ClearRecord();
	void V1ConvertFromCSV ( CCSV& csv );

private:
	int m_nSmartEntPayNo;
	CString m_strSmartEntPayName;
	int m_nSharpPosPayNo;
};

/**********************************************************************/

class CSmartEntertainmentPaymentMapCSVArray : public CSharedCSVArray
{
public:
	CSmartEntertainmentPaymentMapCSVArray();
	~CSmartEntertainmentPaymentMapCSVArray();

public:
	int Open( const char* szFilename, int nMode );
	void PrepareForWrite();

public:
	int GetMapSize(){ return m_mapPayment.GetSize(); }
	void GetMapItem( int n, CSmartEntPaymentMapEntry& item );

public:
	void UpdateMapItem( CSmartEntPaymentMapEntry& item );
	
private:
	void DirectAddMapKey( int nSmartEntPayNo );
	void SetMapKeyName( int nSmartEntPayNo, const char* szSmartEntPayName );

public:
	int GetSharpPosPaymentNo( int nSEPaymentNo );
	const char* GetSmartEntPaymentName( int nSEPaymentNo );

private:
	CReportConsolidationArray<CSmartEntPaymentMapEntry> m_mapPayment;
	CString m_strSmartEntPaymentName;
};

/**********************************************************************/
