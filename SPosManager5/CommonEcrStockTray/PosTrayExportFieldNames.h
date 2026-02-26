#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "ReportConsolidationArray.h"
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_EMPTY 0
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_DATE_CAL 1
#define POSTRAY_EXPORTFIELD_DATE_BUS 2
#define POSTRAY_EXPORTFIELD_DATE_WEEK 3
#define POSTRAY_EXPORTFIELD_YEARNO 4
#define POSTRAY_EXPORTFIELD_MONTHNO 5
#define POSTRAY_EXPORTFIELD_WEEKNO 6
#define POSTRAY_EXPORTFIELD_WEEKDAYNO 7
#define POSTRAY_EXPORTFIELD_TIME_TRAN 8
#define POSTRAY_EXPORTFIELD_TIME_SALE 9
#define POSTRAY_EXPORTFIELD_TIME_VOID 10
#define POSTRAY_EXPORTFIELD_TIME_DISCOUNT 11
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_MONTHNAME 101
#define POSTRAY_EXPORTFIELD_WEEKDAYNAME 102
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_DBNO 201
#define POSTRAY_EXPORTFIELD_LOCNO 202
#define POSTRAY_EXPORTFIELD_RPNO 203
#define POSTRAY_EXPORTFIELD_TRMNO 204
#define POSTRAY_EXPORTFIELD_TBLGRPNO 205
#define POSTRAY_EXPORTFIELD_TBLNO 206
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_DBNAME 301
#define POSTRAY_EXPORTFIELD_LOCNAME 302
#define POSTRAY_EXPORTFIELD_RPNAME 303
#define POSTRAY_EXPORTFIELD_TBLGRPNAME 304
#define POSTRAY_EXPORTFIELD_TBLNAME 305
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_PLUNO 401
#define POSTRAY_EXPORTFIELD_MODNO 402
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_REPTEXT_BASE 501
#define POSTRAY_EXPORTFIELD_REPTEXT_MOD 502
#define POSTRAY_EXPORTFIELD_MODNAME 503
#define POSTRAY_EXPORTFIELD_SUPPREF 504
#define POSTRAY_EXPORTFIELD_SORTCODE 505
#define POSTRAY_EXPORTFIELD_STOCKCODE 506
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_DPTNO_DBASE 601
#define POSTRAY_EXPORTFIELD_DPTNO_SALE 602
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_DPTNAME_DBASE 701
#define POSTRAY_EXPORTFIELD_DPTNAME_SALE 702
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_GRPNO_DBASE 801
#define POSTRAY_EXPORTFIELD_GRPNO_SALE 802
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_GRPNAME_DBASE 901
#define POSTRAY_EXPORTFIELD_GRPNAME_SALE 902
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_ACATNO_DBASE 1001
#define POSTRAY_EXPORTFIELD_ACATNO_SALE 1002
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_ACATNAME_DBASE 1101
#define POSTRAY_EXPORTFIELD_ACATNAME_SALE 1102
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_SERVER_TRAN 1201
#define POSTRAY_EXPORTFIELD_SERVER_ITEM 1202
#define POSTRAY_EXPORTFIELD_SERVER_AUTH 1203
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_SRVNAME_TRAN 1301
#define POSTRAY_EXPORTFIELD_SRVNAME_ITEM 1302
#define POSTRAY_EXPORTFIELD_SRVNAME_AUTH 1303
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_TAXNO 1401
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_TAXNAME 1501
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_PBANDNO 1601 
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_PBANDNAME 1701
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_QTY_BASE 1800
#define POSTRAY_EXPORTFIELD_QTY_MOD 1801
#define POSTRAY_EXPORTFIELD_VALUE_FULL 1802
#define POSTRAY_EXPORTFIELD_DISCOUNTPREMIUM 1803
#define POSTRAY_EXPORTFIELD_VALUE_DISC 1804
#define POSTRAY_EXPORTFIELD_TAX 1805
#define POSTRAY_EXPORTFIELD_VALUE_NET 1806
#define POSTRAY_EXPORTFIELD_COST 1807
#define POSTRAY_EXPORTFIELD_WASTE_QTY_BASE 1808
#define POSTRAY_EXPORTFIELD_WASTE_QTY_MOD 1809
#define POSTRAY_EXPORTFIELD_WASTE_COST 1810
#define POSTRAY_EXPORTFIELD_VALUE_VOID 1811
#define POSTRAY_EXPORTFIELD_LINE_COUNT 1812
#define POSTRAY_EXPORTFIELD_QTY_COVERS 1813
#define POSTRAY_EXPORTFIELD_DISCOUNTONLY 1814
#define POSTRAY_EXPORTFIELD_PREMIUMONLY 1815
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_QTY_PAY 1900
#define POSTRAY_EXPORTFIELD_VALUE_TENDER 1901
#define POSTRAY_EXPORTFIELD_VALUE_AMOUNT 1902
#define POSTRAY_EXPORTFIELD_VALUE_GRATUITY 1903
#define POSTRAY_EXPORTFIELD_VALUE_CASHBACK 1904
#define POSTRAY_EXPORTFIELD_VALUE_SURPLUS 1905
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_PAYNO 2001
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_PAYNAME 2101
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_VOID_SEQNO 2201
#define POSTRAY_EXPORTFIELD_VOID_TABLENO 2202
#define POSTRAY_EXPORTFIELD_VOID_LINE_TYPE 2203
#define POSTRAY_EXPORTFIELD_VOID_BEFOREAFTER 2204
#define POSTRAY_EXPORTFIELD_VOID_REASON 2205
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_DISCOUNT_LINE_TYPE 2301
#define POSTRAY_EXPORTFIELD_DISCOUNT_MODE 2302
#define POSTRAY_EXPORTFIELD_DISCOUNT_USAGE 2303
#define POSTRAY_EXPORTFIELD_DISCOUNT_TYPE 2304
#define POSTRAY_EXPORTFIELD_DISCOUNT_RATE 2305
#define POSTRAY_EXPORTFIELD_PROMONO 2306
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_PROMONAME 2401
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_TRAN_SEQNO 2501
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_S4LABOUR_REV 2601
#define POSTRAY_EXPORTFIELD_S4LABOUR_HOUR 2602
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_MIXTYPENO 2701
#define POSTRAY_EXPORTFIELD_MIXMATCHNO 2702
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_MIXTYPENAME 2801
#define POSTRAY_EXPORTFIELD_MIXMATCHNAME 2802
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_GRPNAME_DBASE 901
#define POSTRAY_EXPORTFIELD_GRPNAME_SALE 902
/**********************************************************************/
#define POSTRAY_EXPORTFIELD_DUMMY 99999
/**********************************************************************/

struct CPosTrayExportFieldName
{
public:
	CPosTrayExportFieldName();
	void Reset();

public:
	int Compare ( CPosTrayExportFieldName& source, int nHint = 0 );
	void Add	( CPosTrayExportFieldName& source );

public:
	const char* GetSingleColumnName();
	const char* GetHeaderNameForExport();
	
public:
	int m_nFieldType;
	
public:
	CString m_strDisplayName1;
	CString m_strDisplayName2;
	CString m_strHeaderNameDefault;
	CString m_strHeaderNameUser;

private:
	CString m_strSingleColumnName;
	CString m_strHeaderNameForExport;
};

/**********************************************************************/

class CPosTrayExportFieldNames : public CSharedStringArray
{
public:
	CPosTrayExportFieldNames();

public:
	void GetNameInfo( CPosTrayExportFieldName& Name );
	void SetNameInfo( CPosTrayExportFieldName& Name );

public:
	int Open( const char* szFilename, int nMode );
	void PrepareForWrite();
	
public:
	void FixTaxNames();

private:
	void AddField( int nFieldType, const char* szDisplayName1, const char* szDisplayName2, const char* szHeaderName );
	
private:
	CReportConsolidationArray<CPosTrayExportFieldName> m_arrayNames;
	bool m_bFixedTaxNames;
};

/**********************************************************************/
