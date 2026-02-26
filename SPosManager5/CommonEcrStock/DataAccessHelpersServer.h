#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "DataAccessHelpers.h"
/**********************************************************************/
#define OPENSERVER_ERROR_NONE 0
#define OPENSERVER_ERROR_OPEN_SERVER 1
#define OPENSERVER_ERROR_OPEN_RESTRICT 2
#define OPENSERVER_ERROR_OPEN_LOGON 3
#define OPENSERVER_ERROR_WRITE_SERVER 4
#define OPENSERVER_ERROR_WRITE_RESTRICT 5
#define OPENSERVER_ERROR_WRITE_LOGON 6
/**********************************************************************/

class CDataAccessHelpersServer : public CDataAccessHelpers
{
public:
	CDataAccessHelpersServer();
	
public:
	void EditServer( int nServerDbIdx, int nServerLocIdx, int nTerminalLocIdx );

private:
	void EditServerInternal( int nServerDbIdx, int nServerLocIdx, int nTerminalLocIdx );

private:
	CDataManagerInfo m_infoServer;
	CDataManagerInfo m_infoRestrict;
	CDataManagerInfo m_infoLogon;
	bool m_bOpenedServer;
	bool m_bOpenedRestrict;
	bool m_bOpenedLogon;
	int m_nErrorType;
};

/**********************************************************************/
#endif
/**********************************************************************/
