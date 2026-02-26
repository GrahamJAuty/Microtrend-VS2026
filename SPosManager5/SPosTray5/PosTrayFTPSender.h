#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#include "PosTrayFNBSender.h"
#include "PosTrayTask.h"
/**********************************************************************/

class CPosTrayFTPSender
{
public:
	CPosTrayFTPSender(void);
	~CPosTrayFTPSender(void);

public:
	void SendExports( CPosTrayTask& Task );

private:
	void GetFileList( CPosTrayTask& Task, CStringArray& arrayFilenames );
	int FTPTransfer( CPosTrayTask& Task, CStringArray& arrayFilenames );

private:
	CString m_strSentFolder;
	CString m_strExtraErrorInfo;
};

/**********************************************************************/
#endif
/**********************************************************************/
