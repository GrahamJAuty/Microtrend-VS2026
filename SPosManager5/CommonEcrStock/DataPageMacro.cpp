/**********************************************************************/
#include "CommsBatchDlg.h"
#include "DownloadDateDlg.h"
#include "ListDataMacroDlg.h"
#include "LocationCSVArray.h"
#include "Password.h"
/**********************************************************************/
#include "PropPageSystemDataList.h"
/**********************************************************************/

void CPropPageSystemDataList::PrepareMacroPage()
{	
}

/**********************************************************************/

void CPropPageSystemDataList::BuildMacroList()
{
	RebuildMacroList();

	m_buttonAction1.SetWindowText( "&Edit" );
	m_buttonAction2.ShowWindow ( SW_HIDE );
	m_buttonAction3.SetWindowText( "&Download" );
	m_buttonAction4.ShowWindow ( SW_HIDE );
	m_buttonAction5.ShowWindow ( SW_HIDE );
	m_buttonAction6.ShowWindow ( SW_HIDE );

	SelectLine( m_listLocation, 0 );
}

/**********************************************************************/

void CPropPageSystemDataList::RebuildMacroList()
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

void CPropPageSystemDataList::SelectLocationMacro()
{
	int nAction ;
	CLocationSelectorEntity LocSelEntity;
	GetMacroAction( LocSelEntity, nAction );

	m_arrayAction.RemoveAll();
	m_listAction.DeleteAllItems();

	AddAction( 999999, "System Macros" );
	AddAction( 999998, "Database Macros" );

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

void CPropPageSystemDataList::GetMacroAction( CLocationSelectorEntity& LocSelEntity, int& nAction )
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

void CPropPageSystemDataList::SelectActionMacro()
{
}

/**********************************************************************/

void CPropPageSystemDataList::Button1MacroPage() 
{
	int nAction;
	CLocationSelectorEntity LocSelSelect;
	GetMacroAction( LocSelSelect, nAction );
	int nDbIdx = LocSelSelect.GetDbIdx();

	switch( nAction )
	{
	case 999999:
		{
			CDataManagerInfo infoDM;
			if ( DataManagerNonDb.OpenCCMacroSystem( DB_READWRITE, infoDM ) == TRUE )
			{
				CLocationSelectorEntity LocSelMacroEdit;
				LocSelMacroEdit.SetSystemNode();

				CListDataMacroDlg dlg( LocSelMacroEdit, this );
						
				if ( dlg.DoModal() == IDOK )
				{
					if ( DataManagerNonDb.WriteCCMacroSystem( infoDM ) == FALSE )
						Prompter.WriteError( infoDM );
				}

				DataManagerNonDb.CloseCCMacroSystem( infoDM );
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
			if ( DataManagerNonDb.OpenCCMacroDatabase( nDbIdx, DB_READWRITE, infoDM ) == TRUE )
			{
				DataManagerNonDb.CCMacroDatabase.LoadParentMacrosSystem();

				CLocationSelectorEntity LocSelMacroEdit;
				LocSelMacroEdit.SetDatabaseNode( nDbIdx );
				CListDataMacroDlg dlg( LocSelMacroEdit, this );
						
				if ( dlg.DoModal() == IDOK )
				{
					if ( DataManagerNonDb.WriteCCMacroDatabase( infoDM ) == FALSE )
						Prompter.WriteError( infoDM );
				}

				DataManagerNonDb.CloseCCMacroDatabase( infoDM );
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
			if ( DataManagerNonDb.OpenCCMacroLocation( nAction, DB_READWRITE, infoDM ) == TRUE )
			{
				DataManagerNonDb.CCMacroLocation.LoadParentMacrosDatabase(nDbIdx);
				DataManagerNonDb.CCMacroLocation.LoadParentMacrosSystem();

				CLocationSelectorEntity LocSelMacroEdit;
				LocSelMacroEdit.SetLocationNode( nAction );
				CListDataMacroDlg dlg( LocSelMacroEdit, this );
						
				if ( dlg.DoModal() == IDOK )
				{
					if ( DataManagerNonDb.WriteCCMacroLocation( infoDM ) == FALSE )
						Prompter.WriteError( infoDM );
				}

				DataManagerNonDb.CloseCCMacroLocation( infoDM );
			}
			else
			{
				Prompter.ShareError( infoDM );
			}
		}
		break;
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button3MacroPage()
{
	int nAction;
	CLocationSelectorEntity LocSelSelect;
	GetMacroAction( LocSelSelect, nAction );
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
		TaskDatabase.CreateSimpleTask( DN_MACRO, arrayLocIdx );
		CString strDate = "";

		{
			CEposTaskHeader TaskHeader;
			TaskDatabase.GetEditableTaskHeader( TaskHeader );

			CString strTitle;
			TaskHeader.SetName( "Download Macro Settings" );
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
