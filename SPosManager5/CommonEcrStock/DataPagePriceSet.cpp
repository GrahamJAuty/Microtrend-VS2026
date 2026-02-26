/**********************************************************************/
#include "CloneDatabaseDlg.h"
#include "Password.h"
#include "PriceSetCustomListDlg.h"
/**********************************************************************/
#include "PropPageSystemDataList.h"
/**********************************************************************/

void CPropPageSystemDataList::PreparePriceSetPage()
{	
	m_Filter.PreparePriceSetFilter();
}

/**********************************************************************/

void CPropPageSystemDataList::BuildPriceSetList()
{
	m_arrayLocation.RemoveAll();
	m_arrayAction.RemoveAll();

	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
			if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
				BuildPriceSetListDatabase( nDbIdx );
	}
	else
	{
		CWordArray arrayDbIdx;
		dbDatabase.SortByName( arrayDbIdx );

		for ( int n = 0; n < arrayDbIdx.GetSize(); n++ )
		{
			int nDbIdx = arrayDbIdx.GetAt(n);
			if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == TRUE )
				BuildPriceSetListDatabase( nDbIdx );
		}
	}
		
	m_buttonAction1.SetWindowText ( "Edit" );
	m_buttonAction2.SetWindowText ( "Report" );
	m_buttonAction3.SetWindowText ( "Clone" );
	m_buttonAction4.SetWindowText ( "Shelf Labels" );
#ifdef STOCKMAN_SYSTEM
	m_buttonAction5.SetWindowText ( "Item Labels" );	
#else
	m_buttonAction5.ShowWindow ( SW_HIDE );
#endif
	m_buttonAction6.ShowWindow ( SW_HIDE );

	if ( CheckCloneDatabase() == FALSE )
		m_buttonAction3.ShowWindow ( SW_HIDE );

	if ( ( Sysset.IsBarmanSystem() == FALSE ) || (EcrmanOptions.GetLabelsShelfAutoBatchFlag() == FALSE ) ) 
		m_buttonAction4.ShowWindow ( SW_HIDE );

	if ( ( Sysset.IsBarmanSystem() == FALSE ) || (EcrmanOptions.GetLabelsProductAutoBatchFlag() == FALSE ) ) 
		m_buttonAction5.ShowWindow ( SW_HIDE );

	SelectLine( m_listLocation, 0 );
}

/**********************************************************************/

void CPropPageSystemDataList::BuildPriceSetListDatabase( int nDbIdx )
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
			if ( FALSE == bGotDatabase )
			{
				AddDatabase( nDbIdx );
				bGotDatabase = TRUE;
			}

			AddLocation( nLocIdx );
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::SelectLocationPriceSet()
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetPriceSetAction( LocSelEntity, nAction );

	m_arrayAction.RemoveAll();
	m_listAction.DeleteAllItems();

	switch( LocSelEntity.GetConType() )
	{
	case NODE_LOCATION:
		{
			AddAction( NODE_PLU, "Master Plu File" );

			int nLocIdx = LocSelEntity.GetEntityIdx();
			for ( int nPriceSet = 1; nPriceSet <= dbLocation.GetPluPriceSetCount( nLocIdx ); nPriceSet++ )
			{
				int nDbPriceSet = 0;
				switch( nPriceSet )
				{
				case 1:	nDbPriceSet = dbLocation.GetDbPriceSet1( nLocIdx );	break;
				case 2:	nDbPriceSet = dbLocation.GetDbPriceSet2( nLocIdx );	break;
				case 3:	nDbPriceSet = dbLocation.GetDbPriceSet3( nLocIdx );	break;
				}

				CString strNodeText;
				if ( nDbPriceSet == 0 )
					strNodeText.Format ( "Location Price Set %d", nPriceSet );
				else
					strNodeText.Format ( "Database Price Set %d", nDbPriceSet );

				switch ( nPriceSet )
				{
				case 1: AddAction( NODE_PRICESET1, strNodeText );	break;
				case 2: AddAction( NODE_PRICESET2, strNodeText );	break;
				case 3: AddAction( NODE_PRICESET3, strNodeText );	break;
				}

				m_buttonAction3.EnableWindow( m_bCanCloneDatabase );
			}
		}
		break;

	case NODE_DATABASE:
		{
			AddAction( 0, "Master Plu File" );

			CArray<int,int> arrayLocIdx;
			dbLocation.GetDatabaseLocIdxSet( LocSelEntity.GetDbIdx(), arrayLocIdx );

			CReportConsolidationArray<CSortedIntByInt> arrayPriceSets;

			for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
			{
				int nLocIdx = arrayLocIdx.GetAt(n);

				for ( int nPriceSet = 1; nPriceSet <= dbLocation.GetPluPriceSetCount( nLocIdx ); nPriceSet++ )
				{
					int nDbPriceSet = 0;
					switch( nPriceSet )
					{
					case 1:	nDbPriceSet = dbLocation.GetDbPriceSet1( nLocIdx );	break;
					case 2:	nDbPriceSet = dbLocation.GetDbPriceSet2( nLocIdx );	break;
					case 3:	nDbPriceSet = dbLocation.GetDbPriceSet3( nLocIdx );	break;
					}

					if ( nDbPriceSet != 0 )
					{
						CSortedIntByInt item;
						item.m_nKey = nDbPriceSet;
						item.m_nVal = nPriceSet + ( nLocIdx * 4 );
						arrayPriceSets.Consolidate( item );
					}
				}
			}

			for ( int n = 0; n < arrayPriceSets.GetSize(); n++ )
			{
				CSortedIntByInt item;
				arrayPriceSets.GetAt( n, item );

				CString strNodeText = "";
				strNodeText.Format ( "Database Price Set %d", item.m_nKey );

				AddAction( item.m_nVal, strNodeText );
			}
		}
		break;
	}

	ForceSelectAction( nAction );
}

/**********************************************************************/

void CPropPageSystemDataList::GetPriceSetAction( CLocationSelectorEntity& LocSelEntity, int& nAction )
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

void CPropPageSystemDataList::SelectActionPriceSet()
{
}

/**********************************************************************/

void CPropPageSystemDataList::Button1PriceSetPage() 
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetPriceSetAction( LocSelEntity, nAction );
	int nDbNo = LocSelEntity.GetDbNo();

	switch( LocSelEntity.GetConType() )
	{
	case NODE_DATABASE:
		{
			if ( 0 == nAction )
				m_pHelpers -> m_Plu.EditPlu( nDbNo, FALSE );
			else
			{
				int nLocIdx = nAction / 4;
				int nPriceSet = nAction % 4;

				if ( ( nLocIdx >= 0 ) && ( nLocIdx < dbLocation.GetSize() ) )
					m_pHelpers -> m_Plu.EditPriceSet ( dbLocation.GetNetworkNo(nLocIdx), nDbNo, dbLocation.GetLocNo(nLocIdx), nPriceSet, FALSE );
			}
		}
		break;

	case NODE_LOCATION:
		{
			int nNetworkNo = LocSelEntity.GetNwkNo();
			int nLocNo = LocSelEntity.GetEntityNo();
			
			switch ( nAction )
			{
			case NODE_PLU:			
				m_pHelpers -> m_Plu.EditPlu ( nDbNo, FALSE );		
				break;

			case NODE_PRICESET1:	
				m_pHelpers -> m_Plu.EditPriceSet ( nNetworkNo, nDbNo, nLocNo, 1, FALSE );	
				break;

			case NODE_PRICESET2:	
				m_pHelpers -> m_Plu.EditPriceSet ( nNetworkNo, nDbNo, nLocNo, 2, FALSE );	
				break;

			case NODE_PRICESET3:	
				m_pHelpers -> m_Plu.EditPriceSet ( nNetworkNo, nDbNo, nLocNo, 3, FALSE );	
				break;
			}
		}
		break;
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button2PriceSetPage() 
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetPriceSetAction( LocSelEntity, nAction );

	int nDbIdx;
	int nDbNo = LocSelEntity.GetDbNo();
	if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == TRUE )
	{
		CArray<int,int> arrayLocIdx;
		dbLocation.GetDatabaseLocIdxSet( nDbIdx, arrayLocIdx );

		if ( arrayLocIdx.GetSize() == FALSE )
			Prompter.Error ( "You have not set up any locations that\nare linked to this database" );
		else if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == FALSE )
			Prompter.Error ( "You are not authorised to access this option\non this databsae." );
		else
		{
			CDataManagerInfo info;
			DataManagerNonDb.OpenAllergen( DB_READONLY, info );
			DataManagerNonDb.Allergen.AddAllergenZero();
			DataManagerNonDb.OpenOPGroupSystem( DB_READONLY, info );

			CPriceSetCustomListDlg dlg ( nDbIdx, this );
			dlg.DoModal();

			DataManagerNonDb.CloseAllergen( info );
			DataManagerNonDb.CloseOPGroupSystem( info );
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button3PriceSetPage() 
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetPriceSetAction( LocSelEntity, nAction );

	int nDbNo = LocSelEntity.GetDbNo();

	if ( nDbNo != 0 )
		CloneDatabase( nDbNo );
}

/**********************************************************************/

void CPropPageSystemDataList::Button4PriceSetPage() 
{
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetPriceSetAction( LocSelEntity, nAction );
	int nDbNo = LocSelEntity.GetDbNo();

	switch( LocSelEntity.GetConType() )
	{
	case NODE_DATABASE:
		if ( 0 == nAction )
			m_pHelpers -> m_Plu.PrintShelfLabels( nDbNo );
		else
		{
			int nLocIdx = nAction / 4;
			int nPriceSet = nAction % 4;

			if ( ( nLocIdx >= 0 ) && ( nLocIdx < dbLocation.GetSize() ) )
				m_pHelpers -> m_Plu.PrintShelfLabels( dbLocation.GetNetworkNo(nLocIdx), nDbNo, dbLocation.GetLocNo(nLocIdx), nPriceSet );
		}
		break;

	case NODE_LOCATION:
		{
			int nNetworkNo = LocSelEntity.GetNwkNo();
			int nLocNo = LocSelEntity.GetEntityNo();
			
			switch ( nAction )
			{
			case NODE_PLU:			m_pHelpers -> m_Plu.PrintShelfLabels( nDbNo );		break;
			case NODE_PRICESET1:	m_pHelpers -> m_Plu.PrintShelfLabels( nNetworkNo, nDbNo, nLocNo, 1 );	break;
			case NODE_PRICESET2:	m_pHelpers -> m_Plu.PrintShelfLabels( nNetworkNo, nDbNo, nLocNo, 2 );	break;
			case NODE_PRICESET3:	m_pHelpers -> m_Plu.PrintShelfLabels( nNetworkNo, nDbNo, nLocNo, 3 );	break;
			}
		}
		break;
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button5PriceSetPage() 
{
#ifdef STOCKMAN_SYSTEM
	int nAction;
	CLocationSelectorEntity LocSelEntity;
	GetPriceSetAction( LocSelEntity, nAction );
	int nDbNo = LocSelEntity.GetDbNo();

	switch( LocSelEntity.GetConType() )
	{
	case NODE_DATABASE:
		if ( 0 == nAction )
			m_pHelpers -> m_Plu.PrintBarcodeLabels( nDbNo );
		else
		{
			int nLocIdx = nAction / 4;
			int nPriceSet = nAction % 4;

			if ( ( nLocIdx >= 0 ) && ( nLocIdx < dbLocation.GetSize() ) )
				m_pHelpers -> m_Plu.PrintBarcodeLabels( dbLocation.GetNetworkNo(nLocIdx), nDbNo, dbLocation.GetLocNo(nLocIdx), nPriceSet );
		}
		break;

	case NODE_LOCATION:
		{
			int nNetworkNo = LocSelEntity.GetNwkNo();
			int nLocNo = LocSelEntity.GetEntityNo();
			
			switch ( nAction )
			{
			case NODE_PLU:			m_pHelpers -> m_Plu.PrintBarcodeLabels( nDbNo );	break;
			case NODE_PRICESET1:	m_pHelpers -> m_Plu.PrintBarcodeLabels( nNetworkNo, nDbNo, nLocNo, 1 );	break;
			case NODE_PRICESET2:	m_pHelpers -> m_Plu.PrintBarcodeLabels( nNetworkNo, nDbNo, nLocNo, 2 );	break;
			case NODE_PRICESET3:	m_pHelpers -> m_Plu.PrintBarcodeLabels( nNetworkNo, nDbNo, nLocNo, 3 );	break;
			}
		}
		break;
	}
#endif
}

/**********************************************************************/

