//**********************************************************************
#include "..\SmartPay4Shared\AESHelpers.h"
#include "..\SmartPay4Shared\MessageLogger.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_ExternalAccount\SQLRepositoryExternalAccount.h"
#include "..\SmartPay4Shared\SQLTable_ExternalAccount\SQLRowExternalAccount.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
//**********************************************************************
#include "EposSmartPayFile.h"
#include "ExternalCommandSigner.h"
#include "ExternalReplyArray.h"
#include "SmartPay4Server.h"
//**********************************************************************
#include "ServerWrapper.h"
//**********************************************************************
#define MAX_COMMAND_LINES 84
//**********************************************************************

void CServerWrapper::ProcessExternalCommand()
{
	m_dateToday.SetCurrentDate();							// set date & time
	m_timeToday.SetCurrentTime();

	CStringArray arrayLines{};

	{
		CString strMsg;
		strMsg.Format("==> EXT.REQ: %s", 
			(const char*) m_LoyReqThreadManager.GetREQFilepath());

		MessageLogger.LogSystemMessage(strMsg);
	}

	for (int n = 0; (n < m_LoyReqThreadManager.GetRequestLineCount()) && (n < MAX_COMMAND_LINES); n++)
	{
		arrayLines.Add(m_LoyReqThreadManager.GetRequestLine(n));
	}

	if (arrayLines.GetSize() > MAX_COMMAND_LINES)
	{
		CExternalReplyArray arrayError{};
		arrayError.CreateErrorReply("1", "Too many lines in command");
		CreateExternalReply(arrayError);
	}
	else
	{
		switch (CExternalCommandSigner::CheckSignature(arrayLines))
		{
		case 1:
		{
			CExternalReplyArray arrayError{};
			arrayError.CreateErrorReply("2", "Invalid command signature");
			CreateExternalReply(arrayError);
		}
		break;

		case 2:
		{
			CExternalReplyArray arrayError{};
			arrayError.CreateErrorReply("7", "Invalid time stamp");
			CreateExternalReply(arrayError);
		}
		break;

		case 0:
		{
			int nCommandType = -1;
			CString strCommand = "";

			if (arrayLines.GetSize() > 0)
			{
				strCommand = arrayLines.GetAt(0);
				nCommandType = m_ExternalParamsArray.Init(strCommand);
			}

			switch (nCommandType)
			{
			case -1:
			{
				CExternalReplyArray arrayError{};
				arrayError.CreateErrorReply("3", "No command received");
				CreateExternalReply(arrayError);
			}
			break;

			case 0:
			{
				CExternalReplyArray arrayError{};
				arrayError.CreateErrorReply("4", "Unknown command", strCommand);
				CreateExternalReply(arrayError);
			}
			break;

			default:
			{
				CExternalReplyArray arrayError{};
				m_ExternalParamsArray.GetParams(arrayLines, arrayError);

				if (arrayError.GetSize() != 0)
				{
					nCommandType = 0;
					CreateExternalReply(arrayError);
				}
			}
			break;
			}

			switch (nCommandType)
			{
			case EXTCOMMAND_AUTHENTICATE:
				CreateExternalAuthenticateReply();
				break;

			case EXTCOMMAND_USER:
				CreateExternalBalanceReply(TRUE);
				break;

			case EXTCOMMAND_BALANCE:
				CreateExternalBalanceReply(FALSE);
				break;

			case EXTCOMMAND_PAYMENT:
				CreateExternalPaymentReply();
				break;

			case EXTCOMMAND_RESETPASSWORD:
				CreateExternalResetPasswordReply();
				break;

			case EXTCOMMAND_CREATEUSER:
				CreateExternalCreateUserReply();
				break;

			case EXTCOMMAND_USEREXISTS:
				CreateExternalUserExistsReply();
				break;
			}
		}
		break;
		}
	}
}

//**********************************************************************

void CServerWrapper::CreateExternalAuthenticateReply()
{
	CExternalReplyArray arrayReply;

	if ( m_ExternalParamsArray.m_strUserName == "")
	{
		arrayReply.CreateErrorReply("101", "No username specified");
	}
	else if (m_ExternalParamsArray.m_strPassword == "")
	{
		arrayReply.CreateErrorReply("102", "No password specified");
	}
	else
	{
		CString strAESUsername = "";
		CAESHelpers::PlainTextToAESHex(m_ExternalParamsArray.m_strUserName, strAESUsername);

		CSQLRowExternalAccount RowExternal;
		RowExternal.SetAESUsername(strAESUsername);

		CSQLRepositoryExternalAccount RepoExt;
		if ( RepoExt.SelectRow(RowExternal,NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			arrayReply.CreateLoginErrorReply(104);
		}
		else
		{
			if (m_ExternalParamsArray.m_strPassword != RowExternal.GetPasswordHash())
			{
				arrayReply.CreateLoginErrorReply(105);
			}
			else
			{
				CString strAccountID = "";
				CAESHelpers::AESHexToPlainText(RowExternal.GetAESUserID(), strAccountID);

				CSQLRowAccountFull RowAccount;
				RowAccount.SetUserID(_atoi64(strAccountID));

				CSQLRepositoryAccount RepoAccount;
				if (RepoAccount.SelectRow(RowAccount,NULL).GetSQLError() != SQLCRUD_ERR_NONE)
				{
					arrayReply.CreateLoginErrorReply(106);
				}
				else
				{
					arrayReply.AddOKReply("UserID", strAccountID);
				}
			}
		}
	}

	CreateExternalReply(arrayReply);
}

//**********************************************************************

void CServerWrapper::CreateExternalBalanceReply(bool bDetail)
{
	CExternalReplyArray arrayReply{};

	if (m_ExternalParamsArray.m_strUserID == "")
	{
		arrayReply.CreateErrorReply("201", "No UserID specified");
	}
	else
	{
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(_atoi64(m_ExternalParamsArray.m_strUserID));
		CSQLRepositoryAccount RepoAccount;
		if ( RepoAccount.SelectRow(RowAccount,NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			arrayReply.CreateErrorReply("203", "Loyalty account not found");
		}
		else
		{
			//FIDDLE : SHOULD TAKE ACCOUNT OF DAY GROUP
			CSQLRowGroupFull RowGroup;
			RowGroup.SetGroupNo(RowAccount.GetGroupNo());
			CSQLRepositoryGroup RepoGroup;
			RepoGroup.SelectRow(RowGroup,NULL);

			if (TRUE == bDetail)
			{
				arrayReply.AddOKReply("UserID", RowAccount.GetUserIDString());
				arrayReply.AddOKReply("Forename", RowAccount.GetForename());
				arrayReply.AddOKReply("Surname", RowAccount.GetSurname());
				arrayReply.AddOKReply("Gender", RowAccount.GetGenderText());
				arrayReply.AddOKReply("DOB", RowAccount.GetDOB());
				arrayReply.AddOKReply("GroupNumber", RowAccount.GetGroupNoString());
				arrayReply.AddOKReply("Info3", RowAccount.GetInfo3());
				arrayReply.AddOKReply("Info4", RowAccount.GetInfo4());

				CString strUserName = "";

				CString strUserID = ::FormatInt64Value(RowAccount.GetUserID());
				CString strAESUserID = "";
				CAESHelpers::PlainTextToAESHex(strUserID, strAESUserID);

				CSQLRowExternalAccount RowExternal;
				RowExternal.SetAESUserID(strAESUserID);

				CSQLRepositoryExternalAccount RepoExt;
				if ( RepoExt.SelectRowByUserID(RowExternal,NULL).GetSQLError() == SQLCRUD_ERR_NONE)
				{
					CAESHelpers::AESHexToPlainText(RowExternal.GetAESUsername(), strUserName);
				}

				arrayReply.AddOKReply("ExtUsername", strUserName);
			}

			{
				CPurse1AvailableSpendInfo ASInfoP1;
				CPurse3AvailableSpendInfo ASInfoP3;
				CalculateBalanceP1(RowAccount, RowGroup, ASInfoP1);
				CalculateBalanceP3(RowAccount, RowGroup, ASInfoP3);

				double dAvailableSpend = 0.0;
				dAvailableSpend += ASInfoP1.m_dAvailableSpendP1;
				dAvailableSpend += ASInfoP1.m_dBalanceP2;
				dAvailableSpend += ASInfoP3.m_dAvailableSpendP3;

				CString strBalance = "";
				strBalance.Format("%d", int(floor(dAvailableSpend * 100.0)));

				arrayReply.AddOKReply("Balance", strBalance);
			}
		}
	}

	CreateExternalReply(arrayReply);
}

//**********************************************************************

bool CServerWrapper::IsHotlistedCard(CSQLRowAccountFull& AccountRow)
{
	bool bResult = FALSE;

	if (AccountRow.IsAlertSet() == TRUE)
	{
		if (System.GetInterfaceType() == nINTERFACE_MIFAREv1)
		{
			bResult = TRUE;
		}
	}

	return bResult;
}

//**********************************************************************

void CServerWrapper::CreateExternalPaymentReply()
{
	CExternalReplyArray arrayReply{};

	if (m_ExternalParamsArray.m_strUserID == "")
	{
		arrayReply.CreateErrorReply("401", "No UserID specified");
	}
	else if ((m_ExternalParamsArray.m_nLoyaltyAmount <= 0) && (m_ExternalParamsArray.m_nOtherAmount <= 0))
	{
		arrayReply.CreateErrorReply("404", "No amount specified");
	}
	else
	{
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(_atoi64(m_ExternalParamsArray.m_strUserID));
		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			arrayReply.CreateErrorReply("403", "Loyalty account not found");
		}
		else
		{
			CSQLRowGroupFull RowGroup;
			RowGroup.SetGroupNo(RowAccount.GetGroupNo());
			
			CSQLRepositoryGroup RepoGroup;
			RepoGroup.SelectRow(RowGroup,NULL);

			if (IsHotlistedCard(RowAccount) == TRUE)
			{
				arrayReply.CreateErrorReply("405", "Loyalty account has been hotlisted");
			}
			else if (RowAccount.GetInactive() == TRUE)
			{
				arrayReply.CreateErrorReply("406", "Loyalty account is flagged as inactive");
			}
			else if (RowAccount.HasUserExpired() == TRUE)
			{
				arrayReply.CreateErrorReply("407", "Loyalty account expired", RowAccount.GetExpiryDate());
			}
			else
			{
				m_ExternalParamsArray.m_nLoyaltyAmount = max(m_ExternalParamsArray.m_nLoyaltyAmount, 0);
				m_ExternalParamsArray.m_nOtherAmount = max(m_ExternalParamsArray.m_nOtherAmount, 0);

				if (m_ExternalParamsArray.m_nLoyaltyAmount > 0)
				{
					CPurse1AvailableSpendInfo ASInfoP1;
					CPurse3AvailableSpendInfo ASInfoP3;
					CalculateBalanceP1(RowAccount, RowGroup,ASInfoP1);
					CalculateBalanceP3(RowAccount, RowGroup,ASInfoP3);

					double dAvailableSpend = 0.0;
					dAvailableSpend += ASInfoP1.m_dAvailableSpendP1;
					dAvailableSpend += ASInfoP1.m_dBalanceP2;
					dAvailableSpend += ASInfoP3.m_dAvailableSpendP3;

					int nMaxSpend = int(floor(dAvailableSpend * 100.0));

					if (nMaxSpend < m_ExternalParamsArray.m_nLoyaltyAmount)
					{
						arrayReply.CreateErrorReply("408", "Insufficient funds");
					}
				}

				if (arrayReply.GetSize() == 0)
				{
					COleDateTime timeNow = COleDateTime::GetCurrentTime();

					CString strDateNow = "";
					strDateNow.Format("%2.2d/%2.2d/%4.4d",
						timeNow.GetDay(),
						timeNow.GetMonth(),
						timeNow.GetYear());

					CString strTimeNow = "";
					strTimeNow.Format("%2.2d:%2.2d:%2.2d",
						timeNow.GetHour(),
						timeNow.GetMinute(),
						timeNow.GetSecond());

					CStringArray arrayTran;

					CCSV csv;
					csv.Add("$TRAN");
					csv.Add(m_ExternalParamsArray.m_strUserID);
					csv.Add(0); //TILL NO
					csv.Add(0); //SERVER NO
					csv.Add(0); //TRANSACTION NO
					csv.Add(m_ExternalParamsArray.m_nLoyaltyAmount + m_ExternalParamsArray.m_nOtherAmount);
					csv.Add(""); //SPECIFIC PURSE
					csv.Add(""); //FIND OUT WHAT THIS DOES
					csv.Add(strDateNow);
					csv.Add(strTimeNow);
					csv.Add(0);
					arrayTran.Add(csv.GetLine());

					if (m_ExternalParamsArray.m_nLoyaltyAmount > 0)
					{
						csv.RemoveAll();
						csv.Add("$PAYM");
						csv.Add(20); //CASHLESS PAYMENT TYPE
						csv.Add("External");
						csv.Add(m_ExternalParamsArray.m_nLoyaltyAmount);
						arrayTran.Add(csv.GetLine());
					}

					if (m_ExternalParamsArray.m_nOtherAmount > 0)
					{
						csv.RemoveAll();
						csv.Add("$PAYM");
						csv.Add(1);
						csv.Add("External");
						csv.Add(m_ExternalParamsArray.m_nOtherAmount);
						arrayTran.Add(csv.GetLine());
					}

					{
						CCSV csvItems(m_ExternalParamsArray.m_strItems);

						int nPos = 0;
						while (nPos <= csvItems.GetSize() - 4)
						{
							csv.RemoveAll();
							csv.Add("$ITEM");
							csv.Add("");								//CARD NO, NOT NEEDED
							csv.Add(csvItems.GetString(nPos));			//PLU NO
							csv.Add(csvItems.GetString(nPos + 1));		//QTY
							csv.Add(csvItems.GetString(nPos + 2));		//VALUE
							csv.Add(csvItems.GetString(nPos + 3));		//TEXT
							csv.Add("");								//DEPT NO, ADD LATER
							csv.Add("");								//TAX CODE, ADD LATER

							arrayTran.Add(csv.GetLine());

							nPos += 4;
						}
					}

					CPendingExportPaymentData Dummy;
					CPendingEposTerminalData Dummy2;
					CEposSmartPayFile SmartPayFile(Dummy,Dummy2,&arrayTran);

					if (SmartPayFile.ScanExternal() == FALSE)
					{
						arrayReply.CreateErrorReply("409", "Unable to process payment data");
					}
					else
					{
						int nGroupNo = 0;
						SmartPayFile.UpdateRecordFromLogData(RowAccount, nGroupNo);
						CSQLRepositoryAccount RepoAccount;
						RepoAccount.UpsertRow(RowAccount, NULL);
						SmartPayFile.SaveEposPaymentAudit();

						CString strCCNo = "";
						strCCNo.Format("%d", SmartPayFile.GetExternalCCNo());
						arrayReply.AddOKReply("CCNo", strCCNo);
					}
				}
			}
		}
	}
	 CreateExternalReply(arrayReply);
}

//**********************************************************************

void CServerWrapper::CreateExternalResetPasswordReply()
{
	CExternalReplyArray arrayReply;

	if (m_ExternalParamsArray.m_strUserName == "")
	{
		arrayReply.CreateErrorReply("501", "No username specified");
	}
	else if (m_ExternalParamsArray.m_strPassword == "")
	{
		arrayReply.CreateErrorReply("502", "No password specified");
	}
	else
	{
		CString strAESUserName = "";
		CAESHelpers::PlainTextToAESHex(m_ExternalParamsArray.m_strUserName, strAESUserName);

		CSQLRowExternalAccount RowExternal;
		RowExternal.SetAESUsername(strAESUserName);

		CSQLRepositoryExternalAccount RepoExt;
		if ( RepoExt.SelectRow(RowExternal,NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			arrayReply.CreateErrorReply("504", "User name not found");
		}
		else
		{
			RowExternal.SetPasswordHash(m_ExternalParamsArray.m_strPassword);
			RepoExt.UpdateRow(RowExternal,NULL);
			arrayReply.Add("OK");
		}
	}

	CreateExternalReply(arrayReply);
}

//**********************************************************************

void CServerWrapper::CreateExternalCreateUserReply()
{
	CExternalReplyArray arrayReply;

	if (m_ExternalParamsArray.m_strUserName == "")
	{
		arrayReply.CreateErrorReply("601", "No username specified");
	}
	else if (m_ExternalParamsArray.m_strPassword == "")
	{
		arrayReply.CreateErrorReply("606", "No password specified");
	}
	else
	{
		CString strAESUserName = "";
		CAESHelpers::PlainTextToAESHex(m_ExternalParamsArray.m_strUserName, strAESUserName);

		CSQLRowExternalAccount RowExternal;
		RowExternal.SetAESUsername(strAESUserName);

		CSQLRepositoryExternalAccount RepoExt;
		switch (RepoExt.SelectRow(RowExternal,NULL).GetSQLError())
		{
		case SQLCRUD_ERR_NONE:
		case SQLCRUD_ERR_DUPLICATE:
			arrayReply.CreateErrorReply("604", "Username already exists");
			break;
		}

		__int64 nNewUserID = 0;
		if (arrayReply.GetSize() == 0)
		{
			CSQLRepositoryAccount RepoAccount;
			nNewUserID = RepoAccount.GetNextFreeUserID(1, System.GetMaxCardNo(),NULL).GetSQLResult();
		
			if (0 == nNewUserID)
			{
				arrayReply.CreateErrorReply("607", "Unable to find free user number");
			}
		}

		if (arrayReply.GetSize() == 0)
		{
			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(nNewUserID);
			RowAccount.SetForename(m_ExternalParamsArray.m_strForename);
			RowAccount.SetSurname(m_ExternalParamsArray.m_strSurname);
			RowAccount.SetInfo3(m_ExternalParamsArray.m_strInfo3);
			RowAccount.SetInfo4(m_ExternalParamsArray.m_strInfo4);

			{
				CString strUsername = m_ExternalParamsArray.m_strForename;

				if (m_ExternalParamsArray.m_strSurname != "")
				{
					if (strUsername != "")
					{
						strUsername += " ";
					}
					strUsername += m_ExternalParamsArray.m_strSurname;
				}

				RowAccount.SetUsername(strUsername);
			}

			if (m_ExternalParamsArray.m_strGender == "F")
			{
				RowAccount.SetGender("F");
			}
			else if (m_ExternalParamsArray.m_strGender == "M")
			{
				RowAccount.SetGender("M");
			}
			else
			{
				RowAccount.SetGender("");
			}

			RowAccount.SetGroupNo(1);
			if ((m_ExternalParamsArray.m_nGroup >= Group::GroupNo.Min) && (m_ExternalParamsArray.m_nGroup <= Group::GroupNo.Max))
			{
				RowAccount.SetGroupNo(m_ExternalParamsArray.m_nGroup);
			}

			RowAccount.SetDOB("");
			COleDateTime oleDOB;
			if (CSQLFieldValidatorAccount::StringToOleDate(m_ExternalParamsArray.m_strDOB, oleDOB) == TRUE)
			{
				if (CSQLFieldValidatorAccount::ValidateDOB(oleDOB) == TRUE)
				{
					RowAccount.SetDOB(CSQLFieldValidatorAccount::OleDateToString(oleDOB));
				}
			}

			CSQLRepositoryAccount RepoAccount;
			if (RepoAccount.InsertRow(RowAccount,NULL).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				arrayReply.CreateErrorReply("605", "Unable to create Loyalty Account");
			}
			else
			{
				CSQLRowExternalAccount ExternalRow;
				ExternalRow.SetAESUsername(strAESUserName);
				ExternalRow.SetPasswordHash(m_ExternalParamsArray.m_strPassword);
				
				CString strCardNo = ::FormatInt64Value( nNewUserID);

				CString strAESCardNo = "";
				CAESHelpers::PlainTextToAESHex(strCardNo, strAESCardNo);
				ExternalRow.SetAESUserID(strAESCardNo);

				RepoExt.InsertRow(ExternalRow,NULL);

				arrayReply.AddOKReply("UserID", strCardNo);
				arrayReply.AddOKReply("Forename", RowAccount.GetForename());
				arrayReply.AddOKReply("Surname", RowAccount.GetSurname());
				arrayReply.AddOKReply("Gender", RowAccount.GetGenderText());
				arrayReply.AddOKReply("DOB", RowAccount.GetDOB());
				arrayReply.AddOKReply("Info3", RowAccount.GetInfo3());
				arrayReply.AddOKReply("Info4", RowAccount.GetInfo4());
				arrayReply.AddOKReply("GroupNumber", RowAccount.GetGroupNoString());
				arrayReply.AddOKReply("ExtUserName", m_ExternalParamsArray.m_strUserName);
			}
		}
	}

	CreateExternalReply(arrayReply);
}

//**********************************************************************

void CServerWrapper::CreateExternalUserExistsReply()
{
	CExternalReplyArray arrayReply;

	if (m_ExternalParamsArray.m_strUserName == "")
	{
		arrayReply.CreateErrorReply("701", "No username specified");
	}
	else
	{
		{
			CString strAESUsername = "";
			CAESHelpers::PlainTextToAESHex(m_ExternalParamsArray.m_strUserName, strAESUsername);

			bool bUserExists = FALSE;

			CSQLRowExternalAccount RowExternal;
			RowExternal.SetAESUsername(strAESUsername);

			CSQLRepositoryExternalAccount RepoExt;
			if ( RepoExt.SelectRow(RowExternal,NULL).GetSQLError() == SQLCRUD_ERR_NONE)
			{ 
				CString strAESUserID = RowExternal.GetAESUserID();

				CString strUserID = "";
				CAESHelpers::AESHexToPlainText(strAESUserID, strUserID);

				CSQLRepositoryAccount RepoAccount;
				bUserExists = (RepoAccount.RowExists(_atoi64(strUserID), NULL).GetSQLResult() != 0);
			}

			arrayReply.AddOKReply("USEREXISTS", bUserExists ? "1" : "0");
		}
	}
	CreateExternalReply(arrayReply);
}

//**********************************************************************

void CServerWrapper::CreateExternalReply(CExternalReplyArray& arrayReply)
{
	CExternalCommandSigner::SignCommand(arrayReply);

	for (int n = 0; n < arrayReply.GetSize(); n++)
	{
		CString strLine = arrayReply.GetAt(n);
		strLine += "\n";
		m_LoyReqThreadManager.AddResponseLine(strLine);
	}

	int nThread = m_LoyReqThreadManager.GetCurrentThread();

	m_LoyRspThreadManager.QueueResponseHeader(nThread);

	for (int n = 0; n < m_LoyReqThreadManager.GetResponseLineCount(); n++)
	{
		m_LoyRspThreadManager.QueueResponseLine(nThread, m_LoyReqThreadManager.GetResponseLine(n));
	}

	m_LoyLogThreadManager.CheckThread(nThread);

	{
		CString strMsg;
		strMsg.Format("<== EXT.RSP: %s", 
			(const char*) m_LoyReqThreadManager.GetRSPFilepath());

		MessageLogger.LogSystemMessage(strMsg);
	}
}

//**********************************************************************
