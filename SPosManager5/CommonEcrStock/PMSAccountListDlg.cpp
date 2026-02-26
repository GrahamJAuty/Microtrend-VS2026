/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSInvoiceBookingBuffer.h"
#include "PMSRoomStatusTable.h"
#include "PriceHelpers.h"
/**********************************************************************/
#include "PMSAccountListDlg.h"
/**********************************************************************/

CPMSAccountListDlg::CPMSAccountListDlg( int nRoomIdx, CWnd* pParent /*=NULL*/)
	: CDialog(CPMSAccountListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPMSAccountListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nRoomIdx = nRoomIdx;
	m_pCalendarDlg = NULL;
	m_bTraining = FALSE;
	m_bInvoicePrint = FALSE;
}

/**********************************************************************/

void CPMSAccountListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPMSAccountListDlg)
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDC_BUTTON1, m_button1);
	DDX_Control(pDX, IDC_BUTTON2, m_button2);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSAccountListDlg, CDialog)
	//{{AFX_MSG_MAP(CPMSAccountListDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPMSAccountListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetButtonControls();

	CPMSRoomCSVRecord Room;
	DataManagerNonDb.PMSRoom.GetAt( m_nRoomIdx, Room );
	
	CString strRoomText;
	::GetPMSRoomText( Room.GetRoomNo(), Room.GetRoomName(), strRoomText );
	
	CString strExtra;
	strExtra.Format( " (%s, %s)",
		(const char*) strRoomText,
		Room.GetRoomTypeName( FALSE ) );

	CString strTraining = "";
	if ( TRUE == m_bTraining )
		strTraining = " (Training Mode)";

	SetWindowText( m_strTitle + strExtra + strTraining );

	m_listAccounts.SubclassDlgItem ( IDC_LIST, this );
	m_listAccounts.InsertColumn ( 0, "Account Name", LVCFMT_LEFT, 170 );
	m_listAccounts.InsertColumn ( 1, "Covers", LVCFMT_RIGHT, 50 );
	m_listAccounts.InsertColumn ( 2, "Balance", LVCFMT_RIGHT, 100 );
	m_listAccounts.InsertColumn ( 3, "", LVCFMT_LEFT, 30 );

	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( m_nRoomIdx, infoRoom );

	m_listAccounts.SetItemCountEx( infoRoom.GetSafeAccountCount() );
	m_listAccounts.SetCurSel(0);

	return TRUE;
}

/**********************************************************************/

void CPMSAccountListDlg::SelectLine ( int nIndex )
{
	int nSize = m_listAccounts.GetItemCount();

	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listAccounts.SetCurSel ( nIndex );
	else 
		m_listAccounts.SetCurSel ( nSize - 1 );
	
	m_listAccounts.Invalidate();
}

/**********************************************************************/

void CPMSAccountListDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CPMSAccountListDlg::PrepareLine( int nIndex )
{
	m_strDisplayLine = "";

	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( m_nRoomIdx, infoRoom );

	if ( ( nIndex >= 0 ) && ( nIndex < infoRoom.GetSafeAccountCount() ) )
	{
		double dBalance = 0.0;

		CPMSRoomAccountInfo infoAccount;
		PMSRoomStatusTable.GetAccountInfo( m_nRoomIdx, nIndex, infoAccount );

		if ( FALSE == m_bInvoicePrint )
			dBalance = infoAccount.CalculateBalance();
		else
			dBalance = DataManagerNonDb.PMSInvoiceBookingBuffer.GetBalance( nIndex );
	
		CString strBalance = "";
		CString strCredit = "";

		switch ( CPriceHelpers::CompareDoubles( dBalance, 0.0, 2 ) )
		{
		case 1:
			CPriceHelpers::FormatDouble( dBalance, 2, strBalance );
			break;

		case -1:
			CPriceHelpers::FormatDouble( -dBalance, 2, strBalance );
			strCredit = "CR";
			break;

		case 0:
		default:
			strBalance = "0.00";
			break;
		}

		CString strGuests;
		strGuests.Format( "%d", infoAccount.GetGuestCount() );

		CCSV csvLine ('\t');
		csvLine.Add( infoAccount.GetGuestName() );
		csvLine.Add( strGuests );
		csvLine.Add( strBalance );
		csvLine.Add( strCredit );
		m_strDisplayLine = csvLine.GetLine();
	}
}

/**********************************************************************/

void CPMSAccountListDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	DoubleClickList();
	*pResult = 0;
}

/**********************************************************************/

void CPMSAccountListDlg::OnButton1()
{ 
	HandleButton1(); 

	if ( m_pCalendarDlg != NULL )
		m_pCalendarDlg -> UpdateDisplay();
}

/**********************************************************************/

void CPMSAccountListDlg::OnButton2()
{ 
	HandleButton2(); 

	if ( m_pCalendarDlg != NULL )
		m_pCalendarDlg -> UpdateDisplay();
}

/**********************************************************************/
#endif
/**********************************************************************/
