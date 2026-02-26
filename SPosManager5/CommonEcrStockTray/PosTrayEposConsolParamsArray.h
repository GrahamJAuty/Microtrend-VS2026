#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "EposReportSelect.h"
#include "PosTrayEposConsolParamsInfo.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CPosTrayEposConsolParamsArray
{
public:
	CPosTrayEposConsolParamsArray();
	void SetBatchNo( int nBatchNo );

public:
	void Read( bool bModifiedBatch );
	void Write();

public:
	void SetParams( CEposReportSelectInfo& infoReport, CPosTrayEposConsolParamsInfo& infoParams );
	void GetParams( CEposReportSelectInfo& infoReport, CPosTrayEposConsolParamsInfo& infoParams );

private:
	CReportConsolidationArray<CSortedStringByInt> m_arrayParams;

private:
	int m_nBatchNo;
	CString m_strBatchFilename;
	CString m_strTempFilename;
	CString m_strParams;
};

/**********************************************************************/
