//**********************************************************************
#include "SQLFieldValidatorPurchaseControlRule.h"
//**********************************************************************
#include "SQLRowPurchaseControlRule.h"
//**********************************************************************

CSQLRowPurchaseControlRule::CSQLRowPurchaseControlRule()
{
	Reset();
}

//**********************************************************************

void CSQLRowPurchaseControlRule::Reset()
{
	m_nRuleID = 1;
	m_bEnabled = FALSE;
	m_nThreshold = 1;
	m_nExpiryDays = 0;
	m_strDescription = "";
	m_nPluCount = 0;
}

//**********************************************************************

CString CSQLRowPurchaseControlRule::GetDisplayDescription()
{
	CString strResult = m_strDescription;
	if (strResult == "")
	{
		if (TRUE == m_bEnabled)
		{
			strResult.Format("Purchase Control %d", m_nRuleID);
		}
		else
		{
			strResult = "----";
		}
	}

	return strResult;
}

//**********************************************************************
