/**********************************************************************/
#include "FontTable.h"
/**********************************************************************/

CStoredFont::CStoredFont ( CDC* pDC, int nFontSize, bool bBold )
{
	pDC -> SetMapMode ( MM_TEXT );

	long lWeight = ( bBold ) ? FW_BOLD : FW_NORMAL;

	LOGFONT lf{};
	lf.lfHeight = nFontSize * 10;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = lWeight;
	lf.lfItalic = FALSE;
	lf.lfUnderline = FALSE;
	lf.lfStrikeOut = FALSE;
	lf.lfCharSet = ANSI_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s ( lf.lfFaceName, "Tahoma" );
	m_Font.CreatePointFontIndirect ( &lf, pDC );

	m_nFontSize = nFontSize;
	m_bBold = bBold;
}

/**********************************************************************/

CFontTable::CFontTable()
{
	m_bIsValid = TRUE;
}

/**********************************************************************/

CFontTable::~CFontTable()
{
	for ( int n = GetDialogFontCount() - 1; n >= 0; n-- )
	{
		CStoredFont* pFont = m_arrayDialogFonts.GetAt(n);
		delete ( pFont );
	}
}

/**********************************************************************/

void CFontTable::BuildFontTable ( CDC* pDC )
{
	//ITEM86 FONTS
	AddDialogFont ( pDC, 8, FALSE );
	AddDialogFont ( pDC, 10, FALSE );
	AddDialogFont ( pDC, 11, FALSE );
	AddDialogFont ( pDC, 12, FALSE );
	AddDialogFont ( pDC, 14, FALSE );
	AddDialogFont ( pDC, 16, FALSE );
	AddDialogFont ( pDC, 18, FALSE );
	AddDialogFont ( pDC, 20, FALSE );
	AddDialogFont ( pDC, 22, FALSE );
	AddDialogFont ( pDC, 24, FALSE );
	AddDialogFont ( pDC, 26, FALSE );
	AddDialogFont ( pDC, 28, FALSE );
	AddDialogFont ( pDC, 30, FALSE );
	AddDialogFont ( pDC, 8, TRUE );
	AddDialogFont ( pDC, 10, TRUE );
	AddDialogFont ( pDC, 11, TRUE );
	AddDialogFont ( pDC, 12, TRUE );
	AddDialogFont ( pDC, 14, TRUE );
	AddDialogFont ( pDC, 16, TRUE );
	AddDialogFont ( pDC, 18, TRUE );
	AddDialogFont ( pDC, 20, TRUE );
	AddDialogFont ( pDC, 22, TRUE );
	AddDialogFont ( pDC, 24, TRUE );
	AddDialogFont ( pDC, 26, TRUE );
	AddDialogFont ( pDC, 28, TRUE );
	AddDialogFont ( pDC, 30, TRUE );
}

/**********************************************************************/

void CFontTable::AddDialogFont ( CDC* pDC, int nFontSize, bool bBold )
{
	if ( m_bIsValid == TRUE )
	{
		CStoredFont* pStoredFont = new CStoredFont( pDC, nFontSize, bBold );

		if ( pStoredFont == NULL )
			m_bIsValid = FALSE;
		else
			m_arrayDialogFonts.Add( pStoredFont );
	}
}

/**********************************************************************/

CFont* CFontTable::GetDialogFont ( int nFontSize, bool bBold )
{
	for ( int n = 0; n < GetDialogFontCount(); n++ )
	{	
		CStoredFont* pStoredFont = m_arrayDialogFonts.GetAt( n );
	
		if ( ( pStoredFont -> GetFontSize() == nFontSize ) && ( pStoredFont -> GetBoldFlag() == bBold ) )
			return pStoredFont -> GetFont();
	}

	CStoredFont* pStoredFont = m_arrayDialogFonts.GetAt( 0 );
	return pStoredFont -> GetFont();
}

/**********************************************************************/

void CFontTable::GetKeyDisplayFont( CDC* pDC, CFont* Font, int nFontSize, bool bBold )
{
	int nBigFontSize = 80;

	switch( nFontSize )
	{
	case 6:
	case 8:
	case 10:
	case 11:
	case 12:
	case 14:
	case 16:
	case 18:
	case 20:
	case 22:
	case 24:
	case 26:
	case 28:
	case 30:
		nBigFontSize = nFontSize * 10;
		break;
	}

	long lWeight = ( bBold ? FW_BOLD : FW_NORMAL );
	
	pDC -> SetMapMode ( MM_TEXT );

	LOGFONT lf{};
	lf.lfHeight = nBigFontSize;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = lWeight;
	lf.lfItalic = FALSE;
	lf.lfUnderline = FALSE;
	lf.lfStrikeOut = FALSE;
	lf.lfCharSet = ANSI_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s ( lf.lfFaceName, "Tahoma" );
	
	Font -> CreatePointFontIndirect ( &lf, pDC );
}

/**********************************************************************/