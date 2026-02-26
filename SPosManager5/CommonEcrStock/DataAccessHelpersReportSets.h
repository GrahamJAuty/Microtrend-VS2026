#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "DataAccessHelpers.h"
/**********************************************************************/

class CDataAccessHelpersReportSets : public CDataAccessHelpers
{
public:
	CDataAccessHelpersReportSets();

public:
	void EditDeptSet ( int nDbNo );
	void EditHistoricalTax ( int nDbNo );
	void EditPluFilter ( int nDbNo );
	void EditEposReports();
	void EditCustomPlu();
	void EditBatchReports();
	void EditSession();
	void EditTimeSliceMaps();

#ifdef STOCKMAN_SYSTEM
	void EditCategorySet ( int nDbNo );
	void EditCustomFieldSet ( int nDbNo );
	void EditSupplierSet();
	void EditCustomStock();
	void EditStockLevelReports();
#else
	void EditEcrSupplier();
#endif
};

/**********************************************************************/
#endif
/**********************************************************************/
