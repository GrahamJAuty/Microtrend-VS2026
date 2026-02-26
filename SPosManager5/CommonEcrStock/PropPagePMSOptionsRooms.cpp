/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "DefEditRoomDlg.h"
#include "DefEditRoomDlg.h"
#include "NewRecordNumberDlg.h"
#include "NodeTypes.h"
#include "PMSOptions.h"
#include "PMSRoomStatusTable.h"
/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "PropPagePMSOptionsRooms.h"
/**********************************************************************/

CPropPagePMSOptionsRooms::CPropPagePMSOptionsRooms() : CPropertyPage(CPropPagePMSOptionsRooms::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePMSOptionsRooms)
	//}}AFX_DATA_INIT
	m_bIsModified = FALSE;

	for ( int nRoomIdx = 0; nRoomIdx < DataManagerNonDb.PMSRoom.GetSize(); nRoomIdx++ )
	{
		CPMSRoomStatusInfo infoRoom;
		PMSRoomStatusTable.GetRoomInfo( nRoomIdx, infoRoom );
		m_arrayOccupied.Add( infoRoom.GetBookingId() != 0 );
	}
}

/**********************************************************************/

CPropPagePMSOptionsRooms::~CPropPagePMSOptionsRooms()
{
}

/**********************************************************************/

void CPropPagePMSOptionsRooms::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePMSOptionsRooms)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePMSOptionsRooms, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePMSOptionsRooms)
	//}}AFX_MSG_MAP
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
END_MESSAGE_MAP()

/**********************************************************************/

int CPropPagePMSOptionsRooms::GetArraySize()
{
	return DataManagerNonDb.PMSRoom.GetSize();
}

/**********************************************************************/

BOOL CPropPagePMSOptionsRooms::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.InsertColumn ( 0, "No", LVCFMT_LEFT, 50 );
	m_listData.InsertColumn ( 1, "Name", LVCFMT_LEFT, 180 );
	m_listData.InsertColumn ( 2, "Type", LVCFMT_LEFT, 110 );
	m_listData.InsertColumn ( 3, "Sleeps", LVCFMT_LEFT, 50 );
	m_listData.InsertColumn ( 4, "Room Rate", LVCFMT_RIGHT, 90 );
	m_listData.InsertColumn ( 5, "Occupied", LVCFMT_LEFT, 80 );
	
	m_listData.ModifyStyle( LVS_OWNERDRAWFIXED, 0, 0 );
	m_listData.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listData.SetItemCountEx ( GetArraySize() );
	m_listData.SetCurSel(0);
	
	return TRUE;  
}

/**********************************************************************/

void CPropPagePMSOptionsRooms::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
   {
	   m_listData.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

BOOL CPropPagePMSOptionsRooms::PreTranslateMessage(MSG* pMsg) 
{
  if(pMsg->message==WM_KEYDOWN)
  {
	  if(pMsg->wParam==VK_RETURN )
	  {
		  if ( GetFocus() == &m_listData )
		  {
			  pMsg->wParam=NULL;
			  OnButtonEdit();
			  return TRUE;
		  }
	  }
  }
  return CDialog::PreTranslateMessage(pMsg);
}

/**********************************************************************/

void CPropPagePMSOptionsRooms::SelectLine ( int nIndex )
{
	m_listData.Invalidate();

	for ( int n = 0; n < m_listData.GetItemCount(); n++ )
	{
		if ( n != nIndex )
			m_listData.SetItemState(n, 0, LVIS_SELECTED);
		else
		{
			m_listData.SetItemState(n, LVIS_SELECTED, LVIS_SELECTED);
			m_listData.SetSelectionMark(n);
			m_listData.SetCurSel(n);
		}
	}

	m_listData.SetFocus();
}

/**********************************************************************/

void CPropPagePMSOptionsRooms::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		PrepareLine( nIndex );
		CCSV csv ( m_strDisplayLine, '\t' );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

bool CPropPagePMSOptionsRooms::PrepareLine( int nIndex )
{
	if ( nIndex < 0 || nIndex >= GetArraySize() )
		return FALSE;

	CPMSRoomCSVRecord Room;
	DataManagerNonDb.PMSRoom.GetAt( nIndex, Room );

	double dRate = 0.0;
	switch( PMSOptions.GetMiscDefaultRoomRateType() )
	{
	case PMS_BOOKING_RATE_BB:
		dRate = Room.GetRoomBBRate();
		break;

	case PMS_BOOKING_RATE_DBB:
		dRate = Room.GetRoomDBBRate();
		break;

	case PMS_BOOKING_RATE_ROOM:
	default:
		dRate = Room.GetRoomOnlyRate();
		break;
	}

	CString strSleeps;
	if ( Room.GetRoomAllowExtraBedFlag() || Room.GetRoomAllowExtraCotFlag() )
		strSleeps.Format( "%d / %d", Room.GetSleeps(), Room.GetSleeps() + 1 );
	else
		strSleeps.Format( "%d", Room.GetSleeps() );

	m_strDisplayLine.Format ( "%d\t%s\t%s\t%s\t%.2f",
		Room.GetRoomNo(),
		Room.GetRoomName(),
		Room.GetRoomTypeName(FALSE),
		(const char*) strSleeps,
		dRate
		 );

	if ( m_arrayOccupied[ nIndex ] == true )
		m_strDisplayLine += "\tYes";
	
	return TRUE;
}

/**********************************************************************/

int CPropPagePMSOptionsRooms::GetSafeSelection()
{
	return GetSafeSelection ( 0, GetArraySize() - 1 );
}

/**********************************************************************/

int CPropPagePMSOptionsRooms::GetSafeSelection( int nMin, int nMax )
{
	int nSel = m_listData.GetCurSel();
	if ( ( nSel < nMin ) || ( nSel > nMax ) || ( nSel >= m_listData.GetItemCount() ) )
		nSel = -1;
	
	return nSel;
}

/**********************************************************************/

void CPropPagePMSOptionsRooms::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnButtonEdit();
	*pResult = 0;
}

/**********************************************************************/

void CPropPagePMSOptionsRooms::OnButtonAdd()
{
	if ( DataManagerNonDb.PMSRoom.GetSize() >= SysInfo.GetMaxPMSRooms() )
	{
		CString strMsg;
		strMsg.Format( "This guest account system allows a maximum of %d rooms.", SysInfo.GetMaxPMSRooms() );
		Prompter.ContactDealer( strMsg );
		return;
	}

	bool bAdd = FALSE;
	int nRoomIdx, nRoomNo;
		
	for ( nRoomNo = 1; nRoomNo <= PMSRoom::RoomNo.Max; nRoomNo++ )
	{
		if ( DataManagerNonDb.PMSRoom.FindRoomByNumber ( nRoomNo, nRoomIdx ) == FALSE )
		{
			bAdd = TRUE;
			break;
		}
	}

	if ( bAdd == FALSE )
	{
		Prompter.Error ( "All available room numbers are already in use." );
		return;
	}

	CNewRecordNumberDlg dlg ( NODE_PMS_ROOM, this );
	dlg.m_nNumber = nRoomNo;
	if ( dlg.DoModal() != IDOK )
		return;

	nRoomNo = dlg.m_nNumber;
	if ( DataManagerNonDb.PMSRoom.FindRoomByNumber ( nRoomNo, nRoomIdx ) )
	{
		Prompter.Error ( "The selected room number is already in use." );
		return;
	}

	CPMSRoomCSVRecord Room;
	Room.SetRoomNo( nRoomNo );
	Room.SetRoomName( Room.GetDefaultName() );
	DataManagerNonDb.PMSRoom.InsertAt ( nRoomIdx, Room );
	m_arrayOccupied.InsertAt( nRoomIdx, false );

	CDefEditRoomDlg dlgEdit ( nRoomIdx, TRUE, this );
	dlgEdit.SetPropPage( this );
	dlgEdit.DoModal();
	
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( dlgEdit.GetIndex() );
	m_bIsModified = TRUE;
}

/**********************************************************************/

void CPropPagePMSOptionsRooms::OnButtonEdit()
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )
		return;

	CDefEditRoomDlg dlg ( nIndex, FALSE, this );
	dlg.SetPropPage( this );
	dlg.DoModal();

	SelectLine ( dlg.GetIndex() );
	
	m_bIsModified = TRUE;
}

/**********************************************************************/

void CPropPagePMSOptionsRooms::OnButtonDelete()
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )
		return;

	if ( m_arrayOccupied[ nIndex ] == TRUE )
	{
		Prompter.Error ( "You cannot delete a room which is currently occupied" );
		return;
	}

	CPMSRoomCSVRecord Room;
	DataManagerNonDb.PMSRoom.GetAt( nIndex, Room );

	CString strRoom;
	::GetPMSRoomText( Room.GetRoomNo(), Room.GetRoomName(), strRoom, TRUE );

	CString strMsg;
	strMsg = "Are you sure you wish to delete this room\n\n";
	strMsg += strRoom;

	if ( Prompter.YesNo ( strMsg ) != IDYES )
		return;

	DataManagerNonDb.PMSRoom.RemoveAt( nIndex );
	m_arrayOccupied.RemoveAt( nIndex );
	
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nIndex );
	m_bIsModified = TRUE;
}

/**********************************************************************/
#endif
/**********************************************************************/
