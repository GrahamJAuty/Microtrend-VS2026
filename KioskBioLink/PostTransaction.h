#pragma once
//****************************************************************************
#include "Defines.h"
//****************************************************************************

class CPostTransaction
{
public:
	CPostTransaction(void);
	bool HaveFile();
	bool AddToOffline();

public:
	static CString Create$TranLine(CSSFile* fp);

private:
	CString m_strPayLinkTTLFilename = "";
	CString m_strOfflineFilename = "";
};

//****************************************************************************
