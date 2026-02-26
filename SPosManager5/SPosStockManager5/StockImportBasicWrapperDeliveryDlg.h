#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StocktakeCSVArray.h"
#include "SuppRefFinder.h"
/**********************************************************************/
#include "StockImportBasicWrapperDlg.h"
/**********************************************************************/

class CStockImportBasicWrapperDeliveryDlg : public CStockImportBasicWrapperDlg
{
public:
	CStockImportBasicWrapperDeliveryDlg ( int nSpIdx, int nSuppIdx, const char* szPath, const char* szReference1, const char* szReference2, CSuppRefFinder& SuppRefFinder, CStocktakeCSVRecord* pStocktake, CWnd* pParent );
	
private:
	virtual void AuditStockChanges ( CSSFile& file, CSSFile& fileTempReport, CCSV& csvOriginalData, int nStockIdx, int nStockSuppIdx, double dDUQty, double dTotalCost, int nAction );
	virtual void FillActionCombo();
	virtual void ShowStockChanges();

private:
	int m_nSuppIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/

