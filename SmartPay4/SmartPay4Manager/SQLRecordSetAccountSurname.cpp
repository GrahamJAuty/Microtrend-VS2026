//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetAccountSurname.h"
//**********************************************************************

CSQLRecordSetMetadataAccountSurname::CSQLRecordSetMetadataAccountSurname() : CSQLRecordSetMetadata()
{
	AddInt64Field(&m_nUserID, Account::UserID.Label, 1);
	AddTextField(&m_strUsername, Account::Username.Label);
	AddTextField(&m_strForename, Account::Forename.Label);
	AddTextField(&m_strSurname, Account::Surname.Label);
	AddIntField(&m_nGroupNo, Account::GroupNo.Label, 1);
	AddIntField(&m_nGroupShiftStatus, Account::GroupShiftStatus.Label);
	AddTextField(&m_strGroupShiftStartDate, Account::GroupShiftStartDate.Label);
	AddTextField(&m_strGroupShiftEndDate, Account::GroupShiftEndDate.Label);
	AddIntField(&m_nGroupShiftFutureGroup, Account::GroupShiftFutureGroup.Label);
}

//**********************************************************************

CSQLRecordSetAccountSurname::CSQLRecordSetAccountSurname(CDatabase* pDatabase) 
	: CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetAccountSurname::CommonInit()
{
	m_nFields = m_MD.GetFieldCount();
	m_nParams = m_MD.GetParamCount();
	SetMetadata(&m_MD);
	SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetAccountSurname::GetDefaultSQL()
{
    return SQLTableNames::Accounts; 
}

//**********************************************************************

