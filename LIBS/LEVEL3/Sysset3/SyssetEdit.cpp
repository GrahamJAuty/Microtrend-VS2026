/**********************************************************************/

#include "SyssetEdit.h"

/**********************************************************************/

BEGIN_MESSAGE_MAP(CUserEdit, CEdit)
	ON_WM_CHAR()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

/**********************************************************************/

CUserEdit::CUserEdit()
{
	m_strValidChars = "";		// allow all chars
	m_crBackgroundColour = NULL;
	m_pBrush = NULL;
}

/**********************************************************************/

CUserEdit::~CUserEdit()
{
	if ( m_pBrush != NULL )
		delete m_pBrush;
}

/**********************************************************************/

BOOL CUserEdit::SubclassDlgItem ( UINT nID, CWnd* pParent )
{
	return CEdit::SubclassDlgItem ( nID, pParent );
}

/**********************************************************************/

void CUserEdit::OnChar ( UINT nChar, UINT nRepCnt, UINT nFlags ) 
{
	if ( m_strValidChars != "" )
	{
		if ( nChar >= 0x20 )	// let control chars pass through as normal
		{
			if ( strchr ( m_strValidChars, nChar ) == NULL )	// discard invalid chars
				return;
		}
	}

	CEdit::OnChar ( nChar, nRepCnt, nFlags );
}

/**********************************************************************/

HBRUSH CUserEdit::CtlColor ( CDC* pDC, UINT nCtlColor )
{
	if ( CWnd::IsWindowEnabled() == TRUE )
	{
		if ( m_crBackgroundColour != NULL )
		{
			pDC->SetBkColor ( m_crBackgroundColour );

			return (HBRUSH) m_pBrush->GetSafeHandle();
		}
	}

	return NULL;		// use the parents handler
}

/**********************************************************************/

void CUserEdit::SetBackgroundColour ( COLORREF crColour )
{
	m_crBackgroundColour = crColour;

	if ( crColour != NULL )
	{
		if ( m_pBrush != NULL )
			delete m_pBrush;

		m_pBrush = new CBrush ( crColour );
	}

	Invalidate();			// invalidate client area
	UpdateWindow();		// force repaint now
}

/**********************************************************************/
