#pragma once
/**********************************************************************/
#include "BromComData.h"
//**********************************************************************
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
#include "..\SmartPay4Shared\SQLTranBase.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CSQLTranBromComImport : public CSQLTranBase
{
public:
	CSQLTranBromComImport(CWorkingDlg& WorkingDlg, CBromComData& BromComData, bool bNewPayments, CWnd* pParent = NULL);

public:
	void DoWork();
	bool GetImportOKFlag() { return m_bImportOK; }

public:
	CString GetBromComError() { return m_strBromComError; }
	int GetValidPaymentCount() { return m_nValidPaymentCount; }
	CString GetFilenamePendingOrders() { return m_strFilenamePendingOrders; }
	CString GetFilenamePendingItems() { return m_strFilenamePendingItems; }

public:
	void SetBromComError(CString str) { m_strBromComError = str; }
	void SetValidPaymentCount(int n) { m_nValidPaymentCount = n; }

private:
	bool ImportNewPayments( CString& strSQLException );
	CString GetPendingOrders();
	bool CopyPendingOrdersToDatabase(CReportConsolidationArray<CSortedIntItem>& arrayOrderID);
	void DeletePendingItemsFromDatabase(CReportConsolidationArray<CSortedIntItem>& arrayOrderID);
	bool CopyPendingItemsToDatabase();
	CString GetPendingItems(CReportConsolidationArray<CSortedIntItem>& arrayOrderID, bool& nNoItemsButOK);

private:
	CBromComData& m_BromComData;
	CWorkingDlg& m_WorkingDlg;
	CWnd* m_pParent;

private:
	CCSV m_csvHeaderOrder;
	CCSV m_csvHeaderOrderItem;

private:
	CString m_strBromComError;
	CString m_strFilenamePendingOrders;
	CString m_strFilenamePendingItems;
	int m_nValidPaymentCount;
	bool m_bNewPayments;
	bool m_bImportOK;
};

/**********************************************************************/
