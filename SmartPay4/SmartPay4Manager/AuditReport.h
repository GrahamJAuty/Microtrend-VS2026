#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\ConsolidationMap.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SmartPayAuditFile.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\TLogDatabase.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\TLogCache.h"
//$$******************************************************************
#include "AuditReportConfig.h"
#include "CardArchiveDlg.h"
//$$******************************************************************

struct CAuditReportLineInfo
{
public:
	CAuditReportLineInfo(CSQLAuditRecord* pAtc);

public:
	bool GotColumnCash1() { return m_bHaveColumnCash1; }
	bool GotColumnPurse1() { return m_bHaveColumnPurse1; }
	bool GotColumnPurse2() { return m_bHaveColumnPurse2; }
	bool GotColumnPurse3() { return m_bHaveColumnPurse3; }
	bool GotColumnPoints() { return m_bHaveColumnPoints; }

public:
	void EnableCash1( bool b ) { m_bHaveColumnCash1 = b; }
	void EnablePurse1(bool b) { m_bHaveColumnPurse1 = b; }
	void EnablePurse2(bool b) { m_bHaveColumnPurse2 = b; }
	void EnablePurse3(bool b) { m_bHaveColumnPurse3 = b; }
	void EnablePoints(bool b) { m_bHaveColumnPoints = b; }

private:
	bool m_bHaveColumnCash1;
	bool m_bHaveColumnPurse1;
	bool m_bHaveColumnPurse2;
	bool m_bHaveColumnPurse3;
	bool m_bHaveColumnPoints;

public:
	double m_dCash1;
	double m_dPurse1Total;
	double m_dPurse1Credit;
	double m_dPurse2;
	double m_dPurse3Total;
	double m_dPurse3Credit;
	int m_nPoints;
};

//$$******************************************************************
#define AUDIT_COLUMN_CASH1 0
#define AUDIT_COLUMN_PURSE1 1
#define AUDIT_COLUMN_PURSE2 2
#define AUDIT_COLUMN_PURSE3 3
#define AUDIT_COLUMN_POINTS 4
//$$******************************************************************

struct CAuditReportColumnInfo
{
public:
	CAuditReportColumnInfo();

public:
	CString m_strColumnName;
	int m_nColumnType;
};

//$$******************************************************************

class CAuditReport
{
public:
	CAuditReport(CAuditReportConfig& ReportConfig);
	int CreateReport();
	int CreateWithArchive(const char* szReportFile);
	int CreateReportFromArray(CStringArray* pAuditArray);

private:
	void ResetTotals(int nIndex);
	void AddColumnText();
	bool SaveStartingBalanceLine(CSQLAuditRecord* pAtc, bool bCardExists = TRUE);
	bool SaveLine(CSQLAuditRecord* pAtc, bool bCardExists);
	void SaveTotals(int nIndex);

	int AuditReport(CSmartPayAuditFile* pAudit, CWorkingDlg* pDlgWorking);
	int AuditReportByDate(CSmartPayAuditFile* pAudit, CWorkingDlg* pDlgWorking);
	int AuditReportByCardNo(CSmartPayAuditFile* pAudit, CWorkingDlg* pDlgWorkings);

	CString GetCommentText(CSQLAuditRecord* pAtc);
	
	int ScanArchive(int nIndex, CWorkingDlg* pProgress);

private:
	bool IsValid(CSQLAuditRecord* pAtc);
	bool IsCardInRange(CSQLAuditRecord* pAtc);

private:
	void WriteHeaderLine(const CMapKeyAuditSort& Key);
	bool AppendAuditLineFields(CCSV& csv, CSQLAuditRecord* pAtc, bool bCardExists);
	void AppendEODLineFields(CCSV& csv, CSQLAuditRecord* pAtc);
	void AppendTranTotals(CCSV& csv, CAuditReportLineInfo& info, CSQLAuditRecord* pAtc);
	void AppendLineTotals(CCSV& csv, bool bGotTotal, double dValue, double(&TotalsAdd)[2], double(&TotalsSub)[2]);
	void AppendLineTotals(CCSV& csv, bool bGotTotal, int nValue, int(&TotalsAdd)[2], int(&TotalsSub)[2]);
	void AppendBalanceTotals(CCSV& csv, CSQLAuditRecord* pAtc);
	void AppendSTDTotals(CCSV& csv, CSQLAuditRecord* pAtc);
	void AppendDeletionTotals(CCSV& csv, CSQLAuditRecord* pAtc);

private:
	void ResetColumnInfo();
	void AddColumnInfoTran(int nColumnType, CString strColumnName, bool bFlag = TRUE);
	void AddColumnInfoBalance(int nColumnType, CString strColumnName, bool bFlag = TRUE);
	void AddColumnInfoSTD(int nColumnType, CString strColumnName, bool bFlag = TRUE);

private:
	bool m_bShowTotalAdded;
	bool m_bShowTotalDeducted;

	double m_dTTLColumnCash1Added[2];
	double m_dTTLColumnCash1Subed[2];

	double m_dTTLColumnPurse1Added[2];
	double m_dTTLColumnPurse1Subed[2];

	double m_dTTLColumnPurse2Added[2];
	double m_dTTLColumnPurse2Subed[2];

	double m_dTTLColumnPurse3Added[2];
	double m_dTTLColumnPurse3Subed[2];

	int m_nTTLColumnPointsSubed[2];
	int m_nTTLColumnPointsAdded[2];

private:
	CAuditReportConfig& m_ReportConfig;
	CReportFileWithEndText m_Report;

private:
	CArray<CAuditReportColumnInfo> m_arrayColumnInfoTran;
	CArray<CAuditReportColumnInfo> m_arrayColumnInfoBalance;
	CArray<CAuditReportColumnInfo> m_arrayColumnInfoSTD;

private:
	CTLogCache m_TLogCache;
};

//$$******************************************************************
