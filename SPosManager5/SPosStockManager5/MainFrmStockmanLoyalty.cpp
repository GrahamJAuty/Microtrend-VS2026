/**********************************************************************/
#include "..\CommonEcrStock\DataAccessHelpersLoyalty.h"
#include "..\CommonEcrStock\ListDataLoyaltySchemeDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "MainFrmStockman.h"
/**********************************************************************/

void CMainFrameStockman::OnLoyalty() 
{
	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_LOY_V2:
	{
		CDataAccessHelpersLoyalty Helpers;
		Helpers.SetParentWnd(this);
		Helpers.RunLoyaltyV2();
	}
	break;

	case LOYALTY_TYPE_LOY_V3:
	{
		CDataAccessHelpersLoyalty Helpers;
		Helpers.SetParentWnd(this);
		Helpers.RunLoyaltyV3();
	}
	break;

	case LOYALTY_TYPE_LOY_V4:
	default:
	{
		CDataAccessHelpersLoyalty Helpers;
		Helpers.SetParentWnd(this);
		Helpers.RunLoyaltyV4();
	}
	break;
	}
}

/**********************************************************************/

void CMainFrameStockman::OnSmartPay() 
{
	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_SMP_V1:
	{
		CDataAccessHelpersLoyalty Helpers;
		Helpers.SetParentWnd(this);
		Helpers.RunSmartPayV1();
	}
	break;

	case LOYALTY_TYPE_SMP_V2:
	{
		CDataAccessHelpersLoyalty Helpers;
		Helpers.SetParentWnd(this);
		Helpers.RunSmartPayV2();
	}
	break;

	case LOYALTY_TYPE_SMP_V4:
	{
		CDataAccessHelpersLoyalty Helpers;
		Helpers.SetParentWnd(this);
		Helpers.RunSmartPayV4();
	}
	break;
	}
}

/**********************************************************************/

