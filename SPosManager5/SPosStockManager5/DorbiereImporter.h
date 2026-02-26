/**********************************************************************/
#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CDorbiereException
{
public:
	CDorbiereException();
	void Reset();

public:
	int Compare ( CDorbiereException& source, int nHint = 0 );
	void Add	( CDorbiereException& source );
	
public:
	CString m_strStockCode;
	int m_nReason;
	
public:
	CString m_strDescription;
};

/**********************************************************************/

class CDorbiereImporter
{
public:
	CDorbiereImporter();
	void CheckForImport();

private:
	void CheckForImportInternal();
	void ImportFileStage1( const char* szFilename );
	bool ImportFileStage2( int nSpIdx );

private:
	const char* GetDorbiereString( CCSV& csv, int nField );
	int GetDorbiereInt( CCSV& csv, int nField );
	double GetDorbiereDouble( CCSV& csv, int nField );

private:
	void LogEvent( const char* szEvent1, const char* szEvent2 );

private:
	CString m_strDorbiereString;
};

/**********************************************************************/
#endif
/**********************************************************************/
