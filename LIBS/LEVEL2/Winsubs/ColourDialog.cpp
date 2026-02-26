/**********************************************************************/

#define szDEFAULT_FONT	"Arial"

/**********************************************************************/
/*	Helper routines										*/
/**********************************************************************/

static BYTE ConvertRGBtoGrey ( BYTE ucR, BYTE ucG, BYTE ucB )
{
	static BYTE ucPrevR = 0;	static int nPrevR = 0;
	static BYTE ucPrevG = 0;	static int nPrevG = 0;
	static BYTE ucPrevB = 0;	static int nPrevB = 0;

	int nR = nPrevR;
	int nG = nPrevG;
	int nB = nPrevB;

	if ( ucR != ucPrevR )
	{
		ucPrevR = ucR;
		nR = (int) ucR * 299;
		nPrevR = nR;
	}

	if ( ucG != ucPrevG )
	{
		ucPrevG = ucG;
		nG = (int) ucG * 587;
		nPrevG = nG;
	}

	if ( ucB != ucPrevB )
	{
		ucPrevB = ucB;
		nB = (int) ucB * 114;
		nPrevB = nB;
	}

	int nGrey = ( nR + nG + nB ) / 1000;		// r * 0.299 + g * 0.587 + b * 0.114

	return (BYTE)(( nGrey * 80 ) / 100 );		// reduce intensity to 80%
}

/**********************************************************************/

static COLORREF ConvertRGBtoGrey ( COLORREF cr )
{
	BYTE grey = ConvertRGBtoGrey ( GetRValue(cr), GetGValue(cr), GetBValue(cr) );

	return RGB ( grey, grey, grey );
}

/**********************************************************************/

static bool CreateDisabledImage ( CImage& imageSource, CImage& imageDest, COLORREF crTransparent )
{
	int nBPP = imageSource.GetBPP();

	if ( nBPP == 24 || nBPP == 32 )
	{
		int nWidth = imageSource.GetWidth();
		int nHeight = imageSource.GetHeight();

		if ( imageDest.Create ( nWidth, nHeight, nBPP ) == TRUE )
		{
			int nPitch = imageSource.GetPitch();		// nPitch +ve = top down (GetBits() points to start of first line)
												// nPitch -ve = bottom up (GetBits() points to start of last line)
			int nBytesPerPixel = abs(nPitch) / nWidth;

			BYTE* pSource = (BYTE*) imageSource.GetBits();
			BYTE* pDest   = (BYTE*) imageDest.GetBits();
	
			BYTE ucRedTransparent   = GetRValue ( crTransparent );
			BYTE ucGreenTransparent = GetGValue ( crTransparent );
			BYTE ucBlueTransparent  = GetBValue ( crTransparent );

			BYTE* pSourcePtr;
			BYTE* pDestPtr;

			BYTE ucRed, ucGreen, ucBlue;

			for ( int y = 0 ; y < nHeight ; y++ )
			{
				long lOffset = y * nPitch;
				pSourcePtr = pSource + lOffset;
				pDestPtr   = pDest   + lOffset;

				for ( int x = 0 ; x < nWidth ; x++ )
				{
					ucBlue =  *( pSourcePtr );
					ucGreen = *( pSourcePtr+1 );
					ucRed =   *( pSourcePtr+2 );

					if ( ucRed == ucRedTransparent && ucGreen == ucGreenTransparent && ucBlue == ucBlueTransparent )	// dont convert to grey
					{
						*( pDestPtr )   = ucBlue;
						*( pDestPtr+1 ) = ucGreen;
						*( pDestPtr+2 ) = ucRed;
					}
					else
					{
						BYTE ucGrey = ConvertRGBtoGrey ( ucRed, ucGreen, ucBlue );

						*( pDestPtr )   = ucGrey;
						*( pDestPtr+1 ) = ucGrey;
						*( pDestPtr+2 ) = ucGrey;
					}

					pSourcePtr += nBytesPerPixel;
					pDestPtr += nBytesPerPixel;
				}
			}
			return TRUE;
		}
	}
	return FALSE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/*														*/
/*	CSSFullScreenWnd										*/
/*														*/
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSSFullScreenWnd::CSSFullScreenWnd()
{
	m_bCreated = FALSE;
	m_crBackground = 0;
}

/**********************************************************************/

BOOL CSSFullScreenWnd::Open ( const char* szTitle, const char* szImageFilename, COLORREF crBackground )
{
	BOOL bReply = Create ( NULL, szTitle, WS_POPUP );
	if ( bReply == TRUE )
	{
		m_crBackground = crBackground;

		m_image.Load ( szImageFilename );

		ShowWindow ( SW_SHOWMAXIMIZED );

		m_bCreated = TRUE;
	}
	return bReply;
}

/**********************************************************************/

void CSSFullScreenWnd::Close()
{
	if ( m_bCreated == TRUE )
	{
		if ( HBITMAP ( m_image ) != NULL )
			m_image.Destroy();

		CFrameWnd::DestroyWindow();	// calls CSSFullScreenWnd::~CSSFullScreenWnd()
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSSFullScreenWnd, CFrameWnd)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSSFullScreenWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	if ( CFrameWnd::PreCreateWindow(cs) == TRUE )
	{
		cs.dwExStyle &= ~WS_EX_CLIENTEDGE;		// remove border
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

BOOL CSSFullScreenWnd::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect ( &rect );

	if ( HBITMAP ( m_image ) != NULL )
	{
		CRect rectSource ( 0, 0, m_image.GetWidth(), m_image.GetHeight() );

		pDC->SetStretchBltMode ( COLORONCOLOR );
		m_image.StretchBlt ( pDC->m_hDC, rect, rectSource, SRCCOPY );
	}
	else
		pDC->FillSolidRect ( rect, m_crBackground );

	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/*														*/
/*	CSSColourDialog										*/
/*														*/
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSSColourDialog::CSSColourDialog ( UINT nIDTemplate, CWnd* pParent )
	: CDialog ( nIDTemplate, pParent )
{
	m_crTransparent = 0x0;
	m_crBackground = 0xFFFFFF;
}

/**********************************************************************/

CSSColourDialog::CSSColourDialog()
	: CDialog()
{
	m_crTransparent = -1;		// no transparent colour selected
	m_crBackground = 0xFFFFFF;
}

/**********************************************************************/

CSSColourDialog::~CSSColourDialog()
{
	if ( HBITMAP ( m_image ) != NULL )
		m_image.Destroy();
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSSColourDialog, CDialog)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSSColourDialog::OnInitDialog ( COLORREF crBackground, bool bRounded )
{
	CDialog::OnInitDialog();

	m_crBackground = crBackground;

	if ( bRounded == TRUE )
	{
		CRect rect;
		GetWindowRect ( &rect );

		CRgn rgn;
		rgn.CreateRoundRectRgn ( 0, 0, rect.Width(), rect.Height(), 30, 30 );
		SetWindowRgn ( static_cast<HRGN>(rgn.GetSafeHandle()), TRUE );	// set the windows region
		rgn.Detach();		// detach the 'CRgn' object from the region or else the 'CRgn' destructor
						// would close the 'HRGN' handle when 'rgn' goes out of scope
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

/**********************************************************************/

void CSSColourDialog::SetImage ( const char* szImageFilename, COLORREF crTransparent )
{
	if ( _access ( szImageFilename, 0 ) != 0 )
		return;

	m_crTransparent = crTransparent;

	if ( HBITMAP ( m_image ) != NULL )	// already have image
		m_image.Destroy();			// destroy current image

	m_image.Load ( szImageFilename );

	Invalidate();
}

/**********************************************************************/

BOOL CSSColourDialog::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect ( &rect );

	if ( HBITMAP ( m_image ) != NULL )
	{
		CRect rectSource ( 0, 0, m_image.GetWidth(), m_image.GetHeight() );

		if ( m_crTransparent != -1 && m_image.IsTransparencySupported() == TRUE )
			m_image.TransparentBlt ( pDC->m_hDC, rect, rectSource, m_crTransparent );
		else
		{
			pDC->SetStretchBltMode ( COLORONCOLOR );
			m_image.StretchBlt ( pDC->m_hDC, rect, rectSource, SRCCOPY );
		}
	}
	else
	{
		if ( m_crBackground == -1 )		// use default dialog background
		{
			CDialog::OnEraseBkgnd(pDC);
		}
		else
		{
			pDC->FillSolidRect ( rect, m_crBackground );
		}
	}

	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/*														*/
/*	CSSColourButton										*/
/*														*/
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSSColourButton::CSSColourButton()
	: CButton()
{
	m_pWndParent = NULL;
	m_crTransparent = -1;		// no transparent colour selected
	m_bInvalidateParent = FALSE;
	m_crText = 0x0;
	m_crBackground = 0xFFFFFF;
}

/**********************************************************************/

CSSColourButton::~CSSColourButton()
{
	if ( HFONT ( m_font ) != NULL )
		m_font.DeleteObject();

	if ( HBITMAP ( m_image ) != NULL )
		m_image.Destroy();

	if ( HBITMAP ( m_imageDisabled ) != NULL )
		m_imageDisabled.Destroy();
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSSColourButton, CButton)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/**********************************************************************/

void CSSColourButton::Init ( UINT nID, CWnd* pParent, COLORREF crText, COLORREF crBackground, const char* szFontName, int nFontSize )
{
	if ( SubclassDlgItem ( nID, pParent ) != 0 )
	{
		m_pWndParent = pParent;
		CString strFontName = szFontName;
		if ( strFontName == "" )
			strFontName = szDEFAULT_FONT;

		CRect rect;
		GetClientRect ( rect );

		int nPointSize = ( nFontSize != -1 ) ? nFontSize*10 : rect.Height()*5;

		m_font.CreatePointFont ( nPointSize, strFontName );	// point size

		SetButtonStyle ( GetButtonStyle() | BS_OWNERDRAW );	// set owner draw style
		m_crBackground = crBackground;
		m_crText = crText;
	}
}

/**********************************************************************/

void CSSColourButton::SetImage ( const char* szImageFilename, COLORREF crTransparent )
{
	if ( _access ( szImageFilename, 0 ) != 0 )
		return;

	m_crTransparent = crTransparent;

	if ( HBITMAP ( m_image ) != NULL )			// already have image
		m_image.Destroy();					// destroy current image

	if ( HBITMAP  ( m_imageDisabled ) != NULL )	// already have image
		m_imageDisabled.Destroy();			// destroy current image

	m_image.Load ( szImageFilename );

	Invalidate();
}

/**********************************************************************/

COLORREF CSSColourButton::SetTextColour ( COLORREF crText )
{
	COLORREF crCurrent = m_crText;
	m_crText = crText;
	Invalidate();
	return crCurrent;
}

/**********************************************************************/

COLORREF CSSColourButton::SetBackgroundColour ( COLORREF crBackground )
{
	COLORREF crCurrent = m_crBackground;
	m_crBackground = crBackground;
	Invalidate();
	return crCurrent;
}

/**********************************************************************/

BOOL CSSColourButton::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;		// don't erase background
}

/**********************************************************************/

static void DrawFrame ( CDC& dc, CRect& rect, int nDepth, bool bPressed )
{
	CPen penHigh;
	penHigh.CreatePen ( PS_SOLID, 1, RGB(224,224,224) );
	CPen penLow;
	penLow.CreatePen ( PS_SOLID, 1, RGB(96,96,96) );

	for ( int n = 0 ; n < nDepth ; n++ )
	{
		CPen* pOldPen = dc.SelectObject ( (bPressed==FALSE)? &penHigh : &penLow );
		dc.MoveTo ( rect.left, rect.bottom );
		dc.LineTo ( rect.left, rect.top );
		dc.LineTo ( rect.right, rect.top );
		dc.SelectObject ( (bPressed==FALSE)? &penLow : &penHigh );
		dc.LineTo ( rect.right, rect.bottom );
		dc.LineTo ( rect.left, rect.bottom );
		dc.SelectObject ( pOldPen );
		rect.DeflateRect(1,1);
	}
}

/**********************************************************************/

void CSSColourButton::DrawItem ( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	CDC dc;
	dc.Attach ( lpDrawItemStruct->hDC );	// get device context object

	bool bPressed = ( (lpDrawItemStruct->itemState & ODS_SELECTED) != 0 ) ? TRUE : FALSE;
	bool bDisabled =  ( (lpDrawItemStruct->itemState & ODS_DISABLED) != 0 ) ? TRUE : FALSE;

	CRect rectButton ( lpDrawItemStruct->rcItem );

	int nDepth = 3;
	if ( HBITMAP ( m_image ) != NULL )
	{
		CImage* pImage = &m_image;	// default to standard image

		if ( bDisabled == TRUE )		// need a disabled image
		{
			if ( HBITMAP ( m_imageDisabled ) == NULL )	// disabled image has not been created yet
			{
				if ( CreateDisabledImage ( m_image, m_imageDisabled, m_crTransparent ) == TRUE )	// create disabled image (only needs tobe done once)
					pImage = &m_imageDisabled;
			}
			else
				pImage = &m_imageDisabled;	// disabled image has already been created
		}

		CRect rectSource;
		if ( bPressed == TRUE )		// pressed: bottom right not displayed
		{
			rectSource.SetRect ( 0, 0, pImage->GetWidth()-nDepth-1, pImage->GetHeight()-nDepth-1 );
		}
		else						// not pressed: top left not displayed
		{
			rectSource.SetRect ( nDepth, nDepth, pImage->GetWidth()-1, pImage->GetHeight()-1 );
		}

		if ( m_crTransparent != -1 && pImage->IsTransparencySupported() == TRUE )
		{
			pImage->TransparentBlt ( dc.m_hDC, rectButton, rectSource, m_crTransparent );

			if ( bPressed == TRUE )
				m_bInvalidateParent = TRUE;	// need to invalidate parent when button released
		}
		else
		{
			dc.SetStretchBltMode ( COLORONCOLOR );
			pImage->StretchBlt ( dc.m_hDC, rectButton, rectSource, SRCCOPY );	// no need to invalidate if transparency not being used
		}

		if ( m_bInvalidateParent == TRUE && bPressed == FALSE )
		{
			m_bInvalidateParent = FALSE;
			InvalidateParentRect();		// invalidate the part of the parent window covered by this button
		}
	}
	else
	{
		COLORREF cr = ( bDisabled == FALSE ) ? m_crBackground : RGB(128,128,128);
		dc.FillSolidRect ( rectButton, cr );
		DrawFrame ( dc, rectButton, nDepth, bPressed );
	}

	if ( HFONT ( m_font ) != NULL )
	{
		COLORREF crText = ( bDisabled == FALSE ) ? m_crText : ConvertRGBtoGrey ( m_crText );

		CFont* pOldFont = dc.SelectObject ( &m_font );
		int nOldBkMode = dc.SetBkMode ( TRANSPARENT );
		COLORREF crOldColour = dc.SetTextColor ( crText );

		CRect rectText = rectButton;
		if ( bPressed == TRUE )
			rectText.OffsetRect ( nDepth-1, nDepth-1 );	// text needs to move when button pressed

		CString strText;
		GetWindowText ( strText );	// get the button text
		dc.DrawText ( strText, strText.GetLength(), rectText, DT_SINGLELINE | DT_VCENTER | DT_CENTER );

		dc.SetTextColor ( crOldColour );
		dc.SetBkMode ( nOldBkMode );
		dc.SelectObject ( pOldFont );
	}

	dc.Detach();
}

/**********************************************************************/

void CSSColourButton::InvalidateParentRect()
{
	if ( m_pWndParent != NULL )
	{
		CRect rectParent;
		m_pWndParent->GetWindowRect ( rectParent );

		CRect rectStatic;
		GetWindowRect ( rectStatic );

		int x = rectStatic.left-rectParent.left;
		int y = rectStatic.top-rectParent.top;

		CRect rectClient;
		GetClientRect( rectClient );
		rectClient.OffsetRect ( x, y );
		m_pWndParent->InvalidateRect ( rectClient );		// invalidate the part of the parent window covered by this button
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/*														*/
/*	CSSColourStatic										*/
/*														*/
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSSColourStatic::CSSColourStatic()
{
	m_pWndParent = NULL;
	m_crTransparent = -1;		// no transparent colour selected
	m_bInvalidateParent = FALSE;
	m_nAlign = DT_CENTER;
	m_crText = 0x0;
	m_crBackground = 0xFFFFFF;
}

/**********************************************************************/

CSSColourStatic::~CSSColourStatic()
{
	if ( HFONT ( m_font ) != NULL )
		m_font.DeleteObject();

	if ( HBITMAP ( m_image ) != NULL )
		m_image.Destroy();
}

/**********************************************************************/

void CSSColourStatic::PreSubclassWindow()
{
	switch ( GetStyle() & 0x03 )
	{
	case 1:	m_nAlign = DT_CENTER;	break;
	case 2:	m_nAlign = DT_RIGHT;	break;
	default:	m_nAlign = DT_LEFT;		break;
	}
/*
	SS_LEFT		0x00000000L		00000
	SS_CENTER		0x00000001L		00001
	SS_RIGHT		0x00000002L		00010
	SS_OWNERDRAW	0x0000000DL		01101
	SS_TYPEMASK	0x0000001FL		11111

	SS_LEFT | SS_OWNERDRAW		= SS_OWNERDRAW
	SS_CENTER | SS_OWNERDRAW		= SS_OWNERDRAW
	SS_RIGHT | SS_OWNERDRAW		= SS_ENHMETAFILE !!! (setting SS_RIGHT corrupts SS_OWNERDRAW so DrawItem never gets called)
*/
	ModifyStyle ( SS_TYPEMASK, SS_OWNERDRAW );		// remove SS_TYPEMASK styles, add SS_OWNERDRAW style

	CStatic::PreSubclassWindow();
}

/**********************************************************************/

void CSSColourStatic::Init ( UINT nID, CWnd* pParent, COLORREF crText, COLORREF crBackground, const char* szFontName, int nFontSize )
{
	if ( SubclassDlgItem ( nID, pParent ) != 0 )
	{
		m_pWndParent = pParent;
		m_crText = crText;
		m_crBackground = crBackground;

		CString strFontName = szFontName;
		if ( strFontName == "" )
			strFontName = szDEFAULT_FONT;

		CRect rect;
		GetClientRect ( rect );

		int nPointSize = ( nFontSize != -1 ) ? nFontSize*10 : rect.Height()*6;

		m_font.CreatePointFont ( nPointSize, strFontName );		// point size
	}
}

/**********************************************************************/

void CSSColourStatic::SetImage ( const char* szImageFilename, COLORREF crTransparent )
{
	if ( _access ( szImageFilename, 0 ) != 0 )
		return;

	m_crTransparent = crTransparent;

	if ( HBITMAP ( m_image ) != NULL )	// already have image
		m_image.Destroy();			// destroy current image

	m_image.Load ( szImageFilename );

	Invalidate();
}

/**********************************************************************/

COLORREF CSSColourStatic::SetTextColour ( COLORREF crText )
{
	COLORREF crCurrent = m_crText;
	m_crText = crText;
	Invalidate();
	return crCurrent;
}

/**********************************************************************/

COLORREF CSSColourStatic::SetBackgroundColour ( COLORREF crBackground )
{
	COLORREF crCurrent = m_crBackground;
	m_crBackground = crBackground;
	Invalidate();
	return crCurrent;
}

/**********************************************************************/

void CSSColourStatic::DrawItem ( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	CDC dc;
	dc.Attach ( lpDrawItemStruct->hDC );	// get device context object

	CRect rectStatic ( lpDrawItemStruct->rcItem );

	if ( HBITMAP ( m_image ) != NULL )
	{
		CRect rectSource ( 0, 0, m_image.GetWidth(), m_image.GetHeight() );

		if ( m_crTransparent != -1 && m_image.IsTransparencySupported() == TRUE )
		{
			m_image.TransparentBlt ( dc.m_hDC, rectStatic, rectSource, m_crTransparent );
		}
		else
		{
			dc.SetStretchBltMode ( COLORONCOLOR );
			m_image.StretchBlt ( dc.m_hDC, rectStatic, rectSource, SRCCOPY );
		}
	}
	else
	{
		if ( m_crBackground != -1 )				// solid background
		{
			dc.FillSolidRect ( rectStatic, m_crBackground );
		}
		else									// transparent background
		{
			m_bInvalidateParent = ( m_bInvalidateParent == FALSE ) ? TRUE : FALSE;	// need to invalidate parent
		}
	}

	if ( m_bInvalidateParent == TRUE )
	{
		InvalidateParentRect();			// invalidate the part of the parent window covered by this static
	}
	else
	{
		if ( HFONT ( m_font ) != NULL )
		{
			CFont* pOldFont = dc.SelectObject ( &m_font );
			COLORREF crOldColour = dc.SetTextColor ( m_crText );
			int nOldBkMode = dc.SetBkMode ( TRANSPARENT );

			CString strText;
			GetWindowText ( strText );	// get the static text
			UINT uiFormat = DT_SINGLELINE | DT_VCENTER | m_nAlign;
			dc.DrawText ( strText, strText.GetLength(), rectStatic, uiFormat );

			dc.SetBkMode ( nOldBkMode );
			dc.SetTextColor ( crOldColour );
			dc.SelectObject ( pOldFont );
		}
	}

	dc.Detach();
}

/**********************************************************************/

void CSSColourStatic::InvalidateParentRect()
{
	if ( m_pWndParent != NULL )
	{
		CRect rectParent;
		m_pWndParent->GetWindowRect ( rectParent );

		CRect rectStatic;
		GetWindowRect ( rectStatic );

		int x = rectStatic.left-rectParent.left;
		int y = rectStatic.top-rectParent.top;

		CRect rectClient;
		GetClientRect( rectClient );
		rectClient.OffsetRect ( x, y );
		m_pWndParent->InvalidateRect ( rectClient );		// invalidate the part of the parent window covered by this static
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/*														*/
/*	CSSColourGroupBox										*/
/*														*/
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSSColourGroupBox::CSSColourGroupBox()
	: CButton()
{
	m_crText = 0x0;
	m_crLine = 0x0;
}

/**********************************************************************/

CSSColourGroupBox::~CSSColourGroupBox()
{
	if ( HFONT ( m_font ) != NULL )
		m_font.DeleteObject();
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSSColourGroupBox, CButton)
	ON_WM_PAINT()
END_MESSAGE_MAP()

/**********************************************************************/

void CSSColourGroupBox::Init ( UINT nID, CWnd* pParent, COLORREF crText, COLORREF crLine, const char* szFontName, int nFontSize )
{
	if ( SubclassDlgItem ( nID, pParent ) != 0 )
	{
		m_crText = crText;
		m_crLine = crLine;

		CString strFontName = szFontName;
		if ( strFontName == "" )
			strFontName = szDEFAULT_FONT;

		m_font.CreatePointFont ( nFontSize*10, strFontName );		// point size
	}
}

/**********************************************************************/

COLORREF CSSColourGroupBox::SetTextColour ( COLORREF crText )
{
	COLORREF crCurrent = m_crText;
	m_crText = crText;
	Invalidate();
	return crCurrent;
}

/**********************************************************************/

COLORREF CSSColourGroupBox::SetLineColour ( COLORREF crLine )
{
	COLORREF crCurrent = m_crLine;
	m_crLine = crLine;
	Invalidate();
	return crCurrent;
}

/**********************************************************************/

void CSSColourGroupBox::OnPaint()
{
	CPaintDC dc ( this );	// device context for painting

	CString strText;
	GetWindowText ( strText );

	CRect rect;
	GetClientRect ( rect );

	CPen pen;
	pen.CreatePen ( PS_SOLID, 1, m_crLine );
	CPen* pOldPen = dc.SelectObject ( &pen );

	if ( strText != "" )
	{
		if ( HFONT ( m_font ) != NULL )
		{
			CFont* pOldFont = dc.SelectObject ( &m_font );
			CSize sizeText = dc.GetTextExtent ( strText );

			dc.MoveTo ( rect.left, rect.top );
			dc.LineTo ( rect.left+10, rect.top );
			dc.MoveTo ( rect.left+sizeText.cx+10+2+2, rect.top );	// leave gap for text
			dc.LineTo ( rect.right, rect.top );
			dc.LineTo ( rect.right, rect.bottom );
			dc.LineTo ( rect.left, rect.bottom );
			dc.LineTo ( rect.left, rect.top );

			int nOldBkMode = dc.SetBkMode ( TRANSPARENT );
			COLORREF crOldColour = dc.SetTextColor ( m_crText );
			dc.TextOut ( rect.left+10+2, rect.top-(sizeText.cy/2), strText );
			dc.SetTextColor ( crOldColour );
			dc.SetBkMode ( nOldBkMode );
			dc.SelectObject ( pOldFont );
		}
	}
	else		// no text (full rectangle)
	{
		dc.MoveTo ( rect.left, rect.top );
		dc.LineTo ( rect.right, rect.top );
		dc.LineTo ( rect.right, rect.bottom );
		dc.LineTo ( rect.left, rect.bottom );
		dc.LineTo ( rect.left, rect.top );
	}

	dc.SelectObject ( pOldPen );
}

/**********************************************************************/
