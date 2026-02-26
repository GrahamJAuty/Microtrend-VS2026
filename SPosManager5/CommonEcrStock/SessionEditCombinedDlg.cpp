/**********************************************************************/
 
/**********************************************************************/
#include "Consolidation.h"
//#include "DataManagerNonDb.h"
//#include "EcrmanOptionsIni.h"
//#include "globalfunctions.h"
#include "MaxLengths.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#include "SessionEditCombinedDlg.h"
/**********************************************************************/

CSessionEditCombinedDlg::CSessionEditCombinedDlg( CSessionCSVRecord& SessionRecord, CWnd* pParent /*=NULL*/)
	: CSSDialog(CSessionEditCombinedDlg::IDD, pParent), m_SessionRecord( SessionRecord )
{
	//{{AFX_DATA_INIT(CSessionEditCombinedDlg)
	//}}AFX_DATA_INIT
	m_ImageList.Create ( IDB_ICONS_COMBO, 31, 0, COLORREF(0xFFFFFF) );
	m_nSessionStartType = SessionRecord.GetSessionStartType();
}

/**********************************************************************/

void CSessionEditCombinedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSessionEditCombinedDlg)
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_RADIO_STARTTIME2, m_radioStartTime2);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_RADIO_STARTTIME, m_nSessionStartType);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSessionEditCombinedDlg, CDialog)
	//{{AFX_MSG_MAP(CSessionEditCombinedDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSessionEditCombinedDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_editName.LimitText( MAX_LENGTH_GENERAL_NAME );
	m_editName.SetWindowText( m_SessionRecord.GetName() );
	
	m_listSessions.SubclassDlgItem ( IDC_LIST, this );
	m_listSessions.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listSessions.InsertColumn ( 0, "", LVCFMT_LEFT, 370 );
	m_listSessions.SetLockItemZeroFlag( FALSE );

	COleDateTime timeDayStart = COleDateTime( 2005, 1, 1, EcrmanOptions.GetReportsDayStartHour(), EcrmanOptions.GetReportsDayStartMinute(), 0 );
	COleDateTime timeDayEnd = timeDayStart - COleDateTimeSpan( 0, 0, 1, 0 );

	CString strText;
	strText.Format ( "Session start time relative to business day (%2.2d:%2.2d to %2.2d:%2.2d)",
		timeDayStart.GetHour(), timeDayStart.GetMinute(), timeDayEnd.GetHour(), timeDayEnd.GetMinute() );

	CReportConsolidationArray<CSortedIntByString> arraySessions;

	for ( int n = 0; n < DataManagerNonDb.ReportSession.GetSize(); n++ )
	{
		CSessionCSVRecord Session;
		DataManagerNonDb.ReportSession.GetAt( n, Session );
		if ( Session.GetCombinedSessionFlag() == FALSE )
		{
			CSortedIntByString item;
			item.m_strItem = Session.GetEntityID();
			item.m_nItem = n;
			arraySessions.Consolidate( item );
		}
	}

	CStringArray arraySelected;
	m_SessionRecord.GetCombinedSessionIDs( arraySelected );

	for ( int n = 0; n < arraySelected.GetSize(); n++ )
	{
		CSortedIntByString item;
		item.m_strItem = arraySelected.GetAt(n);

		int nPos;
		if ( arraySessions.Find( item, nPos ) == TRUE )
		{	
			arraySessions.GetAt( nPos, item );

			CSSListTaggedSelectItem ListItem;
			ListItem.m_strText = DataManagerNonDb.ReportSession.GetName( item.m_nItem );
			ListItem.m_nData = item.m_nItem;
			ListItem.SetTagState(1);
			m_listSessions.AddItemToList( ListItem );

			arraySessions.RemoveAt( nPos );
		}
	}

	for ( int n = 0; n < arraySessions.GetSize(); n++ )
	{
		CSortedIntByString item;
		arraySessions.GetAt( n, item );

		CSSListTaggedSelectItem ListItem;
		ListItem.m_strText = DataManagerNonDb.ReportSession.GetName( item.m_nItem );
		ListItem.m_nData = item.m_nItem;
		ListItem.SetTagState(0);
		m_listSessions.AddItemToList( ListItem );
	}
	
	return TRUE;  
}

/**********************************************************************/

void CSessionEditCombinedDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC_LIST )	
	{
		m_listSessions.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

void CSessionEditCombinedDlg::OnButtonUp() 
{
	int nSel = m_listSessions.GetCurSel();
	if ( nSel > 0 && nSel < m_listSessions.GetItemCount() )
		m_listSessions.SwapItems( nSel, nSel - 1 );
}

/**********************************************************************/

void CSessionEditCombinedDlg::OnButtonDown() 
{
	int nSel = m_listSessions.GetCurSel();
	if ( nSel >= 0 && nSel < m_listSessions.GetItemCount() - 1 )
		m_listSessions.SwapItems( nSel, nSel + 1 );
}

/**********************************************************************/

void CSessionEditCombinedDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CString strName;
		m_editName.GetWindowText( strName );
		::TrimSpaces( strName, FALSE );

		if ( strName == "" )
			strName = "Combined Session";

		m_SessionRecord.SetName( strName );

		CStringArray arraySelected;
		for ( int nPos = 0; nPos < m_listSessions.GetItemCount(); nPos++ )
		{
			CSSListTaggedSelectItem ListItem;
			m_listSessions.GetListItem( nPos, ListItem );

			if ( ListItem.GetTagState() == 1 )
				arraySelected.Add( DataManagerNonDb.ReportSession.GetEntityID( ListItem.m_nData ) );
		}

		m_SessionRecord.SetSessionStartType( m_nSessionStartType );
		m_SessionRecord.SetCombinedSessionIDs( arraySelected );
	
		EndDialog( IDOK );
	}
}

/**********************************************************************/
