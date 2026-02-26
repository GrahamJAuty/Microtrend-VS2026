/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockImportBasicDlg.h"
/**********************************************************************/

class CStockImportBasicTransferDlg : public CStockImportBasicDlg
{
public:
	CStockImportBasicTransferDlg ( int nSpIdx, int nSpIdx2, const char* szPath, const char* szReference1, const char* szReference2, CWnd* pParent );
	virtual void AuditStockChanges ( CSSFile& file, CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData, int nAction );
	virtual void ShowStockChanges();
	virtual void FillActionCombo();

private:
	virtual int GetSpNo2(){ return dbStockpoint.GetSpNo( m_nSpIdx2 ); }
	
private:
	int m_nSpIdx2;
};

/**********************************************************************/
#endif
/**********************************************************************/

