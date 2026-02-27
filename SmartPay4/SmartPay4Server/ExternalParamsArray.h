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
	CString m_strInfo3;
	CString m_strInfo4;
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
	int m_nParamIdxInfo3;
	int m_nParamIdxInfo4;
	int m_nParamIdxGender;
	int m_nParamIdxGroup;
	int m_nParamIdxDOB;
	int m_nParamIdxItems;
};

//**********************************************************************

