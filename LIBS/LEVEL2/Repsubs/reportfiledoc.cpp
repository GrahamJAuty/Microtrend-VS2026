/**********************************************************************/

CReportFileDoc::CReportFileDoc()
	: m_nRevealLevel(0)
{
}

/**********************************************************************/

CReportFileDoc::CReportFileDoc ( const char* szFilename )
	: m_nRevealLevel(0)
{
	CSSFile fileSource;

	if ( fileSource.Open ( szFilename, "rb" ) == TRUE )
	{
		CString strBuf;

		while ( fileSource.ReadString ( strBuf ) == TRUE )
		{
			if ( ProcessHeader ( strBuf ) == FALSE )
				m_TextArray.Add ( strBuf );
		}

		fileSource.Close();
	}
}

/**********************************************************************/

void CReportFileDoc::RemoveAll()
{
	m_Ruler.RemoveAll();
	m_HeaderArray.RemoveAll();
	m_FooterArray.RemoveAll();
	m_TextArray.RemoveAll();
	m_PageArray.RemoveAll();
	m_strTitle = "";
	m_strSubHeader = "";
}

/**********************************************************************/

int CReportFileDoc::GetDisplayTextSize()
{
	int nCount = m_TextArray.GetSize();

	if ( m_strSubHeader != "" )
		nCount++;

	return nCount;
}

const char* CReportFileDoc::GetDisplayTextAt ( int n )
{
	if ( m_strSubHeader != "" )
	{
		if ( n == 0 )
			return m_strSubHeader;
		else
			n--;
	}

	if ( n < m_TextArray.GetSize() )
		return m_TextArray.GetAt(n);
	else
		return "";
}

/**********************************************************************/

int CReportFileDoc::GetPrintTextSize()
{
	return m_TextArray.GetSize();
}

const char* CReportFileDoc::GetPrintTextAt ( int n )
{
	if ( n < m_TextArray.GetSize() )
		return m_TextArray.GetAt(n);
	else
		return "";
}

/**********************************************************************/

void CReportFileDoc::Read ( CArchive& ar )
{
	RemoveAll();

	CString strBuf;

	while ( ar.ReadString ( strBuf ) == TRUE )
	{
		if ( ProcessHeader ( strBuf ) == FALSE )
			m_TextArray.Add ( strBuf );
	}
}

/**********************************************************************/

void CReportFileDoc::ToggleReveal()
{
	switch ( m_nRevealLevel )
	{
	case 0:	m_nRevealLevel = 0x0001;		break;	// reveal 1st level
	case 1:	m_nRevealLevel = 0x03ff;		break;	// reveal all
	default:	m_nRevealLevel = 0;			break;	// hide all
	}
}

/**********************************************************************/

bool CReportFileDoc::ProcessHeader ( const char* szBuf )
{
	if ( szBuf[0] != '<' || strlen ( szBuf ) <= 2 )		// check if header line
		return FALSE;

	if ( strncmp ( szBuf, "<COLUMN>", 8 ) == 0 )			// align,width,text
	{
		int nAlign;
		CCSV csv ( &szBuf[8] );

		if ( strcmp ( csv.GetString(0), "C" ) == 0 )
			nAlign = TA_CENTER;
		else if ( strcmp ( csv.GetString(0), "R" ) == 0 )
			nAlign = TA_RIGHT;
		else
			nAlign = TA_LEFT;

		m_Ruler.AddColumn ( csv.GetString(2), nAlign, csv.GetInt(1)/3 );

		for ( int i = 0 ; i < csv.GetInt(3) ; i++ )		// number of sub columns
		{
			if ( strcmp ( csv.GetString(i*3+4), "C" ) == 0 )
				nAlign = TA_CENTER;
			else if ( strcmp ( csv.GetString(i*3+4), "R" ) == 0 )
				nAlign = TA_RIGHT;
			else
				nAlign = TA_LEFT;

			m_Ruler.AddSubColumn ( m_Ruler.GetSize()-1, csv.GetString(i*3+6), nAlign, csv.GetDouble(i*3+5) );
		}

		CCSV csvSubHeader ( '\t' );

		for ( int i = 0 ; i < m_Ruler.GetSize() ; i++ )
		{
			if ( m_Ruler.GetSubTabSize(i) > 0 )
			{
				for ( int j = 0 ; j < m_Ruler.GetSubTabSize(i) ; j++ )
					csvSubHeader.Add ( m_Ruler.GetSubTabText(i,j) );
			}
			else
				csvSubHeader.Add ( "" );
		}

		CString strSubHeader = csvSubHeader.GetLine();

		if ( strSubHeader.GetLength() > m_Ruler.GetSize() )	// not just blank columns
			m_strSubHeader = strSubHeader;
		else
			m_strSubHeader = "";		// no sub header

		return TRUE;
	}
	else if ( strncmp ( szBuf, "<HEADER>", 8 ) == 0 )
	{
		m_HeaderArray.Add ( &szBuf[8] );
		return TRUE;
	}
	else if ( strncmp ( szBuf, "<FOOTER>", 8 ) == 0 )
	{
		m_FooterArray.Add ( &szBuf[8] );
		return TRUE;
	}
	else if ( strncmp ( szBuf, "<TITLE>", 7 ) == 0 )
	{
		m_strTitle = &szBuf[7];
		return TRUE;
	}
	else if ( strncmp ( szBuf, "<HIDE", 5 ) == 0 )
	{
		if ( szBuf[5] == '>' )
		{
			if (( m_nRevealLevel & 0x0001 ) != 0 )	// reveal
				m_TextArray.Add ( &szBuf[6] );	// discard <HIDE>

			return TRUE;
		}
		else if ( szBuf[6] == '>' )
		{
			bool bReveal = FALSE;

			switch ( szBuf[5] )
			{
			case '1':	if (( m_nRevealLevel & 0x0002 ) != 0 )	bReveal = TRUE;	break;
			case '2':	if (( m_nRevealLevel & 0x0004 ) != 0 )	bReveal = TRUE;	break;
			case '3':	if (( m_nRevealLevel & 0x0008 ) != 0 )	bReveal = TRUE;	break;
			case '4':	if (( m_nRevealLevel & 0x0010 ) != 0 )	bReveal = TRUE;	break;
			case '5':	if (( m_nRevealLevel & 0x0020 ) != 0 )	bReveal = TRUE;	break;
			case '6':	if (( m_nRevealLevel & 0x0040 ) != 0 )	bReveal = TRUE;	break;
			case '7':	if (( m_nRevealLevel & 0x0080 ) != 0 )	bReveal = TRUE;	break;
			case '8':	if (( m_nRevealLevel & 0x0100 ) != 0 )	bReveal = TRUE;	break;
			case '9':	if (( m_nRevealLevel & 0x0200 ) != 0 )	bReveal = TRUE;	break;
			}

			if ( bReveal == TRUE )
				m_TextArray.Add ( &szBuf[7] );	// discard <HIDEn>

			return TRUE;
		}
	}

	return FALSE;
}

/**********************************************************************/
/*														*/
/*	Pagination											*/
/*														*/
/**********************************************************************/

int CReportFileDoc::Paginate ( int nLinesPerPage )
{
	return m_PageArray.Paginate ( m_TextArray, nLinesPerPage );
}

int CReportFileDoc::GetPageNo ( int nPageNo )
{
	return m_PageArray.GetPageNo ( nPageNo-1 );
}

const char* CReportFileDoc::GetPageText ( int nPageNo, int nLine )
{
	if ( nLine < m_PageArray.GetCount ( nPageNo-1 ) )
		return GetPrintTextAt ( m_PageArray.GetIndex ( nPageNo-1 ) + nLine );
	else
		return "";
}

/**********************************************************************/
