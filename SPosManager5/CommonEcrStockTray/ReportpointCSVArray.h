#pragma once
/**********************************************************************/
 
/**********************************************************************/
#define REPORTPOINT_VERSION 1
/**********************************************************************/
#define MAX_SHARED_RPNO 60
/**********************************************************************/

class CReportpointCSVRecord : public CCSVRecord
{
public:
	CReportpointCSVRecord(); 
	virtual ~CReportpointCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
private:
	void SetRpNo ( int nRpNo ) { m_nRpNo = nRpNo; }
	void SetName ( const char* szName );
	void SetDbNo ( int nDbNo ) { m_nDbNo = nDbNo; }

	const char* GetSystemName();

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
		
private:
	int m_nRpNo;
	CString m_strName;
	CString m_strSystemName;
	int m_nDbNo;

	friend class CReportpointCSVArray;
};

/**********************************************************************/

class CReportpointCSVArray : public CPtrArray
{
public:
	CReportpointCSVArray();
	~CReportpointCSVArray();
	void RemoveAll();

	bool RenameForDeletion( int nRpIdx );
	void ClearReportpointFolder( int nRpIdx );
	void ClearDeletionFolder( int nRpIdx );
	
	int AddReportpoint ( int nDbNo );
	int AddReportpoint( int nDbNo, int nRpNo, const char* szPath );
	bool WriteReportpoint( int nRpIdx );

	void DeleteReportpointByNumber ( int nDbNo, int nRpNo );
	void DeleteReportpointByIndex ( int nRpIdx, bool bDeleteFiles );
	void PurgeDatabase( int nDbIdx );
	
	bool FindReportpointByNumber ( int nDbNo, int nRpNo, int& nRpIdx );
	void GetRpIdxRange ( int nDbIdx, int& nStartIdx, int& nEndIdx );

	int GetRpNo( int nRpIdx );
	int GetDbNo( int nRpIdx );
	const char* GetName( int nRpIdx );
	const char* GetSystemName( int nRpIdx );

	const char* GetFolderPath( int nRpIdx );
	const char* GetFolderPathDelete( int nRpIdx );
	const char* GetFilePathInfo( int nRpIdx );
	
	const char* GetCSVLine( int nRpIdx );
	
	void SetRpNo ( int nRpIdx, int nRpNo );
	void SetDbNo ( int nRpIdx, int nDbNo );
	void SetName ( int nRpIdx, const char* szName );

	void SortByName( CWordArray& arrayRpIdx, int nDbIdx = -1 );

public:
	void BuildSortIdxTables();
	int GetSortIdxFromRpIdx( int nRpIdx );
	int GetRpIdxFromSortIdx( int nSortIdx );

private:
	CReportpointCSVRecord* GetReportpoint( int nRpIdx ) { return (CReportpointCSVRecord*) GetAt ( nRpIdx ); }

private:
	CString m_strFolderPath;
	CString m_strFolderPathDelete;
	CString m_strFilePathInfo;
	CString m_strName;
	CString m_strSystemName;
	CString m_strCSVLine;

	CWordArray m_arraySortIdxToRpIdx;
	CWordArray m_arrayRpIdxToSortIdx;
};

/**********************************************************************/
extern CReportpointCSVArray dbReportpoint;
/**********************************************************************/
