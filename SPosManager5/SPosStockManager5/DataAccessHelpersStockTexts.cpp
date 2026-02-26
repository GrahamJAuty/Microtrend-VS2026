/**********************************************************************/
#include "ListDataAllowanceDlg.h"
#include "ListDataCategoryDlg.h"
#include "ListDataCustomFieldDlg.h"
#include "ListDataDelUnitDlg.h"
#include "ListDataSupplierDlg.h"
#include "ListDataSystemCategoryDlg.h"
#include "..\CommonEcrStock\ListDataUnitDlg.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "DataAccessHelpersStockTexts.h"
/**********************************************************************/

CDataAccessHelpersStockTexts::CDataAccessHelpersStockTexts()
{
}

/**********************************************************************/

void CDataAccessHelpersStockTexts::EditCategory( int nDbNo )
{
	if ( ( 0 == nDbNo ) && ( StockOptions.GetGlobalCategoryFlag() == FALSE ) )
	{
		CDataManagerInfo info;
		
		if ( DataManagerNonDb.OpenSystemCategory( DB_READWRITE, info ) == FALSE )  
		{
			Prompter.ShareError( info );
			return;
		}
		
		CListDataSystemCategoryDlg dlg ( 0, m_pParentWnd );
				
		if ( dlg.DoModal() == IDOK )
		{
			if ( DataManagerNonDb.WriteSystemCategory(info) == FALSE )
				Prompter.WriteError( info );
		}

		DataManagerNonDb.CloseSystemCategory(info);
	}
	else
	{
		int nDbIdx = GetDbIdx ( nDbNo );
		if ( nDbIdx == -1 ) return;

		CWordArray arrayReadWrite;
		arrayReadWrite.Add( NODE_CATEGORY );

		CWordArray arrayReadOnly;
		arrayReadOnly.Add( NODE_DEPARTMENT );
		arrayReadOnly.Add( NODE_TAX );

		CDataManagerInfo info;
		if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE ) == FALSE )
		{
			Prompter.ShareError( info );
			return;
		}
			
		CListDataCategoryDlg dlg ( 0, m_pParentWnd );
			
		if ( dlg.DoModal() == IDOK )
		{
			if ( DataManager.WriteCategory( info ) == FALSE )
				Prompter.WriteError( info );
		}
		
		DataManager.CloseDatabase();
	}
}

/**********************************************************************/

void CDataAccessHelpersStockTexts::EditCustomField( int nDbNo )
{
	int nDbIdx = GetDbIdx ( nDbNo );
	if ( nDbIdx == -1 ) return;

	CWordArray arrayReadOnly;
	arrayReadOnly.Add( NODE_CUSTOM_FIELD );

	CWordArray arrayDummy;

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayDummy, arrayReadOnly, info, FALSE ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	CDataManagerInfo info2;
	if ( DataManagerNonDb.OpenCustomFieldNames( DB_READWRITE, info2 ) == FALSE )
	{
		DataManager.CloseDatabase();	
		Prompter.ShareError( info2 );
	}
	else
	{	
		CListDataCustomFieldDlg dlg ( m_pParentWnd );
		dlg.DoModal();	

		DataManagerNonDb.CloseCustomFieldNames( info2 );
	}
}

/**********************************************************************/

void CDataAccessHelpersStockTexts::EditAllowance( int nDbNo )
{
	int nDbIdx = GetDbIdx ( nDbNo );
	if ( nDbIdx == -1 ) return;

	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_ALLOWANCE );

	CWordArray arrayReadOnly;
	arrayReadOnly.Add( NODE_TAX );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CListDataAllowanceDlg dlg ( m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManager.WriteAllowance( info ) == FALSE )
			Prompter.WriteError( info );
	}
	
	DataManager.CloseDatabase();
}

/**********************************************************************/

void CDataAccessHelpersStockTexts::EditSupplier()
{
	CWordArray arrayReadWrite, arrayDummy;
	arrayReadWrite.Add( NODE_SUPPLIER );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( 0, arrayReadWrite, arrayDummy, info, FALSE ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	if ( DataManager.Supplier.IsValid() == FALSE )
		Prompter.SupplierError();
	else
	{	
		CListDataSupplierDlg dlg ( 0, m_pParentWnd );
		
		if ( dlg.DoModal() == IDOK )
		{
			if ( DataManager.WriteSupplier( info ) == FALSE )
				Prompter.WriteError( info );
		}
	}

	DataManager.CloseDatabase();
}

/**********************************************************************/

void CDataAccessHelpersStockTexts::EditStockUnit()
{
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenStockUnit( DB_READWRITE, info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CListDataUnitDlg dlg ( DataManagerNonDb.StockUnit, "", m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManagerNonDb.WriteStockUnit( info ) == FALSE )
			Prompter.WriteError( info );
	}
	
	DataManagerNonDb.CloseStockUnit( info );
}

/**********************************************************************/

void CDataAccessHelpersStockTexts::EditSubUnit()
{
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenSubUnit( DB_READWRITE, info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CListDataUnitDlg dlg ( DataManagerNonDb.SubUnit, "", m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManagerNonDb.WriteSubUnit( info ) == FALSE )
			Prompter.WriteError( info );
	}
	
	DataManagerNonDb.CloseSubUnit( info );
}

/**********************************************************************/

void CDataAccessHelpersStockTexts::EditAdjustmentTexts()
{
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenAdjustText( DB_READWRITE, info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CListDataUnitDlg dlg ( DataManagerNonDb.AdjustText, "", m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManagerNonDb.WriteAdjustText( info ) == FALSE )
			Prompter.WriteError( info );
	}
	
	DataManagerNonDb.CloseAdjustText( info );
}

/**********************************************************************/

void CDataAccessHelpersStockTexts::EditDeliveryUnit()
{
	CWordArray arrayReadWrite, arrayDummy;
	arrayReadWrite.Add( NODE_DELUNIT );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( 0, arrayReadWrite, arrayDummy, info, FALSE ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CListDataDelUnitDlg dlg ( "", 0, 0.0, m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManager.WriteDelUnit( info ) == FALSE )
			Prompter.WriteError( info );
	}
	
	DataManager.CloseDatabase();
}

/**********************************************************************/


