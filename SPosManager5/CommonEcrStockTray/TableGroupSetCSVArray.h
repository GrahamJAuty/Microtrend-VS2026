#pragma once
/**********************************************************************/
#define TABLEGROUPSET_VERSION 1
/**********************************************************************/
#define MAX_TABLEGROUPSET 15
/**********************************************************************/

namespace SPosTableGroupSet
{
	const recString		GroupSetName =	{ "GroupSetName", 0, 40 };
	const recBool		ShowAsFilter =	{ "ShowAsFilter" };
	const recBool		IncludeUnknown ={ "IncludeUnknown" };
	const recBool		IncludeNonTable={ "IncludeNonTable" };
	const recInt		GroupMask =		{ "GroupMask", 0, 0x7FFFF };
};

/**********************************************************************/

class CTableGroupSetCSVRecord : public CCSVRecord
{
public:
	CTableGroupSetCSVRecord(); 
	virtual ~CTableGroupSetCSVRecord(){}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
public:
	const char* GetGroupSetName( int n);
	bool GetShowAsFilterFlag(int n);
	bool GetIncludeUnknownFlag(int n);
	bool GetIncludeNonTableFlag(int n);
	int GetGroupMask(int n);		

	void SetGroupSetName( int n, const char *sz );
	void SetShowAsFilterFlag( int n, bool b );
	void SetIncludeUnknownFlag( int n, bool b );
	void SetIncludeNonTableFlag( int n, bool b );
	void SetGroupMask( int n, int nMask );
	
	bool IsEmpty( int nLevel ); 

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	
private:
	CString m_strGroupSetName[4];
	bool m_bShowAsFilter[4];
	bool m_bIncludeUnknown[4];
	bool m_bIncludeNonTable[4];
	int m_nGroupMask[4];
};

/**********************************************************************/

class CTableGroupSetCSVArray : public CSharedCSVArray
{
public:
	CTableGroupSetCSVArray();
	~CTableGroupSetCSVArray();

public:
	int Open( const char* szFilename, int nMode );
};

/**********************************************************************/
