/**********************************************************************/
#include "FontTable.h"
//#include "globalfunctions.h"
/**********************************************************************/
#include "ColourButton.h"
/**********************************************************************/

CColourButtonBase::CColourButtonBase() : CButton()
{
	m_ColourRefBack = COLORREF( 0xFF );
	m_nButtonNo = 0;
	m_bBlackBorder = FALSE;
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CColourButtonBase, CButton)
	//{{AFX_MSG_MAP(CColourButtonBase)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CColourButtonBase::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{	
	HandleDraw( lpDrawItemStruct );
}

/**********************************************************************/

void CColourButtonBase::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd* pParent = GetParent();
	
	if ( pParent != NULL )
		pParent -> PostMessage( WM_APP, m_nButtonNo, 0 );

	CButton::OnRButtonDown(nFlags, point);
}

/**********************************************************************/

void CColourButtonBase::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CWnd* pParent = GetParent();
	
	if ( pParent != NULL )
		pParent -> PostMessage( WM_APP + 1, m_nButtonNo, 0 );
	
	CButton::OnLButtonDblClk(nFlags, point);
}

/**********************************************************************/

void CColourButtonBase::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CWnd* pParent = GetParent();
	
	if ( pParent != NULL )
		pParent -> PostMessage( WM_APP, m_nButtonNo, 1 );

	CButton::OnRButtonDown(nFlags, point);
}

/**********************************************************************/

void CColourButtonBase::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	CWnd* pParent = GetParent();
	
	if ( pParent != NULL )
		pParent -> PostMessage( WM_APP + 1, m_nButtonNo, 1 );

	CButton::OnRButtonDblClk(nFlags, point);
}

/**********************************************************************/

void CColourButtonBase::HandleDraw(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{	
	CDC dc; 
	dc.Attach(lpDrawItemStruct->hDC);     //Get device context object
   
	CRect rectButton; 
	rectButton = lpDrawItemStruct->rcItem;        //Get button rect

	dc.FillSolidRect(rectButton, m_ColourRefBack );
	
	if ( m_bBlackBorder == TRUE )
	{
		CBrush brush;
		brush.CreateSolidBrush( COLORREF(0) );
		dc.FrameRect( rectButton, &brush );
	}

	dc.Detach();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CColourButtonText::CColourButtonText() : CColourButtonBase()
{
	m_ColourRefText = COLORREF( 0x0 );
	m_nFontSize = 8;
	m_strText = "";
	m_nCurrentImage = 1;
}

/**********************************************************************/

CColourButtonText::~CColourButtonText()
{
	m_BitmapInfo1.DeleteOwnDC();
	m_BitmapInfo2.DeleteOwnDC();
}

/**********************************************************************/

void CColourButtonText::SwapImage()
{
	if ( 1 == m_nCurrentImage )
		m_nCurrentImage = 2;
	else
		m_nCurrentImage = 1;
}

/**********************************************************************/

CBitmapInfo* CColourButtonText::GetBitmapInfo()
{
	return ( 2 == m_nCurrentImage ) ? &m_BitmapInfo2 : &m_BitmapInfo1;
}

/**********************************************************************/

bool CColourButtonText::LoadImage( const char* szFilename )
{
	bool bResult = TRUE;
	CString strFilename = szFilename;

	CBitmapInfo* pBitmapInfo = GetBitmapInfo();
	
	if ( strFilename == "" )
	{
		pBitmapInfo -> DeleteOwnDC();
		bResult = TRUE;
	}
	else
	{
		pBitmapInfo -> CreateOwnDC( GetDC() );
		pBitmapInfo -> LoadImage( strFilename, TRUE );
		
		if ( ( pBitmapInfo -> GetHeight() > 350 ) || ( pBitmapInfo -> GetWidth() > 350 ) )
		{
			pBitmapInfo -> DeleteOwnDC();
			bResult = FALSE;
		}
	}		
	return bResult;
}

/**********************************************************************/

void CColourButtonText::HandleDraw(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{	
	CDC dc; 
	dc.Attach(lpDrawItemStruct->hDC);			  //Get device context object

	CRect rectButton; 
	rectButton = lpDrawItemStruct->rcItem;        //Get button rect

	dc.FillSolidRect(rectButton, m_ColourRefBack );
	
	if ( m_bBlackBorder == TRUE )
	{
		CBrush brush;
		brush.CreateSolidBrush( COLORREF(0) );
		dc.FrameRect( rectButton, &brush );
	}

	CBitmapInfo* pBitmapInfo = GetBitmapInfo();

	if ( pBitmapInfo -> GotImage() == FALSE )
	{
		dc.SetTextColor( m_ColourRefText ); 
		
		CFont* pOldFont = dc.SelectObject ( FontTable.GetDialogFont ( m_nFontSize, FALSE ) );

		TEXTMETRIC tm;
		dc.GetTextMetrics ( &tm );

		int nYStart = rectButton.top + ( ( rectButton.Height() - tm.tmHeight ) / 2 );

		int nOldAlign = dc.SetTextAlign( TA_CENTER );
		int nXPos = rectButton.left + ( rectButton.Width() / 2 );
			
		dc.ExtTextOut ( nXPos, nYStart, ETO_CLIPPED, rectButton, m_strText, m_strText.GetLength(), NULL );
		dc.SetTextAlign ( nOldAlign );
		dc.SelectObject ( pOldFont );
	}
	else 
	{
		if ( TRUE == m_bTransparent )
		{
			dc.TransparentBlt(
				rectButton.left + 10,
				rectButton.top + 10,
				rectButton.Width() - 20,
				rectButton.Height() - 50,
				pBitmapInfo -> GetDCImage(), 0, 0,
				pBitmapInfo -> GetWidth(),
				pBitmapInfo -> GetHeight(),
				0xFFFFFF );
		}
		else
		{
			dc.StretchBlt(
				rectButton.left + 10,
				rectButton.top + 10,
				rectButton.Width() - 20,
				rectButton.Height() - 50,
				pBitmapInfo -> GetDCImage(), 0, 0,
				pBitmapInfo -> GetWidth(),
				pBitmapInfo -> GetHeight(),
				SRCCOPY );
		}

		dc.SetTextColor( m_ColourRefText ); 
		
		CFont* pOldFont = dc.SelectObject ( FontTable.GetDialogFont ( m_nFontSize, FALSE ) );

		TEXTMETRIC tm;
		dc.GetTextMetrics ( &tm );

		CRect rectText = rectButton;
		rectText.top = rectText.bottom - 50;

		int nYStart = rectText.top + ( ( rectText.Height() - tm.tmHeight ) / 2 );

		int nOldAlign = dc.SetTextAlign( TA_CENTER );
		int nXPos = rectText.left + ( rectText.Width() / 2 );
			
		dc.ExtTextOut ( nXPos, nYStart, ETO_CLIPPED, rectButton, m_strText, m_strText.GetLength(), NULL );
		dc.SetTextAlign ( nOldAlign );
		dc.SelectObject ( pOldFont );
	}
	
	dc.Detach();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CColourButtonPMS::CColourButtonPMS() : CColourButtonBase()
{
	m_ColourRefText = COLORREF( 0x0 );
	m_nFontSize = 8;
	m_ColourRefBottomLineBack = COLORREF( 0xFF );
	m_ColourRefBottomLineText = COLORREF( 0x0 );
	m_strLine1 = "";
	m_strLine2 = "";
	m_strLine3 = "";
	m_strLine4 = "";
	m_strLine5 = "";
	m_strLine6 = "";
	m_strLine7 = "";
}

/**********************************************************************/

void CColourButtonPMS::HandleDraw(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{	
	CDC dc; 
	dc.Attach(lpDrawItemStruct->hDC);     //Get device context object
   
	CRect rectButton; 
	rectButton = lpDrawItemStruct->rcItem;        //Get button rect

	dc.FillSolidRect(rectButton, m_ColourRefBack );
	
	if ( m_bBlackBorder == TRUE )
	{
		CBrush brush;
		brush.CreateSolidBrush( COLORREF(0) );
		dc.FrameRect( rectButton, &brush );
	}

	dc.SetTextColor( m_ColourRefText ); 
	
	CFont* pOldFont = dc.SelectObject ( FontTable.GetDialogFont ( 8, FALSE ) );

	CRect rectText = rectButton;
	
	rectText.top = 8;
	rectText.bottom = 25;
	dc.DrawText( m_strLine1,rectText,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX); 

	rectText.top = 26;
	rectText.bottom = 43;
	dc.DrawText( m_strLine2,rectText,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX); 

	rectText.top = 44;
	rectText.bottom = 61;
	dc.DrawText( m_strLine3,rectText,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX); 

	rectText.top = 62;
	rectText.bottom = 79;
	dc.DrawText( m_strLine4,rectText,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);

	rectText.top = 80;
	rectText.bottom = 97;
	dc.DrawText( m_strLine5,rectText,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);

	rectText.top = 98;
	rectText.bottom = 115;
	dc.DrawText( m_strLine6,rectText,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);

	if ( m_strLine7 != "" )
	{
		rectText.bottom = rectButton.bottom;
		rectText.top = rectText.bottom - 17;

		dc.FillSolidRect(rectText, m_ColourRefBottomLineBack );
		dc.SetTextColor( m_ColourRefBottomLineText );
		dc.DrawText( m_strLine7,rectText,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX); 
	}

	dc.SelectObject( pOldFont );
  
	dc.Detach();
}

/**********************************************************************/

