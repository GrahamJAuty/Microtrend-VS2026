/**********************************************************************/

#include "atlcomtime.h"

/**********************************************************************/

#define DB_CLOSED		0
#define DB_READONLY		1
#define DB_READWRITE	2

/**********************************************************************/

CSSDatabase::CSSDatabase()
{
	m_bModified = FALSE;
	m_nMode = DB_CLOSED;
	m_strBackupFolder = "";
	m_strLegacyHeaders = "";
	m_nIndexCol = -1;		// not indexed
	m_nRow = -1;
	m_bAscending = TRUE;
	m_bIgnoreCase = FALSE;
	m_bImpliedHeader = FALSE;
	m_strError = "";

	// user modifiable attributes

	m_cSeparator = ',';
	m_cQuote = '"';
	m_cSortSeparator = ',';
}

/**********************************************************************/

CSSDatabase::~CSSDatabase()
{
	Close();
}

/**********************************************************************/

int CSSDatabase::CreateSSDatabase ( const char* szFilename, const char* szBackupFolder, const char* szHeader, const char* szIndexField, int nRetries )
{
	m_bImpliedHeader = FALSE;
	return DoCreate ( szFilename, szBackupFolder, szHeader, szIndexField, nRetries );
}

/**********************************************************************/

int CSSDatabase::CreateSSDatabaseImplied ( const char* szFilename, const char* szBackupFolder, const char* szImpliedHeader, const char* szIndexField, int nRetries )
{
	m_bImpliedHeader = TRUE;
	return DoCreate ( szFilename, szBackupFolder, szImpliedHeader, szIndexField, nRetries );
}

/**********************************************************************/

int CSSDatabase::DoCreate ( const char* szFilename, const char* szBackupFolder, const char* szHeader, const char* szIndexField, int nRetries )
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

			return OpenSSDatabaseReadWriteImplied ( szFilename,szBackupFolder, szHeader, szIndexField, nRetries );
		}
		else
		{
			if ( m_file.WriteLine ( szHeader ) == FALSE )	// write header
			{
				m_file.Close();
				m_strError = "Error writing header";
				return DB_ERR_FILE;
			}

			return OpenSSDatabaseReadWrite ( szFilename, szBackupFolder, szIndexField, nRetries );
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

int CSSDatabase::OpenSSDatabaseReadOnly ( const char* szFilename, const char* szIndexField, int nRetries )
{
	return OpenSSDatabaseReadOnlyImplied ( szFilename, "", szIndexField, nRetries );	// use header in file
}

/**********************************************************************/

int CSSDatabase::OpenSSDatabaseReadOnlyImplied ( const char* szFilename, const char* szImpliedHeader, const char* szIndexField, int nRetries )
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

	m_arrayHeader.RemoveAll();			// clear header array
	m_arrayRecord.RemoveAll();			// clear data array
	m_bModified = FALSE;				// array same as disk
	m_nRow = -1;
	m_nIndexCol = -1;					// not indexed

	if ( m_file.Open ( szFilename, "rb", _SH_DENYNO, nRetries ) == TRUE )		// deny none
	{
		int nReply;
		if (( nReply = ForceRead ( szImpliedHeader, szIndexField )) != 0 )
		{
			m_file.Close();
			return nReply;
		}

		m_file.Close();	// no need to keep file open

		m_arrayRecord.ClearFilter();
		MoveFirst();
		m_nMode = DB_READONLY;
		m_strBackupFolder = "";
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

int CSSDatabase::OpenSSDatabaseReadWrite ( const char* szFilename, const char* szBackupFolder, const char* szIndexField, int nRetries )
{
	return OpenSSDatabaseReadWriteImplied ( szFilename, szBackupFolder, "", szIndexField, nRetries );	// use header in file
}

/**********************************************************************/

int CSSDatabase::OpenSSDatabaseReadWriteImplied ( const char* szFilename, const char* szBackupFolder, const char* szImpliedHeader, const char* szIndexField, int nRetries )
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

	m_arrayHeader.RemoveAll();		// clear header array
	m_arrayRecord.RemoveAll();		// clear data array
	m_bModified = FALSE;			// array same as disk
	m_nRow = -1;
	m_nIndexCol = -1;				// not indexed

	if ( m_file.Open ( szFilename, "r+b", _SH_DENYWR, nRetries ) == TRUE )	// deny write
	{
		int nReply;
		if (( nReply = ForceRead ( szImpliedHeader, szIndexField )) != 0 )
		{
			m_file.Close();
			return nReply;
		}

		m_arrayRecord.ClearFilter();
		MoveFirst();
		m_nMode = DB_READWRITE;
		m_strBackupFolder = szBackupFolder;
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

int CSSDatabase::OpenSSDatabaseReadWriteNewHeader ( const char* szFilename, const char* szBackupFolder, const char* szNewHeader, const char* szIndexField, int nRetries )
{
	return OpenSSDatabaseReadWriteImpliedNewHeader ( szFilename, szBackupFolder, "", szNewHeader, szIndexField, nRetries );
}

/**********************************************************************/

int CSSDatabase::OpenSSDatabaseReadWriteImpliedNewHeader ( const char* szFilename, const char* szBackupFolder, const char* szOldHeader, const char* szNewHeader, const char* szIndexField, int nRetries )
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

	m_arrayHeader.RemoveAll();		// clear header array
	m_arrayRecord.RemoveAll();		// clear data array
	m_bModified = FALSE;			// array same as disk
	m_nRow = -1;
	m_nIndexCol = -1;				// not indexed

	if ( m_file.Open ( szFilename, "r+b", _SH_DENYWR, nRetries ) == TRUE )	// deny write
	{
		int nReply;
		if (( nReply = ForceReadNewHeader ( szOldHeader, szNewHeader, szIndexField )) != 0 )
		{
			m_file.Close();
			return nReply;
		}

		m_arrayRecord.ClearFilter();
		MoveFirst();
		m_nMode = DB_READWRITE;
		m_strBackupFolder = szBackupFolder;
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

bool CSSDatabase::Close()
{
	if ( m_nMode == DB_CLOSED )
		return TRUE;

	bool bReply = TRUE;

	if ( m_nMode == DB_READWRITE )
	{
		bReply = ForceWrite();
		m_file.Close();
	}

	m_arrayHeader.RemoveAll();		// clear header array
	m_arrayRecord.RemoveAll();		// clear data array
	m_bModified = FALSE;			// array same as disk
	m_nMode = DB_CLOSED;
	m_strBackupFolder = "";
	return bReply;
}

/**********************************************************************/

bool CSSDatabase::MoveFirst()
{
	return MoveTo ( 0 );
}

/**********************************************************************/

bool CSSDatabase::MoveNext ( int nCount )
{
	return MoveTo ( m_nRow + nCount );
}

/**********************************************************************/

bool CSSDatabase::MovePrev ( int nCount )
{
	return MoveTo ( m_nRow - nCount );
}

/**********************************************************************/

bool CSSDatabase::MoveLast()
{
	return MoveTo ( m_arrayRecord.GetSize() - 1 );
}

/**********************************************************************/

bool CSSDatabase::MoveTo ( int nRow )
{
	if ( nRow >= 0 && nRow < m_arrayRecord.GetSize() )
	{
		m_nRow = nRow;
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CSSDatabase::DeleteRecord()
{
	if ( m_nRow >= 0 && m_nRow < m_arrayRecord.GetSize() )
	{
		m_arrayRecord.RemoveAt ( m_nRow );
		if (m_nRow >= m_arrayRecord.GetSize())
		{
			m_nRow = m_arrayRecord.GetSize() - 1;
		}

		m_bModified = TRUE;
	}
}

/**********************************************************************/

const char* CSSDatabase::GetString ( const char* szLabel )
{
	if ( m_nRow >= 0 && m_nRow < m_arrayRecord.GetSize() )
	{
		int n = m_arrayHeader.GetIndex(szLabel);
		if (n >= 0)
		{
			return m_arrayRecord.GetAt(m_nRow)->GetString(n);
		}
	}
	return "";
}

/**********************************************************************/

int CSSDatabase::GetInt ( const char* szLabel )
{
	if ( m_nRow >= 0 && m_nRow < m_arrayRecord.GetSize() )
	{
		int n = m_arrayHeader.GetIndex(szLabel);
		if (n >= 0)
		{
			return m_arrayRecord.GetAt(m_nRow)->GetInt(n);
		}
	}
	return 0;
}

/**********************************************************************/

__int64 CSSDatabase::GetInt64 ( const char* szLabel )
{
	if ( m_nRow >= 0 && m_nRow < m_arrayRecord.GetSize() )
	{
		int n = m_arrayHeader.GetIndex(szLabel);
		if (n >= 0)
		{
			return m_arrayRecord.GetAt(m_nRow)->GetInt64(n);
		}
	}
	return 0;
}

/**********************************************************************/

long CSSDatabase::GetLong ( const char* szLabel )
{
	if ( m_nRow >= 0 && m_nRow < m_arrayRecord.GetSize() )
	{
		int n = m_arrayHeader.GetIndex(szLabel);
		if (n >= 0)
		{
			return m_arrayRecord.GetAt(m_nRow)->GetLong(n);
		}
	}
	return 0L;
}

/**********************************************************************/

double CSSDatabase::GetDouble ( const char* szLabel )
{
	if ( m_nRow >= 0 && m_nRow < m_arrayRecord.GetSize() )
	{
		int n = m_arrayHeader.GetIndex(szLabel);
		if (n >= 0)
		{
			return m_arrayRecord.GetAt(m_nRow)->GetDouble(n);
		}
	}
	return 0.0;
}

/**********************************************************************/

bool CSSDatabase::GetBool(const char* szLabel)
{
	if (m_nRow >= 0 && m_nRow < m_arrayRecord.GetSize())
	{
		int n = m_arrayHeader.GetIndex(szLabel);
		if (n >= 0)
		{
			return m_arrayRecord.GetAt(m_nRow)->GetBool(n);
		}
	}
	return FALSE;
}

/**********************************************************************/

void CSSDatabase::Get ( const char* szLabel, CString& strValue )
{
	if ( m_nRow >= 0 && m_nRow < m_arrayRecord.GetSize() )
	{
		int n = m_arrayHeader.GetIndex(szLabel);
		if ( n >= 0 )
		{
			strValue = m_arrayRecord.GetAt(m_nRow)->GetString(n);
			return;
		}
	}
	strValue = "";
}

/**********************************************************************/

void CSSDatabase::Get ( const char* szLabel, int& nValue )
{
	if ( m_nRow >= 0 && m_nRow < m_arrayRecord.GetSize() )
	{
		int n = m_arrayHeader.GetIndex(szLabel);
		if ( n >= 0 )
		{
			nValue = m_arrayRecord.GetAt(m_nRow)->GetInt(n);
			return;
		}
	}
	nValue = 0;
}

/**********************************************************************/

void CSSDatabase::Get ( const char* szLabel, __int64& nValue )
{
	if ( m_nRow >= 0 && m_nRow < m_arrayRecord.GetSize() )
	{
		int n = m_arrayHeader.GetIndex(szLabel);
		if ( n >= 0 )
		{
			nValue = m_arrayRecord.GetAt(m_nRow)->GetInt64(n);
			return;
		}
	}
	nValue = 0;
}

/**********************************************************************/

void CSSDatabase::Get ( const char* szLabel, long& lValue )
{
	if ( m_nRow >= 0 && m_nRow < m_arrayRecord.GetSize() )
	{
		int n = m_arrayHeader.GetIndex(szLabel);
		if ( n >= 0 )
		{
			lValue = m_arrayRecord.GetAt(m_nRow)->GetLong(n);
			return;
		}
	}
	lValue = 0L;
}

/**********************************************************************/

void CSSDatabase::Get ( const char* szLabel, double& dValue )
{
	if ( m_nRow >= 0 && m_nRow < m_arrayRecord.GetSize() )
	{
		int n = m_arrayHeader.GetIndex(szLabel);
		if ( n >= 0 )
		{
			dValue = m_arrayRecord.GetAt(m_nRow)->GetDouble(n);
			return;
		}
	}
	dValue = 0.0;
}

/**********************************************************************/

void CSSDatabase::Get ( const char* szLabel, bool& bValue )
{
	if ( m_nRow >= 0 && m_nRow < m_arrayRecord.GetSize() )
	{
		int n = m_arrayHeader.GetIndex(szLabel);
		if ( n >= 0 )
		{
			bValue = m_arrayRecord.GetAt(m_nRow)->GetBool(n);
			return;
		}
	}
	bValue = FALSE;
}

/**********************************************************************/

void CSSDatabase::Set ( const char* szLabel, const char* szValue )
{
	if ( m_nRow >= 0 && m_nRow < m_arrayRecord.GetSize() )
	{
		int n = m_arrayHeader.GetIndex(szLabel);
		if ( n >= 0 )
		{
			if (m_arrayRecord.GetAt(m_nRow)->SetAt(n, szValue) == TRUE)
			{
				m_bModified = TRUE;
			}
		}
	}
}

/**********************************************************************/

void CSSDatabase::Set ( const char* szLabel, int nValue )
{
	CString strValue = "";
	strValue.Format ( "%d", nValue );
	Set ( szLabel, strValue );
}

/**********************************************************************/

void CSSDatabase::Set ( const char* szLabel, __int64 nValue )
{
	CString strValue = "";
	strValue.Format ( "%I64d", nValue );
	Set ( szLabel, strValue );
}

/**********************************************************************/

void CSSDatabase::Set ( const char* szLabel, long lValue )
{
	CString strValue = "";
	strValue.Format ( "%ld", lValue );
	Set ( szLabel, strValue );
}

/**********************************************************************/

void CSSDatabase::Set ( const char* szLabel, double dValue, int nDp )
{
	CString strValue = "";
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
/*	Display Array routines									*/
/**********************************************************************/

const char* CSSDatabase::DisplayGetField ( int nRow, int nCol )
{
	return m_arrayRecord.DisplayGetField ( nRow, nCol );
}

/**********************************************************************/

void CSSDatabase::DisplaySetField ( int nRow, int nCol, const char* szValue )
{
	if ( nCol != m_nIndexCol )	// not allowed to modify index field except for CSSUnsortedDatabase
	{
		if (m_arrayRecord.DisplaySetField(nRow, nCol, szValue) == TRUE)
		{
			m_bModified = TRUE;
		}
	}
}

/**********************************************************************/

void CSSDatabase::DisplayDeleteRecord ( int nRow )
{
	m_arrayRecord.DisplayDeleteRecord(nRow);
	m_bModified = TRUE;
}

/**********************************************************************/

static int Compare ( const char* sz1, const char* sz2, int nSortType, int nSortDirection )
{
	int nComp = 0;

	return nComp;
}

/**********************************************************************/

void CSSDatabase::DisplaySort()
{
	m_arrayRecord.DisplaySort ( Compare );
}

/**********************************************************************/

void CSSDatabase::DisplaySort ( int (__cdecl *UserCompare)(const char* arg1, const char* arg2, int nType, int nDirection) )
{
	m_arrayRecord.DisplaySort ( UserCompare );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

void CSSDatabase::ClearFilter()
{
	m_arrayRecord.ClearFilter();
}

/**********************************************************************/

void CSSDatabase::Filter ( const char* szFields, bool (__cdecl *UserFilter)(CCSV& csv) )
{
	CUIntArray uiArray;

	CCSV csvFields ( szFields );
	for ( int i = 0 ; i < csvFields.GetSize() ; i++ )
	{
		int nIndex = m_arrayHeader.GetIndex(csvFields.GetString(i));
		if (nIndex >= 0)
		{
			uiArray.Add(nIndex);
		}
	}

	m_arrayRecord.Filter ( uiArray, UserFilter );
}

/**********************************************************************/

void CSSDatabase::ClearSort()
{
	m_arrayRecord.ClearSort();
}

/**********************************************************************/

void CSSDatabase::AddSort ( const char* szField, int nType, int nDirection )
{
	int nIndex = GetHeaderIndex ( szField );
	
	if (nIndex >= 0)
	{
		m_arrayRecord.AddSort(nIndex, nType, nDirection);
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CSSDatabase::InsertColumn ( int nCol, const char* szLabel )
{
	if ( m_arrayHeader.InsertLabel ( nCol, szLabel ) == 0 )
	{
		if ( m_arrayRecord.InsertColumn ( nCol ) == TRUE )
		{
			m_bModified = TRUE;		// database has been modified

			if (nCol <= m_nIndexCol)
			{
				m_nIndexCol++;		// column inserted before sort index
			}

			return TRUE;
		}
	}
	return FALSE;
}

/**********************************************************************/

bool CSSDatabase::RenameColumn ( int nCol, const char* szLabel )
{
	if ( m_arrayHeader.RenameLabel ( nCol, szLabel ) == 0 )
	{
		m_bModified = TRUE;		// database has been modified
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

bool CSSDatabase::DeleteColumn ( int nCol )
{
	if ( nCol != m_nIndexCol )	// cannot deleted indexed column
	{
		if ( m_arrayHeader.DeleteLabel ( nCol ) == 0 )
		{
			if ( m_arrayRecord.DeleteColumn ( nCol ) == TRUE )
			{
				m_bModified = TRUE;		// database has been modified

				if ( nCol < m_nIndexCol )
					m_nIndexCol--;		// column deleted before sort index

				return TRUE;
			}
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
				
				for (int n = 0; n < nMaxCols; n++)
				{
					csvHeader.AddSuffix(strColText, n + 1);
				}

				strHeader = csvHeader.GetLine();
			}
		}

		int nReply;
		if ((nReply = m_arrayHeader.SetHeader(strHeader, m_cSeparator, m_cQuote, m_strLegacyHeaders, m_strError)) != 0)
		{
			return nReply;
		}

		if ((nReply = ParseSortParams(szIndexField)) != 0)
		{
			return nReply;
		}

		m_arrayRecord.RemoveAll();

		CString strBuf = "";
		int i = 0;
		while ( m_file.ReadString ( strBuf ) == TRUE )
		{
			m_arrayRecord.AddRecord ( strBuf, m_cSeparator, m_cQuote, m_nIndexCol );

			if (IsSorted(i) == FALSE)	// virtual function
			{
				return DB_ERR_UNSORTED;
			}

			i++;
		}

		m_bModified = FALSE;	// array same as disk
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
			CMap <CString, LPCTSTR, int, int> mapOldHeader;
			mapOldHeader.InitHashTable(103);
			
			CCSV csvOldHeader(strHeader, m_cSeparator, m_cQuote);
			CCSV csvLegacy(m_strLegacyHeaders);

			for (int n = 0; n < csvOldHeader.GetSize(); n++)
			{
				CString strField = csvOldHeader.GetString(n);
				
				for (int le = 0; le < csvLegacy.GetSize() - 1; le += 2)
				{
					CString strLegacy = csvLegacy.GetString(le);
					if (strField.CompareNoCase(strLegacy) == 0)
					{
						strField = csvLegacy.GetString(le + 1);
						break;
					}
				}

				strField.MakeUpper();
				mapOldHeader.SetAt(strField, n);
			}

			CCSV csvNewHeader ( szNewHeader, m_cSeparator, m_cQuote );

			for ( int i = 0 ; i < csvNewHeader.GetSize() ; i++ )
			{
				CString strField = csvNewHeader.GetString(i);
				strField.MakeUpper();
				
				int nIndex = 0;
				if (mapOldHeader.Lookup(strField, nIndex) != 0 )
				{
					uiNewIndexArray.Add(nIndex);
				}
				else
				{
					uiNewIndexArray.Add(-1);
				}
			}

			strHeader = szNewHeader;		// use new header
			bNewHeader = TRUE;
		}

		int nReply;
		if ((nReply = m_arrayHeader.SetHeader(strHeader, m_cSeparator, m_cQuote, m_strLegacyHeaders, m_strError)) != 0)
		{
			return nReply;
		}

		if ((nReply = ParseSortParams(szIndexField)) != 0)
		{
			return nReply;
		}

		m_arrayRecord.RemoveAll();

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
		
					if (nIndex >= 0)
					{
						csvNew.Add(csvOld.GetString(nIndex));
					}
					else
					{
						csvNew.Add("");		// new field
					}
				}
				strBuf = csvNew.GetLine();
			}

			m_arrayRecord.AddRecord ( strBuf, m_cSeparator, m_cQuote, m_nIndexCol );

			if (IsSorted(i) == FALSE)	// virtual function
			{
				return DB_ERR_UNSORTED;
			}

			i++;
		}

		m_bModified = bNewHeader;

		return DB_ERR_NONE;
	}
	else
	{
		m_strError = "File seek error";
		return DB_ERR_FILE;
	}
}

/**********************************************************************/

void CSSDatabase::WriteBackup()
{
	if ( TRUE == m_bImpliedHeader )
		return;

	CCSV csv( m_strBackupFolder );
	
	if ( csv.GetSize() != 3 )
		return;

	CString strTempFolder = csv.GetString(0);
	if ( strTempFolder == "" )
		return;

	CString strPrefix = csv.GetString(2);
	if ( ( strPrefix.GetLength() < 4 ) || ( strPrefix.GetLength() > 20 ) )
		return;

	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	CString strBackupFilename;
	strBackupFilename.Format( "\\%s_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d_%6.6X.DAT",
		(const char*) strPrefix,
		timeNow.GetYear(),
		timeNow.GetMonth(),
		timeNow.GetDay(),
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond(),
		m_arrayRecord.GetSize() );

	{
		CSSFile fileTemp;
		if ( fileTemp.Open( strTempFolder + strBackupFilename, "wb" ) == FALSE )
			return;

		fileTemp.WriteLine ( m_arrayHeader.GetHeader(m_cSeparator,m_cQuote) );

		for ( int i = 0 ; i < m_arrayRecord.GetSize() ; i++ )
			fileTemp.WriteLine ( m_arrayRecord.GetAt(i)-> GetLine() );
	}
}

/**********************************************************************/

bool CSSDatabase::ForceWrite()
{
	if ( m_bModified == TRUE )
	{
		WriteBackup();

		if ( m_file.Seek ( 0, SEEK_SET ) == 0 )
		{
			if ( m_file.SetLength ( 0L ) == 0 )
			{
				if ( m_bImpliedHeader == FALSE )	// not using implied header
				{
					if ( m_file.WriteLine ( m_arrayHeader.GetHeader(m_cSeparator,m_cQuote) ) == FALSE )
					{
						m_strError = "Error writing header";
						return FALSE;
					}
				}

				for ( int i = 0 ; i < m_arrayRecord.GetSize() ; i++ )
				{
					if ( m_file.WriteLine ( m_arrayRecord.GetAt(i)->GetLine() ) == FALSE )
					{
						m_strError = "Error writing data";
						return FALSE;
					}
				}

				if ( m_file.Flush() == 0 )	// force write to disk
				{
					m_bModified = FALSE;	// array same as disk
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

int CSSDatabase::ParseSortParams ( const char* szIndexField )
{
	if ( m_bIndexed == TRUE )	// not CSSUnsortedDatabase
	{
		m_bAscending = TRUE;		// default
		m_bIgnoreCase = FALSE;		// default

		CCSV csvSort ( szIndexField, m_cSortSeparator );	// separate label,direction,case
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
			m_nIndexCol = m_arrayHeader.GetIndex ( strSortField );	// index to sort on (-1 means not indexed)
			if ( m_nIndexCol == -1 )
			{
				m_strError.Format ( "Unknown sort field: %s", (const char*) strSortField );
				return DB_ERR_SORTPARAMS3;
			}
		}
		else
		{
			m_strError = "Sort field not specified";
			return DB_ERR_SORTPARAMS4;
		}
	}
	return DB_ERR_NONE;
}

/**********************************************************************/
