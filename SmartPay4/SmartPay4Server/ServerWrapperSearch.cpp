//*******************************************************************
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SQLDb.h"
//*******************************************************************
#include "ServerWrapper.h"
//*******************************************************************

void CServerWrapper::CreateSearchRequestReply(CCSV& CSVFindString)
{
	// save indicator that no records found
	if (SearchRecords(CSVFindString) == 0)
	{
		CString strMsg = "";
		strMsg += StringNoneExists();
		strMsg += ",";
		strMsg += CSVFindString.GetLine();
		m_LoyReqThreadManager.AddResponseLine(strMsg);
	}
}

//*******************************************************************

int CServerWrapper::SearchRecords(CCSV& CSVFindString)
{
	CSQLServerSearchInfo SearchInfo;

	if (GetSearchFields(CSVFindString, SearchInfo) == FALSE)
	{
		return 0;
	}

	if (SearchInfo.m_nMaxToFind < 2)
	{
		SearchInfo.m_nMaxToFind = 2;
	}
	else if (SearchInfo.m_nMaxToFind > 100)
	{
		SearchInfo.m_nMaxToFind = 100;
	}

	CString strWhere = "";
	CSQLBuilderBind BuilderBind{};
	CSQLRepositoryAccount RepoAccount;
	RepoAccount.PrepareServerSearchWhere(SearchInfo, strWhere, BuilderBind);

	CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_ByParams{ &BuilderBind, strWhere });
					
	int nFoundCount = 0;

	CSQLRowAccountFull RowAccount;
	while ((RecordSet.StepSelectAll(RowAccount) == TRUE) && (nFoundCount <= SearchInfo.m_nMaxToFind))
	{
		bool bMatch = TRUE;
		if (SearchInfo.m_nGroupType != -1)
		{
			bMatch = SearchGroupType(RowAccount, SearchInfo.m_nGroupType);
		}

		if (TRUE == bMatch)
		{
			if (++nFoundCount <= SearchInfo.m_nMaxToFind)
			{
				CString strAccountID = RowAccount.GetUserIDString();	// get account user ID

				CCSV csv{};
				CString strBuf = "";

				strBuf.Format("ID=%s", (const char*)strAccountID);
				csv.Add(strBuf);

				SaveRequestDetails(RowAccount, &csv, FALSE);
				m_LoyReqThreadManager.AddResponseLine(csv.GetLine());
			}
		}
	}

	if (nFoundCount > SearchInfo.m_nMaxToFind)
	{
		m_LoyReqThreadManager.AddResponseLine(StringMoreExists());		// save indicator that more records exist
	}

	return nFoundCount;
}

//**********************************************************************

bool CServerWrapper::GetSearchFields(CCSV& CSVFindString, CSQLServerSearchInfo& infoSearch)
{
	for (int i = 0; i < CSVFindString.GetSize(); i++)
	{
		CString strField = CSVFindString.GetString(i);
		strField.MakeUpper();

		if (strField.Left(3) == "N1=")				// look in Surname
		{
			infoSearch.m_strSurname = strField.Mid(3);
		}
		else if (strField.Left(3) == "N2=")				// look in Forename
		{
			infoSearch.m_strForename = strField.Mid(3);
		}
		else if (strField.Left(3) == "I1=")				// look in Info1
		{
			infoSearch.m_strInfo1 = strField.Mid(3);
		}
		else if (strField.Left(3) == "I2=")				// look in Info2
		{
			infoSearch.m_strInfo2 = strField.Mid(3);
		}
		else if (strField.Left(3) == "I3=")				// look in Info3
		{
			infoSearch.m_strInfo3 = strField.Mid(3);
		}
		else if (strField.Left(3) == "I4=")				// look in Info4
		{
			infoSearch.m_strInfo4 = strField.Mid(3);
		}
		else if (strField.Left(3) == "I5=")				// look in Info5
		{
			infoSearch.m_strInfo5 = strField.Mid(3);
		}
		else if (strField.Left(3) == "I6=")				// look in Info6
		{
			infoSearch.m_strInfo6 = strField.Mid(3);
		}
		else if (strField.Left(3) == "I7=")				// look in Info7
		{
			infoSearch.m_strInfo7 = strField.Mid(3);
		}
		else if (strField.Left(3) == "I8=")				// look in Info8
		{
			infoSearch.m_strInfo8 = strField.Mid(3);
		}
		else if (strField.Left(3) == "TY=")				// check group type
		{
			infoSearch.m_nGroupType = atoi(strField.Mid(3));
		}
		else if (strField.Left(5) == "MORE=")					// check for starting point ie where last stopped
		{
			CString strStartAccountID = strField.Mid(5);
			if (strStartAccountID == "")					// no translation so exit
			{
				return FALSE;
			}

			infoSearch.m_nStartID = _atoi64(strStartAccountID);
			break;
		}
	}

	return TRUE;
}

//**********************************************************************

bool CServerWrapper::SearchGroupType(CSQLRowAccountFull& RowAccount, int nGroupType)
{
	bool bMatch = FALSE;

	CSQLRowGroupFull RowGroup;
	RowGroup.SetGroupNo(RowAccount.GetGroupNo());
	CSQLRepositoryGroup RepoGroup;
	if (RepoGroup.SelectRow(RowGroup,NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		bMatch = (nGroupType == RowGroup.GetUserType());
	}
		
	return bMatch;
}

//**********************************************************************
