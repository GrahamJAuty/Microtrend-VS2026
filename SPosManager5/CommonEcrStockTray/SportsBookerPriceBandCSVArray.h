#pragma once
/**********************************************************************/
 
/**********************************************************************/
#define SBPRICETEXT_VERSION 1
/**********************************************************************/
#define MAX_SBPRICETEXT_LEN 20
/**********************************************************************/
#define MAX_SBPLU_PRICES 20
/**********************************************************************/

class CSportsBookerPriceBandCSVRecord : public CCSVRecord
{
public:
	CSportsBookerPriceBandCSVRecord(); 
	virtual ~CSportsBookerPriceBandCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
public:
	const char* GetReportText() { return m_strReportText; }
	int GetSharpPosBand(){ return m_nSharpPosBand; }

public:
	void MakeDefaultRecord( int nBand );
	void SetReportText( const char* szText );
	void SetSharpPosBand( int nBand );
	
private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	
private:
	CString m_strReportText;
	int m_nSharpPosBand;
};

/**********************************************************************/

class CSportsBookerPriceBandCSVArray : public CSharedCSVArray
{
public:
	CSportsBookerPriceBandCSVArray();
	~CSportsBookerPriceBandCSVArray();
	int Open( const char* szFilename, int nMode );

public:
	int GetReportMapPriceBand( int nLevel );
	const char* GetDisplayText( int nBand );
	bool IsActivePriceBand( int nBand );

public:
	void GetRecord( int nLevel, CSportsBookerPriceBandCSVRecord& Record );
	void SetRecord( int nLevel, CSportsBookerPriceBandCSVRecord& Record );

private:
	void BuildReportMap();

private:
	bool m_bGotReportMap;
	int m_nPriceBand[ MAX_SBPLU_PRICES ];
	CString m_strDisplayText;
};

/**********************************************************************/
