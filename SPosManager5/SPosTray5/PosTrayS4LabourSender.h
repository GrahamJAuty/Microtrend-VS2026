#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#include "S4LabourLocationOptions.h"
/**********************************************************************/
#define S4LABOUR_EXPORT_ERROR_NONE 0
#define S4LABOUR_EXPORT_ERROR_FTP_CONNECT 1
#define S4LABOUR_EXPORT_ERROR_FTP_PUTFILE 2
/**********************************************************************/

class CPosTrayS4LabourSender
{
public:
	CPosTrayS4LabourSender(void);
	~CPosTrayS4LabourSender(void);

public:
	void SendExports();

private:
	void GetFileList( int nLocIdx, CS4LabourLocationOptions& S4LabourOptions, CStringArray& arrayFilenames );
	int FTPTransfer( int nLocIdx, CS4LabourLocationOptions& S4LabourOptions, CStringArray& arrayFilenames );

private:
	CString m_strSentFolder;
	CString m_strExtraErrorInfo;
};

/**********************************************************************/
#endif
/**********************************************************************/
