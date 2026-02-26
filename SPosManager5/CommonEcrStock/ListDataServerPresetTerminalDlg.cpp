/**********************************************************************/
#include "BillAndReceiptTexts.h"
#include "DatabaseCSVArray.h"
#include "EditBillTextDlg.h"
#include "FileRemover.h"
#include "ListDataServerPresetDlg.h"
#include "LocationCSVArray.h"
#include "LocationSelectorEntity.h"
#include "NetworkCSVArray.h"
/**********************************************************************/
#include "ListDataServerPresetTerminalDlg.h"
/**********************************************************************/

CSingleListContentHelpersServerPresetTerminal::CSingleListContentHelpersServerPresetTerminal( int nLocIdx )
{
	m_nLocIdx = nLocIdx;
}

/**********************************************************************/

int CSingleListContentHelpersServerPresetTerminal::GetArraySize()
{
	return dbLocation.GetTerminalCount( m_nLocIdx );
}

/**********************************************************************/

bool CSingleListContentHelpersServerPresetTerminal::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		strLine.Format( "TNo %d Presets", dbLocation.GetTNo( m_nLocIdx, nIndex ) );
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataServerPresetTerminalDlg::CListDataServerPresetTerminalDlg( int nLocIdx, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_bMultiSelect = FALSE;
	m_nLocIdx = nLocIdx;
	m_pContentHelpers = new CSingleListContentHelpersServerPresetTerminal( nLocIdx );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataServerPresetTerminalDlg::~CListDataServerPresetTerminalDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataServerPresetTerminalDlg::SetTitle()
{
	SetWindowText ( "Server Presets by Terminal" );
}

/**********************************************************************/

void CListDataServerPresetTerminalDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Edit" );
	m_button2.ShowWindow ( SW_HIDE );
	m_button3.ShowWindow ( SW_HIDE );
	m_button4.ShowWindow ( SW_HIDE );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );
	m_buttonCancel.ShowWindow( SW_HIDE );
}

/**********************************************************************/

void CListDataServerPresetTerminalDlg::SetColumns()
{
	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.InsertColumn ( 0, dbLocation.GetName( m_nLocIdx ), LVCFMT_LEFT, 220 );

	ResizeControl( &m_listData, 170, 150 );
	MoveControl( &m_button1, 193, 7 );
	MoveControl( &m_button2, 193, 7 );
	MoveControl( &m_button3, 193, 7 );
	MoveControl( &m_button4, 193, 7 );
	MoveControl( &m_button5, 193, 7 );
	MoveControl( &m_button6, 193, 7 );
	MoveControl( &m_buttonOK, 193, 143 );
	MoveControl( &m_buttonCancel, 193, 143 );
	ResizeDialog( 250, 164 );
}

/**********************************************************************/

void CListDataServerPresetTerminalDlg::HandleButton1() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )	
		return;

	int nDbIdx;
	if ( dbDatabase.FindDatabaseByNumber( dbLocation.GetDbNo( m_nLocIdx ), nDbIdx ) == FALSE )
		nDbIdx = 0;

	DataManagerNonDb.ServerNameTable.BuildList();

	bool bGotReadWrite = FALSE;
	CDataManagerInfo infoDM;
	int nTNo = dbLocation.GetTNo( m_nLocIdx, nIndex );
	
	if ( DataManagerNonDb.OpenServerPresetLocation( m_nLocIdx, DB_READWRITE, infoDM ) == TRUE )
	{
		if ( DataManagerNonDb.OpenServerPresetTerminal( m_nLocIdx, nTNo, DB_READWRITE, infoDM ) == TRUE )
		{
			bGotReadWrite = TRUE;

			DataManagerNonDb.OpenServerPresetSystem( DB_READONLY, infoDM );
			DataManagerNonDb.OpenServerPresetDatabase( nDbIdx, DB_READONLY, infoDM );
			
			CListDataServerPresetDlg dlg( NODE_LOCATION_TERMINAL, m_nLocIdx, nIndex, this );
				
			bool bCopyToLocation = FALSE;
			bool bWriteError = FALSE;

			if ( dlg.DoModal() == IDOK )
			{
				bCopyToLocation = dlg.GetCopyToLocationFlag();

				if ( TRUE == bCopyToLocation )
				{
					if ( DataManagerNonDb.WriteServerPresetLocation( infoDM ) == FALSE )
					{
						bWriteError = TRUE;
						Prompter.WriteError( infoDM );
					}
				}
				else
				{
					if ( DataManagerNonDb.WriteServerPresetTerminal( infoDM ) == FALSE )
					{
						bWriteError = TRUE;
						Prompter.WriteError( infoDM );
					}
				}
			}

			CDataManagerInfo infoClose;
			DataManagerNonDb.CloseServerPresetSystem( infoClose );
			DataManagerNonDb.CloseServerPresetDatabase( infoClose );
			DataManagerNonDb.CloseServerPresetTerminal( infoClose );

			if ( ( TRUE == bCopyToLocation ) && ( FALSE == bWriteError ) )
			{
				int nNetWkIdx = 0;
				if ( dbNetwork.FindNetworkByNumber( dbLocation.GetNetworkNo( m_nLocIdx ), nNetWkIdx ) == FALSE )
					nNetWkIdx = 0;

				for ( int nTermIdx = 0; nTermIdx < dbLocation.GetTerminalCount( m_nLocIdx ); nTermIdx++ )
				{
					int nTNo = dbLocation.GetTNo( m_nLocIdx, nTermIdx );
					CFilenameUpdater FnUp ( SysFiles::ServerPreset, nNetWkIdx, nTNo, FNUP_FAMILY_TERM );
					CFileRemover FileRemover( FnUp.GetFilenameToUse() );
				}

				Prompter.Info( "Terminal settings have been copied to the\nlocation as requested." );
			}

			DataManagerNonDb.CloseServerPresetLocation( infoClose );
		}
		else
		{
			CDataManagerInfo infoClose;
			DataManagerNonDb.CloseServerPresetLocation( infoClose );
		}
	}
	
	if ( FALSE == bGotReadWrite )
	{
		Prompter.ShareError( infoDM );
	}

	DataManagerNonDb.ServerNameTable.ClearList();
}

/**********************************************************************/
