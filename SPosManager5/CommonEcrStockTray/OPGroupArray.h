#pragma once
/**********************************************************************/
#include "ReportConsolidationArray.h"
/**********************************************************************/
#define OPGROUP_SYSTEM 0
#define OPGROUP_DATABASE 1
#define OPGROUP_LOCATION 2
/**********************************************************************/
#define MAX_OP_GROUP 15
/**********************************************************************/

struct COPGroupMiniInfo
{
public:
	COPGroupMiniInfo();

public:
	int m_nType;
	int m_nGroupNo;
	CString m_strName;
	bool m_bDefer;
	int m_nFlags;
};

/**********************************************************************/

struct COPGroupInfo
{
public:
	COPGroupInfo();
	void Reset();
	void ResetFlags();

public:
	int GetGroupNo(){ return m_nGroupNo; }
	const char* GetName(){ return m_strName; }
	bool GetDeferFlagDatabase(){ return m_bDeferDatabase; }
	bool GetDeferFlagLocation(){ return m_bDeferLocation; }
	//SYSTEM
	int GetEnableFlagsSystem(){ return m_nFlagsSystem; }
	bool GetEnableSystem( int n );
	//DATABASE
	int GetEnableFlagsDatabase(){ return m_nFlagsDatabase; }	
	bool GetEnableDatabase( int n );
	//LOCATION
	int GetEnableFlagsLocation(){ return m_nFlagsLocation; }	
	bool GetEnableLocation( int n );

public:
	void SetGroupNo( int n );
	void SetName( const char* sz );
	void SetDeferFlagDatabase( bool b ){ m_bDeferDatabase = b; }
	void SetDeferFlagLocation( bool b ){ m_bDeferLocation = b; }
	//SYSTEM
	void SetEnableFlagsSystem( int n ){ m_nFlagsSystem = n; }
	void SetEnableSystem( int n, bool b );
	//DATABASE
	void SetEnableFlagsDatabase( int n ){ m_nFlagsDatabase = n; }
	void SetEnableDatabase( int n, bool b );
	//LOCATION
	void SetEnableFlagsLocation( int n ){ m_nFlagsLocation = n; }
	void SetEnableLocation( int n, bool b );

public:
	int Compare ( COPGroupInfo& source, int nHint = 0 );
	void Add	( COPGroupInfo& source );

private:
	int m_nGroupNo;
	CString m_strName;
	bool m_bDeferDatabase;
	bool m_bDeferLocation;
	//SYSTEM
	int m_nFlagsSystem;
	//DATABASE
	int m_nFlagsDatabase;
	//LOCATION
	int m_nFlagsLocation;
};

/**********************************************************************/

class COPGroupTable
{
public:
	COPGroupTable(void);
	void ResetOPGroupList( int nOPGroupMode );

public:
	int GetSize(){ return m_arrayOPGroups.GetSize(); }
	bool GetOPGroup( int nGroupIdx, COPGroupMiniInfo& infoMini );
	bool GetOPGroup( COPGroupMiniInfo& infoMini );

public:
	void UpdateOPGroup( COPGroupMiniInfo& infoMini );
	void UpdateOPGroup( int nGroupIdx, COPGroupMiniInfo& infoMini );

private:
	void AddOPGroup( int nGroupNo, const char* szName );

private:
	CReportConsolidationArray<COPGroupInfo> m_arrayOPGroups;
};

/**********************************************************************/

class COPGroupArray : public CSharedStringArray
{
public:
	COPGroupArray( int nOPGroupMode, COPGroupTable& OPGroupTable );

public:
	int Open( const char* szFilename, int nMode );
	void PrepareForWrite();

public:
	int GetOPGroupCount(){ return m_OPGroupTable.GetSize(); }
	void GetOPGroup( int nGroupIdx, COPGroupMiniInfo& infoMini );
	void GetOPGroup( COPGroupMiniInfo& infoMini );
	void UpdateOPGroup( int nGroupIdx, COPGroupMiniInfo& infoMini );
	void UpdateOPGroup( COPGroupMiniInfo& infoMini );
	void ResetOPGroupList();

private:
	int m_nOPGroupMode;
	COPGroupTable& m_OPGroupTable;
};

/**********************************************************************/
