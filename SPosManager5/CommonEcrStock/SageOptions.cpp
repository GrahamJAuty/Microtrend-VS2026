/**********************************************************************/
//#include "DataManager.h"
//#include "globalfunctions.h"
/**********************************************************************/
#include "SageOptions.h"
/**********************************************************************/

CSageOptions::CSageOptions()
{
	m_ExportEnable.Create( "ExportEnable", FALSE );
	m_ExportDelayTime.Create( "ExportDelayTime", 1, 6, 1 );
	m_AccountRef.Create( "AccountReference", 8, "" );
	m_CodeLength.Create( "NominalCodeLength", 4 ,8, 4 );

	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CSageOptions::CopyFrom( CSageOptions& source )
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile( iniFile );
	ReloadFromIniFile( iniFile );
}

/**********************************************************************/

void CSageOptions::ReloadFromIniFile( CEnhancedIniFile& iniFile )
{
	iniFile.ReadBool( m_ExportEnable );
	iniFile.ReadInt( m_ExportDelayTime );
	iniFile.ReadString( m_AccountRef );
	iniFile.ReadInt( m_CodeLength );
}

/**********************************************************************/

void CSageOptions::PrepareIniFile( CEnhancedIniFile& iniFile )
{
	iniFile.WriteBool( m_ExportEnable );
	iniFile.WriteInt( m_ExportDelayTime );
	iniFile.WriteString( m_AccountRef );
	iniFile.WriteInt( m_CodeLength );
}

/**********************************************************************/

void CSageOptions::Reset()
{
	m_ExportEnable.Reset();
	m_ExportDelayTime.Reset();
	m_AccountRef.Reset();
	m_CodeLength.Reset();
}

/**********************************************************************/

bool CSageOptions::Read()
{
	CFilenameUpdater FnUp( SysFiles::SageOptions );
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

bool CSageOptions::Write()
{
	bool bResult = FALSE;

	if ( DataManager.LockAllUsers() == TRUE )
	{
		::CreateSyssetSubdirectory( "SysFiles" );

		CEnhancedIniFile iniFile;
		PrepareIniFile( iniFile );
		
		CFilenameUpdater FnUp( SysFiles::SageOptions );
		bResult = iniFile.Write( FnUp.GetFilenameToUse() );

		DataManager.UnlockAllUsers();
	}
		
	return bResult;
}

/**********************************************************************/

void CSageOptions::CreateSageFolders()
{
	CreateDataSubdirectory ( "Sage" );
	CreateSyssetSubdirectory ( "Exports" );
	CreateSyssetSubdirectory ( "Exports\\Sage" );
}

/**********************************************************************/