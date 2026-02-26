#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StocktakeCSVArray.h"
#include "SuppRefFinder.h"
/**********************************************************************/
#include "StockImportBasicWrapperDlg.h"
/**********************************************************************/

class CStockImportBasicWrapperSalesDlg : public CStockImportBasicWrapperDlg
{
public:
	CStockImportBasicWrapperSalesDlg( int nSpIdx, const char* szPath, CSuppRefFinder& SuppRefFinder, CStocktakeCSVRecord* pStocktake, CWnd* pParent );
	
private:
	virtual void AuditStockChanges ( CSSFile& file, CSSFile& fileTempReport, CCSV& csvOriginalData, int nStockIdx, int nStockSuppIdx, double dDUQty, double dTotalRetail, int nAction );
	virtual void AuditExceptions ( CSSFile& file, CSSFile& fileTempReport, CCSV& csvOriginalData, int nReason, const char* szStockCode );
	virtual void FillActionCombo();
	virtual void ShowStockChanges();
};

/**********************************************************************/
#endif
/**********************************************************************/

