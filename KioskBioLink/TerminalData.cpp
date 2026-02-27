/**********************************************************************/
#include "SystemData.h"
/**********************************************************************/
#include "TerminalData.h"
/**********************************************************************/

CTerminalData::CTerminalData()
{
	m_strTerminal		= "NONE";
	m_strBaud			= "9600";
	m_strPort			= "COM1";
	m_nUIDFormat		= 0;						// 0 = UID format 1(default), 1 = return UID format 2
	m_nDataFormat		= 0;						// 0=MiFare,1=Paxton,2=Innerrange (see MyMifare.h )
	m_nLocateOn			= 0;						// 0=ExtRef1, 1=ExtRef2
}

bool CTerminalData::Read()
{
	CSSIniFile file;
	if ( file.Read ( System.m_strReaderFilename ) == TRUE )
	{
		m_strTerminal	= file.GetString ( "Reader", m_strTerminal );
		m_strBaud		= file.GetString ( "Baud", m_strBaud );
		m_strPort		= file.GetString ( "Port", m_strPort );
		m_nUIDFormat	= file.GetInt	 ( "Format", m_nUIDFormat );
		m_nDataFormat	= file.GetInt	 ( "DataFormat", m_nDataFormat );
		m_nLocateOn		= file.GetInt	 ( "LocateOn", m_nLocateOn );

		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

bool CTerminalData::Write()
{
	CSSIniFile file;

	file.Set ( "Reader", m_strTerminal );
	file.Set ( "Baud", m_strBaud );
	file.Set ( "Port", m_strPort );
	file.Set ( "Format", m_nUIDFormat);
	file.Set ( "DataFormat", m_nDataFormat );
	file.Set ( "LocateOn", m_nLocateOn );

	return file.Write ( System.m_strReaderFilename );
}

//*******************************************************************

