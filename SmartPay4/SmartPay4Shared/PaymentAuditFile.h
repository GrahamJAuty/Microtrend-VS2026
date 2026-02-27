#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\PaymentAuditRecord.h"
/**********************************************************************/

class CEposPaymentAuditFile
{
public:
	CEposPaymentAuditFile ( const char* szFilename = "" );				// need to set audit type when using this
	
//******************************************************************
#ifndef SYSTEMTYPE_BACKGROUND
//******************************************************************

public:
	int WriteArchive ( const char* szArchiveFile, const char* szDateStop, CWnd* pParent );

//******************************************************************
#endif
//******************************************************************

public:
	~CEposPaymentAuditFile();

	bool Open();
	void Close();
	
	bool GetFirstDate ( CSSDate& date );
	bool ReadLine ( CPaymentAuditRecord* pAtc );
	void JumpIn ( const char* szDate, bool bUseFiledDate = TRUE );

	int GetFileSize() { return m_fileIn1.GetLength(); }
	int GetFilePos() { return m_fileIn1.Tell(); }
	void SetFilePos ( int nPos ) { m_fileIn1.Seek ( (long)nPos, SEEK_SET ); }
	int GetPercentPosition() { return m_fileIn1.GetPercentPosition(); }

private:
	CString m_strAuditFile;
	CSSFile m_fileIn1;
	bool m_bAuditOpen;
};

//******************************************************************
