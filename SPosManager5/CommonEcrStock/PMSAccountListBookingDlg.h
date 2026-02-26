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

class CPMSAccountListBookingDlg : public CPMSAccountListDlg
{
public:
	CPMSAccountListBookingDlg( int nRoomIdx, CWnd* pParent = NULL);

public:
	void DoubleClickList(){ HandleButton2(); }
	void HandleButton1();
	void HandleButton2();
	void SetButtonControls();
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
