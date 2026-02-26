/**********************************************************************/
 
/**********************************************************************/
#include "CloneDatabaseDlg.h"
//#include "EcrmanOptionsIni.h"
#include "Password.h"
/**********************************************************************/
#include "PropPageSystemDataList.h"
/**********************************************************************/

void CPropPageSystemDataList::PrepareCustomerPage()
{	
}

/**********************************************************************/

void CPropPageSystemDataList::BuildCustomerList()
{
	m_arrayLocation.RemoveAll();
	m_arrayAction.RemoveAll();

	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
			if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
				BuildCustomerListDatabase( nDbIdx );
	}
	else
	{
		CWordArray arrayDbIdx;
		dbDatabase.SortByName( arrayDbIdx );

		for ( int n = 0; n < arrayDbIdx.GetSize(); n++ )
		{
			int nDbIdx = arrayDbIdx.GetAt(n);
			if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
				BuildCustomerListDatabase( nDbIdx );
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

void CPropPageSystemDataList::BuildCustomerListDatabase( int nDbIdx )
{
	CArray<int,int> arrayLocIdx;
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
		dbLocation.GetDatabaseLocIdxSet( nDbIdx, arrayLocIdx );
	else
		dbLocation.SortByName( arrayLocIdx, nDbIdx );

	bool bGotDatabase = FALSE;
	bool bShowDatabase = ( EcrmanOptions.GetFeaturesDatabaseEditingType() != 2 );
	for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
	{
		int nLocIdx = arrayLocIdx.GetAt(n);
		if ( PasswordArray.CheckLocationAccessLocIdx( nLocIdx ) == TRUE )
		{
			if ( dbLocation.IsWebSalesLocation( nLocIdx ) == FALSE )
			{
				if ( TRUE == bShowDatabase )
				{
					if ( FALSE == bGotDatabase )
					{
						AddDatabase( nDbIdx );
						bGotDatabase = TRUE;
					}
				}

				AddLocation( nLocIdx, bShowDatabase );
			}
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::SelectLocationCustomer()
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetCustomerAction( LocSelEntity, nAction );

	m_arrayAction.RemoveAll();
	m_listAction.DeleteAllItems();

	if ( NODE_LOCATION == LocSelEntity.GetConType() )
	{
		AddAction( NODE_CUSTOMER_ACCOUNT, "Customer Accounts" );
		AddAction( NODE_CUSTOMER_ONEOFF, "One-Off Customers" );
		
		if ( EcrmanOptions.GetReportsCustomerContactFlag() == TRUE )
		{
			AddAction( NODE_CUSTOMER_TRACKTRACE, "One-Off Contacts" );
		}
	}

	ForceSelectAction( nAction );
}

/**********************************************************************/

void CPropPageSystemDataList::GetCustomerAction( CLocationSelectorEntity& LocSelEntity, int& nAction )
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

void CPropPageSystemDataList::Button1CustomerPage() 
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetCustomerAction( LocSelEntity, nAction );

	if ( NODE_LOCATION == LocSelEntity.GetConType() )
	{
		switch( nAction )
		{
		case NODE_CUSTOMER_ACCOUNT:
			m_pHelpers -> m_Customer.EditCustomerAccount( LocSelEntity.GetEntityIdx() );
			break;

		case NODE_CUSTOMER_ONEOFF:
			m_pHelpers -> m_Customer.EditCustomerOneOff( LocSelEntity.GetEntityIdx() );
			break;

		case NODE_CUSTOMER_TRACKTRACE:
			m_pHelpers -> m_Customer.CustomerTrackTrace( LocSelEntity.GetEntityIdx() );
			break;
		}
	}
}

/**********************************************************************/


