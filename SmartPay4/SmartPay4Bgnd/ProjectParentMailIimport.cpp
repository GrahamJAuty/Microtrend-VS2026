//*******************************************************************
#include "..\SmartPay4Bgnd\MessageLogger.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\ParentMailImport.h"
//*******************************************************************

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//*******************************************************************

void CParentMailImport::LogMessage( CString strMessage )
{
	MessageLogger.LogParentMailMessage( strMessage );
}

//*******************************************************************
