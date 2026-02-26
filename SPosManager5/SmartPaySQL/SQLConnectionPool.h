#pragma once
//**********************************************************************
#include "SQLDb.h"
//**********************************************************************

class CSQLConnectionPool
{
public:
	~CSQLConnectionPool();

public:
	CDatabase* GetPooledConnection(int& nConnection, CDatabase* pDatabase = NULL);
	void FreeConnection(int nConnection);
	void DeleteConnection(int nConnection);
	void DeleteAllConnections();

private:
	CArray<CSQLDb*, CSQLDb*> m_arrayConnections;
};

//**********************************************************************
extern CSQLConnectionPool SQLConnectionPool;
//**********************************************************************
