/**********************************************************************/
#include "ColumnManager.h"
/**********************************************************************/

CColumnInfo::CColumnInfo()
{
	m_nType = 0;
	m_strName = "";
	m_strHeaderText = "";
	m_nColumnFormat = 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CColumnNameMapInfo::CColumnNameMapInfo()
{
	Reset();
}

/**********************************************************************/

void CColumnNameMapInfo::Reset()
{
	m_strName = "";
	m_nIndex = 0;
}

/**********************************************************************/

void CColumnNameMapInfo::Add ( CColumnNameMapInfo& source )
{
}

/**********************************************************************/

int CColumnNameMapInfo::Compare( CColumnNameMapInfo& source, int nHint )
{
	if ( m_strName != source.m_strName )
		return ( ( m_strName > source.m_strName ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CColumnStatusInfo::CColumnStatusInfo()
{
	Reset();
}

/**********************************************************************/

void CColumnStatusInfo::Reset()
{
	m_nStatus = COLUMN_STATUS_HIDDEN;
	m_bNotInFile = FALSE;
	m_nSortOrder = 0;
	m_nArrayIndex = 0;
	m_nWidth = 0;
	m_nDefaultWidth = 0;

	m_nType = 0;
	m_strName = "";
	m_strHeaderText = "";
	m_nColumnFormat = 0;
}

/**********************************************************************/

void CColumnStatusInfo::Add ( CColumnStatusInfo& source )
{
}

/**********************************************************************/

int CColumnStatusInfo::Compare( CColumnStatusInfo& source, int nHint )
{
	if ( m_nStatus != source.m_nStatus )
		return ( ( m_nStatus > source.m_nStatus ) ? 1 : -1 );

	if ( m_bNotInFile != source.m_bNotInFile )
		return ( ( TRUE == m_bNotInFile ) ? 1 : -1 );

	if ( m_nSortOrder != source.m_nSortOrder )
		return ( ( m_nSortOrder > source.m_nSortOrder ) ? 1 : -1 );

	if ( m_nArrayIndex != source.m_nArrayIndex )
		return ( ( m_nArrayIndex > source.m_nArrayIndex ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CColumnManager::CColumnManager( int nExtraWidth )
{	
	m_TableName.FileIdx = 0;
	m_nHiresWidth = 780;
	m_nMinimumLineWidth = 0;
	m_nExtraWidth = nExtraWidth;
}

/**********************************************************************/

bool CColumnManager::AllowWideColumns()
{
	return ( GetSystemMetrics ( SM_CXSCREEN ) > m_nHiresWidth ); 
}

/**********************************************************************/

void CColumnManager::Reset( TableName name, int nMinimumLineWidth, int nHiresWidth )
{
	m_arrayColumns.RemoveAll();
	m_arrayStatusDefault.RemoveAll();
	m_arrayStatusRestore.RemoveAll();
	m_arrayStatusCurrent.RemoveAll();

	AddColumn( ColumnNames::Height, "", 0, 400, 500, COLUMN_STATUS_HEIGHT );

	m_TableName = name;
	m_nMinimumLineWidth = nMinimumLineWidth;
	m_nHiresWidth = nHiresWidth;
}

/**********************************************************************/

void CColumnManager::AddColumn( const ColumnName& ColumnName, const char* szHeaderText, int nFormat, int nLores, int nHires, int nStatus )
{
	CColumnInfo infoColumn;
	infoColumn.m_nType = ColumnName.Type;
	infoColumn.m_strName = ColumnName.szLabel;
	infoColumn.m_strHeaderText = szHeaderText;
	infoColumn.m_nColumnFormat = nFormat;

	int nIndex = m_arrayColumns.GetSize();
	m_arrayColumns.Add( infoColumn );

	CColumnStatusInfo infoStatus;
	infoStatus.m_nStatus = nStatus;
	infoStatus.m_bNotInFile = FALSE;
	infoStatus.m_nSortOrder = nIndex;
	infoStatus.m_nArrayIndex = nIndex;
	
	if ( TRUE == AllowWideColumns() )
	{
		infoStatus.m_nWidth = nHires;
		infoStatus.m_nDefaultWidth = nHires;
	}
	else
	{
		infoStatus.m_nWidth = nLores;
		infoStatus.m_nDefaultWidth = nLores;
	}

	m_arrayStatusDefault.Consolidate( infoStatus );
	m_arrayStatusRestore.Consolidate( infoStatus );
	m_arrayStatusCurrent.Consolidate( infoStatus );	
}

/**********************************************************************/

void CColumnManager::LoadSettings()
{
	m_arrayStatusRestore.RemoveAll();
	for ( int n = 0; n < m_arrayStatusDefault.GetSize(); n++ )
	{
		CColumnStatusInfo infoStatus;
		m_arrayStatusDefault.GetAt( n, infoStatus );
		m_arrayStatusRestore.DirectAdd( infoStatus );
	}

	LoadSettingsInternal( Filenames.GetColumnsFilename( m_TableName.FileIdx ), m_TableName.Label );
	
	m_arrayStatusCurrent.RemoveAll();
	for ( int n = 0; n < m_arrayStatusRestore.GetSize(); n++ )
	{
		CColumnStatusInfo infoStatus;
		m_arrayStatusRestore.GetAt( n, infoStatus );
		m_arrayStatusCurrent.Consolidate( infoStatus );
	}

	m_arrayStatusRestore.RemoveAll();
	for ( int n = 0; n < m_arrayStatusCurrent.GetSize(); n++ )
	{
		CColumnStatusInfo infoStatus;
		m_arrayStatusCurrent.GetAt( n, infoStatus );
		m_arrayStatusRestore.DirectAdd( infoStatus );
	}	

	UpdateCurrentSettings();
}

/**********************************************************************/

bool CColumnManager::LoadSettingsInternal( const char* szFilename, const char* szType )
{
	CReportConsolidationArray<CColumnNameMapInfo> ColumnNameMap;

	//RESTORE ARRAY IS NOT IN SORTED ORDER AFTER THIS LOOP
	for ( int n = 0; n < m_arrayStatusRestore.GetSize(); n++ )
	{
		CColumnStatusInfo infoStatus;
		m_arrayStatusRestore.GetAt( n, infoStatus );
	
		CColumnNameMapInfo infoName;
		infoName.m_strName = m_arrayColumns[infoStatus.m_nArrayIndex].m_strName;
		infoName.m_nIndex = n;
		ColumnNameMap.Consolidate( infoName );

		infoStatus.m_bNotInFile = TRUE;
		infoStatus.m_nSortOrder = n;
		m_arrayStatusRestore.SetAt( n, infoStatus );
	}

	CString strType = szType;
	strType.MakeUpper();

	CFileStringArray arrayTemp;
	arrayTemp.Read( szFilename );

	bool bFound = FALSE;
	
	int nIndex = 0;
	for ( ; nIndex < arrayTemp.GetSize(); nIndex++ )
	{
		CCSV csv ( arrayTemp.GetAt( nIndex ) );
		CString strCheck = csv.GetString(0);
		strCheck.MakeUpper();
		
		if ( strCheck == strType )
		{
			bFound = TRUE;
			break;
		}
	}

	if ( bFound == FALSE )
		return FALSE;

	CCSV csvLine ( arrayTemp.GetAt ( nIndex ) );
	
	int nOffset = 1;
	int nVersion = 0;
	
	nVersion = csvLine.GetInt( nOffset++ );

	switch( nVersion )
	{
	case 0:
	case 1:
		{
			int nCount = csvLine.GetInt( nOffset++ );
			if ( nCount > 25 )
				nCount = 0;

			for ( int n = 0; n < nCount; n++ )
			{
				CColumnNameMapInfo infoName;
				infoName.m_strName = csvLine.GetString( nOffset );

				int nPos;
				if ( ColumnNameMap.Find( infoName, nPos ) == TRUE )
				{
					ColumnNameMap.GetAt( nPos, infoName );

					int nWidth = csvLine.GetInt( nOffset + 1 );
					if ( nWidth < 30 )
						nWidth = 30;

					CColumnStatusInfo infoStatus;
					m_arrayStatusRestore.GetAt( infoName.m_nIndex, infoStatus );

					if ( TRUE == infoStatus.m_bNotInFile )
					{
						if ( 1 == nVersion )
							infoStatus.m_nSortOrder = csvLine.GetInt( nOffset + 2 );
						else
							infoStatus.m_nSortOrder = n;

						switch( infoStatus.m_nStatus )
						{
						case COLUMN_STATUS_HIDDEN:
						case COLUMN_STATUS_SHOWN:
							if ( -1 == infoStatus.m_nSortOrder )
								infoStatus.m_nStatus = COLUMN_STATUS_HIDDEN;
							else
								infoStatus.m_nStatus = COLUMN_STATUS_SHOWN;
							break;
						}
						
						infoStatus.m_nWidth = nWidth;
						infoStatus.m_bNotInFile = FALSE;
	
						m_arrayStatusRestore.SetAt( infoName.m_nIndex, infoStatus );
					}
				}

				nOffset += ( 1 == nVersion ) ? 3 : 2;
			}
		}
		break;
	}
	
	return TRUE;
}

/**********************************************************************/

void CColumnManager::UpdateCurrentSettings()
{
	for ( int n = 1; n < m_arrayStatusCurrent.GetSize(); n++ )
	{
		CColumnStatusInfo infoStatus;
		m_arrayStatusCurrent.GetAt( n, infoStatus );
		infoStatus.m_nType = m_arrayColumns[ infoStatus.m_nArrayIndex ].m_nType;
		infoStatus.m_strName = m_arrayColumns[ infoStatus.m_nArrayIndex ].m_strName;
		infoStatus.m_strHeaderText = m_arrayColumns[ infoStatus.m_nArrayIndex ].m_strHeaderText;
		infoStatus.m_nColumnFormat = m_arrayColumns[ infoStatus.m_nArrayIndex ].m_nColumnFormat;
		m_arrayStatusCurrent.SetAt( n, infoStatus );
	}
}

/**********************************************************************/

void CColumnManager::RememberCurrentLine( CListCtrl* pList, CWnd* pWnd )
{
	for ( int n = 1; n < m_arrayStatusCurrent.GetSize(); n++ )
	{
		CColumnStatusInfo infoStatus;
		m_arrayStatusCurrent.GetAt( n, infoStatus );
			
		switch( infoStatus.m_nStatus )
		{
		case COLUMN_STATUS_LOCKED:
		case COLUMN_STATUS_SHOWN:
			infoStatus.m_nWidth = pList -> GetColumnWidth( n - 1 );
			break;

		case COLUMN_STATUS_HIDDEN:
			infoStatus.m_nWidth = infoStatus.m_nDefaultWidth;
			break;
		}

		m_arrayStatusCurrent.SetAt( n, infoStatus );
	}
}

/**********************************************************************/

void CColumnManager::SaveCurrentLine( CListCtrl* pList, CWnd* pWnd )
{
	if ( m_TableName.FileIdx != 0 )
	{
		int nCount = m_arrayStatusCurrent.GetSize();

		CRect rect;
		pWnd -> GetWindowRect( rect );
		
		CCSV csvOut;
		csvOut.Add( m_TableName.Label );
		csvOut.Add( 1 );
		csvOut.Add( nCount );

		csvOut.Add( ColumnNames::Height.szLabel );
		csvOut.Add( rect.Height() );
		csvOut.Add( -1 );

		for ( int n = 1; n < m_arrayStatusCurrent.GetSize(); n++ )
		{
			CColumnStatusInfo infoStatus;
			m_arrayStatusCurrent.GetAt( n, infoStatus );
			
			csvOut.Add( infoStatus.m_strName );

			switch( infoStatus.m_nStatus )
			{
			case COLUMN_STATUS_LOCKED:
			case COLUMN_STATUS_SHOWN:
				csvOut.Add( pList -> GetColumnWidth( n - 1 ) );
				csvOut.Add( n );
				break;

			case COLUMN_STATUS_HIDDEN:
			default:
				csvOut.Add( infoStatus.m_nDefaultWidth );
				csvOut.Add( -1 );
				break;
			}
		}

		CFileStringArray arrayTemp;
		arrayTemp.Add( csvOut.GetLine() );
		arrayTemp.Write( Filenames.GetColumnsFilename( m_TableName.FileIdx ) );
	}
}

/**********************************************************************/

void CColumnManager::RestoreStartOfSessionLine()
{
	m_arrayStatusCurrent.RemoveAll();
	for( int n = 0; n < m_arrayStatusRestore.GetSize(); n++ )
	{
		CColumnStatusInfo infoStatus;
		m_arrayStatusRestore.GetAt( n, infoStatus );
		m_arrayStatusCurrent.DirectAdd( infoStatus );
	}
	UpdateCurrentSettings();
}

/**********************************************************************/

void CColumnManager::RestoreDefaultLine()
{
	m_arrayStatusCurrent.RemoveAll();
	for( int n = 0; n < m_arrayStatusDefault.GetSize(); n++ )
	{
		CColumnStatusInfo infoStatus;
		m_arrayStatusDefault.GetAt( n, infoStatus );
		m_arrayStatusCurrent.DirectAdd( infoStatus );
	}
	UpdateCurrentSettings();
}

/**********************************************************************/

void CColumnManager::RedrawList( CListCtrl* pList, CWnd* pWnd )
{
	int nColumnCount = pList -> GetHeaderCtrl() -> GetItemCount();

	for ( int i = 0; i < nColumnCount; i++ )
	   pList -> DeleteColumn(0);

	for ( int n = 1; n < m_arrayStatusCurrent.GetSize(); n++ )
	{
		CColumnStatusInfo infoStatus;
		m_arrayStatusCurrent.GetAt( n, infoStatus );

		if ( COLUMN_STATUS_HIDDEN != infoStatus.m_nStatus )
			pList -> InsertColumn( n - 1, infoStatus.m_strHeaderText, infoStatus.m_nColumnFormat, infoStatus.m_nWidth );
		else
			break;
	}

	int nTotalWidth = 0;
	nColumnCount = pList -> GetHeaderCtrl() -> GetItemCount();
	for ( int n = 0; n < nColumnCount; n++ )
		nTotalWidth += pList -> GetColumnWidth ( n );

	CColumnStatusInfo infoHeight;
	m_arrayStatusCurrent.GetAt( 0, infoHeight );

	int nHeight = infoHeight.m_nWidth;
	if ( nHeight < 150 ) 
		nHeight = 150;

	//if ( nTotalWidth < m_nMinimumLineWidth )
	//	nTotalWidth = m_nMinimumLineWidth;
	
	pWnd -> SetWindowPos( NULL, 0, 0, nTotalWidth + 60 + m_nExtraWidth, nHeight, SWP_NOMOVE | SWP_NOZORDER );
}

/**********************************************************************/

int CColumnManager::GetListColumnStatus( int nPos )
{
	if ( ( nPos < 0 ) || ( nPos >= m_arrayStatusCurrent.GetSize() - 1 ) )
		return COLUMN_STATUS_HIDDEN;
	else
	{
		CColumnStatusInfo infoStatus;
		m_arrayStatusCurrent.GetAt( nPos + 1, infoStatus );
		return infoStatus.m_nStatus;
	}
}

/**********************************************************************/

int CColumnManager::GetListColumnType( int nPos )
{
	if ( ( nPos < 0 ) || ( nPos >= m_arrayStatusCurrent.GetSize() - 1 ) )
		return 0;
	else
	{
		CColumnStatusInfo infoStatus;
		m_arrayStatusCurrent.GetAt( nPos + 1, infoStatus );
		return infoStatus.m_nType;
	}
}

/**********************************************************************/

void CColumnManager::GetListColumnDetails( CArray<CColumnStatusInfo,CColumnStatusInfo>& arrayPos )
{
	arrayPos.RemoveAll();

	for ( int n = 1; n < m_arrayStatusCurrent.GetSize(); n++ )
	{
		CColumnStatusInfo infoStatus;
		m_arrayStatusCurrent.GetAt( n, infoStatus );
		arrayPos.Add( infoStatus );
	}
}

/**********************************************************************/

void CColumnManager::SetListColumnDetails( CArray<CColumnStatusInfo,CColumnStatusInfo>& arrayPos )
{
	for ( int n = m_arrayStatusCurrent.GetSize() - 1; n > 0; n-- )
		m_arrayStatusCurrent.RemoveAt(n);

	for ( int n = 0; n < arrayPos.GetSize(); n++ )
	{
		CColumnStatusInfo infoStatus = arrayPos.GetAt(n);
		infoStatus.m_nSortOrder = n;
		infoStatus.m_bNotInFile = FALSE;
		m_arrayStatusCurrent.Consolidate( infoStatus );
	}
}

/**********************************************************************/
