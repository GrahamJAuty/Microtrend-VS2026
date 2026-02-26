//**********************************************************************
#include "ExternalReplyArray.h"
//**********************************************************************

void CExternalReplyArray::AddOKReply(CString strField, CString strValue)
{
	if (GetSize() == 0)
	{
		Add("OK");
	}

	Add(strField);
	Add(strValue);
}

//**********************************************************************

void CExternalReplyArray::CreateLoginErrorReply( int nCode )
{
	CString strCode;
	strCode.Format("%d", nCode);
	CreateErrorReply(strCode, "Login Failure" );
}

//**********************************************************************

void CExternalReplyArray::CreateErrorReply(CString strErrorCode, CString strErrorMessage, CString strErrorDetail)
{
	if (strErrorDetail != "")
	{
		strErrorMessage += " (";
		strErrorMessage += strErrorDetail;
		strErrorMessage += ")";
	}

	RemoveAll();
	Add("ERROR");
	Add("error_message");
	Add(strErrorMessage);
	Add("error_code");
	Add(strErrorCode);
}

//**********************************************************************
