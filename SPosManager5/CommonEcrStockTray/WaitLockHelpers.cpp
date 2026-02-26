/**********************************************************************/
#include "FileRemover.h"
/**********************************************************************/
#include "WaitLockHelpers.h"
/**********************************************************************/

CWaitLockHelpers::CWaitLockHelpers(void)
{
	m_strTerminateRequest = "";
	m_strPauseRequest = "";
	m_strBlockRequest = "";
	m_strResumeRequest = "";
	m_strRunFile = "";
	m_strPauseConfirm = "";
}

/**********************************************************************/
void CWaitLockHelpers::CreateTerminateRequest()	{ CreateTokenFile( m_strTerminateRequest ); }
void CWaitLockHelpers::CreatePauseRequest()		{ CreateTokenFile( m_strPauseRequest ); }
void CWaitLockHelpers::CreateBlockRequest()		{ CreateTokenFile( m_strBlockRequest ); }
void CWaitLockHelpers::CreateResumeRequest()	{ CreateTokenFile( m_strResumeRequest ); }
void CWaitLockHelpers::DeletePauseRequest()		{ DeleteTokenFile( m_strPauseRequest ); }
void CWaitLockHelpers::DeleteBlockRequest()		{ DeleteTokenFile( m_strBlockRequest ); }
bool CWaitLockHelpers::CheckRunFile()			{ return CheckTokenFile( m_strRunFile ); }
bool CWaitLockHelpers::CheckPauseRequest()		{ return CheckTokenFile( m_strPauseRequest ); }
bool CWaitLockHelpers::CheckPauseConfirm()		{ return CheckTokenFile( m_strPauseConfirm ); }
/**********************************************************************/

bool CWaitLockHelpers::CheckRootFolder()
{
	CString strFolder;
	strFolder.Format ( "%sP%2.2d\\SysFiles",
		Sysset.GetDataPath(),
		m_nSyssetType );
	return ( ExistSubdirectory( strFolder ) );
}

/**********************************************************************/

const char* CWaitLockHelpers::GetFilepath( CString& strFilename )
{
	m_strFilepath.Format ( "%sP%2.2d\\SysFiles\\",
		Sysset.GetDataPath(),
		m_nSyssetType );

	m_strFilepath += strFilename;

	return m_strFilepath;
}

/**********************************************************************/

void CWaitLockHelpers::CreateTokenFile( const char* szFilename )
{
	CString strFilename = szFilename;

	if ( strFilename != "" )
	{
		CSSFile file;
		file.Open( GetFilepath( strFilename ), "wb" );
		file.Close();
	}
}

/**********************************************************************/

void CWaitLockHelpers::DeleteTokenFile( const char* szFilename )
{
	CString strFilename = szFilename;

	if ( strFilename != "" )
		CFileRemover FileRemover( GetFilepath( strFilename ) );
}

/**********************************************************************/

bool CWaitLockHelpers::CheckTokenFile( const char* szFilename )
{
	CString strFilename = szFilename;

	if ( strFilename != "" )
		return ( ::FileExists( GetFilepath( strFilename ) ) );
	else
		return FALSE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CWaitLockHelpersLoyalty::CWaitLockHelpersLoyalty()
{
	m_nSyssetType = SYSSET_LOYALTY2;
	m_strTerminateRequest = "";
	m_strPauseRequest = "req.001";
	m_strBlockRequest = "loy.no";
	m_strResumeRequest = "req.002";
	m_strRunFile = "loy.run";
	m_strPauseConfirm = "req.010";
}

/**********************************************************************/

CWaitLockHelpersSmartPay::CWaitLockHelpersSmartPay()
{
	m_nSyssetType = SYSSET_SMARTPAY;
	m_strTerminateRequest = "stop.req";
	m_strPauseRequest = "req.001";
	m_strBlockRequest = "loy.no";
	m_strResumeRequest = "req.002";
	m_strRunFile = "loy.run";
	m_strPauseConfirm = "req.010";
}

/**********************************************************************/

CWaitLockHelpersSmartBack::CWaitLockHelpersSmartBack()
{
	m_nSyssetType = SYSSET_SMARTPAY;
	m_strTerminateRequest = "pfile.xxx";
	m_strPauseRequest = "pfile.xx1";
	m_strBlockRequest = "";
	m_strResumeRequest = "pfile.xx2";
	m_strRunFile = "pfile.run";
	m_strPauseConfirm = "";
}

/**********************************************************************/
