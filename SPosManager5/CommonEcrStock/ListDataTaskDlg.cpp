/**********************************************************************/
#include "JobListDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ListDataTaskDlg.h"
/**********************************************************************/

CSingleListContentHelpersTask::CSingleListContentHelpersTask( CEposTaskDatabase& TaskDatabase ) : m_TaskDatabase( TaskDatabase )
{
}

/**********************************************************************/

int CSingleListContentHelpersTask::GetArraySize()
{
	return SysInfo.GetMaxPresetTasks();
}

/**********************************************************************/

bool CSingleListContentHelpersTask::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CCSV csv ( '\t' );
		
		CEposTaskHeader TaskHeader;
		m_TaskDatabase.GetLibraryTaskHeader( 0x81 + nIndex, TaskHeader );

		csv.Add ( TaskHeader.GetName () );

		int nBorderTime = ( EcrmanOptions.GetReportsDayStartHour() * 60 ) + EcrmanOptions.GetReportsDayStartMinute();
		int nRecordTime = ( TaskHeader.GetHours() * 60 ) + TaskHeader.GetMinutes();
		CString strTime = "";

		if ( TaskHeader.GetHours() < 24 )
		{
			if ( ( nRecordTime >= nBorderTime ) )
				strTime.Format ( "%2.2d:%2.2d", TaskHeader.GetHours (), TaskHeader.GetMinutes () );
			else
				strTime.Format ( "%2.2d:%2.2d **", TaskHeader.GetHours (), TaskHeader.GetMinutes () );
		}
		else
			strTime = "  -- : --";

		csv.Add ( strTime );
		csv.Add ( m_TaskDatabase.GetLibraryTaskJobCount( 0x81 + nIndex ) );

		strLine = csv.GetLine();

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataTaskDlg::CListDataTaskDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_TaskDatabase.Read();
	m_bMultiSelect = FALSE;	
	m_pContentHelpers = new CSingleListContentHelpersTask( m_TaskDatabase );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataTaskDlg::~CListDataTaskDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataTaskDlg::SetTitle()
{
	SetWindowText ( "Setup Preset Tasks" ); 
}

/**********************************************************************/

void CListDataTaskDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Edit" );
	m_button2.ShowWindow ( SW_HIDE );
	m_button3.ShowWindow ( SW_HIDE );
	m_button4.ShowWindow ( SW_HIDE );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );
	MoveControl( &m_button1, 368, 7 );
}

/**********************************************************************/

void CListDataTaskDlg::SetColumns()
{
	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.ModifyStyle( 0, LVS_SINGLESEL, 0 );
	m_listData.InsertColumn ( 0, "Name", LVCFMT_LEFT, 340 );
	m_listData.InsertColumn ( 1, "Time", LVCFMT_LEFT, 80 );
	m_listData.InsertColumn ( 2, "Jobs", LVCFMT_RIGHT, 60 );
}

/**********************************************************************/

void CListDataTaskDlg::HandleButton1() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex < 0 )
		return;

	CEposTaskDatabase EditableTask;
	m_TaskDatabase.LoadEditableTask( EditableTask, 0x81 + nIndex );

	CJobListDlg dlg ( EditableTask, FALSE, this );

	if ( dlg.DoModal() == IDOK )
	{
		m_TaskDatabase.SaveEditableTask( EditableTask );
		m_WarnCancelAfterChange.SetFlag();
	}

	SelectLine( nIndex );
}

/**********************************************************************/

bool CListDataTaskDlg::HandleOK()
{
	bool bResult = CListDataDlg::HandleOK();
	m_TaskDatabase.Write();
	return bResult;
}

/**********************************************************************/
