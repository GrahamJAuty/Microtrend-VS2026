#pragma once
/**********************************************************************/
#include "CashRSPImporterBatchInfo.h"
#include "CashRSPImporterTransaction.h"
/**********************************************************************/

struct CCashRSPImporterOutputFile
{
public:
	CCashRSPImporterOutputFile( int nFileType, CCashRSPImporterBatchInfo& BatchInfo, CCashRSPImporterTransaction& TransactionInfo );
	
public:
	bool GetLastOpenNewFileFlag(){ return m_bLastOpenNewFile; }
	const char* GetCurrentFilename(){ return m_strCurrentFile; }

public:
	bool OpenFile();
	void WriteLine( const char* szLine );
	void CloseFile();

public:
	bool IsOpen(){ return m_bIsOpen; }

private:
	void GetDailyTermFilename();

private:
	int m_nFileType;
	CCashRSPImporterBatchInfo& m_BatchInfo;
	CCashRSPImporterTransaction& m_TransactionInfo;

private:
	CSSFile m_File;
	bool m_bIsOpen;
	bool m_bLastOpenNewFile;
	CString m_strFilename;
	CString m_strCurrentFile;
};

/**********************************************************************/

