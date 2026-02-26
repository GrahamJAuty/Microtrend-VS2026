/**********************************************************************/
//#include "DataManager.h"
//#include "EcrmanOptionsIni.h"
//#include "globalfunctions.h"
#include "LocationCSVArray.h"
#include "MaxLengths.h"
/**********************************************************************/
#include "IDraughtOptions.h"
/**********************************************************************/

CIDraughtOptions::CIDraughtOptions()
{
	m_ConnectRetryTime.Create( "ConnectRetryTime", 1, 30, 1 );
	m_ExportDelayTime.Create( "ExportDelayTime", 1, 6, 1 );
	m_FTPServer.Create( "FTPServer", MAX_LENGTH_IDRAUGHT_SERVER, "ftp.brulines.com" );
	m_FTPUserName.Create( "FTPUserName", MAX_LENGTH_IDRAUGHT_USERNAME, "" );
	m_FTPPassword.Create( "FTPPassword", MAX_LENGTH_IDRAUGHT_PASSWORD, "" );

	m_strSiteIDBuffer = "";

	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CIDraughtOptions::CopyFrom( CIDraughtOptions& source )
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile( iniFile );
	ReloadFromIniFile( iniFile );
}

/**********************************************************************/

void CIDraughtOptions::ReloadFromIniFile( CEnhancedIniFile& iniFile )
{
	iniFile.ReadInt( m_ConnectRetryTime );
	iniFile.ReadInt( m_ExportDelayTime );
	iniFile.ReadString( m_FTPServer );
	iniFile.ReadString( m_FTPUserName );
	iniFile.ReadString( m_FTPPassword );
}

/**********************************************************************/

void CIDraughtOptions::PrepareIniFile( CEnhancedIniFile& iniFile )
{
	iniFile.WriteInt( m_ConnectRetryTime );
	iniFile.WriteInt( m_ExportDelayTime );
	iniFile.WriteString( m_FTPServer );
	iniFile.WriteString( m_FTPUserName );
	iniFile.WriteString( m_FTPPassword );
}

/**********************************************************************/

void CIDraughtOptions::Reset()
{
	m_ConnectRetryTime.Reset();
	m_ExportDelayTime.Reset();
	m_FTPServer.Reset();
	m_FTPUserName.Reset();
	m_FTPPassword.Reset();
}

/**********************************************************************/

bool CIDraughtOptions::Read()
{
	CFilenameUpdater FnUp( SysFiles::IDraughtOptions );
	CString strFilename = FnUp.GetFilenameToUse();
	
	if ( ::FileExists( strFilename ) == TRUE )
	{
		CEnhancedIniFile iniFile;
		if ( iniFile.Read( strFilename ) == FALSE )
		{
			m_bFatalReadError = TRUE;
			return FALSE;
		}

		ReloadFromIniFile( iniFile );
		return TRUE;
	}

	return TRUE;
}

/**********************************************************************/

bool CIDraughtOptions::Write()
{
	bool bResult = FALSE;

	if ( DataManager.LockAllUsers() == TRUE )
	{
		::CreateSyssetSubdirectory( "SysFiles" );

		CEnhancedIniFile iniFile;
		PrepareIniFile( iniFile );
		
		CFilenameUpdater FnUp( SysFiles::IDraughtOptions );
		bResult = iniFile.Write( FnUp.GetFilenameToUse() );

		DataManager.UnlockAllUsers();
	}
		
	return bResult;
}

/**********************************************************************/

void CIDraughtOptions::CreateIDraughtFolders()
{
	CreateDataSubdirectory ( "IDraught" );
	CreateDataSubdirectory ( "IDraught\\Pending" );
	CreateDataSubdirectory ( "IDraught\\Sent" );
}

/**********************************************************************/

void CIDraughtOptions::ResetDepartmentFilters()
{
	m_arrayDeptFilter.RemoveAll();

	CIDraughtDeptFilter info;
	info.m_strFilter = "";
	info.m_bChecked = FALSE;
	info.m_bInvalid = FALSE;
	info.m_nErrorInfo = 0;

	if ( EcrmanOptions.GetGlobalDepartmentFlag() == TRUE )
		m_arrayDeptFilter.Add( info );
	else
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
			m_arrayDeptFilter.Add( info );
	}
}

/**********************************************************************/

void CIDraughtOptions::GetDatabaseDepartmentFilter( int nDbIdx, CIDraughtDeptFilter& info )
{
	int nDbIdxToCheck;
	if ( EcrmanOptions.GetGlobalDepartmentFlag() == TRUE )
		nDbIdxToCheck = 0;
	else
		nDbIdxToCheck = nDbIdx;

	if ( ( nDbIdxToCheck < 0 ) || ( nDbIdxToCheck >= m_arrayDeptFilter.GetSize() ) )
	{
		info.m_strFilter = "";
		info.m_bChecked = TRUE;
		info.m_bInvalid = TRUE;
		info.m_nErrorInfo = -nDbIdxToCheck;
	}
	else
	{
		info = m_arrayDeptFilter.GetAt( nDbIdxToCheck );

		if ( FALSE == info.m_bChecked )
		{
			info.m_strFilter = "";
			info.m_bChecked = TRUE;
			info.m_bInvalid = TRUE;
			info.m_nErrorInfo = dbDatabase.GetDbNo( nDbIdx );

			::AddLeading( info.m_strFilter, Dept::DeptNo.Max + 1, '0' );

			CDataManagerInfo infoDb;
			DataManager.OpenDatabaseQuickNoFiles( nDbIdxToCheck, FALSE );
			DataManager.OpenDepartment( DB_READONLY, infoDb );

			for ( int nDeptIdx = 0; nDeptIdx < DataManager.Department.GetSize(); nDeptIdx++ )
			{
				CDepartmentCSVRecord Department;
				DataManager.Department.GetAt( nDeptIdx, Department );

				if ( Department.GetIDraughtExportFlag() == TRUE )
				{
					int nDeptNo = Department.GetDeptNo();
					if ( ( nDeptNo >= 0 ) && ( nDeptNo < info.m_strFilter.GetLength() ) )
					{
						info.m_strFilter.SetAt( nDeptNo, '1' );
						info.m_bInvalid = FALSE;
						info.m_nErrorInfo = 0;
					}
				}
			}

			DataManager.CloseDatabase();

			m_arrayDeptFilter.SetAt( nDbIdxToCheck, info );
		}
	}
}

/**********************************************************************/

bool CIDraughtOptions::GetDatabaseDepartmentFlag( int nDbIdx, int nDeptNo )
{
	int nDbIdxToCheck;
	if ( EcrmanOptions.GetGlobalDepartmentFlag() == TRUE )
		nDbIdxToCheck = 0;
	else
		nDbIdxToCheck = nDbIdx;

	bool bResult = FALSE;

	if ( ( nDbIdxToCheck >= 0 ) && ( nDbIdxToCheck < m_arrayDeptFilter.GetSize() ) )
		if ( ( nDeptNo >= 0 ) && ( nDeptNo < m_arrayDeptFilter[ nDbIdxToCheck ].m_strFilter.GetLength() ) )
			bResult = ( m_arrayDeptFilter[ nDbIdxToCheck ].m_strFilter.Mid( nDeptNo, 1 ) == "1" );

	return bResult;
}

/**********************************************************************/

void CIDraughtOptions::SetDatabaseDepartmentFlag( int nDbIdx, int nDeptNo, bool bFlag )
{
	int nDbIdxToCheck;
	if ( EcrmanOptions.GetGlobalDepartmentFlag() == TRUE )
		nDbIdxToCheck = 0;
	else
		nDbIdxToCheck = nDbIdx;

	if ( ( nDbIdxToCheck >= 0 ) && ( nDbIdxToCheck < m_arrayDeptFilter.GetSize() ) )
		if ( ( nDeptNo >= 0 ) && ( nDeptNo < m_arrayDeptFilter[ nDbIdxToCheck ].m_strFilter.GetLength() ) )
			m_arrayDeptFilter[ nDbIdxToCheck ].m_strFilter.SetAt( nDeptNo, ( bFlag ) ? '1' : '0' );
}

/**********************************************************************/

int CIDraughtOptions::GetFiddleLevel()
{

#ifndef _DEBUG

	return 0;

#else

	CString str = dbLocation.GetIDraughtID(0);
	str.MakeUpper();

	if ( str == "FIDDLE1" )
		return 1;

	if ( str == "FIDDLE2" )
		return 2;

	return 0;

#endif

}

/**********************************************************************/
