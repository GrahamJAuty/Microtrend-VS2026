//**********************************************************************
#include "SQLFieldValidatorPurchaseControlStamp.h"
//**********************************************************************
#include "SQLRowPurchaseControlStamp.h"
//**********************************************************************

CSQLRowPurchaseControlStamp::CSQLRowPurchaseControlStamp()
{
	Reset();
}

//**********************************************************************

void CSQLRowPurchaseControlStamp::Reset()
{
	m_nUserID = 0;
	m_nRuleID = 0;
	m_strAwardDate = "";
	m_strExpireDate = "";
	m_nStampCount = 0;
}

//**********************************************************************

bool CSQLRowPurchaseControlStamp::Compare(CSQLRowPurchaseControlStamp& source)
{
	bool bResult = TRUE;
	bResult &= (m_nUserID == source.m_nUserID);
	bResult &= (m_nRuleID == source.m_nRuleID);
	bResult &= (m_strAwardDate == source.m_strAwardDate);
	bResult &= (m_strExpireDate == source.m_strExpireDate);
	return bResult;		
}

//**********************************************************************