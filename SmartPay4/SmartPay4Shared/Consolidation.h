#pragma once
/**********************************************************************/

struct CSortedIntItem
{
public:
	CSortedIntItem();
	void Reset();

public:
	int Compare ( CSortedIntItem& source, int nHint = 0 );
	void Add	( CSortedIntItem& source );
	
public:
	int m_nItem;
};

/**********************************************************************/

struct CSortedInt64Item
{
public:
	CSortedInt64Item();
	void Reset();

public:
	int Compare ( CSortedInt64Item& source, int nHint = 0 );
	void Add	( CSortedInt64Item& source );
	
public:
	__int64 m_nItem;
};

/**********************************************************************/

struct CSortedStringItem
{
public:
	CSortedStringItem();
	void Reset();
	
public:
	int Compare ( CSortedStringItem& source, int nHint = 0 );
	void Add	( CSortedStringItem& source );
	
public:
	CString m_strItem;
};

/**********************************************************************/

struct CSortedDateTimeItem
{
public:
	CSortedDateTimeItem();
	void Reset();
	
public:
	void SetDate( COleDateTime date );
	int Compare ( CSortedDateTimeItem& source, int nHint = 0 );
	void Add	( CSortedDateTimeItem& source );
	
public:
	CString m_strDateTime;
	COleDateTime m_oleDateTime;
};

/**********************************************************************/

struct CSortedStringAndIntItem
{
public:
	CSortedStringAndIntItem();
	void Reset();

public:
	int Compare ( CSortedStringAndIntItem& source, int nHint = 0 );
	void Add	( CSortedStringAndIntItem& source );	
public:
	CString m_strItem;
	int m_nItem;
};

/**********************************************************************/

struct CSortedIntAndStringItem
{
public:
	CSortedIntAndStringItem();
	void Reset();

public:
	int Compare ( CSortedIntAndStringItem& source, int nHint = 0 );
	void Add	( CSortedIntAndStringItem& source );
	
public:
	int m_nItem;
	CString m_strItem;
};

/**********************************************************************/

struct CSortedStringCounter
{
public:
	CSortedStringCounter();
	void Reset();

public:
	int Compare ( CSortedStringCounter& source, int nHint = 0 );
	void Add	( CSortedStringCounter& source );
	
public:
	CString m_strItem;
	int m_nCount;
};

/**********************************************************************/

struct CSortedIntByInt64
{
public:
	CSortedIntByInt64();
	void Reset();

public:
	int Compare ( CSortedIntByInt64& source, int nHint = 0 );
	void Add	( CSortedIntByInt64& source );
	
public:
	__int64 m_nKey;
	int m_nVal;
};

/**********************************************************************/

struct CSortedIntByInt
{
public:
	CSortedIntByInt();
	void Reset();

public:
	int Compare ( CSortedIntByInt& source, int nHint = 0 );
	void Add	( CSortedIntByInt& source );
	
public:
	int m_nKey;
	int m_nVal;
};

/**********************************************************************/

struct CSortedIntByString
{
public:
	CSortedIntByString();
	void Reset();

public:
	int Compare ( CSortedIntByString& source, int nHint = 0 );
	void Add	( CSortedIntByString& source );
	
public:
	CString m_strItem;
	int m_nItem;
};

/**********************************************************************/

struct CSortedStringByInt
{
public:
	CSortedStringByInt();
	void Reset();

public:
	int Compare ( CSortedStringByInt& source, int nHint = 0 );
	void Add	( CSortedStringByInt& source );
	
public:
	int m_nItem;
	CString m_strItem;
};

/**********************************************************************/

struct CSortedStringByInt64
{
public:
	CSortedStringByInt64();
	void Reset();

public:
	int Compare(CSortedStringByInt64& source, int nHint = 0);
	void Add(CSortedStringByInt64& source);

public:
	__int64 m_nItem;
	CString m_strItem;
};

/**********************************************************************/

struct CSortedStringAndIntByInt64
{
public:
	CSortedStringAndIntByInt64();
	void Reset();

public:
	int Compare(CSortedStringAndIntByInt64& source, int nHint = 0);
	void Add(CSortedStringAndIntByInt64& source);

public:
	__int64 m_nKey;
	CString m_strVal;
	int m_nVal;
};

/**********************************************************************/

struct CSortedStringByString
{
public:
	CSortedStringByString();
	void Reset();
	
public:
	int Compare ( CSortedStringByString& source, int nHint = 0 );
	void Add	( CSortedStringByString& source );
	
public:
	CString m_strKey;
	CString m_strData;
};

/**********************************************************************/

struct CSortedInt64ByString
{
public:
	CSortedInt64ByString();
	void Reset();

public:
	int Compare(CSortedInt64ByString& source, int nHint = 0);
	void Add(CSortedInt64ByString& source);

public:
	CString m_strKey;
	__int64 m_nData;
};

/**********************************************************************/

struct CSortedIntByInt64AndString
{
public:
	CSortedIntByInt64AndString();
	void Reset();

public:
	int Compare(CSortedIntByInt64AndString& source, int nHint = 0);
	void Add(CSortedIntByInt64AndString& source);

public:
	__int64 m_nKey;
	CString m_strKey;
	int m_nVal;
};

/**********************************************************************/

struct CConsolidatedIntByInt
{
public:
	CConsolidatedIntByInt();
	void Reset();

public:
	int Compare ( CConsolidatedIntByInt& source, int nHint = 0 );
	void Add	( CConsolidatedIntByInt& source );
	
public:
	int m_nKey;
	int m_nVal;
};

/**********************************************************************/

struct CConsolidatedDoubleByInt
{
public:
	CConsolidatedDoubleByInt();
	void Reset();

public:
	int Compare ( CConsolidatedDoubleByInt& source, int nHint = 0 );
	void Add	( CConsolidatedDoubleByInt& source );
	
public:
	int m_nKey;
	double m_dVal;
};

/**********************************************************************/

struct CConsolidatedDoubleByInt64
{
public:
	CConsolidatedDoubleByInt64();
	void Reset();
	
public:
	int Compare ( CConsolidatedDoubleByInt64& source, int nHint = 0 );
	void Add	( CConsolidatedDoubleByInt64& source );
	
public:
	__int64 m_nKey;
	double m_dVal;
};

/**********************************************************************/

struct CConsolidatedDoubleByString
{
public:
	CConsolidatedDoubleByString();
	void Reset();
	
public:
	int Compare ( CConsolidatedDoubleByString& source, int nHint = 0 );
	void Add	( CConsolidatedDoubleByString& source );
	
public:
	CString m_strKey;
	double m_dVal;
};

/**********************************************************************/

struct CSortedIntAndInt
{
public:
	CSortedIntAndInt();
	void Reset();

public:
	int Compare ( CSortedIntAndInt& source, int nHint = 0 );
	void Add	( CSortedIntAndInt& source );
	
public:
	int m_nInt1;
	int m_nInt2;
};

/**********************************************************************/

struct CSortedIntWithFlag
{
public:
	CSortedIntWithFlag();
	void Reset();

public:
	int Compare ( CSortedIntWithFlag& source, int nHint = 0 );
	void Add	( CSortedIntWithFlag& source );
	
public:
	int m_nInt;
	int m_nFlag;
};

/**********************************************************************/

struct CSortedPasswordLine
{
public:
	CSortedPasswordLine();
	void Reset();

public:
	int Compare(CSortedPasswordLine& source, int nHint = 0);
	void Add(CSortedPasswordLine& source);

public:
	int m_nSortPasswordNo;
	CString m_strDate;
	CString m_strTime;
	__int64 m_nLineNo;
	int m_nReportPasswordNo;
	CString m_strUserName;
	int m_nEventType;
	CString m_strDate2;
	CString m_strTime2;
};

/**********************************************************************/

struct CSortedDbReportItem
{
public:
	CSortedDbReportItem();
	void Reset();

public:
	int Compare(CSortedDbReportItem& source, int nHint = 0);
	void Add(CSortedDbReportItem& source);

public:
	__int64 m_nUserID;
	CString m_strSortField;
	int m_nGroupNo;

public:
	int m_nQty;
	double m_dVal;
};

/**********************************************************************/

struct CSortedDbReportRankingItem
{
public:
	CSortedDbReportRankingItem();
	void Reset();

public:
	int Compare(CSortedDbReportRankingItem& source, int nHint = 0);
	void Add(CSortedDbReportRankingItem& source);

public:
	int m_nRanking;
	__int64 m_nUserID;
	CString m_strSortField;
	int m_nGroupNo;

public:
	int m_nConsolIndex;
};

/**********************************************************************/

struct CConsolidatedDoubleAndStringByInt
{
public:
	CConsolidatedDoubleAndStringByInt();
	void Reset();

public:
	int Compare(CConsolidatedDoubleAndStringByInt& source, int nHint = 0);
	void Add(CConsolidatedDoubleAndStringByInt& source);

public:
	int m_nKey;
	double m_dVal;
	CString m_strVal;
};

/**********************************************************************/

struct CReportStatementAccountInfo
{
public:
	CReportStatementAccountInfo();
	void Reset();

public:
	int Compare(CReportStatementAccountInfo& source, int nHint = 0);
	void Add(CReportStatementAccountInfo& source);

public:
	__int64 m_nUserID;
	int m_nFilePos;
};

//**********************************************************************

struct CEPosPaymentGroupSale
{
public:
	CEPosPaymentGroupSale();
	void Reset();

public:
	int Compare(CEPosPaymentGroupSale& source, int nHint = 0);
	void Add(CEPosPaymentGroupSale& source);

public:
	int m_nGroupNo;
	CString m_strGroupText;
	double m_dPurse1;
	double m_dPurse2;
	double m_dPurse3;
	double m_dOther;
};

/**********************************************************************/

struct CPurchaseControlRSPBuffer
{
public:
	CPurchaseControlRSPBuffer();
	void Reset();

public:
	int Compare(CPurchaseControlRSPBuffer& source, int nHint = 0);
	void Add(CPurchaseControlRSPBuffer& source);

public:
	int m_nRuleID;
	int m_nStampCount;
	int m_nThreshold;
	CString m_strRuleName;
};

/**********************************************************************/

struct CStartingBalanceSortItem
{
public:
	CStartingBalanceSortItem();
	void Reset();

public:
	int Compare(CStartingBalanceSortItem& source, int nHint = 0);
	void Add(CStartingBalanceSortItem& source);

public:
	int m_nFilePos;
	__int64 m_nUserID;
	int m_nGroupNo;
};

/**********************************************************************/

struct CPluRankingReportItem
{
public:
	CPluRankingReportItem();
	void Reset();

public:
	int Compare(CPluRankingReportItem& source, int nHint = 0);
	void Add(CPluRankingReportItem& source);

public:
	__int64 m_nPluNo;
	CString m_strUserID;
	double m_dQty;
	double m_dValue;
	double m_dPoints;
};

/**********************************************************************/

struct CPluRankingUserSort
{
public:
	CPluRankingUserSort();
	void Reset();

public:
	int Compare(CPluRankingUserSort& source, int nHint = 0);
	void Add(CPluRankingUserSort& source);

public:
	CString m_strInfoLabel;
	CString m_strRankLabel;
	int m_nIndex;
};

/**********************************************************************/

struct CTidyUpFileInfo
{
public:
	CTidyUpFileInfo();
	void Reset();

public:
	int Compare(CTidyUpFileInfo& source, int nHint = 0);
	void Add(CTidyUpFileInfo& source);

public:
	CString m_strFilename;
	CString m_strModTime;
	LONGLONG m_FirstSeenTime;
};

/**********************************************************************/

struct CSortedInt64AndStringItem
{
public:
	CSortedInt64AndStringItem();
	void Reset();

public:
	int Compare(CSortedInt64AndStringItem& source, int nHint = 0);
	void Add(CSortedInt64AndStringItem& source);

public:
	__int64 m_nItem;
	CString m_strItem;
};

/**********************************************************************/
