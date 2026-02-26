/**********************************************************************/
#include "CashRSPClearBufferThreadManager.h"
#include "CommsHandshaker.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "CashRSPClearBufferDlg.h"
/**********************************************************************/

CCashRSPClearBufferDlg::CCashRSPClearBufferDlg( CArray<int,int>& arrayNetworkIdx, CWnd* pParent)
	: CSSDialog(CCashRSPClearBufferDlg::IDD, pParent), m_arrayNetworkIdx( arrayNetworkIdx )
{
	m_hTimer = NULL;
	m_nDialogState = CLEARBUFFERDLG_ACTIVE;
	m_nCurrentSiteIdx = -1;
	m_nCurrentSiteDisplay = -2;
	m_nTransactionCount = 0;
	m_bTimerBusy = FALSE;
	m_bMoreInfoMode = FALSE;

	for ( int n = 0; n < m_arrayNetworkIdx.GetSize(); n++ )
	{
		int nNetworkIdx = m_arrayNetworkIdx.GetAt(n);

		CString strDataPath;
		dbNetwork.GetSafeFileNetworkPath ( nNetworkIdx, strDataPath );

		CFilenameUpdater FnUp( SysFiles::NetworkCommsLock, nNetworkIdx );
		CString strLockFile = FnUp.GetFilenameToUse();

		CString strLocalPath = dbNetwork.GetFolderPathSyssetCashRSP( nNetworkIdx );
		strLocalPath += "\\";

		if ( strDataPath != "" )
		{	
			CCashRSPClearBufferInfo infoSite;
			infoSite.m_nNetworkIdx = nNetworkIdx;
			infoSite.m_bIsTerminal = FALSE;
			infoSite.m_nLocIdx = -1;
			infoSite.m_nTNo = 0;
			infoSite.m_strDataPath = strDataPath;		//FULL PATH TO REAL TIME SALES, INCLUDING UPAUDIT
			infoSite.m_strLocalPath = strLocalPath;		//FULL LOCAL PATH TO WHICH SALES WILL BE COPIED
			infoSite.m_strLockFile = strLockFile;
			m_arraySites.Add( infoSite );
		}

		int nLocIdxStart, nLocIdxEnd;
		dbLocation.GetNetworkLocIdxRange( nNetworkIdx, nLocIdxStart, nLocIdxEnd );

		//CASHRSP FOLDERS BY TERMINAL
		for ( int nLocIdx = nLocIdxStart; nLocIdx <= nLocIdxEnd; nLocIdx++ )
		{
			CArray<CLocationMasterInfo,CLocationMasterInfo> arrayFolders;
			LocationMasterMap.GetLocationMasterList( nLocIdx, arrayFolders, FALSE, TRUE );

			for ( int n = 0; n < arrayFolders.GetSize(); n++ )
			{
				CLocationMasterInfo infoMaster = arrayFolders.GetAt( n );
				CString strCashRSPFolder = infoMaster.GetCashRSPFolder();

				if ( strCashRSPFolder != "" )
				{
					CCashRSPClearBufferInfo infoSite;
					infoSite.m_nNetworkIdx = nNetworkIdx;
					infoSite.m_bIsTerminal = TRUE;
					infoSite.m_nLocIdx = nLocIdx;
					infoSite.m_nTNo = infoMaster.GetTerminalNo();
					infoSite.m_strDataPath = strCashRSPFolder;					//FULL PATH TO REAL TIME SALES, INCLUDING UPAUDIT
					infoSite.m_strLocalPath = strLocalPath;						//FULL LOCAL PATH TO WHICH SALES WILL BE COPIED
					infoSite.m_strLockFile = strLockFile;
					m_arraySites.Add( infoSite );
				}
			}
		}
	}
}

/**********************************************************************/

void CCashRSPClearBufferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCashRSPClearBufferDlg)
	DDX_Control( pDX, IDC_STATIC_CASHRSP, m_staticCashRSP);
	DDX_Control( pDX, IDC_EDIT_CASHRSP1, m_editCashRSP1);
	DDX_Control( pDX, IDC_EDIT_CASHRSP2, m_editCashRSP2);
	DDX_Control( pDX, IDC_STATIC_TRANSACTIONS, m_staticTransactions);
	DDX_Control( pDX, IDC_EDIT_TRANSACTIONS, m_editTransactions);
	DDX_Control( pDX, IDC_STATIC_SOURCES, m_staticSources);
	DDX_Control( pDX, IDC_EDIT_SOURCES, m_editSources);
	DDX_Control( pDX, IDC_STATIC_TIME, m_staticTime);
	DDX_Control( pDX, IDC_EDIT_TIME, m_editTime);
	DDX_Control( pDX, IDC_STATIC_INFO1, m_staticInfo1);
	DDX_Control( pDX, IDC_STATIC_INFO2, m_staticInfo2);
	DDX_Control( pDX, IDC_STATIC_INFO3, m_staticInfo3);
	DDX_Control( pDX, IDC_STATIC_INFO4, m_staticInfo4);
	DDX_Control( pDX, IDC_STATIC_INFO5, m_staticInfo5);
	DDX_Control( pDX, IDC_BUTTON_MOREINFO, m_buttonMoreInfo);
	DDX_Control( pDX, IDOK, m_buttonOK);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CCashRSPClearBufferDlg, CDialog)
	//{{AFX_MSG_MAP(CCashRSPClearBufferDlg)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_MOREINFO, OnButtonMoreInfo)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CCashRSPClearBufferDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

#ifndef STOCKMAN_SYSTEM
	m_staticInfo1.SetWindowText( "Ecr Manager is attempting to process all outstanding SPOS transactions" );
#else
	m_staticInfo1.SetWindowText( "Stock Manager is attempting to process all outstanding SPOS transactions" );
#endif

	ShowAndEnableWindow( &m_staticCashRSP, FALSE );
	ShowAndEnableWindow( &m_editCashRSP1, FALSE );
	ShowAndEnableWindow( &m_editCashRSP2, FALSE );
	MoveControl( &m_staticTransactions, 20, 22 );
	MoveControl( &m_editTransactions, 89, 20 );
	ShowAndEnableWindow( &m_staticSources, FALSE );
	ShowAndEnableWindow( &m_editSources, FALSE );
	MoveControl( &m_staticTime, 20, 40 );
	MoveControl( &m_editTime, 89, 38 );
	ShowAndEnableWindow( &m_staticInfo1, FALSE );
	ShowAndEnableWindow( &m_staticInfo2, FALSE );
	ShowAndEnableWindow( &m_staticInfo3, FALSE );
	ShowAndEnableWindow( &m_staticInfo4, FALSE );
	ShowAndEnableWindow( &m_staticInfo5, FALSE );
	MoveControl( &m_buttonMoreInfo, 7, 76, 64, 14 );
	MoveControl( &m_buttonOK, 230, 62, 50, 28 );
	ResizeDialog( 287, 97 );

	m_dwTickCountStartTimer = GetTickCount();
	m_hTimer = SetTimer( 1, 100, NULL );
	return TRUE;  
}

/**********************************************************************/

void CCashRSPClearBufferDlg::OnButtonMoreInfo()
{
	if ( FALSE == m_bMoreInfoMode )
	{
		ResizeDialog( 427, 238 );
		ShowAndEnableWindow( &m_staticCashRSP, TRUE );
		ShowAndEnableWindow( &m_editCashRSP1, TRUE );
		ShowAndEnableWindow( &m_editCashRSP2, TRUE );
		MoveControl( &m_staticTransactions, 20, 66 );
		MoveControl( &m_editTransactions, 89, 64 );
		ShowAndEnableWindow( &m_staticSources, TRUE );
		ShowAndEnableWindow( &m_editSources, TRUE );
		MoveControl( &m_staticTime, 239, 66 );
		MoveControl( &m_editTime, 285, 64 );
		ShowAndEnableWindow( &m_staticInfo1, TRUE );
		ShowAndEnableWindow( &m_staticInfo2, TRUE );
		ShowAndEnableWindow( &m_staticInfo3, TRUE );
		ShowAndEnableWindow( &m_staticInfo4, TRUE );
		ShowAndEnableWindow( &m_staticInfo5, TRUE );
		ShowAndEnableWindow( &m_buttonMoreInfo, FALSE );
		MoveControl( &m_buttonOK, 370, 203, 50, 28 );	
		m_bMoreInfoMode = TRUE;
	}
}

/**********************************************************************/

void CCashRSPClearBufferDlg::OnTimer(UINT nIDEvent) 
{
	if ( 1 == nIDEvent )
	{
		if ( FALSE == m_bTimerBusy )
		{
			if ( ( FALSE == m_bMoreInfoMode ) || ( m_nDialogState != CLEARBUFFERDLG_WAITCLOSE ) )
			{
				m_bTimerBusy = TRUE;
				HandleTimer();
				m_bTimerBusy = FALSE;
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}

/**********************************************************************/

void CCashRSPClearBufferDlg::HandleTimer() 
{
	if ( m_nCurrentSiteIdx != m_nCurrentSiteDisplay )
	{
		if ( -1 == m_nCurrentSiteIdx )
		{
			m_editCashRSP1.SetWindowText( "Waiting to restart sales collection task" );
			m_editCashRSP2.SetWindowText( "" );
			m_editTransactions.SetWindowText( "0" );
			m_editSources.SetWindowText( "" );
		}
		else if ( m_nCurrentSiteIdx >= m_arraySites.GetSize() )
		{
			if ( FALSE == m_bMoreInfoMode )
			{
				m_editCashRSP1.SetWindowText( "" );
				m_editCashRSP2.SetWindowText( "" );
			}
		}
		else
		{
			CCashRSPClearBufferInfo infoSite = m_arraySites.GetAt( m_nCurrentSiteIdx );

			m_editCashRSP1.SetWindowText( dbNetwork.GetName( infoSite.m_nNetworkIdx ) );

			if ( FALSE == infoSite.m_bIsTerminal )
				m_editCashRSP2.SetWindowText( "Main CashRSP folder" );
			else
			{
				CString str;
				str.Format( "%s, TNo %d", 
					dbLocation.GetName( infoSite.m_nLocIdx ),
					infoSite.m_nTNo );

				m_editCashRSP2.SetWindowText( str );
			}

			{
				CString str;
				str.Format( "%d / %d", m_nCurrentSiteIdx + 1, m_arraySites.GetSize() );
				m_editSources.SetWindowText( str );
			}
		}
		m_nCurrentSiteDisplay = m_nCurrentSiteIdx;
	}

	if ( ( m_nCurrentSiteDisplay >= 0 ) && ( m_nCurrentSiteDisplay < m_arraySites.GetSize() ) )
	{
		CString str;
		str.Format( "%d", m_nTransactionCount + CashRSPClearBufferThreadManager.GetTransactionCount() );
		m_editTransactions.SetWindowText( str );
	}

	{
		int n = ( GetTickCount() - m_dwTickCountStartTimer ) / 1000;
		
		CString str;
		str.Format( "%d", n + 1 );
		m_editTime.SetWindowText( str );
		
	}
	
	bool bAlreadyRunning = FALSE;

	if ( CashRSPClearBufferThreadManager.ThreadIsNull() == FALSE )
	{
		DWORD dwExitCode = CashRSPClearBufferThreadManager.GetThreadExitCode();

		if (STILL_ACTIVE == dwExitCode)
		{
			bAlreadyRunning = TRUE;
		}
		else
		{
			if ( m_nCurrentSiteIdx >= 0 )
			{
				int nTransactionCount = CashRSPClearBufferThreadManager.GetTransactionCount(); 
				if ( nTransactionCount != 0 )
				{
					m_nTransactionCount += nTransactionCount;
					CashRSPClearBufferThreadManager.ClearTransactionCount();
				}
				else
				{
					m_nCurrentSiteIdx++;
				}
			}

			CashRSPClearBufferThreadManager.DeleteThread();
		}
	}

	if ( FALSE == bAlreadyRunning )
	{
		if (m_nCurrentSiteIdx < 0)
		{
			m_nCurrentSiteIdx = 0;
		}

		if ( m_nCurrentSiteIdx >= m_arraySites.GetSize() )
		{
			if ( m_nDialogState != CLEARBUFFERDLG_WAITCLOSE )
			{
				m_nDialogState = CLEARBUFFERDLG_WAITCLOSE;
				
				if ( TRUE == m_bMoreInfoMode )
				{
					m_editCashRSP1.SetWindowText( "Processing complete or timed out" );
					m_editCashRSP2.SetWindowText( "Click Continue to close this message" );
					m_buttonOK.SetWindowText( "Continue" );
				}
				else
				{
					EndDialog(IDCANCEL);
				}
			}
		}
		else
		{
			C5000SiteInfo infoSite;
			infoSite.m_nTransactionCount = 0;
			infoSite.m_nZeroFileCount = 0;
			infoSite.m_strExportControlLocs = "";
			infoSite.m_strEcrDataPath = m_arraySites[ m_nCurrentSiteIdx ].m_strDataPath;
			infoSite.m_strLocalDestPath = m_arraySites[ m_nCurrentSiteIdx ].m_strLocalPath;
			infoSite.m_strNetworkLockFilename = m_arraySites[ m_nCurrentSiteIdx ].m_strLockFile;

			CashRSPClearBufferThreadManager.PrepareThread( infoSite );
			CashRSPClearBufferThreadManager.RunThread();
		}
	}
}

/**********************************************************************/

void CCashRSPClearBufferDlg::OnDestroy()
{	
	if ( m_hTimer != NULL )
		KillTimer( m_hTimer );

	CDialog::OnDestroy();
}

/**********************************************************************/

void CCashRSPClearBufferDlg::OnCancel()
{
	OnOK();
}

/**********************************************************************/

void CCashRSPClearBufferDlg::OnOK()
{
	CashRSPClearBufferThreadManager.SetForceQuitFlag();
	EndDialog( IDOK );
}

/**********************************************************************/
