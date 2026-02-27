//$$******************************************************************
#include "CkJsonObject.h"
#include "CkJsonArray.h"
#include "CkStringBuilder.h"
//$$******************************************************************
#include "PebbleHelpers.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//$$******************************************************************

CPebblePurseBuffer::CPebblePurseBuffer()
{
	m_strPurseID = "";
	m_strMemberID = "";
	m_strTitle = "";
	m_strType = "";
	m_strBalance = "";
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

CString CPebbleHelpers::GetPurses(CSQLRepositoryPebbleConfig& PebbleConfig, CString strMemberID, CArray<CPebblePurseBuffer, CPebblePurseBuffer>& arrayPurses)
{
	arrayPurses.RemoveAll();

	CString strError = "";

	if (UpdateRestConnect(strError) == FALSE)
	{
		return strError;
	}

	CString strLastKey = "";

	int nMembers = 0;

	while (TRUE)
	{
		ClearRest();

		m_Rest.ClearAllQueryParams();
		m_Rest.AddQueryParam("per_page", "10");

		if (strLastKey != "")
		{
			m_Rest.AddQueryParam("start_key", strLastKey);
		}

		if (UpdateIAT(PebbleConfig, strError, NULL) == FALSE)
		{
			return strError;
		}

		m_Rest.ClearAllHeaders();
		m_Rest.AddHeader("accept", "application/json");
		m_Rest.AddHeader("authorization", GetIATAuth());

		CString strURL = "";
		strURL.Format("/api/v1/orgs/%s/members/%s/purses",
			(const char*)PebbleConfig.m_Row.GetActiveOrganisationID(),
			(const char*)strMemberID);

		CkStringBuilder sbResponseBody;
		bool success = m_Rest.FullRequestNoBodySb("GET", strURL, sbResponseBody);
		if (success != true)
		{
			LogRestError();
			strError = "Pebble API Get Purses Fail";
			Diagnostic(strError, TRUE, "Get Purse");
			return strError;
		}

		if (m_Rest.get_ResponseStatusCode() >= 400)
		{
			strError.Format("Pebble API Get Purses Error %d", m_Rest.get_ResponseStatusCode());
			Diagnostic(strError, TRUE, "Get Purse");
			return strError;
		}

		m_bDoneFirstAPICall = TRUE;

		CkJsonObject jsonResponse;
		jsonResponse.LoadSb(sbResponseBody);
		
		CkJsonArray* pJsonPurses = jsonResponse.ArrayOf("purses");

		if (pJsonPurses != NULL)
		{
			for (int n = 0; n < pJsonPurses->get_Size(); n++)
			{
				CkJsonObject* pPurse = pJsonPurses->ObjectAt(n);

				if (pPurse != NULL)
				{
					Diagnostic(pPurse, TRUE, "Get Purse");

					CPebblePurseBuffer Buffer;
					Buffer.m_strPurseID = pPurse->stringOf("meta.purseId");
					Buffer.m_strTitle = pPurse->stringOf("title");
					Buffer.m_strType = pPurse->stringOf("type");
					Buffer.m_strBalance = pPurse->stringOf("balanceAsInteger");
					arrayPurses.Add(Buffer);

					delete pPurse;
				}
			}

			delete pJsonPurses;
		}

		CkString lastkey;
		jsonResponse.StringOf("lastKey", lastkey);
		strLastKey = lastkey.getStringUtf8();

		if (strLastKey == "")
		{
			break;
		}
	}

	return "";
}

//$$******************************************************************

/*
void CPebbleHelpers::ZeroPurses(CSQLRepositoryPebbleConfig& PebbleConfig)
{
	CStringArray arrayMemberID;

	{
		CSQLRowAccountFull RowAccount;
		CSQLRecordSetAccountFull RecordSetAccount(NULL, ACCOUNTSETFULL_NORMAL_NOPARAMS);

		while (RecordSetAccount.StepSelectAll(RowAccount) == TRUE)
		{
			CString strMemberID = RowAccount.GetMemberID();

			if (strMemberID != "")
			{
				arrayMemberID.Add(strMemberID);
			}
		}
	}

	CArray<CPebblePurseBuffer, CPebblePurseBuffer> arrayPursesToClear;

	for (int n = 0; n < arrayMemberID.GetSize(); n++)
	{
		CArray<CPebblePurseBuffer, CPebblePurseBuffer> arrayMemberPurses;
		CString strMemberID = arrayMemberID[n];
		GetPurses(PebbleConfig, strMemberID, arrayMemberPurses);

		for (int x = 0; x < arrayMemberPurses.GetSize(); x++)
		{
			CPebblePurseBuffer Purse = arrayMemberPurses.GetAt(x);

			if ((Purse.m_strPurseID != "sales") && (Purse.m_strBalance != "0"))
			{
				Purse.m_strMemberID = strMemberID;
				arrayPursesToClear.Add(Purse);
			}
		}
	}

	if (arrayPursesToClear.GetSize() != 0)
	{
		SendZeroPursesTransactionBatch(PebbleConfig, arrayPursesToClear);
	}
}
*/

//$$******************************************************************

/*
CString CPebbleHelpers::DeleteCreditPurse(CSQLRepositoryPebbleConfig& PebbleConfig, CString strMemberID, CString strPurseID)
{
	CString strError = "";

	if (UpdateRestConnect(strError) == FALSE)
	{
		return strError;
	}

	ClearRest();

	if (UpdateIAT(PebbleConfig, strError) == FALSE)
	{
		return strError;
	}

	m_Rest.ClearAllHeaders();
	m_Rest.AddHeader("accept", "application/json");
	m_Rest.AddHeader("authorization", GetIATAuth());
	m_Rest.AddHeader("content-type", "application/json");

	CkJsonObject json;

	CString strURL = "";
	strURL.Format("/api/v1/orgs/%s/members/%s/purses/%s",
		(const char*)PebbleConfig.m_Row.GetActiveOrganisationID(),
		(const char*)strMemberID,
		(const char*)strPurseID);

	Diagnostic(&json, FALSE, "Delete Purse");

	CkStringBuilder sbRequestBody;
	json.EmitSb(sbRequestBody);
	CkStringBuilder sbResponseBody;

	bool success = m_Rest.FullRequestSb("DELETE", strURL, sbRequestBody, sbResponseBody);

	if (success != true)
	{
		LogRestError();
		strError = "Pebble Delete Purse Fail";
		Diagnostic(strError, TRUE, "Delete Purse");
		return strError;
	}

	if (m_Rest.get_ResponseStatusCode() >= 400)
	{
		strError.Format("Pebble API Delete Purse Error %d", m_Rest.get_ResponseStatusCode());
		Diagnostic(strError, TRUE, "Delete Purse");
		return strError;
	}

	CkJsonObject jsonResponse;
	jsonResponse.LoadSb(sbResponseBody);
	Diagnostic(&jsonResponse, TRUE, "Delete Purse");

	m_bDoneFirstAPICall = TRUE;

	return "";
}
*/

//$$******************************************************************
