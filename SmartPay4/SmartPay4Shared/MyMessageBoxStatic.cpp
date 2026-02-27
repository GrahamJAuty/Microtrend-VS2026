/**********************************************************************/
#include "MyMessageBoxStatic.h"
/**********************************************************************/

CMyMessageBoxStatic::CMyMessageBoxStatic( CMyMessageBoxInfo& MessageBoxInfo ) : m_MessageBoxInfo( MessageBoxInfo )
{
	m_nTextWidth = 100;
	m_nTextHeight = 100;
	m_bCanPaint = FALSE;
}

/**********************************************************************/

CMyMessageBoxStatic::~CMyMessageBoxStatic()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CMyMessageBoxStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

/**********************************************************************/

void CMyMessageBoxStatic::CalculateTextExtents()
{ 
	CPaintDC dc(this);

	CFont Font;
	GetFont( &dc, Font, m_MessageBoxInfo.GetFontSize() );	
	CFont* pOldFont = dc.SelectObject ( &Font );

	int nLineHeight = 0;
	int nLineGap = m_MessageBoxInfo.GetLineGap();
	int nTextCount = m_MessageBoxInfo.GetTextCount();
	
	{
		CSize cs = dc.GetTextExtent( "A" );	
		nLineHeight = cs.cy;
	}

	m_nTextHeight = 50;
	m_nTextHeight += ( nTextCount * ( nLineHeight + nLineGap ) );
	m_nTextHeight -= nLineGap;
	
	for ( int n = 0; n < nTextCount; n++ )
	{ 
		CString strText = m_MessageBoxInfo.GetText(n);

		CSize cs = dc.GetTextExtent( strText );		
		
		if ( cs.cx > m_nTextWidth )			
			m_nTextWidth = cs.cx;
 
		if ( strText == "" )
			m_nTextHeight -= nLineHeight;
	}
 
	if ( m_nTextWidth < 200 )
		m_nTextWidth = 200;

	if ( m_nTextHeight < 70 )
		m_nTextHeight = 70;
}

/**********************************************************************/

void CMyMessageBoxStatic::OnPaint()
{
	if ( TRUE == m_bCanPaint )
	{
		// Device context for painting

		CPaintDC dc(this);
	    
		// Double-buffering

		CDC*        pDCMem        = new CDC;
		CBitmap*    pOldBitmap    = NULL;
		CBitmap     bmpCanvas;
		CRect       rFrame;

		GetClientRect( rFrame );

		pDCMem->CreateCompatibleDC( &dc );

		bmpCanvas.CreateCompatibleBitmap( &dc, rFrame.Width(), rFrame.Height() );

		pOldBitmap = pDCMem->SelectObject( &bmpCanvas );

		// START DRAW -------------------------------------------------

		pDCMem->FillSolidRect( rFrame, m_MessageBoxInfo.GetBackColour() );    // Background
		DrawText( pDCMem, rFrame );

		LPCTSTR IconName = NULL;
		switch( m_MessageBoxInfo.GetIconType() )
		{
		case MB_ICONQUESTION:		IconName = IDI_QUESTION;		break;
		case MB_ICONEXCLAMATION:	IconName = IDI_EXCLAMATION;		break;
		case MB_ICONINFORMATION:	IconName = IDI_INFORMATION;		break;
		case MB_ICONSTOP:			IconName = IDI_HAND;			break;
		}

		if ( IconName != NULL )
		{
			CWinApp* pApp = AfxGetApp(); 
			HICON hIcon = pApp -> LoadStandardIcon( IconName );
			pDCMem -> SetMapMode( MM_TEXT );
			pDCMem -> DrawIcon( 20, 20, hIcon );
		}
		
		// END DRAW   -------------------------------------------------

		dc.BitBlt( 0, 0, rFrame.Width(), rFrame.Height(), pDCMem, 0, 0, SRCCOPY );

		pDCMem-> SelectObject( pOldBitmap );

		delete pDCMem;
	}
}

/**********************************************************************/

void CMyMessageBoxStatic::DrawText ( CDC* pDC, CRect rect )
{
	int nLines = m_MessageBoxInfo.GetTextCount();
	if ( nLines > 0 )
	{
		pDC -> SetBkMode ( TRANSPARENT );
		pDC -> SetTextColor ( m_MessageBoxInfo.GetTextColour() );

		CFont Font;
		GetFont( pDC, Font, m_MessageBoxInfo.GetFontSize() );
		
		UINT nOldAlign = pDC-> SetTextAlign ( TA_LEFT );
		CFont* pOldFont = pDC -> SelectObject ( &Font );

		TEXTMETRIC tm;
		pDC -> GetTextMetrics ( &tm );

		int nLineHeight = tm.tmHeight + m_MessageBoxInfo.GetLineGap();
		int nLineSpace = ( nLineHeight * nLines ) - m_MessageBoxInfo.GetLineGap();
		int nHeight = rect.Height();

		//height and y axis are negative
		int nYStart = 25;

		pDC -> SetTextAlign( TA_LEFT );
			
		for ( int nToken = 0; nToken < nLines; nToken++ )
		{			
			CString strToken = m_MessageBoxInfo.GetText( nToken );
				
			if ( strToken != "" )
			{
				pDC -> ExtTextOut ( 70, nYStart, ETO_CLIPPED, rect, strToken, strToken.GetLength(), NULL );	
				nYStart += nLineHeight;
			}
			else
			{
				nYStart += m_MessageBoxInfo.GetLineGap();
			}
		}
		
		pDC -> SetTextAlign ( nOldAlign );
		pDC -> SelectObject ( pOldFont );
	}
}

/**********************************************************************/

void CMyMessageBoxStatic::GetFont( CDC* pDC, CFont& Font, int nFontSize )
{
	int nBigFontSize = 80;

	switch( nFontSize )
	{
	case 8:
	case 10:
	case 11:
	case 12:
	case 14:
		nBigFontSize = nFontSize * 10;
		break;
	}

	pDC -> SetMapMode ( MM_TEXT );

	LOGFONT lf;
	lf.lfHeight = nBigFontSize;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = FALSE;
	lf.lfUnderline = FALSE;
	lf.lfStrikeOut = FALSE;
	lf.lfCharSet = ANSI_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s( lf.lfFaceName, "Tahoma" );
	
	Font.CreatePointFontIndirect ( &lf, pDC );
}

/**********************************************************************/



