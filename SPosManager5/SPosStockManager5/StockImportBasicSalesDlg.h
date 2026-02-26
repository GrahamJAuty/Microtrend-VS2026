#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StocktakeCSVArray.h"
/**********************************************************************/
#include "StockImportBasicDlg.h"
/**********************************************************************/

class CStockImportBasicSalesDlg : public CStockImportBasicDlg
{
public:
	CStockImportBasicSalesDlg ( int nSpIdx, const char* szPath, int nLineType, CStocktakeCSVRecord* pStocktake, CWnd* pParent );
	
private:
	virtual void GetImportLineInfo( CCSV& csvImport, CString& strPluNo, CCSV& csvOriginalData, double& dPluQty, double& dPluValue );
	virtual void AuditExceptions ( CSSFile& file, CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData );
	virtual void AuditStockChanges ( CSSFile& file, CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData, int nAction );
	virtual void AuditIgnored ( CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData );
	virtual void ShowStockChanges();
	virtual void ShowExceptions();
	virtual void ShowImportFile();
	virtual void FillActionCombo();

private:
	int m_nLineType;
};

/**********************************************************************/
#endif
/**********************************************************************/

