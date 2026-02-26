#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "PosTrayTask.h"
/**********************************************************************/
#include "PosTrayExportCreatorBase.h"
/**********************************************************************/

class CPosTrayExportCreatorSales : public CPosTrayExportCreatorBase
{
public:
	CPosTrayExportCreatorSales( CPosTrayTask& Task, bool& bQuitNow, void* pHostDialog = NULL, int nManualExportNo = 1 );
	
protected:
	__int64 ProcessTaskInternal();
};

/**********************************************************************/
