#pragma once
/**********************************************************************/
#define PRICETEXT_VERSION 1
/**********************************************************************/
#define MAX_PRICETEXT_LEN 20
/**********************************************************************/

class CPriceTextCSVRecord : public CCSVRecord
{
public:
	CPriceTextCSVRecord(); 
	virtual ~CPriceTextCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
public:
	const char* GetReportText() { return m_strReportText; }
	
public:
	void SetReportText( const char* szText );
	
private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	
private:
	CString m_strReportText;
};

/**********************************************************************/

class CPriceTextCSVArray : public CSharedCSVArray
{
public:
	CPriceTextCSVArray();
	~CPriceTextCSVArray();
	int Open( const char* szFilename, int nMode );

public:
	const char* GetReportText( int nLevel );
	const char* GetTabbedComboText( int nLevel );

public:
	void SetReportText( int nLevel, const char* szText );

private:
	CString m_strReportText;
	CString m_strTabbedComboText;
};

/**********************************************************************/
