#pragma once
/**********************************************************************/
#include "EposReportPMSModes.h"
#include "EposSelectArray.h"
#include "ReasonInfo.h"
#include "ReportConsolidationArray.h"
#include "TermFileListInfo.h"
/**********************************************************************/

class CEposReportFileListMaker
{
public:
	CEposReportFileListMaker( bool bEODMode, bool bSaleTime, CEposReportPMSModes& PMSModes );
	~CEposReportFileListMaker(void);

public:
	CString GetCheckDateTimeFrom() { return m_strCheckDateTimeFrom; }
	CString GetCheckDateTimeTo() { return m_strCheckDateTimeTo; }

public:
	void GetFileList ( CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, int nFileType, CEposSelectArray& SelectArray, bool bNoMakeList, int& nTotalFileSize );
	void GetTermFileList ( CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, CEposSelectArray& SelectArray, bool bNoMakeList, int& nTotalFileSize );
	void GetTermFileListRebuild( CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, CEposSelectArray& SelectArray, int& nTotalFileSize );
	void GetReasonFileList ( CArray<CReasonFileListInfo,CReasonFileListInfo>& array, CEposSelectArray& SelectArray, int& nTotalFileSize );

private:
	bool m_bEODMode;
	bool m_bSaleTime;
	CEposReportPMSModes m_PMSModes;
	CString m_strCheckDateTimeFrom;
	CString m_strCheckDateTimeTo;
};

/**********************************************************************/

