#pragma once
/**********************************************************************/
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
/**********************************************************************/

struct CLoyReqTidyUpThreadInfo
{
public:
	CLoyReqTidyUpThreadInfo();

public:
	CString m_strREQFolder;
	CReportConsolidationArray<CTidyUpFileInfo>* m_pTidyUpFileBuffer;
};

/**********************************************************************/

class CLoyReqTidyUpThread
{
public:
	static UINT DoWork( LPVOID pParam );

private:
	static void DoWorkInternal( CLoyReqTidyUpThreadInfo* pInfo );
};

/**********************************************************************/
