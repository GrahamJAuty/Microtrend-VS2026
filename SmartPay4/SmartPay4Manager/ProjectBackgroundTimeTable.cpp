//*****************************************************************
#include "BackgroundTimetableDlg.h"
//*****************************************************************
#include "..\SmartPay4ManagerBgnd\BackgroundTimeTable.h"
//*****************************************************************

void CBackgroundTimeTable::Show ( CWnd* pParent )
{
	CBackgroundTimetableDlg dlg ( pParent );
	dlg.DoModal();
}

//***********************************************************
