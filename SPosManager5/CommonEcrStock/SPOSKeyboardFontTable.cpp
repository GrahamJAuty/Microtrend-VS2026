/**********************************************************************/
#include "SPOSKeyboardFontTable.h"
/**********************************************************************/

CSPOSKeyboardStoredFont::CSPOSKeyboardStoredFont ( CDC* pDC, int nFontSize, bool bBold )
{
	pDC -> SetMapMode ( MM_TEXT );

	long lWeight = ( bBold ) ? FW_BOLD : FW_NORMAL;

	LOGFONT lf;
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

CSPOSKeyboardFontTable::CSPOSKeyboardFontTable()
{
	m_bIsValid = TRUE;
}

/**********************************************************************/

void CSPOSKeyboardFontTable::Reset()
{
	for ( int n = GetDialogFontCount() - 1; n >= 0; n-- )
	{
		CSPOSKeyboardStoredFont* pFont = m_arrayDialogFonts.GetAt(n);
		delete ( pFont );
	}

	m_arrayDialogFonts.RemoveAll();
}

/**********************************************************************/

CSPOSKeyboardFontTable::~CSPOSKeyboardFontTable()
{
	for ( int n = GetDialogFontCount() - 1; n >= 0; n-- )
	{
		CSPOSKeyboardStoredFont* pFont = m_arrayDialogFonts.GetAt(n);
		delete ( pFont );
	}
}

/**********************************************************************/

void CSPOSKeyboardFontTable::BuildFontTable(CDC* pDC)
{
	//ITEM86 FONTS
	CArray<int, int> arrayFontSize;
	arrayFontSize.Add(8);
	arrayFontSize.Add(10);
	arrayFontSize.Add(11);
	arrayFontSize.Add(12);
	arrayFontSize.Add(14);
	arrayFontSize.Add(16);
	arrayFontSize.Add(20);
	arrayFontSize.Add(24);
	arrayFontSize.Add(26);
	arrayFontSize.Add(28);
	arrayFontSize.Add(36);
	arrayFontSize.Add(48);
	arrayFontSize.Add(72);
	arrayFontSize.Add(120);

	for (int n = 0; n < arrayFontSize.GetSize(); n++)
	{
		int nFontSize = arrayFontSize.GetAt(n);
		AddDialogFont(pDC, nFontSize, FALSE);
		AddDialogFont(pDC, nFontSize, TRUE);
	}
}

/**********************************************************************/

void CSPOSKeyboardFontTable::AddDialogFont ( CDC* pDC, int nFontSize, bool bBold )
{
	if (m_bIsValid == TRUE)
	{
		CSPOSKeyboardStoredFont* pStoredFont = new CSPOSKeyboardStoredFont(pDC, nFontSize, bBold);

		if (pStoredFont == NULL)
		{
			m_bIsValid = FALSE;
		}
		else
		{
			m_arrayDialogFonts.Add(pStoredFont);
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardFontTable::GetKeyDisplayFont( CDC* pDC, CFont* Font, int nFontSize, bool bBold, double dZoomRatio )
{
	int nBigFontSize = 80;

	if ( CSPOSKeyboardKeyRecord::IsValidFontSize(nFontSize) == TRUE )
	{ 
		nBigFontSize = nFontSize * 10;
	}

	nBigFontSize = int ( nBigFontSize * dZoomRatio );
	long lWeight = ( bBold ? FW_BOLD : FW_NORMAL );
	
	pDC -> SetMapMode ( MM_TEXT );

	LOGFONT lf;
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

CFont* CSPOSKeyboardFontTable::GetDialogFont ( int nFontSize, bool bBold )
{
	for (int n = 0; n < GetDialogFontCount(); n++)
	{
		CSPOSKeyboardStoredFont* pStoredFont = m_arrayDialogFonts.GetAt(n);

		if ((pStoredFont->GetFontSize() == nFontSize) && (pStoredFont->GetBoldFlag() == bBold))
		{
			return pStoredFont->GetFont();
		}
	}

	CSPOSKeyboardStoredFont* pStoredFont = m_arrayDialogFonts.GetAt( 0 );
	return pStoredFont -> GetFont();
}

/**********************************************************************/
