#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ProcessedOrderListDlg.h"
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

class CProcessedOrderListOpenDlg : public CProcessedOrderListDlg
{
public:
	CProcessedOrderListOpenDlg( bool bDbOrders, CArray<int,int>& arrayEntityIdx, CWnd* pParent = NULL);
	
private:
	void ConfigureDialog();
	void HandleButton1();
	void HandleButton2();
	void HandleButton3();
	void HandleDoubleClick();

private:
	void Button1Internal();
	void Button2Internal();
	void Button3Internal();
};

/**********************************************************************/
#endif
/**********************************************************************/
