/**********************************************************************/
//#include "globalfunctions.h"
/**********************************************************************/
#include "LocationCSVArray.h"
/**********************************************************************/

const char* CLocationCSVArray::GetFolderPathSysset( int nLocIdx )
{
	m_strFolderPath = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		m_strFolderPath.Format ( "NetWk%3.3d\\Loc%3.3d", 
			pLocation -> m_nNetworkNo, 
			pLocation -> m_nLocNo );

		GetSyssetProgramPath( m_strFolderPath );
	}
	return m_strFolderPath;	
}

/**********************************************************************/

const char* CLocationCSVArray::GetFolderPathSyssetDelete( int nLocIdx )
{
	m_strFolderPathDelete = GetFolderPathSysset( nLocIdx );
	m_strFolderPathDelete += "XXX";
	return m_strFolderPathDelete;	
}

/**********************************************************************/

