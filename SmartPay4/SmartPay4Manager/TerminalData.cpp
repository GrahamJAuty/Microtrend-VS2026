/**********************************************************************/
#include "TerminalData.h"
/**********************************************************************/

CTerminalData::CTerminalData()
{
	m_strTerminal = szTERMINAL_NONE;
	m_strBaud = "9600";
	m_strPort = "COM1";
	m_nDefaultFormat = 0;
	m_nUIDFormat1 = 0;						// 0 = UID format 1(default), 1 = return UID format 2
	m_nUIDFormat2 = 0;						// 0 = UID format 1(default), 1 = return UID format 2
	m_nDataFormat1 = 0;						// 0=Disable,1=MiFare,2-Paxton,3=Innerrange (see MyMifare.h )
	m_nDataFormat2 = 0;						// 0=Disable,1=MiFare,2-Paxton,3=Innerrange
}

/**********************************************************************/

bool CTerminalData::Read()
{
	m_strFilename.Format("%s\\reader.dat", 
		(const char*) Sysset.GetPCPathClient());	// NOTE: can't do this in constructor

	CSSIniFile file;
	if (file.Read(m_strFilename) == TRUE)
	{
		m_strTerminal = file.GetString("Reader", m_strTerminal);
		m_strBaud = file.GetString("Baud", m_strBaud);
		m_strPort = file.GetString("Port", m_strPort);
		m_nDefaultFormat = file.GetInt("DefaultFormat", m_nDefaultFormat);
		m_nUIDFormat1 = file.GetInt("Format1", m_nUIDFormat1);
		m_nUIDFormat2 = file.GetInt("Format2", m_nUIDFormat2);
		m_nDataFormat1 = file.GetInt("DataFormat1", m_nDataFormat1);
		m_nDataFormat2 = file.GetInt("DataFormat2", m_nDataFormat2);

		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

bool CTerminalData::Write()
{
	CSSIniFile file;

	file.Set("Reader", m_strTerminal);
	file.Set("Baud", m_strBaud);
	file.Set("Port", m_strPort);
	file.Set("DefaultFormat", m_nDefaultFormat);
	file.Set("Format1", m_nUIDFormat1);
	file.Set("Format2", m_nUIDFormat2);
	file.Set("DataFormat1", m_nDataFormat1);
	file.Set("DataFormat2", m_nDataFormat2);

	return file.Write(m_strFilename);
}

//*******************************************************************

