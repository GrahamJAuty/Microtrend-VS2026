//$$******************************************************************
#include "..\SmartPay4ManagerEmail\DefTextEmailAddress.h"
//$$******************************************************************
#include "..\SmartPay4Shared\EmailOptions.h"
#include "..\SmartPay4Shared\FileRemover.h"
#include "..\SmartPay4Shared\SystemData.h"
//$$******************************************************************
#include "EmailLogger.h"
#include "Framework.h"
#include "MyCSSLockFile.h"
#include "SmartPayEmailSender.h"
//$$******************************************************************
#include "SmartPayEmailBatchSend.h"
//$$******************************************************************
extern const char* szVERSION_SMARTPAY_ABOUT;
extern const char* szUPDATE_TOKEN_DATE;
//$$******************************************************************

BEGIN_MESSAGE_MAP(CSmartPayEmailBatchSendApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

//$$******************************************************************

class CMyCommandLineInfo : public CCommandLineInfo
{
public:
	CMyCommandLineInfo();

public:
	virtual void ParseParam(const char* szParam, BOOL bFlag, BOOL bLast);
	int GetSize() { return m_arrayParam.GetSize(); }
	const char* GetAt(int n) { return m_arrayParam.GetAt(n); }

public:
	bool GetEmailModeFlag() { return m_bEmailMode; }
	bool IsSpawned();

private:
	CStringArray m_arrayParam;
	bool m_bEmailMode;
};

//$$******************************************************************

CMyCommandLineInfo::CMyCommandLineInfo() : CCommandLineInfo()
{
	m_bEmailMode = FALSE;
}

//$$******************************************************************

void CMyCommandLineInfo::ParseParam(const char* szParam, BOOL bFlag, BOOL bLast)
{
	if (bFlag == TRUE)					// see if a flag ( strips of  '/' character )
	{
		CString strParam = szParam;

		if (strParam == "EMAIL")
		{
			m_bEmailMode = TRUE;
		}
	}
	else
	{
		m_arrayParam.Add(szParam);
	}

	CCommandLineInfo::ParseParam(szParam, bFlag, bLast);
}

//$$******************************************************************

bool CMyCommandLineInfo::IsSpawned()
{
	return m_bEmailMode;
}

//$$******************************************************************

CSmartPayEmailBatchSendApp::CSmartPayEmailBatchSendApp()
{
}

//$$******************************************************************
CSmartPayEmailBatchSendApp theApp;
//$$******************************************************************
CDefTextEmailAddress EmailAddresses;
CEmailLogger EmailLogger;
CEmailOptions EmailOptions;
CFilenameHandler Filenames;
CSysset Sysset;
CSystemData System;
//$$******************************************************************

BOOL CSmartPayEmailBatchSendApp::InitInstance()
{
	CWinApp::InitInstance();
	CKUnlock();

	CMyCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (cmdInfo.IsSpawned() == TRUE)
	{
		bool bCreatePCId = TRUE;
		int nDummyDatapathIndex = 0;
		if (Sysset.TestRegistrationModule(SYSSET_SMARTPAY4, NULL, bCreatePCId, TRUE, nDummyDatapathIndex) == TRUE)
		{
			{
				CString strPathHost = "";
				GetPCPathHost(strPathHost);

				CString strPathClient = "";
				GetPCPathClient(strPathClient);

				CreateSubdirectory(strPathHost);

				if ( strPathHost != strPathClient)
				{
					CreateSubdirectory(strPathClient);
				}
			}

			System.Read();

			CMyCSSLockFile fileLock;

			if (fileLock.OpenLock(Filenames.GetEmailLockFilename()) == TRUE)
			{
				CSmartPayEmailSender EmailSender;
				EmailSender.LookForReports();

				int nCount = EmailSender.GetPendingEmailCount();

				if (nCount > 0)
				{
					CSSFile fileCount;
					if (fileCount.Open(Filenames.GetEmailPendingCountFilename(), "wb") == TRUE)
					{
						CString str;
						str.Format("%d", nCount);
						fileCount.WriteLine(str);
					}
				}
				else
				{
					CFileRemover FileRemover(Filenames.GetEmailPendingCountFilename());
				}
			}
		}
	}
	
	return FALSE;
}

//$$******************************************************************


