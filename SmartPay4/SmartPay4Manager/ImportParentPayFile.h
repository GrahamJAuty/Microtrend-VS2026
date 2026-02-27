#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
/**********************************************************************/
#include "..\SmartPay4ManagerBgnd\ParentPayData.h"
/**********************************************************************/

class CImportParentPayFile
{
public:
	CImportParentPayFile ( CParentPayData* pData, CWnd* pParent );
	~CImportParentPayFile();

	bool ValidateFile();
	bool Import();
	bool ShowImports();

	CString GetError() { return m_strError; }
	void ShowStatistics();

private:
	void GetNewCardNo ( CString& strCardNo );
	bool MatchRecordUPN ( const char* szCardNo );
	bool LocateRecord ( CString& strCardNo );

	int ProcessFile ( bool bImportReqd );
	void SetRecord(CSQLRowAccountFull& RowAccount);

	void SaveDinerMatchAdvice(CSQLRowAccountFull& RowAccount);

	void ImportLine ( const char* szCardNo );
	bool ShowImportLine ( const char* szCardNo );
	void AddColumnText();
	void AddStatistics();
	bool IsStatusActive();

public:
	bool m_bForceMatchAccount;

private:
	CParentPayData* m_pData;
	CWnd* m_pParent;
	int m_nCurrentRow;

	bool m_bLocatedFlag;
	CString m_strStartFromUserID;

	CSSUnsortedDatabase m_dbParentPayRecords;
	CString m_strParentPayID;
	CString m_strParentPayMisID;
	CString m_strParentPayUPN;
	CString m_strParentPayRollNo;
	CString m_strForename;
	CString m_strSurname;
	CString m_strGender;
	CString m_strDob;
	CString m_strYearGroup;
	CString m_strRegGroup;
	CString m_strStatus;

	CReportFileWithEndText m_Report;

	bool m_bImportReqd;
	bool m_bBackgroundMode;

	int m_nRead;
	int m_nIgnored;
	int m_nUsed;
	int m_nInvalid;
	int m_nAdded;
	int m_nLocatedByRollNo;
	int m_nLocatedByUPN;
	int m_nLocatedByMisID;
	int m_nInactive;
	int m_nExisting;

	CString m_strReportTitle;
	CString m_strTempExceptionFile;
	CString m_strTempPendingFile;
	CString m_strDinerMatchAdviceFile;
	CString m_strError;
	CString m_strExceptionComment;
};
