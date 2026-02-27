/**********************************************************************/
#include "TLogCache.h"
/**********************************************************************/

CTLogCache::CTLogCache()
{
}

/**********************************************************************/

CTLogCache::~CTLogCache()
{
	Reset();
}

/**********************************************************************/

void CTLogCache::Reset()
{
	m_arrayTLogMap.RemoveAll();

	for (int n = 0; n < m_arrayTLogDatabases.GetSize(); n++)
	{
		delete (m_arrayTLogDatabases.GetAt(n));
	}

	m_arrayTLogDatabases.RemoveAll();
}

/**********************************************************************/
