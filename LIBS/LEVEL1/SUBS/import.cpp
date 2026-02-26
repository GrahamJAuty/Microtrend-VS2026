
/**********************************************************************/
/*	IMPORT FILE											*/
/**********************************************************************/

CSSImportFile::CSSImportFile ( const char* szDatabaseHeader )
	: m_strDatabaseHeader ( szDatabaseHeader )
{
	m_pImportLine = NULL;
	m_bHeaderRead = FALSE;
	m_bIsValid = FALSE;
}

/**********************************************************************/

CSSImportFile::~CSSImportFile()
{
	if ( m_pImportLine != NULL )
		delete m_pImportLine;
}

/**********************************************************************/

bool CSSImportFile::ReadData ( CString& strBuffer )
{
	if ( m_bHeaderRead == FALSE )
	{
		m_bHeaderRead = TRUE;

		CString strImportHeader;

		if ( ReadString ( strImportHeader ) == TRUE )
		{
			m_pImportLine = new CSSImportLine ( m_strDatabaseHeader, strImportHeader );

			if ( m_pImportLine != NULL )
				m_bIsValid = m_pImportLine->IsValid();
		}
	}

	if ( m_bIsValid == TRUE )
	{
		CString strImportData;

		if ( ReadString ( strImportData ) == TRUE )
		{
			if ( strImportData.Left(1) != "#" )
			{
				m_pImportLine->GetData ( strImportData, strBuffer );
				return TRUE;
			}
			else				// complex header found
			{
				if ( m_pImportLine != NULL )
					delete m_pImportLine;

				m_pImportLine = new CSSImportLine ( m_strDatabaseHeader, strImportData );

				if ( m_pImportLine != NULL )
				{
					m_bIsValid = m_pImportLine->IsValid();

					return ReadData ( strBuffer );		// recursive call
				}
			}
		}
	}

	return FALSE;
}

/**********************************************************************/

void CSSImportFile::Reset ( const char* szDatabaseHeader )
{
	if ( m_pImportLine != NULL )
		delete m_pImportLine;

	m_strDatabaseHeader = szDatabaseHeader;
	m_pImportLine = NULL;
	m_bHeaderRead = FALSE;
	m_bIsValid = FALSE;

	Seek ( 0L, SEEK_SET );
}

/**********************************************************************/

bool CSSImportFile::IsRequired ( int n )
{
	return ( m_pImportLine != NULL ) ? m_pImportLine->IsRequired(n) : FALSE;
}

/**********************************************************************/

const char* CSSImportFile::GetString ( const char* szLabel )
{
	return ( m_pImportLine != NULL ) ? m_pImportLine->GetString(szLabel) : "";
}

int CSSImportFile::GetInt ( const char* szLabel )
{
	return ( m_pImportLine != NULL ) ? m_pImportLine->GetInt(szLabel) : 0;
}

__int64 CSSImportFile::GetInt64 ( const char* szLabel )
{
	return ( m_pImportLine != NULL ) ? m_pImportLine->GetInt64(szLabel) : 0;
}

long CSSImportFile::GetLong ( const char* szLabel )
{
	return ( m_pImportLine != NULL ) ? m_pImportLine->GetLong(szLabel) : 0L;
}

double CSSImportFile::GetDouble ( const char* szLabel )
{
	return ( m_pImportLine != NULL ) ? m_pImportLine->GetDouble(szLabel) : 0.0;
}

bool CSSImportFile::GetBool ( const char* szLabel )
{
	return ( m_pImportLine != NULL ) ? m_pImportLine->GetBool(szLabel) : FALSE;
}

/**********************************************************************/
/*	IMPORT LINE											*/
/**********************************************************************/

CSSImportLine::CSSImportLine ( const char* szDatabaseHeader, const char* szImportHeader )
	: m_csvDatabaseHeader ( szDatabaseHeader )
{
	m_bIsValid = FALSE;		// assume no matching fields

	CCSV csvImportHeader ( szImportHeader );

	for ( int i = 0 ; i < m_csvDatabaseHeader.GetSize() ; i++ )
	{
		int n;
		for ( n = 0 ; n < csvImportHeader.GetSize() ; n++ )
		{
			if ( _strcmpi ( m_csvDatabaseHeader.GetString(i), csvImportHeader.GetString(n) ) == 0 )
			{
				m_nIndexArray.Add ( n );
				m_bIsValid = TRUE;
				break;
			}
		}

		if ( n >= csvImportHeader.GetSize() )
			m_nIndexArray.Add ( -1 );
	}
}

/**********************************************************************/

void CSSImportLine::GetData ( const char* szImportData, CString& strBuffer )
{
	m_csvImportData.ParseLine ( szImportData );

	CCSV csv;

	for ( int i = 0 ; i < m_nIndexArray.GetSize() ; i++ )
	{
		int n = m_nIndexArray.GetAt(i);

		if ( n != -1 )
			csv.Add ( m_csvImportData.GetString(n) );
		else
			csv.Add ( "" );
	}

	strBuffer = csv.GetLine();
}

/**********************************************************************/

bool CSSImportLine::IsRequired ( int n )
{
	if ( n >= 0 && n < m_nIndexArray.GetSize() )
		if ( m_nIndexArray.GetAt(n) != -1 )
			return TRUE;

	return FALSE;
}

/**********************************************************************/

const char* CSSImportLine::GetString ( const char* szLabel )
{
	int n = GetIndex ( szLabel );

	if ( n >= 0 && n < m_nIndexArray.GetSize() )
		if ( m_nIndexArray.GetAt(n) != -1 )
			return m_csvImportData.GetString(m_nIndexArray.GetAt(n));

	return "";
}

int CSSImportLine::GetInt ( const char* szLabel )
{
	int n = GetIndex ( szLabel );

	if ( n >= 0 && n < m_nIndexArray.GetSize() )
		if ( m_nIndexArray.GetAt(n) != -1 )
			return m_csvImportData.GetInt(m_nIndexArray.GetAt(n));

	return 0;
}

__int64 CSSImportLine::GetInt64 ( const char* szLabel )
{
	int n = GetIndex ( szLabel );

	if ( n >= 0 && n < m_nIndexArray.GetSize() )
		if ( m_nIndexArray.GetAt(n) != -1 )
			return m_csvImportData.GetInt64(m_nIndexArray.GetAt(n));

	return 0;
}

long CSSImportLine::GetLong ( const char* szLabel )
{
	int n = GetIndex ( szLabel );

	if ( n >= 0 && n < m_nIndexArray.GetSize() )
		if ( m_nIndexArray.GetAt(n) != -1 )
			return m_csvImportData.GetLong(m_nIndexArray.GetAt(n));

	return 0L;
}

double CSSImportLine::GetDouble ( const char* szLabel )
{
	int n = GetIndex ( szLabel );

	if ( n >= 0 && n < m_nIndexArray.GetSize() )
		if ( m_nIndexArray.GetAt(n) != -1 )
			return m_csvImportData.GetDouble(m_nIndexArray.GetAt(n));

	return 0.0;
}

bool CSSImportLine::GetBool ( const char* szLabel )
{
	int n = GetIndex ( szLabel );

	if ( n >= 0 && n < m_nIndexArray.GetSize() )
		if ( m_nIndexArray.GetAt(n) != -1 )
			return m_csvImportData.GetBool(m_nIndexArray.GetAt(n));

	return FALSE;
}

/**********************************************************************/
/*	private routines										*/
/**********************************************************************/

int CSSImportLine::GetIndex ( const char* szLabel )
{
	if ( strlen ( szLabel ) != 0 )
	{
		for ( int i = 0 ; i < m_csvDatabaseHeader.GetSize() ; i++ )
		{
			if ( _strcmpi ( m_csvDatabaseHeader.GetString(i), szLabel ) == 0 )
				return i;		// found it
		}
	}

	return -1;	// label not found
}

/**********************************************************************/
