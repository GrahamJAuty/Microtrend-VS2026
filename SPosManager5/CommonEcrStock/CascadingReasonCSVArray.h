#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#define CASCADINGREASON_VERSION 1
/**********************************************************************/
#define REASONTEXT_TYPE_ITEMVOID 0
#define REASONTEXT_TYPE_TRANVOID 1
#define REASONTEXT_TYPE_REFUND 2
#define REASONTEXT_TYPE_VIP 3
#define REASONTEXT_TYPE_PROMO 4
#define REASONTEXT_TYPE_WASTAGE 5
#define REASONTEXT_TYPE_NOSALE 6
#define REASONTEXT_TYPE_VOIDSURCHARGE 7
/**********************************************************************/
#define REASONTEXT_TYPES 8
/**********************************************************************/

namespace CascadingReason
{
	const recInt		ReasonNo =						{ "ReasonNo", 1, 99 };
	const recInt		ParentReasonConLevelInternal =	{ "ParentReasonConLevelInternal", 0, 2 };
	const recString		ReasonText =					{ "ReasonText", 0, 150 };
	const recBool		ManualEntryFlag =				{ "ManualEntry" };
	const recBool		LocalReasonFlag =				{ "LocalReason" };	
};

/**********************************************************************/

class CCascadingReasonCSVRecord : public CCSVRecord
{
public:
	CCascadingReasonCSVRecord();
	virtual ~CCascadingReasonCSVRecord() {}
	virtual void ConvertToCSV(CCSV& csv);
	virtual void ConvertFromCSV(CCSV& csv);

public:
	int GetReasonNo() { return m_nReasonNo; }
	int GetParentReasonConLevelExternal();
	const char* GetParentReasonText() { return m_strParentReasonText; }
	bool GetParentManualEntryFlag() { return m_bParentManualEntry; }
	bool GetLocalReasonFlag() { return m_bLocalReason; }
	const char* GetLocalReasonText() { return m_strLocalReasonText; }
	bool GetLocalManualEntryFlag() { return m_bLocalManualEntry; }

	void SetReasonNo(int n);
	void SetParentReasonConLevelExternal(int n);
	void SetParentReasonText(const char* sz);
	void SetParentManualEntryFlag(bool b);
	void SetLocalReasonFlag(bool b) { m_bLocalReason = b; }
	void SetLocalReasonText(const char* sz);
	void SetLocalManualEntryFlag(bool b);

private:
	int GetParentReasonConLevelInternal() { return m_nParentReasonConLevelInternal; }
	void SetParentReasonConLevelInternal(int n);

public:
	void CheckMinimumSPOSVersion(CMinimumSPOSVersion& version);

private:
	void ClearRecord();
	void V1ConvertFromCSV(CCSV& csv);

private:
	int m_nReasonNo;
	int m_nParentReasonConLevelInternal;
	CString m_strParentReasonText;
	bool m_bParentManualEntry;
	bool m_bLocalReason;
	CString m_strLocalReasonText;
	bool m_bLocalManualEntry;
};

/**********************************************************************/

class CCascadingReasonCSVArray : public CSharedCSVArray
{
public:
	CCascadingReasonCSVArray();
	~CCascadingReasonCSVArray();

public:
	void Reset();

public:
	int Open( const char* szFilename, int nMode );

public:
	int GetReasonNo( int nReasonIdx );
	const char* GetReasonTextFromNumber( int nReasonNo );
	int GetReasonType() { return m_nReasonType;  }
	void SetReasonType(int nType) { m_nReasonType = nType; }

public:
	static CString GetReasonTypeText(int n);
	static CString GetSQLLabelByReasonType(int n);
	static int GetReasonTypeBySQLLabel(CString str);
	static CString GetCfgTillLabelByReasonType(int n, bool bSwitchItemVoid);

public:
	bool FindReasonByNumber( int nReasonNo, int& nReasonIdx );
	int FindFirstFreeNumber();

public:
	void LoadParentReasonsSystem();
	void LoadParentReasonsDatabase( int nDbIdx );
	
private:
	void LoadParentReasons( CLocationSelectorEntity& LocSelEntity );

public:
	void PrepareForWrite();

private:
	CString m_strReasonText;
	int m_nReasonType;
};

/**********************************************************************/
#endif
/**********************************************************************/
