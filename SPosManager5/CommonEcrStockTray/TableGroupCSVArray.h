#pragma once
/**********************************************************************/
#define TABLEGROUP_VERSION 1
/**********************************************************************/
#define MAX_TABLEGROUP 15
/**********************************************************************/

namespace SPosTableGroup
{
	const recString		GroupName =	{ "GroupName", 0, 40 };
	const recBool		ShowAsFilter = { "ShowAsFilter" };
};

/**********************************************************************/

class CTableGroupCSVRecord : public CCSVRecord
{
public:
	CTableGroupCSVRecord(); 
	virtual ~CTableGroupCSVRecord(){}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
public:
	const char* GetGroupName(int n);			
	bool GetShowAsFilterFlag(int n);			

	void SetGroupName( int n, const char *sz );
	void SetShowAsFilterFlag( int n, bool b );

public:
	const char* GetTabbedComboText( int nGroupNo, int n );
	const char* GetPrintReportText( int nGroupNo, int n );
	
private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	
private:
	CString m_strGroupName[4];
	bool m_bShowAsFilter[4];

private:
	CString m_strTabbedComboText;
	CString m_strPrintReportText;
};

/**********************************************************************/

class CTableGroupCSVArray : public CSharedCSVArray
{
public:
	CTableGroupCSVArray();
	~CTableGroupCSVArray();

public:
	int Open( const char* szFilename, int nMode );
};

/**********************************************************************/
