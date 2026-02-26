/**********************************************************************/
#include "UnitechOptions.h"
/**********************************************************************/

CUnitechOptions::CUnitechOptions()
{
	m_Enable.Create( "UnitechEnableFlag", FALSE );
	m_ProgramFolder.Create( "UnitechProgramFolder", UNITECH_MAX_LENGTH_FOLDER, _T("C:\\ptcomm") );
	m_ProgramFile.Create( "UnitechProgramFile", UNITECH_MAX_LENGTH_FILENAME, _T("ptcomm.exe") );
	m_StatusType.Create( "UnitechStatusType", 0, 2, UNITECH_STATUS_TYPE_NONE );
	m_StatusFolder.Create( "UnitechStatusFolder", UNITECH_MAX_LENGTH_FOLDER, _T("C:") );
	m_StatusFile.Create ( "UnitechStatusFile", UNITECH_MAX_LENGTH_FILENAME, _T("$jgp.sts") );
	m_ComPortNo.Create( "UnitechComPortNo", 1, UNITECH_MAX_COMM_PORT, 6 );
	m_FastSpeed.Create ( "UnitechFastSpeedFlag", TRUE );
	m_NetUseFlag.Create ( "UnitechNetUseFlag", 0, 20, 0 );

	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CUnitechOptions::CopyFrom( CUnitechOptions& source )
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile( iniFile );
	ReloadFromIniFile( iniFile );
}

/**********************************************************************/

void CUnitechOptions::ReloadFromIniFile( CEnhancedIniFile& iniFile )
{
	iniFile.ReadBool( m_Enable );
	iniFile.ReadString( m_ProgramFolder );
	iniFile.ReadString( m_ProgramFile );
	iniFile.ReadInt( m_StatusType );
	iniFile.ReadString( m_StatusFolder );
	iniFile.ReadString( m_StatusFile );
	iniFile.ReadInt( m_ComPortNo );
	iniFile.ReadBool( m_FastSpeed );
	iniFile.ReadInt( m_NetUseFlag );
}

/**********************************************************************/

void CUnitechOptions::PrepareIniFile( CEnhancedIniFile& iniFile )
{
	iniFile.WriteBool( m_Enable );
	iniFile.WriteString( m_ProgramFolder );
	iniFile.WriteString( m_ProgramFile );
	iniFile.WriteInt( m_StatusType );
	iniFile.WriteString( m_StatusFolder );
	iniFile.WriteString( m_StatusFile );
	iniFile.WriteInt( m_ComPortNo );
	iniFile.WriteBool( m_FastSpeed );
	iniFile.WriteInt( m_NetUseFlag );
}

/**********************************************************************/

void CUnitechOptions::Reset()
{
	m_Enable.Reset();
	m_ProgramFolder.Reset();
	m_ProgramFile.Reset();
	m_StatusType.Reset();
	m_StatusFolder.Reset();
	m_StatusFile.Reset();
	m_ComPortNo.Reset();
	m_FastSpeed.Reset();
	m_NetUseFlag.Reset();
}

/**********************************************************************/

bool CUnitechOptions::Read()
{
	CFilenameUpdater FnUp( SysFiles::UnitechOptions );
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

bool CUnitechOptions::Write()
{
	bool bResult = FALSE;

	if ( DataManager.LockAllUsers() == TRUE )
	{
		CEnhancedIniFile iniFile;
		PrepareIniFile( iniFile );
		
		CFilenameUpdater FnUp( SysFiles::UnitechOptions );
		bResult = iniFile.Write( FnUp.GetFilenameToUse() );

		DataManager.UnlockAllUsers();
	}
		
	return bResult;
}

/**********************************************************************/

const char* CUnitechOptions::GetFullProgramPath()
{
	m_strFullProgramPath = m_ProgramFolder.GetValue();

	if ( m_strFullProgramPath != "" )
		if ( m_strFullProgramPath.Right(1) != "\\" )
			m_strFullProgramPath += "\\";

	m_strFullProgramPath += m_ProgramFile.GetValue();
	
	return m_strFullProgramPath;
}

/**********************************************************************/

const char* CUnitechOptions::GetFullStatusPath()
{
	m_strFullStatusPath = m_StatusFolder.GetValue();

	if ( m_strFullStatusPath != "" )
		if ( m_strFullStatusPath.Right(1) != "\\" )
			m_strFullStatusPath += "\\";

	m_strFullStatusPath += m_StatusFile.GetValue();
	
	return m_strFullStatusPath;
}

/**********************************************************************/
