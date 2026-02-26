/**********************************************************************/
//#include "DataManager.h"
#include "FileRemover.h"
//#include "globalfunctions.h"
/**********************************************************************/
#include "NetworkResources.h"
/**********************************************************************/

CNetworkResources::CNetworkResources() 
{
	m_ResourceNames.Create			( "ResourceName", MAX_LENGTH_NETWORKRESOURCE_NAME, "", MAX_NETWORK_RESOURCES );
	m_UserNames.Create				( "UserName", MAX_LENGTH_NETWORKRESOURCE_USER, "", MAX_NETWORK_RESOURCES );
	m_Passwords.Create				( "Password", MAX_LENGTH_NETWORKRESOURCE_PASSWORD, "", MAX_NETWORK_RESOURCES );

	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CNetworkResources::CopyFrom( CNetworkResources& source )
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile( iniFile );
	ReloadFromIniFile( iniFile );
}

/**********************************************************************/

void CNetworkResources::ReloadFromIniFile( CEnhancedIniFile& iniFile )
{
	for ( int n = 0; n < MAX_NETWORK_RESOURCES; n++ )
	{
		iniFile.ReadString( m_ResourceNames, n );
		iniFile.ReadString( m_UserNames, n );
		iniFile.ReadString( m_Passwords, n );
	}
}

/**********************************************************************/

void CNetworkResources::PrepareIniFile( CEnhancedIniFile& iniFile )
{
	for ( int n = 0; n < MAX_NETWORK_RESOURCES; n++ )
	{
		iniFile.WriteString( m_ResourceNames, n );
		iniFile.WriteString( m_UserNames, n );
		iniFile.WriteString( m_Passwords, n );
	}
}

/**********************************************************************/

void CNetworkResources::Reset()
{
	for ( int n = 0; n < MAX_NETWORK_RESOURCES; n++ )
	{
		m_ResourceNames.Reset(n);
		m_UserNames.Reset(n);
		m_Passwords.Reset(n);
	}
}

/**********************************************************************/

void CNetworkResources::SetDefaults()
{
	Reset();
}

/**********************************************************************/

bool CNetworkResources::Read()
{
	CFilenameUpdater FnUp( SysFiles::NetworkResources );
	CString strFilename = FnUp.GetFilenameToUse();
	
	if ( ::FileExists( strFilename ) == TRUE )
	{
		CEnhancedIniFile iniFile( SS_INI_AES );
		if ( iniFile.Read( strFilename ) == FALSE )
		{
			m_bFatalReadError = TRUE;
			return FALSE;
		}

		ReloadFromIniFile( iniFile );
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

bool CNetworkResources::Write()
{
	bool bResult = FALSE;

	if ( DataManager.LockAllUsers() == TRUE )
	{
		CEnhancedIniFile iniFile( SS_INI_AES );
		PrepareIniFile( iniFile );
		
		CFilenameUpdater FnUp( SysFiles::NetworkResources );
		bResult = iniFile.Write( FnUp.GetFilenameToUse() );

		DataManager.UnlockAllUsers();
	}
		
	return bResult;
}

/**********************************************************************/

void CNetworkResources::RunBatchFile()
{
	CArray<int,int> arrayPasswords;

	for ( int n = 0; n < MAX_NETWORK_RESOURCES; n++ )
	{
		CString strResource = GetResourceName(n);
		CString strUser = GetUserName(n);
		CString strPassword = GetPassword(n);

		if ( ( strResource != "" ) && ( strUser != "" ) && ( strPassword != "" ) )
			arrayPasswords.Add(n);
	}

	if ( arrayPasswords.GetSize() == 0 )
		return;

	CFilenameUpdater FnUp( SysFiles::BatchPassword );
	CString strBatchFilename = FnUp.GetFilenameToUse();
	strBatchFilename.SetAt( strBatchFilename.GetLength() - 3, 'b' );
	
	CString strQuote = "\x022";
	
	{
		CSSFile fileBatch;
		if ( fileBatch.Open( strBatchFilename, "wb" ) == FALSE )
			return;

		fileBatch.WriteLine( "@echo off" );

		for( int n = 0; n < arrayPasswords.GetSize(); n++ )
		{
			int nIdx = arrayPasswords.GetAt(n);

			CString strLine;

			strLine.Format( "net use %s%s%s /delete >nul 2>nul",
				(const char*) strQuote,
				GetResourceName(nIdx),
				(const char*) strQuote );

			fileBatch.WriteLine( strLine );
				
			strLine.Format( "net use %s%s%s %s /user:%s >nul 2>nul",
				(const char*) strQuote,
				GetResourceName(nIdx),
				(const char*) strQuote,
				GetPassword( nIdx ),
				GetUserName(nIdx ) );

			fileBatch.WriteLine( strLine );
		}
	}

	system( strQuote + strBatchFilename + strQuote );
	CFileRemover FileRemover( strBatchFilename );
}

/**********************************************************************/
