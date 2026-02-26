/**********************************************************************/
#pragma once
/**********************************************************************/

#include <afxtempl.h>
#include <process.h>
#include <share.h>
#include <io.h>
#include <direct.h>
#include <afxwin.h>

/**********************************************************************/
/*	CSV.CPP												*/
/**********************************************************************/

class CCSV
{
public:
	CCSV ( const char* szCSVLine, char cSeparator = ',', char cQuote = '"', int nMaxFields = 0 );
	CCSV ( char cSeparator, char cQuote, bool bRemoveTrailing, bool bRemoveZeros, bool bMinimizeFloat );
	CCSV ( char cSeparator = ',', char cQuote = '"' );

	CString GetLine ( bool bQuotes = FALSE );
	CString GetLine ( CCSV& csvQuoteArray );
	int GetSize() { return m_strArray.GetSize(); }
	void RemoveAll() { m_strArray.RemoveAll(); }
	void RemoveAt ( int nIndex, int nCount=1 ) { m_strArray.RemoveAt ( nIndex, nCount ); }
	void ParseLine ( const char* szBuf, int nMaxFields = 0 );
	void SetSeparator ( char c ) { m_cSeparator = c; }
	void SetQuote ( char c ) { m_cQuote = c; }

	void Get ( int nIndex, CString& strText );
	void Get ( int nIndex, int& nNum );
	void Get ( int nIndex, __int64& nNum );
	void Get ( int nIndex, long& lNum );
	void Get ( int nIndex, double& dNum );
	void Get ( int nIndex, bool& b );

	const char* GetString ( int nIndex );
	int GetInt ( int nIndex );
	__int64 GetInt64 ( int nIndex );
	long GetLong ( int nIndex );
	double GetDouble ( int nIndex );
	bool GetBool ( int nIndex );

	bool IsEmpty ( int nIndex );

	int Add ( const char* szText );
	int Add ( int nNum );
	int Add ( __int64 nNum );
	int Add ( long lNum );
	int Add ( double dNum, int nDp );
	int Add ( double dNum, int nDp, bool bMinimize );
	int Add ( bool b );

	int AddSuffix ( const char* szText, int nSuffix );

	void SetAt ( int nIndex, const char* szText );
	void SetAt ( int nIndex, int nNum );
	void SetAt ( int nIndex, __int64 nNum );
	void SetAt ( int nIndex, long lNum );
	void SetAt ( int nIndex, double dNum, int nDp );
	void SetAt ( int nIndex, double dNum, int nDp, bool bMinimize );
	void SetAt ( int nIndex, bool b );

	void InsertAt ( int nIndex, const char* szText );
	void InsertAt ( int nIndex, int nNum );
	void InsertAt ( int nIndex, __int64 nNum );
	void InsertAt ( int nIndex, long lNum );
	void InsertAt ( int nIndex, double dNum, int nDp );
	void InsertAt ( int nIndex, double dNum, int nDp, bool bMinimize );
	void InsertAt ( int nIndex, bool b );

private:
	void ParseLine ( CStringArray& strArray, const char* szBuf, int nMaxFields );

	const char* GetField ( int nIndex );

	CString FormatString ( const char* szText );
	CString FormatString ( int nNum );
	CString FormatString ( __int64 nNum );
	CString FormatString ( long lNum );
	CString FormatString ( double dNum, int nDp, bool bMinimize );
	CString FormatString ( bool b );

private:
	CStringArray m_strArray;
	bool m_bRemoveTrailing;
	bool m_bRemoveZeros;
	bool m_bMinimizeFloat;
	char m_cSeparator;
	char m_cQuote;
};

/**********************************************************************/
/*	CSVRECORD.CPP											*/
/**********************************************************************/

struct recString { const char* Label; int Min; int Max; };
struct recInt    { const char* Label; int Min; int Max; };
struct recInt64  { const char* Label; __int64 Min; __int64 Max; };
struct recLong   { const char* Label; long Min; long Max; };
struct recDouble { const char* Label; double Min; double Max; };
struct recMask   { const char* Label; int Mask; };
struct recBool   { const char* Label; };

/**********************************************************************/

void AFXAPI DDV_MaxChars ( CDataExchange* pDX, CString& strValue, const recString& rec );
void AFXAPI DDV_MinMaxInt ( CDataExchange* pDX, int nValue, const recInt& rec );
void AFXAPI DDV_MinMaxLong ( CDataExchange* pDX, long lValue, const recLong& rec );
void AFXAPI DDV_MinMaxDouble ( CDataExchange* pDX, double dValue, const recDouble& rec );

/**********************************************************************/

class CCSVRecord
{
public:
	CCSVRecord() {}
	virtual ~CCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ) {}
	virtual void ConvertFromCSV ( CCSV& csv ) {}

	void Import ( const char* szValue, CString& strValue, const recString& rec );
	void Import ( const char* szValue, int& nValue, const recInt& rec );
	void Import ( const char* szValue, __int64& nValue, const recInt64& rec );
	void Import ( const char* szValue, long& lValue, const recLong& rec );
	void Import ( const char* szValue, double& dValue, const recDouble& rec );
	void Import ( const char* szValue, int& nValue, const recMask& rec );
	void Import ( const char* szValue, bool& bValue );

	void Set ( CString& strRec, const char* szString, const recString& rec );
	void Set ( int& nRec, int n, const recInt& rec );
	void Set ( __int64& nRec, __int64 n, const recInt64& rec );
	void Set ( long& lRec, long l, const recLong& rec );
	void Set ( double& dRec, double d, const recDouble& rec );
	void Set ( int& nRec, int n, const recMask& rec );
	void Set ( bool& bRec, bool b );
};

/**********************************************************************/
/*	CSVARRAY.CPP											*/
/**********************************************************************/

class CCSVArray
{
public:
	CCSVArray ( char cSeparator, char cQuote, bool bRemoveTrailing, bool bRemoveZeros, bool bMinimizeFloat );
	CCSVArray ();		// default = ',', '"', FALSE, FALSE, FALSE
	virtual ~CCSVArray() {}

	bool Read ( const char* szFilename );
	bool Write ( const char* szFilename );
	int GetSize() { return m_Array.GetSize(); }
	const char* GetAt ( int nIndex );
	bool GetAt ( int nIndex, CCSVRecord& Record );
	bool SetAt ( int nIndex, CCSVRecord& Record );
	bool RemoveAt ( int nIndex, int nCount=1 );
	bool InsertAt ( int nIndex, CCSVRecord& Record, int nCount=1 );
	int Add ( CCSVRecord& Record );
	bool IsModified() { return m_bModified; }

protected:
	void SetModifiedFlag ( bool b ) { m_bModified = b; }

private:
	CStringArray m_Array;
	char m_cSeparator;
	char m_cQuote;
	bool m_bRemoveTrailing;
	bool m_bRemoveZeros;
	bool m_bMinimizeFloat;
	bool m_bModified;
};

/**********************************************************************/
/*	STRING.CPP											*/
/**********************************************************************/

void AddLeading ( CString& strBuf, int nWidth, char c );
void AddTrailing ( CString& strBuf, int nWidth, char c );

/**********************************************************************/
/*	FILE.CPP												*/
/**********************************************************************/

class CSSFile
{
public:
	CSSFile();
	virtual ~CSSFile();
	bool Open ( const char* szFilename, const char* szMode );							// fsopen
	bool Open ( const char* szFilename, const char* szMode, int nShare, int nRetries=10 );	// fsopen
	int Close();												// fclose
	int Read ( void* Buffer, int nSize, int nCount );					// fread
	int Write ( const void* Buffer, int nSize, int nCount );			// fwrite
	int GetChar();												// fgetc
	int PutChar ( int nChar );									// fputc
	int Seek ( long lOffset, int nOrigin );							// fseek
	long Tell();												// ftell
	int Flush();												// fflush

	int GetPercentPosition();		/* return file position as a percentage (0-100) */
	long GetLength();				/* return file size (filelength) */
	int SetLength ( long lSize );		/* extend or truncate file size (chsize) */ 

	bool ReadLine ( CString& strBuf );			/* read line including crlf */
	bool WriteLine ( const char* szBuf );		/* add crlf and write */

	bool ReadString ( CString& strBuf );		/* read line and strip crlf */
	bool WriteString ( const char* szBuf );		/* write string dont add crlf */

	bool Read ( CCSV& csv, int nMaxFields=0 );	/* read line and convert to csv */
	bool Write ( CCSV& csv );				/* write csv line with crlf */

private:
	FILE* m_fp;
};

/**********************************************************************/

class CFileStringArray : public CStringArray
{
public:
	~CFileStringArray();
	bool Read ( const char* filename, int max=0 );
	bool Write ( const char* filename );
};

/**********************************************************************/

int MakeSubdirectory ( const char* szPath );
bool ExistSubdirectory ( const char* szPath );
CString SS_PathAppend ( const char* szPath, const char* szMore );

/**********************************************************************/
/*	EXPORT.CPP											*/
/**********************************************************************/

class CSSExportFile : public CSSFile
{
public:
	CSSExportFile ( const char* szComplex="" );
	void Add ( const char* szLabel, const char* szText );
	void Add ( const char* szLabel, int nNum );
	void Add ( const char* szLabel, __int64 nNum );
	void Add ( const char* szLabel, long lNum );
	void Add ( const char* szLabel, double dNum, int nDp );
	void Add ( const char* szLabel, bool b );

	void AddSuffix ( const char* szLabel, int nSuffix, const char* szText );
	void AddSuffix ( const char* szLabel, int nSuffix, int nNum );
	void AddSuffix ( const char* szLabel, int nSuffix, __int64 nNum );
	void AddSuffix ( const char* szLabel, int nSuffix, long lNum );
	void AddSuffix ( const char* szLabel, int nSuffix, double dNum, int nDp );
	void AddSuffix ( const char* szLabel, int nSuffix, bool b );

	bool WriteData();
	void StartNewHeader ( const char* szComplex="" );

private:
	CCSV m_csvHeader;
	CCSV m_csvData;
	CString m_strComplex;
	bool m_bHeaderWritten;
};

/**********************************************************************/
/*	FILTER.CPP											*/
/**********************************************************************/

class CSSExportFilterFile : public CSSFile
{
public:
	CSSExportFilterFile ( const char* szDatabaseHeader, const char* szFilterHeader, const char* szComplex="", bool bHeaderRequired=TRUE );
	bool WriteData ( const char* szDatabaseData );
	void StartNewHeader ( const char* szDatabaseHeader, const char* szFilterHeader, const char* szComplex="" );
	bool WriteHeader();

private:
	void GenerateIndex();
	int GetIndex ( const char* szLabel );

private:
	CCSV m_csvDatabaseHeader;
	CCSV m_csvFilterHeader;
	CString m_strFilterHeader;
	CString m_strComplex;
	CUIntArray m_nIndexArray;
	bool m_bHeaderWritten;
	bool m_bHeaderRequired;
};

/**********************************************************************/
/*	IMPORT.CPP											*/
/**********************************************************************/

class CSSImportLine
{
public:
	CSSImportLine ( const char* szDatabaseHeader, const char* szImportHeader );
	void GetData ( const char* szImportData, CString& strBuffer );
	bool IsValid() { return m_bIsValid; }			// returns FALSE if no matching fields
	bool IsRequired ( int n );

	const char* GetString ( const char* szLabel );	// must only be called after GetData()
	int GetInt ( const char* szLabel );			// must only be called after GetData()
	__int64 GetInt64 ( const char* szLabel );		// must only be called after GetData()
	long GetLong ( const char* szLabel );			// must only be called after GetData()
	double GetDouble ( const char* szLabel );		// must only be called after GetData()
	bool GetBool ( const char* szLabel );			// must only be called after GetData()

private:
	int GetIndex ( const char* szLabel );

private:
	CCSV m_csvDatabaseHeader;
	CCSV m_csvImportData;
	CArray <int, int> m_nIndexArray;
	bool m_bIsValid;
};

/**********************************************************************/

class CSSImportFile : public CSSFile
{
public:
	CSSImportFile( const char* szDatabaseHeader );
	virtual ~CSSImportFile();
	bool ReadData ( CString& strBuffer );
	bool IsRequired ( int n );
	void Reset ( const char* szDatabaseHeader );

	const char* GetString ( const char* szLabel );	// must only be called after ReadData()
	int GetInt ( const char* szLabel );			// must only be called after ReadData()
	__int64 GetInt64 ( const char* szLabel );		// must only be called after ReadData()
	long GetLong ( const char* szLabel );			// must only be called after ReadData()
	double GetDouble ( const char* szLabel );		// must only be called after ReadData()
	bool GetBool ( const char* szLabel );			// must only be called after ReadData()

private:
	CString m_strDatabaseHeader;
	CSSImportLine* m_pImportLine;
	bool m_bHeaderRead;
	bool m_bIsValid;
};

/**********************************************************************/
/*	GENERAL.CPP											*/
/**********************************************************************/

void PumpMessage();
void WaitLoop ( DWORD dwMSecs, bool bDisableSleep=FALSE );	// N.B. only disable sleep when very short very accurate times are required
void WaitExit ( bool& bExit, DWORD dwResolution );	// resolution is in milliseconds
bool WaitExit ( bool& bExit, DWORD dwTimeoutMSecs, DWORD dwResolutionMSecs );	// returns true if exit flag set, false if timeout

const char* SSMakeShortString ( CDC* pDC, const char* szLong, int nWidth );

CString GetLastErrorMessage();
CString GetLastErrorMessage ( DWORD dwError );

int SSspawnl ( CWnd* pWndParent, int mode, const char* cmdname, const char* arg0, const char* arg1, ... );

/**********************************************************************/
/*	BCD.CPP												*/
/**********************************************************************/

void BCDtoAscii ( BYTE* pBCD, char* szBuf, int nBytes );
long BCDtoLong ( BYTE* buf, int len );
long SignedBCDtoLong ( BYTE* buf, int len );
void LongToBCD ( long lNum, BYTE* pBuf, int nLen );
void LongToSignedBCD ( long lNum, BYTE* pBuf, int nLen );

/**********************************************************************/
/*	PROTECT.CPP											*/
/**********************************************************************/

class CProtect1
{
public:
	CProtect1();
	const char* GetCode();
	bool Decode ( const char* szCode );

private:
	int m_nTime;
	CString m_strCode;
};

/**********************************************************************/
/*	LOGFILE.CPP											*/
/**********************************************************************/

class CSSLogFile
{
public:
	CSSLogFile();
	CSSLogFile ( const char* szFilename );
	virtual ~CSSLogFile();

	void WriteLine ( const char* szText );
	void AppendFile ( const char* szFilename );
	void SetFilename ( const char* szFilename );

private:
	void WriteArray();

private:
	CString m_strFilename;
	CStringArray m_strArray;
};

/**********************************************************************/
/*	SHAREDCSVARRAY.CPP										*/
/**********************************************************************/

#define DB_READONLY		1
#define DB_READWRITE	2

#define DB_ERR_NONE		0
#define DB_ERR_FILE		1
#define DB_ERR_SHARE	2

class CSharedCSVArray
{
public:
	CSharedCSVArray ( char cSeparator, char cQuote, bool bRemoveTrailing, bool bRemoveZeros, bool bMinimizeFloat );
	CSharedCSVArray ();		// default = ',', '"', FALSE, FALSE, FALSE
	virtual ~CSharedCSVArray();

	virtual int Open ( const char* szFilename, int nMode );// ret 0=ok, 1=file error, 2=file in use
	bool Close();
	const char* GetError() { return m_strError; }

	int GetSize() { return m_Array.GetSize(); }
	const char* GetAt ( int nIndex );
	bool GetAt ( int nIndex, CCSVRecord& Record );
	bool SetAt ( int nIndex, CCSVRecord& Record );
	bool RemoveAt ( int nIndex, int nCount=1 );
	bool InsertAt ( int nIndex, CCSVRecord& Record, int nCount=1 );
	int Add ( CCSVRecord& Record );
	bool IsModified() { return m_bModified; }

	void SetModifiedFlag ( bool b );	// DB_READWRITE mode only
	bool ForceRead();				// DB_READWRITE mode only
	bool ForceWrite();				// DB_READWRITE mode only

	const char* GetFilename() { return m_strFilename; }

	bool Read ( const char* szFilename );	// for compatibilty with pre v4 software only
	bool Write ( const char* szFilename );	// for compatibilty with pre v4 software only

private:
	CSSFile m_File;
	CStringArray m_Array;
	char m_cSeparator;
	char m_cQuote;
	bool m_bRemoveTrailing;
	bool m_bRemoveZeros;
	bool m_bMinimizeFloat;
	bool m_bModified;
	int m_nMode;
	CString m_strError;
	CString m_strFilename;
};

/**********************************************************************/
/*	SHAREDSTRINGARRAY.CPP									*/
/**********************************************************************/

class CSharedStringArray
{
public:
	CSharedStringArray();
	virtual ~CSharedStringArray();

	virtual int Open ( const char* szFilename, int nMode );			// ret 0=ok, 1=file error, 2=file in use
	virtual int Open ( const char* szFilename, int nMode, int nRetries );	// ret 0=ok, 1=file error, 2=file in use
	bool Close();
	const char* GetError() { return m_strError; }

	int GetSize() { return m_Array.GetSize(); }
	const char* GetAt ( int nIndex );
	void SetAt ( int nIndex, const char* szText );
	void RemoveAt ( int nIndex, int nCount=1 );
	void InsertAt ( int nIndex, const char* szText, int nCount=1 );
	int Add ( const char* szText );
	bool IsModified() { return m_bModified; }

	void SetModifiedFlag ( bool b );	// DB_READWRITE mode only
	bool ForceRead();				// DB_READWRITE mode only
	bool ForceWrite();				// DB_READWRITE mode only

	const char* GetFilename() { return m_strFilename; }

	bool Read ( const char* szFilename );	// for compatibilty with pre v4 software only
	bool Write ( const char* szFilename );	// for compatibilty with pre v4 software only

private:
	CSSFile m_File;
	CStringArray m_Array;
	CString m_strError;
	bool m_bModified;
	int m_nMode;
	CString m_strFilename;
};

/**********************************************************************/

extern const char* szVERSION_SUBS;

/**********************************************************************/
