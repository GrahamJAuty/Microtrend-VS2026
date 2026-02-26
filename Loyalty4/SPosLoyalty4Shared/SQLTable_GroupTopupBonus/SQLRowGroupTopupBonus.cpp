//**********************************************************************
#include "..\SQLDefines.h"
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorGroupTopupBonus.h"
//**********************************************************************
#include "SQLRowGroupTopupBonus.h"
//**********************************************************************

CSQLRowGroupTopupBonus::CSQLRowGroupTopupBonus()
{
	Reset();
}

//**********************************************************************

void CSQLRowGroupTopupBonus::Reset()
{
	m_nGroupNo = 1;
	m_nPurseNo = 1;
	m_nTopupNo = 1;
	/*****/
	/*****/
	m_bBonusFlag = FALSE;
	m_bBonusDisable = FALSE;
	m_nBonusType = GroupTopupBonus::BonusType.Min;
	m_nBonusTrip = 0.0;
	m_nBonusPoints = 0;
	m_nBonusValue = 0.0;
	m_nBonusPercent = 0;
}

//**********************************************************************

void CSQLRowGroupTopupBonus::Add(CSQLRowGroupTopupBonus& source)
{
}

/**********************************************************************/

int CSQLRowGroupTopupBonus::Compare(CSQLRowGroupTopupBonus& source, int nHint)
{
	if (m_nGroupNo != source.m_nGroupNo)
		return (m_nGroupNo > source.m_nGroupNo ? 1 : -1);

	if (m_nPurseNo != source.m_nPurseNo)
		return (m_nPurseNo > source.m_nPurseNo ? 1 : -1);

	if (m_nTopupNo != source.m_nTopupNo)
		return (m_nTopupNo > source.m_nTopupNo ? 1 : -1);
	else
		return 0;
}

/**********************************************************************/
