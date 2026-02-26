/**********************************************************************/
#include "LocationCSVArray.h"
#include "ListDataTableAreaDlg.h"
#include "ListDataTableNameDlg.h"
#include "Password.h"
#include "SelectTableGroupDlg.h"
#include "SelectTableGroupSetDlg.h"
/**********************************************************************/
#include "PropPageSystemDataList.h"
/**********************************************************************/

void CPropPageSystemDataList::PrepareTablesPage()
{	
}

/**********************************************************************/

void CPropPageSystemDataList::BuildTablesList()
{
	RebuildTablesList();

	m_buttonAction1.SetWindowText( "&Edit" );
	m_buttonAction2.ShowWindow ( SW_HIDE );
	m_buttonAction3.ShowWindow ( SW_HIDE );
	m_buttonAction4.ShowWindow ( SW_HIDE );
	m_buttonAction5.ShowWindow ( SW_HIDE );
	m_buttonAction6.ShowWindow ( SW_HIDE );

	SelectLine( m_listLocation, 0 );
}

/**********************************************************************/

void CPropPageSystemDataList::RebuildTablesList()
{
	m_arrayLocation.RemoveAll();
	m_arrayAction.RemoveAll();
	
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
			if ( PasswordArray.CheckLocationAccessLocIdx( nLocIdx ) == TRUE )
				AddLocation( nLocIdx, FALSE );
	}
	else
	{
		CArray<int,int> arrayLocIdx;
		dbLocation.SortByName( arrayLocIdx );

		for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
		{
			int nLocIdx = arrayLocIdx.GetAt(n);
			if ( PasswordArray.CheckLocationAccessLocIdx( nLocIdx ) == TRUE )
				AddLocation( nLocIdx, FALSE );
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::SelectLocationTables()
{
	int nAction ;
	CLocationSelectorEntity LocSelEntity;
	GetTablesAction( LocSelEntity, nAction );

	m_arrayAction.RemoveAll();
	m_listAction.DeleteAllItems();

	AddAction( NODE_TABLE_NAME, "Table Names" );
	AddAction( NODE_TABLE_AREA, "Table Areas" );
	AddAction( NODE_TABLE_GROUP, "Table Groups" );
	AddAction( NODE_TABLE_GROUPSET, "Table Group Sets" );

	ForceSelectAction( nAction );
}

/**********************************************************************/

void CPropPageSystemDataList::GetTablesAction( CLocationSelectorEntity& LocSelEntity, int& nAction )
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

void CPropPageSystemDataList::SelectActionTables()
{
}

/**********************************************************************/

void CPropPageSystemDataList::Button1TablesPage() 
{
	int nAction;
	CLocationSelectorEntity LocSelSelect;
	GetTablesAction( LocSelSelect, nAction );
	int nLocIdx = LocSelSelect.GetEntityIdx();

	switch( nAction )
	{
	case NODE_TABLE_NAME:
		{
			CDataManagerInfo infoDM;
			if ( DataManagerNonDb.OpenTableNames( DB_READWRITE, nLocIdx, infoDM ) == TRUE )
			{
				DataManagerNonDb.OpenTableGroups( DB_READONLY, nLocIdx, infoDM );

				CListDataTableNameDlg dlg ( nLocIdx, this );
				if ( dlg.DoModal() == IDOK )
				{
					if ( DataManagerNonDb.WriteTableNames( infoDM ) == FALSE )
					{
						Prompter.WriteError( infoDM );
					}
				}

				DataManagerNonDb.CloseTableGroups( infoDM );
				DataManagerNonDb.CloseTableNames( infoDM );
			}
			else
			{
				Prompter.ShareError( infoDM );
			}
		}
		break;

	case NODE_TABLE_AREA:
		{
			CDataManagerInfo infoDM;
			if ( DataManagerNonDb.OpenTableAreas( DB_READWRITE, nLocIdx, infoDM ) == TRUE )
			{
				CListDataTableAreaDlg dlg ( nLocIdx, this );
				if ( dlg.DoModal() == IDOK )
				{
					if ( DataManagerNonDb.WriteTableAreas( infoDM ) == FALSE )
					{
						Prompter.WriteError( infoDM );
					}
				}

				DataManagerNonDb.CloseTableAreas( infoDM );
			}
			else
			{
				Prompter.ShareError( infoDM );
			}
		}
		break;

	case NODE_TABLE_GROUP:
		{
			CSelectTableGroupDlg dlgSelect( nLocIdx, this );
			dlgSelect.DoModal();
		}
		break;

	case NODE_TABLE_GROUPSET:
		{
			CSelectTableGroupSetDlg dlgSelect( nLocIdx, this );
			dlgSelect.DoModal();
		}
		break;
	}
}

/**********************************************************************/
