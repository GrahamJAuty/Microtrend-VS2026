#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\WaitDlg.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//$$******************************************************************

class CUpgradeDlg : public CDialog
{
public:
	CUpgradeDlg( const char* szSourcePath, CWnd* pParent = NULL);   
	
	enum { IDD = IDD_UPGRADE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	afx_msg void OnButtonStart();
	afx_msg void OnButtonBrowse();
	DECLARE_MESSAGE_MAP()

private:
	int GetPreviousSystemType();
	void ConvertV1();
	void ConvertV2();
	void DisplayResult();

private:
	bool CheckV1Server ( const char* szSystemFolder, CWaitDlg* pDlgWait );
	bool IsV1ServerRunning(const char* szSystemFolder);
	bool TerminateV1Server(const char* szSystemFolder);

private:
	bool CheckV1Bgnd(const char* szSystemFolder, CWaitDlg* pDlgWait);
	bool IsV1BgndRunning(const char* szSystemFolder);
	bool TerminateV1Bgnd(const char* szSystemFolder);

private:
	bool CopyFolderRecursive(const char* szSourceFolder, const char* szDestFolder, bool bSubFolders, bool bExcludeRootFiles, CWorkingDlg* pWorking, int& nFileCount);
	bool SafelyCreateFolder(const char* szFolder);
	bool CopyFileForUpgrade(const char* szSource, const char* szDest);

private:
	void AppendRemoveStartupLinkMessage(int nResult, CString strLoyaltyType, CString& strMsg);

private:
	void ImportAccountDatabase();
	void ImportLeaverDatabase();
	void ImportGroupDatabase();
	void ImportPluDatabase();
	void ImportAuditPeriods();
	void ImportUserTexts();
	void ImportBioRegister();
	void ImportPeriodRefresh();
	void ImportClosingBalance();
	void ImportEposTerminals();
	void ImportCCNo();
	void ImportWebPaymentOptions();

private:
	CString m_strSourcePath;
	CString m_strError;
	bool m_bAbort;
	int m_nV1ServerTerminated;
	int m_nV1OldServerStartupLinkRemoved;
	int m_nV1NewServerStartupLinkRemoved;
	int m_nV1BgndTerminated;
	int m_nV1BgndStartupLinkRemoved;
};

//$$******************************************************************
