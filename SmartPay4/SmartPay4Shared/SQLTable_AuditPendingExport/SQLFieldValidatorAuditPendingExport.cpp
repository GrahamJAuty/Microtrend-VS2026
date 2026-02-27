//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorAuditPendingExport.h"
//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetLineID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, AuditPendingExport::LineID);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetUserID(CString str, CString& strTarget)
{
	SetString(str, strTarget, AuditPendingExport::UserID);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetXFerUserID(CString str, CString& strTarget)
{
	SetString(str, strTarget, AuditPendingExport::XFerUserID);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetUserName(CString str, CString& strTarget)
{
	SetString(str, strTarget, AuditPendingExport::UserName);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetGroupNo(int n, int& nTarget)
{
	SetInt(n, nTarget, AuditPendingExport::GroupNo);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, AuditPendingExport::Date);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, AuditPendingExport::Time);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetActionType(int n, int& nTarget)
{
	SetInt(n, nTarget, AuditPendingExport::ActionType);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetSourceType(int n, int& nTarget)
{
	SetInt(n, nTarget, AuditPendingExport::SourceType);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetAppNo(int n, int& nTarget)
{
	SetInt(n, nTarget, AuditPendingExport::AppNo);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetTerminalNo(int n, int& nTarget)
{
	SetInt(n, nTarget, AuditPendingExport::TerminalNo);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetCCNo(int n, int& nTarget)
{
	SetInt(n, nTarget, AuditPendingExport::CCNo);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetText(CString str, CString& strTarget)
{
	SetString(str, strTarget, AuditPendingExport::Text);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetPointsAdded(int n, int& nTarget)
{
	SetInt(n, nTarget, AuditPendingExport::PointsAdded);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetPointsBalance(int n, int& nTarget)
{
	SetInt(n, nTarget, AuditPendingExport::PointsBalance);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetPurse1Value(double d, double& dTarget)
{
	SetDouble(d, dTarget, AuditPendingExport::Purse1Value);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetPurse2Value(double d, double& dTarget)
{
	SetDouble(d, dTarget, AuditPendingExport::Purse2Value);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetPurse3Value(double d, double& dTarget)
{
	SetDouble(d, dTarget, AuditPendingExport::Purse3Value);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetCashValue(double d, double& dTarget)
{
	SetDouble(d, dTarget, AuditPendingExport::CashValue);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetPurse1Balance(double d, double& dTarget)
{
	SetDouble(d, dTarget, AuditPendingExport::Purse1Balance);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetPurse2Balance(double d, double& dTarget)
{
	SetDouble(d, dTarget, AuditPendingExport::Purse2Balance);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetPurse3Balance(double d, double& dTarget)
{
	SetDouble(d, dTarget, AuditPendingExport::Purse3Balance);
}

//**********************************************************************

void CSQLFieldValidatorAuditPendingExport::SetNotes(CString str, CString& strTarget)
{
	SetString(str, strTarget, AuditPendingExport::Notes);
}

//**********************************************************************


