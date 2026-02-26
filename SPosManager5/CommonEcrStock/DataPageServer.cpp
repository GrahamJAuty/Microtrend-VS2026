/**********************************************************************/
#include "LocationCSVArray.h"
#include "Password.h"
/**********************************************************************/
#include "PropPageSystemDataList.h"
/**********************************************************************/

void CPropPageSystemDataList::PrepareServerPage()
{	
}

/**********************************************************************/

void CPropPageSystemDataList::BuildServerList()
{
	m_arrayLocation.RemoveAll();
	m_arrayAction.RemoveAll();

	int nServerType = EcrmanOptions.GetFeaturesServerSettingsType();
	bool bServerRestriction = EcrmanOptions.GetFeaturesServerRestrictionFlag();

	if ( ( SERVER_SYS == nServerType ) && ( FALSE == bServerRestriction ) )
		AddGlobal();
	else
	{
		if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
		{
			for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
			{
				if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
				{
					if ( ( SERVER_DB == nServerType ) && ( FALSE == bServerRestriction ) )
						AddDatabase( nDbIdx );
					else
						BuildServerListDatabase( nDbIdx );
				}
			}
		}
		else
		{
			CWordArray arrayDbIdx;
			dbDatabase.SortByName( arrayDbIdx );

			for ( int n = 0; n < arrayDbIdx.GetSize(); n++ )
			{
				int nDbIdx = arrayDbIdx.GetAt(n);
				if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
				{
					if ( ( SERVER_DB == nServerType ) && ( FALSE == bServerRestriction ) )
						AddDatabase( nDbIdx );
					else
						BuildServerListDatabase( nDbIdx );
				}
			}
		}
	}

	m_buttonAction1.SetWindowText ( "Edit" );
	m_buttonAction2.ShowWindow ( SW_HIDE );
	m_buttonAction3.ShowWindow ( SW_HIDE );
	m_buttonAction4.ShowWindow ( SW_HIDE );
	m_buttonAction5.ShowWindow ( SW_HIDE );	
	m_buttonAction6.ShowWindow ( SW_HIDE );

	SelectLine( m_listLocation, 0 );
}

/**********************************************************************/

void CPropPageSystemDataList::BuildServerListDatabase( int nDbIdx )
{
	CArray<int,int> arrayLocIdx;
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
		dbLocation.GetDatabaseLocIdxSet( nDbIdx, arrayLocIdx );
	else
		dbLocation.SortByName( arrayLocIdx, nDbIdx );

	bool bGotDatabase = FALSE;		
	for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
	{
		int nLocIdx = arrayLocIdx.GetAt(n);
		if ( PasswordArray.CheckLocationAccessLocIdx( nLocIdx ) == TRUE )
		{
			if ( dbLocation.IsWebSalesLocation( nLocIdx ) == FALSE )
			{
				bool bIndentLocation = TRUE;
				if ( EcrmanOptions.GetFeaturesDatabaseEditingType() == 2 )
				{
					bIndentLocation &= ( bGotDatabase );
					bIndentLocation &= ( EcrmanOptions.GetFeaturesServerSettingsType() == SERVER_DB );
				}

				if ( FALSE == bGotDatabase )
				{
					if ( EcrmanOptions.GetFeaturesDatabaseEditingType() != 2 )
						AddDatabase( nDbIdx );
						
					bGotDatabase = TRUE;
				}
				
				AddLocation( nLocIdx, bIndentLocation );
			}
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::SelectLocationServer()
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetCustomerAction( LocSelEntity, nAction );

	m_arrayAction.RemoveAll();
	m_listAction.DeleteAllItems();

	bool bShowServer = FALSE;
	
	if ( EcrmanOptions.GetFeaturesServerRestrictionFlag() == TRUE )
		bShowServer = ( LocSelEntity.GetConType() == NODE_LOCATION );
	else
	{
		switch( EcrmanOptions.GetFeaturesServerSettingsType() )
		{
		case SERVER_SYS:	bShowServer = ( LocSelEntity.GetConType() == NODE_SYSTEM );		break;
		case SERVER_DB:		bShowServer = ( LocSelEntity.GetConType() == NODE_DATABASE );	break;
		case SERVER_LOC:	bShowServer = ( LocSelEntity.GetConType() == NODE_LOCATION );	break;
		}
	}

	if ( TRUE == bShowServer )
		AddAction( NODE_SERVER, "Servers" );

	ForceSelectAction( nAction );
}

/**********************************************************************/

void CPropPageSystemDataList::GetServerAction( CLocationSelectorEntity& LocSelEntity, int& nAction )
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

void CPropPageSystemDataList::Button1ServerPage() 
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetCustomerAction( LocSelEntity, nAction );

	bool bCanEdit = FALSE;

	if ( EcrmanOptions.GetFeaturesServerRestrictionFlag() == TRUE )
		bCanEdit = ( LocSelEntity.GetConType() == NODE_LOCATION );
	else
	{
		switch( EcrmanOptions.GetFeaturesServerSettingsType() )
		{
		case SERVER_SYS:	bCanEdit = ( LocSelEntity.GetConType() == NODE_SYSTEM );	break;
		case SERVER_DB:		bCanEdit = ( LocSelEntity.GetConType() == NODE_DATABASE );	break;
		case SERVER_LOC:	bCanEdit = ( LocSelEntity.GetConType() == NODE_LOCATION );	break;
		}
	}

	if ( TRUE == bCanEdit )
	{
		int nServerDbIdx = -1;
		int nServerLocIdx = -1;
		int nTerminalLocIdx = -1;

		switch( EcrmanOptions.GetFeaturesServerSettingsType() )
		{
		case SERVER_DB:
			nServerDbIdx = LocSelEntity.GetDbIdx();
			break;

		case SERVER_LOC:
			nServerDbIdx = LocSelEntity.GetDbIdx();
			nServerLocIdx = LocSelEntity.GetEntityIdx();
			break;
		}

		if ( EcrmanOptions.GetFeaturesServerRestrictionFlag() == TRUE )
			nTerminalLocIdx = LocSelEntity.GetEntityIdx();

		m_pHelpers -> m_Server.EditServer( nServerDbIdx, nServerLocIdx, nTerminalLocIdx );
	}
}

/**********************************************************************/


