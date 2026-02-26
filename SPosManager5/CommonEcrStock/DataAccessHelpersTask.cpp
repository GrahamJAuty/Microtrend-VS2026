/**********************************************************************/
#include "CommsBatchDlg.h"
#include "DownloadDateDlg.h"
#include "JobListDlg.h"
#include "KeyboardMap.h"
#include "ListDataTaskDlg.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "Password.h"
#include "SelectMultipleMasterDlg.h" 
/**********************************************************************/
#include "DataAccessHelpersTask.h"
/**********************************************************************/

CDataAccessHelpersTask::CDataAccessHelpersTask()
{
}

/**********************************************************************/

void CDataAccessHelpersTask::AdhocTask()
{
	CEposTaskDatabase TaskDatabase;
	TaskDatabase.CreateAdhocTask();

	CJobListDlg dlgJobList( TaskDatabase, TRUE, m_pParentWnd );
	if ( dlgJobList.DoModal() != IDOK )
		return;

	DoTask( TaskDatabase, 0 );
}

/**********************************************************************/

void CDataAccessHelpersTask::PresetTask( int nType )
{
	CEposTaskDatabase TaskDatabase;
	TaskDatabase.Read();
	DoTask( TaskDatabase, nType );
}

/**********************************************************************/

void CDataAccessHelpersTask::DoTask( CEposTaskDatabase& TaskDatabase, int nTaskNo )
{
	CString strDate = "";

	CEposTaskDatabase EditableTask;
	TaskDatabase.LoadEditableTask( EditableTask, nTaskNo );

	CEposTaskHeader TaskHeader;
	EditableTask.GetEditableTaskHeader( TaskHeader );
	
	if ( TaskHeader.GetImportType() == IMPORT_SPECIFIED )
	{
		if ( EditableTask.HasEditableTaskDownloadJob() == TRUE )
		{
			CString strTitle;
			strTitle.Format ( "Epos Import Date (%s)", TaskHeader.GetName() );

			CDownloadDateDlg dlgDate( strTitle, m_pParentWnd );
			if ( dlgDate.DoModal() == FALSE )
				return;

			if ( dlgDate.m_bImmediate == TRUE )
			{
				TaskHeader.SetImportType( IMPORT_IMMEDIATE );
				EditableTask.SetEditableTaskHeader( TaskHeader );
			}
			else
				strDate = dlgDate.m_strDate;
		}
	}

	CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo> Set;

	if ( TaskHeader.GetSelectLocationsWhenRunFlag() == TRUE )
	{		
		CJobListCSVArray JobList;
		CReportConsolidationArray<CSortedIntAndInt> arraySortedMasters;
		JobList.GetDownloadMasterList( EditableTask, arraySortedMasters);

		for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
		{
			CArray<CLocationMasterInfo,CLocationMasterInfo> arrayMasters;
			LocationMasterMap.GetLocationMasterList( nLocIdx, arrayMasters, TRUE, FALSE );

			for ( int nIdx = 0; nIdx < arrayMasters.GetCount(); nIdx++ )
			{
				CLocationMasterInfo infoMaster = arrayMasters.GetAt(nIdx);

				CSortedIntAndInt infoSortedMaster;
				infoSortedMaster.m_nInt1 = nLocIdx;
				infoSortedMaster.m_nInt2 = infoMaster.GetTerminalNo();

				int nPos;
				if ( arraySortedMasters.Find( infoSortedMaster, nPos ) == TRUE )
				{		
					CSelectMultipleEntityInfo infoSelect;

					if ( dbNetwork.FindNetworkByNumber( dbLocation.GetNetworkNo( nLocIdx ), infoSelect.m_nNwkIdx ) == FALSE )
						infoSelect.m_nNwkIdx = -1;

					infoSelect.m_nLocIdx = nLocIdx;
					infoSelect.m_nTNo = infoMaster.GetTerminalNo();
					infoSelect.m_bSelected = FALSE;

					Set.Add( infoSelect );
				}
			}
		}

		if ( Set.GetSize() == 0 )
		{
			Prompter.Error ( "You are not authorised to run\nthis task for any location" );
			return;
		}
		
		if ( Set.GetSize() > 1 )
		{
			CSelectMultipleMasterDlg dlg( Set, m_pParentWnd );	
			dlg.SetDoubleClickExitFlag();
			if ( dlg.DoModal() != IDOK )	
				return;

			{
				for ( int n = Set.GetSize() - 1; n >= 0; n-- )
					if ( FALSE == Set[n].m_bSelected )
						Set.RemoveAt(n);
			}
		}

		if ( Set.GetSize() == 0 )
			return;
	}

	CKeyboardMap KeyboardMap;
	CCommsBatchDlg BatchDlg ( EditableTask, ( Set.GetSize() > 0  ) ? &Set : NULL, strDate, KeyboardMap, m_pParentWnd );
	BatchDlg.DoModal();
}
	
/**********************************************************************/

bool CDataAccessHelpersTask::SetupTasks()
{
	bool bResult = FALSE;

	if ( dbLocation.GetSize() == 0 )
		Prompter.Error ( "You must set up at least one location\nbefore you can create Preset Tasks" );
	else if ( DataManager.LockAllUsers() == FALSE )
		Prompter.LockError( "setup preset tasks" );
	else
	{
		CListDataTaskDlg dlg ( m_pParentWnd );
		
		if ( dlg.DoModal() == IDOK )
			bResult = TRUE;

		DataManager.UnlockAllUsers();
	}

	return bResult;
}

/**********************************************************************/

bool CDataAccessHelpersTask::CanRunTask( int nTaskNo )
{
	bool bAllow = FALSE;

	switch( nTaskNo )
	{
	case 0x81:	bAllow = PasswordArray.GetEnable( PasswordTicks::TasksMenuPreset1 );	break;
	case 0x82:	bAllow = PasswordArray.GetEnable( PasswordTicks::TasksMenuPreset2 );	break;
	case 0x83:	bAllow = PasswordArray.GetEnable( PasswordTicks::TasksMenuPreset3 );	break;
	case 0x84:	bAllow = PasswordArray.GetEnable( PasswordTicks::TasksMenuPreset4 );	break;
	case 0x85:	bAllow = PasswordArray.GetEnable( PasswordTicks::TasksMenuPreset5 );	break;
	case 0x86:	bAllow = PasswordArray.GetEnable( PasswordTicks::TasksMenuPreset6 );	break;
	case 0x87:	bAllow = PasswordArray.GetEnable( PasswordTicks::TasksMenuPreset7 );	break;
	case 0x88:	bAllow = PasswordArray.GetEnable( PasswordTicks::TasksMenuPreset8 );	break;
	case 0x89:	bAllow = PasswordArray.GetEnable( PasswordTicks::TasksMenuPreset9 );	break;
	case 0x8A:	bAllow = PasswordArray.GetEnable( PasswordTicks::TasksMenuPreset10 );	break;
	case 0x8B:	bAllow = PasswordArray.GetEnable( PasswordTicks::TasksMenuPreset11 );	break;
	case 0x8C:	bAllow = PasswordArray.GetEnable( PasswordTicks::TasksMenuPreset12 );	break;
	case 0x8D:	bAllow = PasswordArray.GetEnable( PasswordTicks::TasksMenuPreset13 );	break;
	case 0x8E:	bAllow = PasswordArray.GetEnable( PasswordTicks::TasksMenuPreset14 );	break;
	case 0x8F:	bAllow = PasswordArray.GetEnable( PasswordTicks::TasksMenuPreset15 );	break;
	case 0x90:	bAllow = PasswordArray.GetEnable( PasswordTicks::TasksMenuPreset16 );	break;
	case 0x91:	bAllow = PasswordArray.GetEnable( PasswordTicks::TasksMenuPreset17 );	break;
	case 0x92:	bAllow = PasswordArray.GetEnable( PasswordTicks::TasksMenuPreset18 );	break;
	case 0x93:	bAllow = PasswordArray.GetEnable( PasswordTicks::TasksMenuPreset19 );	break;
	case 0x94:	bAllow = PasswordArray.GetEnable( PasswordTicks::TasksMenuPreset20 );	break;
	}

	return bAllow;
}

/**********************************************************************/
