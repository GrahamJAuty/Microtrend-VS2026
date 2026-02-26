#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#define UNITECH_BATCH_ERROR_NONE 0
#define UNITECH_BATCH_ERROR_PROGRAM 1
#define UNITECH_BATCH_ERROR_JOBFILE 2
#define UNITECH_BATCH_ERROR_CREATEFOLDER 3
#define UNITECH_BATCH_ERROR_LOCK 4
#define UNITECH_BATCH_ERROR_SPAWN 5
/**********************************************************************/
#include "UnitechBatchJobResultsDlg.h"
/**********************************************************************/

struct CUnitechFileInfo
{
public:
	CString m_strTimeStamp;
	int m_nFileType;
};

/**********************************************************************/

class CUnitechBatchJob
{
public:
	CUnitechBatchJob( int nDbIdx, int nSpIdx );

public:
	int RunBatch( bool bUploadPlu, bool bDownloadPlu, bool bUploadStock, CUnitechBatchJobResults& results, CWnd* pOwner );
	bool GetStatusMessage( CString& strMessage );
	
private:
	int RunBatchInternal( bool bUploadPlu, bool bDownloadPlu, bool bUploadStock, CWnd* pOwner );
	void FilterStockUploads( CUnitechBatchJobResults& results );
	void FilterPluUploads();

private:
	void WriteHeader( CSSFile& file, bool bDownload, const char* szComment );
	void WriteAppend( CSSFile& file, const char* szFilename );
	void WriteFetchPlu( CSSFile& file, const char* szFilename );
	void WriteClear( CSSFile& file, const char* szFilename );

private:
	const char* GetNetUseFolderPath( const char* szSource );
	void CreateNetUseBatchFiles();
	void NetUseOn();
	void NetUseOff();

private:
	LONG GetDWORDRegKey(HKEY hKey, LPCWSTR szValueName, DWORD &nValue, DWORD nDefaultValue);

private:
	bool GetStatusMessageRegistry( CString& strMessage );
	bool GetStatusMessageFile( CString& strMessage );
	
private:
	int m_nDbIdx;
	int m_nSpIdx;

private:
	int m_nNetUseDrive;
	CString m_strNetUseRootPath;
	CString m_strNetUseFolderPath;

private:
	CString m_strTimeStamp;
	bool m_bWrittenHeader;
};

/**********************************************************************/
#endif
/**********************************************************************/
