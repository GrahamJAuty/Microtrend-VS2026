/**********************************************************************/
 
/**********************************************************************/
//#include "EcrmanOptionsIni.h"
#include "LocationCSVArray.h"
/**********************************************************************/
#include "ServerSelectDlg.h"
/**********************************************************************/

CServerSelectDlg::CServerSelectDlg( CServerSelectionList& list, CEposSelectArray& select, CWnd* pParent /*=NULL*/)
	: CDialog(CServerSelectDlg::IDD, pParent), m_ServerSelectionList( list ), m_SelectArray( select )
{
	//{{AFX_DATA_INIT(CServerSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bServer = m_ServerSelectionList.GetUnknownServerFlag();
}

/**********************************************************************/

void CServerSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerSelectDlg)
	DDX_Check(pDX, IDC_CHECK_SERVER, m_bServer );
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CServerSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CServerSelectDlg)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_NONE, OnButtonSelectNone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CServerSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_listServers.SubclassDlgItem ( IDC_LIST, this );

	m_listServers.InsertColumn ( 0, "", LVCFMT_LEFT, 25 );
	m_listServers.InsertColumn ( 1, "Location", LVCFMT_LEFT, 140 );
	m_listServers.InsertColumn ( 2, "Code", LVCFMT_LEFT, 65 );
	m_listServers.InsertColumn ( 3, "Name", LVCFMT_LEFT, 130 );
	m_listServers.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );

	switch( EcrmanOptions.GetFeaturesServerSettingsType() )
	{
	case SERVER_SYS:
		AddListSystem();
		break;

	case SERVER_DB:
		AddListDatabase();
		break;

	case SERVER_LOC:
		AddListLocation();
		break;
	}

	return TRUE;
}

/**********************************************************************/

void CServerSelectDlg::AddListSystem()
{
	int nListPos = 0;
	for ( int nIndex = 0; nIndex < m_ServerSelectionList.GetSize(); nIndex++ )
	{
		CServerSelectionInfo info;
		m_ServerSelectionList.GetServerStatus( nIndex, info );

		CString strCode;
		strCode.Format ( "%4.4d", info.m_nServerNo );

		m_listServers.InsertItem ( nListPos, "" );
		m_listServers.SetItem ( nListPos, 1, LVIF_TEXT, "System", 0, 0, 0, NULL );
		m_listServers.SetItem ( nListPos, 2, LVIF_TEXT, strCode, 0, 0, 0, NULL );
		m_listServers.SetItem ( nListPos, 3, LVIF_TEXT, info.m_strServerName, 0, 0, 0, NULL );
		m_listServers.SetCheck ( nListPos, info.m_bSelected );
		
		m_listServers.SetItemData( nListPos, nIndex );
		nListPos++;				
	}
}

/**********************************************************************/

void CServerSelectDlg::AddListDatabase()
{
	int nListPos = 0;

	for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
	{
		CArray<int,int> arrayLocIdx;
		dbLocation.GetDatabaseLocIdxSet( nDbIdx, arrayLocIdx );

		bool bInclude = FALSE;
		for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
		{
			int nLocIdx = arrayLocIdx.GetAt(n);
			if ( m_SelectArray.GotTerminals( nLocIdx ) == TRUE )
			{
				bInclude = TRUE;
				break;
			}
		}

		if ( bInclude == FALSE )
			continue;

		CServerSelectionInfo info;
		info.m_nDbNo = dbDatabase.GetDbNo( nDbIdx );
		info.m_nNwkNo = 0;
		info.m_nLocNo = 0;
		info.m_nServerNo = 0;

		int nStartIdx;
		m_ServerSelectionList.FindServer( info, nStartIdx );

		int nEndIdx;
		info.m_nDbNo++;
		m_ServerSelectionList.FindServer( info, nEndIdx );

		for ( int nIndex = nStartIdx; nIndex < nEndIdx; nIndex++ )
		{
			CServerSelectionInfo info;
			m_ServerSelectionList.GetServerStatus( nIndex, info );

			CString strCode;
			strCode.Format( "%4.4d", info.m_nServerNo );

			m_listServers.InsertItem ( nListPos, "" );
			m_listServers.SetItem ( nListPos, 1, LVIF_TEXT, dbDatabase.GetName( nDbIdx ), 0, 0, 0, NULL );
			m_listServers.SetItem ( nListPos, 2, LVIF_TEXT, strCode, 0, 0, 0, NULL );
			m_listServers.SetItem ( nListPos, 3, LVIF_TEXT, info.m_strServerName, 0, 0, 0, NULL );
			m_listServers.SetCheck ( nListPos, info.m_bSelected );
		
			m_listServers.SetItemData( nListPos, nIndex );
			nListPos++;				
		}
	}
}

/**********************************************************************/

void CServerSelectDlg::AddListLocation()
{
	int nListPos = 0;

	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
	{
		if ( m_SelectArray.GotTerminals( nLocIdx ) == FALSE )
			continue;

		CServerSelectionInfo info;
		info.m_nDbNo = dbLocation.GetDbNo( nLocIdx );
		info.m_nNwkNo = dbLocation.GetNetworkNo( nLocIdx );
		info.m_nLocNo = dbLocation.GetLocNo( nLocIdx );
		info.m_nServerNo = 0;

		int nStartIdx;
		m_ServerSelectionList.FindServer( info, nStartIdx );

		int nEndIdx;
		info.m_nLocNo++;
		m_ServerSelectionList.FindServer( info, nEndIdx );

		for ( int nIndex = nStartIdx; nIndex < nEndIdx; nIndex++ )
		{
			CServerSelectionInfo info;
			m_ServerSelectionList.GetServerStatus( nIndex, info );

			CString strCode;
			strCode.Format( "%4.4d", info.m_nServerNo );

			m_listServers.InsertItem ( nListPos, "" );
			m_listServers.SetItem ( nListPos, 1, LVIF_TEXT, dbLocation.GetName( nLocIdx ), 0, 0, 0, NULL );
			m_listServers.SetItem ( nListPos, 2, LVIF_TEXT, strCode, 0, 0, 0, NULL );
			m_listServers.SetItem ( nListPos, 3, LVIF_TEXT, info.m_strServerName, 0, 0, 0, NULL );
			m_listServers.SetCheck ( nListPos, info.m_bSelected );
		
			m_listServers.SetItemData( nListPos, nIndex );
			nListPos++;				
		}
	}
}

/**********************************************************************/

void CServerSelectDlg::OnButtonSelectAll() 
{
	for ( int nIndex = 0; nIndex < m_listServers.GetItemCount(); nIndex++ )
		m_listServers.SetCheck ( nIndex, TRUE );
}

/**********************************************************************/

void CServerSelectDlg::OnButtonSelectNone() 
{
	for ( int nIndex = 0; nIndex < m_listServers.GetItemCount(); nIndex++ )
		m_listServers.SetCheck ( nIndex, FALSE );
}

/**********************************************************************/

void CServerSelectDlg::OnOK() 
{
	UpdateData( TRUE );

	for ( int n = 0; n < m_listServers.GetItemCount(); n++ )
	{
		int nIdx = m_listServers.GetItemData(n);

		CServerSelectionInfo info;
		m_ServerSelectionList.GetServerStatus( nIdx, info );
		info.m_bSelected = ( m_listServers.GetCheck( n ) != 0 );
		m_ServerSelectionList.SetServerStatus( nIdx, info );
	}
	
	m_ServerSelectionList.SetUnknownServerFlag( m_bServer != 0 );

	EndDialog( IDOK );
}

/**********************************************************************/




