/**********************************************************************/
 
/**********************************************************************/
//#include "globalfunctions.h"
/**********************************************************************/
#include "DatabaseCSVArray.h"
/**********************************************************************/

const char* CDatabaseCSVArray::GetFolderPathSysset( int nDbIdx )
{
	m_strFolderPath = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx ); 
		m_strFolderPath.Format ( "Db%3.3d", pDatabase -> m_nDbNo );
		GetSyssetProgramPath( m_strFolderPath );
	}
	return m_strFolderPath;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetFolderPathSyssetDelete( int nDbIdx )
{
	m_strFolderPathDelete = GetFolderPathSysset( nDbIdx );
	m_strFolderPathDelete += "XXX";
	return m_strFolderPathDelete;	
}

/**********************************************************************/

