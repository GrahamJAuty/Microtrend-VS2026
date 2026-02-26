#pragma once
//*******************************************************************
#include "AuditRecord_base.h"
//*******************************************************************

class CLoyaltyAuditFile
{
public:
	CLoyaltyAuditFile ( const char* szFilename = "" );
	~CLoyaltyAuditFile();

	int ValidateFile();
	bool OpenReadOnly();
	void Close();

	bool ReadLine ( CAuditRecord_base* pAtc );
	void JumpIn ( const char* szDate, bool bUseFiledDate = TRUE );
	
	long GetFileSize() { return m_fileIn1.GetLength(); }
	long GetFilePos() { return m_fileIn1.Tell(); }
	void SetFilePos ( long lPos ) { m_fileIn1.Seek ( lPos, SEEK_SET ); }
	int GetPercentPosition() { return m_fileIn1.GetPercentPosition(); }

#ifndef LOYSERVER
	void WriteArchive ( CWnd* pParent );
#endif

private:
	bool GetFirstDate ( CSSDate& date );

private:
	CString m_strAuditFile;
	bool m_bAuditOpen;
	CSSFile m_fileIn1;
};

//*******************************************************************
