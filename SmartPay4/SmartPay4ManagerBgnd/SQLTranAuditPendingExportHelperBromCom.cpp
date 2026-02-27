//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_BromComBalanceChanges\SQLRepositoryBromComBalanceChanges.h"
#include "..\SmartPay4Shared\SQLTable_BromComTransaction\SQLRepositoryBromComTransaction.h"
//*******************************************************************
#include "ATCWebDefines.h"
#include "WebPaymentExportTnLog.h"
//*******************************************************************
#include "SQLTranAuditPendingExportHelperBromCom.h"
//*******************************************************************

CSQLTranAuditPendingExportHelperBromCom::CSQLTranAuditPendingExportHelperBromCom()
{
	m_strError = "";

	CString strTmpLogName = ::GetUniqueTempFilename("$31");
	m_strTempLogFilename = System.GetWebPaymentPath(strTmpLogName, nWEBPAYMENT_BROMCOM);

	CCSV csvTmpLogHeader{};
	csvTmpLogHeader.Add(BromComTransaction::PersonID.Label);
	csvTmpLogHeader.Add(BromComTransaction::Details.Label);
	csvTmpLogHeader.Add(BromComTransaction::Amount.Label);
	csvTmpLogHeader.Add(BromComTransaction::Balance.Label);
	csvTmpLogHeader.Add(BromComTransaction::DateTime.Label);
	csvTmpLogHeader.Add(BromComTransaction::Type.Label);
	csvTmpLogHeader.Add(BromComTransaction::TransactionID.Label);
	csvTmpLogHeader.Add(BromComTransaction::WebComment.Label);

	m_strTempLogHeader = csvTmpLogHeader.GetLine();

	m_BromComData.Read();
}

//*******************************************************************

void CSQLTranAuditPendingExportHelperBromCom::CreateBromComLine(CSQLRowAuditPendingExport& RowPending, CDatabase* pDatabase)
{
	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserIDString(RowPending.GetUserID());

	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.SelectRow(RowAccount, pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		CString strMemberID = RowAccount.GetMemberID();

		if (strMemberID != "")
		{
			CString strTransactionDetail = "";
			CSQLRowBromComTransaction RowTran{};
			GetTransactionDetail(RowAccount, RowPending, strTransactionDetail, RowTran);

			if (strTransactionDetail != "")
			{
				LogPendingTransaction(strTransactionDetail);

				{
					CSQLRowBromComBalanceChanges RowBC;
					RowBC.SetPersonID(atoi(strMemberID));
					RowBC.SetAccountID(RowAccount.GetUserID());

					CSQLRepositoryBromComBalanceChanges PrepStat;
					if (PrepStat.RowExists(RowBC.GetPersonID(), pDatabase).GetSQLResult() == 0)
					{
						if (PrepStat.InsertRow(RowBC, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
						{
							m_strError = "Unable to create pending BromCom balance change";
						}
					}
				}

				if ( m_strError == "" )
				{
					CSQLRepositoryBromComTransaction PrepStatTran;
					if (PrepStatTran.InsertRow(RowTran, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
					{
						m_strError = "Unable to create pending BromCom transaction";
					}
				}
			}
		}
	}
}

//***************************************************************************

bool CSQLTranAuditPendingExportHelperBromCom::LogPendingTransaction(CString strLine)
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

void CSQLTranAuditPendingExportHelperBromCom::GetTransactionDetail(CSQLRowAccountFull& RowAccount, CSQLRowAuditPendingExport& RowPending, CString& strTransactionDetail, CSQLRowBromComTransaction& RowTran)
{
	CString strTransactionType = "";
	double dAmount = RowPending.GetPurse1Value() + RowPending.GetPurse2Value();
	
	switch (RowPending.GetActionType())
	{
	case nATCWEB_TOPUP:
	case nATCWEB_TOPUPREFUND:
	case nATCWEB_SALEREFUND:
	case nATCWEB_TRANSFER:
		strTransactionType = "TopUp";
		break;

	case nATCWEB_REFRESH:
		if ( /*m_bIncludePurse2 ==*/ TRUE)		// see if Purse2 used
		{
			strTransactionType = "TopUp";			// TransactionType (enum)
		}
		break;

	case nATCWEB_SALE:
		strTransactionType = "Sale";		// TransactionType
		break;
	}

	if (strTransactionType != "")
	{
		CSSTime time(RowPending.GetTime());
		CString strTime = "";
		strTime.Format("%2.2d:%2.2d ",
			time.GetHour(),
			time.GetMinute());

		double dBalance = RowPending.GetPurse1Balance();
		
		if (m_BromComData.GetPurse1OnlyBalanceFlag() == FALSE)
		{
			dBalance += RowPending.GetPurse2Balance();
		}

		CString strWebComment = "";

		//FOR REFRESH TRANSACTIONS, USE PURSE2 REFRESH TEXT IF SET
		if (RowPending.GetActionType() == nATCWEB_REFRESH)
		{
			CString strPurse2RefreshText = m_BromComData.GetPurse2RefreshText();
			::TrimSpacesFromString(strPurse2RefreshText);

			if (strPurse2RefreshText != "")
			{
				strWebComment = strTime + strPurse2RefreshText;
			}
		}

		if (strWebComment == "")
		{
			strWebComment = strTime + RowPending.GetText();

			if (strWebComment.GetLength() > BromComTransaction::WebComment.Max - 5)
			{
				strWebComment = strWebComment.Left(BromComTransaction::WebComment.Max - 5);
			}
		}

		CString strMemberID = RowAccount.GetMemberID();
		CString strXMLDateTime = RowPending.GetXmlDate(nWEBPAYMENT_BROMCOM);
		int nCCNo = RowPending.GetCCNo();

		int nSource = RowPending.GetSourceType();
		if ((nSource >= 1) && (nSource <= 9))
		{
			nCCNo += (nSource * 100000000);
		}

		CCSV csv;
		csv.Add(strMemberID);
		csv.Add(""); //DETAILS
		csv.Add(dAmount, 2); //AMOUNT
		csv.Add(dBalance, 2); //BALANCE
		csv.Add(strXMLDateTime);
		csv.Add(strTransactionType); //TYPE
		csv.Add(nCCNo); //TRANSACTION ID
		csv.Add(strWebComment); //WEB COMMENT

		strTransactionDetail = csv.GetLine();

		RowTran.SetPersonID(atoi(strMemberID));
		RowTran.SetDetails("");
		RowTran.SetAmount(dAmount);
		RowTran.SetBalance(dBalance);
		RowTran.SetDateTime(strXMLDateTime);
		RowTran.SetType(strTransactionType);
		RowTran.SetTransactionID(nCCNo);
		RowTran.SetWebComment(strWebComment);
	}
}

//**********************************************************************

void CSQLTranAuditPendingExportHelperBromCom::FinaliseLog()
{
	if (::FileExists(m_strTempLogFilename) == TRUE)
	{
		if (LogUpdates("Tn") == TRUE)
		{
			CFileRecycler::SendToRecycleBin(m_strTempLogFilename);
		}
	}
}

//**********************************************************************
// Log the new webupdates (Tnyyyymm \ Toyyyymm )- File has header & fields are in Header order

bool CSQLTranAuditPendingExportHelperBromCom::LogUpdates(const char* szPrefix)
{
	CWebPaymentExportTnLog log(nWEBPAYMENT_BROMCOM, szPrefix);

	CString strLogFile = log.GetCurrentLogName();						// ..\{WebProvider}}\Tnyyyymm.dat
	int nExportLineCount = ::GetFileLineCount(m_strTempLogFilename) - 1;		// number of lines less header line

	bool bReply = ::AppendDataFile(m_strTempLogFilename, strLogFile, TRUE);	// append updates & save in logfile in web based csv format

	log.AddDateStamp(strLogFile, nExportLineCount);				// add date stamp line 'Sent : dd/mm/yy @ hh:mm ' ONLY if LineCount > 0

	return bReply;
}

//*******************************************************************
