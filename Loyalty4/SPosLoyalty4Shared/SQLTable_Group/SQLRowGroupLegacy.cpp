//**********************************************************************
#include "..\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
//**********************************************************************
#include "..\SQLDefines.h"
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorGroup.h"
//**********************************************************************
#include "SQLRowGroupLegacy.h"
//**********************************************************************

CSQLRowGroupLegacy::CSQLRowGroupLegacy()
{
	Reset();
}

//**********************************************************************

void CSQLRowGroupLegacy::Reset()
{
	m_nGroupNo = 1;
	
	/*****/

	for (int n = 1; n < LEGACY_REVALUE_PLUNO_COUNT; n++)
	{
		m_bRevalueFlag[n] = FALSE;
		m_bRevalueDisable[n] = FALSE;
		m_nRevalueType[n] = Group::LegacyRevalueType1.Min;
		m_dRevalueTrip[n] = 0.0;
		m_nRevaluePoints[n] = 0;
		m_dRevalueBonus[n] = 0.0;
		m_nRevaluePercent[n] = 0;
	}
}

//**********************************************************************
