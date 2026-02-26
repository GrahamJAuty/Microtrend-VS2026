/**********************************************************************/
#include "..\SPosLoyalty4Shared\FileRemover.h"
/**********************************************************************/
#include "PCOptions.h"
/**********************************************************************/

CPCOptionsClient::CPCOptionsClient() 
{
	m_DefaultReportPrinter.Create	( "DefaultReportPrinter", 150, "" );

	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CPCOptionsClient::CopyFrom( CPCOptionsClient& source )
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile( iniFile );
	ReloadFromIniFile( iniFile );
}

/**********************************************************************/

void CPCOptionsClient::ReloadFromIniFile( CEnhancedIniFile& iniFile )
{
	iniFile.ReadString( m_DefaultReportPrinter );
}
		
/**********************************************************************/

void CPCOptionsClient::PrepareIniFile( CEnhancedIniFile& iniFile )
{
	iniFile.WriteString( m_DefaultReportPrinter );
}

/**********************************************************************/

void CPCOptionsClient::Reset()
{
	m_DefaultReportPrinter.Reset();
}

/**********************************************************************/

void CPCOptionsClient::SetDefaults()
{
	Reset();
}

/**********************************************************************/

bool CPCOptionsClient::Read()
{
	CString strFilename = Filenames.GetPCOptionsClientFilename();
	
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

	return FALSE;
}

/**********************************************************************/

bool CPCOptionsClient::Write()
{
	CEnhancedIniFile iniFile;
	PrepareIniFile( iniFile );
	return iniFile.Write( Filenames.GetPCOptionsClientFilename() );
}

/**********************************************************************/

