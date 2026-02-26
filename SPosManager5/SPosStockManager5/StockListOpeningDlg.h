/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockAuditBaseReport.h"
#include "SuppRefFinder.h"
#include "StockImportFilterArray.h"
#include "StockListDlg.h"
/**********************************************************************/

class CStockListOpeningDlg : public CStockListDlg
{
public:
	CStockListOpeningDlg( int nSpIdx, CWnd* pParent = NULL) : CStockListDlg ( AUDIT_OPENING, AUDIT_EX_OPENING, nSpIdx, pParent ){};
	virtual void SetColumns();
	virtual void DisplayScreen( int nIndex );
	virtual void EditStockLevel();
	virtual void EditStockLevelRandom();
	virtual const char* GetTitle() { return "Opening Stock"; }
	virtual void SetFilterArray(); 
	virtual void HandleImport();
	virtual void HandleImportWrapper();
	virtual void HandleImportSheetFile();
	virtual void HandleImportSheetFolder();
	virtual void HandlePrintSession();
	virtual void PrepareMenu();
	virtual void AppendAudit();
	virtual void HandleCreateImportSheet();

private:
	bool HandleImportInternal( CStockImportFilterArray& arrayImport, int nAction );
	bool HandleImportWrapperInternal( CStockImportFilterArray& arrayImport, int nAction );

private:
	CSuppRefFinder m_SuppRefFinder;
};

/**********************************************************************/
#endif
/**********************************************************************/
