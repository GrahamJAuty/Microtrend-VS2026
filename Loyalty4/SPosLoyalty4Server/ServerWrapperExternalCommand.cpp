//**********************************************************************
#include "..\SPosLoyalty4Shared\AccountFieldValidator.h"
#include "..\SPosLoyalty4Shared\AESHelpers.h"
#include "..\SPosLoyalty4Shared\MessageLogger.h"
#include "..\SPosLoyalty4Shared\SQLDefines.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_ExternalAccount\SQLRepositoryExternalAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
//**********************************************************************
#include "EposLoyaltyFile.h"
#include "ExternalCommandSigner.h"
#include "SPosLoyalty4Server.h"
#include "SQLStaffAccountBuffer.h"
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

	if (Server.GetLogActionFlag() == TRUE)
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
		
		CSQLRepositoryExternalAccount repoExt;
		if (repoExt.SelectRow(RowExternal, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
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
				RowAccount.SetUserIDString(strAccountID);

				CSQLRepositoryAccount repoAccount;
				if (repoAccount.SelectRow(RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
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
		SQLStaffAccountBuffer.Reset();

		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(_atoi64(m_ExternalParamsArray.m_strUserID));

		CSQLRepositoryAccount repoAccount;
		if ( repoAccount.SelectRow(RowAccount,NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			arrayReply.CreateErrorReply("203", "Loyalty account not found");
		}
		else
		{
			if (TRUE == bDetail)
			{
				arrayReply.AddOKReply("UserID", RowAccount.GetUserIDString());
				arrayReply.AddOKReply("Forename", RowAccount.GetForename());
				arrayReply.AddOKReply("Surname", RowAccount.GetSurname());
				arrayReply.AddOKReply("Gender", RowAccount.GetGenderText());
				arrayReply.AddOKReply("DOB", RowAccount.GetDOB());
				arrayReply.AddOKReply("Address1", RowAccount.GetAddress1());
				arrayReply.AddOKReply("Address2", RowAccount.GetAddress2());
				arrayReply.AddOKReply("Address3", RowAccount.GetAddress3());
				arrayReply.AddOKReply("Address4", RowAccount.GetAddress4());
				arrayReply.AddOKReply("Address5", RowAccount.GetAddress5());
				arrayReply.AddOKReply("Phone1", RowAccount.GetPhone1());
				arrayReply.AddOKReply("Phone2", RowAccount.GetPhone2());
				arrayReply.AddOKReply("GroupNumber", RowAccount.GetGroupNoString());
				arrayReply.AddOKReply("Info1", RowAccount.GetInfo1());
				arrayReply.AddOKReply("Info2", RowAccount.GetInfo2());
				arrayReply.AddOKReply("Info3", RowAccount.GetInfo3());
				arrayReply.AddOKReply("Info4", RowAccount.GetInfo4());

				CString strUserName = "";

				CString strUserID = ::SimpleFormatInt64Value(RowAccount.GetUserID());
				CString strAESUserID = "";
				CAESHelpers::PlainTextToAESHex(strUserID, strAESUserID);

				CSQLRowExternalAccount RowExternal;
				RowExternal.SetAESUserID(strAESUserID);

				CSQLRepositoryExternalAccount repoExt;
				if (repoExt.SelectRowByUserID(RowExternal, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
				{
					CAESHelpers::AESHexToPlainText(RowExternal.GetAESUsername(), strUserName);
				}

				arrayReply.AddOKReply("ExtUsername", strUserName);
			}

			{
				CSQLRowGroup RowGroup;
				RowGroup.SetGroupNo(RowAccount.GetGroupNo());
				
				CSQLRepositoryGroup repoGroup;
				repoGroup.SelectRow(RowGroup,NULL);

				double dP1Balance = 0.0;
				double dP2Balance = 0.0;
				double dAvailableSpend = 0.0;
				CalculateAvailableSpend(RowAccount, RowGroup, dP1Balance, dP2Balance, dAvailableSpend);

				CString strBalance = "";
				strBalance.Format("%d", int(floor(dAvailableSpend * 100.0)));

				arrayReply.AddOKReply("Balance", strBalance);
			}
		}
	}

	CreateExternalReply(arrayReply);
}

//**********************************************************************

void CServerWrapper::CreateExternalPaymentReply()
{	
	CExternalReplyArray arrayReply{};
	bool bNeedRetry = FALSE;

	do
	{
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
			SQLStaffAccountBuffer.Reset();

			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(_atoi64(m_ExternalParamsArray.m_strUserID));

			CSQLRepositoryAccount repoAccount;
			if ( repoAccount.SelectRow(RowAccount,NULL).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				arrayReply.CreateErrorReply("403", "Loyalty account not found");
			}
			else if (RowAccount.IsCardHotlisted() == TRUE)
			{
				arrayReply.CreateErrorReply("405", "Loyalty account has been hotlisted");
			}
			else if (RowAccount.GetInactive() == TRUE)
			{
				arrayReply.CreateErrorReply("406", "Loyalty account is flagged as inactive");
			}
			else if (RowAccount.HasCardExpired() == TRUE)
			{
				arrayReply.CreateErrorReply("407", "Loyalty account expired", RowAccount.GetExpiryDate());
			}
			else
			{
				m_ExternalParamsArray.m_nLoyaltyAmount = max(m_ExternalParamsArray.m_nLoyaltyAmount, 0);
				m_ExternalParamsArray.m_nOtherAmount = max(m_ExternalParamsArray.m_nOtherAmount, 0);

				if (m_ExternalParamsArray.m_nLoyaltyAmount > 0)
				{
					CSQLRowGroup RowGroup;
					RowGroup.SetGroupNo(RowAccount.GetGroupNo());

					CSQLRepositoryGroup repoGroup;
					repoGroup.SelectRow(RowGroup, NULL);

					double dP1Balance = 0.0;
					double dP2Balance = 0.0;
					double dAvailableSpend = 0.0;
					CalculateAvailableSpend(RowAccount, RowGroup, dP1Balance, dP2Balance, dAvailableSpend);

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
					csv.Add(""); //FIND OUT WHAT THIS DOES
					csv.Add(""); //FIND OUT WHAT THIS DOES
					csv.Add(strDateNow);
					csv.Add(strTimeNow);
					csv.Add(0);
					arrayTran.Add(csv.GetLine());

					if (m_ExternalParamsArray.m_nLoyaltyAmount > 0)
					{
						csv.RemoveAll();
						csv.Add("$PAYM");
						csv.Add(14);
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

							arrayTran.Add(csv.GetLine());

							nPos += 4;
						}
					}

					CEposLoyaltyFile LoyaltyFile(&arrayTran);
					if (LoyaltyFile.ScanLogFileExternal() == FALSE)
					{
						arrayReply.CreateErrorReply("409", "Unable to process payment data");
					}
					else
					{
						LoyaltyFile.UpdateLoyaltyAccount(RowAccount, FALSE);

						CSQLRowAccountFull RowCheck;
						RowCheck.SetUserID(RowAccount.GetUserID());

						CSQLRepositoryAccount repoAccount;
						if (repoAccount.SelectRow(RowCheck, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
						{
							bNeedRetry = TRUE;

							if (RowAccount.CompareRowVersion(RowCheck) == TRUE)
							{
								bNeedRetry = FALSE;

								if (repoAccount.UpdateRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
								{
									LoyaltyFile.WriteAuditBuffer();
									LoyaltyFile.ProcessPurchaseHistory(RowAccount);

									CString strCCNo = "";
									strCCNo.Format("%d", LoyaltyFile.GetExternalCCNo());
									arrayReply.AddOKReply("CCNo", strCCNo);
								}
								else
								{
									arrayReply.CreateErrorReply("411", "Unable to save change to account");
								}
							}
						}
						else
						{
							arrayReply.CreateErrorReply("410", "Unable to check account for changes by other user");
						}
					}
				}
			}
		}
	}
	while (TRUE == bNeedRetry);
	
	return CreateExternalReply(arrayReply);
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

		CSQLRepositoryExternalAccount repoExt;
		if (repoExt.SelectRow(RowExternal, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			arrayReply.CreateErrorReply("504", "User name not found");
		}
		else
		{
			RowExternal.SetPasswordHash(m_ExternalParamsArray.m_strPassword);
			
			repoExt.UpdateRow(RowExternal,NULL);
			arrayReply.Add("OK");
		}
	}

	CreateExternalReply(arrayReply);
}

//**********************************************************************

void CServerWrapper::Diagnostic(CString strMessage)
{
	CString strMsg = GetTimeNowString();
	strMsg += " ";
	strMsg += strMessage;

	CSSFile log;
	if (log.Open(".\\diagnose.txt", "ab") == TRUE )
	{
		log.WriteLine(strMsg);
	}
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

		{
			CSQLRepositoryExternalAccount repoExt;
			switch (repoExt.SelectRow(RowExternal, NULL).GetSQLError())
			{
			case SQLCRUD_ERR_NONE:
			case SQLCRUD_ERR_DUPLICATE:
				arrayReply.CreateErrorReply("604", "Username already exists");
				break;
			}
		}

		__int64 nNewUserID = 0;
		if (arrayReply.GetSize() == 0)
		{
			CSQLRepositoryAccount repoAccount;
			nNewUserID = repoAccount.GetNextFreeUserID(1, System.GetMaxCardNo(),NULL).GetSQLResult();
			
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
			RowAccount.SetAddress1(m_ExternalParamsArray.m_strAddress1);
			RowAccount.SetAddress2(m_ExternalParamsArray.m_strAddress2);
			RowAccount.SetAddress3(m_ExternalParamsArray.m_strAddress3);
			RowAccount.SetAddress4(m_ExternalParamsArray.m_strAddress4);
			RowAccount.SetAddress5(m_ExternalParamsArray.m_strAddress5);
			RowAccount.SetPhone1(m_ExternalParamsArray.m_strPhone1);
			RowAccount.SetPhone2(m_ExternalParamsArray.m_strPhone2);
			
			{
				CString strFullname = m_ExternalParamsArray.m_strForename;

				if (m_ExternalParamsArray.m_strSurname != "")
				{
					if (strFullname != "")
					{
						strFullname += " ";
					}
					strFullname += m_ExternalParamsArray.m_strSurname;
				}

				RowAccount.SetFullname(strFullname);
			}

			if (m_ExternalParamsArray.m_strGender == "F")
			{
				RowAccount.SetGender(2);
			}
			else if (m_ExternalParamsArray.m_strGender == "M")
			{
				RowAccount.SetGender(1);
			}
			else
			{
				RowAccount.SetGender(0);
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

			CSQLRepositoryAccount repoAccount;
			if (repoAccount.InsertRow(RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				arrayReply.CreateErrorReply("605", "Unable to create Loyalty Account");
			}
			else
			{
				CSQLRowExternalAccount ExternalRow;
				ExternalRow.SetAESUsername(strAESUserName);
				ExternalRow.SetPasswordHash(m_ExternalParamsArray.m_strPassword);

				CString strCardNo = ::SimpleFormatInt64Value(nNewUserID);

				CString strAESCardNo = "";
				CAESHelpers::PlainTextToAESHex(strCardNo, strAESCardNo);
				ExternalRow.SetAESUserID(strAESCardNo);

				CSQLRepositoryExternalAccount repoExt;
				repoExt.InsertRow(ExternalRow,NULL);
				
				arrayReply.AddOKReply("UserID", strCardNo);
				arrayReply.AddOKReply("Forename", RowAccount.GetForename());
				arrayReply.AddOKReply("Surname", RowAccount.GetSurname());
				arrayReply.AddOKReply("Gender", RowAccount.GetGenderText());
				arrayReply.AddOKReply("DOB", RowAccount.GetDOB());
				arrayReply.AddOKReply("Address1", RowAccount.GetAddress1());
				arrayReply.AddOKReply("Address2", RowAccount.GetAddress2());
				arrayReply.AddOKReply("Address3", RowAccount.GetAddress3());
				arrayReply.AddOKReply("Address4", RowAccount.GetAddress4());
				arrayReply.AddOKReply("Address5", RowAccount.GetAddress5());
				arrayReply.AddOKReply("Phone1", RowAccount.GetPhone1());
				arrayReply.AddOKReply("Phone2", RowAccount.GetPhone2());
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
		CString strAESUsername = "";
		CAESHelpers::PlainTextToAESHex(m_ExternalParamsArray.m_strUserName, strAESUsername);

		bool bUserExists = FALSE;

		CSQLRowExternalAccount RowExternal;
		RowExternal.SetAESUsername(strAESUsername);

		CSQLRepositoryExternalAccount repoExt;
		if (repoExt.SelectRow(RowExternal, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
		{
			CString strAESUserID = RowExternal.GetAESUserID();

			CString strUserID = "";
			CAESHelpers::AESHexToPlainText(strAESUserID, strUserID);

			CSQLRepositoryAccount repoAccount;
			bUserExists = (repoAccount.RowExists(_atoi64(strUserID), NULL).GetSQLResult() != 0);
		}

		arrayReply.AddOKReply("USEREXISTS", bUserExists ? "1" : "0");
	}
	return CreateExternalReply(arrayReply);
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

	CServerDataFolderSetIndex FolderSetIndex = m_LoyReqThreadManager.GetThreadIndex();

	m_LoyRspThreadManager.QueueResponseHeader(FolderSetIndex);

	for (int n = 0; n < m_LoyReqThreadManager.GetResponseLineCount(); n++)
	{
		m_LoyRspThreadManager.QueueResponseLine( FolderSetIndex, m_LoyReqThreadManager.GetResponseLine(n));
	}

	m_LoyLogThreadManager.CheckThread(FolderSetIndex);

	if (Server.GetLogActionFlag() == TRUE)
	{
		CString strMsg;
		strMsg.Format("<== EXT.RSP: %s",
			(const char*) m_LoyReqThreadManager.GetRSPFilepath());

		MessageLogger.LogSystemMessage(strMsg);
	}
}

//**********************************************************************
