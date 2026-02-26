#pragma once
/**********************************************************************/
#include "ReportConsolidationArray.h"
/**********************************************************************/
#define OPNAME_SYSTEM 0
#define OPNAME_DATABASE 1
#define OPNAME_LOCATION 2
/**********************************************************************/
#define MAX_OP_NUMBER 16
/**********************************************************************/

struct COPNameMiniInfo
{
public:
	COPNameMiniInfo();

public:
	int m_nType;
	int m_nPrinterNo;
	CString m_strName;
	bool m_bDefer;
};

/**********************************************************************/

struct COPNameInfo
{
public:
	COPNameInfo();
	void Reset();
	void ResetNames();

public:
	int GetPrinterNo(){ return m_nPrinterNo; }
	bool GetDeferFlagDatabase(){ return m_bDeferDatabase; }
	bool GetDeferFlagLocation(){ return m_bDeferLocation; }
	//SYSTEM
	CString GetNameSystem() { return m_strNameSystem; }
	//DATABASE
	CString GetNameDatabase() { return m_strNameDatabase; }
	//LOCATION
	CString GetNameLocation() { return m_strNameLocation; }

public:
	void SetPrinterNo( int n );
	void SetDeferFlagDatabase( bool b ){ m_bDeferDatabase = b; }
	void SetDeferFlagLocation( bool b ){ m_bDeferLocation = b; }
	//SYSTEM
	void SetNameSystem(CString str);
	//DATABASE
	void SetNameDatabase(CString str);
	//LOCATION
	void SetNameLocation(CString str);

public:
	int Compare ( COPNameInfo& source, int nHint = 0 );
	void Add	( COPNameInfo& source );

private:
	int m_nPrinterNo;
	bool m_bDeferDatabase;
	bool m_bDeferLocation;
	//SYSTEM
	CString m_strNameSystem;
	//DATABASE
	CString m_strNameDatabase;
	//LOCATION
	CString m_strNameLocation;
};

/**********************************************************************/

class COPNameTable
{
public:
	COPNameTable(void);
	void ResetOPNameList( int nOPNameMode );

public:
	int GetSize(){ return m_arrayOPNames.GetSize(); }
	bool GetOPName( int nPrinterIdx, COPNameMiniInfo& infoMini );
	bool GetOPName( COPNameMiniInfo& infoMini );

public:
	void UpdateOPName( COPNameMiniInfo& infoMini );
	void UpdateOPName( int nPrinterIdx, COPNameMiniInfo& infoMini );

private:
	void AddOPName( int nPrinterNo, const char* szName );

private:
	CReportConsolidationArray<COPNameInfo> m_arrayOPNames;
};

/**********************************************************************/

class COPNameArray : public CSharedStringArray
{
public:
	COPNameArray( int nOPNameMode, COPNameTable& OPNameTable );

public:
	int Open( const char* szFilename, int nMode );
	void PrepareForWrite();

public:
	int GetOPNameCount(){ return m_OPNameTable.GetSize(); }
	void GetOPName( int nPrinterIdx, COPNameMiniInfo& infoMini );
	void GetOPName( COPNameMiniInfo& infoMini );
	void UpdateOPName( int nPrinterIdx, COPNameMiniInfo& infoMini );
	void UpdateOPName( COPNameMiniInfo& infoMini );
	void ResetOPNameList();

private:
	int m_nOPNameMode;
	COPNameTable& m_OPNameTable;
};

/**********************************************************************/
