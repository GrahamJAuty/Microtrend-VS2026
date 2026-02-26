#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#define SBPAYMAP_VERSION_NO 1
/**********************************************************************/

namespace SportsBookerPaymentMap
{
const recInt SportsBookerPayNo		= { "SportsBookerPayNo", 0, 999 };
const recInt SharpPosPayNo			= { "SharpPosPayNo", 1, 50 };
};

/**********************************************************************/

class CSportsBookerPaymentMapCSVRecord : public CCSVRecord
{
public:
	CSportsBookerPaymentMapCSVRecord();
	virtual ~CSportsBookerPaymentMapCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv );
	virtual void ConvertFromCSV ( CCSV& csv );

public:
	int GetSportsBookerPayNo(){ return m_nSportsBookerPayNo; }
	int GetSharpPosPayNo(){ return m_nSharpPosPayNo; }

public:
	void SetSportsBookerPayNo( int n );
	void SetSharpPosPayNo( int n );
	
private:
	void ClearRecord();
	void V1ConvertFromCSV ( CCSV& csv );

private:
	int m_nSportsBookerPayNo;
	int m_nSharpPosPayNo;
};

/**********************************************************************/

class CSportsBookerPaymentMapCSVArray : public CSharedCSVArray
{
public:
	CSportsBookerPaymentMapCSVArray();
	~CSportsBookerPaymentMapCSVArray();

public:
	int Open( const char* szFilename, int nMode );
	void PrepareForWrite();

public:
	int GetMapSize(){ return m_mapPayment.GetSize(); }
	void GetMapItem( int n, CSortedIntByInt& item );
	void UpdateMapItem( CSortedIntByInt& item );

public:
	int GetSharpPosPaymentNo( int nSBPaymentNo );
	static CString GetSportsBookerText( int nPayNo );

private:
	CReportConsolidationArray<CSortedIntByInt> m_mapPayment;
};

/**********************************************************************/
