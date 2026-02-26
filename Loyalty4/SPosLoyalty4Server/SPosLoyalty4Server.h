#pragma once
//****************************************
#include "resource.h"
//****************************************
#include "..\SPosLoyalty4Shared\LoyaltyCommandLineInfo.h"
//****************************************

class CLoyalty4ServerApp : public CWinApp
{
public:
	CLoyalty4ServerApp();

public:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();
	void RunService(CLoyaltyCommandLineInfo& cmdInfo);
	void RunDesktop(CLoyaltyCommandLineInfo& cmdInfo);
	void RunCommand(CLoyaltyCommandLineInfo& cmdInfo);
	DECLARE_MESSAGE_MAP()

private:
	int CheckDbAccess();

private:
	int m_nRetCode;
};

//$$******************************************************************
extern CLoyalty4ServerApp theApp;
//$$******************************************************************
