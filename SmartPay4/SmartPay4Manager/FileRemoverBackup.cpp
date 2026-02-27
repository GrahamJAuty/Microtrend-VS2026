/**********************************************************************/
#include <direct.h>
#include <ErrNo.h>
/**********************************************************************/
#include "FileRemoverBackup.h"
/**********************************************************************/

CFileRemoverBackup::CFileRemoverBackup( CBackupProgressDlg* pInfoDlg )
{
	m_pInfoBackupDlg = pInfoDlg;
}

/**********************************************************************/

void CFileRemoverBackup::UpdateDisplay( int nFileNo )
{
	m_pInfoBackupDlg -> SetText( FALSE, BACKUP_PROGRESS_DELETEFILE, nFileNo );
}

/**********************************************************************/

