//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorStaffCard.h"
//**********************************************************************

void CSQLFieldValidatorStaffCard::SetFolderIndexDbNo(int n, int& nTarget)
{
	SetInt(n, nTarget, StaffCards::FolderIndexDbNo);
}

//**********************************************************************

void CSQLFieldValidatorStaffCard::SetFolderIndexSetNo(int n, int& nTarget)
{
	SetInt(n, nTarget, StaffCards::FolderIndexSetNo);
}

//**********************************************************************

void CSQLFieldValidatorStaffCard::SetServerNo(int n, int& nTarget)
{
	SetInt(n, nTarget, StaffCards::ServerNo);
}

//**********************************************************************

void CSQLFieldValidatorStaffCard::SetUserID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, StaffCards::UserID);
}

//**********************************************************************
