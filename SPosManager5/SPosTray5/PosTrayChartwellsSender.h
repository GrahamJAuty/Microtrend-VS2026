#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#include <CkSFtp.h>
/**********************************************************************/
#include "ChartwellsOptions.h"
/**********************************************************************/
#define CHARTWELLS_EXPORT_ERROR_NONE 0
#define CHARTWELLS_EXPORT_ERROR_FTP_CONNECT 1
#define CHARTWELLS_EXPORT_ERROR_FTP_AUTHENTICATE 2
#define CHARTWELLS_EXPORT_ERROR_FTP_INITIALISE 3
#define CHARTWELLS_EXPORT_ERROR_FTP_OPENFILE 4
#define CHARTWELLS_EXPORT_ERROR_FTP_UPLOADFILE 4
/**********************************************************************/

class CPosTrayChartwellsSender
{
public:
	CPosTrayChartwellsSender(void);
	~CPosTrayChartwellsSender(void);

public:
	void SendExports();
	void SendFiles(CkSFtp& SFTPConnect, CStringArray& arrayFilenames, int& nResult);

private:
	void GetFileList( CChartwellsOptions& ChartwellsOptions, CStringArray& arrayFilenames );
	int FTPTransfer( CChartwellsOptions& ChartwellsOptions, CStringArray& arrayFilenames );
	int UploadSiteList(CChartwellsOptions& ChartwellsOptions);
	void LogMessage(CString strMessage, CString strStatus,bool bLeadingLineNetwork = FALSE);

private:
	CString m_strSentFolder;
	CString m_strExtraErrorInfo;
};

/**********************************************************************/
#endif
/**********************************************************************/
