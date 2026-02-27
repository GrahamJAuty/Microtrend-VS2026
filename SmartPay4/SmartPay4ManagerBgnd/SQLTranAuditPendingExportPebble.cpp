/**********************************************************************/
#include "..\SmartPay4Shared\SmartPayAuditFile.h"
#include "..\SmartPay4Shared\TLogFile.h"
/**********************************************************************/
#include "WebPaymentAtcTransactionFile.h"
/**********************************************************************/
#include "SQLTranAuditPendingExportPebble.h"
/**********************************************************************/

CSQLTranAuditPendingExportPebble::CSQLTranAuditPendingExportPebble(CString strAuditFilename) : CSQLTranBase()
{
	m_strAuditFilename = strAuditFilename;
	m_strError = "";
}

/**********************************************************************/

void CSQLTranAuditPendingExportPebble::DoWork()
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		if (::FileExists(m_strAuditFilename) == FALSE)
		{
			m_bCanCommit = TRUE;
			return;
		}

		CSmartPayAuditFile auditFile(m_strAuditFilename);
		if (auditFile.Open() == FALSE)
		{
			m_strError = "Unable to open audit file";
			return;
		}

		CSQLAuditRecord atcRecord;
		while (auditFile.ReadLine(&atcRecord) == TRUE)
		{
			bool bGotLine = FALSE;
			
			if ((atcRecord.GetTransactionPurse1Total() != 0.0) || (atcRecord.GetTransactionPurse2Balance() != 0.0) || (atcRecord.GetTransactionCash() != 0.0))
			{
				bool bUseFiledTime = FALSE;
				CString strText = atcRecord.GetLineOrDefaultComment();

				switch (atcRecord.GetSourceType())
				{
				case AUDIT_PC:
					bUseFiledTime = TRUE;
					break;
				}

				if (SaveTransactionLine(atcRecord, strText, bUseFiledTime) == FALSE)
				{
					m_strError = "Error processing pending audit line";
					return;
				}

				CreatePendingPebbleLine();
			}
		}
		auditFile.Close();
	}

	m_bCanCommit = TRUE;
}

/**********************************************************************/

bool CSQLTranAuditPendingExportPebble::SaveTransactionLine(CSQLAuditRecord& AuditRecord, CString strText, bool bUseFiledTime)
{
	CSSDate date = (bUseFiledTime) ? AuditRecord.m_dateFiled : AuditRecord.m_dateLastUsed;
	
	if (date.IsSet() == FALSE)
	{
		return FALSE;
	}

	CSSTime time = (bUseFiledTime) ? AuditRecord.m_timeFiled : AuditRecord.m_timeLastUsed;
	if (time.IsSet() == FALSE)
	{
		return FALSE;
	}

	m_RowPending.Reset();
	m_RowPending.SetUserID(AuditRecord.GetUserID());
	m_RowPending.SetXFerUserID(AuditRecord.GetXFerUserID());
	m_RowPending.SetUserName(AuditRecord.GetUserName());
	m_RowPending.SetGroupNo(AuditRecord.GetGroupNo());
	m_RowPending.SetDate(date.GetDate());
	m_RowPending.SetTime(time.GetTime());
	m_RowPending.SetActionType(0);
	m_RowPending.SetSourceType(AuditRecord.GetSourceType());
	m_RowPending.SetAppNo(AuditRecord.GetApplicationNo());
	m_RowPending.SetTerminalNo(AuditRecord.GetTerminalNo());
	m_RowPending.SetCCNo(AuditRecord.GetCCNoFull());
	m_RowPending.SetText(strText);
	m_RowPending.SetPointsAdded(AuditRecord.GetTransactionPoints());
	m_RowPending.SetPointsBalance(AuditRecord.GetCurrentPoints());
	m_RowPending.SetPurse1Value(AuditRecord.GetTransactionPurse1Total());
	m_RowPending.SetPurse2Value(AuditRecord.GetTransactionPurse2Balance());
	m_RowPending.SetPurse3Value(AuditRecord.GetTransactionPurse3Total());
	m_RowPending.SetCashValue(AuditRecord.GetTransactionCash());
	m_RowPending.SetPurse1Balance(AuditRecord.GetCurrentPurse1Total());
	m_RowPending.SetPurse2Balance(AuditRecord.GetCurrentPurse2Balance());
	m_RowPending.SetPurse3Balance(AuditRecord.GetCurrentPurse3Total());
	m_RowPending.SetNotes(AuditRecord.GetWebPaymentID());

	return TRUE;
}

//******************************************************************************

void CSQLTranAuditPendingExportPebble::CreatePendingPebbleLine()
{
	m_PebbleHelper.CreatePebbleLine(m_RowPending, m_pDatabase);
	m_strError = m_PebbleHelper.GetError();
}

//******************************************************************************

void CSQLTranAuditPendingExportPebble::AfterTransaction()
{
	CFileRecycler::SendToRecycleBin(m_strAuditFilename);
}

//******************************************************************************
