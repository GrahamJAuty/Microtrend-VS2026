//**********************************************************************
#include "SQLConnectionPool.h"
//**********************************************************************

CDatabase* CSQLConnectionPool::GetPooledConnection(int& nConnection, CDatabase* pDatabase)
{
	if (pDatabase != NULL)
	{
		nConnection = -1;
		return pDatabase;
	}

	int nFirstNullPos = -1;

	for (int n = 0; n < m_arrayConnections.GetSize(); n++)
	{
		CSQLDb* pDb = m_arrayConnections[n];

		if (NULL == pDb)
		{
			if (-1 == nFirstNullPos)
			{
				nFirstNullPos = n;
			}
		}
		else if ( pDb -> IsInUse() == FALSE)
		{
			CString str = "";
			str.Format("Reuse db connection %d\n", n);
			TRACE(str);

			int nError = 0;
			
			nConnection = n;
			m_arrayConnections[n]->SetInUseFlag(TRUE);
			return m_arrayConnections[n]->GetDb();
		}
	}

	CSQLDb* pDb = new CSQLDb;
	
	if (-1 == nFirstNullPos)
	{
		CString str = "";
		nConnection = m_arrayConnections.GetSize();
		str.Format("New db connection %d\n", nConnection);
		TRACE(str);

		m_arrayConnections.Add(pDb);
	}
	else
	{
		CString str = "";
		nConnection = nFirstNullPos;
		str.Format("Recreate db connection %d\n", nConnection);
		TRACE(str);

		m_arrayConnections.SetAt(nConnection, pDb);
	}

	int nError = 0;
	pDb->Connect(nError);

	pDb->SetInUseFlag(TRUE);
	return pDb->GetDb();
}

//**********************************************************************

CSQLConnectionPool::~CSQLConnectionPool()
{
	for (int n = 0; n < m_arrayConnections.GetSize(); n++)
	{
		CSQLDb* pDb = m_arrayConnections.GetAt(n);

		if (pDb != NULL)
		{
			pDb->Close();
			delete pDb;
		}
	}
}

//**********************************************************************

void CSQLConnectionPool::FreeConnection(int n)
{
	if ((n >= 0) && (n < m_arrayConnections.GetSize()))
	{
		CSQLDb* pDb = m_arrayConnections.GetAt(n);

		if (pDb != NULL)
		{
			CString str = "";
			str.Format("Free db connection %d\n", n);
			TRACE(str);

			pDb->SetInUseFlag(FALSE);
		}
		else
		{
			CString str = "";
			str.Format("Free db connection called for null db %d\n", n);
			TRACE(str);
		}
	}
}

//**********************************************************************

void CSQLConnectionPool::DeleteConnection(int n)
{
	if ((n >= 0) && (n < m_arrayConnections.GetSize()))
	{
		CSQLDb* pDb = m_arrayConnections.GetAt(n);

		if (pDb != NULL)
		{
			CString str = "";
			str.Format("Delete db connection %d\n", n);
			TRACE(str);

			delete (pDb);
			m_arrayConnections.SetAt(n, NULL);
		}
		else
		{
			CString str = "";
			str.Format("Delete db connection called for null db %d\n", n);
			TRACE(str);
		}
	}
}

//**********************************************************************

void CSQLConnectionPool::DeleteAllConnections()
{
	for (int n = 0; n < m_arrayConnections.GetSize(); n++)
	{
		DeleteConnection(n);
	}
}

//**********************************************************************
