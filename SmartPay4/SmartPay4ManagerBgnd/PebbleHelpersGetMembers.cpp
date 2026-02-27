//$$******************************************************************
#include "CkJsonObject.h"
#include "CkJsonArray.h"
#include "CkStringBuilder.h"
//$$******************************************************************
#include "PebbleHelpers.h"
//$$******************************************************************

CPebbleMemberBuffer::CPebbleMemberBuffer()
{
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

CString CPebbleHelpers::GetMembers(CSQLRepositoryPebbleConfig& PebbleConfig, CArray<CPebbleMemberBuffer, CPebbleMemberBuffer>& arrayMembers, CWorkingDlg* pWorkingDlg)
{
	arrayMembers.RemoveAll();
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
		m_Rest.AddQueryParam("per_page", "1");

		if (strLastKey != "")
		{
			m_Rest.AddQueryParam("start_key", strLastKey);
		}

		if (UpdateIAT(PebbleConfig, strError, pWorkingDlg) == FALSE)
		{
			return strError;
		}

		m_Rest.ClearAllHeaders();
		m_Rest.AddHeader("accept", "application/json");
		m_Rest.AddHeader("authorization", GetIATAuth());

		CString strURL = "";
		strURL.Format("/api/v1/orgs/%s/members", 
			(const char*) PebbleConfig.m_Row.GetActiveOrganisationID());

		CkStringBuilder sbResponseBody;
		bool success = m_Rest.FullRequestNoBodySb("GET", strURL, sbResponseBody);
		if (success != true)
		{
			LogRestError();
			strError = "Pebble API Get Member Fail";
			Diagnostic(strError, TRUE, "Get Members");
			return strError;
		}

		if (m_Rest.get_ResponseStatusCode() >= 400)
		{
			strError.Format("Pebble API Get Member Error %d", m_Rest.get_ResponseStatusCode());
			Diagnostic(strError, TRUE, "Get Members");
			return strError;
		}

		m_bDoneFirstAPICall = TRUE;

		CkJsonObject jsonResponse;
		jsonResponse.LoadSb(sbResponseBody);
		
		CkJsonArray* pJsonMembers = jsonResponse.ArrayOf("members");

		if (pJsonMembers != NULL)
		{
			for (int n = 0; n < pJsonMembers->get_Size(); n++)
			{
				CkJsonObject* pMember = pJsonMembers->ObjectAt(n);

				if (pMember != NULL)
				{
					Diagnostic(pMember, TRUE, "Get Members");

					if (pWorkingDlg != NULL)
					{
						CString str = "";
						str.Format("Receiving Member Details %d", ++nMembers);
						pWorkingDlg->SetCaption2(str);
						Sleep(50);
					}

					WriteBackgroundBusyFile();

					CPebbleMemberBuffer item;
					item.m_strMemberType = pMember->stringOf("memberType");
					item.m_strMemberID = pMember->stringOf("meta.memberId");
					item.m_strMisID = pMember->stringOf("mis.misId");
					item.m_strUPN = pMember->stringOf("mis.upn");
					item.m_strFirstName = pMember->stringOf("mis.firstName");
					item.m_strLastName = pMember->stringOf("mis.lastName");
					item.m_strDOB = pMember->stringOf("dateOfBirth");
					item.m_bIsActive = pMember->BoolOf("isActive");
					delete pMember;

					CString strTest = item.m_strMemberType;
					strTest.MakeLower();

					if ((strTest != "pupil") && (strTest != "staff"))
					{
						continue;
					}

					if (strTest == "pupil")
					{
						if (item.m_strMisID.Left(2) != "1-")
						{
							item.m_strMisID.Insert(0, "1-");
						}
					}
					else
					{
						if (item.m_strMisID.Left(2) != "3-")
						{
							item.m_strMisID.Insert(0, "3-");
						}
					}

					arrayMembers.Add(item);
				}
			}
			delete pJsonMembers;
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

