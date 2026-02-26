/**********************************************************************/
#include "..\CommonEcrStock\CloneDatabaseDlg.h"
#include "Password.h"
/**********************************************************************/
#include "..\CommonEcrStock\PropPageSystemDataList.h"
/**********************************************************************/

void CPropPageSystemDataList::PrepareStockTextPage()
{	
	m_Filter.PrepareStockTextFilter();
}

/**********************************************************************/

void CPropPageSystemDataList::BuildStockTextList()
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
	}
		
	m_buttonAction1.SetWindowText ( "Edit" );
	m_buttonAction2.SetWindowText ( "Clone" );
	m_buttonAction3.ShowWindow ( SW_HIDE );
	m_buttonAction4.ShowWindow ( SW_HIDE );
	m_buttonAction5.ShowWindow ( SW_HIDE );
	m_buttonAction6.ShowWindow ( SW_HIDE );
	
	if ( CheckCloneDatabase() == FALSE )
		m_buttonAction2.ShowWindow( SW_HIDE );

	SelectLine( m_listLocation, 0 );
}

/**********************************************************************/

void CPropPageSystemDataList::SelectLocationStockText()
{
	int nDbNo, nAction, nConType;
	GetStockTextAction( nDbNo, nAction, nConType );

	m_arrayAction.RemoveAll();
	m_listAction.DeleteAllItems();

	if ( NODE_SYSTEM_FILTER == nConType )
	{
		if ( m_Filter.GetCategoryFlag() == SCOPE_DBASE )		AddAction( NODE_CATEGORY, "System Categories" );
		m_buttonAction2.EnableWindow( FALSE );
	}
	else if ( 0 == nDbNo )
	{
		if ( m_Filter.GetSupplierFlag() == SCOPE_GLOBAL )		AddAction( NODE_SUPPLIER, "Suppliers" );		
		if ( m_Filter.GetCategoryFlag() == SCOPE_GLOBAL )		AddAction( NODE_CATEGORY, "Categories" );
		if ( m_Filter.GetCustomFieldFlag() == SCOPE_GLOBAL )	AddAction( NODE_CUSTOM_FIELD, "Custom Fields" );
		if ( m_Filter.GetAllowanceFlag() == SCOPE_GLOBAL )		AddAction( NODE_ALLOWANCE, "Allowances" );
		if ( m_Filter.GetStockUnitFlag() == SCOPE_GLOBAL )		AddAction( NODE_STOCKUNIT, "Stock Units" );
		if ( m_Filter.GetSubUnitFlag() == SCOPE_GLOBAL )		AddAction( NODE_SUBUNIT, "Sub Units" );
		if ( m_Filter.GetDelUnitFlag() == SCOPE_GLOBAL )		AddAction( NODE_DELUNIT, "Delivery Units" );
		if ( m_Filter.GetAdjustTextFlag() == SCOPE_GLOBAL )		AddAction( NODE_ADJUSTTEXT, "Adjustment Texts" );
		m_buttonAction2.EnableWindow( FALSE );
	}
	else
	{
		if ( m_Filter.GetCategoryFlag() == SCOPE_DBASE )		AddAction( NODE_CATEGORY, "Categories" );
		if ( m_Filter.GetCustomFieldFlag() == SCOPE_DBASE )		AddAction( NODE_CUSTOM_FIELD, "Custom Fields" );
		if ( m_Filter.GetAllowanceFlag() == SCOPE_DBASE )		AddAction( NODE_ALLOWANCE, "Allowances" );
		m_buttonAction2.EnableWindow( m_bCanCloneDatabase );
	}

	ForceSelectAction( nAction );
}

/**********************************************************************/

void CPropPageSystemDataList::GetStockTextAction( int& nDbNo, int& nAction, int& nConType )
{
	nDbNo = 0;
	nAction = 0;
	nConType = 0;

	{
		int nSel = m_listLocation.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_arrayLocation.GetSize() ) )
		{
			CLocationSelectorEntity LocSelEntity = m_arrayLocation.GetAt( nSel );

			nConType = LocSelEntity.GetConType();

			switch( nConType )
			{
			case NODE_SYSTEM:
			case NODE_SYSTEM_FILTER:
				nDbNo = 0;
				break;

			default:
				nDbNo = LocSelEntity.GetEntityNo();
				break;
			}
		}
	}

	{
		int nSel = m_listAction.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_arrayAction.GetSize() ) )
			nAction = m_arrayAction.GetAt(nSel);
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button1StockTextPage()
{
	int nDbNo, nAction, nConType;
	GetStockTextAction( nDbNo, nAction, nConType );

	switch ( nAction )
	{
	case NODE_CATEGORY:		m_pHelpers -> m_StockText.EditCategory ( nDbNo );		break;
	case NODE_CUSTOM_FIELD:	m_pHelpers -> m_StockText.EditCustomField ( nDbNo );	break;
	case NODE_ALLOWANCE:	m_pHelpers -> m_StockText.EditAllowance ( nDbNo );	break;
	case NODE_SUPPLIER:		m_pHelpers -> m_StockText.EditSupplier();				break;
	case NODE_STOCKUNIT:	m_pHelpers -> m_StockText.EditStockUnit();			break;
	case NODE_SUBUNIT:		m_pHelpers -> m_StockText.EditSubUnit();				break;
	case NODE_ADJUSTTEXT:	m_pHelpers -> m_StockText.EditAdjustmentTexts();		break;
	case NODE_DELUNIT:		m_pHelpers -> m_StockText.EditDeliveryUnit();			break;
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button2StockTextPage()
{
	int nDbNo, nAction, nConType;
	GetStockTextAction( nDbNo, nAction, nConType );

	if ( nDbNo != 0 )
		CloneDatabase( nDbNo );	
}

/**********************************************************************/
