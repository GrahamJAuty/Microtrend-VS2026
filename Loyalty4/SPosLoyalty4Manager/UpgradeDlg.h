#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "WorkingDlg.h"
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
	void ConvertV3();
	void DisplayResult();

	bool CheckV1Server ( const char* szSystemFolder, CWorkingDlg&  WorkingDlg );
	bool CheckV2Server(const char* szSystemFolder, CWorkingDlg& WorkingDlg);
	bool IsV1orV2ServerRunning ( const char* szSystemFolder );
	bool TerminateV1orV2Server ( const char* szSystemFolder );

private:
	bool CopyFolderRecursive(CString strSourceFolder, CString strDestFolder, bool bSubFolders, bool bExcludeRootFiles, CWorkingDlg* pWorking, int& nFileCount, CString strMask = "*.*");
	bool SafelyCreateFolder(const char* szFolder);
	bool CopyFileForUpgrade(const char* szSource, const char* szDest);

private:
	void ImportAccountDatabase();
	void ImportGroupDatabase();
	void ImportPluDatabase();

private:
	void AppendRemoveStartupLinkMessage(int nResult, CString strLoyaltyType, CString& strMsg);

private:
	CString m_strSourcePath;
	CString m_strError;
	CString m_strOldVersion;
	bool m_bAbort;
	int m_nV1or2ServerTerminated;
	int m_nV1StartupLinkRemoved;
	int m_nV2OldStartupLinkRemoved;
	int m_nV2NewStartupLinkRemoved;
};

//$$******************************************************************
