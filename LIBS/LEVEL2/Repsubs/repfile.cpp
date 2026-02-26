/**********************************************************************/

CReportFile::CReportFile()
{
	m_bHeaderModified = FALSE;
	m_bFileOpened = FALSE;
	m_bAppend = FALSE;
}

/**********************************************************************/

CReportFile::~CReportFile()
{
	Close();
}

/**********************************************************************/

bool CReportFile::Open ( const char* szFilename, bool bAppend )
{
	m_bAppend = bAppend;

	CString strMode = ( bAppend == TRUE ) ? "ab" : "wb";

	m_bFileOpened = m_file.Open ( szFilename, strMode );

	return m_bFileOpened;
}

/**********************************************************************/

void CReportFile::Close()
{
	m_file.Close();
	m_bHeaderModified = FALSE;
	m_bFileOpened = FALSE;
	m_bAppend = FALSE;
	m_strTitle = "";
	m_arrayHeader.RemoveAll();
	m_strFooter = "";
	m_Ruler.RemoveAll();
}

/**********************************************************************/

CString CReportFile::GetColourText ( const char* szText, DWORD dwFgColour, DWORD dwBgColour )
{
	CString strText;

	CString str;
	if ( dwFgColour != 0xffffffff )
	{
		str.Format ( "<CF#%6.6X>", (dwFgColour&0xffffff) );
		strText += str;
	}
	if ( dwBgColour != 0xffffffff )
	{
		str.Format ( "<CB#%6.6X>", (dwBgColour&0xffffff) );
		strText += str;
	}

	strText += szText;

	return strText;
}

/**********************************************************************/

void CReportFile::SetStyle1 ( const char* szText )
{
	SetTitle ( szText );

	CString strFooter = szText;
	strFooter += "  <DATE><RIGHT>Page <PAGENO>";
	SetFooter ( strFooter );
}

/**********************************************************************/

void CReportFile::SetTitle ( const char* szTitle )
{
	m_strTitle.Format ( "<TITLE>%s", szTitle );
	m_bHeaderModified = TRUE;
}

void CReportFile::SetHeader ( const char* szHeader )
{
	CString strHeader;
	strHeader.Format ( "<HEADER>%s", szHeader );
	m_arrayHeader.Add ( strHeader );
	m_bHeaderModified = TRUE;
}

void CReportFile::SetFooter ( const char* szFooter )
{
	m_strFooter.Format ( "<FOOTER>%s", szFooter );
	m_bHeaderModified = TRUE;
}

/**********************************************************************/

void CReportFile::AddColumn ( const char* szDescription, UINT wTabAlign, int nFieldWidth )
{
	m_Ruler.AddColumn ( szDescription, wTabAlign, nFieldWidth );
	m_bHeaderModified = TRUE;
}

/**********************************************************************/

void CReportFile::AddSubColumn ( int nIndex, const char* szDescription, UINT wTabAlign, double dPercentWidth )
{
	m_Ruler.AddSubColumn ( nIndex, szDescription, wTabAlign, dPercentWidth );
	m_bHeaderModified = TRUE;
}

/**********************************************************************/

bool CReportFile::WriteLine ( const char* szText )
{
	if ( m_bFileOpened == TRUE )
	{
		if ( m_bHeaderModified == TRUE && m_bAppend == FALSE )		// dont write header if appending
			WriteHeader();

		return m_file.WriteLine ( szText );
	}
	return FALSE;
}

/**********************************************************************/

bool CReportFile::WriteLine ( CCSV& csv )
{
	return WriteLine ( csv.GetLine() );
}

/**********************************************************************/

void CReportFile::WriteHeader()
{
	m_file.WriteLine ( m_strTitle );

	for ( int i = 0 ; i < m_arrayHeader.GetSize() ; i++ )
		m_file.WriteLine ( m_arrayHeader.GetAt(i) );

	if ( m_strFooter.GetLength() > 0 )
		m_file.WriteLine ( m_strFooter );

	if ( m_Ruler.GetSize() > 0 )
	{
		CString strAlign;

		for ( int nIndex = 0 ; nIndex < m_Ruler.GetSize() ; nIndex++ )
		{
			CCSV csv;

			switch ( m_Ruler.GetAlign(nIndex) )
			{
				case TA_CENTER:	strAlign = "<COLUMN>C";	break;
				case TA_RIGHT:		strAlign = "<COLUMN>R";	break;
				default:			strAlign = "<COLUMN>L";	break;
			}
			csv.Add ( strAlign );
			csv.Add ( m_Ruler.GetWidth(nIndex) );
			csv.Add ( m_Ruler.GetText(nIndex) );

			if ( m_Ruler.GetSubTabSize(nIndex) > 0 )
			{
				csv.Add ( m_Ruler.GetSubTabSize(nIndex) );

				for ( int n = 0 ; n < m_Ruler.GetSubTabSize(nIndex) ; n++ )
				{
					switch ( m_Ruler.GetSubTabAlign(nIndex,n) )
					{
					case TA_CENTER:	strAlign = "C";	break;
					case TA_RIGHT:		strAlign = "R";	break;
					default:			strAlign = "L";	break;
					}

					csv.Add ( strAlign );
					csv.Add ( m_Ruler.GetSubTabPercentWidth(nIndex,n), 3 );
					csv.Add ( m_Ruler.GetSubTabText(nIndex,n) );
				}
			}

			m_file.WriteLine ( csv.GetLine() );
		}
	}

	m_bHeaderModified = FALSE;
}

/**********************************************************************/

