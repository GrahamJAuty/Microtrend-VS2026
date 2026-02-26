#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "ProgressInfoDlg.h"
/**********************************************************************/

struct CStackTextInfo
{
	CString m_strText1;
	CString m_strText2;
};

/**********************************************************************/

class CStatusProgress : public CProgressCtrl
{
public:
	CStatusProgress();
	void SetParent( CStatusBar* pParent );
	void SetPaneWidth( int nWidth );
	void Reposition();
	
	void Lock( bool bDialog = FALSE, const char* szText = "" );
	void SetPos ( __int64 nProgress, __int64 nTarget );
	void SetPos ( __int64 nProgress, __int64 nTarget, int nOffset, int nMultiplyBy );
	void SetPos ( CSSFile& file );
	void SetPos( CSSFile& file, int nPassNo, int nTotalPass );

	void Unlock();

	void SetDialogText( const char* sz2 );

public:
	void SetFixedForegroundWindow( CWnd* pWnd ){ m_pFixedForegroundWnd = pWnd; }
	void ClearFixedForegroundWindow(){ m_pFixedForegroundWnd = NULL; }

public:	
	//{{AFX_VIRTUAL(CStatusProgress)
	//}}AFX_VIRTUAL

public:
	virtual ~CStatusProgress();

protected:
	//{{AFX_MSG(CStatusProgress)
	//}}AFX_MSG

private:
	int m_nLockCount;
	int m_nPos;

private:
	CStatusBar* m_pParentWnd;
	CWnd* m_pFixedForegroundWnd;
	CWnd* m_pForegroundWnd;
	bool m_bForegroundEnabled;
	bool m_bIsUsable;
	
	DECLARE_MESSAGE_MAP()

private:
	CProgressInfoDlg* m_pProgressDlg;

	CArray<CStackTextInfo,CStackTextInfo> m_stackText;
};

/**********************************************************************/
extern CStatusProgress StatusProgress;
/**********************************************************************/
