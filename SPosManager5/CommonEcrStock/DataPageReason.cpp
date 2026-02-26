/**********************************************************************/
#include "..\CommonEcrStock\DownloadReasonDlg.h"
#include "..\CommonEcrStock\ReasonTextTypeDlg.h"
/**********************************************************************/
#include "CommsBatchDlg.h"
#include "DownloadDateDlg.h"
#include "ListDataReasonDlg.h"
#include "LocationCSVArray.h"
#include "Password.h"
/**********************************************************************/
#include "PropPageSystemDataList.h"
/**********************************************************************/

void CPropPageSystemDataList::PrepareReasonPage()
{	
}

/**********************************************************************/

void CPropPageSystemDataList::BuildReasonList()
{
	RebuildReasonList();

	m_buttonAction1.SetWindowText( "&Edit" );
	m_buttonAction2.ShowWindow ( SW_HIDE );
	m_buttonAction3.SetWindowText( "&Download" );
	m_buttonAction4.ShowWindow ( SW_HIDE );
	m_buttonAction5.ShowWindow ( SW_HIDE );
	m_buttonAction6.ShowWindow ( SW_HIDE );

	SelectLine( m_listLocation, 0 );
}

/**********************************************************************/

void CPropPageSystemDataList::RebuildReasonList()
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

void CPropPageSystemDataList::SelectLocationReason()
{
	int nAction ;
	CLocationSelectorEntity LocSelEntity;
	GetReasonAction( LocSelEntity, nAction );

	m_arrayAction.RemoveAll();
	m_listAction.DeleteAllItems();

	AddAction( 999999, "System Reason Texts" );
	AddAction( 999998, "Database Reason Texts" );

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

void CPropPageSystemDataList::GetReasonAction(CLocationSelectorEntity& LocSelEntity, int& nAction)
{
	LocSelEntity.SetSystemNode();
	nAction = 0;

	{
		int nSel = m_listLocation.GetCurSel();
		if ((nSel >= 0) && (nSel < m_arrayLocation.GetSize()))
			LocSelEntity = m_arrayLocation.GetAt(nSel);
	}

	{
		int nSel = m_listAction.GetCurSel();
		if ((nSel >= 0) && (nSel < m_arrayAction.GetSize()))
			nAction = m_arrayAction.GetAt(nSel);
	}
}

/**********************************************************************/

void CPropPageSystemDataList::SelectActionReason()
{
}

/**********************************************************************/

void CPropPageSystemDataList::Button1ReasonPage() 
{
	int nAction;
	CLocationSelectorEntity LocSelSelect;
	GetReasonAction( LocSelSelect, nAction );
	int nDbIdx = LocSelSelect.GetDbIdx();

	int nReasonType = 0;

	while (TRUE)
	{
		CReasonTextTypeDlg dlgType(this);
		dlgType.m_nSel = nReasonType;

		if (dlgType.DoModal() != IDOK)
		{
			break;
		}

		nReasonType = dlgType.m_nSel;

		switch (nAction)
		{
		case 999999:
		{
			CDataManagerInfo infoDM;
			if (DataManagerNonDb.OpenCCReasonSystem(DB_READWRITE, nReasonType, infoDM) == TRUE)
			{
				CLocationSelectorEntity LocSelReasonEdit;
				LocSelReasonEdit.SetSystemNode();

				CListDataReasonDlg dlg(LocSelReasonEdit, this);

				if (dlg.DoModal() == IDOK)
				{
					if (DataManagerNonDb.WriteCCReasonSystem(infoDM) == FALSE)
						Prompter.WriteError(infoDM);
				}

				DataManagerNonDb.CloseCCReasonSystem(infoDM);
			}
			else
			{
				Prompter.ShareError(infoDM);
			}
		}
		break;

		case 999998:
		{
			CDataManagerInfo infoDM;
			if (DataManagerNonDb.OpenCCReasonDatabase(nDbIdx, DB_READWRITE, nReasonType, infoDM) == TRUE)
			{
				DataManagerNonDb.CCReasonDatabase.LoadParentReasonsSystem();

				CLocationSelectorEntity LocSelReasonEdit;
				LocSelReasonEdit.SetDatabaseNode(nDbIdx);
				CListDataReasonDlg dlg(LocSelReasonEdit, this);

				if (dlg.DoModal() == IDOK)
				{
					if (DataManagerNonDb.WriteCCReasonDatabase(infoDM) == FALSE)
						Prompter.WriteError(infoDM);
				}

				DataManagerNonDb.CloseCCReasonDatabase(infoDM);
			}
			else
			{
				Prompter.ShareError(infoDM);
			}
		}
		break;

		default:
		{
			CDataManagerInfo infoDM;
			if (DataManagerNonDb.OpenCCReasonLocation(nAction, DB_READWRITE, nReasonType, infoDM) == TRUE)
			{
				DataManagerNonDb.CCReasonLocation.LoadParentReasonsDatabase(nDbIdx);
				DataManagerNonDb.CCReasonLocation.LoadParentReasonsSystem();

				CLocationSelectorEntity LocSelReasonEdit;
				LocSelReasonEdit.SetLocationNode(nAction);
				CListDataReasonDlg dlg(LocSelReasonEdit, this);

				if (dlg.DoModal() == IDOK)
				{
					if (DataManagerNonDb.WriteCCReasonLocation(infoDM) == FALSE)
						Prompter.WriteError(infoDM);
				}

				DataManagerNonDb.CloseCCReasonLocation(infoDM);
			}
			else
			{
				Prompter.ShareError(infoDM);
			}
		}
		break;
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button3ReasonPage()
{
	int nAction;
	CLocationSelectorEntity LocSelSelect;
	GetReasonAction(LocSelSelect, nAction);
	int nDbIdx = LocSelSelect.GetDbIdx();

	CArray<int, int> arrayLocIdx;

	switch (nAction)
	{
	case 999999:
	{
		for (int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++)
			arrayLocIdx.Add(nLocIdx);
	}
	break;

	case 999998:
		dbLocation.GetDatabaseLocIdxSet(LocSelSelect.GetDbIdx(), arrayLocIdx);
		break;

	default:
		arrayLocIdx.Add(nAction);
		break;
	}

	if (arrayLocIdx.GetSize() > 0)
	{
		CDownloadReasonDlg dlgType(this);
		if (dlgType.DoModal() == IDOK)
		{
			CEposTaskDatabase TaskDatabase;
			TaskDatabase.CreateSimpleTask(DN_REASON, arrayLocIdx, dlgType.m_nParam1);
			CString strDate = "";

			{
				CEposTaskHeader TaskHeader;
				TaskDatabase.GetEditableTaskHeader(TaskHeader);

				CString strTitle;
				TaskHeader.SetName("Download Reason Texts");
				strTitle.Format("Epos Import Date (%s)", TaskHeader.GetName());

				CDownloadDateDlg dlgDate(strTitle, m_pParentWnd);
				if (dlgDate.RunIfNeeded() == FALSE)
					return;

				if (dlgDate.m_bImmediate == TRUE)
					TaskHeader.SetImportType(IMPORT_IMMEDIATE);
				else
				{
					TaskHeader.SetImportType(IMPORT_SPECIFIED);
					strDate = dlgDate.m_strDate;
				}

				TaskDatabase.SetEditableTaskHeader(TaskHeader);
			}

			CKeyboardMap KeyboardMap;
			CCommsBatchDlg dlgComms(TaskDatabase, strDate, FALSE, KeyboardMap, -1, 0, 0, this);
			dlgComms.DoModal();
		}
	}
}

/**********************************************************************/
