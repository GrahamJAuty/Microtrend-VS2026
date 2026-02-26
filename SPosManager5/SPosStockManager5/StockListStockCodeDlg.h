#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "ProcessedOrderArray.h"
/**********************************************************************/
#include "StockListDlg.h"
/**********************************************************************/

class CStockListStockCodeDlg : public CStockListDlg
{
public:
	CStockListStockCodeDlg( CWnd* pParent = NULL);
	virtual void SetColumns();
	virtual void EditStockLevel();
	virtual const char* GetTitle() { return "Change Stock Codes"; }
	virtual const char* GetSpName();
	virtual void SetFilterArray(); 
	virtual void AppendAudit(){};
	virtual void RemoveAudit(){};
	virtual void HandleCancel();
	virtual void HandleCopySupplierStockCodes();
	virtual void HandleCopyPluNumbers();
	virtual void HandleRemoveStockZeros();
	void ChangeStockCodes();
	void CheckDuplicates();
	virtual void PrepareMenu();

private:
	bool ChangeStockCodesInternal();
	int CopySupplierStockCodesInternal();
	int CopyPluNumbersInternal();
	void RemoveStockZerosInternal();
	void CheckDuplicatesInternal();

private:
	void UpdateProcessedOrders( CProcessedOrderArray& orderArray );

private:
	bool m_bCheckDuplicates;
};

/**********************************************************************/
#endif
/**********************************************************************/
