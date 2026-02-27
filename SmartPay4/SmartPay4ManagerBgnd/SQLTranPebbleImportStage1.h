#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebbleConfig\SQLRepositoryPebbleConfig.h"
//**********************************************************************
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
#include "..\SmartPay4Shared\SQLTranBase.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CSQLTranPebbleImportStage1 : public CSQLTranBase
{
public:
	CSQLTranPebbleImportStage1(CWorkingDlg& WorkingDlg, CSQLRepositoryPebbleConfig& PebbleConfig, CWnd* pParent = NULL);

public:
	void DoWork();
	bool GetImportOKFlag() { return m_bImportOK; }

public:
	CString GetPebbleError() { return m_strPebbleError; }
	void SetPebbleError(CString str) { m_strPebbleError = str; }
	int GetTranCountTotal() { return m_nTranCountTotal; }
	
private:
	bool ImportNewPayments();
	bool GetPendingTransactions(bool bSales,CString& strLatestDateTime);
	bool UpdateLatestTransactionTime(CString& strLatestDateTimeTopup, CString& strLatestDateTimeSale);
	
private:
	CSQLRepositoryPebbleConfig& m_PebbleConfig;
	CWorkingDlg& m_WorkingDlg;
	CWnd* m_pParent;

private:
	CString m_strPebbleError;
	bool m_bImportOK;
	int m_nTranCountTotal;
};

/**********************************************************************/
