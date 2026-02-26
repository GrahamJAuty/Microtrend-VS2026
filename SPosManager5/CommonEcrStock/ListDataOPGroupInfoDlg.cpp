/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "DefEditOPGroupDlg.h"
#include "DefEditOPGroupOverrideDlg.h"
#include "LocationCSVArray.h"
/**********************************************************************/
#include "ListDataOPGroupInfoDlg.h"
/**********************************************************************/

CSingleListContentHelpersOPGroupInfo::CSingleListContentHelpersOPGroupInfo( int nConLevel ) 
{
	m_nConLevel = nConLevel;

	switch( m_nConLevel )
	{
	case NODE_DATABASE:
		m_arrayConLevels.Add( NODE_DATABASE );
		m_arrayConLevels.Add( NODE_SYSTEM );
		break;

	case NODE_LOCATION:
		m_arrayConLevels.Add( NODE_LOCATION );
		m_arrayConLevels.Add( NODE_DATABASE );
		m_arrayConLevels.Add( NODE_SYSTEM );
		break;

	case NODE_SYSTEM:
	default:
		m_arrayConLevels.Add( NODE_SYSTEM );
		break;
	}
}

/**********************************************************************/

int CSingleListContentHelpersOPGroupInfo::GetArraySize()
{
	return DataManagerNonDb.OPGroupSystem.GetOPGroupCount();
}

/**********************************************************************/

bool CSingleListContentHelpersOPGroupInfo::PrepareLine( int nIndex, CString& strLine )
{
	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		for ( int nList = 0; nList < m_arrayConLevels.GetSize(); nList++ )
		{
			COPGroupMiniInfo infoMini;

			switch ( m_arrayConLevels.GetAt(nList ) )
			{
			case NODE_LOCATION:	DataManagerNonDb.OPGroupLocation.GetOPGroup( nIndex, infoMini );	break;
			case NODE_DATABASE:	DataManagerNonDb.OPGroupDatabase.GetOPGroup( nIndex, infoMini );	break;
			case NODE_SYSTEM:	DataManagerNonDb.OPGroupSystem.GetOPGroup( nIndex, infoMini );	break;
			}

			if ( ( FALSE == infoMini.m_bDefer) || ( nList >= m_arrayConLevels.GetSize() - 1 ) )
			{
				CString strGroupNo;
				strGroupNo.Format( "%2.2d", infoMini.m_nGroupNo );

				COPGroupInfo OPGroupInfo;
				OPGroupInfo.SetEnableFlagsSystem( infoMini.m_nFlags );

				CCSV csvFlags;
				for( int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++ )
				{
					if ( OPGroupInfo.GetEnableSystem(n) == TRUE )
						csvFlags.Add(n+1);
				}

				CCSV csv( '\t' );
				csv.Add( strGroupNo );

				if ( NODE_SYSTEM != m_nConLevel )
				{
					switch( m_arrayConLevels.GetAt(nList) )
					{
					case NODE_LOCATION:	csv.Add( "LOC" );	break;
					case NODE_DATABASE:	csv.Add( "DB" );	break;
					case NODE_SYSTEM:	csv.Add( "SYS" );	break;
					default:			csv.Add( "??" );	break;
					}
				}

				csv.Add( infoMini.m_strName );
				csv.Add( csvFlags.GetLine() );
				strLine = csv.GetLine();

				bResult = TRUE;

				break;
			}
		}
	}

	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersOPGroupInfo::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.ModifyStyle( 0, LVS_SINGLESEL, 0 );

	if ( NODE_SYSTEM == m_nConLevel )
	{
		list.InsertColumn ( 0, "Grp.No", LVCFMT_LEFT, 60 );
		list.InsertColumn ( 1, "Name", LVCFMT_LEFT, 170 );
		list.InsertColumn ( 2, "Enabled", LVCFMT_LEFT, 250);
	}
	else
	{
		list.InsertColumn ( 0, "Grp.No", LVCFMT_LEFT, 60 );
		list.InsertColumn ( 1, "Level", LVCFMT_LEFT, 60 );
		list.InsertColumn ( 2, "Name", LVCFMT_LEFT, 150 );
		list.InsertColumn ( 3, "Enabled", LVCFMT_LEFT, 210 );
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataOPGroupInfoDlg::CListDataOPGroupInfoDlg( int nConLevel, int nEntityIdx, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_nConLevel = nConLevel;
	m_nEntityIdx = nEntityIdx;
	m_pContentHelpers = new CSingleListContentHelpersOPGroupInfo( m_nConLevel );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataOPGroupInfoDlg::~CListDataOPGroupInfoDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataOPGroupInfoDlg::SetTitle()
{
	CString strTitle = "Configure Order Printer Groups ";

	switch( m_nConLevel )
	{
	case NODE_DATABASE:
		{
			CString strExtra;
			strExtra.Format( "(%s)", dbDatabase.GetName( m_nEntityIdx ) );
			SetWindowText ( strTitle + strExtra );
		}
		break;

	case NODE_LOCATION:
		{
			CString strExtra;
			strExtra.Format( "(%s)", dbLocation.GetName( m_nEntityIdx ) );
			SetWindowText ( strTitle + strExtra );
		}
		break;

	case NODE_SYSTEM:
	default:
		SetWindowText ( strTitle + "(System)" );
		break;
	}
}

/**********************************************************************/

void CListDataOPGroupInfoDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Edit" );
	m_button2.ShowWindow ( SW_HIDE );
	m_button3.SetWindowText ( "&Reset" );
	m_button4.ShowWindow ( SW_HIDE );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );
}

/**********************************************************************/

void CListDataOPGroupInfoDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataOPGroupInfoDlg::PrepareList()
{
	CListDataDlg::PrepareList();
	SelectLine(0);
}

/**********************************************************************/

void CListDataOPGroupInfoDlg::HandleButton1()
{
	int nSel = GetSafeSelection();
	if ( ( nSel >= 0 ) && ( nSel < DataManagerNonDb.OPGroupSystem.GetOPGroupCount() ) )
	{
		if ( NODE_SYSTEM == m_nConLevel )
		{
			CDefEditOPGroupDlg dlgOPGroup( nSel, m_WarnCancelAfterChange, this );
			dlgOPGroup.SetListDataDlg( this );
			dlgOPGroup.DoModal();
		}
		else
		{
			CDefEditOPGroupOverrideDlg dlgOPGroup( nSel, m_nConLevel, m_WarnCancelAfterChange, this );
			dlgOPGroup.SetListDataDlg( this );
			dlgOPGroup.DoModal();
		}
	}
}

/**********************************************************************/

void CListDataOPGroupInfoDlg::HandleButton3()
{
	switch( m_nConLevel )
	{
	case NODE_LOCATION:	
		if ( Prompter.YesNo( "Reset location printer to defaults?" ) == IDYES )
		{
			DataManagerNonDb.OPGroupLocation.ResetOPGroupList();
			m_listData.Invalidate();
			m_WarnCancelAfterChange.SetFlag();
		}
		break;

	case NODE_DATABASE:	
		if ( Prompter.YesNo( "Reset database printer groups to defaults?" ) == IDYES )
		{
			DataManagerNonDb.OPGroupDatabase.ResetOPGroupList();
			m_listData.Invalidate();
			m_WarnCancelAfterChange.SetFlag();
		}
		break;

	case NODE_SYSTEM:	
		if ( Prompter.YesNo( "Reset system printer groups to defaults?" ) == IDYES )
		{
			DataManagerNonDb.OPGroupSystem.ResetOPGroupList();
			m_listData.Invalidate();
			m_WarnCancelAfterChange.SetFlag();
		}
		break;
	}
}

/**********************************************************************/
