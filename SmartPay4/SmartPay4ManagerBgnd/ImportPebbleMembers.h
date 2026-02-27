#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_PebbleConfig\SQLRepositoryPebbleConfig.h"
/**********************************************************************/
#include "PebbleHelpers.h"
/**********************************************************************/
#ifdef SYSTEMTYPE_MANAGER
/**********************************************************************/
#include "..\SmartPay4Manager\ImportStatsDlg.h"
/**********************************************************************/
#endif
/**********************************************************************/
#define PEBBLE_IMPORTLINE_INACTIVE 0
#define PEBBLE_IMPORTLINE_ALREADYMATCHED 1
#define PEBBLE_IMPORTLINE_MATCHEDBYUPN 2
#define PEBBLE_IMPORTLINE_MATCHEDBYMIS 3
#define PEBBLE_IMPORTLINE_NOTMATCHED 4
#define PEBBLE_IMPORTLINE_UPNMISMATCH 5
/**********************************************************************/

struct CPebbleImportStats	
{
public:
	void Reset()
	{
		m_nRead = 0;
		m_nAlreadyMatched = 0;
		m_nPebbleInactive = 0;
		m_nMatchedByUPN = 0;
		m_nMatchedByMisID = 0;
		m_nNotMatched = 0;
		m_nUPNMismatch = 0;
		m_nNewLinks = 0;
		/*****/
		m_nCurrentLineStatus = PEBBLE_IMPORTLINE_INACTIVE;
		m_strCurrentLineException = "";
	}

	bool GotExceptions() { return (m_nUPNMismatch + m_nNotMatched) > 0; }

public:
	int m_nRead = 0;
	int m_nAlreadyMatched = 0;
	int m_nPebbleInactive = 0;
	int m_nMatchedByUPN = 0;
	int m_nMatchedByMisID = 0;
	int m_nNotMatched = 0;
	int m_nUPNMismatch = 0;
	int m_nNewLinks = 0;
	/*****/
	int m_nCurrentLineStatus = PEBBLE_IMPORTLINE_INACTIVE;
	CString m_strCurrentLineException = "";
};

/**********************************************************************/

class CImportPebbleMembers
{
public:
	CImportPebbleMembers(CSQLRepositoryPebbleConfig* pConfig, CArray<CPebbleMemberBuffer, CPebbleMemberBuffer>& arrayMembers, CWnd* pParent);
	~CImportPebbleMembers();

	bool ImportMembers();
	CString GetError() { return m_strError; }

#ifdef SYSTEMTYPE_MANAGER
	void ShowStatistics(CImportStatsDlgConfig& ImportStatsConfig);
	bool ShowImports(bool bFirstTime);
#endif

private:
	bool CheckPebbleUPN(__int64 nCardNo);
	bool MatchAccount(__int64& nCardNo);

	int ImportMembersInternal(bool bImportReqd);

	void ImportLine(__int64 nCardNo);
	bool ShowImportLine(__int64 nCardNo);
	void AddColumnText();
	void AddStatistics();

public:
	CPebbleImportStats m_ImportStats;

private:
	CSQLRepositoryPebbleConfig* m_pConfig = nullptr;
	CWnd* m_pParent = nullptr;

	int m_nCurrentRow = 0;
	CReportFileWithEndText m_Report;

	bool m_bImportReqd = FALSE;
	bool m_bBackgroundMode = FALSE;
	
	CString m_strReportTitle = "Link Pebble Members";
	CString m_strTempExceptionFile = "";
	CString m_strTempPendingFile = "";
	CString m_strError = "";

private:
	CArray<CPebbleMemberBuffer, CPebbleMemberBuffer>& m_arrayMembers;
	CPebbleMemberBuffer m_currentMember;
};

/**********************************************************************/
