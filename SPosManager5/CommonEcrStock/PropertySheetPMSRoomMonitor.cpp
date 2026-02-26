/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "PropertySheetPMSRoomMonitor.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetPMSRoomMonitor, CPropertySheet)
/**********************************************************************/

CPropertySheetPMSRoomMonitor::CPropertySheetPMSRoomMonitor( CWnd* pWndParent)
	 : CSSPropertySheet("", pWndParent)
{	
	m_psh.dwFlags |= PSH_NOAPPLYNOW;	
	
	m_page1.m_psp.dwFlags |= PSP_USETITLE;
	m_page2.m_psp.dwFlags |= PSP_USETITLE;
	m_page3.m_psp.dwFlags |= PSP_USETITLE;
	m_page4.m_psp.dwFlags |= PSP_USETITLE;
	m_page5.m_psp.dwFlags |= PSP_USETITLE;
	m_page6.m_psp.dwFlags |= PSP_USETITLE;

	int nRooms = DataManagerNonDb.PMSRoom.GetSize();
	
	if ( nRooms > SysInfo.GetMaxPMSRooms() )
		nRooms = SysInfo.GetMaxPMSRooms();

	if ( nRooms > MAX_PMS_ROOMS )	
		nRooms = MAX_PMS_ROOMS;

	if ( nRooms > 90 )	
		nRooms = 90;

	if ( nRooms < 1 )				
		nRooms = 1;
	
	m_strTitle1 = "";
	m_strTitle2 = "";
	m_strTitle3 = "";
	m_strTitle4 = "";
	m_strTitle5 = "";
	m_strTitle6 = "";

	m_page1.SetRoomIdxRange( 0, 14 );
	m_page2.SetRoomIdxRange( 15, 29 );
	m_page3.SetRoomIdxRange( 30, 44 );
	m_page4.SetRoomIdxRange( 45, 59 );
	m_page5.SetRoomIdxRange( 60, 74 );
	m_page6.SetRoomIdxRange( 75, 89 );

	if ( nRooms > 75 )			m_page6.SetRoomIdxRange( 75, nRooms - 1 );
	else if ( nRooms > 60 )		m_page5.SetRoomIdxRange( 60, nRooms - 1 );
	else if ( nRooms > 45 )		m_page4.SetRoomIdxRange( 45, nRooms - 1 );
	else if ( nRooms > 30 )		m_page3.SetRoomIdxRange( 30, nRooms - 1 );
	else if ( nRooms > 15 )		m_page2.SetRoomIdxRange( 15, nRooms - 1 );
	else						m_page1.SetRoomIdxRange( 0, nRooms - 1 );

	if ( nRooms <= 15 )			m_page2.SetRoomIdxRange( 0, -1 );
	if ( nRooms <= 30 )			m_page3.SetRoomIdxRange( 0, -1 );
	if ( nRooms <= 45 )			m_page4.SetRoomIdxRange( 0, -1 );
	if ( nRooms <= 60 )			m_page5.SetRoomIdxRange( 0, -1 );
	if ( nRooms <= 75 )			m_page6.SetRoomIdxRange( 0, -1 );
	
	AddRoomMonitorPage( m_page1, m_strTitle1 );
	AddRoomMonitorPage( m_page2, m_strTitle2 );
	AddRoomMonitorPage( m_page3, m_strTitle3 );
	AddRoomMonitorPage( m_page4, m_strTitle4 );
	AddRoomMonitorPage( m_page5, m_strTitle5 );
	AddRoomMonitorPage( m_page6, m_strTitle6 );
	
	SetTitle ( "PMS Room Status Monitor" );
}

/**********************************************************************/

void CPropertySheetPMSRoomMonitor::AddRoomMonitorPage( CPropPagePMSRoomMonitor& page, CString& strTitle )
{
	if ( page.GetRoomIdxEnd() >= page.GetRoomIdxStart() )
	{
		CPMSRoomCSVRecord Room1, Room2;
		DataManagerNonDb.PMSRoom.GetAt( page.GetRoomIdxStart(), Room1 );
		DataManagerNonDb.PMSRoom.GetAt( page.GetRoomIdxEnd(), Room2 );

		strTitle.Format ( "Rooms %d - %d",
			Room1.GetRoomNo(),
			Room2.GetRoomNo() );

		page.m_psp.pszTitle = (LPCTSTR) strTitle;
	
		AddPage( &page );
	}
}

/**********************************************************************/

CPropertySheetPMSRoomMonitor::~CPropertySheetPMSRoomMonitor()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetPMSRoomMonitor, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetPMSRoomMonitor)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropertySheetPMSRoomMonitor::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	GetDlgItem(IDOK) -> ShowWindow( SW_HIDE );
	GetDlgItem(IDCANCEL) -> SetWindowText( "Close" );
	return bResult;
}

/**********************************************************************/
#endif
/**********************************************************************/
