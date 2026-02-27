//*******************************************************************
#include "..\SmartPay4ManagerBgnd\DatabaseImport.h"
//*******************************************************************
#include "..\SmartPay4Bgnd\MessageLogger.h"
//*******************************************************************

void CImportDatabase::LogMessage ( CString strMsg )
{
	MessageLogger.LogImportDatabaseMessage( strMsg );
}

//**********************************************************************

