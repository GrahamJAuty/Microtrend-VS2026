//*******************************************************************
#include "AutoShutdownStacker.h"
//*******************************************************************

CAutoShutdownDlgStacker::CAutoShutdownDlgStacker(CSSAutoShutdownDialog* pHost)
{
	AutoShutdownHelper.SetActiveWindow(NULL);
	m_pHost = pHost;
}

//*******************************************************************

CAutoShutdownDlgStacker::~CAutoShutdownDlgStacker()
{
	if (AutoShutdownHelper.IsTriggered())
	{
		TRACE("Stacker Dialog Log Off\n\n");
		AutoShutdownHelper.SetActiveWindow(NULL);
		m_pHost->CSSAutoShutdownLogOff();
	}
	else
	{
		TRACE("Stacker Dialog Revert\n\n");
		AutoShutdownHelper.SetActiveWindow(m_pHost);
	}
}

//*******************************************************************
//*******************************************************************
//*******************************************************************

CAutoShutdownPropSheetStacker::CAutoShutdownPropSheetStacker(CSSAutoShutdownPropertySheet* pHost)
{
	AutoShutdownHelper.SetActiveWindow(NULL);
	m_pHost = pHost;
}

//*******************************************************************

CAutoShutdownPropSheetStacker::~CAutoShutdownPropSheetStacker()
{
	if (AutoShutdownHelper.IsTriggered())
	{
		AutoShutdownHelper.SetActiveWindow(NULL);
		TRACE("Stacker PropSheet Log Off\n\n");
		m_pHost->CSSAutoShutdownLogOff();
	}
	else
	{
		TRACE("Stacker PropSheet Revert\n\n");
		AutoShutdownHelper.SetActiveWindow(m_pHost);
	}
}

//*******************************************************************
