/**********************************************************************/
#include "ColumnHandler.h"
/**********************************************************************/

CColumnHandler::CColumnHandler()
{
	m_strFilename = Filenames.GetColumnHandlerFilename();

	m_nHeight = 0;
	for ( int i = 0 ; i < nMAXCOLUMNS ; i++ )
		m_nWidths[i] = 0;
}

//**********************************************************************

bool CColumnHandler::Read ( const char *szLabel, int nColumnCount )
{
	if ( nColumnCount > nMAXCOLUMNS )
		 nColumnCount = nMAXCOLUMNS;

	CSharedStringArray strArray;
	if ( strArray.Open ( m_strFilename, DB_READONLY ) == DB_ERR_NONE )
	{
		CString strLabel = szLabel;
		strLabel.MakeUpper();

		for ( int nIndex = 0; nIndex < strArray.GetSize(); nIndex++ )
		{
			CCSV csv ( strArray.GetAt( nIndex ) );
			CString strCheck = csv.GetString(0);
			strCheck.MakeUpper();
		
			if ( strCheck == strLabel )
			{
				m_nHeight = csv.GetInt(1);
				int nCount = csv.GetInt(2);

				if ( nCount < nColumnCount )				// new columns had been added so use new defaults
					return FALSE;

				if ( nCount > nColumnCount )
					nCount = nColumnCount;

				for ( int i = 0 ; i < nCount ; i++ )
					m_nWidths[i] = csv.GetInt(3+i);

				return TRUE;
			}
		}
	}
	return FALSE;
}

/**********************************************************************/

int CColumnHandler::GetTotalWidth()
{
	int nWindowWidth = 0;
	for ( int i = 0 ; i < nMAXCOLUMNS ; i++ )
		nWindowWidth += m_nWidths[i];

	return nWindowWidth;
}

int CColumnHandler::GetWidth ( int nIndex )
{
	if ( nIndex < nMAXCOLUMNS )
		return m_nWidths[nIndex];

	return 0;
}

void CColumnHandler::SetWidth ( int nIndex, int nWidth )
{
	if ( nIndex < nMAXCOLUMNS )
		m_nWidths[nIndex] = nWidth;
}

/**********************************************************************/

void CColumnHandler::Save ( const char* szLabel, int nHeight, int nColumns, CListCtrl& list )
{
	if ( nColumns > nMAXCOLUMNS )
		nColumns = nMAXCOLUMNS;

	CSharedStringArray strArray;
	if ( strArray.Open ( m_strFilename, DB_READWRITE ) == DB_ERR_NONE )
	{
		CString strLabel = szLabel;
		strLabel.MakeUpper();

		int nIndex;
		for ( nIndex = 0; nIndex < strArray.GetSize(); nIndex++ )
		{
			CCSV csv ( strArray.GetAt( nIndex ) );
			CString strCheck = csv.GetString(0);
			strCheck.MakeUpper();
		
			if ( strCheck == strLabel )
				break;
		}

		CCSV csv;
		csv.Add ( strLabel );					// label
		csv.Add ( nHeight );					// height
		csv.Add ( nColumns );					// column count

		for ( int i = 0; i < nColumns; i++ )
			csv.Add ( list.GetColumnWidth( i ) );

		if ( nIndex < strArray.GetSize() )	strArray.SetAt ( nIndex, csv.GetLine() );
		else								strArray.InsertAt ( nIndex, csv.GetLine() );

		strArray.Close();
	}
}

/**********************************************************************/
