#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "ZSalesImporterStockman.h"
/**********************************************************************/

class CZSalesRedirectorStockman : public CZSalesImporterStockman
{
public:
	CZSalesRedirectorStockman();
	void LookForStockpointSales( int nSpIdx, bool bPMS );
	
private:
	void ProcessStockpointPendingFiles( bool bPMS );
	bool ProcessStockpointPendingFile( CString& strFilename, bool bPMS );
};

/**********************************************************************/
#endif
/**********************************************************************/
