#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StocktakeCSVArray.h"
/**********************************************************************/
#include "StockImportBasicDlg.h"
/**********************************************************************/

class CStockImportBasicDeliveryDlg : public CStockImportBasicDlg
{
public:
	CStockImportBasicDeliveryDlg( int nSpIdx, int nSuppIdx, const char* szPath, const char* szReference1, const char* szReference2, CStocktakeCSVRecord* pStocktake, CWnd* pParent );
	virtual void AuditStockChanges ( CSSFile& file, CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData, int nAction );
	virtual void ShowStockChanges();
	virtual void FillActionCombo();

private:
	int m_nSuppIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/

