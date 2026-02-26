#pragma once
/**********************************************************************/
 
/**********************************************************************/
#define LOCATIONSET_VERSION 1
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CLocationSetCSVRecord : public CCSVRecord
{
public:
	CLocationSetCSVRecord(); 
	virtual ~CLocationSetCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv );
		
private:
	void SetLsNo ( int nLsNo ) { m_nLsNo = nLsNo; }
	void SetName ( const char* szName );
	void SetDbNo ( int nDbNo ) { m_nDbNo = nDbNo; }

	void SetNwkLocNoArray( CDWordArray& NwkLocNoArray );
	void RemoveLocation( int nNwkNo, int nLocNo );
	void TidyNwkLocNoArray( CReportConsolidationArray<CSortedIntItem>& LocIdxArray );
	const char* GetSystemName();

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
		
private:
	int m_nLsNo;
	CString m_strName;
	CString m_strSystemName;
	int m_nDbNo;

private:
	CDWordArray m_arrayNwkLocNo;

	friend class CLocationSetCSVArray;
};

/**********************************************************************/

class CLocationSetCSVArray : public CPtrArray
{
public:
	CLocationSetCSVArray();
	~CLocationSetCSVArray();
	void RemoveAll();

	bool RenameForDeletion( int nLsIdx );
	void ClearLocationSetFolder( int nLsIdx );
	void ClearDeletionFolder( int nLsIdx );

	int GetLocCount( int nLsIdx );
	int GetNwkLocNo( int nLsIdx, int n );
	void SetNwkLocNoArray( int nLsIdx, CDWordArray& NwkLocNoArray );
	void TidyNwkLocNoArray( int nLsIdx, CReportConsolidationArray<CSortedIntItem>& LocIdxArray );
	void RemoveLocation( int nLsIdx, int nNwkNo, int nLocNo );
	
	int AddLocationSet ( int nDbNo );
	int AddLocationSet( int nDbNo, int nLsNo, const char* szPath );
	bool WriteLocationSet( int nLsIdx );

	void DeleteLocationSetByNumber ( int nDbNo, int nLsNo );
	void DeleteLocationSetByIndex ( int nLsIdx, bool bDeleteFiles );
	void PurgeDatabase( int nDbIdx );
	
	bool FindLocationSetByNumber ( int nDbNo, int nLsNo, int& nLsIdx );
	void GetLsIdxRange ( int nDbIdx, int& nStartIdx, int& nEndIdx );

	int GetLsNo( int nLsIdx );
	int GetDbNo( int nLsIdx );
	const char* GetName( int nLsIdx );
	const char* GetSystemName( int nLsIdx );

	const char* GetFolderPath( int nLsIdx );
	const char* GetFolderPathDelete( int nLsIdx );
	const char* GetFilePathInfo( int nLsIdx );
	const char* GetCSVLine( int nLsIdx );
	
	void SetLsNo ( int nLsIdx, int nLsNo );
	void SetDbNo ( int nLsIdx, int nDbNo );
	void SetName ( int nLsIdx, const char* szName );

	void SortByName( CWordArray& arrayLsIdx, int nDbIdx = -1 );

private:
	CLocationSetCSVRecord* GetLocationSet( int nLsIdx ) { return (CLocationSetCSVRecord*) GetAt ( nLsIdx ); }

private:
	CString m_strName;
	CString m_strSystemName;
	CString m_strFolderPath;
	CString m_strFolderPathDelete;
	CString m_strFilePathInfo;
	CString m_strCSVLine;
};

/**********************************************************************/
extern CLocationSetCSVArray dbLocationSet;
/**********************************************************************/
