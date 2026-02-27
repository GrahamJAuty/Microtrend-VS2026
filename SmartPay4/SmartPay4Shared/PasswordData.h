#pragma once
//$$******************************************************************
#define MAX_PASSWORD_LEN 50
#define MAX_USERNAME_LEN 50
//$$******************************************************************

class CPasswordData
{
public:
	CPasswordData();
	void Read(CString strFilename);
	bool Save();
	void ReLoad();
	bool InUse() { return (m_arrayPassword.GetAt(0) != "") ? TRUE : FALSE; }
	int GetPasswordInUse() { return m_nPasswordInUse; }
	bool GetUsingMagicPasswordFlag() { return m_bUsingMagicPassword; }

	int IsValid(CString strUserPassword);
	void SetMasterPassword(CString strText);
	void SetUserPassword(int nPasswordNo, CString strText);
	void SetUserName(int nPasswordNo, CString strUserName);
	
	CString GetPassword(int nPasswordNo);
	CString GetUserName(int nPasswordNo);
	void SetUserDefault(int nPasswordNo);

	void SaveOptions(int nPasswordNo) { m_arrayOptions.SetAt(nPasswordNo, m_csvOptions.GetLine()); }
	void CopyOptions(int nPasswordSrcNo, int nPasswordDstNo);

	bool IsSetupServerAllowed() { return m_csvOptions.GetBool(0); }
	bool IsInstallServerAllowed() { return m_csvOptions.GetBool(1); }
	bool IsAddRecordAllowed() { return m_csvOptions.GetBool(2); }
	bool IsEditRecordAllowed() { return m_csvOptions.GetBool(3); }
	bool IsDeleteRecordAllowed() { return m_csvOptions.GetBool(4); }
	bool IsHotlistRecordAllowed() { return m_csvOptions.GetBool(5); }
	bool IsEditGeneralAllowed() { return m_csvOptions.GetBool(6); }
	bool IsEditPurseAllowed() { return m_csvOptions.GetBool(7); }
	bool IsEditPointsAllowed() { return m_csvOptions.GetBool(8); }
	bool IsTopupButtonAllowed() { return m_csvOptions.GetBool(9); }
	bool IsPointsButtonAllowed() { return m_csvOptions.GetBool(10); }
	//11 Was IsRewardButtonAllowed(), no longer used
	bool IsReportsAllowed() { return m_csvOptions.GetBool(12); }
	bool IsTransferRecordAllowed() { return m_csvOptions.GetBool(13); }
	bool IsImportAllowed() { return m_csvOptions.GetBool(14); }
	bool IsExportAllowed() { return m_csvOptions.GetBool(15); }
	bool IsBatchUpdatesAllowed() { return m_csvOptions.GetBool(16); }
	bool IsSetupSystemAllowed() { return m_csvOptions.GetBool(17); }
	bool IsSetupBgndAllowed() { return m_csvOptions.GetBool(18); }
	bool IsInstallBgndAllowed() { return m_csvOptions.GetBool(19); }
	bool IsEditExternalAccountLinkAllowed() { return m_csvOptions.GetBool(20); }

	void SetSetupServerAllowed(bool bReqd) { return m_csvOptions.SetAt(0, bReqd); }
	void SetInstallServerAllowed(bool bReqd) { return m_csvOptions.SetAt(1, bReqd); }
	void SetAddRecordAllowed(bool bReqd) { return m_csvOptions.SetAt(2, bReqd); }
	void SetEditRecordAllowed(bool bReqd) { return m_csvOptions.SetAt(3, bReqd); }
	void SetDeleteRecordAllowed(bool bReqd) { return m_csvOptions.SetAt(4, bReqd); }
	void SetHotlistRecordAllowed(bool bReqd) { return m_csvOptions.SetAt(5, bReqd); }
	void SetEditGeneralAllowed(bool bReqd) { return m_csvOptions.SetAt(6, bReqd); }
	void SetEditPurseAllowed(bool bReqd) { return m_csvOptions.SetAt(7, bReqd); }
	void SetEditPointsAllowed(bool bReqd) { return m_csvOptions.SetAt(8, bReqd); }
	void SetTopupButtonAllowed(bool bReqd) { return m_csvOptions.SetAt(9, bReqd); }
	void SetPointsButtonAllowed(bool bReqd) { return m_csvOptions.SetAt(10, bReqd); }
	//11 Was IsRewardButtonAllowed(), no longer used
	void SetReportsAllowed(bool bReqd) { return m_csvOptions.SetAt(12, bReqd); }
	void SetTransferRecordAllowed(bool bReqd) { return m_csvOptions.SetAt(13, bReqd); }
	void SetImportAllowed(bool bReqd) { return m_csvOptions.SetAt(14, bReqd); }
	void SetExportAllowed(bool bReqd) { return m_csvOptions.SetAt(15, bReqd); }
	void SetBatchUpdatesAllowed(bool bReqd) { return m_csvOptions.SetAt(16, bReqd); }
	void SetSetupSystemAllowed(bool bReqd) { return m_csvOptions.SetAt(17, bReqd); }
	void SetSetupBgndAllowed(bool bReqd) { return m_csvOptions.SetAt(18, bReqd); }
	void SetInstallBgndAllowed(bool bReqd) { return m_csvOptions.SetAt(19, bReqd); }
	void SetEditExternalAccountLinkAllowed(bool bReqd) { return m_csvOptions.SetAt(20, bReqd); }

private:
	void ExtractOptions(int nPasswordNo);

private:
	CString m_strFilename;
	CString m_strMagicPassword;
	CStringArray m_arrayPassword;
	CStringArray m_arrayUserName;
	CStringArray m_arrayOptions;
	CCSV m_csvOptions;
	int m_nPasswordInUse;
	bool m_bUsingMagicPassword;
};

//$$******************************************************************
extern CPasswordData Passwords;
//$$******************************************************************
