/**********************************************************************/
#include "EntityIDCreator.h"
#include "SelectReportSessionTypeDlg.h"
#include "SessionEditDlg.h"
#include "SessionEditCombinedDlg.h"
#include "SessionCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ListDataSessionDlg.h"
/**********************************************************************/

CSingleListContentHelpersSession::CSingleListContentHelpersSession()
{
}

/**********************************************************************/

int CSingleListContentHelpersSession::GetArraySize()
{
	return DataManagerNonDb.ReportSession.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersSession::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CSessionCSVRecord Session;
		DataManagerNonDb.ReportSession.GetAt( nIndex, Session );

		CString strStart, strEnd;

		if ( Session.GetCombinedSessionFlag() == FALSE )
		{
			strStart.Format ( "%2.2d:%2.2d", Session.GetSessionStartHour(), Session.GetSessionStartMinute() );
			strEnd.Format ( "%2.2d:%2.2d", Session.GetSessionEndHour(), Session.GetSessionEndMinute() );
		}
		else
		{
			strStart = "-----";
			strEnd = "-----";
		}

		CCSV csv ( '\t' );
		csv.Add ( Session.GetName() );
		csv.Add ( strStart );
		csv.Add ( strEnd );

		strLine = csv.GetLine();

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersSession::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManagerNonDb.ReportSession.GetSize() ) )
	{
		DataManagerNonDb.ReportSession.RemoveAt ( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersSession::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Name", LVCFMT_LEFT, 360 );
	list.InsertColumn ( 1, "Start", LVCFMT_RIGHT, 60 );
	list.InsertColumn ( 2, "End", LVCFMT_RIGHT, 60 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataSessionDlg::CListDataSessionDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_pContentHelpers = new CSingleListContentHelpersSession();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "report session" );
	m_pDisplayHelpers -> SetTopPluralText( "report sessions" );
	m_pDisplayHelpers -> SetBottomSingleText( "session" );
	m_pDisplayHelpers -> SetBottomPluralText( "sessions" );
}

/**********************************************************************/

CListDataSessionDlg::~CListDataSessionDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataSessionDlg::SetTitle()
{
	SetWindowText ( "Reporting Sessions" );
}

/**********************************************************************/

void CListDataSessionDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "Move &Up" );
	m_button5.SetWindowText ( "Move D&own" );
	m_button6.ShowWindow ( SW_HIDE );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
	MoveControl( &m_button4, 368, 147 );
	MoveControl( &m_button5, 368, 187 );
}

/**********************************************************************/

void CListDataSessionDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataSessionDlg::HandleButton1() 
{
	if ( SysInfo.IsEcrLiteSystem() == TRUE )
	{
		if ( DataManagerNonDb.ReportSession.GetSize() >= 2 )
		{	
			Prompter.Error( "You can only create a maximum of 2 Report Sessions\nwith Ecr Manager Lite." );
			return;
		}
	}

	int nInsertPos = GetSafeSelection() + 1;
	if ( nInsertPos == 0 ) nInsertPos = DataManagerNonDb.ReportSession.GetSize();

	CSessionCSVRecord NewRecord;
	NewRecord.SetName ( "" );

	CString strID = "";
	CEntityIDCreator<CSessionCSVArray> EntityIDCreator;
	if ( EntityIDCreator.CreateEntityID( DataManagerNonDb.ReportSession, strID ) == TRUE )
	{
		NewRecord.SetEntityID( strID );

		CSelectReportSessionTypeDlg dlg( this );
		if ( dlg.DoModal() == IDCANCEL )
			return;

		NewRecord.SetCombinedSessionFlag( dlg.GetAction() == 1 );

		if ( NewRecord.GetCombinedSessionFlag() == FALSE )
		{
			CSessionEditDlg dlg ( NewRecord, FALSE, this );
			dlg.DoModal();
			
			DataManagerNonDb.ReportSession.InsertAt ( nInsertPos, NewRecord );
			m_listData.SetItemCountEx ( GetArraySize() );
			SelectLine ( nInsertPos );
		}
		else
		{
			CSessionEditCombinedDlg dlg ( NewRecord, this );
			dlg.DoModal();

			DataManagerNonDb.ReportSession.InsertAt ( nInsertPos, NewRecord );
			m_listData.SetItemCountEx ( GetArraySize() );
			SelectLine ( nInsertPos );
		}

		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/

void CListDataSessionDlg::HandleButton2() 
{
	int nSelection = GetSafeSelection();
	if ( nSelection == -1 )
		return;
	
	CSessionCSVRecord SessionRecord;
	DataManagerNonDb.ReportSession.GetAt ( nSelection, SessionRecord );

	bool bChange = FALSE;
	if ( SessionRecord.GetCombinedSessionFlag() == FALSE )
	{
		CSessionEditDlg dlg ( SessionRecord, FALSE, this );
		bChange = ( dlg.DoModal() == IDOK );
	}
	else
	{
		CSessionEditCombinedDlg dlg ( SessionRecord, this );
		bChange = ( dlg.DoModal() == IDOK );
	}
	
	if ( TRUE == bChange )
	{
		DataManagerNonDb.ReportSession.SetAt ( nSelection, SessionRecord );
		m_WarnCancelAfterChange.SetFlag();
	}
		
	SelectLine ( nSelection );
}
		
/**********************************************************************/

void CListDataSessionDlg::HandleButton3() 
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataSessionDlg::HandleButton4()  
{
	int nSelection = GetSafeSelection ( 1, GetArraySize() - 1 );
	if ( nSelection == -1 )
		return;
	
	CSessionCSVRecord TempRecord1; 
	CSessionCSVRecord TempRecord2; 
	DataManagerNonDb.ReportSession.GetAt ( nSelection, TempRecord1 );
	DataManagerNonDb.ReportSession.GetAt ( nSelection - 1, TempRecord2 );
	DataManagerNonDb.ReportSession.SetAt ( nSelection, TempRecord2 );
	DataManagerNonDb.ReportSession.SetAt ( nSelection - 1, TempRecord1 );
	SelectLine ( nSelection - 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataSessionDlg::HandleButton5() 
{
	int nSelection = GetSafeSelection ( 0, GetArraySize() - 2 );
	if ( nSelection == -1 ) 
		return;

	CSessionCSVRecord TempRecord1; 
	CSessionCSVRecord TempRecord2; 
	DataManagerNonDb.ReportSession.GetAt ( nSelection, TempRecord1 );
	DataManagerNonDb.ReportSession.GetAt ( nSelection + 1, TempRecord2 );
	DataManagerNonDb.ReportSession.SetAt ( nSelection, TempRecord2 );
	DataManagerNonDb.ReportSession.SetAt ( nSelection + 1, TempRecord1 );
	SelectLine ( nSelection + 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

