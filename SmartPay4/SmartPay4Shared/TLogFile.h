#pragma once
//**********************************************************************
#include "TLogRecord.h"
//**********************************************************************

class CTLogFile
{
public:
	CTLogFile ( const char* szUserID, const char* szFilename = "" );
	bool Open();
	void Close();
	bool FileExists() { return ::FileExists ( m_strTLogFile ); } 
	int GetPercentPosition() { return m_fileIn1.GetPercentPosition(); }
	bool ReadLine ( CTLogRecord* record );
	void JumpIn ( const char* szDate );

	int Extract ( const char* szFilename, const char* szDateFrom, const char* szTimeFrom, const char* szDateTo = "", const char* szTimeTo = "" );

	bool Sort ( const char* szFilename );

private:
	void Reset();
	bool GetFirstDate ( CSSDate& date, const char* szFilename );
	bool ReadLine1();
	bool ReadLine2();
	bool WriteLine1();
	bool WriteLine2();

	bool Merge ( const char* szFilename );
	bool AppendFile ( const char* szFilename );
	bool AppendCheck ( const char* szFilename );
	int GetCombinedPercentPosition ( int nTotalSize );

private:
	CString m_strUserID;
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
