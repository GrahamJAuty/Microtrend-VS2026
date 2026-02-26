#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "PosTrayTask.h"
/**********************************************************************/
#include "PosTrayExportCreatorBase.h"
/**********************************************************************/

class CPosTrayExportCreatorVoid : public CPosTrayExportCreatorBase
{
public:
	CPosTrayExportCreatorVoid( CPosTrayTask& Task, bool& bQuitNow, void* pHostDialog = NULL, int nManualExportNo = 1 );
	
private:
	__int64 ProcessTaskInternal();
};

/**********************************************************************/

