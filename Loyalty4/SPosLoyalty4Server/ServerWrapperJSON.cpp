/**********************************************************************/
#include "CkJsonObject.h"
#include "CkJsonArray.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SPosLoyalty4Shared\SQLTable_Scheme\SQLRepositoryScheme.h"
#include "..\SPosLoyalty4Shared\SQLTable_Scheme\SQLRecordSetScheme.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\AuditRecord_base.h"
#include "..\SPosLoyalty4Shared\LoyaltyAuditFile.h"
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\MessageLogger.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\SPosLoyalty4Shared\UserTexts.h"
/**********************************************************************/
#include "ServerWrapper.h"
/**********************************************************************/

void CServerWrapper::ProcessJSONRequest()
{
	if (Server.GetLogActionFlag() == TRUE)
	{
		CString strMsg;
		strMsg.Format("==> REQ: %s",
			(const char*)m_LoyReqThreadManager.GetREQFilepath());

		MessageLogger.LogSystemMessage(strMsg);
	}

	{
		CJSONObjectCollector JSONObjectCollector;
		ProcessJSONRequestInternal(JSONObjectCollector);
	}

	if (m_LoyReqThreadManager.GetResponseLineCount() > 2)
	{
		CServerDataFolderSetIndex FolderSetIndex = m_LoyReqThreadManager.GetThreadIndex();

		m_LoyRspThreadManager.QueueResponseHeader(FolderSetIndex);
		for (int n = 0; n < m_LoyReqThreadManager.GetResponseLineCount(); n++)
		{
			m_LoyRspThreadManager.QueueResponseLine(FolderSetIndex, m_LoyReqThreadManager.GetResponseLine(n));
		}
		m_LoyRspThreadManager.CheckThread(FolderSetIndex);

		if (Server.GetLogActionFlag() == TRUE)
		{
			CString strMsg = "";
			strMsg += "<== RSP: ";
			strMsg += m_LoyReqThreadManager.GetResponseLine(1);
			MessageLogger.LogSystemMessage(strMsg);
		}
	}
}

/**********************************************************************/

void CServerWrapper::ProcessJSONRequestInternal(CJSONObjectCollector& JSONObjectCollector)
{
	CString strJSON = "";
	for (int n = 0; n < m_LoyReqThreadManager.GetRequestLineCount(); n++)
	{
		strJSON += m_LoyReqThreadManager.GetRequestLine(n);
	}

	CkJsonObject jsonReply;

	CkJsonObject* jsonReplyMessage = jsonReply.AppendObject("message");

	if (jsonReplyMessage == NULL)
	{
		m_LoyReqThreadManager.AddResponseLine("0");
		return;
	}
	JSONObjectCollector.RememberJSONObject(jsonReplyMessage);

	jsonReplyMessage->AppendInt("version", 1);
	jsonReplyMessage->AppendString("entity", "error");
	jsonReplyMessage->AppendString("messageType", "response");
	jsonReplyMessage->AppendInt("status", 400);

	CkJsonObject* jsonReplyData = jsonReplyMessage->AppendObject("data");

	if (jsonReplyData == NULL)
	{
		m_LoyReqThreadManager.AddResponseLine("0");
		return;
	}
	JSONObjectCollector.RememberJSONObject(jsonReplyData);

	CkJsonObject jsonRequest;

	if (jsonRequest.Load(strJSON) == FALSE)
	{
		ApplyJSONError(jsonReplyData, 400, "Invalid JSON data");
		SaveJSONReply(jsonReply);
		return;
	}

	CkJsonObject* jsonRequestMessage = jsonRequest.ObjectOf("message");

	if (NULL == jsonRequestMessage)
	{
		ApplyJSONError(jsonReplyData, 400, "No message block found");
		SaveJSONReply(jsonReply);
		return;
	}
	JSONObjectCollector.RememberJSONObject(jsonRequestMessage);

	//CHECK THE REQUEST ENTITY TYPE AND SET THE REPLY ENTITY TYPE
	int nEntityType = 0;
	CString strError = "";
	CString strSuccessReply = "";

	{
		CString strEntity = "";
		if (CheckJSONParam(jsonRequestMessage, "entity", strEntity) == FALSE)
		{
			strError = "No entity specified";
		}

		strEntity.MakeUpper();
		if (strEntity == "GETCONFIGREQUEST")
		{
			nEntityType = 1;
			strSuccessReply = "GetConfigResponse";
		}
		else if (strEntity == "CREATEACCOUNTREQUEST")
		{
			nEntityType = 2;
			strSuccessReply = "CreateAccountResponse";
		}
		else
		{
			strError = "Unknown entity type";
		}
	}

	//CHECK REQUEST VERSION
	if (strError == "")
	{
		CString strVersion = "";
		if (CheckJSONParam(jsonRequestMessage, "version", strVersion) == FALSE)
		{
			strError = "No version specified";
		}
		else if (strVersion != "1")
		{
			strError = "Unsupported version";
		}
	}

	//CHECK REQUEST MESSAGE TYPE
	if (strError == "")
	{
		CString strMessageType = "";
		if (CheckJSONParam(jsonRequestMessage, "messageType", strMessageType) == FALSE)
		{
			strError = "No message type specified";
		}
		else
		{
			strMessageType.MakeUpper();

			if (strMessageType != "REQUEST")
			{
				strError = "Unsupported message type";
			}
		}
	}

	//QUIT IF THERE IS AN ERROR WITH BASIC PARAMETERS
	if (strError != "")
	{
		ApplyJSONError(jsonReplyData, 400, strError);
		SaveJSONReply(jsonReply);
		return;
	}

	//PROCESS THE REQUEST BASED ON THE ENTITY TYPE
	bool bJSONCreationError = FALSE;
	{
		CJSONObjectCollector JSONObjectCollector2;

		switch (nEntityType)
		{
		case 1:
			ProcessJSONGetConfig(JSONObjectCollector2, jsonReplyData, bJSONCreationError);
			break;

		case 2:
		default:
			ProcessJSONCreateAccount(JSONObjectCollector2, jsonRequestMessage, jsonReplyData, strError);
			break;
		}
	}

	//SAVE REPLY OR RETURN ERROR
	if (TRUE == bJSONCreationError)
	{
		m_LoyReqThreadManager.AddResponseLine("0");
	}
	else if (strError == "")
	{
		jsonReplyMessage->UpdateInt("status", 200);
		jsonReplyMessage->UpdateString("entity", strSuccessReply);
		SaveJSONReply(jsonReply);
	}
	else
	{
		ApplyJSONError(jsonReplyData, 400, strError);
		SaveJSONReply(jsonReply);
	}
}

/**********************************************************************/

void CServerWrapper::ProcessJSONGetConfig(CJSONObjectCollector& JSONObjectCollector, CkJsonObject* jsonReplyData, bool& bJSONCreationError)
{
	CUserTexts UserTexts;
	UserTexts.Read();
	
	bJSONCreationError = FALSE;

	//GROUPS
	if (jsonReplyData->AddArrayAt(-1, "groups") == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	CkJsonArray* jsonArrayGroup = jsonReplyData -> ArrayAt(jsonReplyData -> get_Size() - 1);

	if (jsonArrayGroup == NULL)
	{
		bJSONCreationError = TRUE;
		return;
	}
	JSONObjectCollector.RememberJSONArray(jsonArrayGroup);

	CSQLRecordSetGroup recordSetGroup(NULL, RSParams_Group_NormalNoParams{});
	CSQLRowGroup rowGroup;

	while (recordSetGroup.StepSelectAll(rowGroup) == TRUE)
	{
		if (jsonArrayGroup->AddObjectAt(-1) == TRUE)
		{
			CkJsonObject* jsonGroup = jsonArrayGroup->ObjectAt(jsonArrayGroup->get_Size() - 1);

			if (jsonGroup != NULL)
			{
				jsonGroup->AppendInt(Group::GroupNo.Label, rowGroup.GetGroupNo());
				jsonGroup->AppendString(Group::GroupName.Label, rowGroup.GetGroupName());
				delete jsonGroup;
			}
			else
			{
				bJSONCreationError = TRUE;
				return;
			}
		}
		else
		{
			bJSONCreationError = TRUE;
			return;
		}
	}

	//SCHEMES
	if (jsonReplyData->AddArrayAt(-1, "schemes") == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	CkJsonArray* jsonArrayScheme = jsonReplyData->ArrayAt(jsonReplyData->get_Size() - 1);

	if (jsonArrayScheme == NULL)
	{
		bJSONCreationError = TRUE;
		return;
	}
	JSONObjectCollector.RememberJSONArray(jsonArrayScheme);

	{
		CSQLRepositoryScheme PrepStatScheme;
		CReportConsolidationArray<CSortedStringByInt> arraySchemeNames;
		PrepStatScheme.GetSchemeNameList(arraySchemeNames, 0, TRUE, NULL);

		CSortedStringByInt itemScheme;
		itemScheme.m_nItem = 0;
		itemScheme.m_strItem = "Use Group Setting";
		arraySchemeNames.InsertAt(0, itemScheme);

		for (int n = 0; n < arraySchemeNames.GetSize(); n++)
		{
			if (jsonArrayScheme->AddObjectAt(-1) == TRUE)
			{
				arraySchemeNames.GetAt(n, itemScheme);

				CkJsonObject* jsonScheme = jsonArrayScheme->ObjectAt(jsonArrayScheme->get_Size() - 1);
				if (jsonScheme != NULL)
				{
					jsonScheme->AppendInt(Scheme::SchemeNo.Label, itemScheme.m_nItem);
					jsonScheme->AppendString(Scheme::SchemeName.Label, itemScheme.m_strItem);
					delete jsonScheme;
				}
				else
				{
					bJSONCreationError = TRUE;
					return;
				}
			}
			else
			{
				bJSONCreationError = TRUE;
				return;
			}
		}
	}

	//FIELDS
	if (jsonReplyData->AddArrayAt(-1, "fields") == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	CkJsonArray* jsonArrayFields = jsonReplyData->ArrayAt(jsonReplyData->get_Size() - 1);

	if (jsonArrayFields == NULL)
	{
		bJSONCreationError = TRUE;
		return;
	}
	
	JSONObjectCollector.RememberJSONArray(jsonArrayFields);

	const recString AccountAsString{ Account::UserID.Label, 1, System.GetMaxCardNoLength() };

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(AccountAsString, LOYALTYFIELD_USERID)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(Account::Fullname, LOYALTYFIELD_FULLNAME)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(Account::Forename, LOYALTYFIELD_FORENAME)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(Account::Surname, LOYALTYFIELD_SURNAME)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(Account::Gender, LOYALTYFIELD_GENDER)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(Account::GroupNo, LOYALTYFIELD_GROUPNO)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(Account::SchemeNo, LOYALTYFIELD_SCHEME)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(Account::ExpiryDate, LOYALTYFIELD_EXPIRYDATE)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(Account::Address1, LOYALTYFIELD_ADDRESS1)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(Account::Address2, LOYALTYFIELD_ADDRESS2)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(Account::Address3, LOYALTYFIELD_ADDRESS3)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(Account::Address4, LOYALTYFIELD_ADDRESS4)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(Account::Address5, LOYALTYFIELD_ADDRESS5)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(Account::Phone1, LOYALTYFIELD_PHONE1)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(Account::Phone2, LOYALTYFIELD_PHONE2)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(Account::DOB, LOYALTYFIELD_DOB)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(Account::ExpiryDate, LOYALTYFIELD_EXPIRYDATE)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(Account::Info1, LOYALTYFIELD_INFO1)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(Account::Info2, LOYALTYFIELD_INFO2)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(Account::Info3, LOYALTYFIELD_INFO3)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}

	if (AddJSONFieldInfo(UserTexts, jsonArrayFields, CJSONFieldInfo(Account::Info4, LOYALTYFIELD_INFO4)) == FALSE)
	{
		bJSONCreationError = TRUE;
		return;
	}
}

/**********************************************************************/

bool CServerWrapper::AddJSONFieldInfo(CUserTexts& UserTexts, CkJsonArray* jsonArrayFields, const CJSONFieldInfo& fieldInfo)
{
	bool bResult = TRUE;

	int nType = 0;
	if (fieldInfo.m_strType == "int") nType = 1;
	if (fieldInfo.m_strType == "double") nType = 2;
	if (fieldInfo.m_strType == "string") nType = 3;

	if ( nType != 0 )
	{
		CString strLabelName = UserTexts.GetText(fieldInfo.m_nLoyaltyField, "");

		if (strLabelName == "")
		{
			strLabelName = fieldInfo.m_strFieldName;
		}

		bool bIsDate = FALSE;
		switch (fieldInfo.m_nLoyaltyField)
		{
		case LOYALTYFIELD_DOB:
		case LOYALTYFIELD_EXPIRYDATE:
			bIsDate = TRUE;
			break;

		case LOYALTYFIELD_INFO1:
		case LOYALTYFIELD_INFO2:
		case LOYALTYFIELD_INFO3:
		case LOYALTYFIELD_INFO4:
			bIsDate = UserTexts.GetDateFlag(fieldInfo.m_nLoyaltyField);
			break;
		}

		bool bIsCompulsory = FALSE;
		switch (fieldInfo.m_nLoyaltyField)
		{
		case LOYALTYFIELD_USERID:
			bIsCompulsory = TRUE;
			break;

		default:
			bIsCompulsory = UserTexts.GetCompulsoryFlag(fieldInfo.m_nLoyaltyField);
			break;
		}

		if (jsonArrayFields->AddObjectAt(-1) == TRUE)
		{
			CkJsonObject* jsonField = jsonArrayFields->ObjectAt(jsonArrayFields->get_Size() - 1);

			jsonField->AppendString("fieldName", fieldInfo.m_strFieldName);
			jsonField->AppendString("labelName", strLabelName);
			jsonField->AppendString("type", fieldInfo.m_strType);
			jsonField->AppendBool("compulsory", bIsCompulsory);
				
			if (jsonField != NULL)
			{
				switch (nType)
				{
				case 1:
					jsonField->AppendInt("minInt", fieldInfo.m_nMinInt);
					jsonField->AppendInt("maxInt", fieldInfo.m_nMaxInt);
					break;

				case 2:
					{
						CString strNumber = "";
						strNumber.Format("%.2f", fieldInfo.m_dMinDouble);
						jsonField->AddNumberAt(-1,"minDouble", strNumber);
						strNumber.Format("%.2f", fieldInfo.m_dMaxDouble);
						jsonField->AddNumberAt(-1, "maxDouble", strNumber);
					}
					break;

				case 3:
					jsonField->AppendBool("isDate", bIsDate);
					jsonField->AppendInt("minLength", fieldInfo.m_nMinLength);
					jsonField->AppendInt("maxLength", fieldInfo.m_nMaxLength);
					break;
				}

				delete jsonField;
			}
			else
			{
				bResult = FALSE;
			}
		}
		else
		{
			bResult = FALSE;
		}
	}
	
	return bResult;
}

/**********************************************************************/

void CServerWrapper::ProcessJSONCreateAccount(CJSONObjectCollector& JSONObjectCollector, CkJsonObject* jsonRequestMessage, CkJsonObject* jsonReplyData, CString& strError)
{
	CUserTexts UserTexts;
	UserTexts.Read();
	bool bDateInfo1 = UserTexts.GetDateFlag(LOYALTYFIELD_INFO1);
	bool bDateInfo2 = UserTexts.GetDateFlag(LOYALTYFIELD_INFO2);
	bool bDateInfo3 = UserTexts.GetDateFlag(LOYALTYFIELD_INFO3);
	bool bDateInfo4 = UserTexts.GetDateFlag(LOYALTYFIELD_INFO4);

	strError = "";

	CkJsonObject* jsonRequestData = jsonRequestMessage->ObjectOf("data");

	if (jsonRequestData == NULL)
	{
		strError = "No data section found";
		return;
	}
	JSONObjectCollector.RememberJSONObject(jsonRequestData);

	CString strUserID = jsonRequestData->stringOf(Account::UserID.Label);

	if (strUserID == "")
	{
		strError = "No UserID specified";
		return;
	}

	__int64 nUserID = 0;

	if (strUserID == "-1")
	{
		CSQLRepositoryAccount repoAccount;
		nUserID = repoAccount.GetNextFreeUserID(1, _atoi64(System.GetHighCardNo()), NULL).GetSQLResult();

		if (nUserID == 0)
		{
			strError = "Unable to assign new user ID";
			return;
		}
	}
	else
	{
		if (::TestNumeric(strUserID) == FALSE)
		{
			strError = "Non numeric UserID specified";
			return;
		}

		nUserID = _atoi64(strUserID);
	}

	if ((nUserID < Account::UserID.Min) || (nUserID > Account::UserID.Max))
	{
		strError = "Invalid UserID specified";
		return;
	}

	strUserID.Format("%I64d", nUserID);
	if (strUserID.GetLength() > System.GetMaxCardNoLength())
	{
		strError = "UserID too long";
		return;
	}

	CSQLRepositoryAccount repoAccount;
	if (repoAccount.RowExists(nUserID, NULL).GetSQLResult() != 0)
	{
		strError = "UserID already exists";
		return;
	}

	bool bGotFullname = FALSE;
	CString strFullname = "";
	CheckJSONString(jsonRequestData, Account::Fullname, bGotFullname, strFullname, strError);
	if (strError != "")
	{
		return;
	}

	bool bGotForename = FALSE;
	CString strForename = "";
	CheckJSONString(jsonRequestData, Account::Forename, bGotForename, strForename, strError);
	if (strError != "")
	{
		return;
	}

	bool bGotSurname = FALSE;
	CString strSurname = "";
	CheckJSONString(jsonRequestData, Account::Surname, bGotSurname, strSurname, strError);
	if (strError != "")
	{
		return;
	}

	bool bGotGender = FALSE;
	CString strGender = "";
	CheckJSONString(jsonRequestData, Account::Gender, bGotGender, strGender, strError);
	if (strError != "")
	{
		return;
	}

	if (TRUE == bGotGender)
	{
		strGender.MakeUpper();
		if ((strGender != "M") && (strGender != "F") && (strGender != ""))
		{
			strError = "Unknown gender text";
			return;
		}
	}

	bool bGotGroupNo = FALSE;
	int nGroupNo = 0;
	CheckJSONInt(jsonRequestData, Account::GroupNo, bGotGroupNo, nGroupNo, strError);
	if (strError != "")
	{
		return;
	}

	if (TRUE == bGotGroupNo)
	{
		CSQLRepositoryGroup repoGroup;
		if (repoGroup.RowExists(nGroupNo, NULL).GetSQLResult() == 0)
		{
			strError = "The specified group does not exist";
			return;
		}
	}

	bool bGotSchemeNo = FALSE;
	int nSchemeNo = 0;
	CheckJSONInt(jsonRequestData, Account::SchemeNo, bGotSchemeNo, nSchemeNo, strError);
	if (strError != "")
	{
		return;
	}

	if (TRUE == bGotSchemeNo)
	{
		if (nSchemeNo > 10)
		{
			CSQLRepositoryScheme PrepStatScheme;
			if (PrepStatScheme.RowExists(nSchemeNo, NULL).GetSQLResult() == 0)
			{
				strError = "The specified scheme does not exist";
				return;
			}
		}
	}

	bool bGotAddress1 = FALSE;
	CString strAddress1 = "";
	CheckJSONString(jsonRequestData, Account::Address1, bGotAddress1, strAddress1, strError);
	if (strError != "")
	{
		return;
	}

	bool bGotAddress2 = FALSE;
	CString strAddress2 = "";
	CheckJSONString(jsonRequestData, Account::Address2, bGotAddress2, strAddress2, strError);
	if (strError != "")
	{
		return;
	}

	bool bGotAddress3 = FALSE;
	CString strAddress3 = "";
	CheckJSONString(jsonRequestData, Account::Address3, bGotAddress3, strAddress3, strError);
	if (strError != "")
	{
		return;
	}

	bool bGotAddress4 = FALSE;
	CString strAddress4 = "";
	CheckJSONString(jsonRequestData, Account::Address4, bGotAddress4, strAddress4, strError);
	if (strError != "")
	{
		return;
	}

	bool bGotAddress5 = FALSE;
	CString strAddress5 = "";
	CheckJSONString(jsonRequestData, Account::Address5, bGotAddress5, strAddress5, strError);
	if (strError != "")
	{
		return;
	}

	bool bGotPhone1 = FALSE;
	CString strPhone1 = "";
	CheckJSONString(jsonRequestData, Account::Phone1, bGotPhone1, strPhone1, strError);
	if (strError != "")
	{
		return;
	}

	bool bGotPhone2 = FALSE;
	CString strPhone2 = "";
	CheckJSONString(jsonRequestData, Account::Phone2, bGotPhone2, strPhone2, strError);
	if (strError != "")
	{
		return;
	}

	bool bGotDOB = FALSE;
	CString strDOB = "";
	CheckJSONDate(jsonRequestData, Account::DOB, TRUE, bGotDOB, strDOB, strError);
	if (strError != "")
	{
		return;
	}

	bool bGotExpiry = FALSE;
	CString strExpiry = "";
	CheckJSONDate(jsonRequestData, Account::ExpiryDate, TRUE, bGotExpiry, strExpiry, strError);
	if (strError != "")
	{
		return;
	}

	bool bGotInfo1 = FALSE;
	CString strInfo1 = "";
	CheckJSONDate(jsonRequestData, Account::Info1, bDateInfo1, bGotInfo1, strInfo1, strError);
	if (strError != "")
	{
		return;
	}

	bool bGotInfo2 = FALSE;
	CString strInfo2 = "";
	CheckJSONDate(jsonRequestData, Account::Info2, bDateInfo2, bGotInfo2, strInfo2, strError);
	if (strError != "")
	{
		return;
	}

	bool bGotInfo3 = FALSE;
	CString strInfo3 = "";
	CheckJSONDate(jsonRequestData, Account::Info3, bDateInfo3, bGotInfo3, strInfo3, strError);
	if (strError != "")
	{
		return;
	}

	bool bGotInfo4 = FALSE;
	CString strInfo4 = "";
	CheckJSONDate(jsonRequestData, Account::Info4, bDateInfo4, bGotInfo4, strInfo4, strError);
	if (strError != "")
	{
		return;
	}

	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserID(nUserID);

	if (TRUE == bGotGroupNo)
	{
		RowAccount.SetGroupNo(nGroupNo);
	}

	if (TRUE == bGotSchemeNo)
	{
		RowAccount.SetSchemeNo(nSchemeNo);
	}

	if (TRUE == bGotFullname)
	{
		RowAccount.SetFullname(strFullname);
	}

	if (TRUE == bGotForename)
	{
		RowAccount.SetForename(strForename);
	}

	if (TRUE == bGotSurname)
	{
		RowAccount.SetSurname(strSurname);
	}

	if (TRUE == bGotGender)
	{
		RowAccount.SetGender(strGender);
	}

	if (TRUE == bGotDOB)
	{
		RowAccount.SetDOB(strDOB);
	}

	if (TRUE == bGotExpiry)
	{
		RowAccount.SetExpiryDate(strExpiry);
	}

	if (TRUE == bGotAddress1)
	{
		RowAccount.SetAddress1(strAddress1);
	}

	if (TRUE == bGotAddress2)
	{
		RowAccount.SetAddress2(strAddress2);
	}

	if (TRUE == bGotAddress3)
	{
		RowAccount.SetAddress3(strAddress3);
	}

	if (TRUE == bGotAddress4)
	{
		RowAccount.SetAddress4(strAddress4);
	}

	if (TRUE == bGotAddress5)
	{
		RowAccount.SetAddress5(strAddress5);
	}

	if (TRUE == bGotPhone1)
	{
		RowAccount.SetPhone1(strPhone1);
	}

	if (TRUE == bGotPhone2)
	{
		RowAccount.SetPhone2(strPhone2);
	}

	if (TRUE == bGotInfo1)
	{
		RowAccount.SetInfo1(strInfo1);
	}

	if (TRUE == bGotInfo2)
	{
		RowAccount.SetInfo2(strInfo2);
	}

	if (TRUE == bGotInfo3)
	{
		RowAccount.SetInfo3(strInfo3);
	}

	if (TRUE == bGotInfo4)
	{
		RowAccount.SetInfo4(strInfo4);
	}

	jsonReplyData->AppendString(Account::UserID.Label, RowAccount.GetUserIDString());
	jsonReplyData->AppendInt(Account::GroupNo.Label, RowAccount.GetGroupNo());
	jsonReplyData->AppendInt(Account::SchemeNo.Label, RowAccount.GetSchemeNo());
	jsonReplyData->AppendString(Account::Fullname.Label, RowAccount.GetFullname());
	jsonReplyData->AppendString(Account::Forename.Label, RowAccount.GetForename());
	jsonReplyData->AppendString(Account::Surname.Label, RowAccount.GetSurname());
	jsonReplyData->AppendString(Account::Gender.Label, RowAccount.GetGender());
	jsonReplyData->AppendString(Account::DOB.Label, RowAccount.GetDOB());
	jsonReplyData->AppendString(Account::ExpiryDate.Label, RowAccount.GetExpiryDate());
	jsonReplyData->AppendString(Account::Address1.Label, RowAccount.GetAddress1());
	jsonReplyData->AppendString(Account::Address2.Label, RowAccount.GetAddress2());
	jsonReplyData->AppendString(Account::Address3.Label, RowAccount.GetAddress3());
	jsonReplyData->AppendString(Account::Address4.Label, RowAccount.GetAddress4());
	jsonReplyData->AppendString(Account::Address5.Label, RowAccount.GetAddress5());
	jsonReplyData->AppendString(Account::Phone1.Label, RowAccount.GetPhone1());
	jsonReplyData->AppendString(Account::Phone2.Label, RowAccount.GetPhone2());
	jsonReplyData->AppendString(Account::Info1.Label, RowAccount.GetInfo1());
	jsonReplyData->AppendString(Account::Info2.Label, RowAccount.GetInfo2());
	jsonReplyData->AppendString(Account::Info3.Label, RowAccount.GetInfo3());
	jsonReplyData->AppendString(Account::Info4.Label, RowAccount.GetInfo4());

	if (repoAccount.InsertRow(RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		strError = "Unable to create account record";
	}
	else
	{
		CAuditRecord_base atc;
		atc.SetSQL(RowAccount);
		atc.SetSourceType(AUDIT_EXT);
		atc.SetApplicationNo(APPNO_INSERT);
		atc.SaveLineToFile();
	}
}

/**********************************************************************/

void CServerWrapper::CheckJSONDate(CkJsonObject* jsonParent, const recString field, bool bIsDate, bool& bGotValue, CString& strValue, CString& strError)
{
	strValue = "";
	strError = "";
	bGotValue = FALSE;
	CheckJSONString(jsonParent, field, bGotValue, strValue, strError);

	if ((FALSE == bGotValue) || (strError != "") || (strValue == "") || (FALSE == bIsDate))
	{
		return;
	}

	strValue = ::ConvertISO8601ToSimpleDateTime(strValue,TRUE);
	
	if (strValue == "")
	{
		strError.Format("Invalid %s date field",
			(const char*)field.Label);
	}
	else
	{
		CString strTemp = "";
		strTemp.Format("%s/%s/%s",
			(const char*)strValue.Mid(6, 2),
			(const char*)strValue.Mid(4, 2),
			(const char*)strValue.Left(4));
		strValue = strTemp;
	}
}

/**********************************************************************/

bool CServerWrapper::CheckJSONParam(CkJsonObject* jsonParent, CString strLabel, CString& strValue)
{
	strValue = "";
	bool bResult = FALSE;

	if (jsonParent->IndexOf(strLabel) != -1)
	{
		bResult = TRUE;
		strValue = jsonParent->stringOf(strLabel);
	}

	return bResult;
}

/**********************************************************************/

void CServerWrapper::CheckJSONString(CkJsonObject* jsonParent, const recString field, bool& bGotValue, CString& strValue, CString& strError)
{
	strValue = "";
	strError = "";
	bGotValue = FALSE;

	if (jsonParent->IndexOf(field.Label) != -1)
	{
		bGotValue = TRUE;
		strValue = jsonParent->stringOf(field.Label);
		
		if (strValue.GetLength() > field.Max)
		{
			strError.Format("%s too long",
				(const char*)field.Label);
		}
	}
}

/**********************************************************************/

void CServerWrapper::CheckJSONInt(CkJsonObject* jsonParent, const recInt field, bool& bGotValue, int& nValue, CString& strError)
{
	nValue = 0;
	bGotValue = FALSE;
	strError = "";

	if (jsonParent->IndexOf(field.Label) != -1)
	{
		bGotValue = TRUE;
		nValue = jsonParent->IntOf(field.Label);

		if ((nValue < field.Min) || (nValue > field.Max))
		{
			strError.Format("%s out of range",
				(const char*)field.Label);
		}
	}
}

/**********************************************************************/

void CServerWrapper::ApplyJSONError(CkJsonObject* jsonReplyData, int nErrorCode, CString strErrorText)
{
	jsonReplyData->AppendInt("ErrorCode", nErrorCode);
	jsonReplyData->AppendString("Message", strErrorText);
}

/**********************************************************************/

void CServerWrapper::SaveJSONReply(CkJsonObject& jsonReply)
{
	jsonReply.put_EmitCompact(FALSE);
	CString strReply = jsonReply.emit();
	m_LoyReqThreadManager.AddResponseLine(strReply);
}

/**********************************************************************/


