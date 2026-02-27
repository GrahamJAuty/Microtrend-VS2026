/**********************************************************************/
#include "..\SmartPay4Shared\GlobalFunctions.h"
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLResultInfo.h"
#include "..\SmartPay4Shared\SQLExceptionLogger.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebblePendingTransaction\SQLRepositoryPebblePendingTransaction.h"
#include "..\SmartPay4Shared\SQLTable_PebblePendingTransaction\SQLRecordSetPebblePendingTransaction.h"
/**********************************************************************/
#include "PebbleHelpers.h"
/**********************************************************************/
#include "SQLTranPebbleImportStage1.h"
/**********************************************************************/

CSQLTranPebbleImportStage1::CSQLTranPebbleImportStage1(CWorkingDlg& WorkingDlg, CSQLRepositoryPebbleConfig& PebbleConfig, CWnd* pParent) : CSQLTranBase(),
	m_WorkingDlg(WorkingDlg),
	m_PebbleConfig(PebbleConfig)
{
	m_pParent = pParent;
	m_bImportOK = FALSE;
	m_strPebbleError = "";
	m_nTranCountTotal = 0;
}

/**********************************************************************/

void CSQLTranPebbleImportStage1::DoWork()
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		CString strSQLException = "";
		m_bCanCommit = ImportNewPayments();

		m_bImportOK = m_bCanCommit;
	}	
}

/**********************************************************************/

bool CSQLTranPebbleImportStage1::ImportNewPayments()
{
	m_WorkingDlg.SetCaption1("Import Pebble Transactions (Sales)");

	CString strLatestDateTimeSale = "";

	if (GetPendingTransactions(TRUE, strLatestDateTimeSale) == FALSE)
	{
		return FALSE;
	}

	m_WorkingDlg.SetCaption1("Import Pebble Transactions (Top-ups)");

	CString strLatestDateTimeTopup = "";
	
	if ( GetPendingTransactions(FALSE, strLatestDateTimeTopup) == FALSE )
	{
		return FALSE;
	}
	
	m_WorkingDlg.SetCaption1("Updating latest transaction times");

	if (UpdateLatestTransactionTime(strLatestDateTimeTopup, strLatestDateTimeSale) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

bool CSQLTranPebbleImportStage1::GetPendingTransactions(bool bSales, CString& strLatestDateTime)
{
	CSQLRepositoryPebbleConfig PebbleConfig;
	PebbleConfig.LoadRow(m_pDatabase);
	CString strImportDate = PebbleConfig.GetSQLFilterDateTime(bSales);
	Server.SetPebbleDiagnosticMessageBoxFlag(TRUE);
	m_strPebbleError = pPebbleHelpers->GetTransactions(PebbleConfig, bSales, m_pDatabase, &m_WorkingDlg, m_nTranCountTotal, strLatestDateTime);
	return (m_strPebbleError == "");
}

//*******************************************************************

bool CSQLTranPebbleImportStage1::UpdateLatestTransactionTime(CString& strLatestDateTimeTopup, CString& strLatestDateTimeSale)
{
	CSQLRepositoryPebbleConfig PrepStatConfig;
	PrepStatConfig.LoadRow(m_pDatabase);
	bool bWantSave = FALSE;

	if (strLatestDateTimeTopup != "")
	{
		CString strOldDate = PrepStatConfig.m_Row.GetLastImportDateTopup();
		CString strOldTime = PrepStatConfig.m_Row.GetLastImportTimeTopup();
		CString strNewDate = strLatestDateTimeTopup.Left(8);
		CString strNewTime = strLatestDateTimeTopup.Right(6);

		bool bUpdateDate = FALSE;
		if (strNewDate > strOldDate)
		{
			bUpdateDate = TRUE;
		}
		else if (strNewDate == strOldDate)
		{
			if (strNewTime > strOldTime)
			{
				bUpdateDate = TRUE;
			}
		}

		if (TRUE == bUpdateDate)
		{
			PrepStatConfig.m_Row.SetLastImportDateTopup(strNewDate);
			PrepStatConfig.m_Row.SetLastImportTimeTopup(strNewTime);
			PrepStatConfig.m_Row.SetFirstImportTopupFlag(FALSE);
			bWantSave = TRUE;
		}
	}
	if (strLatestDateTimeSale != "")
	{
		CString strOldDate = PrepStatConfig.m_Row.GetLastImportDateSale();
		CString strOldTime = PrepStatConfig.m_Row.GetLastImportTimeSale();
		CString strNewDate = strLatestDateTimeSale.Left(8);
		CString strNewTime = strLatestDateTimeSale.Right(6);

		bool bUpdateDate = FALSE;
		if (strNewDate > strOldDate)
		{
			bUpdateDate = TRUE;
		}
		else if (strNewDate == strOldDate)
		{
			if (strNewTime > strOldTime)
			{
				bUpdateDate = TRUE;
			}
		}

		if (TRUE == bUpdateDate)
		{
			PrepStatConfig.m_Row.SetLastImportDateSale(strNewDate);
			PrepStatConfig.m_Row.SetLastImportTimeSale(strNewTime);
			PrepStatConfig.m_Row.SetFirstImportSaleFlag(FALSE);
			bWantSave = TRUE;
		}
	}

	if (TRUE == bWantSave)
	{
		if (PrepStatConfig.SaveRow(m_pDatabase) != SQLCRUD_ERR_NONE)
		{
			m_strPebbleError = "Unable to update next import times";
			return FALSE;
		}
	}

	return TRUE;
}

//*******************************************************************

