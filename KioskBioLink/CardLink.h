//*******************************************************************
#pragma once
//*******************************************************************
#include "Resource.h"
//*******************************************************************
#include "Defines.h"
#include "UserIDRecord.h"
//*******************************************************************

class CCardLinkApp : public CWinApp
{
public:
	CCardLinkApp();

public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()

private:
	void MainProgram(CSSFullScreenWnd* pScreen, bool bIDReaderOffline);
	bool EnrolCard(CSSFullScreenWnd* pScreen, const char* szMiFareNo, CUserIDRecord* pUserRecord, CString& strError);
};

//****************************************************************************
extern CCardLinkApp theApp;
//****************************************************************************
