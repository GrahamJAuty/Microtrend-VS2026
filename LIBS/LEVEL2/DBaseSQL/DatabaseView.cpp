/**********************************************************************/

CSSDatabaseView::CSSDatabaseView()
{
	CreateView ( NULL );
}

/**********************************************************************/

CSSDatabaseView::CSSDatabaseView ( CSSDatabase* pDB )
{
	CreateView ( pDB );
}

/**********************************************************************/

void CSSDatabaseView::CreateView ( CSSDatabase* pDB )
{
	m_pDB = pDB;
	m_strSQL = "";
	m_strError = "";
	m_arHeader.RemoveAll();
	m_arView.RemoveAll();
	m_arSortFields.RemoveAll();

	if ( m_pDB != NULL )
		m_pDB->m_bUpdateView = TRUE;		// need to update display array
}

/**********************************************************************/

void CSSDatabaseView::InitView()
{
	m_pDB = NULL;
	m_strSQL = "";
	m_strError = "";
	m_arHeader.RemoveAll();
	m_arView.RemoveAll();
	m_arSortFields.RemoveAll();
}

/**********************************************************************/

int CSSDatabaseView::GetRecordCount()
{
	if ( m_pDB != NULL )
	{
		Update();		// update if m_db.m_bUpdateView true
		return m_arView.GetSize();
	}

	return 0;
}

/**********************************************************************/

int CSSDatabaseView::GetColumnCount()
{
	if ( m_pDB != NULL )
	{
		Update();		// update if m_db.m_bUpdateView true
		return m_arHeader.GetSize();
	}

	return 0;
}

/**********************************************************************/

CString CSSDatabaseView::GetHeaderLabel ( int nColView )
{
	CString strLabel;

	if ( m_pDB != NULL )
	{
		Update();		// update if m_db.m_bUpdateView true
		strLabel = m_pDB->GetHeaderLabel(m_arHeader.GetAt(nColView));
	}

	return strLabel;
}

/**********************************************************************/

CRecordObj* CSSDatabaseView::GetRecord ( int nRowView )
{
	return m_pDB->GetRecord(m_arView.GetAt(nRowView));
}

/**********************************************************************/

const char* CSSDatabaseView::GetField ( int nRowView, int nColView )
{
	if ( m_pDB != NULL )
	{
		Update();		// update if m_db.m_bUpdateView true

		if ( nRowView >= 0 && nRowView < m_arView.GetSize() )
		{
			if ( nColView >= 0 && nColView < m_arHeader.GetSize() )
				return GetRecord(nRowView)->GetString(m_arHeader.GetAt(nColView));
		}
	}

	return "";
 }

/**********************************************************************/

bool CSSDatabaseView::SetField ( int nRowView, int nColView, const char* szData )
{
	if ( m_pDB != NULL )
	{
		if ( nRowView >= 0 && nRowView < m_arView.GetSize() )
		{
			if ( nColView >= 0 && nColView < m_arHeader.GetSize() )
			{
				if ( GetRecord(nRowView)->SetAt ( m_arHeader.GetAt(nColView), szData ) == TRUE )
				{
					m_pDB->m_bDatabaseModified = TRUE;	// database has been modified
					m_pDB->m_bUpdateView = TRUE;		// need to update view
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

/**********************************************************************/

bool CSSDatabaseView::WriteFile ( const char* szFilename, bool bImpliedHeader )
{
	bool bReply = FALSE;

	if ( m_pDB != NULL )
	{
		Update();		// update if m_db.m_bUpdateView true

		CSSFile file;
		if ( file.Open ( szFilename, "wb" ) == TRUE )
		{
			if ( m_strSQL == "" || m_strSQL == "SELECT * FROM this;" )		// no filtering
			{
				if ( bImpliedHeader == FALSE )
					file.WriteLine ( m_pDB->GetHeader() );

				for ( int nRowView = 0 ; nRowView < m_arView.GetSize() ; nRowView++ )
				{
					bReply = file.WriteLine ( GetRecord(nRowView)->GetLine() );
					if ( bReply == FALSE )
						break;
				}
			}
			else		// filtered
			{
				if ( bImpliedHeader == FALSE )
				{
					CCSV csvHeader ( m_pDB->m_cSeparator, m_pDB->m_cQuote );

					for ( int nColView = 0 ; nColView < m_arHeader.GetSize() ; nColView++ )	// just get selected columns
					{
						int nColDB = m_arHeader.GetAt(nColView);
						csvHeader.Add ( m_pDB->GetHeaderLabel(nColDB) );
					}

					file.WriteLine ( csvHeader.GetLine() );
				}

				for ( int nRowView = 0 ; nRowView < m_arView.GetSize() ; nRowView++ )
				{
					CCSV csvRecord ( m_pDB->m_cSeparator, m_pDB->m_cQuote );

					for ( int nColView = 0 ; nColView < m_arHeader.GetSize() ; nColView++ )	// just get selected columns
					{
						int nColDB = m_arHeader.GetAt(nColView);
						csvRecord.Add ( GetRecord(nRowView)->GetString(nColDB) );
					}

					bReply = file.WriteLine ( csvRecord.GetLine() );
					if ( bReply == FALSE )
						break;
				}
			}

			file.Close();

			if ( bReply == FALSE )
				m_strError = "File write errror";
		}
		else
			m_strError = "Cannot create file";
	}
	else
		m_strError = "NULL database";

	return bReply;
}

/**********************************************************************/

void CSSDatabaseView::Update()
{
	if ( m_pDB->m_bUpdateView == TRUE )	// need to update display array
	{
		m_arHeader.RemoveAll();

		if ( m_strSQL == ""  || m_strSQL == "SELECT * FROM this;" )		// no filtering
		{
			for ( int i = 0 ; i < m_pDB->GetHeaderSize() ; i++ )		// get all columns
				m_arHeader.Add ( i );

			m_arView.SetSize ( m_pDB->GetRecordCount() );

			for ( int i = 0 ; i < m_pDB->GetRecordCount() ; i++ )		// get all rows
				m_arView.SetAt ( i, i );
		}
		else
		{
			Filter ( m_strSQL );
		}

		m_pDB->m_bUpdateView = FALSE;	// display array has been updated
	}
}

/**********************************************************************/

bool CSSDatabaseView::IsModified()
{
	if ( m_pDB != NULL )
	{
		if ( m_pDB->m_bDatabaseModified == TRUE )							// database has been modified
			return TRUE;

		if ( m_arHeader.GetSize() != m_pDB->GetHeaderSize() )					// different number of columns
			return TRUE;

		for ( int nColView = 0 ; nColView < m_arHeader.GetSize() ; nColView++ )	// same number of columns
		{
			if ( m_arHeader.GetAt(nColView) != nColView )					// column order changed
				return TRUE;
		}

		if ( GetRecordCount() != m_pDB->GetRecordCount() )					// different number of records
			return TRUE;

		for ( int nRowView = 0 ; nRowView < GetRecordCount() ; nRowView++ )		// same number of records
		{
			if ( GetDatabaseRow(nRowView) != nRowView )						// database has been sorted
				return TRUE;
		}
	}

	return FALSE;
}

/**********************************************************************/

void CSSDatabaseView::SaveView()
{
	if ( m_pDB != NULL )
	{
		Update();		// update if m_db.m_bUpdateView true

		m_pDB->SaveView ( m_arView );

		for ( int nRowView = 0 ; nRowView < GetRecordCount() ; nRowView++ )		// view same as database
			m_arView.SetAt ( nRowView, nRowView );
	}
}

/**********************************************************************/
