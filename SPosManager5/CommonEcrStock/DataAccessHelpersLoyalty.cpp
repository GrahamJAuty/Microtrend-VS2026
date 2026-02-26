/**********************************************************************/
 
/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "DataAccessHelpersLoyalty.h"
/**********************************************************************/

CDataAccessHelpersLoyalty::CDataAccessHelpersLoyalty()
{
}

/**********************************************************************/

void CDataAccessHelpersLoyalty::RunLoyaltyV2()
{
	CString strDataPath;
	strDataPath.Format( "%d", SysInfo.GetDataPathNo() );
	SSspawnl ( m_pParentWnd -> GetSafeOwner( m_pParentWnd ), P_WAIT, "SPOSLOYALTYMANAGER.EXE", "SPOSLOYALTYMANAGER.EXE", strDataPath, NULL );
}

/**********************************************************************/

void CDataAccessHelpersLoyalty::RunLoyaltyV3()
{
	CString strDataPath;
	strDataPath.Format( "%d", SysInfo.GetDataPathNo() );
	SSspawnl ( m_pParentWnd -> GetSafeOwner( m_pParentWnd ), P_WAIT, "SPOSLOYALTY3MANAGER.EXE", "SPOSLOYALTY3MANAGER.EXE", strDataPath, NULL );
}

/**********************************************************************/

void CDataAccessHelpersLoyalty::RunLoyaltyV4()
{
	SSspawnl(m_pParentWnd->GetSafeOwner(m_pParentWnd), P_WAIT, "SPOSLOYALTY4MANAGER.EXE", "SPOSLOYALTY4MANAGER.EXE", NULL);
}

/**********************************************************************/

void CDataAccessHelpersLoyalty::RunSmartPayV1()
{
	CString strDataPath;
	strDataPath.Format( "%d", SysInfo.GetDataPathNo() );
	SSspawnl ( m_pParentWnd -> GetSafeOwner( m_pParentWnd ), P_WAIT, "SPOSSMARTPAYMANAGER.EXE", "SPOSSMARTPAYMANAGER.EXE", strDataPath, NULL );
}

/**********************************************************************/

void CDataAccessHelpersLoyalty::RunSmartPayV2()
{
	CString strDataPath;
	strDataPath.Format( "%d", SysInfo.GetDataPathNo() );
	SSspawnl ( m_pParentWnd -> GetSafeOwner( m_pParentWnd ), P_WAIT, "SPOSSMARTPAY2MANAGER.EXE", "SPOSSMARTPAY2MANAGER.EXE", strDataPath, NULL );
}

/**********************************************************************/

void CDataAccessHelpersLoyalty::RunSmartPayV4()
{
	SSspawnl(m_pParentWnd->GetSafeOwner(m_pParentWnd), P_WAIT, "SPOSSMARTPAY4MANAGER.EXE", "SPOSSMARTPAY4MANAGER.EXE", NULL);
}

/**********************************************************************/
