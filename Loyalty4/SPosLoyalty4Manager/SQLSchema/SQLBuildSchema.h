#pragma once
//**********************************************************************
#include "..\..\SPosLoyalty4Shared\SQLDb.h"
#include "..\..\SPosLoyalty4Shared\SQLDefines.h"
//**********************************************************************

class CSQLBuildSchema
{
public:
	CSQLBuildSchema();

public:
	void DoBuild(CSQLDb& SQLDb, int& nErrorContext, int& nErrorNumber);

private:
	bool BuildCurrentSchema();
	bool Migrate(int nTargetVersion);
};

//**********************************************************************
