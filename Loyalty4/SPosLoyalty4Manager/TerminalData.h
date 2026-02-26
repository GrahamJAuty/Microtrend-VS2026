//$$******************************************************************
#pragma once
//$$******************************************************************
#define szTERMINAL_NONE "NONE"
//$$******************************************************************

class CTerminalData
{
public:
	CTerminalData();
	bool Read();
	bool Write();

public:
	//FIDDLE int GetDataFormat1(){ return m_nDataFormat1; }
	int GetDataFormat1(){ return 0; }
	void SetDataFormat1( int n );

private:
	CString GetFilename();
	
public:
	CString m_strBaud = "9600";
	CString m_strPort = "COM1";
	CString m_strTerminal = szTERMINAL_NONE;
	int m_nUIDFormat1 = 0;

private:
	int m_nDataFormat1 = 0;
};

//$$******************************************************************
