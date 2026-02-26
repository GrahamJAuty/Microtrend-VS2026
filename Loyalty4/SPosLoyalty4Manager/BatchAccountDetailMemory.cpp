//$$******************************************************************
#include "..\SPosLoyalty4Shared\AccountFieldValidator.h"
//$$******************************************************************
#include "BatchAccountDetailMemory.h"
//$$******************************************************************

CBatchAccountDetailMemory::CBatchAccountDetailMemory()
{
	m_bSetName = FALSE;
	m_bSetGroup = FALSE;
	m_bSetScheme = FALSE;
	m_nExpiryAction = BATCH_DATEACTION_NONE;
	m_nDOBAction = BATCH_DATEACTION_NONE;
	m_bSetMaxSpend = FALSE;
	m_bSetOverSpend = FALSE;
	m_bSetAlert = FALSE;
	m_bSetHotlist = FALSE;
	m_bSetInactive = FALSE;
	m_bSetGender = FALSE;

	m_strName = "";
	m_nGroupNo = 1;
	m_nSchemeNo = 0;
	m_dateExpiry = COleDateTime::GetCurrentTime();
	m_dateDOB = COleDateTime::GetCurrentTime();
	m_dMaxSpend = 0.0;
	m_dOverSpend = 0.0;
	m_strAlertCodes = "";
	m_nHotlistIndex = 0;
	m_nInactiveIndex = 0;
	m_nGenderIndex = 0;
}

//$$******************************************************************

void CBatchAccountDetailMemory::TidyOptions()
{
	if (Server.GetAllowOverSpendFlag() == FALSE)
	{
		m_bSetOverSpend = FALSE;
	}
}

//$$******************************************************************

bool CBatchAccountDetailMemory::HaveUpdates()
{
	if (m_bSetName == TRUE)
	{
		return TRUE;
	}

	if (m_bSetGroup == TRUE)
	{
		return TRUE;
	}

	if (m_bSetScheme == TRUE)
	{
		return TRUE;
	}

	if (BATCH_DATEACTION_CLEAR == m_nExpiryAction)
	{
		return TRUE;
	}

	if (BATCH_DATEACTION_CLEAR == m_nDOBAction)
	{
		return TRUE;
	}

	if (TRUE == m_bSetMaxSpend)
	{
		return TRUE;
	}

	if (TRUE == m_bSetOverSpend)
	{
		return TRUE;
	}

	if (TRUE == m_bSetAlert)
	{
		return TRUE;
	}

	if (TRUE == m_bSetHotlist)
	{
		return TRUE;
	}

	if (TRUE == m_bSetInactive)
	{
		return TRUE;
	}

	if (TRUE == m_bSetGender)
	{
		return TRUE;
	}

	if (BATCH_DATEACTION_SET == m_nExpiryAction)
	{
		if (CAccountFieldValidator::ValidateExpiry(m_dateExpiry) == TRUE)
		{
			return TRUE;
		}
	}

	if (BATCH_DATEACTION_SET == m_nDOBAction)
	{
		if (CAccountFieldValidator::ValidateDOB(m_dateDOB) == TRUE)
		{
			return TRUE;
		}
	}

	return FALSE;
}

//$$******************************************************************

bool CBatchAccountDetailMemory::UpdateRecord(CSQLRowAccountFull& RowAccount)
{
	bool bReply = FALSE;

	// cardname

	if (m_bSetName == TRUE)
	{
		if (RowAccount.GetFullname() != m_strName)
		{
			RowAccount.SetFullname(m_strName);
			bReply = TRUE;
		}
	}

	// Group

	if (m_bSetGroup == TRUE)
	{
		if (RowAccount.GetGroupNo() != m_nGroupNo)
		{
			RowAccount.SetGroupNo(m_nGroupNo);
			bReply = TRUE;
		}
	}

	// Scheme

	if (m_bSetScheme == TRUE)
	{
		if (RowAccount.GetSchemeNo() != m_nSchemeNo)
		{
			RowAccount.SetSchemeNo(m_nSchemeNo);
			bReply = TRUE;
		}
	}

	// Expiry

	switch (m_nExpiryAction)
	{
	case BATCH_DATEACTION_SET:
		if (CAccountFieldValidator::ValidateExpiry(m_dateExpiry) == TRUE)
		{
			CString strExpiry = CAccountFieldValidator::OleDateToString(m_dateExpiry);
			RowAccount.SetExpiryDate(strExpiry);
			bReply = TRUE;
		}
		break;

	case BATCH_DATEACTION_CLEAR:
		RowAccount.SetExpiryDate("");
		bReply = TRUE;
		break;
	}

	// dob

	switch (m_nDOBAction)
	{
	case BATCH_DATEACTION_SET:
		if (CAccountFieldValidator::ValidateDOB(m_dateDOB) == TRUE)
		{
			CString strDOB = CAccountFieldValidator::OleDateToString(m_dateDOB);
			RowAccount.SetDOB(strDOB);
			bReply = TRUE;
		}
		break;

	case BATCH_DATEACTION_CLEAR:
		RowAccount.SetDOB("");
		bReply = TRUE;
		break;
	}

	// Max Spend

	if (m_bSetMaxSpend == TRUE)
	{
		if (RowAccount.GetMaxSpend() != m_dMaxSpend)
		{
			RowAccount.SetMaxSpend(m_dMaxSpend);
			bReply = TRUE;
		}
	}

	// Overspend

	if (m_bSetOverSpend == TRUE)
	{
		if (RowAccount.GetMaxOverdraft() != m_dOverSpend)
		{
			RowAccount.SetMaxOverdraft(m_dOverSpend);
			bReply = TRUE;
		}
	}

	// Alert

	if (m_bSetAlert == TRUE)
	{
		CString strCurrentAlerts = RowAccount.GetAlertCodes();

		if (strCurrentAlerts != m_strAlertCodes)
		{
			RowAccount.SetAlertCodes(m_strAlertCodes);
			bReply = TRUE;
		}
	}

	// Hotlist

	if (m_bSetHotlist == TRUE)
	{
		if (m_nHotlistIndex == 1)
		{
			RowAccount.SetHotlistCard(1);
		}
		else
		{
			RowAccount.SetHotlistCard(0);
		}

		bReply = TRUE;
	}

	// Inactive

	if (m_bSetInactive == TRUE)
	{
		RowAccount.SetInactive(1 == m_nInactiveIndex);
		bReply = TRUE;
	}

	//Gender
	if (m_bSetGender == TRUE)
	{
		if (m_nGenderIndex != -1)
		{
			RowAccount.SetGender(m_nGenderIndex);		// 0=unknown,1=male,2-female
			bReply = TRUE;
		}
	}

	return bReply;
}

//$$******************************************************************
