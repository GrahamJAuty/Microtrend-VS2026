/**********************************************************************/

CSSUnsortedDatabase::CSSUnsortedDatabase()
{
	m_bIndexed = FALSE;
}

/**********************************************************************/

bool CSSUnsortedDatabase::InsertRecord()
{
	CString strBlank ( m_cSeparator, GetHeaderSize()-1 );	// blank record filled with separators
	if ( m_arrayRecord.InsertRecord ( m_nRow, strBlank, m_cSeparator, m_cQuote, m_nIndexCol ) == TRUE )
	{
		m_bModified = TRUE;
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

int CSSUnsortedDatabase::AddBlankRecord()
{
	CString strBlank ( m_cSeparator, GetHeaderSize()-1 );	// blank record filled with separators
	int nRow = m_arrayRecord.AddRecord ( strBlank, m_cSeparator, m_cQuote, m_nIndexCol );
	if ( nRow >= 0 )
	{
		m_nRow = nRow;
		m_bModified = TRUE;
	}
	return nRow;
}

/**********************************************************************/

bool CSSUnsortedDatabase::IsSorted ( int i )
{
	return TRUE;	// pretend to be sorted
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

void CSSUnsortedDatabase::Sort ( int (__cdecl *UserCompare)(const char* arg1, const char* arg2, int nType, int nDirection) )
{
	m_arrayRecord.Sort ( UserCompare );
	m_bModified = TRUE;
}

/**********************************************************************/

static int Compare ( const char* sz1, const char* sz2, int nSortType, int nSortDirection )
{
	int nComp = 0;

	return nComp;
}

void CSSUnsortedDatabase::Sort()
{
	m_arrayRecord.Sort ( Compare );
	m_bModified = TRUE;
}

/**********************************************************************/
