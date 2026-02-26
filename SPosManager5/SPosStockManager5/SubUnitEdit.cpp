/**********************************************************************/
#include "SubUnitEdit.h"
/**********************************************************************/

CSubUnitEdit::CSubUnitEdit() : CEdit()
{
	m_nSUYield = 1;
	m_bSigned = FALSE;
}

/**********************************************************************/

BOOL CSubUnitEdit::SubclassDlgItem( UINT nID, CWnd* pParent, int nWidth, int nSUYield, bool bSigned )
{
	BOOL bReply;

	m_nSUYield = nSUYield;
	if ( m_nSUYield < 1 ) m_nSUYield = 1;
	m_bSigned = bSigned;

	if (( bReply = CEdit::SubclassDlgItem ( nID, pParent )) == TRUE )
		if ( nWidth > 0 )
			LimitText ( nWidth );

	return bReply;
}

/**********************************************************************/

void CSubUnitEdit::SetMode( int nWidth, int nSUYield, bool bSigned )
{
	m_nSUYield = nSUYield;
	if ( m_nSUYield < 1 ) m_nSUYield = 1;
	m_bSigned = bSigned;

	if ( nWidth > 0 )
		LimitText( nWidth );
}

/**********************************************************************/
BEGIN_MESSAGE_MAP (CSubUnitEdit, CEdit)
	ON_WM_CHAR ()
	ON_WM_KILLFOCUS ()
END_MESSAGE_MAP ()
/**********************************************************************/

void CSubUnitEdit::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	bool bValid = FALSE;

	if ( nChar < 0x20 )			/* allow control characters */
		bValid = TRUE;
	else
	{
		if ( IsValidChar ( nChar ) == TRUE )
		{
			CString strBuffer;
			GetWindowText ( strBuffer );
			int nLength = strBuffer.GetLength();
			int nStartPos;
			int nEndPos;

			GetSel ( nStartPos, nEndPos );
			if ( (nEndPos - nStartPos) == nLength )		/* highlighted */
				bValid = TRUE;
			else
			{
				if ( nLength > 0 )
				{
					if ( strBuffer.GetAt(0) != '-' || nStartPos != 0 )
					{
						if (( nChar >= '0') && ( nChar <= '9' ))
							bValid = TRUE;

						else if ( nChar == '-' && m_bSigned == TRUE )
						{
							if ( strBuffer.Find ( '-' ) == -1 )	/* only 1 minus allowed */
								if ( nStartPos == 0 )			/* must be 1st char */
									bValid = TRUE;
						}
						else if ( nChar == '.' || nChar == '\\' || nChar == '/' )
						{
							if ( strBuffer.FindOneOf ( "\\/" ) == -1 )	/* only 1 deliminator allowed */
								bValid = TRUE;
						}
					}
				}
			}
		}
	}

	if ( bValid == TRUE )
		CEdit::OnChar ( nChar, nRepCnt, nFlags );
	else
		::MessageBeep ( MB_OK );
}

/**********************************************************************/

void CSubUnitEdit::OnKillFocus( CWnd* pNewWnd )
{
	__int64 n;
	CString strText;
	
	GetWindowText( strText );
	GetSubUnitQuantity( strText, n );
	GetSubUnitString( n, strText );
	SetWindowText ( strText );

	CEdit::OnKillFocus( pNewWnd );
}

/**********************************************************************/

bool CSubUnitEdit::IsValidChar( UINT nChar )
{
	bool bValid = FALSE;

	if (( nChar >= '0') && ( nChar <= '9' ))
		bValid = TRUE;
	else if ( nChar == '-' && m_bSigned == TRUE )
		bValid = TRUE;
	else if ( nChar == '\\' || nChar == '/' )
		bValid = TRUE;

	return bValid;
}

/**********************************************************************/

void CSubUnitEdit::GetSubUnitString( __int64 n, CString& strSubUnits )
{
	CString strSign = "";
	if ( n < 0 )
	{
		strSign = "-";
		n *= -1;
	}

	__int64 nStock = ( n / m_nSUYield );
	__int64 nSub = ( n - ( nStock * m_nSUYield ) );

	CString strNumber;
	strNumber.Format ( "%I64d/%I64d", nStock, nSub );
	strSubUnits = strSign + strNumber;
}

/**********************************************************************/

void CSubUnitEdit::GetSubUnitQuantity( const char* szSubUnits, __int64& n )
{
	__int64 nStock = 0;
	__int64 nSub = 0;

	CString strText = szSubUnits;
	int nLength = strText.GetLength();

	bool bNegative = FALSE;
	if ( ( nLength != 0 ) && ( strText.GetAt(0) == '-' ) )
	{
		bNegative = TRUE;
		strText = strText.Right ( nLength - 1 );
		nLength -= 1;
	}

	int nPos = 0;
	bool bGotSubUnits = FALSE;

	for ( nPos = 0; nPos < nLength; nPos++ )
	{
		char c = strText.GetAt( nPos );
		if ( c == '\\' || c == '/' )
		{
			bGotSubUnits = TRUE;
			break;
		}
	}

	if ( bGotSubUnits == FALSE )
		nStock = _atoi64 ( strText );
	else
	{
		if ( nPos > 0 )
			nStock = _atoi64 ( strText.Left( nPos ) );
		
		if ( nPos < nLength - 1 )
			nSub = _atoi64 ( strText.Right( nLength - nPos - 1 ) );
	}
	
	n = ( nStock * m_nSUYield ) + nSub;
	if ( bNegative ) n *= -1;
}

/**********************************************************************/

