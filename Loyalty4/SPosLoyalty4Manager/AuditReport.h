#pragma once
//$$******************************************************************
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\LoyaltyAuditFile.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
//$$******************************************************************
#include "AuditRecord.h"
#include "AuditReportConfig.h"
#include "CardArchiveDlg.h"
#include "WorkingDlg.h"
#include "RepmanHandler.h"
#include "TLogDatabase.h"
//$$******************************************************************

struct CAuditReportLineInfo
{
public:
	CAuditReportLineInfo(CAuditRecord* pAtc);

public:
	bool GotColumnEPOSTopup(int p, int t);
	bool GotColumnManualTopupPurse1() { return m_bHaveColumnManualTopupPurse1; }
	bool GotColumnManualTopupPurse2() { return m_bHaveColumnManualTopupPurse2; }
	bool GotColumnImportTopupPurse1() { return m_bHaveColumnImportTopupPurse1; }
	bool GotColumnImportTopupPurse2() { return m_bHaveColumnImportTopupPurse2; }
	bool GotColumnGift() { return m_bHaveColumnGift; }
	bool GotColumnPurse1() { return m_bHaveColumnPurse1; }
	bool GotColumnPurse2() { return m_bHaveColumnPurse2; }
	bool GotColumnPoints() { return m_bHaveColumnPoints; }
	bool GotColumnNonPurse() { return m_bHaveColumnNonPurse; }

public:
	void EnableEPOSTopup(int p, int t, bool b);
	void EnableManualTopupPurse1(bool b) { m_bHaveColumnManualTopupPurse1 = b; }
	void EnableManualTopupPurse2(bool b) { m_bHaveColumnManualTopupPurse2 = b; }
	void EnableImportTopupPurse1(bool b) { m_bHaveColumnImportTopupPurse1 = b; }
	void EnableImportTopupPurse2(bool b) { m_bHaveColumnImportTopupPurse2 = b; }
	void EnableGift(bool b) { m_bHaveColumnGift = b; }
	void EnablePurse1(bool b) { m_bHaveColumnPurse1 = b; }
	void EnablePurse2(bool b) { m_bHaveColumnPurse2 = b; }
	void EnablePoints(bool b) { m_bHaveColumnPoints = b; }
	void EnableNonPurse(bool b) { m_bHaveColumnNonPurse = b; }

private:
	bool m_bHaveColumnEPOSTopup[3][REVALUE_PLUNO_PERPURSE_COUNT + 1];
	bool m_bHaveColumnManualTopupPurse1;
	bool m_bHaveColumnManualTopupPurse2;
	bool m_bHaveColumnImportTopupPurse1;
	bool m_bHaveColumnImportTopupPurse2;
	bool m_bHaveColumnGift;
	bool m_bHaveColumnPurse1;
	bool m_bHaveColumnPurse2;
	bool m_bHaveColumnPoints;
	bool m_bHaveColumnNonPurse;

public:
	double m_dEPOSTopup[3][REVALUE_PLUNO_PERPURSE_COUNT + 1];
	double m_dManualTopupPurse1;
	double m_dManualTopupPurse2;
	double m_dImportTopupPurse1;
	double m_dImportTopupPurse2;
	double m_dGift;
	double m_dPurse1;
	double m_dPurse2;
	int m_nPoints;
	double m_dNonPurse;
};

//$$******************************************************************

struct CAuditTotalInt
{
public:
	CAuditTotalInt()
	{
		m_nTotalMinus[0] = 0;
		m_nTotalMinus[1] = 0;
		m_nTotalPlus[0] = 0;
		m_nTotalPlus[1] = 0;

	}

public:
	void Reset(int nIndex)
	{
		switch (nIndex)
		{
		case 0:
			m_nTotalPlus[0] = 0;
			m_nTotalMinus[0] = 0;
			break;

		case 1:
			m_nTotalPlus[1] = 0;
			m_nTotalMinus[1] = 0;
			break;
		}
	}

	void Add(int nValue)
	{
		if (nValue < 0)
		{
			m_nTotalMinus[0] += nValue;
			m_nTotalMinus[1] += nValue;
		}
		else
		{
			m_nTotalPlus[0] += nValue;
			m_nTotalPlus[1] += nValue;
		}
	}

	int GetTotalPlus(int nIndex)
	{
		switch (nIndex)
		{
		case 0:
			return m_nTotalPlus[0];

		case 1:
			return m_nTotalPlus[1];

		default:
			return 0;
		}
	}

	int GetTotalMinus(int nIndex)
	{
		switch (nIndex)
		{
		case 0:
			return m_nTotalMinus[0];

		case 1:
			return m_nTotalMinus[1];

		default:
			return 0;
		}
	}	

	int GetTotalNet(int nIndex)
	{
		switch (nIndex)
		{
		case 0:
			return m_nTotalPlus[0] + m_nTotalMinus[0];

		case 1:
			return m_nTotalPlus[1] + m_nTotalMinus[1];

		default:
			return 0;
		}
	}

private:
	int m_nTotalPlus[2];
	int m_nTotalMinus[2];	
};

//$$******************************************************************

struct CAuditTotalDouble
{
public:
	CAuditTotalDouble()
	{
		m_dTotalMinus[0] = 0.0;
		m_dTotalMinus[1] = 0.0;
		m_dTotalPlus[0] = 0.0;
		m_dTotalPlus[1] = 0.0;
	}

public:
	void Reset(int nIndex)
	{
		switch (nIndex)
		{
		case 0:
			m_dTotalPlus[0] = 0.0;
			m_dTotalMinus[0] = 0.0;
			break;

		case 1:
			m_dTotalPlus[1] = 0.0;
			m_dTotalMinus[1] = 0.0;
			break;
		}
	}

	void Add(double dValue)
	{
		if (dValue < 0.0)
		{
			m_dTotalMinus[0] += dValue;
			m_dTotalMinus[1] += dValue;
		}
		else
		{
			m_dTotalPlus[0] += dValue;
			m_dTotalPlus[1] += dValue;
		}
	}

	double GetTotalPlus(int nIndex)
	{
		switch (nIndex)
		{
		case 0:
			return m_dTotalPlus[0];

		case 1:
			return m_dTotalPlus[1];

		default:
			return 0.0;
		}
	}

	double GetTotalMinus(int nIndex)
	{
		switch (nIndex)
		{
		case 0:
			return m_dTotalMinus[0];

		case 1:
			return m_dTotalMinus[1];

		default:
			return 0.0;
		}
	}

	double GetTotalNet(int nIndex)
	{
		switch (nIndex)
		{
		case 0:
			return m_dTotalPlus[0] + m_dTotalMinus[0];

		case 1:
			return m_dTotalPlus[1] + m_dTotalMinus[1];

		default:
			return 0.0;
		}
	}

private:
	double m_dTotalPlus[2];
	double m_dTotalMinus[2];
};

//$$******************************************************************
#define AUDIT_COLUMN_NONPURSE 0
#define AUDIT_COLUMN_PURSE1_TOPUP1 1
#define AUDIT_COLUMN_PURSE1_TOPUP2 2
#define AUDIT_COLUMN_PURSE1_TOPUP3 3
#define AUDIT_COLUMN_PURSE1_TOPUP4 4
#define AUDIT_COLUMN_PURSE1_TOPUP5 5
#define AUDIT_COLUMN_PURSE2_TOPUP1 6
#define AUDIT_COLUMN_PURSE2_TOPUP2 7
#define AUDIT_COLUMN_PURSE2_TOPUP3 8
#define AUDIT_COLUMN_PURSE2_TOPUP4 9
#define AUDIT_COLUMN_PURSE2_TOPUP5 10
#define AUDIT_COLUMN_PURSE1_MANTOPUP 11
#define AUDIT_COLUMN_PURSE2_MANTOPUP 12
#define AUDIT_COLUMN_PURSE1_IMPTOPUP 13
#define AUDIT_COLUMN_PURSE2_IMPTOPUP 14
#define AUDIT_COLUMN_GIFT 15
#define AUDIT_COLUMN_PURSE1 16
#define AUDIT_COLUMN_PURSE2 17
#define AUDIT_COLUMN_POINTS 18
#define AUDIT_COLUMN_TTLSPEND 19
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

public:
	int CreateReport();
	int CreateWithArchive(const char* szReportFile);

private:
	void ResetTotals(int nIndex);
	void ClearTLogArrays();
	void AddColumnText();
	bool SaveStartingBalanceLine(CAuditRecord* pAtc, bool bCardExists = TRUE);
	bool SaveLine(CAuditRecord* pAtc, bool bCardExists);
	void SaveTotals(int nIndex);

	int AuditReport(CLoyaltyAuditFile* pAudit, CWorkingDlg* pDlgWorking);
	int AuditReportByDate(CLoyaltyAuditFile* pAudit, CWorkingDlg* pDlgWorking);
	int AuditReportByCardNo(CLoyaltyAuditFile* pAudit, CWorkingDlg* pDlgWorking);

	CString GetCommentText(CAuditRecord* pAtc);

	int ScanArchive(int nIndex, CWorkingDlg* pDlgWorking);

private:
	bool IsValid(CAuditRecord* pAtc);
	bool IsCardInRange(CAuditRecord* pAtc);

private:
	bool AppendAuditLineFields(CCSV& csv, CAuditRecord* pAtc, bool bCardExists);
	void AppendTranTotals(CCSV& csv, CAuditReportLineInfo& info, CAuditRecord* pAtc);
	void AppendLineTotals(CCSV& csv, bool bGotTotal, double dValue, CAuditTotalDouble& Total);
	void AppendLineTotals(CCSV& csv, bool bGotTotal, int nValue, CAuditTotalInt& Total);
	void AppendBalanceTotals(CCSV& csv, CAuditRecord* pAtc);
	void AppendSTDTotals(CCSV& csv, CAuditRecord* pAtc);
	void WriteHeaderLine(CString strLabel);
	void GetEPOSTopupColumnIndex(int nColumnType, int& nPurseNo, int& nTopupNo);

private:
	void ResetColumnInfo();
	void AddColumnInfoTran(int nColumnType, CString strColumnName, bool bFlag = TRUE);
	void AddColumnInfoBalance(int nColumnType, CString strColumnName, bool bFlag = TRUE);
	void AddColumnInfoSTD(int nColumnType, CString strColumnName, bool bFlag = TRUE);

private:
	bool m_bShowTotalAdded;
	bool m_bShowTotalDeducted;

	CAuditTotalDouble m_TotalEPOSTopup[3][REVALUE_PLUNO_PERPURSE_COUNT + 1];
	CAuditTotalDouble m_TotalGift;
	CAuditTotalDouble m_TotalPurse1;
	CAuditTotalDouble m_TotalPurse1ManualTopup;
	CAuditTotalDouble m_TotalPurse1ImportTopup;
	CAuditTotalDouble m_TotalPurse2;
	CAuditTotalDouble m_TotalPurse2ManualTopup;
	CAuditTotalDouble m_TotalPurse2ImportTopup;
	CAuditTotalDouble m_TotalNonPurse;
	CAuditTotalInt m_TotalPoints;

private:
	CAuditReportConfig& m_ReportConfig;
	CReportFile m_Report;

private:
	CArray<CAuditReportColumnInfo> m_arrayColumnInfoTran;
	CArray<CAuditReportColumnInfo> m_arrayColumnInfoBalance;
	CArray<CAuditReportColumnInfo> m_arrayColumnInfoSTD;

private:
	CReportConsolidationArray<CSortedIntByInt64> m_arrayTLogMap;
	CArray<CTLogDatabase*> m_arrayTLogDatabases;
	CTLogDatabase* m_pLastTLogDatabase;
	CStringArray m_arrayDbNames;
};

//$$******************************************************************
