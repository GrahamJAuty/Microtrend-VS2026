#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
//**********************************************************************

class CSQLBuildSchema
{
public:
	CSQLBuildSchema();

public:
	void DoBuild( CSQLDb& SQLDb, int& nErrorContext, int& nErrorNumber);

private:
	bool BuildCurrentSchema();
	bool Migrate(int nTargetVersion);
};

//**********************************************************************
