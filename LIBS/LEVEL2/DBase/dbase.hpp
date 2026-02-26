/**********************************************************************/
#pragma once
/**********************************************************************/

#define DB_ERR_NONE			0	// ok
#define DB_ERR_FILE			1	// various file errors			(use GetError() for message)
#define DB_ERR_SHARE		2	// file is in use				(use GetError() for message)
#define DB_ERR_EXISTS		3	// file already exists			(use GetError() for message)
#define DB_ERR_NOTFOUND		4	// file not found				(use GetError() for message)
#define DB_ERR_HEADER1		5	// duplicate label in header		(use GetError() for message)
#define DB_ERR_HEADER2		6	// invalid header (no column text)	(use GetError() for message)
#define DB_ERR_HEADER3		7	// no header in file			(use GetError() for message)
#define DB_ERR_HEADER4		8	// header not specified			(use GetError() for message)
#define DB_ERR_UNSORTED		9	// unsorted record found			(use GetError() for message)
#define DB_ERR_SORTPARAMS1	10	// unknown sort case			(use GetError() for message)
#define DB_ERR_SORTPARAMS2	11	// unknown sort direction		(use GetError() for message)
#define DB_ERR_SORTPARAMS3	12	// unknown sort field			(use GetError() for message)
#define DB_ERR_SORTPARAMS4	13	// sort field not specified		(use GetError() for message)

/**********************************************************************/

#define DB_SORT_ASCENDING	0	// ascending
#define DB_SORT_DESCENDING	1	// descending

#define DB_SORT_TEXT		0	// text (case sensitive)
#define DB_SORT_TEXT_NOCASE	1	// text (case insensitive)
#define DB_SORT_INT			2	// int64
#define DB_SORT_FLOAT		3	// double
#define DB_SORT_DATE		4	// DD*MM*YYyy
#define DB_SORT_TIME		5	// HH*MM*ss

#define DB_SORT_USER1		50	// user sort 1
#define DB_SORT_USER2		51	// user sort 2
#define DB_SORT_USER3		52	// user sort 3
#define DB_SORT_USER4		53	// user sort 4
#define DB_SORT_USER5		54	// user sort 5
#define DB_SORT_USER6		55	// user sort 6
#define DB_SORT_USER7		56	// user sort 7
#define DB_SORT_USER8		57	// user sort 8
#define DB_SORT_USER9		58	// user sort 9
#define DB_SORT_USER10		59	// user sort 10

/**********************************************************************/
/*	private classes for use by CSSDatabase only					*/
/**********************************************************************/

class CSortObj
{
public:
	CSortObj ( int nColumn, int nType, int nDirection );

public:
	int m_nColumn;
	int m_nType;
	int m_nDirection;
};

/**********************************************************************/

class CSortArray : public CPtrArray
{
public:
	CSortArray();
	~CSortArray();
	void SetFunction ( int (__cdecl *pUserCompare)(const char* arg1, const char* arg2, int nType, int nDirection) );
	CSortObj* GetAt ( int nIndex ) { return (CSortObj*) CPtrArray::GetAt(nIndex); }
	void Add ( int nColumn, int nType, int nDirection );
	void RemoveAll();

public:
	int (__cdecl *m_pUserCompare)(const char* arg1, const char* arg2, int nType, int nDirection);
};

/**********************************************************************/

class CHeaderArray
{
public:
	CHeaderArray();
	int SetHeader ( const char* szHeader, char cSeparator, char cQuote, CString& strLegacyHeaders, CString& strError );
	CString GetHeader ( char cSeparator, char cQuote );
	int GetSize() { return m_arrayHeader.GetSize(); }
	int GetIndex ( const char* szLabel );
	CString GetLabel ( int nCol );
	void RemoveAll();
	int InsertLabel ( int nCol, const char* szLabel );
	int RenameLabel ( int nCol, const char* szLabel );
	int DeleteLabel ( int nCol );

private:
	int SetMapArray ( CString& strError );
	bool AddLabel ( const char* szLabel, int nCol );

private:
	CMap <CString, LPCTSTR, int, int> m_Map;
	CStringArray m_arrayHeader;
};

/**********************************************************************/

class CRecordObj : public CObject
{
public:
	CRecordObj ( const char* szRecord, char cSeparator, char cQuote, int nIndexCol );
	~CRecordObj();
	int GetIntKey ( int nIndexCol );
	__int64 GetInt64Key ( int nIndexCol );
	const char* GetStringKey ( int nIndexCol );
	CString GetLine();
	CCSV* GetCSV();

	const char* GetString ( int nCol );
	int GetInt ( int nCol );
	__int64 GetInt64 ( int nCol );
	long GetLong ( int nCol );
	double GetDouble ( int nCol );
	bool GetBool ( int nCol );

	bool SetAt ( int nCol, const char* szValue );
	void InsertAt ( int nCol, const char* szValue );
	void RemoveAt ( int nCol );

private:
	void Parse();

private:
	CString m_strRecord;
	CCSV* m_pCSVRecord;
	bool m_bModified;
	bool m_bParsed;
};

/**********************************************************************/

class CRecordArray : public CObArray
{
public:
	CRecordArray();
	~CRecordArray();
	int AddRecord ( const char* szRecord, char cSeparator, char cQuote, int nIndexCol );
	bool InsertRecord ( int nRow, const char* szRecord, char cSeparator, char cQuote, int nIndexCol );
	void RemoveAll();
	void RemoveAt ( int nRow, int nCount=1 );
	CRecordObj* GetAt ( int nRow ) { return (CRecordObj*) CObArray::GetAt(nRow); }

public:
	void ClearSort();
	void AddSort ( int nColumn, int nType, int nDirection );
	void Sort ( int (__cdecl *UserCompare)(const char* arg1, const char* arg2, int nType, int nDirection) );
	CSortArray* GetSortArray() { return &m_SortArray; }

	void ClearFilter();
	void Filter ( CHeaderArray& arrayHeader, const char* szSQL );
	void Filter ( CUIntArray& nFields, bool (__cdecl *UserFilter)(CCSV& csv) );

	int GetMaxColumns();
	bool InsertColumn ( int nCol );
	bool DeleteColumn ( int nCol );

	int DisplayGetCount();
	const char* DisplayGetField ( int nRow, int nCol );
	bool DisplaySetField ( int nRow, int nCol, const char* szValue );
	void DisplayDeleteRecord ( int nRow );
	void DisplaySort ( int (__cdecl *UserCompare)(const char* arg1, const char* arg2, int nType, int nDirection) );

private:
	void UpdateDisplay();

private:
	CSortArray m_SortArray;
	CPtrArray m_arrayDisplay;
	bool m_bUpdateDisplay;
};

/**********************************************************************/
/*
	szIndexField:	label,direction,case

	direction:	ascending (default)
				descending

	case:		case (default) (not required for int or int64)
				nocase

				e.g.	"PluNo,ascending"
					"Stockcode,ascending,nocase"
*/
/**********************************************************************/
/*	private base class - DO NOT USE!							*/
/**********************************************************************/

class CSSDatabase
{
public:
	CSSDatabase();
	virtual ~CSSDatabase();

	void SetLegacyHeaders(const char* sz) { m_strLegacyHeaders = sz; }
	const char* GetLegacyHeaders() { return m_strLegacyHeaders; }

	int CreateSSDatabase( const char* szFilename, const char* szBackupFolder, const char* szHeader, const char* szIndexField, int nRetries );	// create (file must not exist)
	int OpenSSDatabaseReadOnly ( const char* szFilename, const char* szIndexField, int nRetries );												// read only (file must exist)
	int OpenSSDatabaseReadWrite ( const char* szFilename, const char* szBackupFolder, const char* szIndexField, int nRetries );					// read and write (file must exist)
	int OpenSSDatabaseReadWriteNewHeader ( const char* szFilename, const char* szBackupFolder, const char* szNewHeader, const char* szIndexField, int nRetries );// read and write and modify header (file must exist)
	bool Close();

//	if implied header = "<xxx>" create auto header xxx1,xxx2,xxx3 etc
	int CreateSSDatabaseImplied ( const char* szFilename, const char* szBackupFolder, const char* szImpliedHeader, const char* szIndexField, int nRetries );	// create (file must not exist) (cannot use auto header)
	int OpenSSDatabaseReadOnlyImplied ( const char* szFilename, const char* szImpliedHeader, const char* szIndexField, int nRetries );						// no header in file (use implied header) (file must exist)
	int OpenSSDatabaseReadWriteImplied ( const char* szFilename, const char* szBackupFolder, const char* szImpliedHeader, const char* szIndexField, int nRetries );						// no header in file (use implied header) (file must exist)
	int OpenSSDatabaseReadWriteImpliedNewHeader ( const char* szFilename, const char* szBackupFolder, const char* szOldHeader, const char* szNewHeader, const char* szIndexField, int nRetries );

	void DeleteRecord();
	int GetRecordCount() { return m_arrayRecord.GetSize(); }
	int GetCurrentRow() { return m_nRow; }
	CString GetError() { return m_strError; }

	bool MoveFirst();
	bool MoveNext ( int nCount=1 );
	bool MovePrev ( int nCount=1 );
	bool MoveLast();
	bool MoveTo ( int nRow );

	const char* GetString ( const char* szLabel );
	int GetInt ( const char* szLabel );
	__int64 GetInt64 ( const char* szLabel );
	long GetLong ( const char* szLabel );
	double GetDouble ( const char* szLabel );
	bool GetBool ( const char* szLabel );

	void Get ( const char* szLabel, CString& strValue );
	void Get ( const char* szLabel, int& nValue );
	void Get ( const char* szLabel, __int64& nValue );
	void Get ( const char* szLabel, long& lValue );
	void Get ( const char* szLabel, double& dValue );
	void Get ( const char* szLabel, bool& bValue );

	void Set ( const char* szLabel, const char* szValue );
	void Set ( const char* szLabel, int nValue );
	void Set ( const char* szLabel, __int64 nValue );
	void Set ( const char* szLabel, long lValue );
	void Set ( const char* szLabel, double dValue, int nDp );
	void Set ( const char* szLabel, double dValue, int nDp, bool bMinimize );
	void Set ( const char* szLabel, bool bValue );

public:	// Sort routines
	void ClearSort();
	void AddSort ( const char* szField, int nType, int nDirection );
	CSortArray* GetSortArray() { return m_arrayRecord.GetSortArray(); }

public:	// Filter routines
	void ClearFilter();
	void Filter ( const char* szSQL ) { m_arrayRecord.Filter(m_arrayHeader, szSQL); }
	void Filter ( const char* szFields, bool (__cdecl *UserFilter)(CCSV& csv) );

public:	// Column routines
	int GetHeaderSize() { return m_arrayHeader.GetSize(); }
	CString GetHeaderLabel ( int nCol ) { return m_arrayHeader.GetLabel(nCol); }
	int GetHeaderIndex ( const char* szLabel ) { return m_arrayHeader.GetIndex(szLabel); }
	CString GetHeader() { return m_arrayHeader.GetHeader(m_cSeparator,m_cQuote); }
	int GetMaxColumns() { return m_arrayRecord.GetMaxColumns(); }	// for internal use only
	bool InsertColumn ( int nCol, const char* szLabel );
	bool RenameColumn ( int nCol, const char* szLabel );
	bool DeleteColumn ( int nCol );

public:	// Display array routines
	int DisplayGetCount() { return m_arrayRecord.DisplayGetCount(); }
	const char* DisplayGetField ( int nRow, int nCol );
	void DisplaySetField ( int nRow, int nCol, const char* szValue );
	void DisplayDeleteRecord ( int nRow );
	void DisplaySort();
	void DisplaySort ( int (__cdecl *UserCompare)(const char* arg1, const char* arg2, int nType, int nDirection) );

public:					// user modifiable attributes
	char m_cSeparator;		// default: ','
	char m_cQuote;			// default: '"'
	char m_cSortSeparator;	// default: ','

public:
	bool m_bModified;		// database has been modified

private:
	virtual bool IsSorted ( int i ) = 0;

private:
	int DoCreate ( const char* szFilename, const char* szBackupFolder, const char* szHeader, const char* szIndexField, int nRetries );
	int ForceRead ( const char* szImpliedHeader, const char* szIndexField );
	int ForceReadNewHeader ( const char* szOldHeader, const char* szNewHeader, const char* szIndexField );
	bool ForceWrite();
	int ParseSortParams ( const char* szIndexField );

	void WriteBackup();

protected:
	CRecordArray m_arrayRecord;
	int m_nRow;
	int m_nIndexCol;		// column on which database is indexed
	bool m_bIgnoreCase;		// default: FALSE
	bool m_bAscending;		// default: TRUE
	bool m_bIndexed;		// FALSE for CSSUnsortedDatabase else TRUE
	CString m_strError;

private:
	CSSFile m_file;
	CHeaderArray m_arrayHeader;
	bool m_bImpliedHeader;
	int m_nMode;

private:
	CString m_strBackupFolder;
	
protected:
	CString m_strLegacyHeaders;
};

/**********************************************************************/

class CSSIntDatabase : public CSSDatabase
{
public:
	CSSIntDatabase();
	bool FindRecord ( int nKey ); // returns false if not found
	bool AddBlankRecord ( int nKey ); // returns false if record already exists

private:
	virtual bool IsSorted ( int i );

private:
	int GetIntKey ( int nRow );
	bool Find ( int nKey );
	int SortCompare ( int nKey0, int nKey1 );
};

/**********************************************************************/

class CSSInt64Database : public CSSDatabase
{
public:
	CSSInt64Database();
	bool FindRecord ( __int64 n64Key );	// returns false if not found
	bool AddBlankRecord ( __int64 n64Key );	 // returns false if record already exists

private:
	virtual bool IsSorted ( int i );

private:
	__int64 GetInt64Key ( int nRow );
	bool Find ( __int64 n64Key );
	int SortCompare ( __int64 n64Key0, __int64 n64Key1 );
};

/**********************************************************************/

class CSSStringDatabase : public CSSDatabase
{
public:
	CSSStringDatabase();
	bool FindRecord ( const char* szKey );	// returns false if not found
	bool AddBlankRecord ( const char* szKey );	// returns false if record already exists

protected:
	virtual const char* FormatKey ( const char* szKey ) { return szKey; }	// format for string comparisons

private:
	virtual bool IsSorted ( int i );

private:
	const char* GetStringKey ( int nRow );
	bool Find ( const char* szKey );
	int SortCompare ( const char* szKey0, const char* szKey1 );
};

/**********************************************************************/

class CSSUnsortedDatabase : public CSSDatabase
{
public:
	CSSUnsortedDatabase();
	int CreateSSDatabase ( const char* szFilename, const char* szBackupFolder, const char* szHeader, int nRetries ) { return CSSDatabase::CreateSSDatabase ( szFilename, szBackupFolder, szHeader, "", nRetries ); }
	int OpenSSDatabaseReadOnly ( const char* szFilename, int nRetries ) { return CSSDatabase::OpenSSDatabaseReadOnly ( szFilename, "", nRetries ); }
	int OpenSSDatabaseReadWrite ( const char* szFilename, const char* szBackupFolder, int nRetries ) { return CSSDatabase::OpenSSDatabaseReadWrite ( szFilename, szBackupFolder, "", nRetries ); }
	int OpenSSDatabaseReadWriteNewHeader ( const char* szFilename, const char* szBackupFolder, const char* szNewHeader, int nRetries ) { return CSSDatabase::OpenSSDatabaseReadWriteNewHeader ( szFilename, szBackupFolder, szNewHeader, "", nRetries ); }

//	if implied header = "<xxx>" create auto header xxx1,xxx2,xxx3 etc
	int CreateSSDatabaseImplied ( const char* szFilename, const char* szBackupFolder, const char* szImpliedHeader, int nRetries ) { return CSSDatabase::CreateSSDatabaseImplied ( szFilename, szBackupFolder, szImpliedHeader, "", nRetries ); }				// create (use implied header)
	int OpenSSDatabaseReadOnlyImplied ( const char* szFilename, const char* szImpliedHeader, int nRetries ) { return CSSDatabase::OpenSSDatabaseReadOnlyImplied ( szFilename, szImpliedHeader, "", nRetries ); }		// no header in file (use implied header)
	int OpenSSDatabaseReadWriteImplied ( const char* szFilename, const char* szBackupFolder, const char* szImpliedHeader, int nRetries ) { return CSSDatabase::OpenSSDatabaseReadWriteImplied ( szFilename, szBackupFolder, szImpliedHeader, "", nRetries ); }	// no header in file (use implied header)
	int OpenSSDatabaseReadWriteImpliedNewHeader ( const char* szFilename, const char* szBackupFolder, const char* szOldHeader, const char* szNewHeader, int nRetries ) { return CSSDatabase::OpenSSDatabaseReadWriteImpliedNewHeader ( szFilename, szBackupFolder, szOldHeader, szNewHeader, "", nRetries ); }

	bool InsertRecord();	// insert record at current index position
	int AddBlankRecord();	// add record to end of database

public:
	void Sort();
	void Sort ( int (__cdecl *UserCompare)(const char* arg1, const char* arg2, int nType, int nDirection) );

private:
	virtual bool IsSorted ( int i );
};

/**********************************************************************/

extern const char* szVERSION_DBASE;

/**********************************************************************/
