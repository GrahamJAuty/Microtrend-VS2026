/**********************************************************************/
#include "..\SPosLoyalty4Shared\FileRemover.h"
/**********************************************************************/
#include "PCOptions.h"
/**********************************************************************/

CPCOptionsClient::CPCOptionsClient()
{
	m_DefaultReportPrinter.Create("DefaultReportPrinter", 150, "");
	m_DefaultBarcodePrinter.Create("DefaultBarcodePrinter", 150, "");

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

void CPCOptionsClient::ReloadFromIniFile(CEnhancedIniFile& iniFile)
{
	iniFile.ReadString(m_DefaultReportPrinter);
	iniFile.ReadString(m_DefaultBarcodePrinter);
}

/**********************************************************************/

void CPCOptionsClient::PrepareIniFile(CEnhancedIniFile& iniFile)
{
	iniFile.WriteString(m_DefaultReportPrinter);
	iniFile.WriteString(m_DefaultBarcodePrinter);
}

/**********************************************************************/

void CPCOptionsClient::Reset()
{
	m_DefaultReportPrinter.Reset();
	m_DefaultBarcodePrinter.Reset();
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

	if ( SolutionGlobalFunctions::FileExists( strFilename ) == TRUE )
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
/**********************************************************************/
/**********************************************************************/

CPCOptionsHost::CPCOptionsHost()
{
	m_BackupAccessType.Create("BackupAccessType", 0, 3, 0);

	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CPCOptionsHost::CopyFrom(CPCOptionsHost& source)
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile(iniFile);
	ReloadFromIniFile(iniFile);
}

/**********************************************************************/

void CPCOptionsHost::ReloadFromIniFile(CEnhancedIniFile& iniFile)
{
	iniFile.ReadInt(m_BackupAccessType);
}

/**********************************************************************/

void CPCOptionsHost::PrepareIniFile(CEnhancedIniFile& iniFile)
{
	iniFile.WriteInt(m_BackupAccessType);
}

/**********************************************************************/

void CPCOptionsHost::Reset()
{
	m_BackupAccessType.Reset();
}

/**********************************************************************/

void CPCOptionsHost::SetDefaults()
{
	Reset();
}

/**********************************************************************/

bool CPCOptionsHost::Read()
{
	CString strFilename = Filenames.GetPCOptionsHostFilename();

	if (SolutionGlobalFunctions::FileExists(strFilename) == TRUE)
	{
		CEnhancedIniFile iniFile;
		if (iniFile.Read(strFilename) == FALSE)
		{
			m_bFatalReadError = TRUE;
			return FALSE;
		}

		ReloadFromIniFile(iniFile);
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

bool CPCOptionsHost::Write()
{
	CEnhancedIniFile iniFile;
	PrepareIniFile(iniFile);
	return iniFile.Write(Filenames.GetPCOptionsHostFilename());
}

/**********************************************************************/
