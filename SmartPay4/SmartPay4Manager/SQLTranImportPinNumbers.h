#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranImportPINNumbers : public CSQLTranBase
{
public:
	void DoWork(CReportConsolidationArray<CSortedIntByInt64>& arrayPINs, bool bRemoveExisting);

public:
	CString m_strError = "";
};

/**********************************************************************/
