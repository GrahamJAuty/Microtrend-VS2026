#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "GroupCSVArrayBase.h"
/**********************************************************************/
/*	DEPARTMENT GROUP DATABASE										  */
/**********************************************************************/
#define	REPORTGROUPDB_VERSION_NO	2		
/**********************************************************************/

namespace ReportGroup
{
const recInt GroupNo			= { "GroupNo", 0, 99 };
const recString	RepText			= { "RepText", 0, 32 };
const recInt ParentGroupNo		= { "ParentGroupNo", 0, 99 };
};

/**********************************************************************/

class CGroupCSVRecordReport : public CGroupCSVRecordBase
{
public:
	CGroupCSVRecordReport();
	virtual ~CGroupCSVRecordReport(){}

public:
	const recInt GetGroupNoRange(){ return ReportGroup::GroupNo; }
	void SetDefaultGroup( int nGroupNo );

public:
	void ClearRecord();
	virtual void ConvertToCSV ( CCSV& csv );
	virtual void ConvertFromCSV ( CCSV& csv );
	void V1ConvertFromCSV ( CCSV& csv );
	void V2ConvertFromCSV ( CCSV& csv );

public:
	const char* GetEposText(){ return GetRepText(); }
	const char* GetRepText(){ return m_strRepText; }
	int GetParentGroupNo() { return m_nParentGroupNo; }

public:
	void SetEposText( const char* sz ){ SetRepText( sz ); }
	void SetRepText( const char* sz );
	void SetParentGroupNo ( int nGroupNo );
			
private:
	CString m_strRepText;
	int m_nParentGroupNo;
};

/**********************************************************************/

class CGroupCSVArrayReport : public CGroupCSVArrayBase
{
public:
	CGroupCSVArrayReport();

public:
	int Open( const char* szFilename, int nMode );
	void GetGroupInfo( int nIndex, CBaseGroupInfo& info );
	bool GetListDataLine( int nIndex, CString& strLine );
	const recInt GetGroupNoRange(){ return ReportGroup::GroupNo; }
	virtual bool CanEditGroupZero(){ return TRUE; }

public:
	void RememberSettings();
	void RestoreSettings();
};

/**********************************************************************/
