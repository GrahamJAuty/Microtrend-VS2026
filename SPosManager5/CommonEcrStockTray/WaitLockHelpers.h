#pragma once
/**********************************************************************/

class CWaitLockHelpers
{
public:
	CWaitLockHelpers(void);

public:
	bool CheckRootFolder();
	void CreateTerminateRequest();
	void CreatePauseRequest();
	void CreateBlockRequest();
	void CreateResumeRequest();
	void DeletePauseRequest();
	void DeleteBlockRequest();
	bool CheckRunFile();
	bool CheckPauseRequest();
	bool CheckPauseConfirm();

private:
	const char* GetFilepath( CString& strFilename );
	void CreateTokenFile( const char* szFilename );
	void DeleteTokenFile( const char* szFilename );
	bool CheckTokenFile( const char* szFilename );

private:
	CString m_strFilepath;
	
protected:
	int m_nSyssetType;
	CString m_strTerminateRequest;
	CString m_strPauseRequest;
	CString m_strBlockRequest;
	CString m_strResumeRequest;
	CString m_strRunFile;
	CString m_strPauseConfirm;
};

/**********************************************************************/

class CWaitLockHelpersLoyalty : public CWaitLockHelpers
{
public:
	CWaitLockHelpersLoyalty();
};

/**********************************************************************/

class CWaitLockHelpersSmartPay : public CWaitLockHelpers
{
public:
	CWaitLockHelpersSmartPay();
};

/**********************************************************************/

class CWaitLockHelpersSmartBack : public CWaitLockHelpers
{
public:
	CWaitLockHelpersSmartBack();
};

/**********************************************************************/
