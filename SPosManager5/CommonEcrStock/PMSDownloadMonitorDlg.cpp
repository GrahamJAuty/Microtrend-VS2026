/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "FileRemover.h"
#include "PMSRoomStatusTable.h"
/**********************************************************************/
#include "PMSDownloadMonitorDlg.h"
/**********************************************************************/
static const int RETRY_TIMER = 1;
/**********************************************************************/
static const int PMS_SYNC_DELETE_FILE = 1;
static const int PMS_SYNC_CREATE_FILE = 2;
static const int PMS_SYNC_DOWNLOAD_FILE = 3;
static const int PMS_SYNC_FINISHED = 4;
/**********************************************************************/

CPMSDownloadMonitorDlg::CPMSDownloadMonitorDlg( CWnd* pParent)
		: CDialog(CPMSDownloadMonitorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPMSDownloadMonitorDlg)
	//}}AFX_DATA_INIT
	m_nRoomIdx = 0;
	m_nAccountIdx = 0;
	m_nTotalFiles = DataManagerNonDb.PMSRoom.GetSize() * MAX_PMS_ACCOUNTS_PER_ROOM;
	m_nFilesProcessed = 0;
	m_nNextAction = PMS_SYNC_DELETE_FILE;
	m_nLastAction = PMS_SYNC_DELETE_FILE;
}

/**********************************************************************/

CPMSDownloadMonitorDlg::~CPMSDownloadMonitorDlg()
{
}

/**********************************************************************/

void CPMSDownloadMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPMSDownloadMonitorDlg)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_STATIC_FILECOUNT, m_staticFileCount );
	DDX_Control( pDX, IDC_STATIC_INFO1, m_staticInfo1 );
	DDX_Control( pDX, IDC_STATIC_INFO2, m_staticInfo2 );
	DDX_Control( pDX, IDOK, m_buttonOK );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSDownloadMonitorDlg, CDialog)
	//{{AFX_MSG_MAP(CPMSDownloadMonitorDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPMSDownloadMonitorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_staticInfo2.SetWindowText( "" );
	m_buttonOK.SetWindowText( "Stop" );

	m_hTimer = SetTimer( RETRY_TIMER, 200, NULL );

	if ( m_hTimer == NULL )
		EndDialog( IDCANCEL );

	UpdateFileCount();
	
	return TRUE;
}

/**********************************************************************/

void CPMSDownloadMonitorDlg::UpdateFileCount()
{
	if ( PMS_SYNC_FINISHED == m_nNextAction )
	{
		m_staticInfo1.SetWindowText( "" );
		m_staticFileCount.SetWindowText( "" );
		m_staticInfo2.SetWindowText( "The room status files have been updated." );
		m_buttonOK.SetWindowText( "Close" );
	}
	else
	{
		CString str;
		str.Format( "%d of %d",
			m_nFilesProcessed + 1,
			m_nTotalFiles );

		m_staticFileCount.SetWindowText( str );
	
		switch( m_nNextAction )
		{
		case PMS_SYNC_DELETE_FILE:
			m_staticInfo2.SetWindowText( "Removing room file from pending folder" );
			break;

		case PMS_SYNC_CREATE_FILE:
			m_staticInfo2.SetWindowText( "Creating room file in pending folder" );
			break;

		case PMS_SYNC_DOWNLOAD_FILE:
			m_staticInfo2.SetWindowText( "Copying room file to EPOS folder" );
			break;
		}
	}
}

/**********************************************************************/

void CPMSDownloadMonitorDlg::OnTimer(UINT nIDEvent) 
{
	CString strFilename = PMSRoomStatusTable.GetDownloadFilename( m_nRoomIdx, m_nAccountIdx );
		
	switch( m_nNextAction )
	{
	case PMS_SYNC_DELETE_FILE:
		{
			CFileRemover FileRemover ( strFilename );
			if ( ::FileExists( strFilename ) == FALSE )
				m_nNextAction = PMS_SYNC_CREATE_FILE;
		}
		break;

	case PMS_SYNC_CREATE_FILE:
		{	
			PMSRoomStatusTable.WriteDownloadFileAccount( m_nRoomIdx, m_nAccountIdx ); 

			if ( ::FileExists( strFilename ) == TRUE )
				m_nNextAction = PMS_SYNC_DOWNLOAD_FILE;
		}
		break;

	case PMS_SYNC_DOWNLOAD_FILE:
		{
			if ( ::FileExists( strFilename ) == FALSE )
			{
				m_nAccountIdx++;

				if ( m_nAccountIdx >= MAX_PMS_ACCOUNTS_PER_ROOM )
				{
					m_nRoomIdx++;
					m_nAccountIdx = 0;
				}

				if ( m_nRoomIdx >= DataManagerNonDb.PMSRoom.GetSize() )
					m_nNextAction = PMS_SYNC_FINISHED;
				else
					m_nNextAction = PMS_SYNC_DELETE_FILE;

				m_nFilesProcessed++;
			}
		}
		break;
	}

	UpdateFileCount();

	CDialog::OnTimer(nIDEvent);
}

/**********************************************************************/

void CPMSDownloadMonitorDlg::OnOK() 
{
	CancelNow();
}

/**********************************************************************/

void CPMSDownloadMonitorDlg::OnCancel() 
{
	CancelNow();
}

/**********************************************************************/

void CPMSDownloadMonitorDlg::CancelNow()
{	
	if ( m_hTimer != NULL )
		KillTimer( m_hTimer );

	if ( m_nFilesProcessed < m_nTotalFiles )
	{
		int nRemain = m_nTotalFiles - m_nFilesProcessed;
		CString strVerb = ( 1 == nRemain ) ? "has" : "have";

		CString strMsg;
		strMsg.Format( "%d of %d room status files %s not been updated.\n\nAre you sure you want to stop updating now.",
			nRemain,
			m_nTotalFiles,
			(const char*) strVerb );

		if ( Prompter.YesNo( strMsg ) != IDYES )
		{
			m_hTimer = SetTimer( RETRY_TIMER, 1000, NULL );
			return;
		}
	}

	EndDialog( IDCANCEL );
}

/**********************************************************************/
#endif
/**********************************************************************/
