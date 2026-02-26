#pragma once
/**********************************************************************/
 
/**********************************************************************/
#define OFFERGROUPS_VERSION 1
/**********************************************************************/

namespace OfferGroups
{
const recInt GroupNo			= { "GroupNo", 1, 999 };	
const recString GroupName		= { "GroupName", 0, 30 };
};

/**********************************************************************/

class COfferGroupsCSVRecord : public CCSVRecord
{
public:
	COfferGroupsCSVRecord( int nNodeType ); 
	virtual ~COfferGroupsCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
public:
	int GetGroupNo() { return m_nGroupNo; }
	const char* GetGroupName() { return m_strGroupName; }
	const char* GetDisplayName();

	void SetGroupNo( int n );
	void SetGroupName( const char* sz );
	
private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	
private:
	int m_nGroupNo;
	CString m_strGroupName;
	
private:
	int m_nNodeType;
	CString m_strDisplayName;
};

/**********************************************************************/

class COfferGroupsCSVArray : public CSharedCSVArray
{
public:
	COfferGroupsCSVArray( int nNodeType );
	~COfferGroupsCSVArray();

	int Open( const char* szFilename, int nMode );

public:
	bool FindTableByNumber( int nTableNo, int& nTableIdx );
	int FindFirstFreeNumber();
	
private:
	int m_nNodeType;
	CString m_strDisplayName;
};

/**********************************************************************/

