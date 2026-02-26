/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "DefEditServerPresetDlg.h"
#include "DefEditServerPresetOverrideDlg.h"
#include "LocationCSVArray.h"
/**********************************************************************/
#include "ListDataServerPresetDlg.h"
/**********************************************************************/

CSingleListContentHelpersServerPreset::CSingleListContentHelpersServerPreset( int nConLevel, int nEntityIdx )
{
	m_nConLevel = nConLevel;
	m_nEntityIdx = nEntityIdx;

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

	case NODE_LOCATION_TERMINAL:
		m_arrayConLevels.Add( NODE_LOCATION_TERMINAL );
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

int CSingleListContentHelpersServerPreset::GetArraySize()
{
	return DataManagerNonDb.ServerPresetSystem.GetPresetCount();
}

/**********************************************************************/

bool CSingleListContentHelpersServerPreset::PrepareLine( int nIndex, CString& strLine )
{
	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		for ( int nList = 0; nList < m_arrayConLevels.GetSize(); nList++ )
		{
			CServerPresetMiniInfo infoMini;

			switch ( m_arrayConLevels.GetAt(nList ) )
			{
			case NODE_LOCATION_TERMINAL:	DataManagerNonDb.ServerPresetTerminal.GetPreset( nIndex, infoMini );	break;
			case NODE_LOCATION:				DataManagerNonDb.ServerPresetLocation.GetPreset( nIndex, infoMini );	break;
			case NODE_DATABASE:				DataManagerNonDb.ServerPresetDatabase.GetPreset( nIndex, infoMini );	break;
			case NODE_SYSTEM:				DataManagerNonDb.ServerPresetSystem.GetPreset( nIndex, infoMini );		break;
			}

			if ( ( infoMini.m_nServerNo >= 0 ) || ( nList >= m_arrayConLevels.GetSize() - 1 ) )
			{
				CString strPresetNo;
				strPresetNo.Format( "%3.3d", infoMini.m_nPresetNo );

				CCSV csv( '\t' );
				csv.Add( strPresetNo );

				if ( NODE_SYSTEM != m_nConLevel )
				{
					switch( m_arrayConLevels.GetAt(nList) )
					{
					case NODE_LOCATION_TERMINAL:	csv.Add( "TERM" );	break;
					case NODE_LOCATION:				csv.Add( "LOC" );	break;
					case NODE_DATABASE:				csv.Add( "DB" );	break;
					case NODE_SYSTEM:				csv.Add( "SYS" );	break;
					default:						csv.Add( "??" );	break;
					}
				}

				csv.Add( infoMini.m_nServerNo );
				csv.Add( DataManagerNonDb.ServerNameTable.GetServerName( m_nConLevel, m_nEntityIdx, infoMini.m_nServerNo, FALSE ) );
				strLine = csv.GetLine();

				bResult = TRUE;

				break;
			}
		}
	}

	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersServerPreset::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.ModifyStyle( 0, LVS_SINGLESEL, 0 );

	if ( NODE_SYSTEM == m_nConLevel )
	{
		list.InsertColumn ( 0, "PSet.No", LVCFMT_LEFT, 60 );
		list.InsertColumn ( 1, "Svr.No", LVCFMT_LEFT, 60 );
		list.InsertColumn ( 2, "Name", LVCFMT_LEFT, 360 );
	}
	else
	{
		list.InsertColumn ( 0, "PSet.No", LVCFMT_LEFT, 60 );
		list.InsertColumn ( 1, "Level", LVCFMT_LEFT, 60 );
		list.InsertColumn ( 2, "Svr.No", LVCFMT_LEFT, 60 );
		list.InsertColumn ( 3, "Name", LVCFMT_LEFT, 300 );
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataServerPresetDlg::CListDataServerPresetDlg( int nConLevel, int nEntityIdx, int nTermIdx, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_nConLevel = nConLevel;
	m_nEntityIdx = nEntityIdx;
	m_nTermIdx = nTermIdx;
	m_bCopyToLocation = FALSE;
	m_pContentHelpers = new CSingleListContentHelpersServerPreset( m_nConLevel, m_nEntityIdx );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataServerPresetDlg::~CListDataServerPresetDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataServerPresetDlg::SetTitle()
{
	CString strTitle = "Configure Server Presets ";

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

	case NODE_LOCATION_TERMINAL:
		{
			CString strExtra;
			strExtra.Format( "(%s, %s)", dbLocation.GetName( m_nEntityIdx ), dbLocation.GetTerminalName( m_nEntityIdx, m_nTermIdx ) );
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

void CListDataServerPresetDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Edit" );
	m_button2.ShowWindow ( SW_HIDE );
	m_button3.SetWindowText ( "&Reset" );
	m_button4.ShowWindow ( SW_HIDE );

	if ( NODE_LOCATION_TERMINAL == m_nConLevel )
		m_button5.SetWindowText ( "&Copy to\nLocation" );
	else
		m_button5.ShowWindow ( SW_HIDE );
	
	m_button6.ShowWindow ( SW_HIDE );
}

/**********************************************************************/

void CListDataServerPresetDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataServerPresetDlg::PrepareList()
{
	CListDataDlg::PrepareList();
	SelectLine(0);
}

/**********************************************************************/

void CListDataServerPresetDlg::HandleButton1()
{
	int nSel = GetSafeSelection();
	if ( ( nSel >= 0 ) && ( nSel < DataManagerNonDb.ServerPresetSystem.GetPresetCount() ) )
	{
		if ( NODE_SYSTEM == m_nConLevel )
		{
			CString strDeferName = DataManagerNonDb.ServerNameTable.GetServerName( NODE_SYSTEM, 0, 0, TRUE );
			CDefEditServerPresetDlg dlgPreset( nSel, strDeferName, m_WarnCancelAfterChange, this );
			dlgPreset.SetListDataDlg( this );
			dlgPreset.DoModal();
		}
		else
		{
			bool bDeferName = FALSE;
			int nServerEntityIdx = -1;
			
			switch( EcrmanOptions.GetFeaturesServerSettingsType() )
			{
			case SERVER_LOC:
				switch( m_nConLevel )
				{
				case NODE_DATABASE:
					bDeferName = TRUE;
					break;

				case NODE_LOCATION:
				case NODE_LOCATION_TERMINAL:
					nServerEntityIdx = m_nEntityIdx;
					break;
				}
				break;

			case SERVER_DB:
				switch( m_nConLevel )
				{
				case NODE_DATABASE:
					nServerEntityIdx = m_nEntityIdx;
					break;

				case NODE_LOCATION:
				case NODE_LOCATION_TERMINAL:
					{
						if ( dbDatabase.FindDatabaseByNumber( dbLocation.GetDbNo( m_nEntityIdx ), nServerEntityIdx ) == FALSE )
							nServerEntityIdx = -1;
					}
					break;
				}
				break;
			}
			
			CDefEditServerPresetOverrideDlg dlgPreset( nSel, m_nConLevel, nServerEntityIdx, bDeferName, m_WarnCancelAfterChange, this );
			dlgPreset.SetListDataDlg( this );
			dlgPreset.DoModal();
		}
	}
}

/**********************************************************************/

void CListDataServerPresetDlg::HandleButton3()
{
	switch( m_nConLevel )
	{
	case NODE_LOCATION_TERMINAL:	
		if ( Prompter.YesNo( "Reset terminal presets to defaults?" ) == IDYES )
		{
			DataManagerNonDb.ServerPresetTerminal.ResetPresetList();
			m_listData.Invalidate();
			m_WarnCancelAfterChange.SetFlag();
		}
		break;

	case NODE_LOCATION:	
		if ( Prompter.YesNo( "Reset location presets to defaults?" ) == IDYES )
		{
			DataManagerNonDb.ServerPresetLocation.ResetPresetList();
			m_listData.Invalidate();
			m_WarnCancelAfterChange.SetFlag();
		}
		break;

	case NODE_DATABASE:	
		if ( Prompter.YesNo( "Reset database presets to defaults?" ) == IDYES )
		{
			DataManagerNonDb.ServerPresetDatabase.ResetPresetList();
			m_listData.Invalidate();
			m_WarnCancelAfterChange.SetFlag();
		}
		break;

	case NODE_SYSTEM:	
		if ( Prompter.YesNo( "Reset system presets to defaults?" ) == IDYES )
		{
			DataManagerNonDb.ServerPresetSystem.ResetPresetList();
			m_listData.Invalidate();
			m_WarnCancelAfterChange.SetFlag();
		}
		break;
	}
}

/**********************************************************************/

void CListDataServerPresetDlg::HandleButton5()
{
	if ( Prompter.YesNo( "Are you sure you wish to apply the current presets\nfor this terminal to the entire location." ) == IDYES )
	{
		m_bCopyToLocation = TRUE;

		for ( int n = 0; n <= DataManagerNonDb.ServerPresetTerminal.GetPresetCount(); n++ )
		{
			CServerPresetMiniInfo infoMini;
			DataManagerNonDb.ServerPresetTerminal.GetPreset( n, infoMini );
			
			if ( infoMini.m_nServerNo != -1 )
				DataManagerNonDb.ServerPresetLocation.UpdatePreset( infoMini );
		}

		EndDialog( IDOK );
	}
}

/**********************************************************************/
