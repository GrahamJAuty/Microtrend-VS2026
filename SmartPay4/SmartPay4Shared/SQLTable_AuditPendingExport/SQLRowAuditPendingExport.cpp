//**********************************************************************
#include "SQLRowAuditPendingExport.h"
//**********************************************************************

CSQLRowAuditPendingExport::CSQLRowAuditPendingExport()
{
	Reset();
}

//**********************************************************************

void CSQLRowAuditPendingExport::Reset()
{
	m_nLineID = 0;
	m_strUserID = "";
	m_strXFerUserID = "";
	m_strUserName = "";
	m_nGroupNo = 0;
	m_strDate = "";
	m_strTime = "";
	m_nActionType = 0;
	m_nSourceType = 0;
	m_nAppNo = 0;
	m_nTerminalNo = 0;
	m_nCCNo = 0;
	m_strText = "";
	m_nPointsAdded = 0;
	m_nPointsBalance = 0;
	m_dPurse1Value = 0.0;
	m_dPurse2Value = 0.0;
	m_dPurse3Value = 0.0;
	m_dCashValue = 0.0;
	m_dPurse1Balance = 0.0;
	m_dPurse2Balance = 0.0;
	m_dPurse3Balance = 0.0;
	m_strNotes = "";
}

//**********************************************************************

CString CSQLRowAuditPendingExport::GetXmlDate(int nWebPaymentType)
{
	CSSDate date(GetDate());
	CSSTime time(GetTime());

	CString strXmlDate = "";

	switch (nWebPaymentType)
	{
	case nWEBPAYMENT_TUCASI:
	case nWEBPAYMENT_TUCASIv2:
		strXmlDate.Format("%4.4d-%2.2d-%2.2d %2.2d:%2.2d:00:000", date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute());
		break;

	default:
		strXmlDate = ::FormatXmlDate(&date, &time);
		break;
	}

	return strXmlDate;
}

//******************************************************************************
