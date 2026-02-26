#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CServerDatabaseHeaderField
{
public:
	CString m_strCurrent;
};

/**********************************************************************/

class CServerDatabaseHeader
{
public:
	CServerDatabaseHeader(void);

public:
	const char* GetDatabaseHeader();
	
private:
	void AddField( const char* szCurrent );
	
private:
	CStringArray m_arrayCurrentFields;
	
private:
	CString m_strHeader;
};

/**********************************************************************/
extern CServerDatabaseHeader ServerDatabaseHeader;
/**********************************************************************/
