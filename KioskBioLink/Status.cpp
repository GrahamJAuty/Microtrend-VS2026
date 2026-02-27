//**********************************************************************
#include "GlobalFunctions.h"
#include "GTData.h"
#include "SystemData.h"
//**********************************************************************
#include "Status.h"
//**********************************************************************

//**********************************************************************
//	status		0 = Set kiosk terminated
//				1 = Set kiosk started
//				2 = kiosk is running \ check server working
//				3 = Set kiosk in service
//				4 = Set kiosk out of service
//				5 = Cash emptied
//**********************************************************************

CStatus::CStatus(int nTerminalNo, const char* szImportFile)
{
	m_nTerminalNo = nTerminalNo;
	m_strImportFile = szImportFile;
}

//*******************************************************************

