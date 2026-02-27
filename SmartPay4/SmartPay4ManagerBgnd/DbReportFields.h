//$$******************************************************************
#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
//$$******************************************************************
#define DBREPORTFIELD_USERID 0
#define DBREPORTFIELD_USERNAME 1
#define DBREPORTFIELD_FORENAME 2
#define DBREPORTFIELD_SURNAME 3
#define DBREPORTFIELD_GROUPNO 4
#define DBREPORTFIELD_DOB 5
#define DBREPORTFIELD_GENDER 6
#define DBREPORTFIELD_EXPIRY 7
#define DBREPORTFIELD_DAYGROUPS 8
#define DBREPORTFIELD_ALERTCODE 9
#define DBREPORTFIELD_ALERTDATE 10
#define DBREPORTFIELD_ALERTTIME 11
#define DBREPORTFIELD_INACTIVE 12
#define DBREPORTFIELD_ALLERGYCODES 13
#define DBREPORTFIELD_DIETARY 14
#define DBREPORTFIELD_OWNMAXSPEND 15
#define DBREPORTFIELD_P1ALLOW 16
#define DBREPORTFIELD_P2ALLOW 17
#define DBREPORTFIELD_P3ALLOW 18
#define DBREPORTFIELD_P1MAXSPEND1 19
#define DBREPORTFIELD_P1MAXSPEND2 20
#define DBREPORTFIELD_P1MAXSPEND3 21
#define DBREPORTFIELD_P1MAXSPEND4 22
#define DBREPORTFIELD_P1MAXSPEND5 23
#define DBREPORTFIELD_P1MAXSPEND6 24
#define DBREPORTFIELD_P1OVERSPEND1 25
#define DBREPORTFIELD_P1OVERSPEND2 26
#define DBREPORTFIELD_P1OVERSPEND3 27
#define DBREPORTFIELD_P1OVERSPEND4 28
#define DBREPORTFIELD_P1OVERSPEND5 29
#define DBREPORTFIELD_P1OVERSPEND6 30
#define DBREPORTFIELD_P3MAXSPEND1 31
#define DBREPORTFIELD_P3MAXSPEND2 32
#define DBREPORTFIELD_P3MAXSPEND3 33
#define DBREPORTFIELD_P3MAXSPEND4 34
#define DBREPORTFIELD_P3MAXSPEND5 35
#define DBREPORTFIELD_P3MAXSPEND6 36
#define DBREPORTFIELD_P3OVERSPEND1 37
#define DBREPORTFIELD_P3OVERSPEND2 38
#define DBREPORTFIELD_P3OVERSPEND3 39
#define DBREPORTFIELD_P3OVERSPEND4 40
#define DBREPORTFIELD_P3OVERSPEND5 41
#define DBREPORTFIELD_P3OVERSPEND6 42
#define DBREPORTFIELD_P1LIABILITY 43
#define DBREPORTFIELD_P1CREDIT 44
#define DBREPORTFIELD_P1DATE 45
#define DBREPORTFIELD_P1TIME 46
#define DBREPORTFIELD_P1LASTSPEND 47
#define DBREPORTFIELD_P1SPENDTD 48
#define DBREPORTFIELD_P2BALANCE 49
#define DBREPORTFIELD_P2DATE 50
#define DBREPORTFIELD_P2TIME 51
#define DBREPORTFIELD_P2LASTSPEND 52
#define DBREPORTFIELD_P2SPENDTD 53
#define DBREPORTFIELD_REFRESHREF 54
#define DBREPORTFIELD_REFRESHDATE 55
#define DBREPORTFIELD_REFRESHTIME 56
#define DBREPORTFIELD_P3LIABILITY 57
#define DBREPORTFIELD_P3CREDIT 58
#define DBREPORTFIELD_P3DATE 59
#define DBREPORTFIELD_P3TIME 60
#define DBREPORTFIELD_P3LASTSPEND 61
#define DBREPORTFIELD_P3SPENDTD 62
#define DBREPORTFIELD_POINTS 63
#define DBREPORTFIELD_POINTSTD 64
#define DBREPORTFIELD_CASHTD 65
#define DBREPORTFIELD_MEMBERID 66
#define DBREPORTFIELD_EMAIL 67
#define DBREPORTFIELD_INFO1 68
#define DBREPORTFIELD_INFO2 69
#define DBREPORTFIELD_INFO3 70
#define DBREPORTFIELD_INFO4 71
#define DBREPORTFIELD_INFO5 72
#define DBREPORTFIELD_INFO6 73
#define DBREPORTFIELD_INFO7 74
#define DBREPORTFIELD_INFO8 75
#define DBREPORTFIELD_TICK1 76
#define DBREPORTFIELD_TICK2 77
#define DBREPORTFIELD_TICK3 78
#define DBREPORTFIELD_TICK4 79
#define DBREPORTFIELD_TOTALSPENDTD 80
#define DBREPORTFIELD_DOB_NEXTAGE 81
#define DBREPORTFIELD_DOB_BDAY 82
#define DBREPORTFIELD_ALERTTEXT 83
#define DBREPORTFIELD_ALLERGYTEXT 84
#define DBREPORTFIELD_EXTREF 85
#define DBREPORTFIELD_EXTREF2 86
#define DBREPORTFIELD_DELETEDDATE 87
#define DBREPORTFIELD_DELETEDTIME 88
#define DBREPORTFIELD_ACCURANK 89
#define DBREPORTFIELD_ACCUPOINTS 90
#define DBREPORTFIELD_ACCUTRANS 91
#define DBREPORTFIELD_ACCUSPEND 92
#define DBREPORTFIELD_STARTBAL_LASTUSED 93
#define DBREPORTFIELD_STARTBAL_NEXTUSED 94
#define DBREPORTFIELD_STARTBAL_SOURCE 95
#define DBREPORTFIELD_STARTBAL_POINTS 96
#define DBREPORTFIELD_STARTBAL_PURSE1 97
#define DBREPORTFIELD_STARTBAL_PURSE3 98
#define DBREPORTFIELD_BLANK 99
#define DBREPORTFIELD_GROUPSHIFT_STATUS 100
#define DBREPORTFIELD_GROUPSHIFT_STARTDATE 101
#define DBREPORTFIELD_GROUPSHIFT_ENDDATE 102
#define DBREPORTFIELD_GROUPSHIFT_FUTUREGROUP 103
#define DBREPORTFIELD_EMPTYFIELD 104
#define DBREPORTFIELD_PIN 105
//$$******************************************************************
#define DBREPORTFIELD_COLTYPE_ORDERBY 0
#define DBREPORTFIELD_COLTYPE_NORMAL 1
#define DBREPORTFIELD_COLTYPE_SPECIAL 2
//$$******************************************************************

namespace DbReportFieldsSpecial
{
	const CString AccuRank = { "AccuRank" };
	const CString AccuPoints = { "AccuPoints" };
	const CString AccuTrans = { "AccuTrans" };
	const CString AccuSpend = { "AccuSpend" };
	const CString StartBalLastUsed = { "StartBalLastUsed" };
	const CString StartBalNextUsed = { "StartBalNextUsed" };
	const CString StartBalSource = { "StartBalSource" };
	const CString StartBalPoints = { "StartBalPoints" };
	const CString StartBalPurse1 = { "StartBalPurse1" };
	const CString StartBalPurse3 = { "StartBalPurse3" };
};

//$$******************************************************************

struct CDbReportFieldInfo
{
public:
	CDbReportFieldInfo();
	void Reset();

public:
	int Compare(CDbReportFieldInfo& source, int nHint = 0);
	void Add(CDbReportFieldInfo& source);

public:
	CString m_strLabel;
	CString m_strHeaderText;
	int m_nColumnType;
	int m_nFieldType;
	int m_nAlign;
	int m_nWidth;
	int m_nColumnPos;
};

/**********************************************************************/

class CDbReportFields
{
public:
	CDbReportFields();

public:
	void ResetColumns();
	void ResetColumnsFromUserText();
	bool AddColumn(CString strLabel, int nColumnType);
	int GetColumnCount() { return m_arrayColumns.GetSize(); }
	int GetColType(int n);
	int GetFieldType(int n);

public:
	void SetHeaderText(CString strLabel, CString strText);
	void CreateReportHeader(CReportFileWithEndText* pReport);
	void CreateColumnListFromHeader(CString strHeader);

private:
	void AddField(CString strLabel,int nFieldType, CString strHeaderText, int nAlign, int nWidth);

private:
	CReportConsolidationArray<CDbReportFieldInfo> m_arrayFields;
	CArray<CDbReportFieldInfo,CDbReportFieldInfo> m_arrayColumns;
};

//$$******************************************************************
extern CDbReportFields DbReportFields;
//$$******************************************************************
