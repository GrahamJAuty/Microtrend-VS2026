#pragma once

//********************************************************

#define szTERMINAL_NONE					"NONE"

//********************************************************

class CTerminalData
{
public:
	CTerminalData();
	bool Read();
	bool Write();

public:
	CString m_strBaud;
	CString m_strPort;
	CString m_strTerminal;

	int m_nDefaultFormat;
	int m_nUIDFormat1;
	int m_nUIDFormat2;
	int m_nDataFormat1;
	int m_nDataFormat2;

private:
	CString m_strFilename;
};

//********************************************************
