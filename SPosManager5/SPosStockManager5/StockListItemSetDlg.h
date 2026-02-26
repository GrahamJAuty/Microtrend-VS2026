/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockAuditBaseReport.h"
#include "SuppRefFinder.h"
#include "StockListDlg.h"
/**********************************************************************/

class CStockListItemSetDlg : public CStockListDlg
{
public:
	CStockListItemSetDlg( int nSpIdx, CWnd* pParent = NULL) : CStockListDlg ( AUDIT_OPENING, AUDIT_EX_OPENING, nSpIdx, pParent ){};
	virtual void SetColumns();
	virtual void DisplayScreen( int nIndex );
	virtual const char* GetTitle() { return "Stockpoint Item Set"; }
	virtual void EditStockLevel();
	virtual void HandleRangeActive();
	virtual void SetFilterArray(); 
	virtual void PrepareMenu();

private:
	CSuppRefFinder m_SuppRefFinder;
};

/**********************************************************************/
#endif
/**********************************************************************/
