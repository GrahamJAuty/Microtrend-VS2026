#pragma once
/**********************************************************************/
#include "5000SiteArray.h"
#include "RealTimeCommsThread.h"
/**********************************************************************/
#define CASHRSP_FLUSH_IDRAUGHT 0
#define CASHRSP_FLUSH_SAGE 1
#define CASHRSP_FLUSH_SYSTEM 2
/**********************************************************************/
#define CASHRSP_FLUSH_ERROR_NONE 0
#define CASHRSP_FLUSH_ERROR_READ_SALES 1
#define CASHRSP_FLUSH_ERROR_COPY_SALES 2
#define CASHRSP_FLUSH_ERROR_NETWORK_LOCK 3
#define CASHRSP_FLUSH_ERROR_NETWORK_NOPATH 4
/**********************************************************************/

class CCashRSPFlusher
{
public:
#ifndef POSTRAY_UTILITY
	static long DoComms( LPVOID pParam, int nType );
	static long GetErrorCode( int nType, int nError );
#endif

public:
	static bool CheckCashRSPFile( CFileFind& finder, CRealTimeFileBufferInfo& FileBufferInfo );
	static bool ProcessRealTimeFile ( CRealTimeCommsThreadInfo* pInfo, int nRenameType, CString& strOriginalFilename, CString& strRenamedFilename, CString& strLocalTempFile, int nFileType );

private:
#ifndef POSTRAY_UTILITY
	static long BuildCASHRSPBuffer ( CRealTimeCommsThreadInfo* pInfo, CArray<CRealTimeFileBufferInfo,CRealTimeFileBufferInfo>& arrayBuffer, int nType, int nStage );
	static long ProcessCASHRSPBuffer( CRealTimeCommsThreadInfo* pInfo, CArray<CRealTimeFileBufferInfo,CRealTimeFileBufferInfo>& arrayBuffer, int nType );
#endif
};

/**********************************************************************/

