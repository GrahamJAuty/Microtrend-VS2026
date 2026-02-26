/**********************************************************************/
#include "TrackedFile.h"
/**********************************************************************/

CTrackedFile::CTrackedFile()
{
	m_strFilename = "";
	m_ModifiedTime = CTime( 1980, 1, 1, 0, 0, 0, -1 );
}

/**********************************************************************/

void CTrackedFile::SetFilename( CString strFilename )
{
	m_strFilename = strFilename;
	m_ModifiedTime = CTime( 1980, 1, 1, 0, 0, 0, -1 );
}

/**********************************************************************/

bool CTrackedFile::CheckFile( bool bForce )
{
	bool bResult = FALSE;

	if ( m_strFilename != "" )
	{
		bResult = bForce;

		CFileStatus FileStatus;
		if ( CFile::GetStatus( m_strFilename, FileStatus ) == TRUE )
		{
			if ( ( FileStatus.m_mtime != m_ModifiedTime ) || ( TRUE == bForce ) )
			{
				m_ModifiedTime = FileStatus.m_mtime;
				bResult = TRUE;
			}
		}
	}

	return bResult;
}

/**********************************************************************/
