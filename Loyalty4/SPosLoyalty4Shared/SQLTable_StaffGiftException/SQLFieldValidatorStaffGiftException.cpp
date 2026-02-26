//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorStaffGiftException.h"
//**********************************************************************

void CSQLFieldValidatorStaffGiftException::SetLineID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, StaffGiftExceptions::LineID);
}

//**********************************************************************

void CSQLFieldValidatorStaffGiftException::SetCustomerID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, StaffGiftExceptions::CustomerID);
}

//**********************************************************************

void CSQLFieldValidatorStaffGiftException::SetFolderIndexDbNo(int n, int& nTarget)
{
	SetInt(n, nTarget, StaffGiftExceptions::FolderIndexDbNo);
}

//**********************************************************************

void CSQLFieldValidatorStaffGiftException::SetFolderIndexSetNo(int n, int& nTarget)
{
	SetInt(n, nTarget, StaffGiftExceptions::FolderIndexSetNo);
}

//**********************************************************************

void CSQLFieldValidatorStaffGiftException::SetServerNo(int n, int& nTarget)
{
	SetInt(n, nTarget, StaffGiftExceptions::ServerNo);
}

//**********************************************************************

void CSQLFieldValidatorStaffGiftException::SetServerID(__int64 n, __int64& nTarget)
{
	SetInt64(n, nTarget, StaffGiftExceptions::ServerID);
}

//**********************************************************************

void CSQLFieldValidatorStaffGiftException::SetDate(CString str, CString& strTarget)
{
	SetString(str, strTarget, StaffGiftExceptions::Date);
}

//**********************************************************************

void CSQLFieldValidatorStaffGiftException::SetTime(CString str, CString& strTarget)
{
	SetString(str, strTarget, StaffGiftExceptions::Time);
}

//**********************************************************************

void CSQLFieldValidatorStaffGiftException::SetValue(double d, double& dTarget)
{
	SetDouble(d, dTarget, StaffGiftExceptions::Value);
}

//**********************************************************************

void CSQLFieldValidatorStaffGiftException::SetErrorCode(int n, int& nTarget)
{
	SetInt(n, nTarget, StaffGiftExceptions::ErrorCode);
}

//**********************************************************************
