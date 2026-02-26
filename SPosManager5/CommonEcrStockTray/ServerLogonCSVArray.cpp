/**********************************************************************/
#include "NamespaceServerLogon.h"
#include "SPOSVersions.h"
/**********************************************************************/
#include "ServerLogonCSVArray.h"
/**********************************************************************/

CServerLogonCSVRecord::CServerLogonCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CServerLogonCSVRecord::ClearRecord()
{
	m_nLogonNo = 0;
	m_nServerNo = 1;
}

/**********************************************************************/

void CServerLogonCSVRecord::ConvertFromCSV(CCSV& csv)
{
	ClearRecord();

	SetLogonNo(csv.GetInt64(0));

	switch (csv.GetInt(1))
	{
	case 1:
		V1ConvertFromCSV(csv);
		break;
	}
}

/**********************************************************************/

void CServerLogonCSVRecord::V1ConvertFromCSV(CCSV& csv)
{
	SetServerNo(csv.GetInt(2));
}

/**********************************************************************/

void CServerLogonCSVRecord::ConvertToCSV(CCSV& csv)
{
	csv.Add(m_nLogonNo);
	csv.Add(SERVERLOGON_VERSION);
	csv.Add(m_nServerNo);
}

/**********************************************************************/

void CServerLogonCSVRecord::SetLogonNo(__int64 n)
{
	Set(m_nLogonNo, n, ServerLogon::LogonNo);
}

/**********************************************************************/

void CServerLogonCSVRecord::SetServerNo( int n )
{
	Set ( m_nServerNo, n, Server::ServerNo );
}

/**********************************************************************/

void CServerLogonCSVRecord::CheckMinimumSPOSVersion( CMinimumSPOSVersion& version )
{
	version.m_nMinVer = SPOS_V4;
	version.m_strReason = "";
	version.m_bInvalidData = FALSE;

	if ( GetServerNo() >= 1000 )
	{
		version.m_nMinVer = SPOS_V4_1124;
		version.m_strReason = "Server number 1000 - 9999";
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CServerLogonCSVArray::CServerLogonCSVArray() 
{
}

/**********************************************************************/

CServerLogonCSVArray::~CServerLogonCSVArray() 
{
}

/**********************************************************************/

int CServerLogonCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if (nReply != DB_ERR_SHARE)
	{
		if (nMode == DB_READONLY)	// if file does not exist use default values
		{
			nReply = DB_ERR_NONE;
		}
	}
	
	__int64 nLastLogonNo = 0;
	int nValidRecords = 0;
	
	CServerLogonCSVRecord ServerLogonRecord;
	for ( ; nValidRecords < GetSize(); nValidRecords++ )
	{
		GetAt( nValidRecords, ServerLogonRecord );

		__int64 nLogonNo = ServerLogonRecord.GetLogonNo();
		if (nLogonNo <= nLastLogonNo || nLogonNo > ServerLogon::LogonNo.Max)
		{
			break;
		}

		nLastLogonNo = nLogonNo;
	}

	while (GetSize() > nValidRecords)
	{
		RemoveAt(GetSize() - 1);
	}

	return nReply;
}

/**********************************************************************/

bool CServerLogonCSVArray::FindLogonByNumber( __int64 nLogonNo, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;
		
		CServerLogonCSVRecord ServerLogon;
		GetAt ( nIndex, ServerLogon );
		__int64 nArrayNo = ServerLogon.GetLogonNo();
			
		if (nLogonNo == nArrayNo)
		{
			return TRUE;
		}

		if (nLogonNo < nArrayNo)
		{
			nEnd = nIndex - 1;
		}
		else
		{
			nStart = nIndex + 1;
		}
	}

	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

__int64 CServerLogonCSVArray::GetLogonNo( int nIndex )
{
	__int64 nLogonNo = 0;

	CString strTemp = "";
	if ( ( strTemp = GetAt ( nIndex ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		nLogonNo = csv.GetInt64(0);
	}

	return nLogonNo;
}

/**********************************************************************/

void CServerLogonCSVArray::GetLogonListForServer(int nServerNo, CArray<__int64, __int64>& arrayLogon)
{
	arrayLogon.RemoveAll();

	for (int n = 0; n < GetSize(); n++)
	{
		CServerLogonCSVRecord ServerLogon;
		GetAt(n, ServerLogon);

		if (ServerLogon.GetServerNo() == nServerNo)
		{
			arrayLogon.Add(ServerLogon.GetLogonNo());
		}
	}
}

/**********************************************************************/

void CServerLogonCSVArray::SetLogonListForServer(int nServerNo, CArray<__int64, __int64>& arrayLogon)
{
	if ( (nServerNo < Server::ServerNo.Min) || (nServerNo > Server::ServerNo.Max) )
	{
		return;
	}

	// Remove existing logons for this server
	for ( int nIndex = GetSize() - 1; nIndex >= 0; nIndex-- )
	{
		CServerLogonCSVRecord ServerLogon;
	
		GetAt(nIndex, ServerLogon);
		if (ServerLogon.GetServerNo() == nServerNo)
		{
			RemoveAt(nIndex);
		}
	}

	for (int nIndex = 0; nIndex < arrayLogon.GetSize(); nIndex++)
	{
		__int64 nLogonNo = arrayLogon.GetAt(nIndex);

		if ((nLogonNo < ServerLogon::LogonNo.Min) || (nLogonNo > ServerLogon::LogonNo.Max))
		{
			continue;
		}

		CServerLogonCSVRecord ServerLogon;
		ServerLogon.SetLogonNo(nLogonNo);
		ServerLogon.SetServerNo(nServerNo);

		int nPos = 0;
		if (FindLogonByNumber(nLogonNo, nPos) == FALSE)
		{
			InsertAt(nPos, ServerLogon);
		}
		else
		{
			SetAt(nPos, ServerLogon);
		}
	}
}

/**********************************************************************/
