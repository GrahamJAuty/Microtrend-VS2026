#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CPluDatabaseHeaderField
{
public:
	CPluDatabaseHeaderField()
	{
		m_strCurrent = "";
		m_nPriceSetStatus = 0;
		m_nLegacyMapIdx = 0;
		m_bHeaderTag = FALSE;
	}

public:
	CString m_strCurrent;
	int m_nPriceSetStatus;
	int m_nLegacyMapIdx;
	bool m_bHeaderTag;
};

/**********************************************************************/

class CPluDatabaseHeader
{
public:
	CPluDatabaseHeader(void);

public:
	
	//FULL DATABASE HEADER WITH CURRENT FIELD NAMES
	const char* GetDatabaseHeader();
	
	//PRICE SET HEADER WITH CURRENT FIELD NAMES
	const char* GetPriceSetHeader();
	
	//FULL DATABASE HEADER WITH LEGACY FIELD NAMES FOR MODIFIERS
	const char* GetLegacyHeader();
	
	//SUBSET OF FIELDS WITH LEGACY FIELD NAMES FOR MODIFIERS
	const char* GetLegacyHeader( const char* szHeader );
	
	//FULL DATABASE HEADER WITH LEGACY FIELD NAMES IF USED IN FILE HEADER
	const char* GetCompatibilityHeader( const char* szFilename );
	
	//SUBSET OF FIELDS WITH LEGACY FIELD NAMES IF USED IN FILE HEADER
	const char* GetCompatibilityHeader( const char* szFilename, const char* szHeader );
	
	//SUBSET OF FIELDS WITH LEGACY FIELD NAMES UPDATED TO CURRENT
	const char* GetUpdatedHeader( const char* szHeader );

private:
	void AddField( const char* szCurrent, const char* szLegacy, int nPriceSetStatus = 0 );
	void TagLegacyFields( const char* szFilename );
	const char* ReplaceLegacyFields( const char* szHeader );

private:
	CArray<CPluDatabaseHeaderField,CPluDatabaseHeaderField> m_arrayCurrentFields;
	CReportConsolidationArray<CSortedIntByString> m_mapLegacyFields;
	CReportConsolidationArray<CSortedIntByString> m_mapCurrentFields;

private:
	CString m_strHeader;
};

/**********************************************************************/
extern CPluDatabaseHeader PluDatabaseHeader;
/**********************************************************************/
