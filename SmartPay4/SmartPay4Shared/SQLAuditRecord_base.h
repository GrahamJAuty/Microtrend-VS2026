#pragma once
//*******************************************************************
#include "AuditReportTexts.h"
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
//*******************************************************************
#include "SQLTable_Account\SQLRowAccountFull.h"
//*******************************************************************
#define REQFILETYPE_NONE 0
#define REQFILETYPE_UNKNOWN 1			
#define REQFILETYPE_DEFAULT 10
#define REQFILETYPE_MANUAL 20
#define REQFILETYPE_BIO 30
#define REQFILETYPE_CARDSCAN 40
#define REQFILETYPE_PIN 60
#define REQFILETYPE_INTERNAL 1000
//*******************************************************************

class CSQLAuditRecord_base
{
public:
	CSQLAuditRecord_base();
	void Set(int nSourceType, int nAppNo = 0);
	void Set(CSQLRowAccountFull* pRow, int nSourceType = AUDIT_PC);

	bool ReadLine(CSSFile* file);
	bool ReadLineFromArray(CStringArray* pArray, int& nNextArrayLineNo);
	bool ReadFromString(CString str);
	bool ExtractData(CCSV* pCsv);
	bool WriteLineToFile(CSSFile* pFile);
	bool WriteLineToArray(CStringArray& array);

	bool SaveLineToFile();
	bool SaveLineToFile(const char* szFilename, bool bIsTemporaryFile);
	bool SaveLineToFile(CSSFile* file, bool bIsTemporaryFile, bool bNoWebUpdateForPebble);

	bool SaveNewLine();
	bool SaveNewLine(const char* szFilename, bool bNoWebUpdateForPebble = FALSE);
	bool SaveNewLine(CSSFile* file);
	bool SaveSessionLine(const char* szFilename);				// save a temp seesion audit lines
	bool SaveLineToArray(CStringArray& array);

	void ClearPendingLines();
	void WritePendingLines();
	void BufferNewLine();

	bool AuditChanges(CSQLRowAccountFull* pRow, bool bForceAudit = FALSE);

	const char* GetDateFiled() { return m_dateFiled.GetDate(); }
	const char* GetCSVDateFiled() { return m_dateFiled.GetCSVDate(); }
	const char* GetTimeFiled() { return m_timeFiled.GetTime(); }

	const char* GetDateLastUsed() { return m_dateLastUsed.GetDate(); }
	void SetDateLastUsed(const char* szDate) { m_dateLastUsed.SetDate(szDate); }

	const char* GetTimeLastUsed() { return m_timeLastUsed.GetTime(); }
	void SetTimeLastUsed(const char* szTime) { m_timeLastUsed.SetTime(szTime); }

	const char* GetCardNo() { return m_strUserID; }
	const char* GetUserID() { return m_strUserID; }
	void SetUserID(const char* szUserID) { m_strUserID = szUserID; }

	const char* GetXFerCardNo() { return m_strXFerUserID; }
	const char* GetXFerUserID() { return m_strXFerUserID; }
	void SetXFerUserID(const char* szXFerUserID) { m_strXFerUserID = szXFerUserID; }

	const char* GetUserName() { return m_strUserName; }
	void SetUserName(const char* szUserName) { m_strUserName = szUserName; }

	int GetGroupNo() { return m_nGroupNo; }
	void SetGroupNo(int nGroupNo) { m_nGroupNo = nGroupNo; }
	void SetAlternateGroupNo(int nGroupNo) { m_nAlternateGroupNo = nGroupNo; }

	int GetPasswordNo() { return m_nPasswordNo; }
	CString GetReportPasswordNo();

	//PURSE AND POINTS GET FUNCTIONS
	int GetCurrentPoints() { return m_nCurrentPoints; }
	double GetCurrentPurse1Liability() { return m_dCurrentPurse1Liability; }
	double GetCurrentPurse1Credit() { return m_dCurrentPurse1Credit; }
	double GetCurrentPurse1Total() { return m_dCurrentPurse1Liability + m_dCurrentPurse1Credit; }
	double GetCurrentPurse2Balance() { return m_dCurrentPurse2Balance; }
	double GetCurrentPurse3Liability() { return m_dCurrentPurse3Liability; }
	double GetCurrentPurse3Credit() { return m_dCurrentPurse3Credit; }
	double GetCurrentPurse3Total() { return m_dCurrentPurse3Liability + m_dCurrentPurse3Credit; }

	int GetTransactionPoints() { return m_nTransactionPoints; }
	double GetTransactionPurse1Liability() { return m_dTransactionPurse1Liability; }
	double GetTransactionPurse1Credit() { return m_dTransactionPurse1Credit; }
	double GetTransactionPurse1Total() { return m_dTransactionPurse1Liability + m_dTransactionPurse1Credit; }
	double GetTransactionPurse2Balance() { return m_dTransactionPurse2Balance; }
	double GetTransactionPurse3Liability() { return m_dTransactionPurse3Liability; }
	double GetTransactionPurse3Credit() { return m_dTransactionPurse3Credit; }
	double GetTransactionPurse3Total() { return m_dTransactionPurse3Liability + m_dTransactionPurse3Credit; }
	double GetTransactionCash() { return m_dTransactionCash; }
	double GetTransactionTotal();

	int GetToDatePoints() { return m_nToDatePoints; }
	double GetToDatePurse1Spend() { return m_dToDatePurse1Spend; }
	double GetToDatePurse2Spend() { return m_dToDatePurse2Spend; }
	double GetToDatePurse3Spend() { return m_dToDatePurse3Spend; }
	double GetToDateCashSpend() { return m_dToDateCashSpend; }

	//PURSE AND POINTS SET FUNCTIONS
	void SetCurrentPoints(int n) { m_nCurrentPoints = n; }
	void SetCurrentPurse1Liability(double d) { m_dCurrentPurse1Liability = d; }
	void SetCurrentPurse1Credit(double d) { m_dCurrentPurse1Credit = d; }
	void SetCurrentPurse2Balance(double d) { m_dCurrentPurse2Balance = d; }
	void SetCurrentPurse3Liability(double d) { m_dCurrentPurse3Liability = d; }
	void SetCurrentPurse3Credit(double d) { m_dCurrentPurse3Credit = d; }

	void SetTransactionPoints(int n) { m_nTransactionPoints = n; }
	void SetTransactionPurse1Liability(double d) { m_dTransactionPurse1Liability = d; }
	void SetTransactionPurse1Credit(double d) { m_dTransactionPurse1Credit = d; }
	void SetTransactionPurse2Balance(double d) { m_dTransactionPurse2Balance = d; }
	void SetTransactionPurse3Liability(double d) { m_dTransactionPurse3Liability = d; }
	void SetTransactionPurse3Credit(double d) { m_dTransactionPurse3Credit = d; }
	void SetTransactionCash(double d) { m_dTransactionCash = d; }

	void SetToDatePoints(int n) { m_nToDatePoints = n; }
	void SetToDatePurse1Spend(double d) { m_dToDatePurse1Spend = d; }
	void SetToDatePurse2Spend(double d) { m_dToDatePurse2Spend = d; }
	void SetToDatePurse3Spend(double d) { m_dToDatePurse3Spend = d; }
	void SetToDateCashSpend(double d) { m_dToDateCashSpend = d; }

	//OTHER GET AND SET FUNCTIONS
	int GetSourceType() { return m_nSourceType; }
	void SetSourceType(int nSourceType) { m_nSourceType = nSourceType; }

	int GetApplicationNo() { return m_nApplicationNo; }
	void SetApplicationNo(int nNo) { m_nApplicationNo = nNo; }

	int GetCCNoFull() { return m_nCCNoFull; }
	int GetCCNoShort() { return m_nCCNoFull % 10000; }
	void SetCCNoFull(int nCCNo) { m_nCCNoFull = nCCNo; }

	int GetTerminalNo() { return m_nTerminalNo; }
	void SetTerminalNo(int nTerminalNo) { m_nTerminalNo = nTerminalNo; }

	//	int GetOperatorNo() { return m_nOperatorNo; }
	//	void SetOperatorNo ( int nOperatorNo ) { m_nOperatorNo = nOperatorNo; }
	CString GetOperatorID() { return m_strOperatorID; }
	void SetOperatorID(const char* szID) { m_strOperatorID = szID; }
	void SetOperatorID(int nOperatorNo);

	CString GetAuditLineExtraText() { return m_strAuditLineExtraText; }
	void SetAuditLineExtraText(const char* strText) { m_strAuditLineExtraText = strText; }
	void SetAuditLineExtraText(int nRef) { m_strAuditLineExtraText.Format("%d", nRef); }

	CString GetLineComment() { return m_strComment; }
	CString GetLineOrDefaultComment();
	void SetLineComment(const char* strComment) { m_strComment = strComment; }

	int GetFolderSet() { return m_nFolderSet; }
	void SetFolderSet(int n) { m_nFolderSet = n; }

	int GetREQFileType() { return m_nREQFileType; }
	CString GetREQFileTypeText();
	void SetREQFileType(int n) { m_nREQFileType = n; }

	void SetWebPaymentInfo(__int64 nPaymentID);
	CString GetWebPaymentID() { return GetAuditLineExtraText(); }

public:
	CSSDate m_dateFiled;				// date line save in audit file
	CSSTime m_timeFiled;				// time line savedd in audit
	CSSDate m_dateLastUsed;				// date last used @ till
	CSSTime m_timeLastUsed;				// time last used @ till

private:
	bool Save(const char* szFilename, bool bIsTemporaryFile, bool bNoWebUpdateForPebble = FALSE);
	void CreateWriteableLine(CCSV& csv);
	void SaveWebUpdates(bool bNoWebUpdateForPebble);

protected:
	CString m_strUserID;				// card no
	CString m_strXFerUserID;			// card no
	CString m_strUserName;				// card name
	int m_nGroupNo;						// group number
	int m_nSourceType;					// AUDIT_SERVER, AUDIT_PC
	int m_nApplicationNo;				// application no
	int m_nCCNoFull;
	int m_nTerminalNo;
	CString m_strOperatorID;			// was numeric not string 22/09/2016
	CString m_strAuditLineExtraText;
	CString m_strComment;

	int m_nCurrentPoints;
	double m_dCurrentPurse1Liability;
	double m_dCurrentPurse1Credit;
	double m_dCurrentPurse2Balance;
	double m_dCurrentPurse3Liability;
	double m_dCurrentPurse3Credit;

	int m_nTransactionPoints;
	double m_dTransactionPurse1Liability;
	double m_dTransactionPurse1Credit;
	double m_dTransactionPurse2Balance;
	double m_dTransactionPurse3Liability;
	double m_dTransactionPurse3Credit;
	double m_dTransactionCash;

	int m_nToDatePoints;
	double m_dToDatePurse1Spend;
	double m_dToDatePurse2Spend;
	double m_dToDatePurse3Spend;
	double m_dToDateCashSpend;

	int m_nFolderSet;
	int m_nREQFileType;
	int m_nPasswordNo;

private:
	int m_nAlternateGroupNo;			// alternate group number

private:
	CStringArray m_arrayPendingAudits;
};

/**********************************************************************/
