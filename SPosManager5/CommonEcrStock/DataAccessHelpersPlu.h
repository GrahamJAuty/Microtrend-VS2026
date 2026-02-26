#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "DataAccessHelpers.h"
/**********************************************************************/

class CDataAccessHelpersPlu : public CDataAccessHelpers
{
public:
	CDataAccessHelpersPlu();
	
public:
	void EditPlu( int nDbNo, bool bFromButton );
	void EditPriceSet( int nNetworkNo, int nDbNo, int nLocNo, int nPriceSet, bool bFromButton );
	void AutoDeleteBarcode();
	void PrintShelfLabels( int nDbNo );
	void PrintShelfLabels( int nNetworkNo, int nDbNo, int nLocNo, int nPriceSet );
	void RunPluChangesTask( int nDbIdx );
	
#ifdef STOCKMAN_SYSTEM
	void AutoDeleteStock();
	void PrintBarcodeLabels( int nDbNo );
	void PrintBarcodeLabels( int nNetworkNo, int nDbNo, int nLocNo, int nPriceSet );
#endif

private:
	void EditPluInternal( int nDbIdx, bool bReadOnly );
	void EditPriceSetInternal( int nDbIdx, int nLocIdx, int nPriceSet, bool bReadOnly );
	void RunPluDatabase( int nDbIdx, bool bReadOnly, bool& bCanDoChangesTask, bool& bCanDeleteStock, bool& bCanDeleteBarcode );
};

/**********************************************************************/
#endif
/**********************************************************************/
