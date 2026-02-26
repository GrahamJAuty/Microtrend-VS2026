#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CCashRSPImporterBatchInfo
{
public:
	CCashRSPImporterBatchInfo()
	{
		m_nProcessType = 0;
		m_strRebuildDateFrom = "";
		m_strRebuildDateTo = "";
		m_strLocalPath = "";
		m_nFileType = 0;
		m_nNetworkIdx = 0;
		m_nCurrentServerDbIdx = 0;
		m_nCurrentServerLocIdx = 0;
		m_strTNoOriginal = "";
	};

public:
	int m_nProcessType;
	CString m_strRebuildDateFrom;
	CString m_strRebuildDateTo;
	CString m_strLocalPath;
	int m_nFileType;
	int m_nNetworkIdx;
	int m_nCurrentServerDbIdx;
	int m_nCurrentServerLocIdx;
	CString m_strTNoOriginal;
	CReportConsolidationArray<CSortedStringItem> m_arrayRebuildHeaders;
};

/**********************************************************************/