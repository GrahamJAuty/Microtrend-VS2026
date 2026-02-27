#pragma once

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
	int m_nUIDFormat;
	int m_nDataFormat;
	int m_nLocateOn;
};

//********************************************************
