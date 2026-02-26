#pragma once
/**********************************************************************/
#include "5000Server.h"
#include "CashRSPImporterStatusDisplay.h"
#include "..\SPosTray5\PosTrayStatusDlg.h"
/**********************************************************************/

class CCommsHandshaker
{
public:
	static void InitialiseComms();
	static void BeginComms();
	static void TerminateComms();
	static void TerminateThread();

	static void EnableComms();
	static void DisableComms();

	static void PauseComms();
	static void ResumeComms();

	static void PMSPulse();

#ifndef POSTRAY_UTILITY
	static void ProcessSales();
#else
	static bool ProcessSalesForPosTray( CCashRSPImporterStatusDisplay& StatusDisplay );
	static bool CheckStoppedForPosTray();
#endif

	static bool ConfirmStopped( const char* szAction, CWnd* pParent );
	static bool ConfirmStopped( const char* szAction, bool& bFlag, CWnd* pParent );
	static bool ConfirmStoppedOrPMS( const char* szAction, bool bPMS, CWnd* pParent );

	static bool GetConfirmStoppedFlag();
	
	static int GetTranCountForPosTray( const char* sz );

	static bool GetEnableRealTimeSalesFlag(){ return m_bEnableRealTime; }

#ifndef POSTRAY_UTILITY
	static long FetchAllRealTimeSales( int nNetworkIdx, int nType );
#endif
	
private:
	static C5000Server m_5000Server;
	static bool m_bInitialisedComms;
	static bool m_bEnableRealTime;
};

/**********************************************************************/
extern CCommsHandshaker CommsHandshaker;
/**********************************************************************/
