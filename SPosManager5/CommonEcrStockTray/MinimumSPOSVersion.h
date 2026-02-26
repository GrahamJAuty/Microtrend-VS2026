#pragma once
/**********************************************************************/

struct CMinimumSPOSVersion
{
public:
	CMinimumSPOSVersion();
	void Merge(int nMinVer,CString strReason);

public:
	void CheckSPOSVersion( int nSPOSVersion, int& nErrorType );
	const char* GetMinVerName();

public:
	CString m_strReason;
	int m_nMinVer;
	bool m_bInvalidData;

private:
	CString m_strMinVerName;
};

/**********************************************************************/
