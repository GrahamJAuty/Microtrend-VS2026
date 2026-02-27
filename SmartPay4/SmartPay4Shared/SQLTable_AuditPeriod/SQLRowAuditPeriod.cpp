//**********************************************************************
#include "SQLRowAuditPeriod.h"
//**********************************************************************

CSQLRowAuditPeriod::CSQLRowAuditPeriod()
{
	Reset();
}

//**********************************************************************

void CSQLRowAuditPeriod::Reset()
{
	m_nPeriodID = 0;
	m_strPeriodName = "";
	m_nStartTime = 0;
}

//**********************************************************************

CString CSQLRowAuditPeriod::FormatStartTime()
{
	CString strResult = "";
	strResult.Format("%2.2d:%2.2d", m_nStartTime / 60, m_nStartTime % 60);
	return strResult;
}

//**********************************************************************
