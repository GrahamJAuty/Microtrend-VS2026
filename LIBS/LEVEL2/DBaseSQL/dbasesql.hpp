/**********************************************************************/
#pragma once
/**********************************************************************/

#define DB_ERR_NONE			0	// ok
#define DB_ERR_FILE			1	// various file errors						(use GetError() for message)
#define DB_ERR_SHARE		2	// file is in use							(use GetError() for message)
#define DB_ERR_EXISTS		3	// file already exists						(use GetError() for message)
#define DB_ERR_NOTFOUND		4	// file not found							(use GetError() for message)
#define DB_ERR_HEADER1		5	// duplicate label in header					(use GetError() for message)
#define DB_ERR_HEADER2		6	// invalid header (no column text)				(use GetError() for message)
#define DB_ERR_HEADER3		7	// no header in file						(use GetError() for message)
#define DB_ERR_HEADER4		8	// header not specified						(use GetError() for message)
#define DB_ERR_HEADER5		9	// invalid header (blank column text not allowed)	(use GetError() for message)
#define DB_ERR_UNSORTED		10	// unsorted record found						(use GetError() for message)
#define DB_ERR_SORTPARAMS1	11	// unknown sort case						(use GetError() for message)
#define DB_ERR_SORTPARAMS2	12	// unknown sort direction					(use GetError() for message)
#define DB_ERR_SORTPARAMS3	13	// unknown sort field						(use GetError() for message)

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
	CSortObj* GetAt ( int nIndex ) { return (CSortObj*) CPtrArray::GetAt(nIndex); }
	void Add ( int nColumn, int nType, int nDirection );
	void RemoveAll();
};

/**********************************************************************/

class CHeaderArray
{
public:
	CHeaderArray();
	int SetHeader ( const char* szHeader, char cSeparator, char cQuote, CString& strError );
	CString GetHeader ( char cSeparator, char cQuote );
	int GetSize() { return m_arrayHeader.GetSize(); }
	int GetIndex ( const char* szLabel );
	CString GetLabel ( int nCol );
	void RemoveAll();
	int AddLabel ( const char* szLabel );
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

	void Add ( const char* szValue );
	CString GetLine();
	CCSV* GetCSV();

	int GetIntKey ( int nIndexCol );
	__int64 GetInt64Key ( int nIndexCol );
	const char* GetStringKey ( int nIndexCol );

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
	bool m_bRecordModified;
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

	int GetMaxColumns();			// scans all records for record with max columns (slow)
	void InsertColumn ( int nCol );
	void DeleteColumn ( int nCol );
	void AddColumn();

	void SaveView ( CUIntArray& arView );
};

/**********************************************************************/
/*	How to iterate a Database:								*/
/**********************************************************************/
/*
old method:		if ( db.MoveFirst() == TRUE )
				{
					do
					{
						do something
					}
					while ( db.MoveNext() == TRUE );
				}

replaced by:		for ( int nRow = 0 ; nRow < db.GetRecordCount() ; nRow++ )
				{
					db.MoveTo ( nRow );		// can't ever be FALSE
					do something
				}

or:				int nRow = 0;
				while ( db.MoveTo ( nRow++ ) == TRUE )
				{
					do something
				}
*/

/**********************************************************************/
/*	How to create an in-memory database:						*/
/**********************************************************************/
/*
	METHOD 1:

	CSSUnsortedDatabase db;							// this method allows duplicate labels
	if ( db.Create ( "x,y" ) == DB_ERR_NONE )
	{
		CSSDatabaseView dbView ( &db );
		dbView.Filter ( "SELECT * FROM this ORDER BY x;" );	// sorting is only performed once when dbView.WriteFile() called

		db.AddRecord ( "One,1" );
		db.AddRecord ( "Two,2" );
		db.AddRecord ( "Three,3" );
		db.AddRecord ( "Four,4" );
		db.AddRecord ( "Five,5" );

		dbView.WriteFile ( "test.dat" );

		db.Close();
	}

	METHOD 2:

	CSSStringDatabase db;							// this method discards duplicate labels
	if ( db.Create ( "x,y" ) == DB_ERR_NONE )			// defaults to x,ascending,case
	{
		db.AddRecord ( "One" );		db.Set ( "y", 1 );
		db.AddRecord ( "Two" );		db.Set ( "y", 2 );
		db.AddRecord ( "Three" );	db.Set ( "y", 3 );
		db.AddRecord ( "Four" );		db.Set ( "y", 4 );
		db.AddRecord ( "Five" );		db.Set ( "y", 5 );

		CSSDatabaseView dbView ( &db );
		dbView.WriteFile ( "test.dat" );

		db.Close();
	}
*/

/**********************************************************************/
/*	CSSDatabase: private base class - DO NOT USE!				*/
/**********************************************************************/
/*
	szIndexField:	""					(for CSSUnsortedDatabase)
	szIndexField:	label,direction		(for CSSIntDatabase, CSSInt64Database)
	szIndexField:	label,direction,case	(for CSSStringDatabase)

	direction:	ascending (default) or descending		eg "PluNo,descending"
	case:		case (default) or nocase				ed "Stockcode,ascending,nocase"
*/
/**********************************************************************/

class CSSDatabase
{
public:
	CSSDatabase();
	virtual ~CSSDatabase();

	int Create ( const char* szFilename, const char* szHeader, const char* szIndexField, int nRetries );		// create (file must not exist)
	int OpenReadOnly ( const char* szFilename, const char* szIndexField, int nRetries );						// read only (file must exist)
	int OpenReadWrite ( const char* szFilename, const char* szIndexField, int nRetries );					// read and write (file must exist)
	int OpenReadWriteNewHeader ( const char* szFilename, const char* szNewHeader, const char* szIndexField, int nRetries );// read and write and modify header (file must exist)
	bool Close();

//	if implied header = "<xxx>" create auto header xxx1,xxx2,xxx3 etc

	int CreateImplied ( const char* szFilename, const char* szImpliedHeader, const char* szIndexField, int nRetries );			// create (file must not exist) (cannot use auto header)
	int OpenReadOnlyImplied ( const char* szFilename, const char* szImpliedHeader, const char* szIndexField, int nRetries );	// no header in file (use implied header) (file must exist)
	int OpenReadWriteImplied ( const char* szFilename, const char* szImpliedHeader, const char* szIndexField, int nRetries );	// no header in file (use implied header) (file must exist)
	int OpenReadWriteImpliedNewHeader ( const char* szFilename, const char* szOldHeader, const char* szNewHeader, const char* szIndexField, int nRetries );

	int Create ( const char* szHeader, const char* szIndexField="" );		// create database in memory only

	CString GetError() { return m_strError; }
	int GetRecordCount() { return m_arRecord.GetSize(); }

	bool MoveTo ( int nRow );

	void DeleteRecord();

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

	int AddColumn ( const char* szLabel );
	bool DeleteColumn ( const char* szLabel );
	bool RenameColumn ( const char* szLabel, const char* szNewLabel );
	bool InsertColumn ( const char* szLabel, const char* szNewLabel );

public:						// user modifiable attributes
	char m_cSeparator;			// default: ','
	char m_cQuote;				// default: '"'
	char m_cIndexSeparator;		// default: ','	(separator for the IndexField when creating/opening a database)

public:
	bool m_bDatabaseModified;	// database has been modified

	// the following functions are not for general use

public:
	void InitDatabase();							// needed for disp.exe Document/View architecture
	void DeleteRecords ( CUIntArray& arRow );
	void SaveView ( CUIntArray& arView );			// for use by CSSDatabaseView only

	int GetMaxColumns() { return m_arRecord.GetMaxColumns(); }	// slow - has to look at every record
	CRecordObj* GetRecord ( int nRow ) { return m_arRecord.GetAt(nRow); }

	CString GetHeader() { return m_arHeader.GetHeader(m_cSeparator,m_cQuote); }
	int GetHeaderSize() { return m_arHeader.GetSize(); }
	CString GetHeaderLabel ( int nCol ) { return m_arHeader.GetLabel(nCol); }
	int GetHeaderIndex ( const char* szLabel ) { return m_arHeader.GetIndex(szLabel); }

private:
	virtual bool IsSorted ( int i ) { return FALSE; }

private:
	int DoCreate ( const char* szFilename, const char* szHeader, const char* szIndexField, int nRetries );
	int ForceRead ( const char* szImpliedHeader, const char* szIndexField );
	int ForceReadNewHeader ( const char* szOldHeader, const char* szNewHeader, const char* szIndexField );
	bool ForceWrite();
	int SetIndexField ( const char* szIndexField );

public:
	bool m_bUpdateView;		// need to update view (for use by CSSDatabaseView only)

protected:
	CRecordArray m_arRecord;
	int m_nRow;
	int m_nIndexCol;		// column on which database is indexed
	bool m_bIgnoreCase;		// default: FALSE
	bool m_bAscending;		// default: TRUE
	bool m_bIndexed;		// FALSE for CSSUnsortedDatabase else TRUE
	CString m_strError;

private:
	CSSFile m_file;
	CHeaderArray m_arHeader;
	bool m_bImpliedHeader;
	int m_nMode;
};

/**********************************************************************/

class CSSIntDatabase : public CSSDatabase
{
public:
	CSSIntDatabase();
	bool FindRecord ( int nKey );			// returns false if not found
	bool AddRecord ( int nKey );			// insert record in sorted position, returns false if record already exists

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
	bool AddRecord ( __int64 n64Key );		// insert record in sorted position, returns false if record already exists

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
	bool AddRecord ( const char* szKey );	// insert record in sorted position, returns false if record already exists

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

	int AddRecord();						// add record to end of database
	int AddRecord ( const char* szCSV );		// add record to end of database

	bool InsertRecord();					// insert record at current index position
	bool InsertRecord ( const char* szCSV );	// insert record at current index position

private:
	virtual bool IsSorted ( int i ) { return TRUE; }		// pretend to be sorted so no error generated when loading file
};

/**********************************************************************/
/*	How to Filter or Sort a Database:							*/
/**********************************************************************/
/*
	CSSIntDatabase or CSSInt64Database or CSSStringDatabase or CSSUnsortedDatabase db;

	CSSDatabaseView dbView ( &db );
	if ( dbView.Filter ( "SELECT * FROM this ORDER BY Customer;" ) == TRUE )
	{
		dbView.SaveView();		// if you want to replace the original database with the view
							// otherwise just display it
	}
*/
/**********************************************************************/
/*	How to iterate a View:									*/
/**********************************************************************/
/*
	CSSIntDatabase or CSSInt64Database or CSSStringDatabase or CSSUnsortedDatabase db;

	CSSDatabaseView dbView ( &db );
	for ( int nRowView = 0 ; nRowView < dbView.GetRecordCount() ; nRowView++ )
	{
		for ( int nColView = 0 ; nColView < dbView.GetColumnCount() ; nColView++ )
			dbView.GetField ( nRowView, nColView ) );
	}
*/
/**********************************************************************/

class CSSDatabaseView
{
public:
	CSSDatabaseView();
	CSSDatabaseView ( CSSDatabase* pDB );		// don't need to call CreateView if you use this constructor

	void CreateView ( CSSDatabase* pDB );
	int GetRecordCount();
	int GetDatabaseRow ( int nRowView ) { return m_arView.GetAt(nRowView); }
	int GetColumnCount();
	CString GetHeaderLabel ( int nColView );
	const char* GetField ( int nRowView, int nColView );
	bool SetField ( int nRowView, int nColView, const char* szValue );

	bool Filter ( const char* szSQL );
	void ClearFilter();

	bool IsModified();
	bool WriteFile ( const char* szFilename, bool bImpliedHeader=FALSE );	// write view to disk
	void SaveView();						// replace database with view (don't use with anything other 
										// than CSSUnsortedDatabase unless sorted by Key Field)
	CString GetError() { return m_strError; }

private:
	CRecordObj* GetRecord ( int nRowView );
	bool TestExpression ( const char* szCSV );
	bool TestWhere ( CStringArray& arWhere );
	bool Select ( CCSV& csvFields, CStringArray& arWhere );
	bool SelectDistinct ( CCSV& csvFields, CStringArray& arWhere );
	void OrderBy ( CString strOrderBy, CString strOrderByDirection );
	void Limit ( CString strLimit );
	void AddSort ( int nColumn, int nType, int nDirection );
	void ClearSort();
	void Sort();
	void Update();

public:
	CSSDatabase* GetDatabase() { return m_pDB; }				// for internal use only
	CSortArray* GetSortArray() { return &m_arSortFields; }		// for internal use only
	void InitView();									// for internal use only

private:
	CUIntArray m_arHeader;
	CUIntArray m_arView;
	CSortArray m_arSortFields;
	CSSDatabase* m_pDB;
	CString m_strSQL;
	CString m_strError;
};

/**********************************************************************/
/*	Sample SQL statements									*/
/**********************************************************************/
/*
	SELECT [DISTINCT] FROM [WHERE] {ORDER BY] [LIMIT]		(fields inside [] are optional)

	SELECT * FROM custlist.csv WHERE CustNo>=5000 AND CustNo<6000 ORDER BY Dealer,CustNo:int ASC/DESC LIMIT 100

	SELECT * FROM custlist.csv WHERE CustNo>=5000 AND CustNo<6000 ORDER BY Dealer,CustNo:int ASC/DESC

	SELECT * FROM custlist.csv WHERE Dealer='CCM'

	SELECT CustNo,Dealer FROM custlist.csv WHERE Dealer='CCM' OR Dealer='SCI'

	SELECT * FROM custlist.csv WHERE Customer LIKE '%golf%'		(select everything containing the word golf)

	SELECT * FROM custlist.csv WHERE Customer NOT LIKE '%school%'	(select everything not containing the word school)

	wildcard 'test%' wildcards can only be used in LIKE statement
	LIKE statement is not case dependent

	ORDER BY Dealer:text,CustNo:int DESC

		:text	(case sensitive)	// default
		:texti	(ignore case)
		:int		(int64)
		:float	(double)
		:date	(DD*MM*YYyy)		// * = any non-numeric character, lower case optional
		:time	(HH*MM*ss*mmm)		// * = any non-numeric character, lower case optional (mmm = milliseconds)
		:ipv4	(n.n.n.n)

	SELECT DISTINCT Col FROM filename

	SELECT DISTINCT Col1,Col2 FROM filename ORDER BY Col1

	SELECT DISTINCT * FROM filename	(will discard duplicate records)
*/
/**********************************************************************/

class CSQL
{
public:
	CSQL();

	bool SetSQL ( const char* szSQL );									// eg SELECT * FROM this WHERE CustNo>=5000 AND CustNo<6000 ORDER BY Dealer;
	CString GetSQL();												// default = "SELECT * FROM this;"

	// the following functions allow you to build or modify an sql statement

	void SetCommand ( const char* szCommand );							// eg SELECT (default) or SELECT DISTINCT
	void AddSelect ( const char* szColumnName );							// eg CustNo
	void SetFrom ( const char* szFilename ) { m_strFrom = szFilename; }		// eg custlist.csv
	void AddAndWhere ( const char* szWhere );							// eg CustNo>=5000
	void AddOrWhere ( const char* szWhere );							// eg CustNo>=5000
	void SetWhere ( const char* szWhere ) { m_strWhere = szWhere; }			// eg CustNo>=5000 AND CustNo<6000
	void AddOrderBy ( const char* szColumnName, const char* szSortType );		// eg CustNo,int
	void SetOrderByDirection ( const char* szDirection );					// eg ASC or DESC
	void SetLimit ( int nLimit );										// eg 100

	void ClearAll();
	void ClearSelect() { m_strSelect = ""; }
	void ClearWhere() { m_strWhere = ""; }
	void ClearOrderBy() { m_strOrderBy = ""; m_strOrderByDirection = ""; }

	// the following functions are not for general use

public:
	CString GetCommand() { return m_strCommand; }
	CString GetSelect() { return m_strSelect; }
	CString GetFrom() { return m_strFrom; }
	CString GetWhere() { return m_strWhere; }
	CString GetOrderBy() { return m_strOrderBy; };
	CString GetOrderByDirection() { return m_strOrderByDirection; };
	CString GetLimit() { return m_strLimit; }

private:
	void AddWhere ( const char* szAndOr, const char* szWhere );

private:
	CString m_strCommand;			// eg "SELECT" or "SELECT DISTINCT"
	CString m_strSelect;			// eg "CustNo,Customer,Dealer,Comment"
	CString m_strFrom;				// eg "custlist.csv"
	CString m_strWhere;				// eg "CustNo>=5000 AND CustNo<6000 AND Customer LIKE '%school%'"
	CString m_strOrderBy;			// eg "Dealer,Customer"
	CString m_strOrderByDirection;	// eg "DESC"
	CString m_strLimit;				// eg "100"
};

/**********************************************************************/

extern const char* szVERSION_DBASESQL;

/**********************************************************************/
