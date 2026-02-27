#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranUnhotlistAccount : public CSQLTranBase
{
public:
	void DoWork(__int64 nUserID, CString strComment);
};

/**********************************************************************/
