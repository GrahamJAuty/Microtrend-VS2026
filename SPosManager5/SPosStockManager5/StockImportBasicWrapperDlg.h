#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockImportBasicDlg.h"
#include "SuppRefFinder.h"
/**********************************************************************/

class CStockImportBasicWrapperDlg : public CStockImportBasicDlg
{
public:
	CStockImportBasicWrapperDlg( int nSpIdx, const char* szPath, CSuppRefFinder& SuppRefFinder, CWnd* pParent );
	
protected:
	virtual void AuditStockChanges ( CSSFile& file, CSSFile& fileTempReport, CCSV& csvOriginalData, int nStockIdx, int nStockSuppIdx, double dDUQty, double dTotalCost, int nAction ){};
	virtual void AuditExceptions ( CSSFile& file, CSSFile& fileTempReport, CCSV& csvOriginalData, int nReason, const char* szStockCode );
	
	virtual void ShowStockChanges(){}
	virtual void ShowImportFile();
	virtual void FillActionCombo() = 0;

protected:
	virtual void ProcessImportDataInternal( int nAction );
	virtual int CheckStockLevelStatus( int nStockIdx );

protected:
	CSuppRefFinder& m_SuppRefFinder;
	int m_nSuppNo;
};

/**********************************************************************/
#endif
/**********************************************************************/

