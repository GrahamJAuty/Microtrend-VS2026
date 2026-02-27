#pragma once
/**********************************************************************/
#include "SQLAuditRecord_base.h"
/**********************************************************************/

class CSmartPayAuditFile
{
public:
	CSmartPayAuditFile( const char* szFilename = "" );				// need to set audit type when using this
	CSmartPayAuditFile( CStringArray* pArray );
	~CSmartPayAuditFile();

	bool Open();
	void Close();
	int ValidateFile();
	
	bool GetFirstDate ( CSSDate& date );
	bool ReadLine ( CSQLAuditRecord_base* pAtc );
	void JumpIn ( const char* szDate, bool bUseFiledDate = TRUE );
	
	int GetFileSize();
	int GetFilePos();
	void SetFilePos( int nPos );
	int GetPercentPosition();

#ifndef SYSTEMTYPE_BACKGROUND
#ifndef SYSTEMTYPE_SERVER

public:
	bool AppendFile(const char* szFilename);
	void WriteArchive(CWnd* pParent);
	bool AppendArray(CStringArray& array,bool bNoWebUpdateForPebble = FALSE);

private:
	void RemoveClosingBalances ( CStringArray* pArray );
	
#endif
#endif


private:
	CString m_strAuditFile;
	CSSFile m_fileIn1;
	bool m_bAuditOpen;

private:
	bool m_bArrayMode;
	CStringArray* m_pAuditArray;
	int m_nNextArrayLine;
};

/**********************************************************************/
