#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
//#include "globalfunctions.h"
#include "TLogRecord.h"
//**********************************************************************

class CTLogFile
{
public:
	CTLogFile ( const char* szCardNo );
	bool Open();
	void Close();
	int GetPercentPosition() { return m_fileIn1.GetPercentPosition(); }
	bool ReadLine ( CTLogRecord* record );
	void JumpIn ( const char* szDate );

private:
	void Reset();
	bool Sort ( const char* szFilename );
	bool Merge ( const char* szFilename );

	bool GetFirstDate ( CSSDate& date, const char* szFilename );
	bool ReadLine1();
	bool ReadLine2();
	bool WriteLine1();
	bool WriteLine2();
	bool AppendFile ( const char* szFilename );
	bool AppendCheck ( const char* szFilename );

private:
	CString m_strTLogFile;
	CSSFile m_fileIn1;
	CSSFile m_fileIn2;
	CSSFile m_fileOut;
	bool m_bEOF1;
	bool m_bEOF2;
	CString m_strTLogLine1;
	CString m_strTLogLastLine1;
	CString m_strTLogLine2;
	CString m_strTitle;
};
