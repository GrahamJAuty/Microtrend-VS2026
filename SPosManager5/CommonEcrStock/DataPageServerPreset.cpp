/**********************************************************************/
#include "CommsBatchDlg.h"
#include "DownloadDateDlg.h"
#include "ListDataServerPresetDlg.h"
#include "ListDataServerPresetTerminalDlg.h"
#include "LocationCSVArray.h"
#include "Password.h"
#include "ServerPresetArray.h"
/**********************************************************************/
#include "PropPageSystemDataList.h"
/**********************************************************************/

void CPropPageSystemDataList::PrepareServerPresetPage()
{	
}

/**********************************************************************/

void CPropPageSystemDataList::BuildServerPresetList()
{
	RebuildServerPresetList();

	m_buttonAction1.SetWindowText ( "&Edit" );
	m_buttonAction2.SetWindowText ( "&Terminals" );
	m_buttonAction3.SetWindowText ( "&Download" );
	m_buttonAction4.ShowWindow ( SW_HIDE );
	m_buttonAction5.ShowWindow ( SW_HIDE );
	m_buttonAction6.ShowWindow ( SW_HIDE );

	if ( PasswordArray.GetEnable( PasswordTicks::CommsDownloads ) == FALSE )
		m_buttonAction3.ShowWindow( SW_HIDE );
	
	SelectLine( m_listLocation, 0 );
}

/**********************************************************************/

void CPropPageSystemDataList::RebuildServerPresetList()
{
	m_arrayLocation.RemoveAll();
	m_arrayAction.RemoveAll();
	
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
			if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
				AddDatabase( nDbIdx );
	}
	else
	{
		CWordArray arrayDbIdx;
		dbDatabase.SortByName( arrayDbIdx );

		for ( int n = 0; n < arrayDbIdx.GetSize(); n++ )
		{
			int nDbIdx = arrayDbIdx.GetAt(n);
			if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
				AddDatabase( nDbIdx );
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::SelectLocationServerPreset()
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetServerPresetAction( LocSelEntity, nAction );

	m_arrayAction.RemoveAll();
	m_listAction.DeleteAllItems();

	AddAction( 999999, "System Presets" );
	AddAction( 999998, "Database Presets" );

	int nDbIdx = LocSelEntity.GetDbIdx();

	CArray<int,int> arrayLocIdx;
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
		dbLocation.GetDatabaseLocIdxSet( nDbIdx, arrayLocIdx );
	else
		dbLocation.SortByName( arrayLocIdx, nDbIdx );

	for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
	{
		int nLocIdx = arrayLocIdx.GetAt(n);
		if ( PasswordArray.CheckLocationAccessLocIdx( nLocIdx ) == TRUE )
			AddAction( nLocIdx, dbLocation.GetName( nLocIdx ) );			
	}
	
	ForceSelectAction( nAction );
}

/**********************************************************************/

void CPropPageSystemDataList::GetServerPresetAction( CLocationSelectorEntity& LocSelEntity, int& nAction )
{
	LocSelEntity.SetSystemNode();
	nAction = 0;

	{
		int nSel = m_listLocation.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_arrayLocation.GetSize() ) )
			LocSelEntity = m_arrayLocation.GetAt( nSel );
	}

	{
		int nSel = m_listAction.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_arrayAction.GetSize() ) )
			nAction = m_arrayAction.GetAt(nSel);
	}
}

/**********************************************************************/

void CPropPageSystemDataList::SelectActionServerPreset()
{
}

/**********************************************************************/

void CPropPageSystemDataList::Button1ServerPresetPage() 
{
	int nAction;
	CLocationSelectorEntity LocSelEdit;
	GetServerPresetAction( LocSelEdit, nAction );
	int nDbIdx = LocSelEdit.GetDbIdx();

	bool bGotSelect = TRUE;

	switch( nAction )
	{
	case 999999:
		LocSelEdit.SetSystemNode();
		bGotSelect = TRUE;
		break;

	case 999998:
		LocSelEdit.SetDatabaseNode( nDbIdx );
		bGotSelect = TRUE;
		break;

	default:
		LocSelEdit.SetLocationNode( nAction );
		bGotSelect = TRUE;
		break;
	}

	if ( TRUE == bGotSelect )
	{
		EditServerPresets( LocSelEdit );
	}
}

/**********************************************************************/

void CPropPageSystemDataList::EditServerPresets( CLocationSelectorEntity& LocSelEdit ) 
{
	int nAction;
	CLocationSelectorEntity LocSelSelect;
	GetServerPresetAction( LocSelSelect, nAction );
	int nDbIdx = LocSelSelect.GetDbIdx();

	DataManagerNonDb.ServerNameTable.BuildList();

	switch( nAction )
	{
	case 999999:
		{
			CDataManagerInfo infoDM;
			if ( DataManagerNonDb.OpenServerPresetSystem( DB_READWRITE, infoDM ) == TRUE )
			{
				CListDataServerPresetDlg dlg( NODE_SYSTEM, 0, 0, this );
						
				if ( dlg.DoModal() == IDOK )
				{
					if ( DataManagerNonDb.WriteServerPresetSystem( infoDM ) == FALSE )
						Prompter.WriteError( infoDM );
				}

				DataManagerNonDb.CloseServerPresetSystem( infoDM );
			}
			else
			{
				Prompter.ShareError( infoDM );
			}
		}
		break;

	case 999998:
		{
			CDataManagerInfo infoDM;
			if ( DataManagerNonDb.OpenServerPresetDatabase( nDbIdx, DB_READWRITE, infoDM ) == TRUE )
			{
				DataManagerNonDb.OpenServerPresetSystem( DB_READONLY, infoDM );

				CListDataServerPresetDlg dlg( NODE_DATABASE, nDbIdx, 0, this );
						
				if ( dlg.DoModal() == IDOK )
				{
					if ( DataManagerNonDb.WriteServerPresetDatabase( infoDM ) == FALSE )
						Prompter.WriteError( infoDM );
				}

				DataManagerNonDb.CloseServerPresetSystem( infoDM );
				DataManagerNonDb.CloseServerPresetDatabase( infoDM );
			}
			else
			{
				Prompter.ShareError( infoDM );
			}
		}
		break;

	default:
		{
			CDataManagerInfo infoDM;
			if ( DataManagerNonDb.OpenServerPresetLocation( nAction, DB_READWRITE, infoDM ) == TRUE )
			{
				DataManagerNonDb.OpenServerPresetSystem( DB_READONLY, infoDM );
				DataManagerNonDb.OpenServerPresetDatabase( nDbIdx, DB_READONLY, infoDM );

				CListDataServerPresetDlg dlg( NODE_LOCATION, nAction, 0, this );
						
				if ( dlg.DoModal() == IDOK )
				{
					if ( DataManagerNonDb.WriteServerPresetLocation( infoDM ) == FALSE )
						Prompter.WriteError( infoDM );
				}

				DataManagerNonDb.CloseServerPresetSystem( infoDM );
				DataManagerNonDb.CloseServerPresetDatabase( infoDM );
				DataManagerNonDb.CloseServerPresetLocation( infoDM );
			}
			else
			{
				Prompter.ShareError( infoDM );
			}
		}
		break;
	}

	DataManagerNonDb.ServerNameTable.ClearList();
}

/**********************************************************************/

void CPropPageSystemDataList::Button2ServerPresetPage() 
{
	int nLocIdx;
	CLocationSelectorEntity LocSelSelect;
	GetServerPresetAction( LocSelSelect, nLocIdx );
	
	if ( ( nLocIdx >= 0 ) && ( nLocIdx < dbLocation.GetSize() ) )
	{
		if ( dbLocation.GetTerminalCount( nLocIdx ) >= 1 )
		{
			CListDataServerPresetTerminalDlg dlg( nLocIdx, this );
			dlg.DoModal();
		}
		else
		{
			Prompter.Error( "You have not defined any terminals for this location" );
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button3ServerPresetPage()
{
	int nAction;
	CLocationSelectorEntity LocSelSelect;
	GetServerPresetAction( LocSelSelect, nAction );
	int nDbIdx = LocSelSelect.GetDbIdx();

	CArray<int,int> arrayLocIdx;

	switch( nAction )
	{
	case 999999:
		{
			for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
				arrayLocIdx.Add( nLocIdx );
		}
		break;

	case 999998:
		dbLocation.GetDatabaseLocIdxSet( LocSelSelect.GetDbIdx(), arrayLocIdx );
		break;

	default:
		arrayLocIdx.Add( nAction );
		break;
	}

	if ( arrayLocIdx.GetSize() > 0 )
	{
		CEposTaskDatabase TaskDatabase;
		TaskDatabase.CreateSimpleTask( DN_SERVER_PRESET, arrayLocIdx );
		CString strDate = "";

		{
			CEposTaskHeader TaskHeader;
			TaskDatabase.GetEditableTaskHeader( TaskHeader );

			CString strTitle;
			TaskHeader.SetName( "Download Server Presets" );
			strTitle.Format ( "Epos Import Date (%s)", TaskHeader.GetName() );

			CDownloadDateDlg dlgDate( strTitle, m_pParentWnd );
			if ( dlgDate.RunIfNeeded() == FALSE )
				return;

			if ( dlgDate.m_bImmediate == TRUE )
				TaskHeader.SetImportType( IMPORT_IMMEDIATE );
			else
			{
				TaskHeader.SetImportType( IMPORT_SPECIFIED );
				strDate = dlgDate.m_strDate;
			}

			TaskDatabase.SetEditableTaskHeader( TaskHeader );
		}
	
		CKeyboardMap KeyboardMap;
		CCommsBatchDlg dlgComms( TaskDatabase, strDate, FALSE, KeyboardMap, -1, 0, 0, this );
		dlgComms.DoModal();
	}
}

/**********************************************************************/
