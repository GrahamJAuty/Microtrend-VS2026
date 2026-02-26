#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
//include "MySSListCtrl.h"
#include "ReorderImportHelper.h"
#include "StockListDlg.h"
#include "SuppRefFinder.h"
/**********************************************************************/

class CStockListReorderDlg : public CStockListDlg
{
public:
	CStockListReorderDlg( int nSpIdx, CWnd* pParent = NULL);
	virtual void SetColumns();
	virtual void EditStockLevel();
	virtual void EditStockLevelRandom();
	virtual const char* GetTitle();
	virtual void SetFilterArray(); 
	virtual void AppendAudit(){};
	virtual void RemoveAudit(){};
	virtual void HandleCopyLevels();
	virtual void HandleImport();
	virtual void HandlePrintSession();
	virtual void PrepareMenu();

private:
	bool ImportPluInternal( const char* szImportPath, CReorderImportHelper& ImportHelpers );
	bool ImportWrapperInternal( const char* szImportPath, CReorderImportHelper& ImportHelpers );

private:
	bool m_bAllowReorder;
	CSuppRefFinder m_SuppRefFinder;
};

/**********************************************************************/
#endif
/**********************************************************************/
