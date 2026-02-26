/**********************************************************************/

CRecordObj::CRecordObj ( const char* szRecord, char cSeparator, char cQuote, int nIndexCol )
{
	m_strRecord = szRecord;
	int nMaxFields = ( nIndexCol >= 0 ) ? nIndexCol+1 : 1;
	m_pCSVRecord = new CCSV ( szRecord, cSeparator, cQuote, nMaxFields );
	m_bRecordModified = FALSE;
	m_bParsed = FALSE;
}

CRecordObj::~CRecordObj()
{
	delete (CCSV*) m_pCSVRecord;
}

/**********************************************************************/

void CRecordObj::Parse()
{
	if ( m_bParsed == FALSE )
	{
		m_pCSVRecord->ParseLine ( m_strRecord );
		m_bParsed = TRUE;
	}
}

/**********************************************************************/

CString CRecordObj::GetLine()
{
	return ( m_bRecordModified == TRUE ) ? m_pCSVRecord->GetLine() : m_strRecord;
}

/**********************************************************************/

int CRecordObj::GetIntKey ( int nIndexCol )
{
	// no need to parse since key parsed when record created

	return m_pCSVRecord->GetInt ( nIndexCol );
}

/**********************************************************************/

__int64 CRecordObj::GetInt64Key ( int nIndexCol )
{
	// no need to parse since key parsed when record created

	return m_pCSVRecord->GetInt64 ( nIndexCol );
}

/**********************************************************************/

const char* CRecordObj::GetStringKey ( int nIndexCol )
{
	// no need to parse since key parsed when record created

	return m_pCSVRecord->GetString ( nIndexCol );
}

/**********************************************************************/

const char* CRecordObj::GetString ( int nCol )
{
	Parse();		// only parse if not already parsed
	return m_pCSVRecord->GetString(nCol);
}

int CRecordObj::GetInt ( int nCol )
{
	Parse();		// only parse if not already parsed
	return m_pCSVRecord->GetInt(nCol);
}

__int64 CRecordObj::GetInt64 ( int nCol )
{
	Parse();		// only parse if not already parsed
	return m_pCSVRecord->GetInt64(nCol);
}

long CRecordObj::GetLong ( int nCol )
{
	Parse();		// only parse if not already parsed
	return m_pCSVRecord->GetLong(nCol);
}

double CRecordObj::GetDouble ( int nCol )
{
	Parse();		// only parse if not already parsed
	return m_pCSVRecord->GetDouble(nCol);
}

bool CRecordObj::GetBool ( int nCol )
{
	Parse();		// only parse if not already parsed
	return m_pCSVRecord->GetBool(nCol);
}

/**********************************************************************/

CCSV* CRecordObj::GetCSV()
{
	Parse();		// only parse if not already parsed
	return m_pCSVRecord;
}

/**********************************************************************/

void CRecordObj::Add ( const char* szValue )
{
	Parse();		// only parse if not already parsed

	m_pCSVRecord->Add ( szValue );
	m_bRecordModified = TRUE;
}

/**********************************************************************/

bool CRecordObj::SetAt ( int nCol, const char* szValue )
{
	Parse();		// only parse if not already parsed

	if ( strcmp ( m_pCSVRecord->GetString(nCol), szValue ) != 0 )
	{
		m_pCSVRecord->SetAt ( nCol, szValue );
		m_bRecordModified = TRUE;
		return TRUE;	// modified
	}
	return FALSE;
}

/**********************************************************************/

void CRecordObj::InsertAt ( int nCol, const char* szValue )
{
	Parse();		// only parse if not already parsed

	m_pCSVRecord->InsertAt ( nCol, szValue );
	m_bRecordModified = TRUE;
}

/**********************************************************************/

void CRecordObj::RemoveAt ( int nCol )
{
	Parse();		// only parse if not already parsed

	if ( nCol >= 0 && nCol < m_pCSVRecord->GetSize() )
	{
		m_pCSVRecord->RemoveAt ( nCol );
		m_bRecordModified = TRUE;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CRecordArray::CRecordArray()
{
}

/**********************************************************************/

CRecordArray::~CRecordArray()
{
	RemoveAll();
}

/**********************************************************************/

int CRecordArray::AddRecord ( const char* szRecord, char cSeparator, char cQuote, int nIndexCol )
{
	CRecordObj* pRecord = new CRecordObj ( szRecord, cSeparator, cQuote, nIndexCol );
	if ( pRecord != NULL )
	{
		return CObArray::Add ( pRecord );
	}
	return -1;
}

/**********************************************************************/

bool CRecordArray::InsertRecord ( int nRow, const char* szRecord, char cSeparator, char cQuote, int nIndexCol )
{
	CRecordObj* pRecord = new CRecordObj ( szRecord, cSeparator, cQuote, nIndexCol );
	if ( pRecord != NULL )
	{
		CObArray::InsertAt ( nRow, pRecord );
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CRecordArray::RemoveAll()
{
	for ( int i = 0 ; i < CObArray::GetSize() ; i++ )
		delete GetAt(i);

	CObArray::RemoveAll();
}

/**********************************************************************/

void CRecordArray::RemoveAt ( int nRow, int nCount )
{
	for ( int i = 0 ; i < nCount ; i++ )
		delete GetAt(nRow+i);

	CObArray::RemoveAt ( nRow, nCount );
}

/**********************************************************************/

void CRecordArray::InsertColumn ( int nCol )
{
	for ( int i = 0 ; i < CObArray::GetSize() ; i++ )
		GetAt(i)->InsertAt ( nCol, "" );
}

/**********************************************************************/

void CRecordArray::DeleteColumn ( int nCol )
{
	for ( int i = 0 ; i < CObArray::GetSize() ; i++ )
		GetAt(i)->RemoveAt ( nCol );
}

/**********************************************************************/

void CRecordArray::AddColumn()
{
	for ( int i = 0 ; i < CObArray::GetSize() ; i++ )
		GetAt(i)->Add ( "" );
}

/**********************************************************************/

int CRecordArray::GetMaxColumns()
{
	int nMax = 0;

	for ( int i = 0 ; i < GetSize() ; i++ )
	{
		int n = GetAt(i)->GetCSV()->GetSize();
		if ( n > nMax )
			nMax = n;
	}

	return nMax;
}

/**********************************************************************/

void CRecordArray::SaveView ( CUIntArray& arView )
{
	int nCount = GetSize();

	if ( nCount > 0 )
	{
		for ( int i = 0 ; i < arView.GetSize() ; i++ )
		{
			int n = arView.GetAt(i);
			CRecordObj* pObj = GetAt(n);
			Add ( pObj );				// add to end of array;
			SetAt ( n, NULL );			// to indicate record copied;
		}

		for ( int i = 0 ; i < nCount ; i++ )	// delete any records that have not been copied
		{
			CRecordObj* pObj = GetAt(i);
			if ( pObj != NULL )
				delete pObj;
		}

		CObArray::RemoveAt ( 0, nCount );		// delete copied and deleted records from array
	}
}

/**********************************************************************/
