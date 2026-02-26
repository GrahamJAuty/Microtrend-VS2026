/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "ListDataSwitchRoomDlg.h"
/**********************************************************************/

CSingleListContentHelpersSwitchRoom::CSingleListContentHelpersSwitchRoom( CWordArray& arrayRoomIdx ) : m_arrayRoomIdx( arrayRoomIdx )
{
}

/**********************************************************************/

int CSingleListContentHelpersSwitchRoom::GetArraySize()
{
	return m_arrayRoomIdx.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersSwitchRoom::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		int nRoomIdx = m_arrayRoomIdx.GetAt( nIndex );

		CPMSRoomCSVRecord Room;
		DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, Room );

		CString strOption = "";
		if ( Room.GetRoomAllowExtraBedFlag() && Room.GetRoomAllowExtraCotFlag() )
			strOption = "Extra Bed or Cot";
		else if ( Room.GetRoomAllowExtraBedFlag() )
			strOption = "Extra Bed";
		else if ( Room.GetRoomAllowExtraCotFlag() )
			strOption = "Extra Cot";

		CString strSleeps = "";
		{
			int nSleeps = Room.GetSleeps();
			if ( strOption == "" )
				strSleeps.Format( "%d", nSleeps );
			else
				strSleeps.Format( "%d / %d", nSleeps, nSleeps + 1 );
		}
		
		strLine.Format ( "%d\t%s\t%s\t%s",
			Room.GetRoomNo(),
			Room.GetRoomName(),
			(const char*) strSleeps,
			(const char*) strOption );

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataSwitchRoomDlg::CListDataSwitchRoomDlg( const char* szRoomName, int nGuests, bool bTraining, CWordArray& arrayRoomIdx, CWnd* pParent) : CListDataDlg ( pParent ), m_arrayRoomIdx( arrayRoomIdx )
{
	m_nDestRoomIdx = 0;
	m_strRoomName = szRoomName;
	m_nGuests = nGuests;
	m_bTraining = bTraining;

	m_bMultiSelect = FALSE;
	m_pContentHelpers = new CSingleListContentHelpersSwitchRoom( m_arrayRoomIdx );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataSwitchRoomDlg::~CListDataSwitchRoomDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataSwitchRoomDlg::SetTitle()
{
	CString strText;

	if ( m_nGuests != 1 )
	{
		strText.Format ( "Switch Room (%s, %d guests)",
			(const char*) m_strRoomName,
			m_nGuests );
	}
	else
	{
		strText.Format ( "Switch Room (%s, 1 guest)",
			(const char*) m_strRoomName );
	}

	CString strTraining = "";
	if ( TRUE == m_bTraining )
		strTraining = " (Training Mode)";

	SetWindowText( strText + strTraining );
}

/**********************************************************************/

void CListDataSwitchRoomDlg::SetButtonTexts()
{
	m_button1.ShowWindow( SW_HIDE );
	m_button2.ShowWindow( SW_HIDE );
	m_button3.ShowWindow( SW_HIDE );
	m_button4.ShowWindow( SW_HIDE );
	m_button5.ShowWindow( SW_HIDE );
	m_button6.ShowWindow( SW_HIDE );
}

/**********************************************************************/

void CListDataSwitchRoomDlg::SetColumns()
{
	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.ModifyStyle( 0, LVS_SINGLESEL, 0 );
	m_listData.InsertColumn ( 0, "No", LVCFMT_LEFT, 60 );
	m_listData.InsertColumn ( 1, "Room Name", LVCFMT_LEFT, 260 );
	m_listData.InsertColumn ( 2, "Sleeps", LVCFMT_LEFT, 60 );
	m_listData.InsertColumn ( 3, "Options", LVCFMT_LEFT, 100 );
}

/**********************************************************************/

bool CListDataSwitchRoomDlg::HandleOK()
{
	int nSel = GetSafeSelection();
	m_nDestRoomIdx = m_arrayRoomIdx.GetAt( nSel );
	return TRUE;
}

/**********************************************************************/
#endif
/**********************************************************************/