#pragma once
//*********************************************************************

class CBackgroundRegistration
{
public:
	CBackgroundRegistration();

public:
	void CheckRegistration();
	bool CheckDbAccess();
	const char* GetErrorText1() { return m_strError1; }
	const char* GetErrorText2() { return m_strError2; }

public:
	void SetDatapath(CString str) { m_strDatapath = str; }
	bool IsRegistered() { return m_bIsRegistered; }
	bool CanGiveUp();
	int GetAttemptCount1() { return m_nAttemptCount1; }
	int GetAttemptCount2() { return m_nAttemptCount2; }
	
private:
	CString m_strDatapath;
	bool m_bIsRegistered;
	bool m_bDoneChangeDir;
	CString m_strError1;
	CString m_strError2;
	ULONGLONG m_nRetryTime;
	ULONGLONG m_nFirstTryTime;
	int m_nAttemptCount1;
	int m_nAttemptCount2;
};

//*********************************************************************
extern CBackgroundRegistration BackgroundRegistration;
//*********************************************************************

