/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "LocationCSVArray.h"
#include "..\SPosStockManager5\StockpointCSVArray.h"
/**********************************************************************/
#include "ProgressInfoDlg.h"
/**********************************************************************/

CProgressInfoDlg::CProgressInfoDlg() 
{
    //{{AFX_DATA_INIT(CProgressInfoDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT   
	m_nMode = PROGRESSINFO_MODE_GENERAL;
	m_nProgressPos = 0;
	m_nProgressType = -1;
	m_nProgressPart = -1;
}

/**********************************************************************/

CProgressInfoDlg::~CProgressInfoDlg()
{
	if(m_hWnd!=NULL) 
		DestroyWindow();
}

/**********************************************************************/

bool CProgressInfoDlg::CreateForGeneralUse()
{
	m_nMode = PROGRESSINFO_MODE_GENERAL;

	if(!CDialog::Create(CProgressInfoDlg::IDD, AfxGetMainWnd() ) )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

void CProgressInfoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CProgressInfoDlg)
	DDX_Control(pDX, IDC_STATIC_INFO2, m_staticInfo2);
    DDX_Control(pDX, IDC_STATIC_INFO, m_staticInfo);
    DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CProgressInfoDlg, CDialog)
    //{{AFX_MSG_MAP(CProgressInfoDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
  
void CProgressInfoDlg::SetPos( __int64 nProgress, __int64 nTarget, int nOffset, int nMultiplyBy )
{
	RestoreWaitCursor();
	PumpMessages();

	if ( nTarget == 0 )
		return;

	int nPos = int ( ( nProgress * nMultiplyBy ) / nTarget ) + nOffset;
	if ( nPos < 0 || nPos > 100 ) nPos = 0;
	
	if ( nPos != m_nProgressPos )
	{
		m_nProgressPos = nPos;
		m_Progress.SetPos(nPos);
	}
}

/**********************************************************************/

void CProgressInfoDlg::SetPos( __int64 nProgress, __int64 nTarget )
{
	RestoreWaitCursor();
	PumpMessage();

	if ( nTarget == 0 )
		return;

	int nPos = int ( ( nProgress * 100 ) / nTarget );
	if ( nPos < 0 || nPos > 100 ) nPos = 0;
	
	if ( nPos != m_nProgressPos )
	{
		m_nProgressPos = nPos;
		m_Progress.SetPos(nPos);
	}
}

/**********************************************************************/

void CProgressInfoDlg::SetPos( CSSFile& file )
{
	RestoreWaitCursor();
	PumpMessage();

	int nPos = file.GetPercentPosition();
	
	if ( nPos != m_nProgressPos )
	{
		m_nProgressPos = nPos;
		m_Progress.SetPos(nPos);
	}
}

/**********************************************************************/

void CProgressInfoDlg::PumpMessages()
{
	// Must call Create() before using the dialog
	ASSERT(m_hWnd!=NULL);

	MSG msg;
	// Handle dialog messages
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if(!IsDialogMessage(&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg); 
		}
	}
}

/**********************************************************************/
    
BOOL CProgressInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
    
	m_Progress.SetRange( 0, 100);
	m_Progress.SetPos( 0);

	CString strTitle = "";

	switch ( m_nMode )
	{
	case PROGRESSINFO_MODE_GENERAL:
		strTitle = "Please Wait";
		break;
	}

	SetWindowText( strTitle );
	m_staticInfo.SetWindowText ( "" );

	return TRUE;  
}

/**********************************************************************/

void CProgressInfoDlg::SetGeneralInfoText( const char* sz1, const char* sz2 )
{
	CString str = sz1;
	if ( str != "" )
		m_staticInfo.SetWindowText( str );
	
	str = sz2;
	if ( str != "" )
	m_staticInfo2.SetWindowText( str );
}

/**********************************************************************/
