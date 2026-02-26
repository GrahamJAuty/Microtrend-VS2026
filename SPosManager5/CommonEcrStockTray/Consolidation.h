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

struct CSortedInt64AndStringItem
{
public:
	CSortedInt64AndStringItem();
	void Reset();

public:
	int Compare ( CSortedInt64AndStringItem& source, int nHint = 0 );
	void Add	( CSortedInt64AndStringItem& source );
	
public:
	__int64 m_nItem;
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

struct CSortedIntAndStringByInt
{
public:
	CSortedIntAndStringByInt();
	void Reset();

public:
	int Compare ( CSortedIntAndStringByInt& source, int nHint = 0 );
	void Add	( CSortedIntAndStringByInt& source );
	
public:
	int m_nItem;
	int m_nData;
	CString m_strData;
};

/**********************************************************************/

struct CSortedIntAndStringByInt64
{
public:
	CSortedIntAndStringByInt64();
	void Reset();

public:
	int Compare(CSortedIntAndStringByInt64& source, int nHint = 0);
	void Add(CSortedIntAndStringByInt64& source);

public:
	__int64 m_nItem;
	int m_nData;
	CString m_strData;
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

struct CSortedImageInfo
{
public:
	CSortedImageInfo();
	void Reset();

public:
	int Compare ( CSortedImageInfo& source, int nHint = 0 );
	void Add	( CSortedImageInfo& source );
	
public:
	CString m_strFileName;
	CString m_strFilePath;
	bool m_bImageOK;
};

/**********************************************************************/

struct CSPOSKeyboardSortedString
{
public:
	CSPOSKeyboardSortedString();
	void Reset();

public:
	int Compare ( CSPOSKeyboardSortedString& source, int nHint = 0 );
	void Add	( CSPOSKeyboardSortedString& source );
	
public:
	CString m_strItem;
};

/**********************************************************************/

struct CCustomerTrackTraceInfo
{
public:
	CCustomerTrackTraceInfo();
	void Reset();

public:
	int Compare ( CCustomerTrackTraceInfo& source, int nHint = 0 );
	void Add	( CCustomerTrackTraceInfo& source );
	
public:
	int m_nCustID;
	int m_nCustIndex;
	int m_nStartMinute;
	bool m_bGotEndMinute;
	int m_nEndMinute;
};

/**********************************************************************/

struct CSortedTimeplanEntry
{
public:
	CSortedTimeplanEntry();
	void Reset();

public:
	int Compare(CSortedTimeplanEntry& source, int nHint = 0);
	void Add(CSortedTimeplanEntry& source);

public:
	int m_nPromoNo;
	int m_nPlanNo;
	int m_nTemplate;
	int m_nLineNo;
	int m_nEntryNo;
};

/**********************************************************************/