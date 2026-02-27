//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorUserText.h"
//**********************************************************************

void CSQLFieldValidatorUserText::SetFieldLabel(CString str, CString& strTarget)
{
	SetString(str, strTarget, UserText::FieldLabel);
}

//**********************************************************************

void CSQLFieldValidatorUserText::SetUserText(CString str, CString& strTarget)
{
	SetString(str, strTarget, UserText::UserText);
}

//**********************************************************************
