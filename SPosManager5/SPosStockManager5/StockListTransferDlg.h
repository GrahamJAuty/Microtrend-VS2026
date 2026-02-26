/**********************************************************************/
#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SuppRefFinder.h"
/**********************************************************************/
#include "StockListDlg.h"
/**********************************************************************/

class CStockListTransferDlg : public CStockListDlg
{
public:
	CStockListTransferDlg( int nSpIdx, int nSpIdx2, const char* szReference1, const char* szReference2, CWnd* pParent = NULL);
	virtual void SetColumns();
	virtual void EditStockLevel();
	virtual void EditStockLevelRandom();
	virtual const char* GetTitle() { return "Stock Transfer"; }
	virtual const char* GetSpName();
	virtual void SetFilterArray();
	virtual void HandleCancel();
	virtual void PrepareMenu();
	virtual void HandleImport();
	virtual void HandleImportWrapper();
	virtual void HandlePrintSession();
	virtual void AppendAudit();

private:
	bool HandleImportInternal( const char* szImportPath, int nAction );
	bool HandleImportWrapperInternal( const char* szImportPath, int nAction );

private:
	CString m_strReference1;
	CString m_strReference2;
	int m_nSpIdx2;

private:
	CSuppRefFinder m_SuppRefFinder;
};

/**********************************************************************/
#endif
/**********************************************************************/
