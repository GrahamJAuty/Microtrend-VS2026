#pragma once
//**********************************************************************

class CExternalReplyArray : public CStringArray
{
public:
	void AddOKReply(CString strField, CString strValue);
	void CreateLoginErrorReply(int nCode);
	void CreateErrorReply(CString strErrorCode, CString strErrorMessage, CString strErrorDetail = "" );
};

//**********************************************************************

