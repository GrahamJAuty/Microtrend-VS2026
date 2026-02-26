#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "StockImportFilterArray.h"
#include "StockImportFilterPluDlg.h"
/**********************************************************************/

class CStockImportFilterPluOpenCloseDlg : public CStockImportFilterPluDlg
{
public:
	CStockImportFilterPluOpenCloseDlg( int nSpIdx, bool bClosing, const char* szPath, CStockImportFilterArray& arrayImport, CWnd* pParent );  
	
private:
	void PreviewImportedData();
	
private:
	void ProcessImportDataInternal();
	void GetImportLineInfoQtyValue( CCSV& csvImport, CString& strPluNo, CCSV& csvOriginalData, double& dPluQty, double& dPluValue );

private:
	void AuditExceptions( CSSFile& fileExceptions, CStringArray& StockDetails, CCSV& csvOriginalData );
	void AuditImportData ( CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData, bool bExplode );

private:
	bool m_bClosing;
};

/**********************************************************************/
#endif
/**********************************************************************/

