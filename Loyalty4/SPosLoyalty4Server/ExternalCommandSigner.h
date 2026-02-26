#pragma once
//**********************************************************************

class CExternalCommandSigner
{
public:
	static int CheckSignature(CStringArray& arrayLines);
	static void SignCommand(CStringArray& arrayCommand);

private:
	static int CheckSignatureInternal(CStringArray& arrayLines);

public:
	static CString GetDelimiter() { return "----------"; }

private:
	static CString GetSecret() { return "fjewioj32587239shfeiuhewiNKNIUWHIU"; }
	
};

//**********************************************************************

