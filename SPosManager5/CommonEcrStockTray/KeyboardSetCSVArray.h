#pragma once
/**********************************************************************/
 
/**********************************************************************/
#define KEYBOARDSET_VERSION 1
/**********************************************************************/

class CKeyboardSetCSVRecord : public CCSVRecord
{
public:
	CKeyboardSetCSVRecord(); 
	virtual ~CKeyboardSetCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
private:
	void SetKbsNo ( int nKbsNo ) { m_nKbsNo = nKbsNo; }
	void SetName ( const char* szName );
	void SetDbNo ( int nDbNo ) { m_nDbNo = nDbNo; }

	const char* GetSystemName();

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
		
private:
	int m_nKbsNo;
	CString m_strName;
	CString m_strSystemName;
	int m_nDbNo;

	friend class CKeyboardSetCSVArray;
};

/**********************************************************************/

class CKeyboardSetCSVArray : public CPtrArray
{
public:
	CKeyboardSetCSVArray();
	~CKeyboardSetCSVArray();
	void RemoveAll();

	bool RenameForDeletion( int nKbsIdx );
	void ClearKeyboardSetFolder( int nKbsIdx );
	void ClearDeletionFolder( int nKbsIdx );
	
	int AddKeyboardSet ( int nDbNo );
	int AddKeyboardSet( int nDbNo, int nKbsNo, const char* szPath );
	bool WriteKeyboardSet( int nRpIdx );

	void DeleteKeyboardSetByNumber ( int nDbNo, int nKbsNo );
	void DeleteKeyboardSetByIndex ( int nKbsIdx, bool bDeleteFiles );
	void PurgeDatabase( int nDbIdx );
	
	bool FindKeyboardSetByNumber ( int nDbNo, int nKbsNo, int& nKbsIdx );
	void GetKbsIdxRange ( int nDbIdx, int& nStartIdx, int& nEndIdx );

	int GetKbsNo( int nKbsIdx );
	int GetDbNo( int nKbsIdx );
	const char* GetName( int nKbsIdx );
	const char* GetSystemName( int nKbsIdx );

	const char* GetFolderPath( int nKbsIdx );
	const char* GetFolderPathDelete( int nKbsIdx );
	const char* GetFolderPathKeyboard( int nKbsIdx, int nKbNo );
	const char* GetFilePathInfo( int nKbsIdx );
	
	const char* GetCSVLine( int nKbsIdx );
	
	void SetKbsNo ( int nKbsIdx, int nKbsNo );
	void SetDbNo ( int nKbsIdx, int nDbNo );
	void SetName ( int nKbsIdx, const char* szName );

	void SortByName( CWordArray& arrayKbsIdx, int nDbIdx = -1 );

public:
	void BuildSortIdxTables();
	int GetSortIdxFromKbsIdx( int nKbsIdx );
	int GetKbsIdxFromSortIdx( int nSortIdx );

private:
	CKeyboardSetCSVRecord* GetKeyboardSet( int nKbsIdx ) { return (CKeyboardSetCSVRecord*) GetAt ( nKbsIdx ); }

private:
	CString m_strFolderPath;
	CString m_strFolderPathDelete;
	CString m_strFilePathInfo;
	CString m_strName;
	CString m_strSystemName;
	CString m_strCSVLine;

	CWordArray m_arraySortIdxToKbsIdx;
	CWordArray m_arrayKbsIdxToSortIdx;
};

/**********************************************************************/
extern CKeyboardSetCSVArray dbKeyboardSet;
/**********************************************************************/

