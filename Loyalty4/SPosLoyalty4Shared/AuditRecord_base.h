#pragma once
//**********************************************************************
#include "AuditReportTexts.h"
//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//**********************************************************************

class CAuditRecord_base
{
public:
	CAuditRecord_base();
	void Set(int nSourceType,bool bSameEposTran = FALSE);
	void SetSQL(CSQLRowAccountFull& RowAccount, int nSourceType = AUDIT_PC, bool bSameEposTran = FALSE);

	bool ReadLine(CSSFile* file);
	bool ReadFromString(CString str);
	bool WriteLineToFile(CSSFile* pFile);
	bool WriteLineToArray(CStringArray& array);

	bool SaveLineToFile();
	bool SaveLineToFile(const char* szFilename, bool bIsTemporaryFile);
	bool SaveLineToFile(CSSFile* file, bool bIsTemporaryFile);
	bool SaveLineToArray(CStringArray& array);

	bool ExtractData(CCSV* pCsv);

	const char* GetDateFiled() { return m_dateFiled.GetDate(); }
	const char* GetTimeFiled() { return m_timeFiled.GetTime(); }

	const char* GetDateLastUsed() { return m_dateLastUsed.GetDate(); }
	void SetDateLastUsed(const char* szDate) { m_dateLastUsed.SetDate(szDate); }

	const char* GetTimeLastUsed() { return m_timeLastUsed.GetTime(); }
	void SetTimeLastUsed(const char* szTime) { m_timeLastUsed.SetTime(szTime); }

	const char* GetCardNo() { return m_strCardNo; }
	void SetCardNo(const char* szCardNo) { m_strCardNo = szCardNo; }

	const char* GetUserName() { return m_strCardName; }
	const char* GetCardName() { return m_strCardName; }
	void SetCardName(const char* szCardName) { m_strCardName = szCardName; }

	int GetGroupNo() { return m_nGroupNo; }
	void SetGroupNo(int nGroupNo) { m_nGroupNo = nGroupNo; }

	int GetPoints() { return m_nPoints; }
	void SetPoints(int nPoints) { m_nPoints = nPoints; }
	int GetPointsTransaction() { return m_nPointsTransaction; }
	void SetPointsTransaction(int nPoints) { m_nPointsTransaction = nPoints; }

	double GetPurse1() { return m_dPurse1; }
	void SetPurse1(double dPurse) { m_dPurse1 = dPurse; }

	double GetPurse2() { return m_dPurse2; }
	void SetPurse2(double dPurse) { m_dPurse2 = dPurse; }

	double GetPurse1Transaction() { return m_dPurse1Transaction; }
	void SetPurse1Transaction(double dPurse) { m_dPurse1Transaction = dPurse; }

	double GetPurse2Transaction() { return m_dPurse2Transaction; }
	void SetPurse2Transaction(double dPurse) { m_dPurse2Transaction = dPurse; }

	double GetCashTransaction() { return m_dCashTransaction; }
	void SetCashTransaction(double dPurse) { m_dCashTransaction = dPurse; }

	double GetTotalDiscount() { return m_dTotalDiscount; }
	void SetTotalDiscount(double dDiscount) { m_dTotalDiscount = dDiscount; }

	double GetTotalTransaction() { return m_dPurse1Transaction + m_dPurse2Transaction + m_dCashTransaction; }

	int GetPointsTD() { return m_nPointsTD; }
	void SetPointsTD(int nPoints) { m_nPointsTD = nPoints; }

	double GetPurse1SpendTD() { return m_dPurse1SpendTD; }
	void SetPurse1SpendTD(double dSpend) { m_dPurse1SpendTD = dSpend; }

	double GetPurse2SpendTD() { return m_dPurse2SpendTD; }
	void SetPurse2SpendTD(double dSpend) { m_dPurse2SpendTD = dSpend; }

	double GetCashSpendTD() { return m_dCashSpendTD; }
	void SetCashSpendTD(double dSpend) { m_dCashSpendTD = dSpend; }

	double GetTotalSpendTD() { return m_dPurse1SpendTD + m_dPurse2SpendTD + m_dCashSpendTD; }

	int GetSourceType() { return m_nSourceType; }
	void SetSourceType(int nSourceType) { m_nSourceType = nSourceType; }

	int GetApplicationNo() { return m_nApplicationNo; }
	void SetApplicationNo(int nNo) { m_nApplicationNo = nNo; }

	int GetCCNo() { return m_nCCNo; }
	void SetCCNo(int nCCNo) { m_nCCNo = nCCNo; }

	int GetTerminalNo() { return m_nTerminalNo; }
	void SetTerminalNo(int nTerminalNo) { m_nTerminalNo = nTerminalNo; }

	int GetOperatorNo() { return m_nOperatorNo; }
	void SetOperatorNo(int nOperatorNo) { m_nOperatorNo = nOperatorNo; }

	CString GetEposTranNo() { return m_strEposTranNo; }
	void SetEposTranNo(CString str) { m_strEposTranNo = str; }

	const char* GetComment() { return m_strComment; }
	void SetComment(const char* strComment) { m_strComment = strComment; }

	void SetFolderSetIndex(CServerDataFolderSetIndex& index) { m_FolderSetIndex = index; }
	CServerDataFolderSetIndex GetFolderSetIndex() { return m_FolderSetIndex; }

private:
	void CreateWriteableLine(CCSV& csv);

public:
	CSSDate m_dateFiled;				// date line save in audit file
	CSSTime m_timeFiled;				// time line savedd in audit
	CSSDate m_dateLastUsed;				// date last used @ till
	CSSTime m_timeLastUsed;				// time last used @ till

protected:
	CString m_strCardNo = "";			// card no
	CString m_strCardName = "";			// card name
	int m_nGroupNo = 0;					// group number
	int  m_nPointsTransaction = 0;			// points added
	double m_dPurse1Transaction = 0.0;		// Purse1 portion of transaction
	double m_dPurse2Transaction = 0.0;		// Purse2 portion of transaction
	double m_dCashTransaction = 0.0;			// cash portion of transaction
	double m_dTotalDiscount = 0.0;			// total transaction discount
	int m_nPoints = 0;						// current points
	int  m_nPointsTD = 0;					// points todate
	double m_dPurse1 = 0.0;					// current purse1
	double m_dPurse2 = 0.0;					// current purse2
	double m_dPurse1SpendTD = 0.0;			// Purse1 spend todate
	double m_dPurse2SpendTD = 0.0;			// Purse2 spend todate
	double m_dCashSpendTD = 0.0;				// cash spend todate
	int m_nSourceType = 0;					// AUDIT_SERVER, AUDIT_PC
	int m_nApplicationNo = 0;				// application no
	int m_nCCNo = 0;
	int m_nTerminalNo = 0;
	int m_nOperatorNo = 0;
	CString m_strEposTranNo = "";
	CString m_strComment = "";
	CServerDataFolderSetIndex m_FolderSetIndex;
};

//**********************************************************************
