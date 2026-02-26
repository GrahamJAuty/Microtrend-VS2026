/**********************************************************************/

#define DDF_NONE	0
#define DDF_INT	1
#define DDF_LONG	2
#define DDF_DOUBLE	3
#define DDF_TEXT	4

/**********************************************************************/
/*	CEditFilter											*/
/**********************************************************************/

BEGIN_MESSAGE_MAP ( CEditFilter, CEdit )
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_MESSAGE(WM_PASTE,OnPaste)
END_MESSAGE_MAP()

/**********************************************************************/

CEditFilter::CEditFilter ( int nType, const char* szFmt )
{
	SetFormat ( nType, szFmt );
}

/**********************************************************************/

void CEditFilter::SetFormat ( int nType, const char* szFmt )
{
	m_bAllowBlank = (( nType & 0x80 ) != 0 ) ? TRUE : FALSE;
	m_nType = nType & 0x7f;
	m_strFmt = szFmt;
	m_nNumType = DDF_NONE;

	if ( m_nType == SS_DATE )
		m_strFmt = "%d/%m/%Y";
	else
	{
		if ( m_strFmt != "" )
		{
			if ( m_strFmt.Right(1) == "f" )		m_nNumType = DDF_DOUBLE;
			else if ( m_strFmt.Right(2) == "ld" )	m_nNumType = DDF_LONG;
			else if ( m_strFmt.Right(2) == "li" )	m_nNumType = DDF_LONG;
			else if ( m_strFmt.Right(2) == "lu" )	m_nNumType = DDF_LONG;
			else if ( m_strFmt.Right(1) == "d" )	m_nNumType = DDF_INT;
			else if ( m_strFmt.Right(1) == "i" )	m_nNumType = DDF_INT;
			else if ( m_strFmt.Right(1) == "u" )	m_nNumType = DDF_INT;
			else if ( m_strFmt.Right(1) == "s" )	m_nNumType = DDF_TEXT;
		}
	}
}

/**********************************************************************/

bool CEditFilter::ValidateNumber ( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	bool bValid = FALSE;
	bool bSign = FALSE;
	bool bDp = FALSE;

	if ( m_nType == SS_NUM )
	{
		if ( ( nChar >= '0') && ( nChar <= '9' ) )
			bValid = TRUE;
	}
	else if (  m_nType == SS_NUM_S )
	{
		bSign = TRUE;
		if ( ( nChar >= '0') && ( nChar <= '9' ) || nChar == '-' )
			bValid = TRUE;
	}
	else if (  m_nType == SS_NUM_DP )
	{
		bDp = TRUE;
		if ( ( nChar >= '0') && ( nChar <= '9' ) || nChar == '.' )
			bValid = TRUE;
	}
	else if (  m_nType == SS_NUM_SDP )
	{
		bSign = TRUE;
		bDp = TRUE;
		if ( ( nChar >= '0') && ( nChar <= '9' ) || nChar == '.' || nChar == '-' )
			bValid = TRUE;
	}

	if ( bValid == TRUE )
	{
		bValid = FALSE;

		CString strBuffer;
		GetWindowText ( strBuffer );
		int nLength = strBuffer.GetLength();
		int nStartPos;
		int nEndPos;

		GetSel ( nStartPos, nEndPos );
		if ( (nEndPos-nStartPos) == nLength )		/* highlighted */
			bValid = TRUE;
		else
		{
			if ( nLength > 0 )
			{
				if ( strBuffer.GetAt(0) != '-' || nStartPos != 0 )
				{
					if (( nChar >= '0') && ( nChar <= '9' ))
						bValid = TRUE;

					else if ( nChar == '-' && bSign == TRUE )
					{
						if ( strBuffer.Find ( '-' ) == -1 )	/* only 1 minus allowed */
							if ( nStartPos == 0 )			/* must be 1st char */
								bValid = TRUE;
					}
					else if ( nChar == '.' && bDp == TRUE )
					{
						if ( strBuffer.Find ( '.' ) == -1 )	/* only 1 dp allowed */
							bValid = TRUE;
					}
				}
			}
		}
	}

	return bValid;
}

/**********************************************************************/

void CEditFilter::OnChar ( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	bool bValid = FALSE;

	if ( nChar < 0x20 )			/* allow control characters */
		bValid = TRUE;
	else
	{
		if ( m_nType == SS_NUM || m_nType == SS_NUM_S || m_nType == SS_NUM_DP || m_nType == SS_NUM_SDP )
		{
			bValid = ValidateNumber ( nChar, nRepCnt, nFlags );
		}
		else if ( m_nType == SS_DATE )
		{
			if ( ( nChar >= '0') && ( nChar <= '9' ) || nChar == '/' )
				bValid = TRUE;

			else if ( nChar == '+' )
			{
				CString strBuf;
				GetWindowText ( strBuf );
				COleDateTime Date;
				if ( Date.ParseDateTime ( strBuf ) == TRUE )
				{
					Date += COleDateTimeSpan(1,0,0,0);		/* add 1 day */
					strBuf = Date.Format ( m_strFmt );
					SetWindowText ( strBuf );
				}
				return;
			}

			else if ( nChar == '-' )
			{
				CString strBuf;
				GetWindowText ( strBuf );
				COleDateTime Date;
				if ( Date.ParseDateTime ( strBuf ) == TRUE )
				{
					Date -= COleDateTimeSpan(1,0,0,0);		/* subtract 1 day */
					strBuf = Date.Format ( m_strFmt );
					SetWindowText ( strBuf );
				}
				return;
			}
		}
	}

	if ( bValid == TRUE )
		CEdit::OnChar ( nChar, nRepCnt, nFlags );
	else
		::MessageBeep ( MB_OK );
}

/**********************************************************************/

void CEditFilter::OnKillFocus ( CWnd* pNewWnd )
{
	FormatWindowText();

	CEdit::OnKillFocus ( pNewWnd );

	if ( pNewWnd != NULL )
		((CEdit*)pNewWnd)->SetSel(0,-1);
}

/**********************************************************************/

bool CEditFilter::ValidateNumberAfterPaste( CString& strNum )
{
	bool bValid = TRUE;

	switch( m_nType )
	{
	case SS_NUM:
	case SS_NUM_S:
	case SS_NUM_DP:
	case SS_NUM_SDP:
		{
			bool bGotDecimal = FALSE;
			for ( int n = 0; ( n < strNum.GetLength() ) && ( TRUE == bValid ) ; n++ )
			{
				char c = strNum.GetAt(n);
				
				if ( ( c < '0' ) || ( c > '9' ) )
				{
					if ( c == '-' )
					{
						if ( ( n != 0 ) || ( SS_NUM == m_nType ) || ( SS_NUM_DP == m_nType ) )
							bValid = FALSE;
					}
					else if ( c == '.' )
					{
						if ( ( TRUE == bGotDecimal ) || ( SS_NUM == m_nType ) || ( SS_NUM_S == m_nType ) )
							bValid = FALSE;
						else
							bGotDecimal = TRUE;
					}
					else
					{
						bValid = FALSE;
					}
				}
			}
		}
		break;
	}

	return bValid;
}

/**********************************************************************/

LRESULT CEditFilter::OnPaste(WPARAM wParam, LPARAM lParam)
{
	CString strOldText;
	GetWindowText( strOldText );

	int nStart, nEnd;
	GetSel( nStart, nEnd );

	CWnd::Default();

	CString strNewText;
	GetWindowText( strNewText );
	if ( ValidateNumberAfterPaste( strNewText ) == FALSE )
	{
		SetWindowText( strOldText );
		SetSel( nStart, nEnd );
	}
	
	return 0;
}

/**********************************************************************/

void CEditFilter::FormatWindowText()
{
	CString strOld;
	GetWindowText ( strOld );

	CString strNew = Format ( strOld );

	if ( strNew != strOld )
		SetWindowText ( strNew );
}

/**********************************************************************/

CString CEditFilter::Format ( const char* szBuf )
{
	CString strBuf;

	if ( m_nType == SS_DATE )
	{
		COleDateTime Date;

		if ( Date.ParseDateTime ( szBuf ) == TRUE )		// else blank
			strBuf = Date.Format ( m_strFmt );
	}
	else
	{
		if ( m_bAllowBlank == TRUE )		// blank field is allowed
		{
			if ( strlen(szBuf) == 0 )
				return strBuf;
		}

		switch ( m_nNumType )
		{
		case DDF_INT:		strBuf.Format ( m_strFmt, atoi(szBuf) );	break;

		case DDF_LONG:		strBuf.Format ( m_strFmt, atol(szBuf) );	break;

		case DDF_DOUBLE:	strBuf.Format ( m_strFmt, atof(szBuf) );	// prevent -0.00
						strBuf.Format ( m_strFmt, atof(strBuf) );
						break;

		case DDF_TEXT:		strBuf.Format ( m_strFmt, szBuf );		break;
		}
	}

	return strBuf;
}

/**********************************************************************/
