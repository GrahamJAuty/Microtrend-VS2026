/**********************************************************************/
#include "SPOSKeyboardColourButton.h"
/**********************************************************************/

CSPOSKeyboardColourButton::CSPOSKeyboardColourButton()
{
	m_ColourRefBack = COLORREF( 0xFF );
	m_ColourRefText = COLORREF( 0x0 );
	m_nButtonNo = 0;
	m_nFontSize = 8;
	m_bActiveTableMode = FALSE;
	m_b86ItemMode = FALSE;
	m_b86Transparent = FALSE;
	m_n86ItemWidth = 0;
	m_n86ItemHeight = 0;
	m_nHAlign = 1;
	m_nVAlign = 1;
	m_bBold = FALSE;
}

/**********************************************************************/

CSPOSKeyboardColourButton::~CSPOSKeyboardColourButton()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardColourButton, CButton)
	//{{AFX_MSG_MAP(CSPOSKeyboardColourButton)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CSPOSKeyboardColourButton::Set86ItemHeight(int n)
{
	if ((n >= 0) && (n <= 99))
	{
		m_n86ItemHeight = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardColourButton::Set86ItemWidth(int n)
{
	if ((n >= 0) && (n <= 99))
	{
		m_n86ItemWidth = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardColourButton::SetVAlign(int n)
{
	if ((n >= 0) && (n <= 2))
	{
		m_nVAlign = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardColourButton::SetHAlign(int n)
{
	if ((n >= 0) && (n <= 2))
	{
		m_nHAlign = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardColourButton::SetFontSize( int n )
{
	if ( CSPOSKeyboardKeyRecord::IsValidFontSize(n) == TRUE )
	{
		m_nFontSize = n;
	}
}

/**********************************************************************/

void CSPOSKeyboardColourButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{	
	int nVAlignFlag = 0;

	switch( m_nVAlign )
	{
	case 0:
		nVAlignFlag = DT_TOP;
		break;

	case 1:
		nVAlignFlag = DT_VCENTER;
		break;

	case 2:
		nVAlignFlag = DT_BOTTOM;
		break;

	default:
		nVAlignFlag = DT_VCENTER;
		break;
	}

	int nHAlignFlag = 0;

	switch( m_nHAlign )
	{
	case 0:		
		nHAlignFlag = DT_LEFT;
		break;

	case 1:
		nHAlignFlag = DT_CENTER;
		break;

	case 2:
		nHAlignFlag = DT_RIGHT;
		break;

	default:
		nHAlignFlag = DT_CENTER;	
		break;
	}

	CDC dc; 
	dc.Attach(lpDrawItemStruct->hDC);     //Get device context object
   
	if ( TRUE == m_b86ItemMode )
	{
		CRect rt; 
		rt = lpDrawItemStruct->rcItem;        //Get button rect

		dc.FillSolidRect(rt, COLORREF( 0xCCCCCC ) );
		
		int nTopGapPercent = 100 - m_n86ItemHeight;
		int nNewTop = ( rt.bottom * nTopGapPercent ) / 100;

		int nLeftGapPercent = 100 - m_n86ItemWidth;
		int nNewLeft = ( rt.right * nLeftGapPercent ) / 100;

		if ( ( nNewTop < rt.bottom ) && ( nNewLeft < rt.right ) )
		{
			rt.top = nNewTop;
			rt.left = nNewLeft;
			
			if (m_b86Transparent == FALSE)
			{
				dc.FillSolidRect(rt, m_ColourRefBack);
			}

			dc.SetTextColor( m_ColourRefText ); 
	
			CString strTemp;
			GetWindowText(strTemp);
			CFont* pOldFont = dc.SelectObject ( SPOSKeyboardManager.GetFontTable() -> GetDialogFont ( m_nFontSize, m_bBold ) );
			dc.DrawText(strTemp,rt,DT_CENTER|nVAlignFlag|DT_SINGLELINE);  
			dc.SelectObject( pOldFont ); 
		}
	}
	else if ( TRUE == m_bActiveTableMode )
	{
		CRect rt; 
		rt = lpDrawItemStruct->rcItem;        //Get button rect

		dc.FillSolidRect(rt, COLORREF( 0xCCCCCC ) );
		rt.bottom /= 2;
		rt.right /= 2;

		dc.FillSolidRect(rt, m_ColourRefBack );	
		dc.SetTextColor( m_ColourRefText ); 
	
		CString strTemp;
		GetWindowText(strTemp);
		CFont* pOldFont = dc.SelectObject ( SPOSKeyboardManager.GetFontTable() -> GetDialogFont ( m_nFontSize, m_bBold ) );
		dc.DrawText(strTemp,rt,nHAlignFlag|nVAlignFlag|DT_SINGLELINE);  
		dc.SelectObject( pOldFont ); 
	}
	else
	{
		CRect rt; 
		rt = lpDrawItemStruct->rcItem;        //Get button rect

		dc.FillSolidRect(rt, m_ColourRefBack ); 
		dc.SetTextColor( m_ColourRefText ); 
	
		CString strTemp;
		GetWindowText(strTemp);
		CFont* pOldFont = dc.SelectObject ( SPOSKeyboardManager.GetFontTable() -> GetDialogFont ( m_nFontSize, m_bBold ) );
		dc.DrawText(strTemp,rt,nHAlignFlag|nVAlignFlag|DT_SINGLELINE);  
		dc.SelectObject( pOldFont );
	}
	
	dc.Detach();
}

/**********************************************************************/

void CSPOSKeyboardColourButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd* pParent = GetParent();

	if (pParent != NULL)
	{
		pParent->PostMessage(WM_APP, m_nButtonNo, 0);
	}

	CButton::OnRButtonDown(nFlags, point);
}

/**********************************************************************/

void CSPOSKeyboardColourButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd* pParent = GetParent();

	if (pParent != NULL)
	{
		pParent->PostMessage(WM_APP + 1, m_nButtonNo, 0);
	}

	CButton::OnLButtonDblClk(nFlags, point);
}

/**********************************************************************/

void CSPOSKeyboardColourButton::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWnd* pParent = GetParent();

	if (pParent != NULL)
	{
		pParent->PostMessage(WM_APP, m_nButtonNo, 1);
	}

	CButton::OnRButtonDown(nFlags, point);
}

/**********************************************************************/

void CSPOSKeyboardColourButton::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	CWnd* pParent = GetParent();
	
	if (pParent != NULL)
	{
		pParent->PostMessage(WM_APP + 1, m_nButtonNo, 1);
	}

	CButton::OnRButtonDblClk(nFlags, point);
}

/**********************************************************************/
