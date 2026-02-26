/**********************************************************************/
#include "CloneDatabaseDlg.h"
#include "Password.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPageSystemDataList.h"
/**********************************************************************/

void CPropPageSystemDataList::PrepareReportSetPage()
{	
	m_Filter.PrepareReportSetFilter();
}

/**********************************************************************/

void CPropPageSystemDataList::BuildReportSetList()
{
	m_arrayLocation.RemoveAll();
	m_arrayAction.RemoveAll();

	if ( GotLocalData() == TRUE )
	{
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

	if ( PasswordArray.CheckLocationAccessSystem() == TRUE )
	{
		if ( GotSystemFilterData() == TRUE )
			AddSystemFilter();

		if ( GotGlobalData() == TRUE )
			AddGlobal();

		if ( GotReportListData() == TRUE )
			AddReportList();

		if ( GotTimeSettingData() == TRUE )
			AddTimeSetting();
	}
			
	m_buttonAction1.SetWindowText ( "Edit" );
	m_buttonAction2.SetWindowText ( "Clone" );
	m_buttonAction3.ShowWindow ( SW_HIDE );
	m_buttonAction4.ShowWindow ( SW_HIDE );
	m_buttonAction5.ShowWindow ( SW_HIDE );
	m_buttonAction6.ShowWindow ( SW_HIDE );

	if ( CheckCloneDatabase() == FALSE )
		m_buttonAction2.ShowWindow ( SW_HIDE );

	SelectLine( m_listLocation, 0 );
}

/**********************************************************************/

void CPropPageSystemDataList::SelectLocationReportSet()
{
	int nDbNo, nAction, nConType;
	GetEcrDataAction( nDbNo, nAction, nConType );

	m_arrayAction.RemoveAll();
	m_listAction.DeleteAllItems();

	switch( nConType )
	{
	case NODE_SYSTEM_FILTER:
		if ( m_Filter.GetPluFilterFlag() == SCOPE_DBASE )		AddAction( NODE_PLUFILTER_NAME, "System Plu Report Filters" );
		if ( m_Filter.GetDeptSetFlag() == SCOPE_DBASE )			AddAction( NODE_DEPARTMENT_SET, "System Department Sets" );
		if ( m_Filter.GetCategorySetFlag() == SCOPE_DBASE )		AddAction( NODE_CATEGORY_SET, "System Category Sets" );
		if ( m_Filter.GetCustomFieldSetFlag() == SCOPE_DBASE )	AddAction( NODE_CUSTOM_FIELD_SET, "System Custom Field Sets" );
		m_buttonAction2.EnableWindow( FALSE );
		break;

	case NODE_TIME_SETTING:
		if ( m_Filter.GetSessionFlag() == SCOPE_GLOBAL )				AddAction( NODE_SESSION, "Report Sessions" );
		if ( m_Filter.GetTimeSliceMapsFlag() == SCOPE_GLOBAL )			AddAction( NODE_TIMESLICE, "Time Slice Maps" );	
		m_buttonAction2.EnableWindow( FALSE );
		break;

	case NODE_REPORT_LIST:

		if ( m_Filter.GetBatchEposFlag() == SCOPE_GLOBAL )				AddAction( NODE_BATCH_TERMINAL, "EPOS Report Batch Print" );
		if ( m_Filter.GetEposReportsFlag() == SCOPE_GLOBAL )			AddAction( NODE_EPOS_REPORT, "EPOS Report List Order" );
		if ( m_Filter.GetSalesHistoryPluFlag() == SCOPE_GLOBAL )		AddAction( NODE_HISTORY_PLU, "Plu Sales History Reports" );
			
	#ifdef STOCKMAN_SYSTEM
		if ( m_Filter.GetSalesHistoryStockFlag() == SCOPE_GLOBAL )		AddAction( NODE_HISTORY_STOCK, "Stock Sales History Reports" );
		if ( m_Filter.GetStockLevelReportsFlag() == SCOPE_GLOBAL )		AddAction( NODE_STOCK_REPORT, "Stock Level Report List Order" );
	#endif

		m_buttonAction2.EnableWindow( FALSE );
		break;
		
	default:
		if ( 0 == nDbNo )
		{
			if ( SysInfo.IsEcrLiteSystem() == FALSE )
			{
				if ( m_Filter.GetEcrSupplierFlag() == SCOPE_GLOBAL )		AddAction( NODE_ECR_SUPPLIER, "Suppliers" );
				if ( m_Filter.GetDeptSetFlag() == SCOPE_GLOBAL )			AddAction( NODE_DEPARTMENT_SET, "Department Sets" );
				if ( m_Filter.GetCategorySetFlag() == SCOPE_GLOBAL )		AddAction( NODE_CATEGORY_SET, "Category Sets" );
				if ( m_Filter.GetCustomFieldSetFlag() == SCOPE_GLOBAL )		AddAction( NODE_CUSTOM_FIELD_SET, "Custom Field Sets" );
				if ( m_Filter.GetTaxFlag() == SCOPE_GLOBAL )				AddAction( NODE_TAX_DATE, EcrmanOptions.GetTaxString( "Historical %T rates" ) );
			}

	#ifdef STOCKMAN_SYSTEM
			if ( m_Filter.GetSupplierSetFlag() == SCOPE_GLOBAL )		AddAction( NODE_SUPPLIER_SET, "Supplier Sets" );
	#endif
			
			m_buttonAction2.EnableWindow( FALSE );
		}
		else
		{
			if ( SysInfo.IsEcrLiteSystem() == FALSE )
			{
				if ( m_Filter.GetPluFilterFlag() == SCOPE_DBASE )			AddAction( NODE_PLUFILTER_NAME, "Plu Report Filters" );
				if ( m_Filter.GetDeptSetFlag() == SCOPE_DBASE )				AddAction( NODE_DEPARTMENT_SET, "Department Sets" );
				if ( m_Filter.GetCategorySetFlag() == SCOPE_DBASE )			AddAction( NODE_CATEGORY_SET, "Category Sets" );
				if ( m_Filter.GetCustomFieldSetFlag() == SCOPE_DBASE )		AddAction( NODE_CUSTOM_FIELD_SET, "Custom Field Sets" );
				if ( m_Filter.GetTaxFlag() == SCOPE_DBASE )					AddAction( NODE_TAX_DATE, EcrmanOptions.GetTaxString( "Historical %T rates" ) );
			}
			m_buttonAction2.EnableWindow( m_bCanCloneDatabase );
		}
		break;
	}

	ForceSelectAction( nAction );
}

/**********************************************************************/

void CPropPageSystemDataList::GetReportSetAction( int& nDbNo, int& nAction )
{
	nDbNo = 0;
	nAction = 0;

	{
		int nSel = m_listLocation.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_arrayLocation.GetSize() ) )
		{
			CLocationSelectorEntity LocSelEntity = m_arrayLocation.GetAt( nSel );

			if ( LocSelEntity.GetConType() == NODE_SYSTEM )
				nDbNo = 0;
			else
				nDbNo = LocSelEntity.GetEntityNo();
		}
	}

	{
		int nSel = m_listAction.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_arrayAction.GetSize() ) )
			nAction = m_arrayAction.GetAt(nSel);
	}
}

/**********************************************************************/

void CPropPageSystemDataList::SelectActionReportSet()
{
}

/**********************************************************************/

void CPropPageSystemDataList::Button1ReportSetPage() 
{
	int nDbNo, nNodeType, nConType;
	GetEcrDataAction( nDbNo, nNodeType, nConType );

	switch ( nNodeType )
	{
	case NODE_DEPARTMENT_SET:	m_pHelpers -> m_ReportSet.EditDeptSet ( nDbNo );			break;
	case NODE_PLUFILTER_NAME:	m_pHelpers -> m_ReportSet.EditPluFilter ( nDbNo );			break;
	case NODE_TAX_DATE:			m_pHelpers -> m_ReportSet.EditHistoricalTax( nDbNo );		break;
	case NODE_BATCH_TERMINAL:	m_pHelpers -> m_ReportSet.EditBatchReports();				break;
	case NODE_SESSION:			m_pHelpers -> m_ReportSet.EditSession();					break;
	case NODE_HISTORY_PLU:		m_pHelpers -> m_ReportSet.EditCustomPlu();					break;
	case NODE_TIMESLICE:		m_pHelpers -> m_ReportSet.EditTimeSliceMaps();				break;
	case NODE_EPOS_REPORT:		m_pHelpers -> m_ReportSet.EditEposReports();				break;
#ifdef STOCKMAN_SYSTEM
	case NODE_HISTORY_STOCK:	m_pHelpers -> m_ReportSet.EditCustomStock();				break;
	case NODE_STOCK_REPORT:		m_pHelpers -> m_ReportSet.EditStockLevelReports();			break;
	case NODE_CATEGORY_SET:		m_pHelpers -> m_ReportSet.EditCategorySet ( nDbNo );		break;
	case NODE_CUSTOM_FIELD_SET:	m_pHelpers -> m_ReportSet.EditCustomFieldSet ( nDbNo );		break;
	case NODE_SUPPLIER_SET:		m_pHelpers -> m_ReportSet.EditSupplierSet();				break;
#else
	case NODE_ECR_SUPPLIER:		m_pHelpers -> m_ReportSet.EditEcrSupplier();				break;
#endif
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button2ReportSetPage() 
{
	int nDbNo, nAction, nConType;
	GetEcrDataAction( nDbNo, nAction, nConType );
	
	if ( nDbNo != 0 )
		CloneDatabase( nDbNo );
}

/**********************************************************************/
