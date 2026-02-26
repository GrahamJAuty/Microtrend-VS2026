#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "ZDeliveryImporterStockman.h"
/**********************************************************************/

class CZDeliveryRedirectorStockman : public CZDeliveryImporterStockman
{
public:
	CZDeliveryRedirectorStockman();
	void LookForStockpointDeliveries( int nSpIdx );
	
private:
	void ProcessStockpointPendingFiles();
	bool ProcessStockpointPendingFile( CString& strFilename );
};

/**********************************************************************/
#endif
/**********************************************************************/
