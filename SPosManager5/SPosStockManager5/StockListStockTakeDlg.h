#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockImportFilterArray.h"
#include "StockListDlg.h"
#include "StocktakeCSVArray.h"
#include "SuppRefFinder.h"
/**********************************************************************/

class CStockListStocktakeDlg : public CStockListDlg
{
public:
	CStockListStocktakeDlg( int nSpIdx, CStocktakeCSVRecord& Stocktake, CWnd* pParent = NULL);
	virtual void SetColumns();
	virtual void EditStockLevel();
	virtual void EditStockLevelRandom();
	virtual const char* GetTitle();
	virtual void SetFilterArray();
	virtual void HandleImport();
	virtual void HandleImportWrapper();
	virtual void HandleImportSheetFile();
	virtual void HandleImportSheetFolder();
	virtual void HandlePrintSession();
	virtual void PrepareMenu();
	virtual void AppendAudit();
	virtual void HandleCreateImportSheet();

public:
	int GetUpdatedItemCount() { return m_nUpdatedItemCount; }

private:
	bool HandleImportInternal ( CStockImportFilterArray& arrayImport, int nAction );
	bool HandleImportWrapperInternal ( CStockImportFilterArray& arrayImport, int nAction );

private:
	virtual void DisplayScreen( int nIndex );

private:
	int m_nUpdatedItemCount;
	CStocktakeCSVRecord& m_Stocktake;
	CString m_strTitle;

private:
	CSuppRefFinder m_SuppRefFinder;
};

/**********************************************************************/
#endif
/**********************************************************************/
