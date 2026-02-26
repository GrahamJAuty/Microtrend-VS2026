/**********************************************************************/
#include "PresentationOptions.h"
/**********************************************************************/
#ifndef SERVERICON
/**********************************************************************/

CPresentationOptions::CPresentationOptions()
{
	//CUSTOM MESSAGE BOX
	m_CustomMessageBox.Create			( "CustomMessageBox", TRUE );
	m_CustomMessageBoxFont.Create		( "CustomMessageBoxFont", 8, 14, 8 );
	m_CustomMessageBoxLineGap.Create	( "CustomMessageBoxLineGap", 5, 15, 5 );
	m_CustomMessageBoxColourBack.Create	( "CustomMessageBoxColourBack", 6, "FFFFFF" );
	m_CustomMessageBoxColourText.Create	( "CustomMessageBoxColourText", 6, "000000" );
	
	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CPresentationOptions::SetCustomMessageBoxFont( int n )
{
	switch(n)
	{
	case 8:
	case 10:
	case 11:
	case 12:
	case 14:
		m_CustomMessageBoxFont.SetValue(n);
		break;

	default:
		m_CustomMessageBoxFont.SetValue(10);
		break;
	}
}

/**********************************************************************/

int CPresentationOptions::MakeColourInt( CIniFileString& iniString )
{
	int nResult = 0xFFFFFF;

	CString strColour = iniString.GetValue();
	if ( strColour.GetLength() == 6 )
	{
		int n = ::HexToInt( strColour.Right(2) + strColour.Mid(2,2) + strColour.Left(2) );
		if ( ( n >= 0 ) && ( n <= 0xFFFFFF ) )
			nResult = n;
	}

	return nResult;
}

/**********************************************************************/

bool CPresentationOptions::MakeColourString( int nColour, CString& strColour )
{
	bool bResult = FALSE;
	if ( ( nColour >= 0 ) && ( nColour <= 0xFFFFFF ) )
	{
		CString strHex;
		strHex.Format( "%6.6X", nColour );
		strColour = strHex.Right(2) + strHex.Mid(2,2) + strHex.Left(2);
		bResult = TRUE;
	}
	return bResult;
}

/**********************************************************************/

void CPresentationOptions::SetCustomMessageBoxColourTextInt( int n )
{
	CString strColour;
	if ( MakeColourString( n, strColour ) == TRUE )
	{
		m_CustomMessageBoxColourText.SetValue( strColour );
		m_nCustomMessageBoxColourText = MakeColourInt( m_CustomMessageBoxColourText );
	}
}

/**********************************************************************/

void CPresentationOptions::SetCustomMessageBoxColourBackInt( int n )
{
	CString strColour;
	if ( MakeColourString( n, strColour ) == TRUE )
	{
		m_CustomMessageBoxColourBack.SetValue( strColour );
		m_nCustomMessageBoxColourBack = MakeColourInt( m_CustomMessageBoxColourBack );
	}
}

/**********************************************************************/

void CPresentationOptions::CopyFrom( CPresentationOptions& source )
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile( iniFile );
	ReloadFromIniFile( iniFile );
}

/**********************************************************************/

void CPresentationOptions::ReloadFromIniFile( CEnhancedIniFile& iniFile )
{
	//CUSTOM MESSAGE BOX
	iniFile.ReadBool( m_CustomMessageBox );
	iniFile.ReadInt( m_CustomMessageBoxFont );
	iniFile.ReadInt( m_CustomMessageBoxLineGap );
	SetCustomMessageBoxFont( GetCustomMessageBoxFont() );
	iniFile.ReadString( m_CustomMessageBoxColourBack );
	iniFile.ReadString( m_CustomMessageBoxColourText );
	MakeIntSettings();
}

/**********************************************************************/

void CPresentationOptions::PrepareIniFile( CEnhancedIniFile& iniFile )
{
	//CUSTOM MESSAGE BOX
	iniFile.WriteBool( m_CustomMessageBox );
	iniFile.WriteInt( m_CustomMessageBoxFont );
	iniFile.WriteInt( m_CustomMessageBoxLineGap );
	iniFile.WriteString( m_CustomMessageBoxColourBack );
	iniFile.WriteString( m_CustomMessageBoxColourText );
}

/**********************************************************************/

void CPresentationOptions::Reset()
{
	//CUSTOM MESSAGE BOX TAB
	m_CustomMessageBox.Reset();
	m_CustomMessageBoxFont.Reset();
	m_CustomMessageBoxLineGap.Reset();
	m_CustomMessageBoxColourBack.Reset();
	m_CustomMessageBoxColourText.Reset();
	MakeIntSettings();
}

/**********************************************************************/

void CPresentationOptions::SetDefaults()
{
	Reset();
}

/**********************************************************************/

bool CPresentationOptions::Read()
{
	return ReadInternal();
}

/**********************************************************************/

bool CPresentationOptions::ReadInternal()
{
	if ( ::FileExists( Filenames.GetPresentationOptionsFilename() ) == TRUE )
	{
		CEnhancedIniFile iniFile;
		if ( iniFile.Read( Filenames.GetPresentationOptionsFilename()) == FALSE )
		{
			m_bFatalReadError = TRUE;
			return FALSE;
		}

		ReloadFromIniFile( iniFile );
	}
	return TRUE;
}

/**********************************************************************/

bool CPresentationOptions::Write()
{
	CEnhancedIniFile iniFile;
	PrepareIniFile( iniFile );
	return iniFile.Write( Filenames.GetPresentationOptionsFilename() );
}

/**********************************************************************/

void CPresentationOptions::MakeIntSettings()
{
	m_nCustomMessageBoxColourBack = MakeColourInt( m_CustomMessageBoxColourBack );
	m_nCustomMessageBoxColourText = MakeColourInt( m_CustomMessageBoxColourText );
}

/**********************************************************************/
#endif
/**********************************************************************/
