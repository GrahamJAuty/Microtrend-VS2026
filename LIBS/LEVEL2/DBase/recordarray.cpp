/**********************************************************************/

CRecordObj::CRecordObj ( const char* szRecord, char cSeparator, char cQuote, int nIndexCol )
{
	m_strRecord = szRecord;
	int nMaxFields = ( nIndexCol >= 0 ) ? nIndexCol+1 : 1;
	m_pCSVRecord = new CCSV ( szRecord, cSeparator, cQuote, nMaxFields );
	m_bModified = FALSE;
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
	return ( m_bModified == TRUE ) ? m_pCSVRecord->GetLine() : m_strRecord;
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

bool CRecordObj::SetAt ( int nCol, const char* szValue )
{
	Parse();		// only parse if not already parsed

	if ( strcmp ( m_pCSVRecord->GetString(nCol), szValue ) != 0 )
	{
		m_pCSVRecord->SetAt ( nCol, szValue );
		m_bModified = TRUE;
		return TRUE;	// modified
	}
	return FALSE;
}

/**********************************************************************/

void CRecordObj::InsertAt ( int nCol, const char* szValue )
{
	Parse();		// only parse if not already parsed

	m_pCSVRecord->InsertAt ( nCol, szValue );
	m_bModified = TRUE;
}

/**********************************************************************/

void CRecordObj::RemoveAt ( int nCol )
{
	Parse();		// only parse if not already parsed

	if ( nCol >= 0 && nCol < m_pCSVRecord->GetSize() )
	{
		m_pCSVRecord->RemoveAt ( nCol );
		m_bModified = TRUE;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CRecordArray::CRecordArray()
{
	m_bUpdateDisplay = TRUE;		// need to update display array
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
		m_bUpdateDisplay = TRUE;		// need to update display array
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
		m_bUpdateDisplay = TRUE;		// need to update display array
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

	m_SortArray.RemoveAll();
	m_arrayDisplay.RemoveAll();
	m_bUpdateDisplay = TRUE;		// need to update display array
}

/**********************************************************************/

void CRecordArray::RemoveAt ( int nRow, int nCount )
{
	for ( int i = 0 ; i < nCount ; i++ )
		delete GetAt(nRow+i);

	CObArray::RemoveAt ( nRow, nCount );

	m_bUpdateDisplay = TRUE;		// need to update display array
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
/*	Display Array routines									*/
/**********************************************************************/

int CRecordArray::DisplayGetCount()
{
	UpdateDisplay();
	
	return m_arrayDisplay.GetSize();
}

/**********************************************************************/

const char* CRecordArray::DisplayGetField ( int nRow, int nCol )
{
	UpdateDisplay();

	if ( nRow >= 0 && nRow < m_arrayDisplay.GetSize() )
		return ((CRecordObj*) m_arrayDisplay.GetAt(nRow))->GetString(nCol);

	return "";
 }

/**********************************************************************/

bool CRecordArray::DisplaySetField ( int nRow, int nCol, const char* szData )
{
	bool bModified = FALSE;

	if ( nRow >= 0 && nRow < m_arrayDisplay.GetSize() )
		bModified = ((CRecordObj*) m_arrayDisplay.GetAt(nRow))->SetAt ( nCol, szData );

	return bModified;
}

/**********************************************************************/

void CRecordArray::DisplayDeleteRecord ( int nRow )
{
	CRecordObj* pObj = (CRecordObj*) m_arrayDisplay.GetAt(nRow);
	m_arrayDisplay.RemoveAt(nRow);

	for ( int i = 0 ; i < CObArray::GetSize() ; i++ )
	{
		if ( GetAt(i) == pObj )
		{
			delete GetAt(i);		// delete CRecordObj
			CObArray::RemoveAt(i);
			break;
		}
	}
}

/**********************************************************************/

void CRecordArray::UpdateDisplay()
{
	if ( m_bUpdateDisplay == TRUE )	// need to update display array
	{
		m_arrayDisplay.SetSize ( CObArray::GetSize() );

		for ( int i = 0 ; i < CObArray::GetSize() ; i++ )
			m_arrayDisplay.SetAt ( i, GetAt(i) );

		m_bUpdateDisplay = FALSE;	// display array has been updated
	}
}

/**********************************************************************/

bool CRecordArray::InsertColumn ( int nCol )
{
	for ( int i = 0 ; i < CObArray::GetSize() ; i++ )
		GetAt(i)->InsertAt ( nCol, "" );

	m_bUpdateDisplay = TRUE;		// need to update display array

	return TRUE;
}

/**********************************************************************/

bool CRecordArray::DeleteColumn ( int nCol )
{
	for ( int i = 0 ; i < CObArray::GetSize() ; i++ )
		GetAt(i)->RemoveAt ( nCol );

	m_bUpdateDisplay = TRUE;		// need to update display array

	return TRUE;
}

/**********************************************************************/
