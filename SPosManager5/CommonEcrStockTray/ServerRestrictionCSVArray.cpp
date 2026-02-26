/**********************************************************************/
 
/**********************************************************************/
//#include "DataManagerNonDb.h"
/**********************************************************************/
#include "ServerRestrictionCSVArray.h"
/**********************************************************************/

CServerRestrictionCSVArray::CServerRestrictionCSVArray() : CSharedCSVArray()
{
	m_nCurrentLocIdx = -1;
}

/**********************************************************************/

void CServerRestrictionCSVArray::RemoveAll()
{
	RemoveAt( 0, GetSize() );
}

/**********************************************************************/

int CServerRestrictionCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAll();

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	return nReply;
}

/**********************************************************************/

bool CServerRestrictionCSVArray::FindServerByNumber( int nServerNo, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;
		
		int nArrayNo = GetServerNo( nIndex );
			
		if ( nServerNo == nArrayNo )
			return TRUE;

		if ( nServerNo < nArrayNo )
			nEnd = nIndex - 1;
		else
			nStart = nIndex + 1;
	}

	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

int CServerRestrictionCSVArray::GetServerNo( int nIndex )
{
	int nServerNo = 0;

	CString strTemp;
	if ( ( strTemp = GetAt ( nIndex ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		nServerNo = csv.GetInt(0);
	}

	return nServerNo;
}

/**********************************************************************/

const char* CServerRestrictionCSVArray::GetTimeStamp( int nIndex )
{
	m_strTimeStamp = "";

	CString strTemp;
	if ( ( strTemp = GetAt ( nIndex ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 2 );
		m_strTimeStamp = csv.GetString(1);
	}

	return m_strTimeStamp;
}

/**********************************************************************/

void CServerRestrictionCSVArray::GetRestrictions( CServerCSVRecord& ServerRecord, CServerRestrictionCSVRecord& TerminalRecord )
{
	TerminalRecord.ClearRecord();
	TerminalRecord.SetServerNo( ServerRecord.GetServerNo() );
	TerminalRecord.SetTimeStamp( ServerRecord.GetTimeStamp() );
	
	int nIndex = 0;
	if (FindServerByNumber(ServerRecord.GetServerNo(), nIndex) == TRUE)
	{
		CString strTimeStamp = ServerRecord.GetTimeStamp();
		if (strTimeStamp == GetTimeStamp(nIndex))
		{
			GetAt(nIndex, TerminalRecord);
		}
	}
}

/**********************************************************************/

void CServerRestrictionCSVArray::SetRestrictions( CServerRestrictionCSVRecord& TerminalRecord )
{
	CString strRestrictions = TerminalRecord.GetRestrictions();

	if (strRestrictions == "")
	{
		int nIndex;
		if (FindServerByNumber(TerminalRecord.GetServerNo(), nIndex) == TRUE)
		{
			RemoveAt(nIndex);
		}
	}
	else
	{
		int nIndex;
		if (FindServerByNumber(TerminalRecord.GetServerNo(), nIndex) == TRUE)
		{
			SetAt(nIndex, TerminalRecord);
		}
		else
		{
			InsertAt(nIndex, TerminalRecord);
		}
	}
}

/**********************************************************************/

void CServerRestrictionCSVArray::PurgeDeletedServers()
{
	int nServerPos = 0;
	int nRestrictPos = 0;
	
	while ( ( nRestrictPos < GetSize() ) && ( nServerPos < DataManagerNonDb.Server.GetSize() ) )
	{
		int nServerNo = DataManagerNonDb.Server.GetServerNo( nServerPos );
		int nRestrictNo = GetServerNo( nRestrictPos );

		if ( nServerNo == nRestrictNo )
		{
			nServerPos++;
			nRestrictPos++;
		}
		else if ( nServerNo < nRestrictNo )
		{
			nServerPos++;
		}
		else
		{
			RemoveAt( nRestrictPos );
		}
	}

	for ( int n = GetSize() - 1; n >= nRestrictPos; n-- )
		RemoveAt(n);
}

/**********************************************************************/
