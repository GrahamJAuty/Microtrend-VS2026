#pragma once
//****************************************
#include "resource.h"
//****************************************
#include "..\SmartPay4Shared\SmartPayCommandLineInfo.h"
//****************************************

class CSmartPay4ServerApp : public CWinApp
{
public:
	CSmartPay4ServerApp();

public:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();
	void RunService(CSmartPayCommandLineInfo& cmdInfo);
	void RunDesktop(CSmartPayCommandLineInfo& cmdInfo);
	void RunCommand(CSmartPayCommandLineInfo& cmdInfo);
	DECLARE_MESSAGE_MAP()

private:
	int CheckDbAccess();

private:
	int m_nRetCode;
};

//$$******************************************************************
extern CSmartPay4ServerApp theApp;
//$$******************************************************************
