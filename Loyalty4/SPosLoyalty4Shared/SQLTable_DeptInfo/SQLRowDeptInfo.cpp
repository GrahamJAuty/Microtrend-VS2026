//**********************************************************************
#include "SQLFieldValidatorDeptInfo.h"
//**********************************************************************
#include "SQLRowDeptInfo.h"
//**********************************************************************

CSQLRowDeptInfo::CSQLRowDeptInfo()
{
	Reset();
}

//**********************************************************************

void CSQLRowDeptInfo::Reset()
{
	m_nDbNo = 0;
	m_nDeptNo = 0;
	m_strName = "";
	m_dLoyaltyBonusTrip = 0.0;
	m_nLoyaltyBonusType = 0;
	m_nLoyaltyBonusPoints = 0;
	m_dLoyaltyBonusValue = 0.0;
	m_nLoyaltyBonusPercent = 0;
	m_bGeneralSpendBonusFlag = TRUE;
}

//**********************************************************************

