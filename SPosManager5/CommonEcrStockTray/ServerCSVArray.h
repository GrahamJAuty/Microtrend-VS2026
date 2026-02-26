#pragma once
/**********************************************************************/
#define SERVER_VERSION 9
/**********************************************************************/
#include "MinimumSPOSVersion.h"
#include "NamespaceServer.h"
/**********************************************************************/
#include <map>
/**********************************************************************/
#define SERVER_SORT_NUMBER 0
#define SERVER_SORT_NAME_UP 1
#define SERVER_SORT_NAME_DOWN 2
#define SERVER_SORT_HOMELOC_UP 3
#define SERVER_SORT_HOMELOC_DOWN 4
/**********************************************************************/
#define MAX_SERVERNO_LEN 4
/**********************************************************************/

class CServerCSVRecord : public CCSVRecord
{
public:
	CServerCSVRecord();
	virtual ~CServerCSVRecord() {}
	virtual void ConvertToCSV(CCSV& csv);
	virtual void ConvertFromCSV(CCSV& csv);
	void ImportLine(CSSImportFile& import, CCSV& csv);

private:
	void ImportString(CCSV& csv, CSSImportFile& import, int& n, CString& str, const recString& rec, bool bFilter = FALSE);
	void ImportDouble(CCSV& csv, CSSImportFile& import, int& n, double& d, const recDouble& rec);
	void ImportInt(CCSV& csv, CSSImportFile& import, int& n, int& i, const recInt& rec);
	void ImportInt64(CCSV& csv, CSSImportFile& import, int& n, __int64& i, const recInt64& rec);
	void ImportBool(CCSV& csv, CSSImportFile& import, int& n, bool& b);

public:
	int GetServerNo() { return m_nServerNo; }
	const char* GetTimeStamp() { return m_strTimeStamp; }
	const char* GetReceiptName() { return m_strReceiptName; }
	const char* GetFullName() { return m_strFullName; }
	int GetAuthority() { return m_nAuthority; }
	int GetPassword() { return m_nPassword; }
	int GetDrawer() { return m_nDrawer; }
	bool GetFlipViewFlag() { return m_bFlipView; }
	bool GetTrainingModeFlag() { return m_bTrainingMode; }
	int GetLogonMacro() { return m_nLogonMacro; }
	int GetOpenTableMode() { return m_nOpenTableMode; }
	int GetHomeNwkLocNo() { return m_nHomeNwkLocNo; }
	double GetHourlyRate() { return m_dHourlyRate; }
	int GetDailyHours() { return m_nDailyHours; }
	bool GetMidnightShiftFlag() { return m_bMidnightShift; }
	int GetTradingColourScheme() { return m_nTradingColourScheme; }
	
	const char* GetReportName();
	const char* GetReceiptEditName();
	CString GetHomeLocName();

	void SetServerNo(int n);
	void SetTimeStamp(const char* sz);
	void SetReceiptName(const char* sz);
	void SetFullName(const char* sz);
	void SetAuthority(int n);
	void SetPassword(int n);
	void SetDrawer(int n);
	void SetFlipViewFlag(bool b) { m_bFlipView = b; }
	void SetTrainingModeFlag(bool b) { m_bTrainingMode = b; }
	void SetLogonMacro(int n);
	void SetOpenTableMode(int n);
	void SetHomeNwkLocNo(int n);
	void SetHourlyRate(double d);
	void SetDailyHours(int n);
	void SetMidnightShiftFlag(bool b) { m_bMidnightShift = b; }
	void SetTradingColourScheme(int n);
	
public:
	void CheckMinimumSPOSVersion(CMinimumSPOSVersion& version);

private:
	void ClearRecord();
	void V1ConvertFromCSV(CCSV& csv);
	void V2ConvertFromCSV(CCSV& csv);
	void V3ConvertFromCSV(CCSV& csv);
	void V4to9ConvertFromCSV(CCSV& csv, int nVer);
	
private:
	int m_nServerNo;
	CString m_strTimeStamp;
	CString m_strReceiptName;
	CString m_strFullName;
	int m_nAuthority;
	int m_nPassword;
	int m_nDrawer;
	bool m_bFlipView;
	bool m_bTrainingMode;
	int m_nLogonMacro;
	int m_nOpenTableMode;
	int m_nHomeNwkLocNo;
	double m_dHourlyRate;
	int m_nDailyHours;
	bool m_bMidnightShift;
	int m_nTradingColourScheme;
	
private:
	CString m_strReportName;
	CString m_strReceiptEditName;
	CString m_strHomeLocName;
};

/**********************************************************************/

class CServerCSVArray : public CSharedCSVArray
{
public:
	CServerCSVArray();
	~CServerCSVArray();

public:
	int Open( const char* szFilename, int nMode );

public:
	int GetServerNo( int nIndex );

public:
	bool FindServerByNumber( int nServerNo, int& nServerIdx );
	int FindFirstFreeNumber();
	bool IsTrainingServer( int nServerNo );

public:
	const char* GetReportName( int nServer );
	
public:
	void CreateServerReport( int nDbIdx, int nLocIdx, int nTerminalLocIdx, bool bSortOrder, CReportFile& ReportFile );

private:
	void AppendRecordToReport(CReportFile& ReportFile, int nServerIdx, int nTerminalLocIdx, std::map<int, int64_t>& MapLogonNoToServerNo);

public:
	int ImportLine(int nImportMethod, int& nIndex, CSSImportFile& import, const char* szImportLine);

public:
	void SetSortMode(int nMode );
	void RefreshSort() { SetSortMode(m_nSortMode ); }
	int GetSortMode() { return m_nSortMode; }
	int GetSortArraySize();
	int GetArrayIdxFromSortIdx(int n);
	int GetSortIdxFromArrayIdx(int n);

public:
	void SetShowHomeLocFlag(bool b) { m_bShowHomeLoc = b; }
	void SetServerDbNo(int n) { m_nServerDbNo = n;  }
	bool GetShowHomeLocFlag() { return m_bShowHomeLoc;  }
	int GetServerDbNo() { return m_nServerDbNo; }

private:
	int m_nSortMode;
	CArray<int, int> m_arraySort;

private:
	int m_nServerDbNo;
	bool m_bShowHomeLoc;

private:
	CString m_strReportName;
};

/**********************************************************************/
