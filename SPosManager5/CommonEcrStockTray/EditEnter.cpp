/**********************************************************************/
 
/**********************************************************************/
#include "EditEnter.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CEditEnter, CEdit)
/**********************************************************************/

CEditEnter::CEditEnter()
{
	m_pMessageTarget = NULL;
	m_nMessageNo = 0;
}

/**********************************************************************/

CEditEnter::~CEditEnter()
{
}

/**********************************************************************/

void CEditEnter::SetMessageTarget( CWnd* pMessageTarget, int nMessageNo )
{
	m_pMessageTarget = pMessageTarget;
	m_nMessageNo = nMessageNo;
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CEditEnter, CEdit)
	ON_MESSAGE( WM_GETDLGCODE, OnGetDlgCode )
	ON_WM_CHAR()
END_MESSAGE_MAP()

/**********************************************************************/

afx_msg long CEditEnter::OnGetDlgCode ( WPARAM wIndex, LPARAM lParam )
{
	LPMSG lpmsg = (LPMSG)lParam;

    if ( lpmsg != NULL )
	{
		if ( ( WM_KEYDOWN == lpmsg -> message ) && ( VK_RETURN == lpmsg -> wParam ) )
          return CEdit::OnGetDlgCode() | DLGC_WANTALLKEYS;
	}
   
	return CEdit::OnGetDlgCode();
}

/**********************************************************************/

void CEditEnter::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ( nChar == VK_RETURN )
	{
		if ( m_pMessageTarget != NULL )
		{
			m_pMessageTarget -> SendMessage( m_nMessageNo, 0, 0 );
			return;
		}
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

/**********************************************************************/

