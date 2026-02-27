//$$******************************************************************
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//$$******************************************************************
#include "BatchAccountDetailMemory.h"
//$$******************************************************************

CBatchAccountDetailMemory::CBatchAccountDetailMemory()
{
	m_bSetGroup = FALSE;
	m_bSetExpiry = FALSE;
	m_bSetDob = FALSE;
	m_bSetMaxSpend = FALSE;
	m_bSetOverSpend = FALSE;
	m_bSetAlert = FALSE;
	m_bSetName = FALSE;

	m_bClearGroup = FALSE;
	m_bClearExpiry = FALSE;
	m_bClearDob = FALSE;
	m_bClearMaxSpend = FALSE;
	m_bClearOverSpend = FALSE;

	m_nGroupNo = 1;
	m_dateExpiry = COleDateTime::GetCurrentTime();
	m_dateDob = COleDateTime::GetCurrentTime();
	m_dMaxSpend = 0.0;
	m_dOverSpend = 0.0;
	m_nAlertCode = -1;
}

//$$******************************************************************

void CBatchAccountDetailMemory::TidyOptions()
{
	if (FALSE == Server.GetAllowOverdraftFlag())
	{
		m_bSetOverSpend = FALSE;
		m_bClearOverSpend = FALSE;
	}

	m_bClearGroup &= (!m_bSetGroup);
	m_bClearExpiry &= (!m_bSetExpiry);
	m_bClearDob &= (!m_bSetDob);
	m_bClearMaxSpend &= (!m_bSetMaxSpend);
	m_bClearOverSpend &= (!m_bSetOverSpend);

	if (TRUE == m_bClearMaxSpend)
	{
		m_dMaxSpend = 0.0;
	}

	if (TRUE == m_bClearOverSpend)
	{
		m_dOverSpend = 0.0;
	}
}

//$$******************************************************************

bool CBatchAccountDetailMemory::HaveUpdates()
{
	bool bResult = FALSE;
	bResult |= m_bSetGroup;
	bResult |= m_bClearGroup;
	bResult |= m_bSetExpiry;
	bResult |= m_bClearExpiry;
	bResult |= m_bSetDob;
	bResult |= m_bClearDob;
	bResult |= m_bSetMaxSpend;
	bResult |= m_bClearMaxSpend;
	bResult |= m_bSetOverSpend;
	bResult |= m_bClearOverSpend;
	bResult |= m_bSetAlert;
	bResult |= m_bSetName;
	return bResult;
}

//$$******************************************************************

bool CBatchAccountDetailMemory::UpdateRecord(CSQLRowAccountFull& RowAccount)
{
	bool bReply = FALSE;

	// Group

	if (m_bSetGroup || m_bClearGroup)
	{
		if (RowAccount.GetGroupNo() != m_nGroupNo)
		{
			RowAccount.SetGroupNo(m_nGroupNo);
			bReply = TRUE;
		}
	}

	// Expiry

	if (m_bSetExpiry)
	{
		if (CSQLFieldValidatorAccount::ValidateExpiry(m_dateExpiry))
		{
			RowAccount.SetExpiryDate(CSQLFieldValidatorAccount::OleDateToString(m_dateExpiry));
			bReply = TRUE;
		}
	}
	else if (m_bClearExpiry)
	{
		CString strDate = "";
		strDate = RowAccount.GetExpiryDate();
		if (strDate != "")
		{
			RowAccount.SetExpiryDate("");
			bReply = TRUE;
		}
	}

	// dob

	if (m_bSetDob)
	{
		if (CSQLFieldValidatorAccount::ValidateDOB(m_dateDob))
		{
			RowAccount.SetDOB(CSQLFieldValidatorAccount::OleDateToString(m_dateDob));
			bReply = TRUE;
		}
	}
	else if (m_bClearDob)
	{
		CString strDate = "";
		strDate = RowAccount.GetDOB();
		if (strDate != "")
		{
			RowAccount.SetDOB("");
			bReply = TRUE;
		}
	}

	// Max Spend

	if (m_bSetMaxSpend == TRUE)
	{
		if (RowAccount.GetMaxSpendPurse1Period1() != m_dMaxSpend)
		{
			RowAccount.SetMaxSpendPurse1Period1(m_dMaxSpend);
			bReply = TRUE;
		}
	}
	else if (m_bClearMaxSpend == TRUE)
	{
		if (RowAccount.GetMaxSpendPurse1Period1() != 0.0)
		{
			RowAccount.SetMaxSpendPurse1Period1(0);
			bReply = TRUE;
		}
	}

	// Overspend

	if (m_bSetOverSpend == TRUE)
	{
		if (RowAccount.GetMaxOverdraftPurse1Period1() != m_dOverSpend)
		{
			RowAccount.SetMaxOverdraftPurse1Period1(m_dOverSpend);
			bReply = TRUE;
		}
	}
	else if (m_bClearOverSpend == TRUE)
	{
		if (RowAccount.GetMaxOverdraftPurse1Period1() != 0.0)
		{
			RowAccount.SetMaxOverdraftPurse1Period1(0);
			bReply = TRUE;
		}
	}

	// Alert

	if (m_bSetAlert == TRUE)
	{
		if (RowAccount.GetAlertCode() != m_nAlertCode)
		{
			RowAccount.SetAlertCode(m_nAlertCode);
			bReply = TRUE;
		}
	}

	if (m_bSetName == TRUE)
	{
		CString strFullName = RowAccount.GetUsername();
		CString strForename = RowAccount.GetForename();
		CString strSurname = RowAccount.GetSurname();

		if ((strForename == "") || (strSurname == ""))
		{
			CStringArray arrayTokens;

			CString strToken = "";
			for (int n = 0; n < strFullName.GetLength(); n++)
			{
				char c = strFullName.GetAt(n);
				if (c == ' ')
				{
					if (strToken != "")
					{
						arrayTokens.Add(strToken);
						strToken = "";
					}
				}
				else
				{
					strToken += c;
				}
			}

			if (strToken != "")
			{
				arrayTokens.Add(strToken);
			}

			if (arrayTokens.GetSize() > 0)
			{
				RowAccount.SetForename(arrayTokens.GetAt(0));
			}

			if (arrayTokens.GetSize() > 1)
			{
				RowAccount.SetSurname(arrayTokens.GetAt(arrayTokens.GetSize() - 1));
			}

		}
	}

	return bReply;
}

//$$******************************************************************

void CBatchAccountDetailMemory::AddWarnings(CStringArray* pArray, CAlertText& AlertText)
{
	// set Max Spend & Over spend

	if ((m_bSetMaxSpend) && (m_dMaxSpend != 0.0))
	{
		CString strMsg = "";
		strMsg.Format("The Max Spend limit will be set to %.2f.", m_dMaxSpend);
		pArray->Add(strMsg);
	}
	else if (m_bClearMaxSpend)
	{
		pArray->Add("The Max Spend limit will be CLEARED.");
	}

	if ((m_bSetOverSpend) && (m_dOverSpend != 0.0))
	{
		CString strMsg = "";
		strMsg.Format("The Overdraft limit will be set to %.2f.", m_dOverSpend);
		pArray->Add(strMsg);
	}
	else if (m_bClearOverSpend)
	{
		pArray->Add("The Overdraft limit will be CLEARED.");
	}

	// group

	if (m_bSetGroup)
	{
		CString strMsg = "";
		strMsg.Format("The user Group will be set to %d.", m_nGroupNo);
		pArray->Add(strMsg);
	}
	else if (m_bClearGroup)
	{
		pArray->Add("The user Group will be set to 1.");
	}

	// 	expiry

	if (m_bSetExpiry)
	{
		if (CSQLFieldValidatorAccount::ValidateExpiry(m_dateExpiry) == TRUE)
		{
			CString strExpiryDate = CSQLFieldValidatorAccount::OleDateToString(m_dateExpiry);

			CString strMsg = "";
			strMsg.Format("The Expiry date will be set to %s.",
				(const char*)strExpiryDate);

			pArray->Add(strMsg);
		}
	}
	else if (m_bClearExpiry)
	{
		pArray->Add("The Expiry date will be CLEARED.");
	}

	// anniversary

	if (m_bSetDob)
	{
		if (CSQLFieldValidatorAccount::ValidateDOB(m_dateDob) == TRUE)
		{
			CString strDobDate = CSQLFieldValidatorAccount::OleDateToString(m_dateDob);

			CSQLRepositoryUserText RepoText;

			CString strMsg = "";
			strMsg.Format("The %s date will be set to %s.",
				(const char*) RepoText.GetDOBText(Account::DOB.Label), 
				(const char*) strDobDate);

			pArray->Add(strMsg);
		}
	}
	else if (m_bClearDob)
	{
		CSQLRepositoryUserText RepoText;

		CString strMsg = "";
		strMsg.Format("The %s date will be CLEARED.", 
			(const char*) RepoText.GetDOBText(Account::DOB.Label));
		pArray->Add(strMsg);
	}

	// Alert

	if ((m_bSetAlert) && (m_nAlertCode != -1))
	{
		CString strMsg = "The user alert will be set to ";

		if (0 == m_nAlertCode)
		{
			strMsg += "Clear";
		}
		else
		{
			strMsg += AlertText.GetText(m_nAlertCode);
		}

		pArray->Add(strMsg);
	}

	if (TRUE == m_bSetName)
	{
		pArray->Add("Usernames will be copied to forename and surname.");
	}
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************
