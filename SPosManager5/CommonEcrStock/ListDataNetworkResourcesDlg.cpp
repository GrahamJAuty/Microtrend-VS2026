/**********************************************************************/
#include "EditNetworkResourceDlg.h"
#include "NetworkResources.h"
/**********************************************************************/
#include "ListDataNetworkResourcesDlg.h"
/**********************************************************************/

CSingleListContentHelpersNetworkResources::CSingleListContentHelpersNetworkResources() 
{
}

/**********************************************************************/

int CSingleListContentHelpersNetworkResources::GetArraySize()
{
	return MAX_NETWORK_RESOURCES;
}

/**********************************************************************/

bool CSingleListContentHelpersNetworkResources::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CString strResource = NetworkResources.GetResourceName( nIndex );
		CString strUser = NetworkResources.GetUserName( nIndex );
		CString strPassword = NetworkResources.GetPassword( nIndex );

		CCSV csvLine( '\t' );
		csvLine.Add( nIndex + 1 );
		csvLine.Add( strResource );
		csvLine.Add( strUser );
		csvLine.Add( ( strPassword != "" ) ? "<Set>" : "" );
		strLine = csvLine.GetLine();
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersNetworkResources::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.ModifyStyle( 0, LVS_SINGLESEL, 0 );
	list.InsertColumn ( 0, "No", LVCFMT_LEFT, 40 );
	list.InsertColumn ( 1, "Resource Name", LVCFMT_LEFT, 350 );
	list.InsertColumn ( 2, "User Name", LVCFMT_LEFT, 200 );
	list.InsertColumn ( 3, "Password", LVCFMT_LEFT, 80 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataNetworkResourcesDlg::CListDataNetworkResourcesDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_pContentHelpers = new CSingleListContentHelpersNetworkResources();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataNetworkResourcesDlg::~CListDataNetworkResourcesDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataNetworkResourcesDlg::SetTitle()
{
	SetWindowText ( "Configure Access to Windows Network Resources" );
}

/**********************************************************************/

void CListDataNetworkResourcesDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Edit\nSettings" );
	m_button2.ShowWindow ( SW_HIDE );
	m_button3.SetWindowText ( "&Clear\nSettings" );
	m_button4.ShowWindow ( SW_HIDE );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );
}

/**********************************************************************/

void CListDataNetworkResourcesDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
	
	ResizeDialog( 547, 278 );
	ResizeControl( &m_listData, 467, 264 );
	MoveControl( &m_button1, 490, 7 );
	MoveControl( &m_button2, 490, 42 );
	MoveControl( &m_button3, 490, 77 );
	MoveControl( &m_button4, 490, 137 );
	MoveControl( &m_button5, 490, 172 );
	MoveControl( &m_button6, 490, 207 );
	MoveControl( &m_buttonOK, 490, 241 );
	MoveControl( &m_buttonCancel, 490, 257 );
}

/**********************************************************************/

void CListDataNetworkResourcesDlg::HandleButton1()
{
	int nSel = GetSafeSelection();
	if ( nSel != -1 )
	{
		CEditNetworkResourceDlg dlg( nSel, this );
		if ( dlg.DoModal() == IDOK )
		{
			m_listData.Invalidate();
			m_WarnCancelAfterChange.SetFlag();
		}
	}
}

/**********************************************************************/

void CListDataNetworkResourcesDlg::HandleButton3()
{
	int nSel = GetSafeSelection();
	if ( nSel != -1 )
	{
		CString strResource = NetworkResources.GetResourceName( nSel );
		CString strUser = NetworkResources.GetUserName( nSel );
		CString strPassword = NetworkResources.GetPassword( nSel );

		if ( ( strResource != "" ) || ( strUser != "" ) || ( strPassword != "" ) )
		{
			CString strMsg;
			strMsg.Format( "Are you sure you want to clear the settings\nfor network resource #%d", nSel + 1 );

			if ( Prompter.YesNo( strMsg ) == IDYES )
			{
				NetworkResources.SetResourceName( nSel, "" );
				NetworkResources.SetUserName( nSel, "" );
				NetworkResources.SetPassword( nSel, "" );
				m_listData.Invalidate();
				m_WarnCancelAfterChange.SetFlag();
			}
		}
	}
}

/**********************************************************************/

