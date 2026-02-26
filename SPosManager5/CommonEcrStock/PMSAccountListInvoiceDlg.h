#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PMSAccountListDlg.h"
/**********************************************************************/

class CPMSAccountListInvoiceDlg : public CPMSAccountListDlg
{
public:
	CPMSAccountListInvoiceDlg( int nRoomIdx, CWnd* pParent = NULL);   

public:
	void DoubleClickList(){ HandleButton1(); }
	void HandleButton1();
	void SetButtonControls();
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
