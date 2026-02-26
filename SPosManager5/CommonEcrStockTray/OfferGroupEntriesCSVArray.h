#pragma once
/**********************************************************************/
#include "MinimumSPOSVersion.h"
/**********************************************************************/
#define OFFERGROUPENTRIES_VERSION 2
/**********************************************************************/
#define OFFERENTRY_PLU 1
#define OFFERENTRY_DEPT 2
#define OFFERENTRY_GROUP 3
#define OFFERENTRY_PLU_EXCLUDE 4
#define OFFERENTRY_DEPT_SAMEPLU 5
/**********************************************************************/

namespace OfferGroupEntries
{
const recInt GroupNo			= { "GroupNo", 1, 999 };	
const recInt EntryType			= { "EntryType", 1, 3 };
const recInt64 EntryPluNo		= { "EntryPluNo", 1, 99999999999999 };
const recInt EntryDeptNo		= { "EntryDeptNo", 1, 999 };
const recInt EntryGroupNo		= { "EntryGroupNo", 1, 99 };
const recInt ModifierFlags		= { "ModFlags", 1, 1023 };
};

/**********************************************************************/

class COfferGroupEntriesCSVRecord : public CCSVRecord
{
public:
	COfferGroupEntriesCSVRecord(); 
	virtual ~COfferGroupEntriesCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
public:
	int GetGroupNo() { return m_nGroupNo; }
	int GetEntryType() { return m_nEntryType; }
	__int64 GetEntryPluNo() { return m_nEntryPluNo; }
	int GetEntryDeptNo(){ return m_nEntryDeptNo; }
	int GetEntryGroupNo(){ return m_nEntryGroupNo; }
	int GetModifierFlags() { return m_nModifierFlags; }

	void SetGroupNo( int n );
	void SetEntryPluNo( int nEntryType, __int64 nPluNo );
	void SetEntryDeptNo( int nEntryType, int nDeptNo );
	void SetEntryGroupNo( int nGroupNo );
	void SetModifierFlags( int n );

	void SetSingleModifierFlag( int n );
	bool GetSingleModifierFlag( int n );

	//FOR USE BY SQL IMPORTER ONLY
	void ClearModifierFlags(){ m_nModifierFlags = 0; }
	
public:
	const char* GetEntryLookupKey();
	void MakeDummy( int nGroup );
	void MakeDummy( int nGroup, int nType );

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	void V2ConvertFromCSV( CCSV& csv );

#ifndef POSTRAY_UTILITY

public:
	void CheckMinimumSPOSVersion( CMinimumSPOSVersion& version, __int64& nSharpPluNo, bool& bModifiable );

#endif
	
private:
	int m_nGroupNo;
	
	int m_nEntryType;
	__int64 m_nEntryPluNo;
	int m_nEntryDeptNo;
	int m_nEntryGroupNo;

	int m_nModifierFlags;

private:
	CString m_strKey;
};

/**********************************************************************/

class COfferGroupEntriesCSVArray : public CSharedCSVArray
{
public:
	COfferGroupEntriesCSVArray();
	~COfferGroupEntriesCSVArray();

public:
	int Open( const char* szFilename, int nMode );

public:
	bool FindEntryKey( COfferGroupEntriesCSVRecord& record, int& nIndex );
	bool InsertRecord( COfferGroupEntriesCSVRecord& record, int& nIndex );
	bool CheckPluNo( int nFilterNo, CSQLPluNoInfo& infoPlu );
	bool IsFilterEmpty( int nFilterNo );

public:
	void Validate();
	void PurgeGroup( int nGroupNo );
	void GetGroupRange( int nGroupNo, int& nIdx1, int& nIdx2 );
	void GetGroupSubRange( int nGroupNo, int nType, int& nIdx1, int& nIdx2 );
};

/**********************************************************************/
