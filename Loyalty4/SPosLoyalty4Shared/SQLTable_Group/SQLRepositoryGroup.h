#pragma once
//**********************************************************************
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "..\..\SPosLoyalty4Shared\IDatabase.h"
#include "..\..\SPosLoyalty4Shared\SQLTranBase.h"
//**********************************************************************
#include "SQLRecordSetGroup.h"
#include "SQLRowGroup.h"
//**********************************************************************

class CSQLRepositoryGroup
{
public:
	CSQLRepositoryGroup();
	CSQLRepositoryGroup(CSQLTranBase* pTransaction);

public:
	CSQLResultInfo SelectRow(CSQLRowGroup& SQLRowGroup, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowGroup& SQLRowGroup, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowGroup& SQLRowGroup, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowGroup& SQLRowGroup, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow_Modern(CSQLRowGroup& SQLRowGroup);
	CSQLResultInfo RowExists(int nGroupNo, CDatabase* pDatabase);

private:
	CSQLTranBase* m_pTransaction;
	CDatabase* GetLegacyDatabase(CDatabase* pDatabase) const;
	IDatabase* GetModernDatabase() const {return nullptr;}
};

//**********************************************************************

