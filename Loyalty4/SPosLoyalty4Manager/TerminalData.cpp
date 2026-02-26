//$$******************************************************************
#include "MyMiFare.h"
//$$******************************************************************
#include "TerminalData.h"
//$$******************************************************************

CTerminalData::CTerminalData()
{
}

//$$******************************************************************

CString CTerminalData::GetFilename()
{
	CString strFilename = "";
	
	strFilename.Format("%s\\reader.dat",
		(const char*)Sysset.GetPCPathClient());
	
	return strFilename;
}

//$$******************************************************************

bool CTerminalData::Read()
{
	CSSIniFile file;
	if (file.Read(GetFilename()) == TRUE)
	{
		m_strTerminal = file.GetString("Reader", m_strTerminal);
		m_strBaud = file.GetString("Baud", m_strBaud);
		m_strPort = file.GetString("Port", m_strPort);
		m_nUIDFormat1 = file.GetInt("Format", m_nUIDFormat1);
		SetDataFormat1(file.GetInt("DataFormat", m_nDataFormat1));
		return TRUE;
	}
	return FALSE;
}

//$$******************************************************************

bool CTerminalData::Write()
{
	CSSIniFile file;

	file.Set("Reader", m_strTerminal);
	file.Set("Baud", m_strBaud);
	file.Set("Port", m_strPort);
	file.Set("Format", m_nUIDFormat1);
	file.Set("DataFormat", m_nDataFormat1);

	return file.Write(GetFilename());
}

//$$******************************************************************

void CTerminalData::SetDataFormat1(int n)
{
	switch (n)
	{
	case nMIFARE_DATAFORMAT_STANDARD:
	case nMIFARE_DATAFORMAT_PAXTON:
	case nMIFARE_DATAFORMAT_INNERRANGE:
		m_nDataFormat1 = n;
		break;
	}
}

//$$******************************************************************
