#pragma once
/**********************************************************************/
#include "AuditRecord.h"
/**********************************************************************/
#include "..\SPOSLoyalty4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranUnhotlistAccount : public CSQLTranBase
{
public:
	void DoWork(__int64 nUserID, CString strComment);
};

/**********************************************************************/
