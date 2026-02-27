//**********************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
//**********************************************************************

CRepmanHandler::CRepmanHandler(const char* szTitle)
{
	m_strTitle = szTitle;
	m_nMode = P_NOWAIT;
	m_nSaveAsType = nSAVEAS_NONE;
	m_strSaveAsFilename = "";
}

//**********************************************************************
