/**********************************************************************/
#include "CustomExportManager.h"	
#include "DatabaseCSVArray.h"
#include "ListDataServerDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DataAccessHelpersServer.h"
/**********************************************************************/

CDataAccessHelpersServer::CDataAccessHelpersServer()
{
	m_bOpenedServer = FALSE;
	m_bOpenedRestrict = FALSE;
	m_bOpenedLogon = FALSE;
	m_nErrorType = OPENSERVER_ERROR_NONE;
}

/**********************************************************************/

void CDataAccessHelpersServer::EditServer(int nServerDbIdx, int nServerLocIdx, int nTerminalLocIdx)
{
	DataManagerNonDb.CCMacroActive.Reset();

	if (nServerLocIdx != -1)
	{
		DataManagerNonDb.CCMacroActive.LoadParentMacrosLocation(nServerLocIdx);
	}

	if (nServerDbIdx != -1)
	{
		DataManagerNonDb.CCMacroActive.LoadParentMacrosDatabase(nServerDbIdx);
	}

	DataManagerNonDb.CCMacroActive.LoadParentMacrosSystem();

	EditServerInternal(nServerDbIdx, nServerLocIdx, nTerminalLocIdx);

	if (TRUE == m_bOpenedServer)
	{
		DataManagerNonDb.CloseServer(m_infoServer);
	}

	if (TRUE == m_bOpenedRestrict)
	{
		DataManagerNonDb.CloseServerRestriction(m_infoRestrict);
	}

	if (TRUE == m_bOpenedLogon)
	{
		DataManagerNonDb.CloseServerLogon(m_infoLogon);
	}

	switch (m_nErrorType)
	{
	case OPENSERVER_ERROR_OPEN_SERVER:
		Prompter.ShareError(m_infoServer);
		break;

	case OPENSERVER_ERROR_OPEN_RESTRICT:
		Prompter.ShareError(m_infoRestrict);
		break;

	case OPENSERVER_ERROR_OPEN_LOGON:
		Prompter.ShareError(m_infoLogon);
		break;

	case OPENSERVER_ERROR_WRITE_SERVER:
		Prompter.WriteError(m_infoServer);
		break;

	case OPENSERVER_ERROR_WRITE_RESTRICT:
		Prompter.WriteError(m_infoRestrict);
		break;

	case OPENSERVER_ERROR_WRITE_LOGON:
		Prompter.WriteError(m_infoLogon);
		break;
	}
}

/**********************************************************************/

void CDataAccessHelpersServer::EditServerInternal(int nServerDbIdx, int nServerLocIdx, int nTerminalLocIdx)
{
	m_bOpenedServer = FALSE;
	m_bOpenedRestrict = FALSE;
	m_bOpenedLogon = FALSE;
	m_nErrorType = OPENSERVER_ERROR_NONE;

	if (DataManagerNonDb.OpenServer(DB_READWRITE, nServerDbIdx, nServerLocIdx, m_infoServer) == FALSE)
	{
		m_nErrorType = OPENSERVER_ERROR_OPEN_SERVER;
		return;
	}
	else
	{
		m_bOpenedServer = TRUE;
	}

	if (DataManagerNonDb.OpenServerLogon(DB_READWRITE, nServerDbIdx, nServerLocIdx, m_infoLogon) == FALSE)
	{
		m_nErrorType = OPENSERVER_ERROR_OPEN_LOGON;
		return;
	}
	else
	{
		m_bOpenedLogon = TRUE;
	}

	if (nTerminalLocIdx != -1)
	{
		if (DataManagerNonDb.OpenServerRestriction(DB_READWRITE, nTerminalLocIdx, m_infoRestrict) == FALSE)
		{
			m_nErrorType = OPENSERVER_ERROR_OPEN_RESTRICT;
			return;
		}
		else
		{
			m_bOpenedRestrict = TRUE;
		}
	}

	CListDataServerDlg dlg(nServerDbIdx, nServerLocIdx, nTerminalLocIdx, m_pParentWnd);

	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	if (DataManagerNonDb.WriteServer(m_infoServer) == FALSE)
	{
		m_nErrorType = OPENSERVER_ERROR_WRITE_SERVER;
		return;
	}

	if (SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_SMP_V4)
	{
		CCustomExportManagerBase Export;
		Export.WriteOperatorFile();
	}

	if (DataManagerNonDb.WriteServerLogon(m_infoLogon) == FALSE)
	{
		m_nErrorType = OPENSERVER_ERROR_WRITE_LOGON;
		return;
	}

	if (nTerminalLocIdx != -1)
	{
		DataManagerNonDb.ServerRestriction.PurgeDeletedServers();
		if (DataManagerNonDb.WriteServerRestriction(m_infoRestrict) == FALSE)
		{
			m_nErrorType = OPENSERVER_ERROR_WRITE_RESTRICT;
			return;
		}
	}
}

/**********************************************************************/
