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

class CPMSAccountListCheckoutDlg : public CPMSAccountListDlg
{
public:
	CPMSAccountListCheckoutDlg( int nRoomIdx, CWnd* pParent = NULL);   

public:
	void DoubleClickList(){ HandleButton1(); }
	void HandleButton1();
	void HandleButton2();
	void SetButtonControls();
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
