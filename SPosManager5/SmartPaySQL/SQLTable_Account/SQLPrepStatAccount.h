#pragma once
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowAccountFull.h"
#include "SQLRecordSetAccountFull.h"
//**********************************************************************

class CSQLPrepStatAccount
{
public:
	CSQLPrepStatAccount();

public:
	CSQLResultInfo SelectRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase );
};

//**********************************************************************

