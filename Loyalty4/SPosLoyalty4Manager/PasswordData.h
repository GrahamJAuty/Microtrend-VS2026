#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#define MAX_PASSWORD_LEN 50
#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_CARDNO_LEN 30
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

	int ValidatePassword(CString strPassword, CString strCardNo);

	void SetMasterPassword(CString strText);
	void SetUserPassword(int nPasswordNo, CString strPassword);
	void SetUserName(int nPasswordNo, CString strUserName);
	
	CString GetPassword(int nPasswordNo);
	CString GetUserName(int nPasswordNo);
	void SetUserDefault(int nPasswordNo);

	void SetMasterCardNo(CString strCardNo);
	void SetCardNo(int nPasswordNo, CString strCardNo);
	CString GetCardNo(int nPasswordNo);

	void SaveOptions(int nPasswordNo) { m_arrayOptions.SetAt(nPasswordNo, m_csvOptions.GetLine()); }

	void CopyOptions(int nSrcPasswordNo, int nDstPasswordNo);

	bool IsServerSetupAllowed() { return m_csvOptions.GetBool(0); }
	bool IsServerInstallAllowed() { return m_csvOptions.GetBool(1); }
	bool IsAddRecordAllowed() { return m_csvOptions.GetBool(2); }
	bool IsEditRecordAllowed() { return m_csvOptions.GetBool(3); }
	bool IsDeleteRecordAllowed() { return m_csvOptions.GetBool(4); }
	bool IsHotlistRecordAllowed() { return m_csvOptions.GetBool(5); }
	bool IsEditGeneralAllowed() { return m_csvOptions.GetBool(6); }
	bool IsEditPurseAllowed() { return m_csvOptions.GetBool(7); }
	bool IsEditPointsAllowed() { return m_csvOptions.GetBool(8); }
	bool IsPurse1TopupButtonAllowed() { return m_csvOptions.GetBool(9); }
	bool IsPointsTopupButtonAllowed() { return m_csvOptions.GetBool(10); }
	bool IsManualBonusButtonAllowed() { return m_csvOptions.GetBool(11); }
	bool IsReportsAllowed() { return m_csvOptions.GetBool(12); }
	bool IsTransferRecordAllowed() { return m_csvOptions.GetBool(13); }
	bool IsImportRecordsAllowed() { return m_csvOptions.GetBool(14); }
	bool IsExportRecordsAllowed() { return m_csvOptions.GetBool(15); }
	bool IsBatchUpdatesAllowed() { return m_csvOptions.GetBool(16); }
	bool IsSystemSetupAllowed() { return m_csvOptions.GetBool(17); }
	bool IsPointsRedeemButtonAllowed() { return m_csvOptions.GetBool(18); }
	bool IsEditCashSpendAllowed() { return m_csvOptions.GetBool(19); }
	bool IsEditExternalAccountLinkAllowed() { return m_csvOptions.GetBool(20); }
	bool IsPurse1SpendButtonAllowed() { return m_csvOptions.GetBool(21); }

	void SetServerSetupAllowed(bool bReqd) { m_csvOptions.SetAt(0, bReqd); }
	void SetServerInstallAllowed(bool bReqd) { m_csvOptions.SetAt(1, bReqd); }
	void SetAddRecordAllowed(bool bReqd) { m_csvOptions.SetAt(2, bReqd); }
	void SetEditRecordAllowed(bool bReqd) { m_csvOptions.SetAt(3, bReqd); }
	void SetDeleteRecordAllowed(bool bReqd) { m_csvOptions.SetAt(4, bReqd); }
	void SetHotlistRecordAllowed(bool bReqd) { m_csvOptions.SetAt(5, bReqd); }
	void SetEditGeneralAllowed(bool bReqd) { m_csvOptions.SetAt(6, bReqd); }
	void SetEditPurseAllowed(bool bReqd) { m_csvOptions.SetAt(7, bReqd); }
	void SetEditPointsAllowed(bool bReqd) { m_csvOptions.SetAt(8, bReqd); }
	void SetPurse1TopupButtonAllowed(bool bReqd) { m_csvOptions.SetAt(9, bReqd); }
	void SetPointsTopupButtonAllowed(bool bReqd) { m_csvOptions.SetAt(10, bReqd); }
	void SetManualBonusButtonAllowed(bool bReqd) { m_csvOptions.SetAt(11, bReqd); }
	void SetReportsAllowed(bool bReqd) { m_csvOptions.SetAt(12, bReqd); }
	void SetTransferRecordAllowed(bool bReqd) { m_csvOptions.SetAt(13, bReqd); }
	void SetImportRecordsAllowed(bool bReqd) { m_csvOptions.SetAt(14, bReqd); }
	void SetExportRecordsAllowed(bool bReqd) { m_csvOptions.SetAt(15, bReqd); }
	void SetBatchUpdatesAllowed(bool bReqd) { m_csvOptions.SetAt(16, bReqd); }
	void SetSystemSetupAllowed(bool bReqd) { m_csvOptions.SetAt(17, bReqd); }
	void SetPointsRedeemButtonAllowed(bool bReqd) { m_csvOptions.SetAt(18, bReqd); }
	void SetEditCashSpendAllowed(bool bReqd) { m_csvOptions.SetAt(19, bReqd); }
	void SetEditExternalAccountLinkAllowed(bool bReqd) { m_csvOptions.SetAt(20, bReqd); }
	void SetPurse1SpendButtonAllowed(bool bReqd) { m_csvOptions.SetAt(21, bReqd); }

private:
	void ExtractOptions(int nPasswordNo);
	int IsValidPassword(CString strPassword);
	int IsValidCardNo(CString strCardNo);

private:
	CString m_strFilename;
	CString m_strMagicPassword;
	CStringArray m_arrayPassword;
	CStringArray m_arrayUserName;
	CStringArray m_arrayCardNo;
	CStringArray m_arrayOptions;
	CCSV m_csvOptions;
	int m_nPasswordInUse;
};

//$$******************************************************************
extern CPasswordData Passwords;
//$$******************************************************************
