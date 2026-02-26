/**********************************************************************/
 
/**********************************************************************/
#include "..\CommonStockTray\StockCSVArray.h"
/**********************************************************************/

CStockItemSupplier::CStockItemSupplier()
{
	Clear();
}

/**********************************************************************/

void CStockItemSupplier::Clear()
{
	m_nSuppNo = 0;						//zero means unused
	m_strCase = "";
	m_nDUItems = 1;
	m_dDUItemSize = 1.0;
	m_dCost = 0.0;
	m_strSuppRef = "";
	m_strWrapper = "";
	m_dRetail = 0.0;
}

/**********************************************************************/
