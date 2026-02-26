#pragma once
/**********************************************************************/
 
/**********************************************************************/
#define PAIDINOUT_VERSION_NO 1
/**********************************************************************/
#define PAIDIO_TEXT_COUNT 20
/**********************************************************************/

namespace PaidInOutText
{
const recString	EposText			= { "EposText", 0, 40 };
};

/**********************************************************************/

class CPaidInOutTextCSVRecord : public CCSVRecord
{
public:
	CPaidInOutTextCSVRecord();
	virtual ~CPaidInOutTextCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv );
	virtual void ConvertFromCSV ( CCSV& csv );

public:
	void SetEposText ( const char* szEposText );

public:
	const char* GetEposText() { return m_strEposText; }
	
private:
	void ClearRecord();
	void V1ConvertFromCSV ( CCSV& csv );

private:
	CString m_strEposText;
};

/**********************************************************************/

class CPaidInOutTextCSVArray : public CSharedCSVArray
{
public:
	CPaidInOutTextCSVArray();

public:
	int Open( const char* szFilename, int nMode );

	void GetPaidInOutText( int nNo, CPaidInOutTextCSVRecord& Record );
	void GetReportPaidInOutText( int nNo, CPaidInOutTextCSVRecord& Record );
	void SetPaidInOutText( int nNo, CPaidInOutTextCSVRecord& Record );

public:
	bool GotFile() { return m_bGotFile; }

private:
	void RemoveAll();
	
public:
	void FillArray();
	
private:
	CString m_strFilename;
	bool m_bGotFile;
};

/**********************************************************************/
