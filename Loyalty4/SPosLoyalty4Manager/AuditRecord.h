#pragma once
//*******************************************************************
#include "TLogDatabase.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\AuditRecord_base.h"
//*******************************************************************

class CAuditRecord : public CAuditRecord_base
{
public:
	CAuditRecord(void){}

public:
	bool AuditChanges(CSQLRowAccountFull& RowAccount, bool bForceAudit = FALSE);

public:
	CString ExtractPurchaseText(CTLogDatabase* pDatabase, bool bNewDatabase);
};

//*******************************************************************
