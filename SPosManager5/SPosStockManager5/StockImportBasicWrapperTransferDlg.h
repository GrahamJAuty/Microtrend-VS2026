#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockImportBasicWrapperDlg.h"
#include "SuppRefFinder.h"
/**********************************************************************/

class CStockImportBasicWrapperTransferDlg : public CStockImportBasicWrapperDlg
{
public:
	CStockImportBasicWrapperTransferDlg( int nSpIdx, int nSpIdx2, const char* szPath, const char* szReference1, const char* szReference2, CSuppRefFinder& SuppRefFinder, CWnd* pParent );
	
private:
	virtual void AuditStockChanges ( CSSFile& file, CSSFile& fileTempReport, CCSV& csvOriginalData, int nStockIdx, int nStockSuppIdx, double dDUQty, double dTotalCost, int nAction );
	virtual void FillActionCombo();
	virtual void ShowStockChanges();

private:
	virtual int CheckStockLevelStatus( int nStockIdx );

private:
	int m_nSpIdx2;
};

/**********************************************************************/
#endif
/**********************************************************************/

