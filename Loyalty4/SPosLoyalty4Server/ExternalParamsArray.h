#pragma once
//**********************************************************************
#define EXTCOMMAND_AUTHENTICATE 1
#define EXTCOMMAND_USER 2
#define EXTCOMMAND_BALANCE 3
#define EXTCOMMAND_PAYMENT 4
#define EXTCOMMAND_RESETPASSWORD 5
#define EXTCOMMAND_CREATEUSER 6
#define EXTCOMMAND_USEREXISTS 7
//**********************************************************************
#include "ExternalReplyArray.h"
//**********************************************************************

struct CExternalParamInfo
{
public:
	CExternalParamInfo();

public:
	CString m_strParam;
	bool m_bFound;
};

//**********************************************************************

struct CExternalCommandInfo
{
public:
	CExternalCommandInfo();

public:
	CString m_strCommand;
	int m_nCommandType;
};

//**********************************************************************

class CExternalParamsArray
{
public:
	CExternalParamsArray();

public:
	int Init(CString strCommand);
	void GetParams(CStringArray& arrayLines, CExternalReplyArray& arrayError);

private:
	int AddParam(CString strParam);
	void AddCommand(CString strCommand, int nCommandType);
	void ResetParams();
	
public:
	CString m_strUserName;
	CString m_strPassword;
	CString m_strUserID;
	int m_nLoyaltyAmount;
	int m_nOtherAmount;
	CString m_strForename;
	CString m_strSurname;
	CString m_strAddress1;
	CString m_strAddress2;
	CString m_strAddress3;
	CString m_strAddress4;
	CString m_strAddress5;
	CString m_strPhone1;
	CString m_strPhone2;
	CString m_strGender;
	int m_nGroup;
	CString m_strDOB;
	CString m_strItems;

private:
	CArray<CExternalCommandInfo, CExternalCommandInfo> m_arrayCommands;
	CArray<CExternalParamInfo, CExternalParamInfo> m_arrayParams;
	CArray<int, int> m_arrayParamsWanted;

private:
	int m_nParamIdxUserName;
	int m_nParamIdxPassword;
	int m_nParamIdxPasswordPT;
	int m_nParamIdxUserID;
	int m_nParamIdxLoyaltyAmount;
	int m_nParamIdxOtherAmount;
	int m_nParamIdxForename;
	int m_nParamIdxSurname;
	int m_nParamIdxAddress1;
	int m_nParamIdxAddress2;
	int m_nParamIdxAddress3;
	int m_nParamIdxAddress4;
	int m_nParamIdxAddress5;
	int m_nParamIdxPhone1;
	int m_nParamIdxPhone2;
	int m_nParamIdxGender;
	int m_nParamIdxGroup;
	int m_nParamIdxDOB;
	int m_nParamIdxItems;
};

//**********************************************************************

