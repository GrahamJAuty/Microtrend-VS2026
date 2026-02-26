#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "GroupCSVArrayBase.h"
/**********************************************************************/
/*	DEPARTMENT GROUP DATABASE										  */
/**********************************************************************/
#define	EPOSGROUPDB_VERSION_NO	2		
/**********************************************************************/

namespace EposGroup
{
const recInt GroupNo			= { "GroupNo", 0, 99 };
const recString	RepText			= { "RepText", 0, 32 };
const recString	EposText		= { "EposText", 0, 32 };
const recInt SageNominalCode	= { "SageNominalCode", 0, 99999999 };
const recBool SportsBooker		= { "SportsBooker" };
};

/**********************************************************************/

class CGroupCSVRecordEpos : public CGroupCSVRecordBase
{
public:
	CGroupCSVRecordEpos();
	virtual ~CGroupCSVRecordEpos() {}

public:
	const recInt GetGroupNoRange(){ return EposGroup::GroupNo; }
	void SetDefaultGroup( int nGroupNo );

public:
	void ClearRecord();
	virtual void ConvertToCSV ( CCSV& csv );
	virtual void ConvertFromCSV ( CCSV& csv );
	void V1ConvertFromCSV ( CCSV& csv );
	void V2ConvertFromCSV ( CCSV& csv );

public:
	const char* GetEposText(){ return m_strEposText; }
	const char* GetRepText(){ return m_strRepText; }
	void SetSageNominalCode ( int n );
	void SetSportsBookerFlag( bool b );

public:
	void SetEposText( const char* sz );
	void SetRepText( const char* sz );
	int GetSageNominalCode() { return m_nSageNominalCode; }
	bool GetSportsBookerFlag() { return m_bSportsBooker; }
	
public:
	const char* GetReportText();
	
private:
	CString m_strEposText;
	CString m_strRepText;
	int m_nSageNominalCode;
	bool m_bSportsBooker;
};

/**********************************************************************/

class CGroupCSVArrayEpos : public CGroupCSVArrayBase
{
public:
	CGroupCSVArrayEpos();

public:
	int Open( const char* szFilename, int nMode );
	void GetGroupInfo( int nIndex, CBaseGroupInfo& info );
	bool GetListDataLine( int nIndex, CString& strLine );
	const recInt GetGroupNoRange(){ return EposGroup::GroupNo; }

public:
	void RememberSettings();
	void RestoreSettings();

public:
	void AddPMSGroup( int nGroupNo, const char* szText );

public:
	int GetSportsBookerGroupNo();
	void SetSportsBookerGroupNo( int nGroupNo );
	void FixSportsBookerGroupNo();
	
private:
	bool m_bCheckedSportsBookerGroupNo;
	int m_nSportsBookerGroupNo;
};

/**********************************************************************/
