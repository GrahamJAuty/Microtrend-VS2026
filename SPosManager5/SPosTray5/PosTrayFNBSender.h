#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#include "FNBDatabaseOptions.h"
/**********************************************************************/
#define FNB_EXPORT_ERROR_NONE 0
#define FNB_EXPORT_ERROR_FTP_CONNECT 1
#define FNB_EXPORT_ERROR_FTP_PUTFILE 2
/**********************************************************************/

class CPosTrayFNBSender
{
public:
	CPosTrayFNBSender(void);
	~CPosTrayFNBSender(void);

public:
	void SendExports();

private:
	void GetFileList( int nDbIdx, CFNBDatabaseOptions& FNBOptions, CStringArray& arrayFilenames );
	int FTPTransfer( int nDbIdx, CFNBDatabaseOptions& FNBOptions, CStringArray& arrayFilenames );

private:
	CString m_strSentFolder;
	CString m_strExtraErrorInfo;
};

/**********************************************************************/
#endif
/**********************************************************************/
