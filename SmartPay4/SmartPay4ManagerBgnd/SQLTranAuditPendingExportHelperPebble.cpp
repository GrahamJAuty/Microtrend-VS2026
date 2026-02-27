//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_PebbleExportTransaction\SQLRepositoryPebbleExportTransaction.h"
//*******************************************************************
#include "ATCWebDefines.h"
#include "WebPaymentExportTnLog.h"
//*******************************************************************
#include "SQLTranAuditPendingExportHelperPebble.h"
//*******************************************************************

CSQLTranAuditPendingExportHelperPebble::CSQLTranAuditPendingExportHelperPebble()
{
	m_strError = "";

	CString strTmpLogName = ::GetUniqueTempFilename("$31");
	m_strTempLogFilename = System.GetWebPaymentPath(strTmpLogName, nWEBPAYMENT_PEBBLE);

	CCSV csvTmpLogHeader{};
	csvTmpLogHeader.Add(PebbleExportTransaction::MemberID.Label);
	csvTmpLogHeader.Add("Name");
	csvTmpLogHeader.Add(PebbleExportTransaction::Amount.Label);
	csvTmpLogHeader.Add("DateTime");
	csvTmpLogHeader.Add(PebbleExportTransaction::WebComment.Label);

	m_strTempLogHeader = csvTmpLogHeader.GetLine();
}

//*******************************************************************

void CSQLTranAuditPendingExportHelperPebble::CreatePebbleLine(CSQLRowAuditPendingExport& RowPending, CDatabase* pDatabase)
{
	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserIDString(RowPending.GetUserID());

	//ALLOW ROWS WITH NO MEMBER ID SO WE CAN CLEAR PAYMENT DETAIL LATER
	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.SelectRow(RowAccount, pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		CString strTransactionDetail = "";
		CSQLRowPebbleExportTransaction RowTran{};
		GetTransactionDetail(RowAccount, RowPending, RowTran);

		{
			CSQLRepositoryPebbleExportTransaction PrepStatTran;
			if (PrepStatTran.InsertRow(RowTran, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				m_strError = "Unable to create pending Pebble transaction";
			}
		}
	}
}

//***************************************************************************

bool CSQLTranAuditPendingExportHelperPebble::LogPendingTransaction(CString strLine)
{
	bool bReply = FALSE;
	bool bFileExists = ::FileExists(m_strTempLogFilename);

	CSSFile file;
	if ((bReply = file.Open(m_strTempLogFilename, "ab")) == TRUE)
	{
		if (FALSE == bFileExists)
		{
			file.WriteLine(m_strTempLogHeader);
		}

		file.WriteLine(strLine);
		file.Close();
	}
	return bReply;
}

//***************************************************************************

void CSQLTranAuditPendingExportHelperPebble::GetTransactionDetail(CSQLRowAccountFull& RowAccount, CSQLRowAuditPendingExport& RowPending, CSQLRowPebbleExportTransaction& RowTran)
{
	double dAmount = RowPending.GetPurse1Value() + RowPending.GetPurse2Value() + RowPending.GetCashValue();

	/*
	CSSTime time(RowPending.GetTime());
	CString strTime = "";
	strTime.Format("%2.2d:%2.2d ",
		time.GetHour(),
		time.GetMinute());
	*/

	CString strWebComment = /*strTime +*/ RowPending.GetText();

	if (strWebComment.GetLength() > PebbleExportTransaction::WebComment.Max - 5)
	{
		strWebComment = strWebComment.Left(PebbleExportTransaction::WebComment.Max - 5);
	}

	CString strMemberID = RowAccount.GetMemberID();
	CString strXMLDateTime = RowPending.GetXmlDate(nWEBPAYMENT_PEBBLE);

	/*
	CCSV csv;
	csv.Add(strMemberID);
	csv.Add(RowAccount.GetAccountDisplayName()); //DETAILS
	csv.Add(dAmount, 2); //AMOUNT
	csv.Add(strXMLDateTime);
	csv.Add(strWebComment); //WEB COMMENT

	strTransactionDetail = csv.GetLine();
	*/

	RowTran.SetUserID(_atoi64(RowPending.GetUserID()));
	RowTran.SetXFerUserID(_atoi64(RowPending.GetXFerUserID()));
	RowTran.SetMemberID(strMemberID);
	RowTran.SetDate(RowPending.GetDate());
	RowTran.SetTime(RowPending.GetTime());
	RowTran.SetCCNo(RowPending.GetCCNo());
	RowTran.SetSourceType(RowPending.GetSourceType());
	RowTran.SetAppNo(RowPending.GetAppNo());
	RowTran.SetAmount(dAmount);
	RowTran.SetPurse1(RowPending.GetPurse1Value());
	RowTran.SetPurse2(RowPending.GetPurse2Value());
	RowTran.SetCash(RowPending.GetCashValue());
	RowTran.SetWebComment(strWebComment);
}

//**********************************************************************

