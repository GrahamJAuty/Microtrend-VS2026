#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "GroupCSVArrayBase.h"
/**********************************************************************/
/*	DEPARTMENT GROUP DATABASE										  */
/**********************************************************************/
#define	CONSOLGROUPDB_VERSION_NO	2		
/**********************************************************************/

namespace ConsolGroup
{
const recInt GroupNo			= { "GroupNo", 0, 99 };
const recString	RepText			= { "RepText", 0, 32 };
};

/**********************************************************************/

class CGroupCSVRecordConsol : public CGroupCSVRecordBase
{
public:
	CGroupCSVRecordConsol();
	virtual ~CGroupCSVRecordConsol() {}

public:
	const recInt GetGroupNoRange(){ return ConsolGroup::GroupNo; }
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

public:
	void SetEposText( const char* sz ){ SetRepText( sz ); }
	void SetRepText( const char* sz );

private:
	CString m_strRepText;
};

/**********************************************************************/

class CGroupCSVArrayConsol : public CGroupCSVArrayBase
{
public:
	CGroupCSVArrayConsol();

public:
	int Open( const char* szFilename, int nMode );
	void GetGroupInfo( int nIndex, CBaseGroupInfo& info );
	bool GetListDataLine( int nIndex, CString& strLine );
	const recInt GetGroupNoRange(){ return ConsolGroup::GroupNo; }
	virtual bool CanEditGroupZero(){ return TRUE; }
	
public:
	void RememberSettings();
	void RestoreSettings();
};

/**********************************************************************/