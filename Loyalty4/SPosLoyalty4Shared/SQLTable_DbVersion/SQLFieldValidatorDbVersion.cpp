//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorDbVersion.h"
//**********************************************************************

void CSQLFieldValidatorDbVersion::SetLineID(int n, int& nTarget)
{
	SetInt(n, nTarget, DbVersion::LineID);
}

//**********************************************************************

void CSQLFieldValidatorDbVersion::SetVersion(int n, int& nTarget)
{
	SetInt(n, nTarget, DbVersion::Version);
}

//**********************************************************************