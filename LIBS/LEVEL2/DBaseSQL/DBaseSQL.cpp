/**********************************************************************/

#define DB_CLOSED		0
#define DB_READONLY		1
#define DB_READWRITE	2

/**********************************************************************/

CSSDatabase::CSSDatabase()
{
	InitDatabase();

	// user modifiable attributes (don't want InitDatabase to change these)

	m_cSeparator = ',';
	m_cQuote = '"';
	m_cIndexSeparator = ',';
}

void CSSDatabase::InitDatabase()
{
	m_arHeader.RemoveAll();		// clear header array
	m_arRecord.RemoveAll();		// clear data array

	m_bDatabaseModified = FALSE;
	m_bUpdateView = FALSE;
	m_nMode = DB_CLOSED;
	m_nIndexCol = -1;		// not indexed
	m_nRow = -1;
	m_bAscending = TRUE;
	m_bIgnoreCase = FALSE;
	m_bImpliedHeader = FALSE;
	m_strError = "";
}

/**********************************************************************/

CSSDatabase::~CSSDatabase()
{
	Close();
}

/**********************************************************************/

bool CSSDatabase::Close()
{
	bool bReply = TRUE;

	if ( m_nMode == DB_READWRITE )
	{
		bReply = ForceWrite();
		m_file.Close();
	}

	m_arHeader.RemoveAll();		// clear header array
	m_arRecord.RemoveAll();		// clear data array
	m_bDatabaseModified = FALSE;	// array same as disk
	m_nMode = DB_CLOSED;

	return bReply;
}

/**********************************************************************/

int CSSDatabase::Create ( const char* szHeader, const char* szIndexField )		// create database in memory only
{
	InitDatabase();

	int nReply;
	if (( nReply = m_arHeader.SetHeader ( szHeader, m_cSeparator, m_cQuote, m_strError )) == 0 )
	{
		nReply = SetIndexField ( szIndexField );
	}

	return nReply;
}

/**********************************************************************/

int CSSDatabase::Create ( const char* szFilename, const char* szHeader, const char* szIndexField, int nRetries )
{
	m_bImpliedHeader = FALSE;
	return DoCreate ( szFilename, szHeader, szIndexField, nRetries );
}

/**********************************************************************/

int CSSDatabase::CreateImplied ( const char* szFilename, const char* szImpliedHeader, const char* szIndexField, int nRetries )
{
	m_bImpliedHeader = TRUE;
	return DoCreate ( szFilename, szImpliedHeader, szIndexField, nRetries );
}

/**********************************************************************/

int CSSDatabase::DoCreate ( const char* szFilename, const char* szHeader, const char* szIndexField, int nRetries )
{
	if ( strlen(szHeader) == 0 )
	{
		m_strError = "Header not specified";
		return DB_ERR_HEADER4;
	}

	if ( m_nMode != DB_CLOSED )
	{
		m_strError = "File is already open";
		return DB_ERR_FILE;
	}

	if ( _access ( szFilename, 0 ) == 0 )	// file exists
	{
		m_strError = "File already exists";
		return DB_ERR_EXISTS;
	}

	if ( m_file.Open ( szFilename, "w+b", _SH_DENYWR, nRetries ) == TRUE )	// create file
	{
		if ( m_bImpliedHeader == TRUE )
		{
			m_file.Close();

			return OpenReadWriteImplied ( szFilename, szHeader, szIndexField, nRetries );
		}
		else
		{
			if ( m_file.WriteLine ( szHeader ) == FALSE )	// write header
			{
				m_file.Close();
				m_strError = "Error writing header";
				return DB_ERR_FILE;
			}

			return OpenReadWrite ( szFilename, szIndexField, nRetries );
		}
	}
	else
	{
		DWORD dwError = GetLastError();
		if ( dwError == ERROR_SHARING_VIOLATION || dwError == 0 )
		{
			m_strError = "File is in use";
			return DB_ERR_SHARE;
		}
		else
		{
			m_strError = GetLastErrorMessage ( dwError );
			return DB_ERR_FILE;
		}
	}
}

/**********************************************************************/

int CSSDatabase::OpenReadOnly ( const char* szFilename, const char* szIndexField, int nRetries )
{
	return OpenReadOnlyImplied ( szFilename, "", szIndexField, nRetries );	// use header in file
}

/**********************************************************************/

int CSSDatabase::OpenReadOnlyImplied ( const char* szFilename, const char* szImpliedHeader, const char* szIndexField, int nRetries )
{
	if ( m_nMode != DB_CLOSED )
	{
		m_strError = "File is already open";
		return DB_ERR_FILE;
	}

	if ( _access ( szFilename, 0 ) != 0 )	// file does not exist
	{
		m_strError = "File not found";
		return DB_ERR_NOTFOUND;
	}

	m_arHeader.RemoveAll();			// clear header array
	m_arRecord.RemoveAll();			// clear data array
	m_bDatabaseModified = FALSE;		// array same as disk
	m_nRow = -1;
	m_nIndexCol = -1;				// not indexed

	if ( m_file.Open ( szFilename, "rb", _SH_DENYNO, nRetries ) == TRUE )		// deny none
	{
		int nReply;
		if (( nReply = ForceRead ( szImpliedHeader, szIndexField )) != 0 )
		{
			m_file.Close();
			return nReply;
		}

		m_file.Close();	// no need to keep file open

		MoveTo ( 0 );
		m_nMode = DB_READONLY;
		m_strError = "Ok";
		return DB_ERR_NONE;
	}
	else
	{
		DWORD dwError = GetLastError();
		if ( dwError == ERROR_SHARING_VIOLATION || dwError == 0 )
		{
			m_strError = "File is in use";
			return DB_ERR_SHARE;
		}
		else
		{
			m_strError = GetLastErrorMessage ( dwError );
			return DB_ERR_FILE;
		}
	}
}

/**********************************************************************/

int CSSDatabase::OpenReadWrite ( const char* szFilename, const char* szIndexField, int nRetries )
{
	return OpenReadWriteImplied ( szFilename, "", szIndexField, nRetries );	// use header in file
}

/**********************************************************************/

int CSSDatabase::OpenReadWriteImplied ( const char* szFilename, const char* szImpliedHeader, const char* szIndexField, int nRetries )
{
	if ( m_nMode != DB_CLOSED )
	{
		m_strError = "File is already open";
		return DB_ERR_FILE;
	}

	if ( _access ( szFilename, 0 ) != 0 )	// file does not exist
	{
		m_strError = "File not found";
		return DB_ERR_NOTFOUND;
	}

	m_arHeader.RemoveAll();		// clear header array
	m_arRecord.RemoveAll();		// clear data array
	m_bDatabaseModified = FALSE;	// array same as disk
	m_nRow = -1;
	m_nIndexCol = -1;			// not indexed

	if ( m_file.Open ( szFilename, "r+b", _SH_DENYWR, nRetries ) == TRUE )	// deny write
	{
		int nReply;
		if (( nReply = ForceRead ( szImpliedHeader, szIndexField )) != 0 )
		{
			m_file.Close();
			return nReply;
		}

		MoveTo ( 0 );
		m_nMode = DB_READWRITE;
		m_strError = "Ok";
		return DB_ERR_NONE;
	}
	else
	{
		DWORD dwError = GetLastError();
		if ( dwError == ERROR_SHARING_VIOLATION || dwError == 0 )
		{
			m_strError = "File is in use";
			return DB_ERR_SHARE;
		}
		else
		{
			m_strError = GetLastErrorMessage ( dwError );
			return DB_ERR_FILE;
		}
	}
}

/**********************************************************************/

int CSSDatabase::OpenReadWriteNewHeader ( const char* szFilename, const char* szNewHeader, const char* szIndexField, int nRetries )
{
	return OpenReadWriteImpliedNewHeader ( szFilename, "", szNewHeader, szIndexField, nRetries );
}

/**********************************************************************/

int CSSDatabase::OpenReadWriteImpliedNewHeader ( const char* szFilename, const char* szOldHeader, const char* szNewHeader, const char* szIndexField, int nRetries )
{
	if ( strlen ( szNewHeader ) == 0 )
	{
		m_strError = "Header not specified";
		return DB_ERR_HEADER4;
	}

	if ( m_nMode != DB_CLOSED )
	{
		m_strError = "File is already open";
		return DB_ERR_FILE;
	}

	if ( _access ( szFilename, 0 ) != 0 )	// file does not exist
	{
		m_strError = "File not found";
		return DB_ERR_NOTFOUND;
	}

	m_arHeader.RemoveAll();		// clear header array
	m_arRecord.RemoveAll();		// clear data array
	m_bDatabaseModified = FALSE;	// array same as disk
	m_nRow = -1;
	m_nIndexCol = -1;			// not indexed

	if ( m_file.Open ( szFilename, "r+b", _SH_DENYWR, nRetries ) == TRUE )	// deny write
	{
		int nReply;
		if (( nReply = ForceReadNewHeader ( szOldHeader, szNewHeader, szIndexField )) != 0 )
		{
			m_file.Close();
			return nReply;
		}

		MoveTo ( 0 );
		m_nMode = DB_READWRITE;
		m_strError = "Ok";
		return DB_ERR_NONE;
	}
	else
	{
		DWORD dwError = GetLastError();
		if ( dwError == ERROR_SHARING_VIOLATION || dwError == 0 )
		{
			m_strError = "File is in use";
			return DB_ERR_SHARE;
		}
		else
		{
			m_strError = GetLastErrorMessage ( dwError );
			return DB_ERR_FILE;
		}
	}
}

/**********************************************************************/

bool CSSDatabase::MoveTo ( int nRow )
{
	if ( nRow >= 0 && nRow < m_arRecord.GetSize() )
	{
		m_nRow = nRow;
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CSSDatabase::DeleteRecord()
{
	if ( m_nRow >= 0 && m_nRow < m_arRecord.GetSize() )
	{
		m_arRecord.RemoveAt ( m_nRow );
		if ( m_nRow >= m_arRecord.GetSize() )
			m_nRow = m_arRecord.GetSize()-1;

		m_bDatabaseModified = TRUE;	// database has been modified
		m_bUpdateView = TRUE;		// need to update view
	}
}

/**********************************************************************/

static int CompareIntAscending ( void* context, const void* arg1, const void* arg2 )
{
	int n1 = *(int*) arg1;
	int n2 = *(int*) arg2;
	return n1 - n2;
}

void CSSDatabase::DeleteRecords ( CUIntArray& arRow )
{
	if ( arRow.GetSize() > 0 )
	{
		qsort_s ( arRow.GetData(), arRow.GetSize(), sizeof(void*), CompareIntAscending, NULL );	// sort ascending

		for ( int i = arRow.GetSize()-1 ; i >= 0 ; i-- )		// need to delete in reverse order
			m_arRecord.RemoveAt ( arRow.GetAt(i) );
			
		if ( m_nRow >= m_arRecord.GetSize() )
			m_nRow = m_arRecord.GetSize()-1;

		m_bDatabaseModified = TRUE;		// database has been modified
		m_bUpdateView = TRUE;			// need to update view
	}
}

/**********************************************************************/

const char* CSSDatabase::GetString ( const char* szLabel )
{
	if ( m_nRow >= 0 && m_nRow < m_arRecord.GetSize() )
	{
		int n;
		if (( n = m_arHeader.GetIndex ( szLabel )) >= 0 )
			return m_arRecord.GetAt(m_nRow)->GetString(n);
	}
	return "";
}

/**********************************************************************/

int CSSDatabase::GetInt ( const char* szLabel )
{
	if ( m_nRow >= 0 && m_nRow < m_arRecord.GetSize() )
	{
		int n;
		if (( n = m_arHeader.GetIndex ( szLabel )) >= 0 )
			return m_arRecord.GetAt(m_nRow)->GetInt(n);
	}
	return 0;
}

/**********************************************************************/

__int64 CSSDatabase::GetInt64 ( const char* szLabel )
{
	if ( m_nRow >= 0 && m_nRow < m_arRecord.GetSize() )
	{
		int n;
		if (( n = m_arHeader.GetIndex ( szLabel )) >= 0 )
			return m_arRecord.GetAt(m_nRow)->GetInt64(n);
	}
	return 0;
}

/**********************************************************************/

long CSSDatabase::GetLong ( const char* szLabel )
{
	if ( m_nRow >= 0 && m_nRow < m_arRecord.GetSize() )
	{
		int n;
		if (( n = m_arHeader.GetIndex ( szLabel )) >= 0 )
			return m_arRecord.GetAt(m_nRow)->GetLong(n);
	}
	return 0L;
}

/**********************************************************************/

double CSSDatabase::GetDouble ( const char* szLabel )
{
	if ( m_nRow >= 0 && m_nRow < m_arRecord.GetSize() )
	{
		int n;
		if (( n = m_arHeader.GetIndex ( szLabel )) >= 0 )
			return m_arRecord.GetAt(m_nRow)->GetDouble(n);
	}
	return 0.0;
}

/**********************************************************************/

bool CSSDatabase::GetBool ( const char* szLabel )
{
	if ( m_nRow >= 0 && m_nRow < m_arRecord.GetSize() )
	{
		int n;
		if (( n = m_arHeader.GetIndex ( szLabel )) >= 0 )
			return m_arRecord.GetAt(m_nRow)->GetBool(n);
	}
	return FALSE;
}

/**********************************************************************/

void CSSDatabase::Get ( const char* szLabel, CString& strValue )
{
	if ( m_nRow >= 0 && m_nRow < m_arRecord.GetSize() )
	{
		int n;
		if (( n = m_arHeader.GetIndex ( szLabel )) >= 0 )
		{
			strValue = m_arRecord.GetAt(m_nRow)->GetString(n);
			return;
		}
	}
	strValue = "";
}

/**********************************************************************/

void CSSDatabase::Get ( const char* szLabel, int& nValue )
{
	if ( m_nRow >= 0 && m_nRow < m_arRecord.GetSize() )
	{
		int n;
		if (( n = m_arHeader.GetIndex ( szLabel )) >= 0 )
		{
			nValue = m_arRecord.GetAt(m_nRow)->GetInt(n);
			return;
		}
	}
	nValue = 0;
}

/**********************************************************************/

void CSSDatabase::Get ( const char* szLabel, __int64& nValue )
{
	if ( m_nRow >= 0 && m_nRow < m_arRecord.GetSize() )
	{
		int n;
		if (( n = m_arHeader.GetIndex ( szLabel )) >= 0 )
		{
			nValue = m_arRecord.GetAt(m_nRow)->GetInt64(n);
			return;
		}
	}
	nValue = 0;
}

/**********************************************************************/

void CSSDatabase::Get ( const char* szLabel, long& lValue )
{
	if ( m_nRow >= 0 && m_nRow < m_arRecord.GetSize() )
	{
		int n;
		if (( n = m_arHeader.GetIndex ( szLabel )) >= 0 )
		{
			lValue = m_arRecord.GetAt(m_nRow)->GetLong(n);
			return;
		}
	}
	lValue = 0L;
}

/**********************************************************************/

void CSSDatabase::Get ( const char* szLabel, double& dValue )
{
	if ( m_nRow >= 0 && m_nRow < m_arRecord.GetSize() )
	{
		int n;
		if (( n = m_arHeader.GetIndex ( szLabel )) >= 0 )
		{
			dValue = m_arRecord.GetAt(m_nRow)->GetDouble(n);
			return;
		}
	}
	dValue = 0.0;
}

/**********************************************************************/

void CSSDatabase::Get ( const char* szLabel, bool& bValue )
{
	if ( m_nRow >= 0 && m_nRow < m_arRecord.GetSize() )
	{
		int n;
		if (( n = m_arHeader.GetIndex ( szLabel )) >= 0 )
		{
			bValue = m_arRecord.GetAt(m_nRow)->GetBool(n);
			return;
		}
	}
	bValue = FALSE;
}

/**********************************************************************/

void CSSDatabase::Set ( const char* szLabel, const char* szValue )
{
	if ( m_nRow >= 0 && m_nRow < m_arRecord.GetSize() )
	{
		int n;
		if (( n = m_arHeader.GetIndex ( szLabel )) >= 0 )
		{
			if ( m_arRecord.GetAt(m_nRow)->SetAt(n,szValue) == TRUE )
			{
				m_bDatabaseModified = TRUE;	// database has been modified
				m_bUpdateView = TRUE;		// need to update view
			}
		}
	}
}

/**********************************************************************/

void CSSDatabase::Set ( const char* szLabel, int nValue )
{
	CString strValue;
	strValue.Format ( "%d", nValue );
	Set ( szLabel, strValue );
}

/**********************************************************************/

void CSSDatabase::Set ( const char* szLabel, __int64 nValue )
{
	CString strValue;
	strValue.Format ( "%I64d", nValue );
	Set ( szLabel, strValue );
}

/**********************************************************************/

void CSSDatabase::Set ( const char* szLabel, long lValue )
{
	CString strValue;
	strValue.Format ( "%ld", lValue );
	Set ( szLabel, strValue );
}

/**********************************************************************/

void CSSDatabase::Set ( const char* szLabel, double dValue, int nDp )
{
	CString strValue;
	strValue.Format ( "%.*f", nDp, dValue );
	Set ( szLabel, strValue );
}

/**********************************************************************/

void CSSDatabase::Set ( const char* szLabel, double dValue, int nDp, bool bMinimize )
{
	CString strValue;
	strValue.Format ( "%.*f", nDp, dValue );
	if ( nDp > 0 && bMinimize == TRUE )
	{
		strValue.TrimRight ( '0' );
		strValue.TrimRight ( '.' );
	}
	Set ( szLabel, strValue );
}

/**********************************************************************/

void CSSDatabase::Set ( const char* szLabel, bool bValue )
{
	CString strValue;
	strValue = ( bValue == FALSE ) ? "0" : "1";
	Set ( szLabel, strValue );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

int CSSDatabase::AddColumn ( const char* szLabel )
{
	int nIndex = -1;

	if ( m_arHeader.AddLabel ( szLabel ) == 0 )
	{
		m_arRecord.AddColumn();

		m_bDatabaseModified = TRUE;	// database has been modified
		m_bUpdateView = TRUE;		// need to update view

		nIndex = GetHeaderSize()-1;	// index of added column
	}

	return nIndex;
}

/**********************************************************************/

bool CSSDatabase::DeleteColumn ( const char* szLabel )
{
	int nCol = m_arHeader.GetIndex(szLabel);
	if ( nCol >= 0 )
	{
		if ( nCol != m_nIndexCol )	// cannot deleted indexed column
		{
			m_arHeader.DeleteLabel ( nCol );

			m_bDatabaseModified = TRUE;	// database has been modified
			m_bUpdateView = TRUE;		// need to update view

			if ( nCol < m_nIndexCol )
				m_nIndexCol--;		// column deleted before sort index

			return TRUE;
		}
	}

	return FALSE;
}

/**********************************************************************/

bool CSSDatabase::RenameColumn ( const char* szLabel, const char* szNewLabel )
{
	int nCol = m_arHeader.GetIndex ( szLabel );
	if ( nCol >= 0 )
	{
		if ( m_arHeader.RenameLabel ( nCol, szNewLabel ) == 0 )
		{
			m_bDatabaseModified = TRUE;	// database has been modified
			m_bUpdateView = TRUE;		// need to update view

			return TRUE;
		}
	}

	return FALSE;
}

/**********************************************************************/

bool CSSDatabase::InsertColumn ( const char* szLabel, const char* szNewLabel )
{
	int nCol = m_arHeader.GetIndex ( szLabel );
	if ( nCol >= 0 )
	{
		if ( m_arHeader.InsertLabel ( nCol, szNewLabel ) == 0 )
		{
			m_arRecord.InsertColumn ( nCol );

			m_bDatabaseModified = TRUE;	// database has been modified
			m_bUpdateView = TRUE;		// need to update view

			if ( nCol <= m_nIndexCol )
				m_nIndexCol++;		// column inserted before sort index

			return TRUE;
		}
	}

	return FALSE;
}

/**********************************************************************/
/*	private routines										*/
/**********************************************************************/

int CSSDatabase::ForceRead ( const char* szImpliedHeader, const char* szIndexField )
{
	if ( m_file.Seek ( 0L, SEEK_SET ) == 0 )
	{
		CString strHeader = szImpliedHeader;
		m_bImpliedHeader = ( strHeader != "" ) ? TRUE : FALSE;

		if ( m_bImpliedHeader == FALSE )		// not using implied header
		{
			if ( m_file.ReadString ( strHeader ) == FALSE )		// use real header from file
			{
				m_strError = "No header in file";
				return DB_ERR_HEADER3;
			}
		}

		if ( m_bImpliedHeader == TRUE )
		{
			strHeader = szImpliedHeader;

			if ( strHeader.Left(1) == "<" && strHeader.Right(1) == ">" )		// auto generate column headings
			{
				CString strBuf;
				int nMaxCols = 0;
				while ( m_file.ReadString(strBuf) == TRUE )	// find max columns
				{
					CCSV csv ( strBuf, m_cSeparator );
					nMaxCols = max ( csv.GetSize(), nMaxCols );
				}

				m_file.Seek ( 0L, SEEK_SET );		// put file pointer back to start

				CString strColText = strHeader.Mid(1,strHeader.GetLength()-2);	 // extract text between <>
				CCSV csvHeader ( m_cSeparator );
				for ( int n = 0 ; n < nMaxCols ; n++ )
					csvHeader.AddSuffix ( strColText, n+1 );

				strHeader = csvHeader.GetLine();
			}
		}

		int nReply;
		if (( nReply = m_arHeader.SetHeader ( strHeader, m_cSeparator, m_cQuote, m_strError )) != 0 )
			return nReply;

		if (( nReply = SetIndexField ( szIndexField )) != 0 )
			return nReply;

		m_arRecord.RemoveAll();

		CString strBuf;
		int i = 0;
		while ( m_file.ReadString ( strBuf ) == TRUE )
		{
			m_arRecord.AddRecord ( strBuf, m_cSeparator, m_cQuote, m_nIndexCol );

			if ( IsSorted ( i ) == FALSE )	// virtual function
				return DB_ERR_UNSORTED;
			i++;
		}

		m_bDatabaseModified = FALSE;	// array same as disk
		return DB_ERR_NONE;
	}
	else
	{
		m_strError = "File seek error";
		return DB_ERR_FILE;
	}
}

/**********************************************************************/

int CSSDatabase::ForceReadNewHeader ( const char* szOldHeader, const char* szNewHeader, const char* szIndexField )
{
	if ( m_file.Seek ( 0L, SEEK_SET ) == 0 )
	{
		CString strHeader = szOldHeader;
		m_bImpliedHeader = ( strHeader != "" ) ? TRUE : FALSE;

		if ( m_bImpliedHeader == FALSE )		// not using implied header
		{
			if ( m_file.ReadString ( strHeader ) == FALSE )		// use real header from file
			{
				m_strError = "No header in file";
				return DB_ERR_HEADER3;
			}
		}

		bool bNewHeader = FALSE;
		CUIntArray uiNewIndexArray;

		if ( strHeader != szNewHeader )	// new header is different
		{
			CCSV csvOldHeader ( strHeader, m_cSeparator, m_cQuote );
			CCSV csvNewHeader ( szNewHeader, m_cSeparator, m_cQuote );

			for ( int i = 0 ; i < csvNewHeader.GetSize() ; i++ )
			{
				int j;
				for ( j = 0 ; j < csvOldHeader.GetSize() ; j++ )
				{
					if ( strcmp ( csvNewHeader.GetString(i), csvOldHeader.GetString(j) ) == 0 )
					{
						uiNewIndexArray.Add ( j );
						break;
					}
				}
				if ( j == csvOldHeader.GetSize() )	// not found
					uiNewIndexArray.Add ( -1 );
			}
			strHeader = szNewHeader;		// use new header
			bNewHeader = TRUE;
		}

		int nReply;
		if (( nReply = m_arHeader.SetHeader ( strHeader, m_cSeparator, m_cQuote, m_strError )) != 0 )
			return nReply;

		if (( nReply = SetIndexField ( szIndexField )) != 0 )
			return nReply;

		m_arRecord.RemoveAll();

		CString strBuf;
		int i = 0;
		while ( m_file.ReadString ( strBuf ) == TRUE )
		{
			if ( bNewHeader == TRUE )
			{
				CCSV csvOld ( strBuf, m_cSeparator, m_cQuote );
				CCSV csvNew ( "", m_cSeparator, m_cQuote );

				for ( int j = 0 ; j < uiNewIndexArray.GetSize() ; j++ )
				{
					int nIndex = uiNewIndexArray.GetAt(j);
					if ( nIndex >= 0 )
						csvNew.Add ( csvOld.GetString ( nIndex ));
					else
						csvNew.Add ( "" );		// new field
				}
				strBuf = csvNew.GetLine();
			}

			m_arRecord.AddRecord ( strBuf, m_cSeparator, m_cQuote, m_nIndexCol );

			if ( IsSorted ( i ) == FALSE )	// virtual function
				return DB_ERR_UNSORTED;
			i++;
		}

		if ( bNewHeader == FALSE )
			m_bDatabaseModified = FALSE;		// array same as disk
		else
			m_bDatabaseModified = TRUE;		// want new database format to be written

		return DB_ERR_NONE;
	}
	else
	{
		m_strError = "File seek error";
		return DB_ERR_FILE;
	}
}

/**********************************************************************/

bool CSSDatabase::ForceWrite()
{
	if ( m_bDatabaseModified == TRUE )
	{
		if ( m_file.Seek ( 0, SEEK_SET ) == 0 )
		{
			if ( m_file.SetLength ( 0L ) == 0 )
			{
				if ( m_bImpliedHeader == FALSE )	// not using implied header
				{
					if ( m_file.WriteLine ( m_arHeader.GetHeader(m_cSeparator,m_cQuote) ) == FALSE )
					{
						m_strError = "Error writing header";
						return FALSE;
					}
				}

				for ( int i = 0 ; i < m_arRecord.GetSize() ; i++ )
				{
					if ( m_file.WriteLine ( m_arRecord.GetAt(i)->GetLine() ) == FALSE )
					{
						m_strError = "Error writing data";
						return FALSE;
					}
				}

				if ( m_file.Flush() == 0 )	// force write to disk
				{
					m_bDatabaseModified = FALSE;	// array same as disk
					return TRUE;
				}
			}
		}
		else
		{
			m_strError = "File seek error";
			return FALSE;
		}
	}
	return TRUE;
}

/**********************************************************************/

int CSSDatabase::SetIndexField ( const char* szIndexField )
{
	if ( m_bIndexed == TRUE )	// everything except CSSUnsortedDatabase
	{
		m_bAscending = TRUE;		// default
		m_bIgnoreCase = FALSE;		// default

		CCSV csvSort ( szIndexField, m_cIndexSeparator );		// extract label,direction,case
		int nParams = csvSort.GetSize();

		if ( nParams >= 3 )		// case
		{
			CString strSortCase = csvSort.GetString(2);
			if ( strSortCase == "case" )
				m_bIgnoreCase = FALSE;
			else if ( strSortCase == "nocase" )
				m_bIgnoreCase = TRUE;
			else
			{
				m_strError.Format ( "Unknown sort case: %s", (const char*) strSortCase );
				return DB_ERR_SORTPARAMS1;
			}
		}
		if ( nParams >= 2 )		// direction
		{
			CString strSortDirection = csvSort.GetString(1);
			if ( strSortDirection == "ascending" )
				m_bAscending = TRUE;
			else if ( strSortDirection == "descending" )
				m_bAscending = FALSE;
			else
			{
				m_strError.Format ( "Unknown sort direction: %s", (const char*) strSortDirection );
				return DB_ERR_SORTPARAMS2;
			}
		}
		if ( nParams >= 1 )		// field
		{
			CString strSortField = csvSort.GetString(0);
			m_nIndexCol = m_arHeader.GetIndex ( strSortField );	// index to sort on (-1 means not indexed)
			if ( m_nIndexCol == -1 )
			{
				m_strError.Format ( "Unknown sort field: %s", (const char*) strSortField );
				return DB_ERR_SORTPARAMS3;
			}
		}
		else		// nParams = 0
		{
			m_nIndexCol = 0;		// assume first column if label not specified
		}
	}
	else						// CSSUnsortedDatabase only
		m_nIndexCol = -1;		// not indexed
		
	return DB_ERR_NONE;
}

/**********************************************************************/

void CSSDatabase::SaveView ( CUIntArray& arView )
{
	m_arRecord.SaveView ( arView );
	m_bDatabaseModified = TRUE;
}

/**********************************************************************/
