/**********************************************************************/
#include "BackupProgressDlg.h"
/**********************************************************************/

CBackupProgressDlg::CBackupProgressDlg() 
{
    //{{AFX_DATA_INIT(CBackupProgressDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT   
	
	m_nProgressPos1 = 0;
	m_nProgressType1 = -1;
	m_nProgressPart1 = -1;

	m_bRestore = FALSE;
}

/**********************************************************************/

CBackupProgressDlg::~CBackupProgressDlg()
{
	if (m_hWnd != NULL)
	{
		DestroyWindow();
	}
}

/**********************************************************************/

bool CBackupProgressDlg::CreateOnMainWnd(bool bRestore)
{
	m_bRestore = bRestore;
	if (!CDialog::Create(CBackupProgressDlg::IDD, AfxGetMainWnd()))
	{
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

void CBackupProgressDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CBackupProgressDlg)
	DDX_Control(pDX, IDC_STATIC_INFO_1A, m_staticInfo1A);
	DDX_Control(pDX, IDC_STATIC_INFO_1B, m_staticInfo1B);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress1);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CBackupProgressDlg, CDialog)
    //{{AFX_MSG_MAP(CBackupProgressDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
  
void CBackupProgressDlg::SetPos(__int64 nProgress, __int64 nTarget, int nOffset, int nMultiplyBy)
{
	SetPos(m_Progress1, m_nProgressPos1, nProgress, nTarget, nOffset, nMultiplyBy);
}

/**********************************************************************/
  
void CBackupProgressDlg::SetPos( CProgressCtrl& progress, int& nProgressPos, __int64 nProgress, __int64 nTarget, int nOffset, int nMultiplyBy )
{
	RestoreWaitCursor();
	PumpMessages();

	if (nTarget != 0)
	{
		int nPos = int((nProgress * nMultiplyBy) / nTarget) + nOffset;

		if (nPos < 0 || nPos > 100)
		{
			nPos = 0;
		}

		if (nPos != nProgressPos)
		{
			nProgressPos = nPos;
			progress.SetPos(nPos);
		}
	}
}

/**********************************************************************/

void CBackupProgressDlg::SetPos( __int64 nProgress, __int64 nTarget)
{
	SetPos(m_Progress1, m_nProgressPos1, nProgress, nTarget);
}

/**********************************************************************/

void CBackupProgressDlg::SetPos( CProgressCtrl& progress, int& nProgressPos, __int64 nProgress, __int64 nTarget )
{
	RestoreWaitCursor();
	PumpMessage();

	if ( nTarget != 0 )
	{
		int nPos = int ( ( nProgress * 100 ) / nTarget );
		
		if (nPos < 0 || nPos > 100)
		{
			nPos = 0;
		}
		
		if ( nPos != nProgressPos )
		{
			nProgressPos = nPos;
			progress.SetPos(nPos);
		}
	}
}

/**********************************************************************/

void CBackupProgressDlg::SetPos(CSSFile& file)
{
	SetPos(m_Progress1, m_nProgressPos1, file);
}

/**********************************************************************/

void CBackupProgressDlg::SetPos( CProgressCtrl& progress, int& nProgressPos, CSSFile& file )
{
	RestoreWaitCursor();
	PumpMessage();

	int nPos = file.GetPercentPosition();

	if ( nPos != nProgressPos )
	{
		nProgressPos = nPos;
		progress.SetPos(nPos);
	}
}

/**********************************************************************/

void CBackupProgressDlg::PumpMessages()
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
    
BOOL CBackupProgressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
    
	m_Progress1.SetRange( 0, 100);
	m_Progress1.SetPos( 0);

	CString strTitle = "";
	
	if (FALSE == m_bRestore)
	{
		strTitle += "Backup ";
	}
	else
	{
		strTitle += "Restore ";
	}

	strTitle += AfxGetAppName();
	strTitle += " Files";

	SetWindowText( strTitle );
	m_staticInfo1A.SetWindowText ( "" );
	m_staticInfo1B.SetWindowText ( "" );
	
	return TRUE;  
}

/**********************************************************************/

void CBackupProgressDlg::SetText( int nType, __int64 nPart )
{
	SetText( nType, nPart, 0 );
}

/**********************************************************************/

void CBackupProgressDlg::SetText(int nType, __int64 nPart, __int64 nTarget)
{
	SetText(m_staticInfo1A, m_staticInfo1B, m_Progress1, m_nProgressType1, m_nProgressPart1, nType, nPart, nTarget);
}

/**********************************************************************/

void CBackupProgressDlg::SetText( CStatic& staticA, CStatic& staticB, CProgressCtrl& progress, int& nProgressType, __int64& nProgressPart, int nType, __int64 nPart, __int64 nTarget )
{
	if ( nType != nProgressType )
	{
		CString strText = "";
		switch ( nType )
		{
		case BACKUP_PROGRESS_BACKUP:			strText = "Creating backup file";		break;
		case BACKUP_PROGRESS_RESTORE:			strText = "Restoring backup file";		break;
		case BACKUP_PROGRESS_DELETEFILE:		strText = "Removing temporary file";	break;
		case BACKUP_PROGRESS_COUNTFILE:			strText = "Counting files to backup";	break;
		case BACKUP_PROGRESS_BACKUPSOFAR:		strText = "Preparing files to backup";			break;
		case BACKUP_PROGRESS_RESTORESOFAR:		strText = "Extracting file";			break;
		case BACKUP_PROGRESS_FINALISE:			strText = "Creating backup file";		break;
		default:								strText = "";							break;
		}
		staticA.SetWindowText( strText );
		nProgressType = nType;
		nProgressPart = -1;
	}

	if ( nPart != nProgressPart )
	{
		CString strPart = "";
		switch ( nType )
		{
		case BACKUP_PROGRESS_BACKUP:
		case BACKUP_PROGRESS_RESTORE:			
			strPart.Format ( "(part %I64d)", nPart );	
			break;

		case BACKUP_PROGRESS_DELETEFILE:
		case BACKUP_PROGRESS_COUNTFILE:			
			strPart.Format ( "%I64d", nPart );			
			break;

		case BACKUP_PROGRESS_BACKUPSOFAR:			
			strPart.Format ( "%I64d of %I64d", nPart, nTarget );			
			break;

		case BACKUP_PROGRESS_RESTORESOFAR:			
			strPart.Format ( "%I64d of up to %I64d", nPart, nTarget );			
			break;

		case BACKUP_PROGRESS_FINALISE:
			strPart.Format("%I64d %%", nPart);
			break;
		
		default:								
			strPart = "";								
			break;
		}
		staticB.SetWindowText( strPart );
		nProgressPart = nPart;
	}
}

/**********************************************************************/

