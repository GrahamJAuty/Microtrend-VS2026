//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorOperatorName.h"
//**********************************************************************

void CSQLFieldValidatorOperatorName::SetOperatorNo(int n, int& nTarget)
{
	SetInt(n, nTarget, OperatorName::OperatorNo);
}

//**********************************************************************

void CSQLFieldValidatorOperatorName::SetOperatorName(CString str, CString& strTarget)
{
	SetString(str, strTarget, OperatorName::OperatorName);
}

//**********************************************************************
