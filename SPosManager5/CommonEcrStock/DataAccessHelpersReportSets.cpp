/**********************************************************************/
#include "EposReportSelectNewDlg.h"
#include "FileRemover.h"
#include "ListDataBatchRepDlg.h"
#include "..\SPosStockManager5\ListDataCategorySetDlg.h"
#include "..\SPosStockManager5\ListDataCustomFieldSetDlg.h"
#include "ListDataDepartmentSetDlg.h"
#include "..\SPosEcrManager5\ListDataEcrmanSupplierDlg.h"
#include "ListDataHistoricalTaxDlg.h"
#include "ListDataPluFilterDlg.h"
#include "ListDataSalesHistoryPluDlg.h"
#include "..\SPosStockManager5\ListDataSalesHistoryStockDlg.h"
#include "ListDataSessionDlg.h"
#include "..\SPosStockManager5\ListDataSupplierSetDlg.h"
#include "..\SPosStockManager5\ListDataSystemCustomFieldSetDlg.h"
#include "ListDataTimeSliceMapDlg.h"
#include "NodeTypes.h"
#include "PresetReportManager.h"
#include "..\SPosStockManager5\StockOptionsIni.h"
#include "..\SPosStockManager5\StockReportSelectDlg.h"
/**********************************************************************/
#include "DataAccessHelpersReportSets.h"
/**********************************************************************/

CDataAccessHelpersReportSets::CDataAccessHelpersReportSets()
{
}

/**********************************************************************/

void CDataAccessHelpersReportSets::EditDeptSet ( int nDbNo )
{
	if ( ( 0 == nDbNo ) && ( EcrmanOptions.GetGlobalDepartmentFlag() == FALSE ) )
	{
		CDataManagerInfo info;
		
		DataManagerNonDb.OpenSystemDepartment( DB_READONLY, info );
		
		if ( DataManagerNonDb.OpenSystemDepartmentSet( DB_READWRITE, info ) == FALSE )  
		{
			Prompter.ShareError( info );
			DataManagerNonDb.CloseSystemDepartment(info);
			return;
		}
		
		CListDataDepartmentSetDlg dlg ( TRUE, m_pParentWnd );
				
		if (dlg.DoModal() == IDOK)
		{
			if (DataManagerNonDb.WriteSystemDepartmentSet(info) == FALSE)
			{
				Prompter.WriteError(info);
			}
		}

		DataManagerNonDb.CloseSystemDepartment(info);
		DataManagerNonDb.CloseSystemDepartmentSet(info);
	}
	else
	{
		int nDbIdx = GetDbIdx ( nDbNo );
		if ( nDbIdx == -1 ) return;

		CWordArray arrayReadWrite;
		arrayReadWrite.Add( NODE_DEPARTMENT_SET );

		CWordArray arrayReadOnly;
		arrayReadOnly.Add( NODE_DEPARTMENT );

		CDataManagerInfo info;
		if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE ) == FALSE )
		{
			Prompter.ShareError( info );
			return;
		}
			
		CListDataDepartmentSetDlg dlg ( FALSE, m_pParentWnd );
			
		if ( dlg.DoModal() == IDOK )
		{
			if ( DataManager.WriteDepartmentSet( info ) == FALSE )
				Prompter.WriteError( info );
		}

		DataManager.CloseDatabase();
	}
}

/**********************************************************************/

void CDataAccessHelpersReportSets::EditHistoricalTax ( int nDbNo )
{
	int nDbIdx = GetDbIdx ( nDbNo );
	if ( nDbIdx == -1 ) return;

	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_TAX_DATE );

	CWordArray arrayReadOnly;
	arrayReadOnly.Add( NODE_TAX );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	DataManager.HistoricalTaxRates.BuildTaxDateInfoArray();

	CListDataHistoricalTaxDlg dlg( m_pParentWnd );
	
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManager.WriteHistoricalTax( info ) == FALSE )
			Prompter.WriteError( info );
	}

	DataManager.CloseDatabase();
}

/**********************************************************************/

void CDataAccessHelpersReportSets::EditPluFilter( int nDbNo )
{
	if ( nDbNo != 0 )
	{
		int	nDbIdx = GetDbIdx ( nDbNo );
		if ( nDbIdx == -1 ) 
			return;
		
		CWordArray arrayReadWrite;
		arrayReadWrite.Add( NODE_PLUFILTER_NAME );
		arrayReadWrite.Add( NODE_PLUFILTER_ITEM );

		CWordArray arrayReadOnly;
		arrayReadOnly.Add( NODE_PLU );
		arrayReadOnly.Add( NODE_MODIFIER );
		arrayReadOnly.Add( NODE_DEPARTMENT );
		
		CDataManagerInfo info;
		if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE ) == FALSE )
		{
			Prompter.ShareError( info );
			return;
		}
				
		CListDataPluFilterDlg dlg ( FALSE, m_pParentWnd );
				
		if ( dlg.DoModal() == IDOK )
		{
			if ( DataManager.WriteDbPluFilterNames( info ) == FALSE )
				Prompter.WriteError( info );
			else if ( DataManager.WriteDbPluFilterItems( info ) == FALSE )
				Prompter.WriteError( info );
		}

		DataManager.CloseDatabase();
	}
	else
	{
		CDataManagerInfo info;
		
		if ( DataManagerNonDb.OpenSystemPluFilterNames( DB_READWRITE, info ) == FALSE )  
		{
			Prompter.ShareError( info );
			return;
		}

		if ( DataManagerNonDb.OpenSystemPluFilterItems( DB_READWRITE, info ) == FALSE )  
		{
			DataManagerNonDb.CloseSystemPluFilterNames(info);
			Prompter.ShareError( info );
			return;
		}
				
		CListDataPluFilterDlg dlg ( TRUE, m_pParentWnd );
				
		if ( dlg.DoModal() == IDOK )
		{
			if ( DataManagerNonDb.WriteSystemPluFilterNames(info) == FALSE )
				Prompter.WriteError( info );
			else if ( DataManagerNonDb.WriteSystemPluFilterItems(info) == FALSE )
				Prompter.WriteError( info );
		}

		DataManagerNonDb.CloseSystemPluFilterNames(info);
		DataManagerNonDb.CloseSystemPluFilterItems(info);
	}
}

/**********************************************************************/

void CDataAccessHelpersReportSets::EditEposReports()
{
	CDataManagerInfo info;
	DataManagerNonDb.OpenTimeSliceList( DB_READONLY, info );
	CEposReportSelectNewDlg dlg( m_pParentWnd );
	dlg.DoModal();
	DataManagerNonDb.CloseTimeSliceList( info );
}

/**********************************************************************/

void CDataAccessHelpersReportSets::EditCustomPlu()
{
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenSalesHistoryPlu( DB_READWRITE, info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CListDataSalesHistoryPluDlg dlg ( 0, m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManagerNonDb.WriteSalesHistoryPlu( info ) == FALSE )
			Prompter.WriteError( info );
	}

	DataManagerNonDb.CloseSalesHistoryPlu( info );
}

/**********************************************************************/

void CDataAccessHelpersReportSets::EditBatchReports()
{
	CDataManagerInfo info;
	if ( PresetReportManager.Open( NODE_PRESET_EPOS_BATCH, DB_READWRITE, info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
	
	PresetReportManager.EposBatch.GetNextFreeEntityNo();
	CListDataBatchRepDlg dlg ( m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		if ( PresetReportManager.Write( NODE_PRESET_EPOS_BATCH, info ) == FALSE )
			Prompter.WriteError( info );
	}

	PresetReportManager.Close( NODE_PRESET_EPOS_BATCH, info );
}

/**********************************************************************/

void CDataAccessHelpersReportSets::EditSession()
{
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenReportSession( DB_READWRITE, info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CListDataSessionDlg dlg ( m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManagerNonDb.WriteReportSession( info ) == FALSE )
			Prompter.WriteError( info );
	}

	DataManagerNonDb.CloseReportSession( info );
}

/**********************************************************************/

void CDataAccessHelpersReportSets::EditTimeSliceMaps()
{
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenTimeSliceList( DB_READWRITE, info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	DataManagerNonDb.OpenReportSession( DB_READONLY, info );
	
	int nChangedMaps = 0;
	CListDataTimeSliceMapDlg dlg ( 0, nChangedMaps, m_pParentWnd );
	
	for ( int n = 1; n <= TimeSliceMap::FileNo.Max; n++ )
	{
		CFilenameUpdater FnUp1( SysFiles::TimeSliceDetail, -1, n );
		CString strFilename1 = FnUp1.GetFilenameToUse();

		CFilenameUpdater FnUp2( SysFiles::TimeSliceBackup, -1, n );
		CString strFilename2 = FnUp2.GetFilenameToUse();

		if ( ::FileExists( strFilename1 ) == FALSE )
			CFileRemover( strFilename2 );
		else
			::CopyFile( strFilename1, strFilename2, FALSE );
	}

	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManagerNonDb.WriteTimeSliceList( info ) == FALSE )
			Prompter.WriteError( info );
	}
	else
	{
		int nMask = 1;

		for ( int n = 1; n <= TimeSliceMap::FileNo.Max; n++ )
		{
			if ( ( nChangedMaps & nMask ) != 0 )
			{
				CFilenameUpdater FnUp1( SysFiles::TimeSliceBackup, -1, n );
				CString strFilename1 = FnUp1.GetFilenameToUse();

				CFilenameUpdater FnUp2( SysFiles::TimeSliceDetail, -1, n );
				CString strFilename2 = FnUp2.GetFilenameToUse();

				if ( ::FileExists( strFilename1 ) == FALSE )
					CFileRemover( strFilename2 );
				else
					::CopyFile( strFilename1, strFilename2, FALSE );
			}

			nMask *= 2;
		}
	}

	DataManagerNonDb.CloseReportSession( info );
	DataManagerNonDb.CloseTimeSliceList( info );

}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

void CDataAccessHelpersReportSets::EditCategorySet ( int nDbNo )
{
	if ( ( 0 == nDbNo ) && ( StockOptions.GetGlobalCategoryFlag() == FALSE ) )
	{
		CDataManagerInfo info;
		
		DataManagerNonDb.OpenSystemCategory( DB_READONLY, info );
		
		if ( DataManagerNonDb.OpenSystemCategorySet( DB_READWRITE, info ) == FALSE )  
		{
			Prompter.ShareError( info );
			DataManagerNonDb.CloseSystemCategory(info);
			return;
		}
		
		CListDataCategorySetDlg dlg ( TRUE, m_pParentWnd );
				
		if ( dlg.DoModal() == IDOK )
		{
			if ( DataManagerNonDb.WriteSystemCategorySet(info) == FALSE )
				Prompter.WriteError( info );
		}

		DataManagerNonDb.CloseSystemCategory(info);
		DataManagerNonDb.CloseSystemCategorySet(info);
	}
	else
	{
		int nDbIdx = GetDbIdx ( nDbNo );
		if ( nDbIdx == -1 ) return;

		CWordArray arrayReadWrite;
		arrayReadWrite.Add( NODE_CATEGORY_SET );

		CWordArray arrayReadOnly;
		arrayReadOnly.Add( NODE_CATEGORY );

		CDataManagerInfo info;
		if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE ) == FALSE )
		{
			Prompter.ShareError( info );
			return;
		}
			
		CListDataCategorySetDlg dlg ( FALSE, m_pParentWnd );
			
		if ( dlg.DoModal() == IDOK )
		{
			if ( DataManager.WriteCategorySet( info ) == FALSE )
				Prompter.WriteError( info );
		}

		DataManager.CloseDatabase();
	}
}

/**********************************************************************/

void CDataAccessHelpersReportSets::EditCustomFieldSet ( int nDbNo )
{
	if ( ( 0 == nDbNo ) && ( StockOptions.GetGlobalCustomFieldFlag() == FALSE ) )
	{
		CDataManagerInfo info;
		if ( DataManagerNonDb.OpenCustomFieldNames( DB_READWRITE, info ) == FALSE )
		{
			Prompter.ShareError( info );
		}
		else
		{	
			CListDataSystemCustomFieldSetDlg dlg ( m_pParentWnd );
			dlg.DoModal();	

			DataManagerNonDb.CloseCustomFieldNames( info );
		}
	}
	else
	{
		int nDbIdx = GetDbIdx ( nDbNo );
		if ( nDbIdx == -1 ) return;

		CWordArray arrayReadOnly;
		arrayReadOnly.Add( NODE_CUSTOM_FIELD );
		arrayReadOnly.Add( NODE_CUSTOM_FIELD_SET );

		CWordArray arrayDummy;

		CDataManagerInfo info;
		if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayDummy, arrayReadOnly, info, FALSE ) == FALSE )
		{
			Prompter.ShareError( info );
			return;
		}

		if ( DataManagerNonDb.OpenCustomFieldNames( DB_READWRITE, info ) == FALSE )
		{
			DataManager.CloseDatabase();	
			Prompter.ShareError( info );
		}
		else
		{	
			CListDataCustomFieldSetDlg dlg ( m_pParentWnd );
			dlg.DoModal();	

			DataManagerNonDb.CloseCustomFieldNames( info );
		}
	}
}

/**********************************************************************/

void CDataAccessHelpersReportSets::EditSupplierSet()
{
	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_SUPPLIER_SET );

	CWordArray arrayReadOnly;
	arrayReadOnly.Add( NODE_SUPPLIER );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( 0, arrayReadWrite, arrayReadOnly, info, FALSE ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CListDataSupplierSetDlg dlg ( m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManager.WriteSupplierSet( info ) == FALSE )
			Prompter.WriteError( info );
	}

	DataManager.CloseDatabase();
}

/**********************************************************************/

void CDataAccessHelpersReportSets::EditCustomStock()
{
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenSalesHistoryStock( DB_READWRITE, info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CListDataSalesHistoryStockDlg dlg ( 0, m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManagerNonDb.WriteSalesHistoryStock( info ) == FALSE )
			Prompter.WriteError( info );
	}

	DataManagerNonDb.CloseSalesHistoryStock( info );
}

/**********************************************************************/

void CDataAccessHelpersReportSets::EditStockLevelReports()
{
	CStockReportSelectDlg dlg( m_pParentWnd );
	dlg.DoModal();
}

/**********************************************************************/
#else
/**********************************************************************/

void CDataAccessHelpersReportSets::EditEcrSupplier()
{
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenEcrmanSupplier( DB_READWRITE, info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}
		
	CListDataEcrmanSupplierDlg dlg ( 0, m_pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManagerNonDb.WriteEcrmanSupplier( info ) == FALSE )
			Prompter.WriteError( info );
	}

	DataManagerNonDb.CloseEcrmanSupplier( info );
}

/**********************************************************************/
#endif
/**********************************************************************/
