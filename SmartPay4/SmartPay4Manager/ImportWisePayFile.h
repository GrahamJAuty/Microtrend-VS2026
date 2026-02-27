#pragma once
//********************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//********************************************************************
#include "..\SmartPay4ManagerBgnd\WisePayData.h"
//********************************************************************

class CImportWisePayFile
{
public:
	CImportWisePayFile ( CWisePayData* pData, CWnd* pParent );
	~CImportWisePayFile();

	bool Import();
	bool ShowImports();

	CString GetError() { return m_strError; }
	void ShowStatistics();

private:
	void GetNewCardNo ( __int64& nCardNo );

	int ProcessFile ( bool bImportReqd );
	void SetRecord( CSQLRowAccountFull& RowAccount);

	bool HaveEmailAddress();

	void ImportLine ( __int64 nUserID );
	bool ShowImportLine (__int64 nUserID);
	void AddColumnText();
	void AddStatistics();

private:
	CWisePayData* m_pData;
	CWnd* m_pParent;
	int m_nCurrentRow;

	CSSUnsortedDatabase m_dbWisePayRecords;
	CString m_strUID;
	CString m_strWisePayID;
	CString m_strForename;
	CString m_strSurname;
	CString m_strGender;
	CString m_strDob;
	CString m_strYearGroup;
	CString m_strRegGroup;
	CString m_strEmail;

	CReportFileWithEndText m_Report;

	CString m_strStartFromUserID;
	bool m_bImportReqd;
	bool m_bBackgroundMode;

	int m_nRead;
	int m_nIgnored;
	int m_nUsed;
	int m_nInvalid;
	int m_nAdded;

	CString m_strReportTitle;
	CString m_strTempExceptionFile;
	CString m_strTempPendingFile;
	CString m_strDinerMatchAdviceFile;
	CString m_strError;
	CString m_strExceptionComment;
};
