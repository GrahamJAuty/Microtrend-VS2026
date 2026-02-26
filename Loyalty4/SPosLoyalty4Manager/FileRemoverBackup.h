#pragma once
/**********************************************************************/
#include "..\SPosLoyalty4Shared\FileRemover.h"
/**********************************************************************/
#include "BackupProgressDlg.h"
/**********************************************************************/

class CFileRemoverBackup : public CFileRemover
{
public:
	CFileRemoverBackup( CBackupProgressDlg* pInfoDlg );

private:
	virtual void UpdateDisplay( int nFileNo );

private:
	CBackupProgressDlg* m_pInfoBackupDlg;
};

/**********************************************************************/


