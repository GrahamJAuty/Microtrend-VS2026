#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "StockImportFilterArray.h"
#include "StockImportFilterPluDlg.h"
/**********************************************************************/

class CStockImportFilterPluOpenCloseSheetDlg : public CStockImportFilterPluDlg
{
public:
	CStockImportFilterPluOpenCloseSheetDlg( int nSpIdx, bool bClosing, bool bMultiSheet, const char* szPath, CStockImportFilterArray& arrayImport, CWnd* pParent );  
	
protected:
	void AdjustControls();

private:
	void PreviewImportedData();
		
private:
	void ProcessImportDataInternal();
	
private:
	void AuditExceptions( CSSFile& fileExceptions, const char* szStockCode, double dSUQty, const char* szProblem );

private:
	bool m_bClosing;
};

/**********************************************************************/
#endif
/**********************************************************************/

