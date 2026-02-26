/**********************************************************************/
 
/**********************************************************************/
#include "SubUnitFractionEdit.h"
/**********************************************************************/
static const int chartype_control = 0;
static const int chartype_minus = 1;
static const int chartype_number = 2;
static const int chartype_slash = 3;
static const int chartype_dot = 4;
static const int chartype_invalid = 5;
static const int state_want_minus = 1;
static const int state_want_stock = 2;
static const int state_want_sub = 3;
static const int state_want_fraction = 4;
static const int state_invalid = 5;
/**********************************************************************/

CSubUnitFractionEdit::CSubUnitFractionEdit() : CEdit()
{
	m_nSUYield = 1;
	m_bSigned = FALSE;
}

/**********************************************************************/

BOOL CSubUnitFractionEdit::SubclassDlgItem( UINT nID, CWnd* pParent, int nWidth, int nSUYield, bool bSigned )
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

void CSubUnitFractionEdit::SetMode( int nWidth, int nSUYield, bool bSigned )
{
	m_nSUYield = nSUYield;
	if ( m_nSUYield < 1 ) m_nSUYield = 1;
	m_bSigned = bSigned;

	if ( nWidth > 0 )
		LimitText( nWidth );
}

/**********************************************************************/
BEGIN_MESSAGE_MAP (CSubUnitFractionEdit, CEdit)
	ON_WM_CHAR ()
	ON_WM_KILLFOCUS ()
END_MESSAGE_MAP ()
/**********************************************************************/

void CSubUnitFractionEdit::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	bool bValid = FALSE;

	switch ( GetCharType ( nChar ) )
	{
	case chartype_control:
		bValid = TRUE;
		break;

	case chartype_invalid:
		bValid = FALSE;
		break;

	default:
		{
			CString strText;
			GetWindowText( strText );
			int nLength = strText.GetLength();

			int nStart, nEnd;
			GetSel ( nStart, nEnd );

			if ( nEnd > nStart )
			{
				CString strStart = "";
				CString strEnd = "";

				if ( nStart > 0 ) strStart = strText.Left( nStart );
				if ( nEnd < nLength ) strEnd = strText.Right ( nLength - nEnd );

				strText = strStart;
				strText += (char) nChar;
				strText += strEnd;
			}
			else
				strText.Insert( nStart, nChar );
	
			double dQty;
			bValid = GetSubUnitQuantity( strText, dQty );
		}
		break;
	}

	if ( bValid == TRUE )
		CEdit::OnChar ( nChar, nRepCnt, nFlags );
	else
		::MessageBeep ( MB_OK );
}

/**********************************************************************/

void CSubUnitFractionEdit::OnKillFocus( CWnd* pNewWnd )
{
	double d;
	CString strText;
	
	GetWindowText( strText );
	GetSubUnitQuantity( strText, d );
	GetSubUnitString( d, strText );
	SetWindowText ( strText );

	CEdit::OnKillFocus( pNewWnd );
}

/**********************************************************************/

void CSubUnitFractionEdit::GetSubUnitString( double dQty, CString& strSubUnits )
{
	CString strQty;
	strQty.Format ( "%.5f", dQty );

	int nStockUnits = atoi ( strQty.Left( strQty.GetLength() - 6 ) );
	double dFraction = atof ( strQty.Right(6) );

	strSubUnits.Format ( "%s/%.3f",
		(const char*) strQty.Left ( strQty.GetLength() - 6 ),
		dFraction * (double) m_nSUYield );

	if ( strSubUnits.Right(4) == ".000" )
		strSubUnits = strSubUnits.Left( strSubUnits.GetLength() - 4 );
}

/**********************************************************************/

int CSubUnitFractionEdit::GetCharType( UINT c )
{
	if ( c < 0x20 )
		return chartype_control;

	switch ( c )
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	return chartype_number;
	case '-':	return chartype_minus;
	case '.':	return chartype_dot;
	case '\\':
	case '/':	return chartype_slash;
	default:	return chartype_invalid;
	}
}

/**********************************************************************/

bool CSubUnitFractionEdit::GetSubUnitQuantity( const char* szSubUnits, double& dQty )
{
	bool bMinus = FALSE;
	CString strStock = "";
	CString strSub = "";
	CString strFraction = "";
	
	int nPos = 0;
	int nState = ( m_bSigned ) ? state_want_minus : state_want_stock;
	
	CString strBuffer = szSubUnits;
	while ( ( nPos < strBuffer.GetLength() ) && ( nState != state_invalid ) )
	{
		char c = strBuffer.GetAt( nPos );
		int nCharType = GetCharType ( c );

		switch ( nState )
		{
		case state_want_minus:
			switch ( nCharType )
			{
			case chartype_minus:	nState = state_want_stock;	bMinus = TRUE;	break;
			case chartype_number:	nState = state_want_stock;	strStock = c;	break;
			case chartype_slash:	nState = state_want_sub; break;
			default:				nState = state_invalid;	break;
			}
			break;
	
		case state_want_stock:
			switch ( nCharType )
			{
			case chartype_number:	strStock += c;	break;
			case chartype_slash:	nState = state_want_sub; break;
			default:				nState = state_invalid;	break;
			}
			break;

		case state_want_sub:
			switch ( nCharType )
			{
			case chartype_number:	strSub += c;	break;
			case chartype_dot:		nState = state_want_fraction; break;
			default:				nState = state_invalid;	break;
			}
			break;

		case state_want_fraction:
			switch ( nCharType )
			{
			case chartype_number:	strFraction += c;	break;
			default:				nState = state_invalid;	break;
			}
			break;
		}
		nPos++;
	}

	strFraction = strFraction + "000";
	strFraction = strFraction.Left(3);

	strSub += ".";
	strSub += strFraction;

	dQty = atof ( strStock ) + ( atof ( strSub ) / (double) m_nSUYield );
	
	return ( nState != state_invalid );
}

/**********************************************************************/

