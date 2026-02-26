#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StocktakeCSVArray.h"
/**********************************************************************/
#include "StockImportBasicDlg.h"
/**********************************************************************/

class CStockImportBasicAdjustmentStockDlg : public CStockImportBasicDlg
{
public:
	CStockImportBasicAdjustmentStockDlg ( int nSpIdx, const char* szPath, const char* szReference1, const char* szReference2, const char* szReason, CStocktakeCSVRecord* pStocktake, CWnd* pParent );
	virtual void AuditStockChanges ( CSSFile& file, CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData, int nAction );
	virtual void ShowStockChanges();
	virtual void FillActionCombo();

private:
	CString m_strReason;
};

/**********************************************************************/
#endif
/**********************************************************************/

