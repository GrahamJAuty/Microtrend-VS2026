#pragma once
//*******************************************************************

class CCsvImportData
{
public:
	CCsvImportData();
	bool Read();
	bool Save();

	const char* GetLocateOnLabel ( int nLocateOn );
	CString ConvertCustomHeader ( const char* szCsvFile );
	int GetCustomFieldCount() { return m_nCustomFieldCount; }
	int GetCustomMatchCount() { return m_nCustomMatchCount; }

	CString GetMandatoryHeaderStandard() { return m_strMandatoryHeaderStandard; }
	void SetMandatoryHeaderStandard ( CString strHeader ) 
	{ 
		if (strHeader.GetLength() <= 1000)
		{
			m_strMandatoryHeaderStandard = strHeader;
		}
	}

	int GetFilenameCacheHoursStandard() { return m_nFilenameCacheHoursStandard; }
	void SetFilenameCacheHoursStandard(int n)
	{
		if ((n >= 0) && (n <= 999))
		{
			m_nFilenameCacheHoursStandard = n;
		}
	}

	CString GetMandatoryHeaderTopUp() { return m_strMandatoryHeaderTopUp; }
	void SetMandatoryHeaderTopUp(CString strHeader)
	{
		if (strHeader.GetLength() <= 1000)
		{
			m_strMandatoryHeaderTopUp = strHeader;
		}
	}

	int GetFilenameCacheHoursTopUp() { return m_nFilenameCacheHoursTopUp; }
	void SetFilenameCacheHoursTopUp(int n)
	{
		if ((n >= 0) && (n <= 999))
		{
			m_nFilenameCacheHoursTopUp = n;
		}
	}
	
public:
	CString m_strImportFilename;
	CString m_strCustomDbFilename;
	bool m_bEnableImport;
	int m_nImportMethod;
	int m_nLocateOn;
	CString m_strLastFreeUserID;
	bool m_bDeleteFile;
	CString m_strStandardNextNewUserID;
	CString m_strCustomNextNewUserID;
	CString m_strTopupNextNewUserID;

	bool m_bEnableTopupImport;
	int m_nTopupLocateOn;
	CString m_strTopupFilename;
	bool m_bTopupAddUnknown;
	bool m_bDeleteTopupFile;
	int m_nImportTopupMethod;

	bool m_bEnableCustomImport;
	int m_nCustomLocateOn;
	CString m_strCustomMenuText;
	CString m_strCustomFilename;
	bool m_bDeleteCustomFile;
	int m_nImportCustomMethod;
	bool m_bIgnoreCustomCase;

	bool m_bEnableDbaseAuditImport;
	CString m_strDbaseAuditFilename;
	int m_nDbaseImportMethod;

private:
	CString m_strFilename;
	CString m_strMandatoryHeaderStandard;
	CString m_strMandatoryHeaderTopUp;
	int m_nFilenameCacheHoursStandard;
	int m_nFilenameCacheHoursTopUp;
	int m_nCustomFieldCount;
	int m_nCustomMatchCount;
};

//*******************************************************************
