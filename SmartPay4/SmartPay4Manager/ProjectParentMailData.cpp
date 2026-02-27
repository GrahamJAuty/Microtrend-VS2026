//*******************************************************************
#include "..\SmartPay4ManagerBgnd\ParentMailData.h"
//*******************************************************************

void CParentMailData::DisplayStaffListError ( CWnd* pParent )
{
	CString strMsg = "The none pupil list contains invalid characters!\n\nUse only digits 0 through 9, '-' or ','.\n\nFor example:  1,20,25-30";
	Prompter.FatalError( strMsg, "Staff Group List Error" );
}

//*********************************************************************

