//*******************************************************************
#include "..\SmartPay4ManagerBgnd\SQLTranPebbleImportStage1.h"
#include "..\SmartPay4ManagerBgnd\SQLTranPebbleImportStage2.h"
//*******************************************************************
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLConnectionPool.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_PebblePendingTransaction\SQLRecordSetPebblePendingTransaction.h"
#include "..\SmartPay4Shared\SQLTable_PebblePendingTransaction\SQLRepositoryPebblePendingTransaction.h"
//*******************************************************************
#include "WebPaymentDefines.h"
//*******************************************************************
#include "PebbleImport.h"
//*******************************************************************

CPebbleImport::CPebbleImport(CWnd* pParent) : m_WorkingDlg( "Import Pebble Transactions", FALSE)
{
	m_pParent = pParent;
	m_nTotalPaymentCount = 0;
	m_nValidPaymentCount = 0;
	m_strTitle = "Synchronise Pebble Transactions";
	m_strPebbleError = "";
}

//*******************************************************************

bool CPebbleImport::Import(bool bNewPayments)
{
	m_strPebbleError = "";
	m_nTotalPaymentCount = 0;
	m_nValidPaymentCount = 0;

	m_PebbleConfig.LoadRow(NULL);
	if (m_PebbleConfig.IsSet() == FALSE)
	{
		m_strPebbleError = szINTERFACENOTSET;
		return FALSE;
	}

	PurgeRecentTransactions();
	PurgeRecentPayments();

	m_WorkingDlg.Create();

	if (TRUE == bNewPayments)
	{
		if (FetchNewTransactions() == FALSE)
		{
			if (m_strPebbleError == "")
			{
				m_strPebbleError = "Unable to Process SQL Transaction (Pebble Stage 1)";
			}

			return FALSE;
		}
	}

	CSQLTranPebbleImportStage2 TranStageTwo(m_WorkingDlg, bNewPayments, m_nValidPaymentCount, m_pParent);

	// delete session exceptions, session import log, delete session exceptions
	CFileRecycler::SendToRecycleBin(TranStageTwo.GetNewTempExceptionFile());
	CFileRecycler::SendToRecycleBin(TranStageTwo.GetTempLogFile());					
	CFileRecycler::SendToRecycleBin(TranStageTwo.GetTempExceptionFile());
	
	if (ProcessTransactions(TranStageTwo) == FALSE)
	{
		if (m_strPebbleError == "")
		{
			m_strPebbleError = "Unable to Process SQL Transaction (Pebble Stage 2)";
		}

		return FALSE;
	}

	m_WorkingDlg.DestroyWindow();

	DisplayImportResults(TranStageTwo);

	// only log exceptions on importing New Payments
	if ((TRUE == bNewPayments) && (TranStageTwo.GetNewExceptionCount() > 0))
	{
		CString strFile = "";
		strFile.Format("%s.dat",
			(const char*)TranStageTwo.GetExceptionName());

		strFile = System.GetWebPaymentPath(strFile);
		::AppendDataFile(TranStageTwo.GetNewTempExceptionFile(), strFile);
	}

	// delete session exceptions, session import log, delete session exceptions
	CFileRecycler::SendToRecycleBin(TranStageTwo.GetNewTempExceptionFile());
	CFileRecycler::SendToRecycleBin(TranStageTwo.GetTempLogFile());				
	CFileRecycler::SendToRecycleBin(TranStageTwo.GetTempExceptionFile());			
	
	return TRUE;
}

//*******************************************************************

void CPebbleImport::PurgeRecentTransactions()
{
	int nDays = m_PebbleConfig.m_Row.GetRecentCacheDays();

	if (nDays <= 0)
	{
		return;
	}

	CString strLastTopupTime = ConvertISO8601ToSimpleDateTime(m_PebbleConfig.GetSQLFilterDateTime(FALSE));
	CString strLastSaleTime = ConvertISO8601ToSimpleDateTime(m_PebbleConfig.GetSQLFilterDateTime(TRUE));

	if ((strLastTopupTime == "") || (strLastSaleTime == ""))
	{
		return;
	}

	CString strTimeToUse = strLastTopupTime;
	if (strLastSaleTime < strTimeToUse)
	{
		strTimeToUse = strLastSaleTime;
	}

	COleDateTime oleTimeToUse = COleDateTime(
		atoi(strTimeToUse.Left(4)),
		atoi(strTimeToUse.Mid(4, 2)),
		atoi(strTimeToUse.Mid(6, 2)),
		0, 0, 0);

	if (oleTimeToUse.m_status != COleDateTime::valid)
	{
		return;
	}

	if (oleTimeToUse > COleDateTime::GetCurrentTime())
	{
		return;
	}

	oleTimeToUse -= COleDateTimeSpan(nDays, 0, 0, 0);

	CString strSQL = "";
	strSQL.Format("DELETE FROM %s WHERE %s < '%4.4d%2.2d%2.2d000000'",
		(const char*)SQLTableNames::PebbleRecentTransactions,
		(const char*)PebbleRecentTransaction::DateTime.Label,
		oleTimeToUse.GetYear(),
		oleTimeToUse.GetMonth(),
		oleTimeToUse.GetDay());

	int nConnection = -1;
	CDatabase* pDatabase = SQLConnectionPool.GetPooledConnection(nConnection);
	if (pDatabase != NULL)
	{
		CSQLHelpers::ExecuteSQL(pDatabase, strSQL);
		SQLConnectionPool.FreeConnection(nConnection);
	}
}

//*******************************************************************

void CPebbleImport::PurgeRecentPayments()
{
	int nDays = m_PebbleConfig.m_Row.GetPaymentCacheDays();

	if (nDays <= 0)
	{
		return;
	}

	COleDateTime timeFilter = COleDateTime::GetCurrentTime();
	timeFilter -= COleDateTimeSpan(nDays, 0, 0, 0);

	CString strSQL = "";
	strSQL.Format("DELETE FROM %s WHERE %s < '%4.4d%2.2d%2.2d000000'",
		(const char*)SQLTableNames::PebbleExportPayments,
		(const char*)PebbleExportPayment::CreateDateTime.Label,
		timeFilter.GetYear(),
		timeFilter.GetMonth(),
		timeFilter.GetDay());

	int nConnection = -1;
	CDatabase* pDatabase = SQLConnectionPool.GetPooledConnection(nConnection);
	if (pDatabase != NULL)
	{
		CSQLHelpers::ExecuteSQL(pDatabase, strSQL);
		SQLConnectionPool.FreeConnection(nConnection);
	}
}

//*******************************************************************

bool CPebbleImport::FetchNewTransactions()
{
	bool bResult = FALSE;

	CSQLTranPebbleImportStage1 Tran(m_WorkingDlg, m_PebbleConfig, m_pParent);

	if (Tran.BeginTrans() == SQLTRAN_STATE_ACTIVE)
	{
		Tran.DoWork();

		switch (Tran.EndTrans())
		{
		case SQLTRAN_STATE_COMMITOK:
			bResult = TRUE;
			m_nTotalPaymentCount = Tran.GetTranCountTotal();
			break;

		default:
			if (Tran.GetPebbleError() == "")
			{
				m_strPebbleError = "Unable to process SQL Transaction (Pebble Stage 1)";
			}
			else
			{
				m_strPebbleError = Tran.GetPebbleError();
			}
			break;
		}
	}
	else
	{
		m_strPebbleError = "Unable to create SQL Transaction (Pebble Stage 1)";
	}

	return bResult;
}

//*******************************************************************

bool CPebbleImport::ProcessTransactions(CSQLTranPebbleImportStage2& Tran)
{
	bool bResult = FALSE;

	if (Tran.BeginTrans() == SQLTRAN_STATE_ACTIVE)
	{
		Tran.DoWork();

		switch (Tran.EndTrans())
		{
		case SQLTRAN_STATE_COMMITOK:
			bResult = TRUE;
			Tran.AfterTransaction();
			break;

		default:
			if (Tran.GetPebbleError() == "")
			{
				m_strPebbleError = "Unable to process SQL transaction (Pebble Stage 2)";
			}
			else
			{
				m_strPebbleError = Tran.GetPebbleError();
			}
			break;
		}
	}
	else
	{
		m_strPebbleError = "Unable to create SQL transaction (Pebble Stage 2)";
	}

	return bResult;
}

//*******************************************************************
