#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "GroupCSVArrayBase.h"
/**********************************************************************/
#define	ALLERGENDB_VERSION_NO	1	
/**********************************************************************/

namespace Allergen
{
const recInt AllergenNo			= { "AllergenNo", 0, 14 };
const recString	EposText		= { "EposText", 0, 40 };
};

/**********************************************************************/

class CGroupCSVRecordAllergen : public CGroupCSVRecordBase
{
public:
	CGroupCSVRecordAllergen();
	virtual ~CGroupCSVRecordAllergen() {}

public:
	const recInt GetGroupNoRange(){ return Allergen::AllergenNo; }
	void SetDefaultGroup( int nGroupNo );

public:
	void ClearRecord();
	virtual void ConvertToCSV ( CCSV& csv );
	virtual void ConvertFromCSV ( CCSV& csv );
	void V1ConvertFromCSV ( CCSV& csv );

public:
	const char* GetEposText(){ return m_strEposText; }
	const char* GetRepText(){ return GetEposText(); }

public:
	void SetEposText( const char* sz );
	void SetRepText( const char* sz ){ SetEposText( sz ); }

public:
	const char* GetDefaultText();
	const char* GetReportText();

private:
	CString m_strEposText;
};

/**********************************************************************/

class CGroupCSVArrayAllergen : public CGroupCSVArrayBase
{
public:
	CGroupCSVArrayAllergen();

public:
	int Open( const char* szFilename, int nMode );
	void GetGroupInfo( int nIndex, CBaseGroupInfo& info );
	bool GetListDataLine( int nIndex, CString& strLine );
	const recInt GetGroupNoRange(){ return Allergen::AllergenNo; }

public:
	void DirectAddAllergen( int nGroupNo, const char* szName );
	void WriteSMAllergenFile();
	
public:
	void RememberSettings();
	void RestoreSettings();

public:
	void AddAllergenZero();
	void RemoveAllergenZero();
};

/**********************************************************************/
