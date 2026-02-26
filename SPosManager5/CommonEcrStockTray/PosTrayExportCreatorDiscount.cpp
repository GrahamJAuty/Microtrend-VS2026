/**********************************************************************/
#include "FileRemover.h"
#include "PosTrayExportConsolidatorDiscount.h"
#include "PosTrayOptions.h"
#include "ReportHelpers.h"
/**********************************************************************/
#include "PosTrayExportCreatorDiscount.h"
/**********************************************************************/

CPosTrayExportCreatorDiscount::CPosTrayExportCreatorDiscount( CPosTrayTask& Task, bool& bQuitNow, void* pHostDialog, int nManualExportNo ) : CPosTrayExportCreatorBase( Task, bQuitNow, pHostDialog, nManualExportNo )
{
	m_strExportFileType = "discount";
	m_strEmailSubjectType = "Discounts";
	m_strInternalFolderName = "EXPORTDISCOUNT";
	m_bWantCustomerFile = TRUE;
}

/**********************************************************************/

__int64 CPosTrayExportCreatorDiscount::ProcessTaskInternal()
{
	__int64 nLineCount = 0;

	PrepareDatesAndFolders();
	
	if ( ::ExistSubdirectory( m_strExportFolderPending ) == FALSE )
		return -1;

	CTimeSliceMapCSVRecord TimeSliceRecord;
	PrepareSessionAndTimeSlice( TimeSliceRecord );

	CArray<int,int> arrayFields;
	for ( int n = 1; n <= POSTRAY_MAX_FIELDS_DISCOUNT; n++ )
		arrayFields.Add( m_Task.GetBatchExportFieldDiscount(n) );

	int nDbIdx = 0;
	int nDbNo = m_Task.GetBatchConDbNo();
	if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
		nDbIdx = 0;

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );

	int nExportDateType = m_Task.GetTaskExportDateType();
	if ( 0 == nExportDateType )
		nExportDateType = PosTrayOptions.GetExportDateType();

	CEposSelectArray SelectArray;
	CPosTrayExportConsolidatorDiscount ExportConsolidator( nExportDateType, SelectArray, m_bQuitNow, m_pHostDialog );
	ExportConsolidator.Reset( arrayFields, m_Task, m_oleDateFrom, m_oleDateTo, m_ReportSession, TimeSliceRecord );
	ExportConsolidator.SetSaleTimeFlag( m_Task.GetBatchSaleTimeFlag() );	
	
	if ( m_Task.GetBatchPluRangeFlag() == TRUE )
	{
		ExportConsolidator.SetPluRangeFlag( TRUE );
		ExportConsolidator.SetPluFrom( m_Task.GetBatchPluNoFrom() );
		ExportConsolidator.SetPluTo( m_Task.GetBatchPluNoTo() );
	}

	ExportConsolidator.SetCustomerFlag( m_Task.GetBatchCustomerOnlyFlag() );
	ExportConsolidator.SetVIPSalesFlag( m_Task.GetBatchVIPOnlyFlag() );

	{
		int nConType = m_Task.GetBatchConType();

		int nDepartmentFilter = m_Task.GetBatchDeptFilter();
		if ( 0 == nDepartmentFilter ) nDepartmentFilter = FIRST_DEPARTMENTSET_FILTER - 1;
		ExportConsolidator.m_ReportFilters.DeptFilter.SetConsolidationType( nConType );
		ExportConsolidator.m_ReportFilters.DeptFilter.SetDeptFilterNo( nDepartmentFilter );

		int nPluFilter = m_Task.GetBatchPluFilter();
		ExportConsolidator.m_ReportFilters.PluFilter.SetConsolidationType( nConType );
		ExportConsolidator.m_ReportFilters.PluFilter.SetMasterFilterNo( nPluFilter );
	}

	ExportConsolidator.SetDepartmentByItemFlag( m_Task.GetBatchDeptFilterType() == 1 );
	
	int nTaxFilter = TAXBAND_FILTER_ALL;

	{
		int nTaxFilterType = m_Task.GetBatchTaxBandFilter();

		if ((nTaxFilterType >= 1) && (nTaxFilterType <= MAX_TAX_BANDS + 1))
		{
			nTaxFilter = 1 << (nTaxFilterType - 1);
		}
		else if (999 == nTaxFilterType)
		{
			nTaxFilter = m_Task.GetBatchTaxBandCustom();
		}
	}

	ExportConsolidator.SetTaxFilter( nTaxFilter );

	int nPriceFilter = PRICELEVEL_FILTER_ALL;
	switch( m_Task.GetBatchPriceLevelFilter() )
	{
	case 1:		nPriceFilter = 1;		break;
	case 2:		nPriceFilter = 2;		break;
	case 3:		nPriceFilter = 4;		break;
	case 4:		nPriceFilter = 8;		break;
	case 5:		nPriceFilter = 16;		break;
	case 6:		nPriceFilter = 32;		break;
	case 7:		nPriceFilter = 64;		break;
	case 8:		nPriceFilter = 128;		break;
	case 9:		nPriceFilter = 256;		break;
	case 10:	nPriceFilter = 512;		break;
	case 999:	nPriceFilter = m_Task.GetBatchPriceLevelCustom(); break;
	}
	ExportConsolidator.SetPriceLevelFilter( nPriceFilter );

	int nModifierFilter = MODIFIER_FILTER_ALL;
	switch( m_Task.GetBatchModifierFilter() )
	{
	case 1:		nModifierFilter = 1;	break;
	case 2:		nModifierFilter = 2;	break;
	case 3:		nModifierFilter = 4;	break;
	case 4:		nModifierFilter = 8;	break;
	case 5:		nModifierFilter = 16;	break;
	case 6:		nModifierFilter = 32;	break;
	case 7:		nModifierFilter = 64;	break;
	case 8:		nModifierFilter = 128;	break;
	case 9:		nModifierFilter = 256;	break;
	case 10:	nModifierFilter = 512;	break;
	case 999:	nModifierFilter = m_Task.GetBatchModifierCustom(); break;
	}
	ExportConsolidator.SetModifierFilter( nModifierFilter );
	
	ExportConsolidator.SetOfferFilter( m_Task.GetBatchOfferFilter() ); 

	{
		CTableFilter TableFilter;
		TableFilter.m_nType = m_Task.GetBatchTableFilterType();
		TableFilter.m_nLevel = m_Task.GetBatchTableFilterLevel();
		TableFilter.m_nEntityNo = m_Task.GetBatchTableFilterEntityNo();
		ExportConsolidator.SetTableFilter( TableFilter );
	}

	if ( ExportConsolidator.PrepareFields() == TRUE )
	{
		ExportConsolidator.Consolidate();
		
		m_strTempFilepath = "";
		m_strTempFilepath += m_strExportFolderPending;
		m_strTempFilepath += "\\Discount.tmp";
		
		if ( FALSE == m_bQuitNow )
		{
			nLineCount = ExportConsolidator.CreateExport( m_strTempFilepath );

			if ( nLineCount >= 2 )
				if ( ProcessExport() == FALSE )
					nLineCount = -1;

			CFileRemover FileRemover( m_strTempFilepath );
		}
	}

	return nLineCount;
}

/**********************************************************************/
