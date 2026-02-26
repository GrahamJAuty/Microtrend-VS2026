/**********************************************************************/

CSSUnsortedDatabase::CSSUnsortedDatabase()
{
	m_bIndexed = FALSE;
}

/**********************************************************************/

bool CSSUnsortedDatabase::InsertRecord()
{
	CString strBlank ( m_cSeparator, GetHeaderSize()-1 );	// blank record filled with separators

	return InsertRecord ( strBlank );
}

/**********************************************************************/

bool CSSUnsortedDatabase::InsertRecord ( const char* szCSV )
{
	if ( m_arRecord.InsertRecord ( m_nRow, szCSV, m_cSeparator, m_cQuote, m_nIndexCol ) == TRUE )
	{
		m_bDatabaseModified = TRUE;	// database has been modified
		m_bUpdateView = TRUE;		// need to update view
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

int CSSUnsortedDatabase::AddRecord()
{
	CString strBlank ( m_cSeparator, GetHeaderSize()-1 );	// blank record filled with separators

	return AddRecord ( strBlank );
}

/**********************************************************************/

int CSSUnsortedDatabase::AddRecord ( const char* szCSV )
{
	int nRow = m_arRecord.AddRecord ( szCSV, m_cSeparator, m_cQuote, m_nIndexCol );
	if ( nRow >= 0 )
	{
		m_nRow = nRow;
		m_bDatabaseModified = TRUE;	// database has been modified
		m_bUpdateView = TRUE;		// need to update view
	}
	return nRow;
}

/**********************************************************************/
