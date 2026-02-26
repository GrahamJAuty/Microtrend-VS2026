 /**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "StatusProgress.h"
/**********************************************************************/

CStatusProgress::CStatusProgress()
{
	m_pParentWnd = NULL;
	m_pForegroundWnd = NULL;
	m_pFixedForegroundWnd = NULL;
	m_bForegroundEnabled = FALSE;
	m_bIsUsable = FALSE;
	m_nLockCount = 0;
	m_nPos = -1;
	m_pProgressDlg = NULL;
}

/**********************************************************************/

CStatusProgress::~CStatusProgress()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStatusProgress, CProgressCtrl)
	//{{AFX_MSG_MAP(CStatusProgress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CStatusProgress::SetParent( CStatusBar* pParent )
{
	m_pParentWnd = pParent;

	CRect r;
	int i = m_pParentWnd->CommandToIndex( ID_INDICATOR_PROGRESS );
     
	m_pParentWnd->GetItemRect(i, &r);
	m_pParentWnd->SetPaneText(i, "");

	if(r.IsRectEmpty())
	{ 
		CRect r1;	
		m_pParentWnd->GetWindowRect(&r1); // get parent width
		r.left = r1.right + 1;
		r.top =  r1.top;
		r.right = r1.right + 2;
		r.bottom = r1.bottom;
	} 

	m_bIsUsable = ( CProgressCtrl::Create( WS_VISIBLE | PBS_SMOOTH | WS_CHILD, r, m_pParentWnd, ID_INDICATOR_PROGRESS ) != 0 );
	SetRange (0, 100 );
}

/**********************************************************************/

void CStatusProgress::SetPaneWidth ( int nWidth )
{
	UINT nID, nStyle;
	int cxWidth;
	int i = m_pParentWnd->CommandToIndex( ID_INDICATOR_PROGRESS );
	m_pParentWnd->GetPaneInfo( i, nID, nStyle, cxWidth );
	m_pParentWnd->SetPaneInfo( i, nID, nStyle, nWidth );
}

/**********************************************************************/

void CStatusProgress::Reposition()
{
	if( m_hWnd != NULL)
	{
		UINT id = ::GetWindowLong(m_hWnd, GWL_ID);
		CRect r;

		int i = m_pParentWnd->CommandToIndex(id); 
		m_pParentWnd->GetItemRect(i, &r);
		SetWindowPos(&wndTop, r.left, r.top, r.Width(), r.Height(), 0); 
	}
}

/**********************************************************************/

void CStatusProgress::SetPos( __int64 nProgress, __int64 nTarget )
{
	if ( ( m_bIsUsable == FALSE ) || ( m_nLockCount == 0 ) )
		return;

	if ( m_pProgressDlg != NULL )
		m_pProgressDlg -> SetPos( nProgress, nTarget );
	else
	{
		RestoreWaitCursor();
		PumpMessage();

		if ( nTarget == 0 )
			return;

		int nPos = int ( ( nProgress * 100 ) / nTarget );
		if ( nPos < 0 || nPos > 100 ) nPos = 0;
	
		if ( nPos != m_nPos )
		{
			m_nPos = nPos;
			CProgressCtrl::SetPos(nPos);
		}
	}
}

/**********************************************************************/

void CStatusProgress::SetPos( __int64 nProgress, __int64 nTarget, int nOffset, int nMultiplyBy )
{
	if ( ( m_bIsUsable == FALSE ) || ( m_nLockCount == 0 ) )
		return;

	if ( m_pProgressDlg != NULL )
		m_pProgressDlg -> SetPos( nProgress, nTarget, nOffset, nMultiplyBy );
	else
	{
		RestoreWaitCursor();
		PumpMessage();

		if ( nTarget == 0 )
			return;

		int nPos = int ( ( nProgress * nMultiplyBy ) / nTarget ) + nOffset;
		if ( nPos < 0 || nPos > 100 ) nPos = 0;
	
		if ( nPos != m_nPos )	
		{
			m_nPos = nPos;
			CProgressCtrl::SetPos(nPos);
		}
	}
}

/**********************************************************************/

void CStatusProgress::SetPos( CSSFile& file )
{
	if ( ( m_bIsUsable == FALSE ) || ( m_nLockCount == 0 ) )
		return;

	if ( m_pProgressDlg != NULL )
		m_pProgressDlg -> SetPos( file );
	else
	{
		RestoreWaitCursor();
		PumpMessage();

		int nPos = file.GetPercentPosition();
	
		if ( nPos != m_nPos )
		{
			m_nPos = nPos;
			CProgressCtrl::SetPos(nPos);
		}
	}
}

/**********************************************************************/

void CStatusProgress::SetPos( CSSFile& file, int nPassNo, int nTotalPass )
{
	if ( ( m_bIsUsable == FALSE ) || ( m_nLockCount == 0 ) )
		return;

	RestoreWaitCursor();
	PumpMessage();

	int nPos = 0;
	if ( nTotalPass >= 1 && nPassNo >= 1 )
	{
		int nTotalTarget = nTotalPass * 100;
		int nTotalPos = ( ( nPassNo - 1 ) * 100 ) + file.GetPercentPosition();
		nPos = ( nTotalPos * 100 ) / nTotalTarget;
	}
		
	if ( nPos != m_nPos )
	{
		m_nPos = nPos;

		if ( m_pProgressDlg != NULL )
			m_pProgressDlg -> SetPos( nPos, 100 );
		else
		{
			RestoreWaitCursor();
			PumpMessage();

			CProgressCtrl::SetPos(nPos);
		}
	}
}

/**********************************************************************/

void CStatusProgress::Lock( bool bDialog, const char* szText1 )    
{
	if ( m_bIsUsable == FALSE )
		return;
	
	if ( m_nLockCount == 0 )
	{
#ifndef _DEBUG
		m_bForegroundEnabled = FALSE;

		if ( NULL == m_pFixedForegroundWnd )
			m_pForegroundWnd = GetSafeOwner ( GetForegroundWindow() );
		else
			m_pForegroundWnd = m_pFixedForegroundWnd;

		if ( m_pForegroundWnd != NULL )
		{
			m_bForegroundEnabled = ( m_pForegroundWnd -> IsWindowEnabled() != 0 );
			m_pForegroundWnd -> EnableWindow ( FALSE );
		}
#endif

		BeginWaitCursor();

		if ( bDialog == TRUE )
		{
			if ( m_pProgressDlg == NULL )
			{
				m_pProgressDlg = new CProgressInfoDlg;
				if ( m_pProgressDlg != NULL )
					m_pProgressDlg -> CreateForGeneralUse();
			}
		}
	}

	if ( m_pProgressDlg != NULL )
		m_pProgressDlg -> SetGeneralInfoText( szText1, " " );

	m_nPos = 0;
	CProgressCtrl::SetPos(0);
	m_nLockCount++;

	CStackTextInfo info;
	info.m_strText1 = szText1;
	info.m_strText2 = " ";
	m_stackText.Add( info );
}

/**********************************************************************/

void CStatusProgress::Unlock()
{
	if ( ( m_bIsUsable == FALSE ) || ( m_nLockCount == 0 ) )
		return;

	CString strText1 = " ";
	CString strText2 = " ";

	int nSize = m_stackText.GetSize();

	if ( nSize > 1 )
	{
		strText1 = m_stackText[ nSize - 2 ].m_strText1;
		strText2 = m_stackText[ nSize - 2 ].m_strText2;
	}

	if ( nSize > 0 )
		m_stackText.RemoveAt( nSize - 1 );

	if ( --m_nLockCount == 0 )
	{
#ifndef _DEBUG
		if ( ( m_pForegroundWnd != NULL ) && ( m_bForegroundEnabled == TRUE ) )
			m_pForegroundWnd -> EnableWindow ( TRUE ); 
#endif
		m_nPos = 0;
		CProgressCtrl::SetPos(0);
		EndWaitCursor();
		m_pForegroundWnd = NULL;
		m_bForegroundEnabled = FALSE;

		if ( m_pProgressDlg != NULL )
		{
			delete ( m_pProgressDlg );
			m_pProgressDlg = NULL;
		}
	}
	else
	{
		if ( m_pProgressDlg != NULL )
			m_pProgressDlg -> SetGeneralInfoText ( strText1, strText2 );
	}
}

/**********************************************************************/

void CStatusProgress::SetDialogText( const char* sz2 )
{	
	int nSize = m_stackText.GetSize();
	if ( nSize > 0 )
		m_stackText[nSize - 1].m_strText2 = sz2;

	if ( m_pProgressDlg != NULL )
		m_pProgressDlg -> SetGeneralInfoText( "", sz2 );
}

/**********************************************************************/
